// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/String.h"

#include "Zeitgeist.h"

// fprintf, stderr
#include <stdio.h>

// free
#include <malloc.h>

// memcmp
#include <string.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

void
Zeitgeist_String_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_String* string	
	)
{/*Intentionally empty. Nothing to do here.*/}

void
Zeitgeist_String_finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_String* string
	)
{
	Zeitgeist_String** previous = &state->strings;
	Zeitgeist_String* current = state->strings;
	while (current && current != string) {
		previous = &current->next;
		current = current->next;
	}
	if (current) {
		*previous = current->next;
	}
}

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

Zeitgeist_String*
Zeitgeist_String_concatenate
	(
		Zeitgeist_State* state,
		Zeitgeist_String* v,
		Zeitgeist_String* w
	)
{
	// The string would be too long.
	if (SIZE_MAX - sizeof(Zeitgeist_String) - v->numberOfBytes < w->numberOfBytes) {
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	size_t hashValue = v->numberOfBytes + w->numberOfBytes;
	for (size_t i = 0, n = v->numberOfBytes; i < n; ++i) {
		hashValue = (hashValue << 5) ^ (hashValue >> 3) | (size_t)v->bytes[i];
	}
	for (size_t i = 0, n = w->numberOfBytes; i < n; ++i) {
		hashValue = (hashValue << 5) ^ (hashValue >> 3) | (size_t)w->bytes[i];
	}
	Zeitgeist_String* string = malloc(sizeof(Zeitgeist_String) + v->numberOfBytes + w->numberOfBytes);
	if (!string) {
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	string->hashValue = hashValue;
	string->numberOfBytes = v->numberOfBytes + w->numberOfBytes;
	memcpy(string->bytes, v->bytes, v->numberOfBytes);
	memcpy(string->bytes + v->numberOfBytes, w->bytes, w->numberOfBytes);

	string->next = state->strings;
	state->strings = string;

	((Zeitgeist_Gc_Object*)string)->typeTag = Zeitgeist_Gc_TypeTag_String;
	((Zeitgeist_Gc_Object*)string)->next = state->gc.all;
	state->gc.all = (Zeitgeist_Gc_Object*)string;
	((Zeitgeist_Gc_Object*)string)->color = Zeitgeist_Gc_Color_White;

	return string;
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

	((Zeitgeist_Gc_Object*)string)->typeTag = Zeitgeist_Gc_TypeTag_String;
	((Zeitgeist_Gc_Object*)string)->next = state->gc.all;
	state->gc.all = (Zeitgeist_Gc_Object*)string;
	((Zeitgeist_Gc_Object*)string)->color = Zeitgeist_Gc_Color_White;

	return string;
}
