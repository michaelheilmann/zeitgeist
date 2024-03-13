// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(SERVICEGL_H_INCLUDED)
#define SERVICEGL_H_INCLUDED

#include "Zeitgeist.h"

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <GL/gl.h>
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#include <GL/gl.h>
#endif

#include <GL/glext.h>

#define Define(Type, Name) \
	extern Type Name;
#include "ServiceGl_Functions.i"
#undef Define

void
ServiceGl_startup
	(
		Zeitgeist_State* state
	);

void
ServiceGl_shutdown
	(
		Zeitgeist_State* state
	);

void
ServiceGl_setTitle
	(
		Zeitgeist_State* state,
		Zeitgeist_String* title
	);

void
ServiceGl_getClientSize
	(
		Zeitgeist_State* state,
		Zeitgeist_Integer* width,
		Zeitgeist_Integer* height
	);

void
ServiceGl_beginFrame
	(
		Zeitgeist_State* state
	);

void
ServiceGl_endFrame
	(
		Zeitgeist_State* state
	);

void
ServiceGl_update
	(
		Zeitgeist_State* state
	);

Zeitgeist_Boolean
ServiceGl_quitRequested
	(
		Zeitgeist_State* state
	);

GLuint
ServiceGl_compileShader
	(
		Zeitgeist_State* state,
		GLenum type,
		const GLchar *source
	);

GLuint
ServiceGl_linkProgram
	(
		Zeitgeist_State* state,
		GLuint vert,
		GLuint frag
	);

#endif // SERVICEGL_H_INCLUDED
