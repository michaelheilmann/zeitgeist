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

#endif // ZEITGEIST_RENDITION_SERVICEWGL_H_INCLUDED
