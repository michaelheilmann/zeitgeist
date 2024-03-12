// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_OBJECT_H_INCLUDED)
#define ZEITGEIST_OBJECT_H_INCLUDED

#include "Zeitgeist/Gc.h"
#include "Zeitgeist/Value.h"

struct Zeitgeist_Object {
	Zeitgeist_Gc_Object _parent;
	// certain objects (like Zeitgeist_Object) are considered as having too many references to traverse them in a single step.
	// they are added to the gray list.
	Zeitgeist_Gc_Object* gclist;
#if 0
	size_t(*hash)(Zeitgeist_State* state, Zeitgeist_Object* object);
#endif
	void (*visit)(Zeitgeist_State* state, Zeitgeist_Object* object);
	void (*finalize)(Zeitgeist_State* state, Zeitgeist_Object* object);
};

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
