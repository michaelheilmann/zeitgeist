// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/Gc.h"

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

void
Zeitgeist_Gc_Object_setBlack
	(
		Zeitgeist_Gc_Object* object
	)
{ object->color = Zeitgeist_Gc_Color_Black; }

bool
Zeitgeist_Gc_Object_isBlack
	(
		Zeitgeist_Gc_Object* object
	)
{ return Zeitgeist_Gc_Color_Black == object->color; }

void
Zeitgeist_Gc_Object_setWhite
	(
		Zeitgeist_Gc_Object* object
	)
{ object->color = Zeitgeist_Gc_Color_White; }

bool
Zeitgeist_Gc_Object_isWhite
	(
		Zeitgeist_Gc_Object* object
	)
{ return Zeitgeist_Gc_Color_White == object->color; }
