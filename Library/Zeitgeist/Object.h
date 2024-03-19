// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_OBJECT_H_INCLUDED)
#define ZEITGEIST_OBJECT_H_INCLUDED

#include "Zeitgeist/Gc.h"
#include "Zeitgeist/Value.h"

// size_t
#include <stddef.h>

struct Zeitgeist_Object {
	Zeitgeist_Gc_Object _parent;
	/** For being added to the "gray" list during the "mark" phase of the garbage collector. */
	Zeitgeist_Gc_Object* gclist;
	void (*visit)(Zeitgeist_State* state, Zeitgeist_Object* object);
	void (*finalize)(Zeitgeist_State* state, Zeitgeist_Object* object);
};

/**
 * @since 0.1
 * @brief Allocate an object of the specified size.
 * The object is managed by the GC.
 * @undefined size is smaller than sizeof(Zeitgeist_Object).
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined finalize is not null and does not point to a finalize function.
 * @undefined visit is not null and does not point to a visit function.
 * @warning
 * Specified finalize or visit callbacks will be invoked regardless of if the fields of the object are properly initialized.
 * It is recommended not to set those via this function.
 */
void*
Zeitgeist_allocateObject
	(
		Zeitgeist_State* state,
		size_t size,
		void (*finalize)(Zeitgeist_State*, Zeitgeist_Object*),
		void (*visit)(Zeitgeist_State*, Zeitgeist_Object*)
	);

void
Zeitgeist_Object_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_Object* object
	);

void
Zeitgeist_Object_finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_Object* object
	);

#endif // ZEITGEIST_OBJECT_H_INCLUDED
