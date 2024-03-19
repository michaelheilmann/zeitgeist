// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_LIST_H_INCLUDED)
#define ZEITGEIST_LIST_H_INCLUDED

#if Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#include <stddef.h>
#endif
#include "Zeitgeist/Gc.h"
#include "Zeitgeist/Value.h"

struct Zeitgeist_List {
	Zeitgeist_Gc_Object _parent;
	/** For being added to the "gray" list during the "mark" phase of the garbage collector. */
	Zeitgeist_Gc_Object* gclist;

	size_t size;
	size_t capacity;
	Zeitgeist_Value* elements;
};

void
Zeitgeist_List_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list
	);

void
Zeitgeist_List_finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list
	);

/**
 * @since 0.1
 * @brief Create a Zeitgeist_List object.
 * @param state A pointer to the Zeitgeist_State object.
 * @return A pointer to the Zeitgeist_List object.
 * @undefined state does not point to a Zeitgeist_State object.
 */
Zeitgeist_List*
Zeitgeist_createList
	(
		Zeitgeist_State* state
	);

/**
 * @since 0.1
 * @brief Get the value in a Zeitgeist_List object at the specified index.
 * @param state A pointer to the Zeitgeist_State object.
 * @param list A pointer to the Zeitgeist_List object.
 * @param index The index at which to insert the value.
 * @return
 * The value at the specified index.
 * The Void value the index is out of bounds.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefine list does not point to a Zeitgeist_List object.
 * @undefined value does not point to a Zeitgeist_Value object.
 */
Zeitgeist_Value
Zeitgeist_List_getValue
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		size_t index
	);

/**
 * @since 0.1
 * @brief Get the size of a Zeitgeist_List object.
 * @param state A pointer to the Zeitgeist_State object.
 * @param list A pointer to the Zeitgeist_List object.
 * @param index The index at which to insert the value.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefine list does not point to a Zeitgeist_List object.
 */
Zeitgeist_Value
Zeitgeist_List_getSize
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list
	);

/**
 * @since 0.1
 * @brief Insert the value stored in a Zeitgeist_Value object to a Zeitgeist_List object.
 * @param state A pointer to the Zeitgeist_State object.
 * @param list A pointer to the Zeitgeist_List object.
 * @param index The index at which to insert the value.
 * @param value A pointer to the Zeitgeist_value object.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefine list does not point to a Zeitgeist_List object.
 * @undefined value does not point to a Zeitgeist_Value object.
 * @warning Inserting a void value is a no operation; nothing will be added.
 */
void
Zeitgeist_List_insertValue
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		size_t index,
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1
 * @brief Append the value stored in a Zeitgeist_Value object to a Zeitgeist_List object.
 * @param state A pointer to the Zeitgeist_State object.
 * @param list A pointer to the Zeitgeist_List object.
 * @param value A pointer to the Zeitgeist_value object.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefine list does not point to a Zeitgeist_List object.
 * @undefined value does not point to a Zeitgeist_Value object.
 */
void
Zeitgeist_List_appendValue
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1
 * @brief Prepend the value stored in a Zeitgeist_Value object to a Zeitgeist_List object.
 * @param state A pointer to the Zeitgeist_State object.
 * @param list A pointer to the Zeitgeist_List object.
 * @param value A pointer to the Zeitgeist_value object.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefine list does not point to a Zeitgeist_List object.
 * @undefined value does not point to a Zeitgeist_Value object.
 */
void
Zeitgeist_List_prependValue
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		Zeitgeist_Value const* value
	);

/**
 * @since 0.1
 * @brief Append a Zeitgeist_List* value to a Zeitgeist_List object.
 * @param list A pointer to the Zeitgeist_List object to append to.
 * @param element A pointer to the Zeitgeist_List object to append.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined list does not point to a Zeitgeist_List object.
 * @undefined element is not a Zeitgeist_List* value.
 */
static inline void
Zeitgeist_List_appendList
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		Zeitgeist_List* element
	)
{ 
	Zeitgeist_Value value;
	Zeitgeist_Value_setList(&value, element);
	Zeitgeist_List_appendValue(state, list, &value);
}

/**
 * @since 0.1
 * @brief Append a Zeitgeist_Boolean value to a Zeitgeist_List object.
 * @param list A pointer to the Zeitgeist_List object to append to.
 * @param element A Zeitgeist_Boolean value object to append.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined list does not point to a Zeitgeist_List object.
 */
static inline void
Zeitgeist_List_appendBoolean
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		Zeitgeist_Boolean element
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setBoolean(&value, element);
	Zeitgeist_List_appendValue(state, list, &value);
}

/**
 * @since 0.1
 * @brief Append a Zeitgeist_ForeignFunction* value to a Zeitgeist_List object.
 * @param list A pointer to the Zeitgeist_List object to append to.
 * @param element the Zeitgeist_ForeignFunction* value to append.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined list does not point to a Zeitgeist_List object.
 * @undefined element ist not a Zeitgeist_ForeignFunction* value.
 */
static inline void
Zeitgeist_List_appendForeignFunction
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		Zeitgeist_ForeignFunction* element
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setForeignFunction(&value, element);
	Zeitgeist_List_appendValue(state, list, &value);
}

/**
 * @since 0.1
 * @brief Append a Zeitgeist_Integer value to a Zeitgeist_List object.
 * @param list A pointer to the Zeitgeist_List object to append to.
 * @param element the Zeitgeist_Integer value to append.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined list does not point to a Zeitgeist_List object.
 */
static inline void
Zeitgeist_List_appendInteger
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		Zeitgeist_Integer element
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setInteger(&value, element);
	Zeitgeist_List_appendValue(state, list, &value);
}

/**
 * @since 0.1
 * @brief Append a Zeitgeist_ForeignObject* value to a Zeitgeist_List object.
 * @param list A pointer to the Zeitgeist_List object to append to.
 * @param element The Zeitgeist_Object* value to append.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined list does not point to a Zeitgeist_List object.
 * @undefined element is not a Zeitgeist_ForeignObject* value.
 */
static inline void
Zeitgeist_List_appendForeignObject
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		Zeitgeist_ForeignObject * element
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setForeignObject(&value, element);
	Zeitgeist_List_appendValue(state, list, &value);
}

/**
 * @since 0.1
 * @brief Append a Zeitgeist_String* value to a Zeitgeist_List object.
 * @param list A pointer to the Zeitgeist_List object to append to.
 * @param element The Zeitgeist_String* value to append.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined list does not point to a Zeitgeist_List object.
 * @undefined element is not a Zeitgeist_String* value.
 */
static inline void
Zeitgeist_List_appendString
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		Zeitgeist_String* element
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setString(&value, element);
	Zeitgeist_List_appendValue(state, list, &value);
}

/**
 * @since 0.1
 * @brief Append a Zeitgeist_Void value to a Zeitgeist_List object.
 * @param list A pointer to the Zeitgeist_List object to append to.
 * @param element The Zeitgeist_Void value to append.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined list does not point to a Zeitgeist_List object.
 * @warning Adding a void value is a no operation; nothing will be added.
 */
static inline void
Zeitgeist_List_appendVoid
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list,
		Zeitgeist_Void element
	)
{
	Zeitgeist_Value value;
	Zeitgeist_Value_setVoid(&value, element);
	Zeitgeist_List_appendValue(state, list, &value);
}

#endif // ZEITGEIST_LIST_H_INCLUDED
