// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_RENDITION_SERVICEOPENGL_H_INCLUDED)
#define ZEITGEIST_RENDITION_SERVICEOPENGL_H_INCLUDED

#include "Zeitgeist.h"

/**
 * @brief Startup the OpenGL service.
 * @param state A pointer to the Zeitgeist_State object.
 */
void ServiceOpenGl_startup(Zeitgeist_State* state);

/**
 * @brief Shutdown the OpenGL service.
 * @param state A pointer to the Zeitgeist_State object.
 */
void ServiceOpenGl_shutdown(Zeitgeist_State* state);

#endif // ZEITGEIST_RENDITION_SERVICEOPENGL_H_INCLUDED
