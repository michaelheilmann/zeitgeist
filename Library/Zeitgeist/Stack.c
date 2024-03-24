// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/Stack.h"

// malloc, free
#include <malloc.h>

// fprintf, stderr
#include <stdio.h>

void
Stack_preLastGcCheck
	(
		Zeitgeist_State* state
	)
{
	// Perfrom a sanity check before the last gargabe collection is performed before shutdown.
	if (state->stack->size > 0) {
		fprintf(stderr, "%s:%d: warning: stack is not empty\n", __FILE__, __LINE__);
	}
}

void
Stack_premark
	(
		Zeitgeist_State* state
	)
{
	// Premark stack.
	for (size_t i = 0, n = state->stack->size; i < n; ++i) {
		Zeitgeist_Gc_visitValue(state, state->stack->elements + i);
	}
}

void
Stack_initialize
	(
		Zeitgeist_State* state
	)
{
	state->stack = malloc(sizeof(Stack));
	if (!state->stack) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	state->stack->maximalCapacity = SIZE_MAX / sizeof(Zeitgeist_Value);
	if (state->stack->maximalCapacity > Zeitgeist_Integer_Maximum) {
		state->stack->maximalCapacity = Zeitgeist_Integer_Maximum;
	}
	state->stack->capacity = 8;
	if (state->stack->capacity > state->stack->maximalCapacity) {
		free(state->stack);
		state->stack = NULL;
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	state->stack->elements = malloc(sizeof(Zeitgeist_Value) * state->stack->capacity);
	if (!state->stack->elements) {
		free(state->stack);
		state->stack = NULL;
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	state->stack->capacity = 8;
	state->stack->size = 0;
}

void
Stack_uninitialize
	(
		Zeitgeist_State* state
	)
{
	state->stack->size = 0;
	free(state->stack->elements);
	state->stack->elements = NULL;
	state->stack->capacity = 0;
	free(state->stack);
	state->stack = NULL;
}

size_t
Zeitgeist_Stack_getSize
	(
		Zeitgeist_State* state
	)
{ return state->stack->size; }

void
Zeitgeist_Stack_push
	(
		Zeitgeist_State* state,
		Zeitgeist_Value* value
	)
{
	if (state->stack->size == state->stack->capacity) {
		size_t oldCapacity = state->stack->capacity;
		size_t newCapacity;
		if (oldCapacity > state->stack->maximalCapacity / 2) {
			// As 
			// oldCapacity * 2 > maximumCapacity if and only if oldCapacity > maximumCapacity / 2
			// holds we cannot double the capacity. Instead, try to saturate the capacity.
			if (oldCapacity == state->stack->maximalCapacity) {
				Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
			} else {
				newCapacity = state->stack->maximalCapacity;
			}
		} else {
			newCapacity = oldCapacity * 2;
		}
		Zeitgeist_Value* newElements = realloc(state->stack->elements, newCapacity * sizeof(Zeitgeist_Value));
		if (!newElements) {
			Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
		}
		state->stack->elements = newElements;
		state->stack->capacity = newCapacity;
	}
	state->stack->elements[state->stack->size++] = *value;
}

void
Zeitgeist_Stack_pop
	(
		Zeitgeist_State* state
	)
{ state->stack->size--; }

void
Zeitgeist_Stack_pushBoolean
	(
		Zeitgeist_State* state,
		Zeitgeist_Boolean booleanValue
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setBoolean(&value, booleanValue);
	Zeitgeist_Stack_push(state, &value);
}

bool
Zeitgeist_Stack_isBoolean
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_hasBoolean(state->stack->elements + state->stack->size - 1 - index); }

Zeitgeist_Boolean
Zeitgeist_Stack_getBoolean
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_getBoolean(state->stack->elements + state->stack->size - 1 - index); }

void
Zeitgeist_Stack_pushForeignFunction
	(
		Zeitgeist_State* state,
		Zeitgeist_ForeignFunction* foreignFunctionValue
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setForeignFunction(&value, foreignFunctionValue);
	Zeitgeist_Stack_push(state, &value);
}

bool
Zeitgeist_Stack_isForeignFunction
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_hasForeignFunction(state->stack->elements + state->stack->size - 1 - index); }

Zeitgeist_ForeignFunction*
Zeitgeist_Stack_getForeignFunction
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_getForeignFunction(state->stack->elements + state->stack->size - 1 - index); }

void
Zeitgeist_Stack_pushForeignObject
	(
		Zeitgeist_State* state,
		Zeitgeist_ForeignObject* foreignObjectValue
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setForeignObject(&value, foreignObjectValue);
	Zeitgeist_Stack_push(state, &value);
}

bool
Zeitgeist_Stack_isForeignObject
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_hasForeignObject(state->stack->elements + state->stack->size - 1 - index); }

Zeitgeist_ForeignObject*
Zeitgeist_Stack_getForeignObject
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_getForeignObject(state->stack->elements + state->stack->size - 1 - index); }

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

bool
Zeitgeist_Stack_isInteger
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_hasInteger(state->stack->elements + state->stack->size - 1 + index); }

Zeitgeist_Integer
Zeitgeist_Stack_getInteger
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_getInteger(state->stack->elements + state->stack->size - 1 + index); }

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

bool
Zeitgeist_Stack_isList
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_hasList(state->stack->elements + state->stack->size - 1 + index); }

Zeitgeist_List*
Zeitgeist_Stack_getList
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_getList(state->stack->elements + state->stack->size - 1 + index); }

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

bool
Zeitgeist_Stack_isMap
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_hasMap(state->stack->elements + state->stack->size - 1 - index); }

Zeitgeist_Map*
Zeitgeist_Stack_getMap
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_getMap(state->stack->elements + state->stack->size - 1 + index); }

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

bool
Zeitgeist_Stack_isReal32
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_hasReal32(state->stack->elements + state->stack->size - 1 - index); }

Zeitgeist_Real32
Zeitgeist_Stack_getReal32
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_getReal32(state->stack->elements + state->stack->size - 1 + index); }

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

bool
Zeitgeist_Stack_isString
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_hasString(state->stack->elements + state->stack->size - 1 - index); }

Zeitgeist_String*
Zeitgeist_Stack_getString
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_getString(state->stack->elements + state->stack->size - 1 + index); }

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

bool
Zeitgeist_Stack_isVoid
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_hasVoid(state->stack->elements + state->stack->size - 1 - index); }

Zeitgeist_Void
Zeitgeist_Stack_getVoid
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_getVoid(state->stack->elements + state->stack->size - 1 + index); }

void
Zeitgeist_Stack_pushWeakReference
	(
		Zeitgeist_State* state,
		Zeitgeist_WeakReference* weakReference
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setWeakReference(&value, weakReference);
	Zeitgeist_Stack_push(state, &value);
}

bool
Zeitgeist_Stack_isWeakReference
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_hasWeakReference(state->stack->elements + state->stack->size - 1 - index); }

Zeitgeist_WeakReference*
Zeitgeist_Stack_getWeakReference
	(
		Zeitgeist_State* state,
		size_t index
	)
{ return Zeitgeist_Value_getWeakReference(state->stack->elements + state->stack->size - 1 + index); }
