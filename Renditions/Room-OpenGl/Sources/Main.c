// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist.h"
#include "Zeitgeist/Value.h"
#include "Zeitgeist/Object.h"

#include "Zeitgeist/UpstreamRequests.h"

// strlen
#include <string.h>

// fprintf, stdout
#include <stdio.h>

#include "ServiceGl.h"

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
static const GLchar* g_vertexShader =
	"#version 330 core\n"
	"layout(location = 0) in vec3 vertexPosition;\n"
	"uniform mat4 scale = mat4(1);\n"
	"void main() {\n"
	"  gl_Position = scale * vec4(vertexPosition.xyz, 1.0);\n"
	"}\n"
	;

// Fragment shader.
// The color (255, 204, 51) is the websafe color "sunglow".
static const GLchar* g_fragmentShader =
	"#version 330 core\n"
	"out vec4 outputColor;\n"
	//"uniform vec4 inputColor = vec4(1.0, 1.0, 1.0, 0.0);\n"
	"uniform vec4 inputColor = vec4(1.0, 0.8, 0.2, 0.0);\n"
	"void main() {\n"
	"  outputColor = inputColor;\n"
	"}\n"
	;

static GLint Positions_Index = 0;

typedef struct VERTEX {
	float x, y, z;
} VERTEX;

static VERTEX const FRONT[] = {
	{ -1.0f,  1.0f, -1.f, },
	{ -1.0f, -1.0f, -1.f, },
	{  1.0f,  1.0f, -1.f, },
	{  1.0f, -1.0f, -1.f, },
};

static GLuint g_programId = 0;

typedef struct Mesh {
	GLuint bufferId;
	GLuint vertexArrayId;
} Mesh;

static Mesh g_frontMesh = { .bufferId = 0, .vertexArrayId = 0 };

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
	
	location = glGetUniformLocation(g_programId, "inputColor");
	if (-1 == location) {
		fprintf(stderr, "%s:%d: unable to get uniform location of `%s`\n", __FILE__, __LINE__, "inputColor");
	} else {
		// The color (255, 204, 51) is the websafe color "sunglow".
		GLfloat vector[4] = { 1.0, 0.8, 0.2, 1.0 };
		glUniform4fv(location, 1, &vector[0]);
	}

	glBindVertexArray(g_frontMesh.vertexArrayId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(FRONT)/ sizeof(VERTEX));
	glBindVertexArray(0);
	glUseProgram(0);

	ServiceGl_endFrame(state);
}

Zeitgeist_Rendition_Export void
Zeitgeist_Rendition_load
	(
		Zeitgeist_State* state
	)
{
	ServiceGl_startup(state);
	ServiceGl_setTitle(state, Zeitgeist_State_createString(state, "Room (OpenGL)", strlen("Room (OpenGL)")));

	GLuint vertexShaderId, fragmentShaderId;
	
	vertexShaderId = ServiceGl_compileShader(state, GL_VERTEX_SHADER, g_vertexShader);
	fragmentShaderId = ServiceGl_compileShader(state, GL_FRAGMENT_SHADER, g_fragmentShader);
	g_programId = ServiceGl_linkProgram(state, vertexShaderId, fragmentShaderId);
	glDeleteShader(fragmentShaderId);
	glDeleteShader(vertexShaderId);

	glGenBuffers(1, &g_frontMesh.bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, g_frontMesh.bufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FRONT), FRONT, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &g_frontMesh.vertexArrayId);
	glBindVertexArray(g_frontMesh.vertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, g_frontMesh.bufferId);
	glVertexAttribPointer(Positions_Index, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Positions_Index);
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
	glDeleteVertexArrays(1, &g_frontMesh.vertexArrayId);
	g_frontMesh.vertexArrayId = 0;
	glDeleteBuffers(1, &g_frontMesh.bufferId);
	g_frontMesh.bufferId = 0;
	glDeleteProgram(g_programId);
	g_programId = 0;
	ServiceGl_shutdown(state);
}
