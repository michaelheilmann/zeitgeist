#include "Zeitgeist/Locks.h"

// malloc, free
#include <malloc.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

// fprintf, stderr
#include <stdio.h>

/// @unmanaged
typedef struct LockNode LockNode;

struct LockNode {
	LockNode* next;
	size_t hashValue;
	Zeitgeist_Gc_Object* object;
};

struct Locks {
	LockNode* free;
	LockNode** buckets;
	size_t size;
	size_t capacity;
	size_t maximalCapacity;
};

void
Locks_premark
	(
		Zeitgeist_State* state
	)
{
	// Premark locks.
	for (size_t i = 0, n = state->locks->capacity; i < n; ++i) {
		LockNode* lockNode = state->locks->buckets[i];
		while (lockNode) {
			Zeitgeist_Gc_Object* object = lockNode->object;
			switch (object->typeTag) {
				case Zeitgeist_Gc_TypeTag_List: {
					if (Zeitgeist_Gc_Object_isWhite(object)) {
						((Zeitgeist_List*)object)->gclist = state->gc.gray;
						state->gc.gray = object;
						Zeitgeist_Gc_Object_setGray(object);
					}
				} break;
				case Zeitgeist_Gc_TypeTag_Map: {
					if (Zeitgeist_Gc_Object_isWhite(object)) {
						((Zeitgeist_Map*)object)->gclist = state->gc.gray;
						state->gc.gray = object;
						Zeitgeist_Gc_Object_setGray(object);
					}
				} break;
				case Zeitgeist_Gc_TypeTag_Object: {
					if (Zeitgeist_Gc_Object_isWhite(object)) {
						((Zeitgeist_Object*)object)->gclist = state->gc.gray;
						state->gc.gray = object;
						Zeitgeist_Gc_Object_setGray(object);
					}
				} break;
				case Zeitgeist_Gc_TypeTag_String: {
					Zeitgeist_Gc_Object_setBlack(object);
				} break;
				default: {
					fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
					exit(EXIT_FAILURE);
				} break;
			};
			lockNode = lockNode->next;
		}
	}
}

void
Locks_initialize
	(
		Zeitgeist_State* state
	)
{
	state->locks = malloc(sizeof(Locks));
	if (!state->locks) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	state->locks->maximalCapacity = SIZE_MAX / sizeof(LockNode*);
	if (state->locks->maximalCapacity > Zeitgeist_Integer_Maximum) {
		state->locks->maximalCapacity = Zeitgeist_Integer_Maximum;
	}
	state->locks->capacity = 8;
	if (state->locks->capacity > state->locks->maximalCapacity) {
		free(state->locks);
		state->locks = NULL;
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	state->locks->buckets = malloc(sizeof(LockNode*) * state->locks->capacity);
	if (!state->locks->buckets) {
		free(state->locks);
		state->locks = NULL;
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	for (size_t i = 0, n = state->locks->capacity; i < n; ++i) {
		state->locks->buckets[i] = NULL;
	}
	state->locks->size = 0;
}

void
Locks_uninitialize
	(
		Zeitgeist_State* state
	)
{
	for (size_t i = 0, n = state->locks->capacity; i < n; ++i) {
		LockNode** bucket = &(state->locks->buckets[i]);
		while (*bucket) {
			LockNode* node = *bucket;
			*bucket = (*bucket)->next;
			free(node);
		}
	}
	free(state->locks);
	state->locks = NULL;
}
