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
	// vertex variables
	"layout(location = 0) in vec3 vertexPosition;\n"
	"layout(location = 1) in vec4 vertexAmbientColor;\n"
	// decide if mesh ambient color or vertex ambient color is used.
	"uniform int inputFragmentAmbientColorType = 0;\n"
	"uniform vec4 meshAmbientColor = vec4(1.0, 0.8, 0.2, 1.0);\n"
	"out vec4 inputFragmentAmbientColor;\n"
	"uniform mat4 projection = mat4(1);\n"
	"uniform mat4 view = mat4(1);\n"
	"uniform mat4 world = mat4(1);\n"
	"void main() {\n"
	"  mat4 worldProjection = projection * view * world;"
	"  gl_Position = worldProjection * vec4(vertexPosition, 1.);\n"
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

static GLint g_vertexPositionIndex = 0;
static GLint g_vertexAmbientColorIndex = 1;

static GLuint g_programId = 0;
static World* g_world = NULL;

static void
bindMatrix4Uniform
	(
		Zeitgeist_State* state,
		GLuint programID,
		char const* name,
		Matrix4R32* value
	)
{
	GLint location = glGetUniformLocation(g_programId, name);
	if (-1 == location) {
		fprintf(stderr, "%s:%d: unable to get uniform location of uniform `%s`\n", __FILE__, __LINE__, name);
	} else {
		glUniformMatrix4fv(location, 1, GL_TRUE, idlib_matrix_4x4_f32_get_data(&value->m));
	}
}

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

	//
	Matrix4R32* world = NULL;
	world = Matrix4R32_createScale(state, Vector3R32_create(state, 0.75f, 0.75f, 1.f));
	bindMatrix4Uniform(state, g_programId, "world", world);
	//
	Matrix4R32* viewTranslate = NULL;
	viewTranslate = Matrix4R32_createTranslate(state, Vector3R32_create(state, -g_world->player->position->v.e[0], -g_world->player->position->v.e[1], -g_world->player->position->v.e[2]));
	Matrix4R32* viewRotateY = NULL;
	viewRotateY = Matrix4R32_createRotateY(state, g_world->player->rotationY);
	Matrix4R32* view = NULL;
	view = Matrix4R32_multiply(state, viewTranslate, viewRotateY);
	bindMatrix4Uniform(state, g_programId, "view", view);
	
	//
	Matrix4R32* projection = NULL;
	//projection = Matrix4R32_createOrthographic(state, -1.f, +1.f, -1.f, +1.f, -100.f, +100.f);
	projection = Matrix4R32_createPerspective(state, 90.f, viewportHeight > 0.f ? viewportWidth / viewportHeight : 16.f/9.f, 0.1f, 100.f);
	bindMatrix4Uniform(state, g_programId, "projection", projection);
	
	GLint location;

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

	Zeitgeist_Value sizeValue = Zeitgeist_List_getSize(state, g_world->geometries);
	for (Zeitgeist_Integer i = 0, n = Zeitgeist_Value_getInteger(&sizeValue); i < n; ++i) {
		Zeitgeist_Value elementValue = Zeitgeist_List_getValue(state, g_world->geometries, i);
		StaticGeometryGl *element = (StaticGeometryGl*)Zeitgeist_Value_getForeignObject(&elementValue);
		glBindVertexArray(element->vertexArrayId);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, element->numberOfVertices);
	}

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
			case KeyboardKey_Q: {
				g_world->player->rotationY -= 0.25f;
			} break;
			case KeyboardKey_E: {
				g_world->player->rotationY += 0.25f;
			} break;
			case KeyboardKey_W:
			case KeyboardKey_Up: {
				g_world->player->position = Vector3R32_add(state, g_world->player->position, Vector3R32_create(state, 0.f, 0.f, -0.25f));
			} break;
			case KeyboardKey_S:
			case KeyboardKey_Down: {
				g_world->player->position = Vector3R32_add(state, g_world->player->position, Vector3R32_create(state, 0.f, 0.f, +0.25f));
			} break;
			case KeyboardKey_A:
			case KeyboardKey_Left: {
				g_world->player->position = Vector3R32_add(state, g_world->player->position, Vector3R32_create(state, -0.25f, 0.f, 0.f));
			} break;
			case KeyboardKey_D:
			case KeyboardKey_Right: {
				g_world->player->position = Vector3R32_add(state, g_world->player->position, Vector3R32_create(state, +0.25f, 0.f, 0.f));
			} break;
		};
	}
	Zeitgeist_Stack_pop(state);
	Zeitgeist_Stack_pop(state);
}

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
		g_world = World_create(state);
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
		Zeitgeist_lock(state, (Zeitgeist_Gc_Object*)g_world);
		Zeitgeist_State_popJumpTarget(state);
	} else {
		Zeitgeist_State_popJumpTarget(state);
		g_world = NULL;
		glDeleteProgram(g_programId);
		g_programId = 0;
		longjmp(state->jumpTarget->environment, -1);
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);

	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.f);

	fprintf(stdout, "OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, "OpenGL vendor:   %s\n", glGetString(GL_VENDOR));
	fprintf(stdout, "OpenGL version:  %s\n", glGetString(GL_VERSION));
}

Zeitgeist_Rendition_Export void
Zeitgeist_Rendition_unload
	(
		Zeitgeist_State* state
	)
{
	Zeitgeist_Value sizeValue = Zeitgeist_List_getSize(state, g_world->geometries);
	for (Zeitgeist_Integer i = 0, n = Zeitgeist_Value_getInteger(&sizeValue); i < n; ++i) {
		Zeitgeist_Value elementValue = Zeitgeist_List_getValue(state, g_world->geometries, i);
		StaticGeometryGl* element = (StaticGeometryGl*)Zeitgeist_Value_getForeignObject(&elementValue);
		StaticGeometryGl_unmaterialize(state, element);
	}
	Zeitgeist_unlock(state, (Zeitgeist_Gc_Object*)g_world);
	g_world = NULL;
	glDeleteProgram(g_programId);
	g_programId = 0;
	ServiceGl_shutdown(state);
}
