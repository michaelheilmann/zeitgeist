// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist.h"
#include "Zeitgeist/Value.h"
#include "Zeitgeist/Object.h"

#include "Zeitgeist/UpstreamRequests.h"

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	#include "ServiceWgl.h"
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#include "ServiceGlx.h"
#else
	#error("operating system not (yet) supported")
#endif

// strlen
#include <string.h>

// fprintf, stdout
#include <stdio.h>

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	#define Zeitgeist_Rendition_Export _declspec(dllexport)
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#define Zeitgeist_Rendition_Export
#else
	#error("operating system not (yet) supported")
#endif

Zeitgeist_Rendition_Export Zeitgeist_String*
Zeitgeist_Rendition_getName
	(
		Zeitgeist_State* state
	)
{
	return Zeitgeist_State_createString(state, "Hello World (OpenGL)", strlen("Hello World (OpenGL)"));
}

Zeitgeist_Rendition_Export void
Zeitgeist_Rendition_update
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
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	if (ServiceWgl_quitRequested(state)) {
		Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
		Zeitgeist_sendUpstreamRequest(state, request);
	}
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	if (ServiceGlx_quitRequested(state)) {
		Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
		Zeitgeist_sendUpstreamRequest(state, request);
	}
#else
	#error("operating system not (yet) supported")
#endif
}

Zeitgeist_Rendition_Export void
Zeitgeist_Rendition_load
	(
		Zeitgeist_State* state
	)
{
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	ServiceWgl_startup(state);
	ServiceWgl_setTitle(state, Zeitgeist_State_createString(state, "Hello, World!", strlen("Hello, World!")));
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	ServiceGlx_startup(state);
	ServiceGlx_setTitle(state, Zeitgeist_State_createString(state, "Hello, World!", strlen("Hello, World!")));
#else
	#error("operating system not (yet) supported")
#endif
}

Zeitgeist_Rendition_Export void
Zeitgeist_Rendition_unload
	(
		Zeitgeist_State* state
	)
{
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	ServiceWgl_shutdown(state);
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	ServiceGlx_shutdown(state);
#else
	#error("operating system not (yet) supported")
#endif
}
