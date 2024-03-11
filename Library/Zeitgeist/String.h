// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_STRING_H_INCLUDED)
#define ZEITGEIST_STRING_H_INCLUDED

#include "Zeitgeist/Value.h"

struct Zeitgeist_String {
	Zeitgeist_String* next;
	size_t hashValue;
	size_t numberOfBytes;
	char bytes[];
};

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
