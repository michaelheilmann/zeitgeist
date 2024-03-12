// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_STRING_H_INCLUDED)
#define ZEITGEIST_STRING_H_INCLUDED

#include "Zeitgeist/Value.h"

struct Zeitgeist_String {
	Zeitgeist_Gc_Object _parent;
	Zeitgeist_String* next;
	size_t hashValue;
	size_t numberOfBytes;
	char bytes[];
};

void
Zeitgeist_String_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_String* string
	);

void
Zeitgeist_String_finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_String* string
	);

/**
 * @since 0.1
 * @brief Create a Zeitgeist_String object from the specified sequence of Bytes.
 * @param state A pointer to the Zeitgeist_State object.
 * @param bytes, numberOfBytes @a bytes points to an array of @a numberOfBytes Bytes.
 * @return A pointer to the Zeitgeist_String object.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined bytes does not point to an array of @a numberOfBytes Bytes.
 */
Zeitgeist_String*
Zeitgeist_State_createString
	(
		Zeitgeist_State* state,
		char const* bytes,
		size_t numberOfBytes
	);

/**
 * @since 0.1
 * @brief Get if two strings are equal.
 * @param state A pointer to the state.
 * @param v, w The strings.
 * @return #Zeitgeist_Boolean_True if the strings are equal. #Zeitgeist_Boolean_False otherwise.
 */
Zeitgeist_Boolean
Zeitgeist_String_areEqual
	(
		Zeitgeist_State* state,
		Zeitgeist_String* v,
		Zeitgeist_String* w
	);

/**
 * @since 0.1
 * @brief Concatenate two string.
 * @param state A pointer to the state.
 * @param v The prefix string.
 * @param w The suffix string.
 * @return The string representing the concatenation of the prefix string with the suffix string.
 */
Zeitgeist_String*
Zeitgeist_String_concatenate
	(
		Zeitgeist_State* state,
		Zeitgeist_String* v,
		Zeitgeist_String* w
	);

#endif // ZEITGEIST_STRING_H_INCLUDED
