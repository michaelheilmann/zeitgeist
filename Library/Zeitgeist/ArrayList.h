// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_ARRAYLIST_H_INCLUDED)
#define ZEITGEIST_ARRAYLIST_H_INCLUDED

#if Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#include <stddef.h>
#endif
#include "Zeitgeist/Value.h"

struct Zeitgeist_ArrayList {
	Zeitgeist_ArrayList* next;
	size_t size;
	size_t capacity;
	Zeitgeist_Value* elements;
};

/**
 * @since 0.1
 * @brief Create a Zeitgeist_ArrayList object.
 * @param state A pointer to the Zeitgeist_State object.
 * @return A pointer to the Zeitgeist_ArrayList object.
 * @undefined state does not point to a Zeitgeist_State object.
 */
Zeitgeist_ArrayList*
Zeitgeist_createArrayList
	(
		Zeitgeist_State* state
	);

/**
 * @since 0.1
 * @brief Get the value in a Zeitgeist_ArrayList object at the specified index.
 * @param state A pointer to the Zeitgeist_State object.
 * @param arrayList A pointer to the Zeitgeist_ArrayList object.
 * @param index The index at which to insert the value.
 * @return
 * The value at the specified index.
 * The Void value the index is out of bounds.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefine arrayList does not point to a Zeitgeist_ArrayList object.
 * @undefined value does not point to a Zeitgeist_Value object.
 */
Zeitgeist_Value
Zeitgeist_ArrayList_getValue
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		size_t index
	);

/**
 * @since 0.1
 * @brief Get the size of a Zeitgeist_ArrayList object.
 * @param state A pointer to the Zeitgeist_State object.
 * @param arrayList A pointer to the Zeitgeist_ArrayList object.
 * @param index The index at which to insert the value.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefine arrayList does not point to a Zeitgeist_ArrayList object.
 */
Zeitgeist_Value
Zeitgeist_ArrayList_getSize
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList
	);

/**
 * @since 0.1
 * @brief Insert the value stored in a Zeitgeist_Value object to a Zeitgeist_ArrayList object.
 * @param state A pointer to the Zeitgeist_State object.
 * @param arrayList A pointer to the Zeitgeist_ArrayList object.
 * @param index The index at which to insert the value.
 * @param value A pointer to the Zeitgeist_value object.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefine arrayList does not point to a Zeitgeist_ArrayList object.
 * @undefined value does not point to a Zeitgeist_Value object.
 * @warning Inserting a void value is a no operation; nothing will be added.
 */
void
Zeitgeist_ArrayList_insertValue
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		size_t index,
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1
 * @brief Append the value stored in a Zeitgeist_Value object to a Zeitgeist_ArrayList object.
 * @param state A pointer to the Zeitgeist_State object.
 * @param arrayList A pointer to the Zeitgeist_ArrayList object.
 * @param value A pointer to the Zeitgeist_value object.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefine arrayList does not point to a Zeitgeist_ArrayList object.
 * @undefined value does not point to a Zeitgeist_Value object.
 */
void
Zeitgeist_ArrayList_appendValue
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1
 * @brief Prepend the value stored in a Zeitgeist_Value object to a Zeitgeist_ArrayList object.
 * @param state A pointer to the Zeitgeist_State object.
 * @param arrayList A pointer to the Zeitgeist_ArrayList object.
 * @param value A pointer to the Zeitgeist_value object.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefine arrayList does not point to a Zeitgeist_ArrayList object.
 * @undefined value does not point to a Zeitgeist_Value object.
 */
void
Zeitgeist_ArrayList_prependValue
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1
 * @brief Append a Zeitgeist_ArrayList* value to a Zeitgeist_ArrayList object.
 * @param arrayList A pointer to the Zeitgeist_ArrayList object to append to.
 * @param element A pointer to the Zeitgeist_ArrayList object to append.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined arrayList does not point to a Zeitgeist_ArrayList object.
 * @undefined element does not point to a Zeitgeist_ArrayList object.
 */
static inline void
Zeitgeist_ArrayList_appendArrayList
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		Zeitgeist_ArrayList* element
	)
{ 
	Zeitgeist_Value value;
	Zeitgeist_Value_setArrayList(&value, element);
	Zeitgeist_ArrayList_appendValue(state, arrayList, &value);
}

/**
 * @since 0.1
 * @brief Append a Zeitgeist_Boolean value to a Zeitgeist_ArrayList object.
 * @param arrayList A pointer to the Zeitgeist_ArrayList object to append to.
 * @param element A Zeitgeist_Boolean value object to append.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined arrayList does not point to a Zeitgeist_ArrayList object.
 */
static inline void
Zeitgeist_ArrayList_appendBoolean
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		Zeitgeist_Boolean element
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setBoolean(&value, element);
	Zeitgeist_ArrayList_appendValue(state, arrayList, &value);
}

/**
 * @since 0.1
 * @brief Append a Zeitgeist_Integer value to a Zeitgeist_ArrayList object.
 * @param arrayList A pointer to the Zeitgeist_ArrayList object to append to.
 * @param element the Zeitgeist_Integer value to append.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined arrayList does not point to a Zeitgeist_ArrayList object.
 */
static inline void
Zeitgeist_ArrayList_appendInteger
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		Zeitgeist_Integer element
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setInteger(&value, element);
	Zeitgeist_ArrayList_appendValue(state, arrayList, &value);
}

/**
 * @since 0.1
 * @brief Append a Zeitgeist_Object* value to a Zeitgeist_ArrayList object.
 * @param arrayList A pointer to the Zeitgeist_ArrayList object to append to.
 * @param element The Zeitgeist_Object* value to append.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined arrayList does not point to a Zeitgeist_ArrayList object.
 * @undefined element does not point to a Zeitgeist_Object object.
 */
static inline void
Zeitgeist_ArrayList_appendObject
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		Zeitgeist_Object* element
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setObject(&value, element);
	Zeitgeist_ArrayList_appendValue(state, arrayList, &value);
}

/**
 * @since 0.1
 * @brief Append a Zeitgeist_String* value to a Zeitgeist_ArrayList object.
 * @param arrayList A pointer to the Zeitgeist_ArrayList object to append to.
 * @param element The Zeitgeist_String* value to append.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined arrayList does not point to a Zeitgeist_ArrayList object.
 * @undefined element does not point to a Zeitgeist_String object.
 */
static inline void
Zeitgeist_ArrayList_appendString
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		Zeitgeist_String* element
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setString(&value, element);
	Zeitgeist_ArrayList_appendValue(state, arrayList, &value);
}

/**
 * @since 0.1
 * @brief Append a Zeitgeist_Void value to a Zeitgeist_ArrayList object.
 * @param arrayList A pointer to the Zeitgeist_ArrayList object to append to.
 * @param element The Zeitgeist_Void value to append.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined arrayList does not point to a Zeitgeist_ArrayList object.
 * @warning Adding a void value is a no operation; nothing will be added.
 */
static inline void
Zeitgeist_ArrayList_appendVoid
	(
		Zeitgeist_State* state,
		Zeitgeist_ArrayList* arrayList,
		Zeitgeist_Void element
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setVoid(&value, element);
	Zeitgeist_ArrayList_appendValue(state, arrayList, &value);
}

#endif // ZEITGEIST_ARRAYLIST_H_INCLUDED
