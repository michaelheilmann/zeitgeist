// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/Map.h"

#include "Zeitgeist.h"

// fprintf, stderr
#include <stdio.h>

// malloc, free
#include <malloc.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

static size_t g_maximumCapacity = 0;

static bool g_initialized = false;

void
Zeitgeist_MapModule_startup
	(
	)
{
	if (!g_initialized) {
		g_maximumCapacity = SIZE_MAX / sizeof(Zeitgeist_Map_Node*);
		if (g_maximumCapacity > Zeitgeist_Integer_Maximum) {
			g_maximumCapacity = Zeitgeist_Integer_Maximum;
		}
		g_initialized = true;
	}
}

void
Zeitgeist_Map_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_Map* map
	)
{ 
	for (size_t i = 0, n = map->capacity; i < n; ++i) {
		Zeitgeist_Map_Node* node = map->buckets[i];
		while (node) {
			Zeitgeist_Gc_visitValue(state, &node->key);
			Zeitgeist_Gc_visitValue(state, &node->value);
			node = node->next;
		}
	}
}

void
Zeitgeist_Map_finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_Map* map
	)
{
	for (size_t i = 0, n = map->capacity; i < n; ++i) {
		Zeitgeist_Map_Node** bucket = &(map->buckets[i]);
		while (*bucket) {
			Zeitgeist_Map_Node* node = *bucket;
			*bucket = node->next;
			free(node);
		}
	}
	free(map->buckets);
}

Zeitgeist_Map*
Zeitgeist_Map_create
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
	((Zeitgeist_Gc_Object*)map)->color = Zeitgeist_Gc_Color_White;
	map->gclist = NULL;

	return map;
}
