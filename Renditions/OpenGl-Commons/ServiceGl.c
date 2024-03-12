// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "ServiceGl.h"

// malloc
#include <malloc.h>

// fprintf, stdio
#include <stdio.h>

Vector3R32*
Vector3R32_create
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 x,
		Zeitgeist_Real32 y,
		Zeitgeist_Real32 z
	)
{
	Vector3R32* self = malloc(sizeof(Vector3R32));
	if (!self) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	self->e[0] = x;
	self->e[1] = y;
	self->e[2] = z;

	((Zeitgeist_Object*)self)->finalize = NULL;
	((Zeitgeist_Object*)self)->visit = NULL;

	((Zeitgeist_Gc_Object*)self)->typeTag = Zeitgeist_Gc_TypeTag_Object;
	((Zeitgeist_Gc_Object*)self)->next = state->gc.all;
	state->gc.all = (Zeitgeist_Gc_Object*)self;
	((Zeitgeist_Gc_Object*)self)->color = Zeitgeist_Gc_Color_White;
	((Zeitgeist_Object*)self)->gclist = NULL;

	return self;
}

Matrix4R32*
Matrix4R32_createIdentity
	(
		Zeitgeist_State* state
	)
{
	Matrix4R32* self = malloc(sizeof(Matrix4R32));
	if (!self) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}

	self->e[0][0] = 1.f;
	self->e[0][1] = 0.f;
	self->e[0][2] = 0.f;
	self->e[0][3] = 0.f;
	
	self->e[1][0] = 0.f;
	self->e[1][1] = 1.f;
	self->e[1][2] = 0.f;
	self->e[1][3] = 0.f;

	self->e[2][0] = 0.f;
	self->e[2][1] = 0.f;
	self->e[2][2] = 1.f;
	self->e[2][3] = 0.f;

	self->e[3][0] = 0.f;
	self->e[3][1] = 0.f;
	self->e[3][2] = 0.f;
	self->e[3][3] = 1.f;

	((Zeitgeist_Object*)self)->finalize = NULL;
	((Zeitgeist_Object*)self)->visit = NULL;

	((Zeitgeist_Gc_Object*)self)->typeTag = Zeitgeist_Gc_TypeTag_Object;
	((Zeitgeist_Gc_Object*)self)->next = state->gc.all;
	state->gc.all = (Zeitgeist_Gc_Object*)self;
	((Zeitgeist_Gc_Object*)self)->color = Zeitgeist_Gc_Color_White;
	((Zeitgeist_Object*)self)->gclist = NULL;

	return self;
}

Matrix4R32*
Matrix4R32_createTranslate
	(
		Zeitgeist_State* state,
		Vector3R32* translate
	)
{
	Matrix4R32* self = malloc(sizeof(Matrix4R32));
	if (!self) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}

	self->e[0][0] = 1.f;
	self->e[0][1] = 0.f;
	self->e[0][2] = 0.f;
	self->e[0][3] = 0.f;

	self->e[1][0] = 0.f;
	self->e[1][1] = 1.f;
	self->e[1][2] = 0.f;
	self->e[1][3] = 0.f;

	self->e[2][0] = 0.f;
	self->e[2][1] = 0.f;
	self->e[2][2] = 1.f;
	self->e[2][3] = 0.f;

	self->e[3][0] = translate->e[0];
	self->e[3][1] = translate->e[1];
	self->e[3][2] = translate->e[2];
	self->e[3][3] = 1.f;

	((Zeitgeist_Object*)self)->finalize = NULL;
	((Zeitgeist_Object*)self)->visit = NULL;

	((Zeitgeist_Gc_Object*)self)->typeTag = Zeitgeist_Gc_TypeTag_Object;
	((Zeitgeist_Gc_Object*)self)->next = state->gc.all;
	state->gc.all = (Zeitgeist_Gc_Object*)self;
	((Zeitgeist_Gc_Object*)self)->color = Zeitgeist_Gc_Color_White;
	((Zeitgeist_Object*)self)->gclist = NULL;

	return self;
}

KeyboardKeyMessage*
KeyboardKeyMessage_create
	(
		Zeitgeist_State* state,
		Zeitgeist_Integer action,
		Zeitgeist_Integer key
	)
{
	KeyboardKeyMessage* self = malloc(sizeof(KeyboardKeyMessage));
	if (!self) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}

	self->action = action;
	self->key = key;

	((Zeitgeist_Object*)self)->finalize = NULL;
	((Zeitgeist_Object*)self)->visit = NULL;

	((Zeitgeist_Gc_Object*)self)->typeTag = Zeitgeist_Gc_TypeTag_Object;
	((Zeitgeist_Gc_Object*)self)->next = state->gc.all;
	state->gc.all = (Zeitgeist_Gc_Object*)self;
	((Zeitgeist_Gc_Object*)self)->color = Zeitgeist_Gc_Color_White;
	((Zeitgeist_Object*)self)->gclist = NULL;

	return self;
}

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	#include "ServiceWgl.h"
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <GL/gl.h>
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#include "ServiceGlx.h"
	#include <GL/gl.h>
#else
	#error("operating system not (yet) supported")
#endif

#include <GL/glext.h>

#define Define(Type, Name) \
	Type Name = NULL;
#include "ServiceGl_Functions.i"
#undef Define

static void*
link
	(
		Zeitgeist_State* state,
		char const* functionName,
		char const* extensionName
	)
{
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	return ServiceWgl_link(state, functionName, extensionName);
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	return ServiceGlx_link(state, functionName, extensionName);
#else
	#error("operating system not (yet) supported")
#endif
}

void
ServiceGl_startup
	(
		Zeitgeist_State* state
	)
{ 
#define Define(Type, Name) \
	Name = (Type)link(state, #Name, NULL);
#include "ServiceGl_Functions.i"
#undef Define
}

void
ServiceGl_shutdown
	(
		Zeitgeist_State* state
	)
{ }

GLuint
ServiceGl_compileShader
	(
		Zeitgeist_State* state,
		GLenum type,
		const GLchar *source
	)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	GLint param;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &param);
	if (!param) {
		GLchar log[4096];
		glGetShaderInfoLog(shader, sizeof(log), NULL, log);
		fprintf(stderr, "error: %s: %s\n",
				    type == GL_FRAGMENT_SHADER ? "frag" : "vert", (char *) log);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	return shader;
}

GLuint
ServiceGl_linkProgram
	(
		Zeitgeist_State* state,
		GLuint vert,
		GLuint frag
	)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	GLint param;
	glGetProgramiv(program, GL_LINK_STATUS, &param);
	if (!param) {
		GLchar log[4096];
		glGetProgramInfoLog(program, sizeof(log), NULL, log);
		fprintf(stderr, "error: link: %s\n", (char *) log);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	return program;
}
