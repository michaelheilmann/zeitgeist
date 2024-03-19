// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/Object.h"

#include "Zeitgeist.h"

// fprintf, stderr
#include <stdio.h>

// malloc
#include <malloc.h>

void*
Zeitgeist_allocateForeignObject
	(
		Zeitgeist_State* state,
		size_t size,
		void (*finalize)(Zeitgeist_State*, Zeitgeist_ForeignObject*),
		void (*visit)(Zeitgeist_State*, Zeitgeist_ForeignObject*)
	)
{
	Zeitgeist_ForeignObject* self = malloc(size);
	if (!self) {
		fprintf(stderr, "%s:%d: unable to allocate %zu Bytes\n", __FILE__, __LINE__, size);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}

	((Zeitgeist_ForeignObject*)self)->finalize = finalize;
	((Zeitgeist_ForeignObject*)self)->visit = visit;

	// This is currently a constant as the incremental GC is not yet implemented.
	static const bool isGcRunning = false;

	// Add the object to the "all" list.
	((Zeitgeist_Gc_Object*)self)->next = state->gc.all;
	state->gc.all = (Zeitgeist_Gc_Object*)self;

	// Set its type tag to "foreign object".
	((Zeitgeist_Gc_Object*)self)->typeTag = Zeitgeist_Gc_TypeTag_ForeignObject;

	if (isGcRunning) {
		// If the GC is running, color the object "gray" and add it to the "gray" list.
		((Zeitgeist_Gc_Object*)self)->color = Zeitgeist_Gc_Color_Gray;
		self->gclist = state->gc.gray;
		state->gc.gray = (Zeitgeist_Gc_Object*)self;
	} else {
		// If the GC is not running, color the object "white".
		((Zeitgeist_Gc_Object*)self)->color = Zeitgeist_Gc_Color_White;
		self->gclist = NULL;
	}
	return self;
}

void
Zeitgeist_ForeignObject_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_ForeignObject* foreignObject
	)
{
	if (foreignObject->visit) {
		foreignObject->visit(state, foreignObject);
	}
}

void
Zeitgeist_ForeignObject_finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_ForeignObject* foreignObject
	)
{
	if (foreignObject->finalize) {
		foreignObject->finalize(state, foreignObject);
	}
}
