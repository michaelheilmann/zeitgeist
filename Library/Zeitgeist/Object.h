// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_FOREIGNOBJECT_H_INCLUDED)
#define ZEITGEIST_FOREIGNOBJECT_H_INCLUDED

#include <stddef.h>
#include "Zeitgeist/Gc.h"
#include "Zeitgeist/Value.h"

struct Zeitgeist_ForeignObject {
	Zeitgeist_Gc_Object _parent;
	/** For being added to the "gray" list during the "mark" phase of the garbage collector. */
	Zeitgeist_Gc_Object* gclist;
	void (*visit)(Zeitgeist_State* state, Zeitgeist_ForeignObject* foreignObject);
	void (*finalize)(Zeitgeist_State* state, Zeitgeist_ForeignObject* foreignObject);
};

/**
 * @since 0.1
 * @brief Allocate an object of the specified size.
 * The object is managed by the GC.
 * @undefined size is smaller than sizeof(Zeitgeist_ForeignObject).
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined finalize is not null and does not point to a finalize function.
 * @undefined visit is not null and does not point to a visit function.
 * @warning
 * Specified finalize or visit callbacks will be invoked regardless of if the fields of the object are properly initialized.
 * It is recommended not to set those via this function.
 */
void*
Zeitgeist_allocateForeignObject
	(
		Zeitgeist_State* state,
		size_t size,
		void (*finalize)(Zeitgeist_State*, Zeitgeist_ForeignObject*),
		void (*visit)(Zeitgeist_State*, Zeitgeist_ForeignObject*)
	);

void
Zeitgeist_ForeignObject_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_ForeignObject* foreignObject
	);

void
Zeitgeist_ForeignObject_finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_ForeignObject* foreignObject
	);

#endif // ZEITGEIST_FOREIGNOBJECT_H_INCLUDED
