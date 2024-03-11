// Copyright (c) 2024 Michael Heilmann. All rights reserved.
#include "Zeitgeist.h"

// malloc, free
#include <malloc.h>

// memcpy
#include <string.h>

// SIZE_MAX
#include <limits.h>

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
	state->gc.all = NULL;
	state->jumpTarget = NULL;
	return state;
}

static void finalizeList(Zeitgeist_State* state, Zeitgeist_List* list) {
	free(list->elements);
	free(list);
}

static void finalizeMap(Zeitgeist_State* state, Zeitgeist_Map* map) {
	for (size_t i = 0, n = map->capacity; i < n; ++i) {
		Zeitgeist_Map_Node** bucket = &(map->buckets[i]);
		while (*bucket) {
			Zeitgeist_Map_Node* node = *bucket;
			*bucket = node->next;
			free(node);
		}
	}
	free(map->buckets);
	free(map);
}

static void finalizeObject(Zeitgeist_State* state, Zeitgeist_Object* object) {
	if (object->finalize) {
		object->finalize(state, object);
	}
	free(object);
}

static void runGc(Zeitgeist_State* state) {
	while (state->gc.all) {
		Zeitgeist_Gc_Object* object = state->gc.all;
		state->gc.all = object->next;
		switch (object->typeTag) {
			case Zeitgeist_Gc_TypeTag_List: {
				finalizeList(state, (Zeitgeist_List*)object);
			} break;
			case Zeitgeist_Gc_TypeTag_Map: {
				free(((Zeitgeist_Map*)object)->buckets);
				free(object);
			} break;
			case Zeitgeist_Gc_TypeTag_Object: {
				finalizeObject(state, (Zeitgeist_Object*)object);
			} break;
		}
	}
	while (state->strings) {
		Zeitgeist_String* string = state->strings;
		state->strings = state->strings->next;
		free(string);
	}
}

void
Zeitgeist_State_destroy
	(
		Zeitgeist_State* state
	)
{
	runGc(state);
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

void
Zeitgeist_State_raiseError
	(
		Zeitgeist_State* state,
		char *file,
		int line,
		int error
	)
{
	state->lastError = error;
	longjmp(state->jumpTarget->environment, -1);
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
