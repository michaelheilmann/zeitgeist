// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/Object.h"

void
Zeitgeist_Object_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_Object* object
	)
{
	if (object->visit) {
		object->visit(state, object);
	}
}

void
Zeitgeist_Object_finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_Object* object
	)
{
	if (object->finalize) {
		object->finalize(state, object);
	}
}
