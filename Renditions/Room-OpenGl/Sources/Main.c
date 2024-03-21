// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist.h"

#include "Zeitgeist/Locks.h"
#include "Zeitgeist/Object.h"
#include "Zeitgeist/Value.h"
#include "Zeitgeist/UpstreamRequests.h"

// strlen
#include <string.h>

// fprintf, stdout
#include <stdio.h>

#include "KeyboardKeyMessage.h"
#include "ServiceGl.h"
#include "World.h"

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	#define Zeitgeist_Rendition_Export _declspec(dllexport)
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#define Zeitgeist_Rendition_Export
#else
	#error("operating system not (yet) supported")
#endif

Zeitgeist_Rendition_Export Zeitgeist_String*
Zeitgeist_Rendition_getName
	(
		Zeitgeist_State* state
	)
{
	return Zeitgeist_State_createString(state, "Room (OpenGL)", strlen("Room (OpenGL)"));
}

// Vertex shader.
// constant inputs:
//	meshAmbientColor vec4: The ambient color of the mesh in RGBF32.
//  inputFragmentAmbientColorType : Determine if the mesh ambient color or the vertex ambient color
//                                  is used as the input fragment color. This can assume two values:
//                                  0 (default and fallback, the mesh ambient color is used)
//                                  1 (the vertex ambient color is used)
// variable inputs:
//	vertexPosition vec3: The vertex position in model coordinates.
//  vertexAmbientColor vec4: The vertex ambient color in RGBF32.
static const GLchar* g_vertexShader =
	"#version 330 core\n"
	"layout(location = 0) in vec3 vertexPosition;\n"
	"layout(location = 1) in vec4 vertexAmbientColor;\n"
	"uniform int inputFragmentAmbientColorType = 0;\n"
	"uniform vec4 meshAmbientColor = vec4(1.0, 0.8, 0.2, 1.0);\n"
	"out vec4 inputFragmentAmbientColor;\n"
	"uniform mat4 scale = mat4(1);\n"
	"void main() {\n"
	"  gl_Position = scale * vec4(vertexPosition.xyz, 1.0);\n"
	"  if(inputFragmentAmbientColorType == 1) {\n"
	"    inputFragmentAmbientColor = vec4(vertexAmbientColor.rgb, vertexAmbientColor.a);\n"
	"  } else {\n"
	"    inputFragmentAmbientColor = vec4(meshAmbientColor.rgb, meshAmbientColor.a);\n"
	"  }\n"
	"}\n"
	;

// Fragment shader.
// The color (255, 204, 51) is the websafe color "sunglow".
static const GLchar* g_fragmentShader =
	"#version 330 core\n"
	"out vec4 outputFragmentColor;\n"
	"in vec4 inputFragmentAmbientColor;\n"
	"void main() {\n"
	"  outputFragmentColor = inputFragmentAmbientColor;\n"
	"}\n"
	;

typedef struct Player {
	struct {
		/** Rotation around the y-axis. */
		float y;
	} rotation;
	/** @brief Position in world coordinates. */
	struct {
		float x, y, z;
	} position;
} Player;

static Player g_player = {
	.rotation = { .y = 0 },
	.position = { .x = 0, .y = 0, .z = 0 },
};

static GLint g_vertexPositionIndex = 0;
static GLint g_vertexAmbientColorIndex = 1;

typedef struct VERTEX {
	struct {
		float x, y, z;
	} position;
	struct {
		float r, g, b, a;
	} ambientColor;
} VERTEX;

static VERTEX const FRONT[] = {
	{.position = { -1.0f,  1.0f, -1.f, }, .ambientColor = { 1.f, 0.f, 0.f, 1.f }, },
	{.position = { -1.0f, -1.0f, -1.f, }, .ambientColor = { 1.f, 1.f, 0.f, 1.f }, },
	{.position = {  1.0f,  1.0f, -1.f, }, .ambientColor = { 1.f, 0.f, 1.f, 1.f }, },
	{.position = {  1.0f, -1.0f, -1.f, }, .ambientColor = { 1.f, 1.f, 1.f, 1.f }, },
};

