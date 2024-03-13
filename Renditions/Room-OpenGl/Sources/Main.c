// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist.h"
#include "Zeitgeist/Value.h"
#include "Zeitgeist/Object.h"

#include "Zeitgeist/UpstreamRequests.h"

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	#include "ServiceWgl.h"
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#include "ServiceGlx.h"
#else
	#error("operating system not (yet) supported")
#endif

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
const GLchar* g_vertexShader =
	"#version 330\n"
	"layout(location = 0) in vec3 point;\n"
	"uniform mat4 modelToWorld = mat4(1);\n"
	"uniform mat4 worldToView = mat4(1);\n"
	"uniform mat4 viewToProjection = mat4(1);\n"
	"void main() {\n"
	"  mat4 modelToProjection = viewToProjection * worldToView * modelToWorld;\n"
	"  gl_Position = modelToProjection * vec4(0.75 * point.xy, point.z, 1.0);\n"
	"}\n"
	;

// Fragment shader.
// The color (255, 204, 51) is the websafe color "sunglow".
const GLchar* g_fragmentShader =
	"#version 330\n"
	"out vec4 color;\n"
	"void main() {\n"
	"  color = vec4(1, 0.8, 0.2, 0);\n"
	"}\n"
	;

static GLint Positions_Index = 0;
static GLint ModelToWorld_Index = -1;
static GLint WorldToView_Index = -1;
static GLint ViewToProjection_Index = -1;

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
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	ServiceWgl_update(state);
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	ServiceGlx_update(state);
#else
	#error("operating system not (yet) supported")
#endif
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	if (ServiceWgl_quitRequested(state)) {
		Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
		Zeitgeist_sendUpstreamRequest(state, request);
	}
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	if (ServiceGlx_quitRequested(state)) {
		Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
		Zeitgeist_sendUpstreamRequest(state, request);
	}
#else
	#error("operating system not (yet) supported")
#endif

	Zeitgeist_Integer viewportWidth, viewportHeight;
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	ServiceWgl_getClientSize(state, &viewportWidth, &viewportHeight);
	ServiceWgl_beginFrame(state);
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	ServiceGlx_getClientSize(state, &viewportWidth, &viewportHeight);
	ServiceGlx_beginFrame(state);
#else
	#error("operating system not (yet) supported")
#endif

	glViewport(0, 0, viewportWidth, viewportHeight);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(g_programId);
	// bind view
	// bind projection
	glBindVertexArray(g_frontMesh.vertexArrayId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(FRONT)/ sizeof(VERTEX));
	glBindVertexArray(0);
	glUseProgram(0);


#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	ServiceWgl_endFrame(state);
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	ServiceGlx_endFrame(state);
#else
	#error("operating system not (yet) supported")
#endif
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

	ModelToWorld_Index = glGetUniformLocation(g_programId, "modelToWorld");
	WorldToView_Index = glGetUniformLocation(g_programId, "worldToView");
	ViewToProjection_Index = glGetUniformLocation(g_programId, "ViewToProjection");

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
