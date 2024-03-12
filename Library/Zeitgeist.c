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

	state->stack.elements = malloc(sizeof(Zeitgeist_Value) * 8);
	if (!state->stack.elements) {
		free(state);
		return NULL;
	}

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
				Zeitgeist_List_visit(state, (Zeitgeist_List*)object);
			} break;
			case Zeitgeist_Gc_TypeTag_Map: {
				state->gc.gray = ((Zeitgeist_Map*)object)->gclist;
				Zeitgeist_Map_visit(state, (Zeitgeist_Map*)object);
			} break;
			case Zeitgeist_Gc_TypeTag_Object: {
				state->gc.gray = ((Zeitgeist_Object*)object)->gclist;
				Zeitgeist_Object_visit(state, (Zeitgeist_Object*)object);
			} break;
			case Zeitgeist_Gc_TypeTag_String:
			default: {
				fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
				exit(EXIT_FAILURE);
			} break;
		};
		Zeitgeist_Gc_Object_isBlack(object);
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
premark
	(
		Zeitgeist_State* state
	)
{ 

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
	if (state->gc.all) {
		fprintf(stderr, "%s:%d: warning: gc all list not empty\n", __FILE__, __LINE__);
	}
	if (state->gc.gray) {
		fprintf(stderr, "%s:%d: warning: gc gray list not empty\n", __FILE__, __LINE__);
	}
	if (state->strings) {
		fprintf(stderr, "%s:%d: warning: string list not empty\n", __FILE__, __LINE__);
	}
}

void
Zeitgeist_State_destroy
	(
		Zeitgeist_State* state
	)
{
	runGc(state);

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
	/*runGc(state);*/
}

void
Zeitgeist_Stack_push
	(
		Zeitgeist_State* state,
		Zeitgeist_Value* value
	)
{
}

void
Zeitgeist_Stack_pushBoolean
	(
		Zeitgeist_State* state,
		Zeitgeist_Boolean booleanValue
	)
{ 
}

void
Zeitgeist_Stack_pushInteger
	(
		Zeitgeist_State* state,
		Zeitgeist_Integer integerValue
	)
{
}

void
Zeitgeist_Stack_pushList
	(
		Zeitgeist_State* state,
		Zeitgeist_List* listValue
	)
{
}

void
Zeitgeist_Stack_pushMap
	(
		Zeitgeist_State* state,
		Zeitgeist_Map* mapValue
	)
{
}

void
Zeitgeist_Stack_pushObject
	(
		Zeitgeist_State* state,
		Zeitgeist_Object* objectValue
	)
{
}

void
Zeitgeist_Stack_pushReal32
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 real32Value
	)
{
}

void
Zeitgeist_Stack_pushString
	(
		Zeitgeist_State* state,
		Zeitgeist_String* stringValue
	)
{
}

void
Zeitgeist_Stack_pushVoid
	(
		Zeitgeist_State* state,
		Zeitgeist_Void voidValue
	)
{
}

void
Zeitgeist_Stack_pop
	(
		Zeitgeist_State* state
	)
{
}
