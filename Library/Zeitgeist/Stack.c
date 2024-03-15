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
		Zeitgeist_Value_visit(state, state->stack->elements + i);
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

void
Zeitgeist_Stack_pop
	(
		Zeitgeist_State* state
	)
{
	state->stack->size--;
}
