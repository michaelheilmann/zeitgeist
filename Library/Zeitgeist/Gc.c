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

void
Zeitgeist_Gc_Object_setGray
	(
		Zeitgeist_Gc_Object* object
	)
{ object->color = Zeitgeist_Gc_Color_Gray; }

bool
Zeitgeist_Gc_Object_isGray
	(
		Zeitgeist_Gc_Object* object
	)
{ return Zeitgeist_Gc_Color_Gray == object->color; }

#include "Zeitgeist.h"

void
Zeitgeist_Gc_visitForeignObject
	(
		Zeitgeist_State* state,
		Zeitgeist_ForeignObject* foreignObject
	)
{
	if (Zeitgeist_Gc_Object_isWhite((Zeitgeist_Gc_Object*)foreignObject)) {
		foreignObject->gclist = state->gc.gray;
		state->gc.gray = (Zeitgeist_Gc_Object*)foreignObject;
		Zeitgeist_Gc_Object_setGray((Zeitgeist_Gc_Object*)foreignObject);
	}
}

void
Zeitgeist_Gc_visitList
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list
	)
{
	if (Zeitgeist_Gc_Object_isWhite((Zeitgeist_Gc_Object*)list)) {
		list->gclist = state->gc.gray;
		state->gc.gray = (Zeitgeist_Gc_Object*)list;
		Zeitgeist_Gc_Object_setGray((Zeitgeist_Gc_Object*)list);
	}
}

void
Zeitgeist_Gc_visitMap
	(
		Zeitgeist_State* state,
		Zeitgeist_Map* map
	)
{
	if (Zeitgeist_Gc_Object_isWhite((Zeitgeist_Gc_Object*)map)) {
		map->gclist = state->gc.gray;
		state->gc.gray = (Zeitgeist_Gc_Object*)map;
		Zeitgeist_Gc_Object_setGray((Zeitgeist_Gc_Object*)map);
	}
}

void
Zeitgeist_Gc_visitString
	(
		Zeitgeist_State* state,
		Zeitgeist_String* string
	)
{
	Zeitgeist_Gc_Object_setBlack((Zeitgeist_Gc_Object*)string);
}

void
Zeitgeist_Gc_visitValue
	(
		Zeitgeist_State* state,
		Zeitgeist_Value* value
	)
{
	switch (value->tag) {
		case Zeitgeist_ValueTag_Map: {
			Zeitgeist_Gc_visitMap(state, value->mapValue);
		} break;
		case Zeitgeist_ValueTag_ForeignObject: {
			Zeitgeist_Gc_visitForeignObject(state, value->foreignObjectValue);
		} break;
		case Zeitgeist_ValueTag_List: {
			Zeitgeist_Gc_visitList(state, value->listValue);
		} break;
		case Zeitgeist_ValueTag_String: {
			Zeitgeist_Gc_visitString(state, value->stringValue);
		} break;
		case Zeitgeist_ValueTag_WeakReference: {
			Zeitgeist_Gc_visitWeakReference(state, value->weakReferenceValue);
		} break;
	};
}

void
Zeitgeist_Gc_visitWeakReference
	(
		Zeitgeist_State* state,
		Zeitgeist_WeakReference* weakReference
	)
{
	Zeitgeist_Gc_Object_setBlack((Zeitgeist_Gc_Object*)weakReference);
}
