// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/Map.h"

#include "Zeitgeist.h"

// malloc, free
#include <malloc.h>

static size_t g_maximumCapacity = 0;

static bool g_initialized = false;

void Zeitgeist_MapModule_startup() {
	if (!g_initialized) {
		g_maximumCapacity = SIZE_MAX / sizeof(Zeitgeist_Map_Node*);
		if (g_maximumCapacity > Zeitgeist_Integer_Maximum) {
			g_maximumCapacity = Zeitgeist_Integer_Maximum;
		}
		g_initialized = true;
	}
}

Zeitgeist_Map*
Zeitgeist_State_createMap
	(
		Zeitgeist_State* state
	)
{
	Zeitgeist_Map* map = malloc(sizeof(Zeitgeist_Map));
	if (!map) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}

	map->buckets = malloc(8 * sizeof(Zeitgeist_Map_Node*));
	if (!map->buckets) {
		free(map);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	for (size_t i = 0, n = 8; i < n; ++i) {
		map->buckets[i] = NULL;
	}
	map->size = 0;
	map->capacity = 8;

	((Zeitgeist_Gc_Object*)map)->typeTag = Zeitgeist_Gc_TypeTag_Map;
	((Zeitgeist_Gc_Object*)map)->next = state->gc.all;
	state->gc.all = (Zeitgeist_Gc_Object*)map;

	return map;
}