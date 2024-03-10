// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist.h"
#include "Zeitgeist/UpstreamRequests.h"

// strlen
#include <string.h>

// fprintf, stdout
#include <stdio.h>

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	#define Zeitgeist_Rendition_Export _declspec(dllexport)
#else
	#define Zeitgeist_Rendition_Export
#endif

Zeitgeist_Rendition_Export Zeitgeist_String*
Zeitgeist_Rendition_getName
	(
		Zeitgeist_State* state
	)
{
	return Zeitgeist_State_createString(state, "Hello World", strlen("Hello World"));
}

Zeitgeist_Rendition_Export void
Zeitgeist_Rendition_run
	(
		Zeitgeist_State* state
	)
{
	fprintf(stdout, "Hello, World!\n");
	Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
	Zeitgeist_sendUpstreamRequest(state, request);
}
