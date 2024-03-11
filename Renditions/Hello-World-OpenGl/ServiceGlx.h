// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_RENDITION_SERVICEGLX_H_INCLUDED)
#define ZEITGEIST_RENDITION_SERVICEGLX_H_INCLUDED

#include "Zeitgeist.h"

/**
 * @brief Startup the GLX service.
 * @param state A pointer to the Zeitgeist_State object.
 */
void ServiceGlx_startup(Zeitgeist_State* state);

/**
 * @brief Shutdown the GLX service.
 * @param state A pointer to the Zeitgeist_State object.
 */
void ServiceGlx_shutdown(Zeitgeist_State* state);

/**
 * @brief Set the window title.
 * @param state A pointer to the Zeitgeist_State object.
 * @param title A pointer to the title.
 */
void ServiceGlx_setTitle(Zeitgeist_State* state, Zeitgeist_String* title);

void ServiceGlx_update(Zeitgeist_State* state);

bool ServiceGlx_quitRequested(Zeitgeist_State* state);

#endif // ZEITGEIST_RENDITION_SERVICEGLX_H_INCLUDED
