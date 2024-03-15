// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/Value.h"

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

#include "Zeitgeist.h"
#include "Zeitgeist/Map.h"
#include "Zeitgeist/Gc.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void
Zeitgeist_Value_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_Value* value
	)
{
	switch (value->tag) {
		case Zeitgeist_ValueTag_Map: {
			if (Zeitgeist_Gc_Object_isWhite((Zeitgeist_Gc_Object*)value->mapValue)) {
				value->mapValue->gclist = state->gc.gray;
				state->gc.gray = (Zeitgeist_Gc_Object*)value->mapValue;
				Zeitgeist_Gc_Object_setGray((Zeitgeist_Gc_Object*)value->mapValue);
			}
		} break;
		case Zeitgeist_ValueTag_List: {
			if (Zeitgeist_Gc_Object_isWhite((Zeitgeist_Gc_Object*)value->listValue)) {
				value->listValue->gclist = state->gc.gray;
				state->gc.gray = (Zeitgeist_Gc_Object*)value->listValue;
				Zeitgeist_Gc_Object_setGray((Zeitgeist_Gc_Object*)value->listValue);
			}
		} break;
		case Zeitgeist_ValueTag_Object: {
			if (Zeitgeist_Gc_Object_isWhite((Zeitgeist_Gc_Object*)value->objectValue)) {
				value->objectValue->gclist = state->gc.gray;
				state->gc.gray = (Zeitgeist_Gc_Object*)value->objectValue;
				Zeitgeist_Gc_Object_setGray((Zeitgeist_Gc_Object*)value->objectValue);
			}
		} break;
		case Zeitgeist_ValueTag_String: {
			Zeitgeist_Gc_Object_setBlack((Zeitgeist_Gc_Object*)value->stringValue);
		} break;
	};
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool
Zeitgeist_Value_hasBoolean
	(
		Zeitgeist_Value const* value
 )
{ return Zeitgeist_ValueTag_Boolean == value->tag; }

void
Zeitgeist_Value_setBoolean
	(
		Zeitgeist_Value *value,
		Zeitgeist_Boolean booleanValue
	)
{ 
	value->tag = Zeitgeist_ValueTag_Boolean;
	value->booleanValue = booleanValue;
}

Zeitgeist_Boolean
Zeitgeist_Value_getBoolean
	(
		Zeitgeist_Value* value
	)
{ return value->booleanValue; }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool
Zeitgeist_Value_hasForeignFunction
	(
		Zeitgeist_Value const* value
	)
{ return Zeitgeist_ValueTag_ForeignFunction == value->tag; }

void
Zeitgeist_Value_setForeignFunction
	(
		Zeitgeist_Value* value,
		Zeitgeist_ForeignFunction* foreignFunction
	)
{ 
	value->tag = Zeitgeist_ValueTag_ForeignFunction;
	value->foreignFunction = foreignFunction;
}

Zeitgeist_ForeignFunction*
Zeitgeist_Value_getForeignFunction
	(
		Zeitgeist_Value* value
	)
{ return value->foreignFunction; }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool
Zeitgeist_Value_hasInteger
	(
		Zeitgeist_Value const* value
	)
{ return Zeitgeist_ValueTag_Integer == value->tag; } 

void
Zeitgeist_Value_setInteger
	(
		Zeitgeist_Value *value,
		Zeitgeist_Integer integerValue
	)
{
	value->tag = Zeitgeist_ValueTag_Integer;
	value->integerValue = integerValue;
}

Zeitgeist_Integer
Zeitgeist_Value_getInteger
	(
		Zeitgeist_Value const* value
	)
{ return value->integerValue; }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool
Zeitgeist_Value_hasList
	(
		Zeitgeist_Value const* value
	)
{ return Zeitgeist_ValueTag_List == value->tag; }

void
Zeitgeist_Value_setList
	(
		Zeitgeist_Value* value,
		Zeitgeist_List* listValue
	)
{
	value->tag = Zeitgeist_ValueTag_List;
	value->listValue = listValue;
}

Zeitgeist_List*
Zeitgeist_Value_getList
	(
		Zeitgeist_Value const* value
	)
{
	return value->listValue;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool
Zeitgeist_Value_hasMap
	(
		Zeitgeist_Value const* value
	)
{ return Zeitgeist_ValueTag_Map == value->tag; }

void
Zeitgeist_Value_setMap
	(
		Zeitgeist_Value* value,
		Zeitgeist_Map* mapValue
	)
{
	value->tag = Zeitgeist_ValueTag_Map;
	value->mapValue = mapValue;
}

Zeitgeist_Map*
Zeitgeist_Value_getMap
	(
		Zeitgeist_Value const* value
	)
{ return value->mapValue; }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool
Zeitgeist_Value_hasObject
	(
		Zeitgeist_Value const* value
	)
{ return Zeitgeist_ValueTag_Object == value->tag; }

void
Zeitgeist_Value_setObject
	(
		Zeitgeist_Value* value,
		Zeitgeist_Object* objectValue
	)
{
	value->tag = Zeitgeist_ValueTag_Object;
	value->objectValue = objectValue;
}

Zeitgeist_Object*
Zeitgeist_Value_getObject
	(
		Zeitgeist_Value const* value
	)
{ return value->objectValue; }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool
Zeitgeist_Value_hasReal32
	(
		Zeitgeist_Value const* value
	)
{ return Zeitgeist_ValueTag_Real32 == value->tag; }

void
Zeitgeist_Value_setReal32
	(
		Zeitgeist_Value* value,
		Zeitgeist_Real32 real32Value
	)
{
	value->tag = Zeitgeist_ValueTag_Real32;
	value->real32Value = real32Value;
}

Zeitgeist_Real32
Zeitgeist_Value_getReal32
	(
		Zeitgeist_Value const* value
	)
{ return value->real32Value; }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool
Zeitgeist_Value_hasString
	(
		Zeitgeist_Value const* value
	)
{ return Zeitgeist_ValueTag_String == value->tag; }

void
Zeitgeist_Value_setString
	(
		Zeitgeist_Value* value,
		Zeitgeist_String* stringValue
	)
{
	value->tag = Zeitgeist_ValueTag_String;
	value->stringValue = stringValue;
}

Zeitgeist_String*
Zeitgeist_Value_getString
	(
		Zeitgeist_Value const* value
	)
{ return value->stringValue; }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool
Zeitgeist_Value_hasVoid
	(
		Zeitgeist_Value const* value
	)
{ return Zeitgeist_ValueTag_Void == value->tag; }

void
Zeitgeist_Value_setVoid
	(
		Zeitgeist_Value *value,
		Zeitgeist_Void voidValue
	)
{
	value->tag = Zeitgeist_ValueTag_Void;
	value->voidValue = voidValue;
}

Zeitgeist_Void
Zeitgeist_Value_getVoid
	(
		Zeitgeist_Value const* value
	)
{ return value->voidValue; }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
