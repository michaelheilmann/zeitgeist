// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_RENDITION_SERVICEWGL_H_INCLUDED)
#define ZEITGEIST_RENDITION_SERVICEWGL_H_INCLUDED

#include "Zeitgeist.h"

/**
 * @brief Startup the WGL service.
 * @param state A pointer to the Zeitgeist_State object.
 */
void ServiceWgl_startup(Zeitgeist_State* state);

/**
 * @brief Shutdown the WGL service.
 * @param state A pointer to the Zeitgeist_State object.
 */
void ServiceWgl_shutdown(Zeitgeist_State* state);

/**
 * @brief Set the window title.
 * @param state A pointer to the Zeitgeist_State object.
 * @param title A pointer to the title.
 */
void ServiceWgl_setTitle(Zeitgeist_State* state, Zeitgeist_String* title);

void ServiceWgl_update(Zeitgeist_State* state);

bool ServiceWgl_quitRequested(Zeitgeist_State* state);

#endif // ZEITGEIST_RENDITION_SERVICEWGL_H_INCLUDED
