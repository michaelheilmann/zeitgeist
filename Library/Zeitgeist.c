// Copyright (c) 2024 Michael Heilmann. All rights reserved.
#include "Zeitgeist.h"

// malloc, free
#include <malloc.h>

// memcpy
#include <string.h>

// SIZE_MAX
#include <limits.h>

Zeitgeist_Boolean
Zeitgeist_String_areEqual
  (
    Zeitgeist_State* state,
    Zeitgeist_String* v,
    Zeitgeist_String* w
  )
{
  if (v == w) return Zeitgeist_Boolean_True;
  else if (v->numberOfBytes == w->numberOfBytes && v->hashValue == w->hashValue) {
    return !memcmp(v->bytes, w->bytes, v->numberOfBytes);
  } else {
    return Zeitgeist_Boolean_False;
  }
}

Zeitgeist_Boolean
Zeitgeist_State_isExitProcessRequested
  (
    Zeitgeist_State* state
  )
{
  return state->exitProcessRequested; 
}

Zeitgeist_State*
Zeitgeist_createState
  (
  )
{
  Zeitgeist_State* state = malloc(sizeof(Zeitgeist_State));
  if (!state) {
    return NULL;
  }
  state->exitProcessRequested = Zeitgeist_Boolean_False;
  state->lastError = 0;
  state->strings = NULL;
  state->objects = NULL;
  state->arrayLists = NULL;
  state->jumpTarget = NULL;
  return state;
}

void
Zeitgeist_State_destroy
  (
    Zeitgeist_State* state
  )
{
  while (state->objects) {
    Zeitgeist_Object* object = state->objects;
    state->objects = state->objects->next;
    if (object->finalize) {
      object->finalize(state, object);
    }
    free(object);
  }
  while (state->arrayLists) {
    Zeitgeist_ArrayList* arrayList = state->arrayLists;
    state->arrayLists = state->arrayLists->next;
    free(arrayList->elements);
    free(arrayList);
  }
  while (state->strings) {
    Zeitgeist_String* string = state->strings;
    state->strings = state->strings->next;
    free(string);
  }
  free(state);
}

void
Zeitgeist_State_pushJumpTarget
  (
    Zeitgeist_State* state,
    Zeitgeist_JumpTarget* jumpTarget
  )
{
  jumpTarget->previous = state->jumpTarget;
  state->jumpTarget = jumpTarget;
}

void
Zeitgeist_State_popJumpTarget
  (
    Zeitgeist_State* state
  )
{
  state->jumpTarget = state->jumpTarget->previous;
}

Zeitgeist_String*
Zeitgeist_State_createString
  (
    Zeitgeist_State* state,
    char const* bytes,
    size_t numberOfBytes
  )
{
  if (SIZE_MAX - sizeof(Zeitgeist_String) < numberOfBytes) {
    state->lastError = 1;
    longjmp(state->jumpTarget->environment, -1);
  }
  size_t hashValue = numberOfBytes;
  for (size_t i = 0, n = numberOfBytes; i < n; ++i) {
    hashValue = (hashValue << 5) ^ (hashValue >> 3) | (size_t)bytes[i];
  }
  Zeitgeist_String* string = malloc(sizeof(Zeitgeist_String) + numberOfBytes);
  if (!string) {
    state->lastError = 1;
    longjmp(state->jumpTarget->environment, -1);
  }
  string->hashValue = hashValue;
  string->numberOfBytes = numberOfBytes;
  memcpy(string->bytes, bytes, numberOfBytes);
  
  string->next = state->strings;
  state->strings = string;
  return string;
}

Zeitgeist_String*
Zeitgeist_String_concatenate
  (
    Zeitgeist_State* state,
    Zeitgeist_String* prefix,
    Zeitgeist_String* suffix
  )
{
  // The string would be too long.
  if (SIZE_MAX - sizeof(Zeitgeist_String) - prefix->numberOfBytes < suffix->numberOfBytes) {
    state->lastError = 1;
    longjmp(state->jumpTarget->environment, -1);
  }
  size_t hashValue = prefix->numberOfBytes + suffix->numberOfBytes;
  for (size_t i = 0, n = prefix->numberOfBytes; i < n; ++i) {
    hashValue = (hashValue << 5) ^ (hashValue >> 3) | (size_t)prefix->bytes[i];
  }
  for (size_t i = 0, n = suffix->numberOfBytes; i < n; ++i) {
    hashValue = (hashValue << 5) ^ (hashValue >> 3) | (size_t)suffix->bytes[i];
  }
  Zeitgeist_String* string = malloc(sizeof(Zeitgeist_String) + prefix->numberOfBytes + suffix->numberOfBytes);
  if (!string) {
    state->lastError = 1;
    longjmp(state->jumpTarget->environment, -1);
  }
  string->hashValue = hashValue;
  string->numberOfBytes = prefix->numberOfBytes + suffix->numberOfBytes;
  memcpy(string->bytes, prefix->bytes, prefix->numberOfBytes);
  memcpy(string->bytes + prefix->numberOfBytes, suffix->bytes, suffix->numberOfBytes);

  string->next = state->strings;
  state->strings = string;
  return string;
}
