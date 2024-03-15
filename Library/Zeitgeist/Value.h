// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_VALUE_H_INCLUDED)
#define ZEITGEIST_VALUE_H_INCLUDED

#include "Zeitgeist/Configure.h"

// jmp_buf
#include <setjmp.h>

// bool, true, false
#include <stdbool.h>

// int8_t, int64_t, INT64_MAX, INT64_MIN
#include <inttypes.h>

typedef struct Zeitgeist_State Zeitgeist_State;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * A Zeitgeist_Boolean object.
 */
typedef bool Zeitgeist_Boolean;
/**
 * @since 0.1
 * The value Zeitgeist_Boolean_True of a Zeitgeist_Boolean object.
 */
#define Zeitgeist_Boolean_True (true)
/**
 * @since 0.1
 * The value Zeitgeist_Boolean_False of a Zeitgeist_Boolean object.
 */
#define Zeitgeist_Boolean_False (false)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * A Zeitgeist_ForeignFunction object.
 */
typedef void (Zeitgeist_ForeignFunction)(Zeitgeist_State*);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * A Zeitgeist_Integer object.
 */
typedef int64_t Zeitgeist_Integer;
/**
 * @since 0.1
 * The maximum value of a Zeitgeist_Integer object.
 */
#define Zeitgeist_Integer_Maximum (INT64_MAX)
/**
 * @since 0.1
 * The minimum value of a Zeitgeist_Integer object.
 */
#define Zeitgeist_Integer_Minimum (INT64_MIN)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * A Zeitgeist_Real32 object.
 */
typedef double Zeitgeist_Real32;

/**
 * @since 0.1
 * The maximum value of a Zeitgeist_Real32 object.
 */
#define Zeitgeist_Real32_Maximum (-FLT_MAX)
/**
 * @since 0.1
 * The minimum value of a Zeitgeist_Real32 object.
 */
#define Zeitgeist_Real32_Minimum (-FLT_MAX)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * A Zeitgeist_Void object.
 */
typedef int8_t Zeitgeist_Void;
/**
 * @since 0.1
 * The only value of a Zeitgeist_Void object.
 */
#define Zeitgeist_Void_Void (0)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * A Zeitgeist_JumpTarget object.
 */
typedef struct Zeitgeist_JumpTarget Zeitgeist_JumpTarget;

/**
 * @since 0.1
 * A Zeitgeist_State object.
 */
typedef struct Zeitgeist_State Zeitgeist_State;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * A Zeitgeist_List object.
 */
typedef struct Zeitgeist_List Zeitgeist_List;

/**
 * @since 0.1
 * A Zeitgeist_Map object.
 */
typedef struct Zeitgeist_Map Zeitgeist_Map;

/**
 * @since 0.1
 * A Zeitgeist_Object object.
 */
typedef struct Zeitgeist_Object Zeitgeist_Object;

/**
 * @since 0.1
 * A Zeitgeist_String object.
 */
typedef struct Zeitgeist_String Zeitgeist_String;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * A Zeitgeist_Value object.
 */
typedef struct Zeitgeist_Value Zeitgeist_Value;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	


/**
 * @since 0.1
 * Symbolic constant. If its value is assigned to the field v.tag of a Zeitgeist_Value v,
 * then v.booleanValue holds a Zeitgeist_Boolean value.
 */
#define Zeitgeist_ValueTag_Boolean (0)

 /**
 * @since 0.1
 * Symbolic constant. If its value is assigned to the field v.tag of a Zeitgeist_Value v,
 * then v.foreignFunctionValue holds a Zeitgeist_ForeignFunction value.
 */
#define Zeitgeist_ValueTag_ForeignFunction (1)

 /**
 * @since 0.1
 * Symbolic constant. If its value is assigned to the field v.tag of a Zeitgeist_Value v,
 * then v.integerValue holds a Zeitgeist_Integer value.
 */
#define Zeitgeist_ValueTag_Integer (1)

/**
 * @since 0.1
 * Symbolic constant. If its value is assigned to the field v.tag of a Zeitgeist_Value v,
 * then v.listValue holds a Zeitgeist_List value.
 */
