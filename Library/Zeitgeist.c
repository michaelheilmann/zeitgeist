// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist.h"

// malloc, free
#include <malloc.h>

// memcpy
#include <string.h>

// SIZE_MAX
#include <limits.h>

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

static void
finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_Gc_Object* object
	);

static void
premark
	(
		Zeitgeist_State* state
	);

static void
mark
	(
		Zeitgeist_State* state
	);

static void
sweep
	(
		Zeitgeist_State* state
	);

static void
runGc
	(
		Zeitgeist_State* state
	);

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
	state->gc.gray = NULL;
	state->jumpTarget = NULL;

	state->stack.maximalCapacity = SIZE_MAX / sizeof(Zeitgeist_Value);
	if (state->stack.maximalCapacity > Zeitgeist_Integer_Maximum) {
		state->stack.maximalCapacity = Zeitgeist_Integer_Maximum;
	}
	state->stack.elements = malloc(sizeof(Zeitgeist_Value) * 8);
	if (!state->stack.elements) {
		free(state);
		return NULL;
	}
	state->stack.capacity = 8;
	state->stack.size = 0;

	return state;
}

static void 
finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_Gc_Object* object
	)
{ 
	switch (object->typeTag) {
		case Zeitgeist_Gc_TypeTag_List: {
			Zeitgeist_List_finalize(state, (Zeitgeist_List*)object);
			free(object);
		} break;
		case Zeitgeist_Gc_TypeTag_Map: {
			Zeitgeist_Map_finalize(state, (Zeitgeist_Map*)object);
			free(object);
		} break;
		case Zeitgeist_Gc_TypeTag_Object: {
			Zeitgeist_Object_finalize(state, (Zeitgeist_Object*)object);
			free(object);
		} break;
		case Zeitgeist_Gc_TypeTag_String: {
			Zeitgeist_String_finalize(state, (Zeitgeist_String*)object);
			free(object);
		} break;
	}
}

static void
mark
	(
		Zeitgeist_State* state
	)
{
	while (state->gc.gray) {
		Zeitgeist_Gc_Object* object = state->gc.gray;
		switch (object->typeTag) {
			case Zeitgeist_Gc_TypeTag_List: {
				state->gc.gray = ((Zeitgeist_List*)object)->gclist;
				((Zeitgeist_List*)object)->gclist = NULL;
				Zeitgeist_List_visit(state, (Zeitgeist_List*)object);
			} break;
			case Zeitgeist_Gc_TypeTag_Map: {
				state->gc.gray = ((Zeitgeist_Map*)object)->gclist;
				((Zeitgeist_Map*)object)->gclist = NULL;
				Zeitgeist_Map_visit(state, (Zeitgeist_Map*)object);
			} break;
			case Zeitgeist_Gc_TypeTag_Object: {
				state->gc.gray = ((Zeitgeist_Object*)object)->gclist;
				((Zeitgeist_Object*)object)->gclist = NULL;
				Zeitgeist_Object_visit(state, (Zeitgeist_Object*)object);
			} break;
			default: {
				fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
				exit(EXIT_FAILURE);
			} break;
		};
		Zeitgeist_Gc_Object_setBlack(object);
	}
}

static void
premark
	(
		Zeitgeist_State* state
	)
{ 
	for (size_t i = 0, n = state->stack.size; i < n; ++i) {
		Zeitgeist_Value_visit(state, state->stack.elements + i);
	}
}

static void
sweep
	(
		Zeitgeist_State* state
	)
{
	Zeitgeist_Gc_Object** previous = &state->gc.all;
	Zeitgeist_Gc_Object* current = state->gc.all;
	while (NULL != current) {
		if (Zeitgeist_Gc_Object_isWhite(current)) {
			Zeitgeist_Gc_Object* object = current;
			*previous = current->next;
			current = current->next;
			finalize(state, object);
		} else {
			Zeitgeist_Gc_Object_setWhite(current);
			previous = &current->next;
			current = current->next;
		}
	}
}

