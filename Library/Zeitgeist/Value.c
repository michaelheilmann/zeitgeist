// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/Value.h"

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void
Zeitgeist_Value_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_Value* value
	)
{
	fprintf(stderr, "%s:%d: not yet implemented\n", __FILE__, __LINE__);
	exit(EXIT_FAILURE);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool
Zeitgeist_Value_hasArrayList
	(
		Zeitgeist_Value const* value
	)
{ return Zeitgeist_ValueTag_ArrayList == value->tag; }

void
Zeitgeist_Value_setArrayList
	(
		Zeitgeist_Value *value,
		Zeitgeist_ArrayList* arrayListValue
	)
{
	value->tag = Zeitgeist_ValueTag_ArrayList;
	value->arrayListValue = arrayListValue;
}

Zeitgeist_ArrayList*
Zeitgeist_Value_getArrayList
	(
		Zeitgeist_Value const* value
	)
{ return value->arrayListValue; }

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