#define Zeitgeist_ValueTag_List (2)

/**
 * @since 0.1
 * Symbolic constant. If its value is assigned to the field v.tag of a Zeitgeist_Value v,
 * then v.mapValue holds a Zeitgeist_Map value.
 */
#define Zeitgeist_ValueTag_Map (3)

/**
 * @since 0.1
 * Symbolic constant. If its value is assigned to the field v.tag of a Zeitgeist_Value v,
 * then v.objectValue holds a Zeitgeist_Object value.
 */
#define Zeitgeist_ValueTag_Object (4)

/**
 * @since 0.1
 * Symbolic constant. If its value is assigned to the field v.tag of a Zeitgeist_Value v,
 * then v.stringValue holds a Zeitgeist_Real32 value.
 */
#define Zeitgeist_ValueTag_Real32 (5)

/**
 * @since 0.1
 * Symbolic constant. If its value is assigned to the field v.tag of a Zeitgeist_Value v,
 * then v.stringValue holds a Zeitgeist_String value.
 */
#define Zeitgeist_ValueTag_String (6)

/**
 * @since 0.1
 * Symbolic constant. If its value is assigned to the field v.tag of a Zeitgeist_Value v,
 * then v.voidValue holds a Zeitgeist_Void value.
 */
#define Zeitgeist_ValueTag_Void (7)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct Zeitgeist_Value {
	/**
	 * @since 0.1
	 * The tag indicates the value stored in this Zeitgeist_Value object.
	 * Must be the value of one of the Zeitgeist_Value_Tag symbolic constants.
	 */
	uint8_t tag;

	union {
		/**
		 * @since 0.1
		 * Holds a Zeitgeist_Boolean value if tag is Zeitgeist_ValueTag_Boolean.
		 */
		Zeitgeist_Boolean booleanValue;
		/**
		 * @since 0.1
		 * Holds a Zeitgeist_ForeignFunction value if tag is Zeitgeist_ValueTag_ForeignFunction.
		 */
		Zeitgeist_ForeignFunction* foreignFunction;
		/**
		 * @since 0.1
		 * Holds a Zeitgeist_Integer value if tag is Zeitgeist_ValueTag_Integer.
		 */
		Zeitgeist_Integer integerValue;
		/**
		 * @since 0.1
		 * Holds a Zeitgeist_List* value if tag is Zeitgeist_ValueTag_List.
		 */
		Zeitgeist_List* listValue;
		/**
		 * @since 0.1
		 * Holds a Zeitgeist_Map* value if tag is Zeitgeist_ValueTag_Map.
		 */
		Zeitgeist_Map* mapValue;
		/**
		 * @since 0.1
		 * Holds a Zeitgeist_Object* value if tag is Zeitgeist_ValueTag_Object.
		 */
		Zeitgeist_Object* objectValue;
		/**
		 * @since 0.1
		 * Holds a Zeitgeist_Real32 value if tag is Zeitgeist_ValueTag_Real32.
		 */
		Zeitgeist_Real32 real32Value;
		/**
		 * @since 0.1
		 * Holds a Zeitgeist_String* value if tag is Zeitgeist_ValueTag_String.
		 */
		Zeitgeist_String* stringValue;
		/**
		 * @since 0.1
		 * Holds the Zeitgeist_Void_Void value if tag is Zeitgeist_ValueTag_Void.
		 */
		Zeitgeist_Void voidValue;
	};
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * @brief Visit a Zeitgeist_Value object.
 * @param state A pointer to the state.
 * @param value A pointer to the value.
 */
void
Zeitgeist_Value_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_Value* value
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * @brief Get if a Zeitgeist_Value object stores a Zeitgeist_Boolean value.
 * @param value A pointer to a Zeitgeist_value object.
 * @return true if the Zeitgeist_Value object stores a Zeitgeist_Boolean value. false otherwise.
 */
bool
Zeitgeist_Value_hasBoolean
	(
		Zeitgeist_Value const* value
 );

/**
 * @since 0.1 
 * @brief Store a Zeitgeist_Boolean value in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @param booleanValue A Zeitgeist_Boolean value.
 */