static GLuint g_programId = 0;
static StaticGeometryGl* g_buildingGeometry = NULL;

Zeitgeist_Rendition_Export void
Zeitgeist_Rendition_update
	(
		Zeitgeist_State* state
	)
{
	ServiceGl_update(state);

	if (ServiceGl_quitRequested(state)) {
		Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
		Zeitgeist_sendUpstreamRequest(state, request);
	}

	Zeitgeist_Integer viewportWidth, viewportHeight;
	ServiceGl_getClientSize(state, &viewportWidth, &viewportHeight);
	ServiceGl_beginFrame(state);

	glViewport(0, 0, viewportWidth, viewportHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(g_programId);

	GLint location;

	location = glGetUniformLocation(g_programId, "scale");
	if (-1 == location) {
		fprintf(stderr, "%s:%d: unable to get uniform location of `%s`\n", __FILE__, __LINE__, "scale");
	} else {
		GLfloat matrix[16] = {
			.75f, 0.f,  0.f, 0.f,
			0.f,  .75f, 0.f, 0.f,
			0.f,  0.f,  1.f, 0.f,
			0.f,  0.f,  0.f, 1.f,
		};
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0]);
	}

	location = glGetUniformLocation(g_programId, "meshAmbientColor");
	if (-1 == location) {
		fprintf(stderr, "%s:%d: unable to get uniform location of `%s`\n", __FILE__, __LINE__, "meshAmbientColor");
	} else {
		// The color (255, 204, 51) is the websafe color "sunglow".
		GLfloat vector[4] = { 1.0, 0.8, 0.2, 1.0 };
		glUniform4fv(location, 1, &vector[0]);
	}

	location = glGetUniformLocation(g_programId, "inputFragmentAmbientColorType");
	if (-1 == location) {
		fprintf(stderr, "%s:%d: unable to get uniform location of `%s`\n", __FILE__, __LINE__, "inputFragmentAmbientColorType");
	} else {
		GLint scalar = 1;
		glUniform1i(location, scalar);
	}

	location = glGetUniformLocation(g_programId, "meshAmbientColor");
	if (-1 == location) {
		fprintf(stderr, "%s:%d: unable to get uniform location of `%s`\n", __FILE__, __LINE__, "meshAmbientColor");
	} else {
		// The color (255, 204, 51) is the websafe color "sunglow".
		GLfloat vector[4] = { 1.0, 0.8, 0.2, 1.0 };
		glUniform4fv(location, 1, &vector[0]);
	}

	glBindVertexArray(g_buildingGeometry->vertexArrayId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(FRONT) / sizeof(VERTEX));
	glBindVertexArray(0);
	glUseProgram(0);

	ServiceGl_endFrame(state);
}

