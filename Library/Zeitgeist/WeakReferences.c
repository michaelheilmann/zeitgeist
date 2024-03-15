#include "Zeitgeist/WeakReferences.h"

// malloc, free
#include <malloc.h>

/// @ingroup state
typedef struct WeakReferenceNode WeakReferenceNode;

struct WeakReferenceNode {
	WeakReferenceNode* next;
	/** @brief The address of the managed object. */
	void* address;
	/** @brief The hash value of the address of the managed object. */
	size_t hashValue;
	/** @brief A pointer to the weak reference. */
	Zeitgeist_WeakReference* weakReference;
};

struct WeakReferences {
	WeakReferenceNode* free;
	WeakReferenceNode** buckets;
	size_t size;
	size_t capacity;
	size_t maximalCapacity;
};

static size_t hashPointer(void* p) {
	return (size_t)(uintptr_t)p;
}

void
WeakReferences_initialize
	(
		Zeitgeist_State* state
	)
{
	state->weakReferences = malloc(sizeof(WeakReferences));
	if (!state->weakReferences) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	state->weakReferences->maximalCapacity = SIZE_MAX / sizeof(WeakReferenceNode*);
	if (state->weakReferences->maximalCapacity > Zeitgeist_Integer_Maximum) {
		state->weakReferences->maximalCapacity = Zeitgeist_Integer_Maximum;
	}
	state->weakReferences->capacity = 8;
	if (state->weakReferences->capacity > state->weakReferences->maximalCapacity) {
		free(state->weakReferences);
		state->weakReferences = NULL;
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	state->weakReferences->buckets = malloc(sizeof(WeakReferenceNode*) * state->weakReferences->capacity);
	if (!state->weakReferences->buckets) {
		free(state->weakReferences);
		state->weakReferences = NULL;
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	for (size_t i = 0, n = state->weakReferences->capacity; i < n; ++i) {
		state->weakReferences->buckets[i] = NULL;
	}
	state->weakReferences->capacity = 8;
	state->weakReferences->size = 0;
}

void
WeakReferences_uninitialize
	(
		Zeitgeist_State* state
	)
{
	for (size_t i = 0, n = state->weakReferences->capacity; i < n; ++i) {
		WeakReferenceNode** bucket = &(state->weakReferences->buckets[i]);
		while (*bucket) {
			WeakReferenceNode* node = *bucket;
			*bucket = (*bucket)->next;
			free(node);
		}
	}
	free(state->weakReferences);
	state->weakReferences = NULL;
}

void
Zeitgeist_WeakReference_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_WeakReference* weakReference
	)
{/*intentionally empty*/}

void
Zeitgeist_WeakReference_finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_WeakReference* weakReference
	)
{
	size_t hashValue = hashPointer(weakReference->reference);
	size_t hashIndex = hashValue % state->weakReferences->capacity;
	WeakReferenceNode** previous = &(state->weakReferences->buckets[hashIndex]);
	WeakReferenceNode* current = state->weakReferences->buckets[hashIndex];
	while (current) {
		if (current->weakReference == weakReference) {
			*previous = current->next;
			state->weakReferences->size--;
			break;
		} else {
			previous = &current->next;
			current = current->next;
		}
	}
}

void
WeakReferences_notifyDestroy
	(
		Zeitgeist_State* state,
		void* address
	)
{
	size_t hashValue = hashPointer(address);
	size_t hashIndex = hashValue % state->weakReferences->capacity;
	WeakReferenceNode* current = state->weakReferences->buckets[hashIndex];
	while (current) {
		if (current->address == address) {
			current->address = NULL;
			break;
		} else {
			current = current->next;
		}
	}
}

Zeitgeist_WeakReference*
Zeitgeist_WeakReference_create
	(
		Zeitgeist_State* state,
		Zeitgeist_Gc_Object* reference
	)
{
	Zeitgeist_WeakReference* self = malloc(sizeof(Zeitgeist_WeakReference));
	if (!self) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}

	((Zeitgeist_Gc_Object*)self)->typeTag = Zeitgeist_Gc_TypeTag_WeakReference;
	((Zeitgeist_Gc_Object*)self)->next = state->gc.all;
	state->gc.all = (Zeitgeist_Gc_Object*)self;
	((Zeitgeist_Gc_Object*)self)->color = Zeitgeist_Gc_Color_White;

	return self;
}
