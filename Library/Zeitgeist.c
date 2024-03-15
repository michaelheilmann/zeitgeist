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

#include "Zeitgeist/Locks.h"
#include "Zeitgeist/Stack.h"
#include "Zeitgeist/WeakReferences.h"

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
		Zeitgeist_State* state,
		size_t* live,
		size_t* dead
	);

static void
runGc
	(
		Zeitgeist_State* state,
		size_t *live,
		size_t* dead
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
	
	state->stack = NULL;
	state->locks = NULL;
	state->weakReferences = NULL;
	
	state->strings = NULL;
	state->gc.all = NULL;
	state->gc.gray = NULL;
	state->jumpTarget = NULL;

	Zeitgeist_JumpTarget jumpTarget;

	Zeitgeist_State_pushJumpTarget(state,&jumpTarget);
	if (!setjmp(state->jumpTarget->environment)) {
		Stack_initialize(state);
		Zeitgeist_State_popJumpTarget(state);
	} else {
		Zeitgeist_State_popJumpTarget(state);
		free(state);
		return NULL;
	}

	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(state->jumpTarget->environment)) {
		Locks_initialize(state);
		Zeitgeist_State_popJumpTarget(state);
	} else {
		Zeitgeist_State_popJumpTarget(state);
		Stack_uninitialize(state);
		free(state);
		return NULL;
	}

	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(state->jumpTarget->environment)) {
		WeakReferences_initialize(state);
		Zeitgeist_State_popJumpTarget(state);
	} else {
		Zeitgeist_State_popJumpTarget(state);
		Locks_uninitialize(state);
		Stack_uninitialize(state);
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
			WeakReferences_notifyDestroy(state, object);
			Zeitgeist_List_finalize(state, (Zeitgeist_List*)object);
			free(object);
		} break;
		case Zeitgeist_Gc_TypeTag_Map: {
			WeakReferences_notifyDestroy(state, object);
			Zeitgeist_Map_finalize(state, (Zeitgeist_Map*)object);
			free(object);
		} break;
		case Zeitgeist_Gc_TypeTag_Object: {
			WeakReferences_notifyDestroy(state, object);
			Zeitgeist_Object_finalize(state, (Zeitgeist_Object*)object);
			free(object);
		} break;
		case Zeitgeist_Gc_TypeTag_String: {
			WeakReferences_notifyDestroy(state, object);
			Zeitgeist_String_finalize(state, (Zeitgeist_String*)object);
			free(object);
		} break;
		case Zeitgeist_Gc_TypeTag_WeakReference: {
			WeakReferences_notifyDestroy(state, object);
			Zeitgeist_WeakReference_finalize(state, (Zeitgeist_WeakReference*)object);
			free(object);
		} break;
	}
}

static void
premark
	(
		Zeitgeist_State* state
	)
{
	// Premark locks.
	if (state->locks) {
		Locks_premark(state);
	}
	// Premark stack.
	if (state->stack) {
		Stack_premark(state);
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
			case Zeitgeist_Gc_TypeTag_String:
			case Zeitgeist_Gc_TypeTag_WeakReference:
			default: {
				fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
				exit(EXIT_FAILURE);
			} break;
		};
		Zeitgeist_Gc_Object_setBlack(object);
	}
}

static void
sweep
	(
		Zeitgeist_State* state,
		size_t *live,
		size_t *dead
	)
{
	size_t live_ = 0,
		     dead_ = 0;
	Zeitgeist_Gc_Object** previous = &state->gc.all;
	Zeitgeist_Gc_Object* current = state->gc.all;
	while (NULL != current) {
		if (Zeitgeist_Gc_Object_isWhite(current)) {
			Zeitgeist_Gc_Object* object = current;
			*previous = current->next;
			current = current->next;
			finalize(state, object);
			dead_++;
		} else {
			Zeitgeist_Gc_Object_setWhite(current);
			previous = &current->next;
			current = current->next;
			live_++;
		}
	}
	*live = live_;
	*dead = dead_;
}

static void
runGc
	(
		Zeitgeist_State* state,
		size_t *live,
		size_t *dead
	)
{
	premark(state);
	mark(state);
	sweep(state, live, dead);
}

void
Zeitgeist_State_destroy
	(
		Zeitgeist_State* state
	)
{
	// Assert stack is empty.
	Stack_preLastGcCheck(state);
	// Assert nothing is locked.
	Locks_preLastGcCheck(state);

	size_t runs = 0,
		     live = 0, dead = 0;
	do {
		runGc(state, &live, &dead);
		runs++;
	} while ((dead > 0 || live > 0) && runs < 5);
	Locks_uninitialize(state);
	WeakReferences_uninitialize(state);
	Stack_uninitialize(state);
	if (state->gc.all) {
		fprintf(stderr, "%s:%d: warning: gc all list not empty\n", __FILE__, __LINE__);
	}
	if (state->gc.gray) {
		fprintf(stderr, "%s:%d: warning: gc gray list not empty\n", __FILE__, __LINE__);
	}
	if (state->strings) {
		fprintf(stderr, "%s:%d: warning: string list not empty\n", __FILE__, __LINE__);
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
	size_t live = 0, dead = 0;
	runGc(state, &live, &dead);
}
