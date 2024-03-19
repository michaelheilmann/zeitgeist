// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_UPSTREAMREQUESTS_H_INCLUDED)
#define ZEITGEIST_UPSTREAMREQUESTS_H_INCLUDED

#include "Zeitgeist/Object.h"
#include "Zeitgeist/Value.h"

/**
 * @since 0.1
 * @brief An upstream request is send from an Rendition to Zeitgeist.
 * The only upstream
 */
typedef struct Zeitgeist_UpstreamRequest Zeitgeist_UpstreamRequest;

/**
 * @since 0.1
 * Symbolic constant that when assigned to Zeitgeist_UpstreamRequest indicates that it is a "exit process request" upstream request.
 */
#define Zeitgeist_UpstreamRequestType_ExitProcessRequest (1)

struct Zeitgeist_UpstreamRequest {
  Zeitgeist_ForeignObject _parent;
  Zeitgeist_Integer type;
};

/**
 * @since 0.1
 * @brief Create a "exit process request" upstream request.
 * @param state A pointer to the Zeitgeist_State object.
 * @return A pointer to the Zeitgeist_UpstreamRequest object.
 */
Zeitgeist_UpstreamRequest*
Zeitgeist_UpstreamRequest_createExitProcessRequest
  (
    Zeitgeist_State* state
  );

/**
 * @since 0.1
 * @brief Send an upstream request.
 * @param state A pointer to the Zeitgeist_State object.
 * @param state A pointer to the Zeitgeist_UpstreamRequest object.
 */
void
Zeitgeist_sendUpstreamRequest
  (
    Zeitgeist_State* state,
    Zeitgeist_UpstreamRequest* request
  );

#endif // ZEITGEIST_UPSTREAMREQUESTS_H_INCLUDED
