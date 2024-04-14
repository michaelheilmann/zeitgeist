// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_H_INCLUDED)
#define ZEITGEIST_H_INCLUDED

#if Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#include <stddef.h>
#endif
#include "Zeitgeist/List.h"
#include "Zeitgeist/Object.h"
#include "Zeitgeist/Value.h"

struct Zeitgeist_JumpTarget {
	Zeitgeist_JumpTarget* previous;
	jmp_buf environment;
};

struct Zeitgeist_String {
	Zeitgeist_String* next;
	size_t hashValue;
	size_t numberOfBytes;
	char bytes[];
};

/**
 * @since 0.1
 * Get if two strings are equal.
 * @param state A pointer to the state.
 * param v, w The string.
 * @return #Zeitgeist_Boolean_True if the strings are equal. #Zeitgeist_Boolean_False otherwise.
 */
Zeitgeist_Boolean
Zeitgeist_String_areEqual
	(
		Zeitgeist_State* state,
		Zeitgeist_String* v,
		Zeitgeist_String* w
	);

struct Zeitgeist_State {
	int lastError;
	/**
	 * @brief Zeitgeist checks the value of this variable each n (usually n = 1) updates.
	 * If this this true by that time, it will exit the process. Otherwise it will do nothing.
	 * @initial @a Zeitgeist_False.
	 */
	bool exitProcessRequested;
	Zeitgeist_JumpTarget* jumpTarget;
	Zeitgeist_List* lists;
	Zeitgeist_Object* objects;
	Zeitgeist_String *strings;
};

Zeitgeist_Boolean
Zeitgeist_State_isExitProcessRequested
	(
		Zeitgeist_State* state
	);

/**
 * @since 0.1
 * @brief Create a Zeitgeist_State object.
 * @return A pointer to the Zeitgeist_State object on success. A null pointer on failure.
 */
Zeitgeist_State*
Zeitgeist_createState
	(
	);

/**
 * @since 0.1
 * @brief Destroy a Zeitgeist_State object.
 * @param state A pointer to the Zeitgeist_State object.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined The jump target stack of the Zeitgeist_State object is not empty.
 */
void
Zeitgeist_State_destroy
	(
		Zeitgeist_State* state
	);

/**
 * @since 0.1
 * @brief Push a Zeitgeist_JumpTarget object on the jump target stack of a Zeitgeist_State object.
 * @param state A pointer to the Zeitgeist_State object.
 * @undefined state does not point to a Zeitgeist_State object.
 */
void
Zeitgeist_State_pushJumpTarget
	(
		Zeitgeist_State* state,
		Zeitgeist_JumpTarget* jumpTarget
	);

/**
 * @since 0.1
 * @brief Pop a Zeitgeist_JumpTarget object from the jump target stack of a Zeitgeist_State object.
 * @param state A pointer to the Zeitgeist_State object.
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined The jump target stack of the Zeitgeist_State object is empty.
 */
void
Zeitgeist_State_popJumpTarget
	(
		Zeitgeist_State* state
	);

/** 
 * @since 0.1
 * Set the state error variable to the specified value.
 * Jump to the current jump target.
 * @param state A pointer to the state.
 * @param file The C source code file as provided by __FILE__.
 * @param line The C source code line as provide by __LINE__.
 * @undefined error is a zero value.
 */
void
Zeitgeist_State_raiseError
	(
		Zeitgeist_State* state,
		char *file,
		int line,
		int error
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

Zeitgeist_String*
Zeitgeist_String_concatenate
	(
		Zeitgeist_State* state,
		Zeitgeist_String* prefix,
		Zeitgeist_String* suffix
	);

#endif // ZEITGEIST_H_INCLUDED
