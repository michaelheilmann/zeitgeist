// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "ServiceGl.h"

// malloc
#include <malloc.h>

// fprintf, stdio
#include <stdio.h>

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
