// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/List.h"

#include "Zeitgeist.h"

// malloc, realloc, free
#include <malloc.h>

// memmove
#include <string.h>

static size_t g_maximumCapacity = 0;

static bool g_initialized = false;

void
Zeitgeist_ListModule_startup
	(
	)
{
	if (!g_initialized) {
		g_maximumCapacity = SIZE_MAX / sizeof(Zeitgeist_Value);
		if (g_maximumCapacity > Zeitgeist_Integer_Maximum) {
			g_maximumCapacity = Zeitgeist_Integer_Maximum;
		}
		g_initialized = true;
	}
}

void
Zeitgeist_List_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list
	)
{
	for (size_t i = 0, n = list->size; i < n; ++i) {
		Zeitgeist_Gc_visitValue(state, list->elements + i);
	}
}

void
Zeitgeist_List_finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list
	)
{
	list->size = 0;
	free(list->elements);
	list->elements = NULL;
	list->capacity = 0;
}

Zeitgeist_List*
Zeitgeist_List_create
	(
		Zeitgeist_State* state
	)
{
	Zeitgeist_List* list = malloc(sizeof(Zeitgeist_List));
	if (!list) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}

	list->elements = malloc(8 * sizeof(Zeitgeist_Value));
	if (!list->elements) {
		free(list);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	list->size = 0;
	list->capacity = 8;

	((Zeitgeist_Gc_Object*)list)->typeTag = Zeitgeist_Gc_TypeTag_List;
	((Zeitgeist_Gc_Object*)list)->next = state->gc.all;
	state->gc.all = (Zeitgeist_Gc_Object*)list;
	((Zeitgeist_Gc_Object*)list)->color = Zeitgeist_Gc_Color_White;
	list->gclist = NULL;
	
	return list;
}

static Zeitgeist_Value const IndexOutOfBounds = { .tag = Zeitgeist_ValueTag_Void, .voidValue = Zeitgeist_Void_Void };

Zeitgeist_Value
Zeitgeist_List_getValue
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		size_t index
	)
{
	if (index >= list->size) {
		return IndexOutOfBounds;
	}
	return list->elements[index];
}

Zeitgeist_Value
Zeitgeist_List_getSize
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setInteger(&value, (Zeitgeist_Integer)list->size);
	return value;
}

void
Zeitgeist_List_insertValue
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		size_t index,
		Zeitgeist_Value const* value
	)
{
	if (Zeitgeist_Value_hasVoid(value)) {
		return;
	}
	if (list->capacity == list->size) {
		Zeitgeist_ListModule_startup();
		size_t oldCapacity = list->capacity;
		size_t newCapacity;
		if (oldCapacity > g_maximumCapacity / 2) {
			// as the following fact holds
			// oldCapacity * 2 > maximumCapacity if and only if oldCapacity > maximumCapacity / 2
			// we cannot double the capacity.
			// try to saturate the capacity.
			if (oldCapacity == g_maximumCapacity) {
				Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
			} else {
				newCapacity = g_maximumCapacity;
			}
		} else {
			newCapacity = oldCapacity * 2;
		}
		Zeitgeist_Value* newElements = realloc(list->elements, newCapacity * sizeof(Zeitgeist_Value));
		if (!newElements) {
			Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
		}
		list->elements = newElements;
		list->capacity = newCapacity;
	}
	if (index < list->size) {
		memmove(list->elements + index,
						list->elements + index + 1,
						sizeof(Zeitgeist_Value) * (list->size - index));
	}
	list->elements[index] = *value;
	list->size++;
}

void
Zeitgeist_List_appendValue
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		Zeitgeist_Value const* value
	)
{
	Zeitgeist_List_insertValue(state, list, list->size, value);
}

void
Zeitgeist_List_prependValue
	( 
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		Zeitgeist_Value const* value
	)
{
	Zeitgeist_List_insertValue(state, list, 0, value);
}
