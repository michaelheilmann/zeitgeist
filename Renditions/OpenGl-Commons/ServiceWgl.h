// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(SERVICEWGL_H_INCLUDED)
#define SERVICEWGL_H_INCLUDED

#include "Zeitgeist.h"

/**
 * @brief Startup the WGL service.
 * @param state A pointer to the Zeitgeist_State object.
 */
void
ServiceWgl_startup
	(
		Zeitgeist_State* state
	);

/**
 * @brief Shutdown the WGL service.
 * @param state A pointer to the Zeitgeist_State object.
 */
void
ServiceWgl_shutdown
	(
		Zeitgeist_State* state
	);

/**
 * @brief Set the window title.
 * @param state A pointer to the Zeitgeist_State object.
 * @param title A pointer to the title.
 */
void
ServiceWgl_setTitle
	(
		Zeitgeist_State* state,
		Zeitgeist_String* title
	);

void
ServiceWgl_update
	(
		Zeitgeist_State* state
	);

Zeitgeist_Boolean
ServiceWgl_quitRequested
	(
		Zeitgeist_State* state
	);

/**
 * @brief Get the size, in pixels, of the client rectangle.
 * @param state A pointer to the Zeitgeist_State object.
 * @param width A pointer to a Zeitgeist_Integer variable.
 * If this function succeeds, this variable is assigned the client width.
 * @param height A pointer to a Zeitgeist_Integer variable.
 * If this function succeeds, this variable is assigned the client height.
 */
void
ServiceWgl_getClientSize
	(
		Zeitgeist_State* state,
		Zeitgeist_Integer *width,
		Zeitgeist_Integer *height
	);

/**
 * @brief Try to link the an OpenGL function.
 * @param state A pointer to the Zeitgeist_State object.
 * @param functionName A pointer to the function name.
 * @param extensionName A pointer to the extension name or the null pointer.
 * @return A poiner to the function on success.
 */
void*
ServiceWgl_link
	(
		Zeitgeist_State* state,
		char const* functionName,
		char const* extensionName
	);
	
void
ServiceWgl_beginFrame
	(
		Zeitgeist_State* state
	);

void
ServiceWgl_endFrame
	(
		Zeitgeist_State* state
	);

#endif // SERVICEWGL_H_INCLUDED
