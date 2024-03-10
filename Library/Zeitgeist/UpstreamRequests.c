// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/UpstreamRequests.h"

#include "Zeitgeist.h"
// malloc, free
#include <malloc.h>

Zeitgeist_UpstreamRequest*
Zeitgeist_UpstreamRequest_createExitProcessRequest
  (
    Zeitgeist_State* state
  )
{
  Zeitgeist_UpstreamRequest* upstreamRequest = malloc(sizeof(Zeitgeist_UpstreamRequest));
  if (!upstreamRequest) {
    state->lastError = 1;
    longjmp(state->jumpTarget->environment, -1);
  }
  upstreamRequest->type = Zeitgeist_UpstreamRequestType_ExitProcessRequest;
  ((Zeitgeist_Object*)upstreamRequest)->finalize = NULL;
  ((Zeitgeist_Object*)upstreamRequest)->next = state->objects;
  state->objects = (Zeitgeist_Object*)upstreamRequest;
  return upstreamRequest;
}


void
Zeitgeist_sendUpstreamRequest
  (
    Zeitgeist_State* state,
    Zeitgeist_UpstreamRequest* request
  )
{ 
  if (Zeitgeist_UpstreamRequestType_ExitProcessRequest == request->type) {
    state->exitProcessRequested = true;
  }
}
