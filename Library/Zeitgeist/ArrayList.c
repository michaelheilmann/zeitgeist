// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/ArrayList.h"

#include "Zeitgeist.h"
#include <malloc.h>

static size_t g_maximumCapacity = 0;

static bool g_initialized = false;

void Zeitgeist_ArrayListModule_startup() {
	if (!g_initialized) {
		g_maximumCapacity = SIZE_MAX / sizeof(Zeitgeist_Value);
		if (g_maximumCapacity > Zeitgeist_Integer_Maximum) {
			g_maximumCapacity = Zeitgeist_Integer_Maximum;
		}
		g_initialized = true;
	}
}

Zeitgeist_ArrayList*
Zeitgeist_createArrayList
	(
		Zeitgeist_State* state
	)
{
	Zeitgeist_ArrayList* arrayList = malloc(sizeof(Zeitgeist_ArrayList));
	if (!arrayList) {
		longjmp(state->jumpTarget->environment, -1);
	}
	arrayList->elements = malloc(8 * sizeof(Zeitgeist_Value));
	if (!arrayList->elements) {
		free(arrayList);
		longjmp(state->jumpTarget->environment, -1);
	}
	arrayList->size = 0;
	arrayList->capacity = 8;
	arrayList->next = state->arrayLists;
	state->arrayLists = arrayList;
	return arrayList;
}

// memmove
#include <string.h>

static Zeitgeist_Value const IndexOutOfBounds = { .tag = Zeitgeist_ValueTag_Void, .voidValue = Zeitgeist_Void_Void };

Zeitgeist_Value
Zeitgeist_ArrayList_getValue
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		size_t index
	)
{
	if (index >= arrayList->size) {
		return IndexOutOfBounds;
	}
	return arrayList->elements[index];
}

Zeitgeist_Value
Zeitgeist_ArrayList_getSize
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setInteger(&value, (Zeitgeist_Integer)arrayList->size);
	return value;
}

void
Zeitgeist_ArrayList_insertValue
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		size_t index,
		Zeitgeist_Value const* value
	)
{
	if (Zeitgeist_Value_hasVoid(value)) {
		return;
	}
	if (arrayList->capacity == arrayList->size) {
		Zeitgeist_ArrayListModule_startup();
		size_t oldCapacity = arrayList->capacity;
		size_t newCapacity;
		if (oldCapacity > g_maximumCapacity / 2) {
			// as the following fact holds
			// oldCapacity * 2 > maximumCapacity if and only if oldCapacity > maximumCapacity / 2
			// we cannot double the capacity.
			// try to saturate the capacity.
			if (oldCapacity == g_maximumCapacity) {
				longjmp(state->jumpTarget->environment, -1);
			} else {
				newCapacity = g_maximumCapacity;
			}
		} else {
			newCapacity = oldCapacity * 2;
		}
		Zeitgeist_Value* newElements = realloc(arrayList->elements, newCapacity * sizeof(Zeitgeist_Value));
		if (!newElements) {
			longjmp(state->jumpTarget->environment, -1);
		}
		arrayList->elements = newElements;
		arrayList->capacity = newCapacity;
	}
	if (index < arrayList->size) {
		memmove(arrayList->elements + index,
						arrayList->elements + index + 1,
						sizeof(Zeitgeist_Value) * (arrayList->size - index));
	}
	arrayList->elements[index] = *value;
	arrayList->size++;
}

void
Zeitgeist_ArrayList_appendValue
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		Zeitgeist_Value const* value
	)
{
	Zeitgeist_ArrayList_insertValue(state, arrayList, arrayList->size, value);
}

void
Zeitgeist_ArrayList_prependValue
	( 
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		Zeitgeist_Value const* value
	)
{
	Zeitgeist_ArrayList_insertValue(state, arrayList, 0, value);
}
