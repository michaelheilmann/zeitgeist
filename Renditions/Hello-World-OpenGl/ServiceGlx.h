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

#endif // ZEITGEIST_RENDITION_SERVICEGLX_H_INCLUDED