static void
runGc
	(
		Zeitgeist_State* state
	)
{
	premark(state);
	mark(state);
	sweep(state);
}

void
Zeitgeist_State_destroy
	(
		Zeitgeist_State* state
	)
{
	runGc(state);
	if (state->gc.all) {
		fprintf(stderr, "%s:%d: warning: gc all list not empty\n", __FILE__, __LINE__);
	}
	if (state->gc.gray) {
		fprintf(stderr, "%s:%d: warning: gc gray list not empty\n", __FILE__, __LINE__);
	}
	if (state->strings) {
		fprintf(stderr, "%s:%d: warning: string list not empty\n", __FILE__, __LINE__);
	}

	state->stack.size = 0;
	free(state->stack.elements);
	state->stack.elements = NULL;
	state->stack.capacity = 0;
	
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

void
Zeitgeist_State_update
	(
		Zeitgeist_State* state
	)
{
	runGc(state);
}

void
Zeitgeist_Stack_push
	(
		Zeitgeist_State* state,
		Zeitgeist_Value* value
	)
{
	if (state->stack.size == state->stack.capacity) {
		size_t oldCapacity = state->stack.capacity;
		size_t newCapacity;
		if (oldCapacity > state->stack.maximalCapacity / 2) {
			// As 
			// oldCapacity * 2 > maximumCapacity if and only if oldCapacity > maximumCapacity / 2
			// holds we cannot double the capacity. Instead, try to saturate the capacity.
			if (oldCapacity == state->stack.maximalCapacity) {
				Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
			} else {
				newCapacity = state->stack.maximalCapacity;
			}
		} else {
			newCapacity = oldCapacity * 2;
		}
		Zeitgeist_Value* newElements = realloc(state->stack.elements, newCapacity * sizeof(Zeitgeist_Value));
		if (!newElements) {
			Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
		}
		state->stack.elements = newElements;
		state->stack.capacity = newCapacity;
	}
	state->stack.elements[state->stack.size++] = *value;
}

void
Zeitgeist_Stack_pushBoolean
	(
		Zeitgeist_State* state,
		Zeitgeist_Boolean booleanValue
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setBoolean(&value, booleanValue);
}

void
Zeitgeist_Stack_pushInteger
	(
		Zeitgeist_State* state,
		Zeitgeist_Integer integerValue
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setInteger(&value, integerValue);
	Zeitgeist_Stack_push(state, &value);
}

void
Zeitgeist_Stack_pushList
	(
		Zeitgeist_State* state,
		Zeitgeist_List* listValue
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setList(&value, listValue);
	Zeitgeist_Stack_push(state, &value);
}

void
Zeitgeist_Stack_pushMap
	(
		Zeitgeist_State* state,
		Zeitgeist_Map* mapValue
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setMap(&value, mapValue);
	Zeitgeist_Stack_push(state, &value);
}

void
Zeitgeist_Stack_pushObject
	(
		Zeitgeist_State* state,
		Zeitgeist_Object* objectValue
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setObject(&value, objectValue);
	Zeitgeist_Stack_push(state, &value);
}

void
Zeitgeist_Stack_pushReal32
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 real32Value
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setReal32(&value, real32Value);
	Zeitgeist_Stack_push(state, &value);
}

void
Zeitgeist_Stack_pushString
	(
		Zeitgeist_State* state,
		Zeitgeist_String* stringValue
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setString(&value, stringValue);
	Zeitgeist_Stack_push(state, &value);
}

void
Zeitgeist_Stack_pushVoid
	(
		Zeitgeist_State* state,
		Zeitgeist_Void voidValue
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setVoid(&value, voidValue);
	Zeitgeist_Stack_push(state, &value);
}

void
Zeitgeist_Stack_pop
	(
		Zeitgeist_State* state
	)
{
	state->stack.size--;
}