void
Zeitgeist_Value_setBoolean
	(
		Zeitgeist_Value *value,
		Zeitgeist_Boolean booleanValue
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_Boolean value stored in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @return The Zeitgeist_Boolean value.
 * @undefined value does not point to a Zeitgeist_Value object.
 * @undefined The Zeitgeist_Value object does not store an Zeitgeist_Boolean value.
 */
Zeitgeist_Boolean
Zeitgeist_Value_getBoolean
	(
		Zeitgeist_Value* value
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * @brief Get if a Zeitgeist_Value object stores a Zeitgeist_ForeignFunction* value.
 * @param value A pointer to a Zeitgeist_value object.
 * @return true if the Zeitgeist_Value object stores a Zeitgeist_ForeignFunction* value. false otherwise.
 */
bool
Zeitgeist_Value_hasForeignFunction
	(
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1
 * @brief Store a Zeitgeist_ForeignFunction* value in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @param foreignFunctionValue A Zeitgeist_ForeignFunction* value.
 */
void
Zeitgeist_Value_setForeignFunction
	(
		Zeitgeist_Value* value,
		Zeitgeist_ForeignFunction* foreignFunction
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_ForeignFunction* value stored in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @return The Zeitgeist_ForeignFunction* value.
 * @undefined value does not point to a Zeitgeist_Value object.
 * @undefined The Zeitgeist_Value object does not store an Zeitgeist_ForeignFunction* value.
 */
Zeitgeist_ForeignFunction*
Zeitgeist_Value_getForeignFunction
	(
		Zeitgeist_Value* value
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * @brief Get if a Zeitgeist_Value object stores a Zeitgeist_Integer value.
 * @param value A pointer to a Zeitgeist_value object.
 * @return true if the Zeitgeist_Value object stores a Zeitgeist_Integer value. false otherwise.
 */
bool
Zeitgeist_Value_hasInteger
	(
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1 
 * @brief Store a Zeitgeist_Integer value in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @param integerValue A Zeitgeist_Integer value.
 */
void
Zeitgeist_Value_setInteger
	(
		Zeitgeist_Value *value,
		Zeitgeist_Integer integerValue
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_Integer value stored in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @return The Zeitgeist_Integer value.
 * @undefined value does not point to a Zeitgeist_Value object.
 * @undefined The Zeitgeist_Value object does not store an Zeitgeist_Integer value.
 */
Zeitgeist_Integer
Zeitgeist_Value_getInteger
	(
		Zeitgeist_Value const* value
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * @brief Get if a Zeitgeist_Value object stores a Zeitgeist_List* value.
 * @param value A pointer to a Zeitgeist_value object.
 * @return true if the Zeitgeist_Value object stores a Zeitgeist_List* value. false otherwise.
 */
bool
Zeitgeist_Value_hasList
	(
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1
 * @brief Store a Zeitgeist_List* value in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @param listValue A pointer to a Zeitgeist_List object.
 */
void
Zeitgeist_Value_setList
	(
		Zeitgeist_Value* value,
		Zeitgeist_List* listValue
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_List* value stored in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @return The Zeitgeist_List* value.
 * @undefined value does not point to a Zeitgeist_Value object.
 * @undefined The Zeitgeist_Value object does not store an Zeitgeist_List* value.
 */
Zeitgeist_List*
Zeitgeist_Value_getList
	(
		Zeitgeist_Value const* value
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * @brief Get if a Zeitgeist_Value object stores a Zeitgeist_Map* value.
 * @param value A pointer to a Zeitgeist_value object.
 * @return true if the Zeitgeist_Value object stores a Zeitgeist_Map* value. false otherwise.
 */
bool
Zeitgeist_Value_hasMap
	(
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1
 * @brief Store a Zeitgeist_Map* value in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @param mapValue A pointer to a Zeitgeist_Map object.
 */
void
Zeitgeist_Value_setMap
	(
		Zeitgeist_Value* value,
		Zeitgeist_Map* mapValue
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_Map* value stored in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @return The Zeitgeist_Map* value.
 * @undefined value does not point to a Zeitgeist_Value object.
 * @undefined The Zeitgeist_Value object does not store an Zeitgeist_Map* value.
 */
Zeitgeist_Map*
Zeitgeist_Value_getMap
	(
		Zeitgeist_Value const* value
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * @brief Get if a Zeitgeist_Value object stores a Zeitgeist_Object* value.
 * @param value A pointer to a Zeitgeist_value object.
 * @return true if the Zeitgeist_Value object stores a Zeitgeist_Object* value. false otherwise.
 */
bool
Zeitgeist_Value_hasObject
	(
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1 
 * @brief Store a Zeitgeist_Integer value in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @param objectValue A Zeitgeist_Object value.
 */
void
Zeitgeist_Value_setObject
	(
		Zeitgeist_Value* value,
		Zeitgeist_Object* objectValue
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_Object* value stored in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @return The Zeitgeist_Object* value.
 * @undefined value does not point to a Zeitgeist_Value object.
 * @undefined The Zeitgeist_Value object does not store an Zeitgeist_Object* value.
 */
Zeitgeist_Object*
Zeitgeist_Value_getObject
	(
		Zeitgeist_Value const* value
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * @brief Get if a Zeitgeist_Value object stores a Zeitgeist_Real32 value.
 * @param value A pointer to a Zeitgeist_value object.
 * @return true if the Zeitgeist_Value object stores a Zeitgeist_Real32 value. false otherwise.
 */
bool
Zeitgeist_Value_hasReal32
	(
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1
 * @brief Store a Zeitgeist_Real32 value in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @param real32Value A Zeitgeist_Real32 value.
 */
void
Zeitgeist_Value_setReal32
	(
		Zeitgeist_Value* value,
		Zeitgeist_Real32 real32Value
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_Real32 value stored in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @return The Zeitgeist_Real32 value.
 * @undefined value does not point to a Zeitgeist_Value object.
 * @undefined The Zeitgeist_Value object does not store an Zeitgeist_Real32 value.
 */
Zeitgeist_Real32
Zeitgeist_Value_getReal32
	(
		Zeitgeist_Value const* value
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * @brief Get if a Zeitgeist_Value object stores a Zeitgeist_String* value.
 * @param value A pointer to a Zeitgeist_value object.
 * @return true if the Zeitgeist_Value object stores a Zeitgeist_String* value. false otherwise.
 */
bool
Zeitgeist_Value_hasString
	(
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1 
 * @brief Store a Zeitgeist_String* value in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @param stringValue A Zeitgeist_String* value.
 */
void
Zeitgeist_Value_setString
	(
		Zeitgeist_Value* value,
		Zeitgeist_String* stringValue
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_String* value stored in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @return The Zeitgeist_String* value.
 * @undefined value does not point to a Zeitgeist_Value object.
 * @undefined The Zeitgeist_Value object does not store an Zeitgeist_String* value.
 */
Zeitgeist_String*
Zeitgeist_Value_getString
	(
		Zeitgeist_Value const* value
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.1
 * @brief Get if a Zeitgeist_Value object stores a Zeitgeist_Void value.
 * @param value A pointer to a Zeitgeist_value object.
 * @return true if the Zeitgeist_Value object stores a Zeitgeist_Void value. false otherwise.
 */
bool
Zeitgeist_Value_hasVoid
	(
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1 
 * @brief Store a Zeitgeist_Void value in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @param voidValue A Zeitgeist_Void value.
 */
void
Zeitgeist_Value_setVoid
	(
		Zeitgeist_Value *value,
		Zeitgeist_Void voidValue
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_Void value stored in a Zeitgeist_Value object.
 * @param value A pointer to a Zeitgeist_Value object.
 * @return The Zeitgeist_Void value.
 * @undefined value does not point to a Zeitgeist_Value object.
 * @undefined The Zeitgeist_Value object does not store an Zeitgeist_Void value.
 */
Zeitgeist_Void
Zeitgeist_Value_getVoid
	(
		Zeitgeist_Value const* value
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // ZEITGEIST_VALUE_H_INCLUDED