static void
onKeyboardKeyMessage
(
	Zeitgeist_State* state
) {
	if (Zeitgeist_Stack_getSize(state) < 2) {
		fprintf(stderr, "%s:%d: too few arguments\n", __FILE__, __LINE__);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	if (!Zeitgeist_Stack_isInteger(state, 0)) {
		fprintf(stderr, "%s:%d: invalid argument type\n", __FILE__, __LINE__);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	if (1 != Zeitgeist_Stack_getInteger(state, 0)) {
		fprintf(stderr, "%s:%d: invalid number of arguments\n", __FILE__, __LINE__);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	if (!Zeitgeist_Stack_isForeignObject(state, 1)) {
		fprintf(stderr, "%s:%d: invalid argument type\n", __FILE__, __LINE__);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	fprintf(stdout, "%s:%d: keyboard key message received\n", __FILE__, __LINE__);
	KeyboardKeyMessage* message = (KeyboardKeyMessage*)Zeitgeist_Stack_getForeignObject(state, 1);
	if (KeyboardKey_Action_Released == KeyboardKeyMessage_getAction(state, message)) {
		switch (KeyboardKeyMessage_getKey(state, message)) {
			case KeyboardKey_Escape: {
				Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
				Zeitgeist_sendUpstreamRequest(state, request);
			} break;
			case KeyboardKey_Up: {
				//g_playerRotation -= 0.1f;
			} break;
			case KeyboardKey_Down: {
				//g_playerRotation += 0.1f;
			} break;
			case KeyboardKey_Left: {
				g_player.rotation.y -= 0.1f;
				// clamp rotation to [0, 360)
				if (g_player.rotation.y >= 360.f) {
					do {
						g_player.rotation.y -= 360.f;
					} while (g_player.rotation.y > 360.f);
				} else if (g_player.rotation.y < 0.f) {
					do {
						g_player.rotation.y += 360.f;
					} while (g_player.rotation.y < 0.f);
				}
			} break;
			case KeyboardKey_Right: {
				g_player.rotation.y += 0.1f;
				// clamp rotation to [0, 360)
				if (g_player.rotation.y >= 360.f) {
					do {
						g_player.rotation.y -= 360.f;
					} while (g_player.rotation.y > 360.f);
				} else if (g_player.rotation.y < 0.f) {
					do {
						g_player.rotation.y += 360.f;
					} while (g_player.rotation.y < 0.f);
				}
			} break;
		};
	}
	Zeitgeist_Stack_pop(state);
	Zeitgeist_Stack_pop(state);
}

#include "Zeitgeist/Locks.h"

Zeitgeist_Rendition_Export void
Zeitgeist_Rendition_load
	(
		Zeitgeist_State* state
	)
{
	ServiceGl_startup(state);
	ServiceGl_setTitle(state, Zeitgeist_State_createString(state, "Room (OpenGL)", strlen("Room (OpenGL)")));

	Zeitgeist_Value temporary;
	Zeitgeist_Value_setForeignFunction(&temporary, &onKeyboardKeyMessage);
	ServiceGl_addKeyboardKeyCallback(state, &temporary);

	GLuint vertexShaderId, fragmentShaderId;
	
	vertexShaderId = ServiceGl_compileShader(state, GL_VERTEX_SHADER, g_vertexShader);
	fragmentShaderId = ServiceGl_compileShader(state, GL_FRAGMENT_SHADER, g_fragmentShader);
	g_programId = ServiceGl_linkProgram(state, vertexShaderId, fragmentShaderId);
	glDeleteShader(fragmentShaderId);
	glDeleteShader(vertexShaderId);

	Zeitgeist_JumpTarget jumpTarget;
	//
	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		g_buildingGeometry = StaticGeometryGl_create(state);
		Zeitgeist_State_popJumpTarget(state);
	} else {
		Zeitgeist_State_popJumpTarget(state);
		glDeleteProgram(g_programId);
		g_programId = 0;
		longjmp(state->jumpTarget->environment, -1);
	}
	//
	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		Zeitgeist_lock(state, (Zeitgeist_Gc_Object*)g_buildingGeometry);
		Zeitgeist_State_popJumpTarget(state);
	} else {
		Zeitgeist_State_popJumpTarget(state);
		g_buildingGeometry = NULL;
		glDeleteProgram(g_programId);
		g_programId = 0;
		longjmp(state->jumpTarget->environment, -1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, g_buildingGeometry->bufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FRONT), FRONT, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(g_buildingGeometry->vertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, g_buildingGeometry->bufferId);

	glEnableVertexAttribArray(g_vertexPositionIndex);
	glVertexAttribPointer(g_vertexPositionIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX),
		                    (void*)(uintptr_t)offsetof(VERTEX, position));

	glEnableVertexAttribArray(g_vertexAmbientColorIndex);
	glVertexAttribPointer(g_vertexAmbientColorIndex, 4, GL_FLOAT, GL_TRUE, sizeof(VERTEX),
		                    (void*)(uintptr_t)offsetof(VERTEX, ambientColor));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glClearDepth(1.f);
}

Zeitgeist_Rendition_Export void
Zeitgeist_Rendition_unload
	(
		Zeitgeist_State* state
	)
{
	StaticGeometryGl_unmaterialize(state, g_buildingGeometry);
	Zeitgeist_unlock(state, (Zeitgeist_Gc_Object*)g_buildingGeometry);
	g_buildingGeometry = NULL;
	glDeleteProgram(g_programId);
	g_programId = 0;
	ServiceGl_shutdown(state);
}
