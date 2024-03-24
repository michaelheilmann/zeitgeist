// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "ServiceGl.h"

// malloc
#include <malloc.h>

// fprintf, stdio
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

#include "Zeitgeist/Locks.h"
#include "Zeitgeist/WeakReferences.h"

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

static Zeitgeist_List* g_keyboardKeyListeners = NULL;

static Zeitgeist_Integer g_referenceCount = 0;

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
	if (g_referenceCount == 0) {
	#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
		ServiceWgl_startup(state);
	#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
		ServiceGlx_startup(state);
	#else
		#error("operating system not (yet) supported")
	#endif
	#define Define(Type, Name) \
		Name = (Type)link(state, #Name, NULL);
	#include "ServiceGl_Functions.i"
	#undef Define
	}
	g_referenceCount++;
}

void
ServiceGl_shutdown
	(
		Zeitgeist_State* state
	)
{
	if (0 == --g_referenceCount) {
		if (g_keyboardKeyListeners) {
			Zeitgeist_unlock(state, (Zeitgeist_Gc_Object*)g_keyboardKeyListeners);
			g_keyboardKeyListeners = NULL;
		}
	#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
		ServiceWgl_shutdown(state);
	#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
		ServiceGlx_shutdown(state);
	#else
		#error("operating system not (yet) supported")
	#endif
	}
}

void
ServiceGl_setTitle
	(
		Zeitgeist_State* state,
		Zeitgeist_String* title
	)
{
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	ServiceWgl_setTitle(state, title);
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	ServiceGlx_setTitle(state, title);
#else
	#error("operating system not (yet) supported")
#endif
}

void
ServiceGl_getClientSize
	(
		Zeitgeist_State* state,
		Zeitgeist_Integer* width,
		Zeitgeist_Integer* height
	)
{
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	ServiceWgl_getClientSize(state, width, height);
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	ServiceGlx_getClientSize(state, width, height);
#else
	#error("operating system not (yet) supported")
#endif
}

void
ServiceGl_beginFrame
	(
		Zeitgeist_State* state
	)
{
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	ServiceWgl_beginFrame(state);
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	ServiceGlx_beginFrame(state);
#else
	#error("operating system not (yet) supported")
#endif
}

void
ServiceGl_endFrame
	(
		Zeitgeist_State* state
	)
{
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	ServiceWgl_endFrame(state);
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	ServiceGlx_endFrame(state);
#else
	#error("operating system not (yet) supported")
#endif
}

void
ServiceGl_update
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
}

Zeitgeist_Boolean
ServiceGl_quitRequested
	(
		Zeitgeist_State* state
	)
{
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	return ServiceWgl_quitRequested(state);
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	return ServiceGlx_quitRequested(state);
#else
	#error("operating system not (yet) supported")
#endif
}

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

void
ServiceGl_emitKeyboardKeyMessage
	(
		Zeitgeist_State* state,
		KeyboardKeyMessage* message
	)
{
	Zeitgeist_Value temporary = Zeitgeist_List_getSize(state, g_keyboardKeyListeners);
	for (size_t i = 0, n = Zeitgeist_Value_getInteger(&temporary); i < n; ++i) {
		temporary = Zeitgeist_List_getValue(state, g_keyboardKeyListeners, i);
		if (Zeitgeist_Value_hasWeakReference(&temporary)) {
			Zeitgeist_WeakReference* weakReference = Zeitgeist_Value_getWeakReference(&temporary);
			if (weakReference->reference) {
				switch (weakReference->reference->typeTag) {
					case Zeitgeist_Gc_TypeTag_List: {
						fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
						exit(EXIT_FAILURE);
					} break;
					case Zeitgeist_Gc_TypeTag_Map: {
						fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
						exit(EXIT_FAILURE);
					} break;
					case Zeitgeist_Gc_TypeTag_ForeignObject: {
						fprintf(stderr, "%s:%d: not yet implemented\n", __FILE__, __LINE__);
						exit(EXIT_FAILURE);
					} break;
					case Zeitgeist_Gc_TypeTag_String: {
						fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
						exit(EXIT_FAILURE);
					} break;
					default: {
						fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
						exit(EXIT_FAILURE);
					} break;
				};
			}
		} else if (Zeitgeist_Value_hasForeignFunction(&temporary)) {
			Zeitgeist_ForeignFunction* foreignFunction = Zeitgeist_Value_getForeignFunction(&temporary);
			Zeitgeist_Stack_pushForeignObject(state, (Zeitgeist_ForeignObject*)message);
			Zeitgeist_Stack_pushInteger(state, 1);
			(*foreignFunction)(state);
		} else {
			fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
			exit(EXIT_FAILURE);
		}
	}
}

void
ServiceGl_addKeyboardKeyCallback
	(
		Zeitgeist_State* state,
		Zeitgeist_Value* value
	)
{
	if (!g_keyboardKeyListeners) {
		g_keyboardKeyListeners = Zeitgeist_List_create(state);
		Zeitgeist_JumpTarget jumpTarget;
		Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
		if (!setjmp(jumpTarget.environment)) {
			Zeitgeist_lock(state, (Zeitgeist_Gc_Object*)g_keyboardKeyListeners);
			Zeitgeist_State_popJumpTarget(state);
		} else {
			Zeitgeist_State_popJumpTarget(state);
			g_keyboardKeyListeners = NULL;
			longjmp(state->jumpTarget->environment, -1);
		}
	}
	if (Zeitgeist_Value_hasVoid(value)) {
		return;
	} else if (Zeitgeist_Value_hasForeignFunction(value)) {
		Zeitgeist_List_appendValue(state, g_keyboardKeyListeners, value);
	} else if (Zeitgeist_Value_hasForeignObject(value)) {
		Zeitgeist_Value temporary;
		Zeitgeist_Value_setWeakReference(&temporary, Zeitgeist_WeakReference_create(state, (Zeitgeist_Gc_Object*)Zeitgeist_Value_getForeignObject(value)));
		Zeitgeist_List_appendValue(state, g_keyboardKeyListeners, &temporary);
	} else {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
}
