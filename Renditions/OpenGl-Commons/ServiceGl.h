// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(SERVICEGL_H_INCLUDED)
#define SERVICEGL_H_INCLUDED

#include "Zeitgeist.h"

typedef struct Vector3R32 {
	Zeitgeist_Object _parent;
	Zeitgeist_Real32 e[3];
} Vector3R32;

Vector3R32*
Vector3R32_create
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 x,
		Zeitgeist_Real32 y,
		Zeitgeist_Real32 Z
	);

typedef struct Matrix4R32 {
	Zeitgeist_Object _parent;
	Zeitgeist_Real32 e[4][4];
} Matrix4R32;

Matrix4R32*
Matrix4R32_createIdentity
	(
		Zeitgeist_State *state
	);

Matrix4R32*
Matrix4R32_createTranslate
	(
		Zeitgeist_State* state,
		Vector3R32* translation
	);

#define KeyboardKey_Action_Released (0)
#define KeyboardKey_Action_Pressed (1)

#define KeyboardKey_Up (1)
#define KeyboardKey_Down (2)
#define KeyboardKey_Left (3)
#define KeyboardKey_Right (4)

typedef struct KeyboardKeyMessage {
	Zeitgeist_Object _parent;
	Zeitgeist_Integer action;
	Zeitgeist_Integer key;
} KeyboardKeyMessage;

KeyboardKeyMessage*
KeyboardKeyMessage_create	
	(
		Zeitgeist_State* state,
		Zeitgeist_Integer action,
		Zeitgeist_Integer key
	);

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
