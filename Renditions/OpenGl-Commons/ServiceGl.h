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

/**
 * @since 0.1
 * @brief Initialize the GL service.
 * @remarks The GL service is shared between renditions.
 * The service is reference counted.
 * Each time a rendition calls ServiceGL_startup successfully,
 * the reference count of the service is incremented by @a 1.
 * Each time a rendition calls ServiceGL_shutdown successfully,
 * the reference count of the service is decremented by @a 1.
 * If the reference count increments from zero to one, the service is created.
 * If the reference count decrements from one to zero, the service is destroyed.
 */
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

typedef struct KeyboardKeyMessage KeyboardKeyMessage;

/**
 * @since 0.1
 * @brief Emit a keyboard key message.
 * @param message A pointer to the KeyboardKeyMessage object.
 */
void
ServiceGl_emitKeyboardKeyMessage
	(
		Zeitgeist_State* state,
		KeyboardKeyMessage* message
	);

/**
 * @since 0.1
 * @brief Add a keyboard key callback.
 * @param state A pointer to the Zeitgeist_State.
 * @param value The value to be used as a callback.
 * @remarks
 * - If the value stores a Zeitgeist_ForeignFunction* value,
 *   then this Zeitgeist_ForeignFunction* is added to the callback list.
 *   If Zeitgeist_KeyboardKeyMessage is dispatched,
 *   then the Zeitgeist_ForeignFunction* is invoked with the KeyboardKeyMessage* value as its single argument.
 * - If the value stores a Zeitgeist_Object value,
 *   then this object is wrapped into a weak reference and the weak reference is added to the callback list.
 *   If a Zeitgeist_KeyboardKeyMessage is dispatched,
 *   then the the Zeitgeist_Object value in the weak reference is retrieved.
 *   If it expired, then the weak reference is removed from the callback list.
 *   Otherwise the Zeitgeist_Object's "call" metamethod is invoked with
 *   the Zeitgeist_Object as the first argument
 *   and
 *   the KeyboardKeyMessage object as the second argument.
 * - If the value stores a Zeitgeist_Void value,
 *   then nothing is added to the callback list.
 * - If value stores any other value than the above,
 *   an error is raised.
 */
void
ServiceGl_addKeyboardKeyCallback
	(
		Zeitgeist_State* state,
		Zeitgeist_Value* value
	);

#endif // SERVICEGL_H_INCLUDED
