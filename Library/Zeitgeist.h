// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_H_INCLUDED)
#define ZEITGEIST_H_INCLUDED

#if Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#include <stddef.h>
#endif
#include "Zeitgeist/List.h"
#include "Zeitgeist/Map.h"
#include "Zeitgeist/Object.h"
#include "Zeitgeist/String.h"
#include "Zeitgeist/Value.h"

struct Zeitgeist_JumpTarget {
	Zeitgeist_JumpTarget* previous;
	jmp_buf environment;
};

struct Zeitgeist_State {
	int lastError;
	/**
	 * @brief Zeitgeist checks the value of this variable each n (usually n = 1) updates.
	 * If this this true by that time, it will exit the process. Otherwise it will do nothing.
	 * @initial @a Zeitgeist_False.
	 */
	bool exitProcessRequested;
	Zeitgeist_JumpTarget* jumpTarget;
	Zeitgeist_String *strings;

	struct {
		Zeitgeist_Gc_Object* all;
		Zeitgeist_Gc_Object* gray;
	} gc;

	struct {
		Zeitgeist_Value* elements;
		size_t size;
		size_t capacity;
		size_t maximalCapacity;
	} stack;
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

void
Zeitgeist_State_update
	(
		Zeitgeist_State* state
	);

/**
 * @since 0.1
 * @brief Push a value on the stack.
 * @param state A pointer to the state.
 * @param value A pointer to the value.
 * @remark If the value is a Void value, then nothing is pushed.
 */
void
Zeitgeist_Stack_push
	(
		Zeitgeist_State* state,
		Zeitgeist_Value* value
	);

/**
 * @since 0.1
 * @brief Push a Boolean value on the stack.
 * @param state A pointer to the state.
 * @param booleanValue The Boolean value.
 */
void
Zeitgeist_Stack_pushBoolean
	(
		Zeitgeist_State* state,
		Zeitgeist_Boolean booleanValue
	);

/**
 * @since 0.1
 * @brief Push a Zeitgeist_Integer value on the stack.
 * @param state A pointer to the state.
 * @param integerValue The Zeitgeist_Integer value.
 */
void
Zeitgeist_Stack_pushInteger
	(
		Zeitgeist_State* state,
		Zeitgeist_Integer integerValue
	);

/**
 * @since 0.1
 * @brief Push a Zeitgeist_List* value on the stack.
 * @param state A pointer to the state.
 * @param listValue The Zeitgeist_List* value.
 */
void
Zeitgeist_Stack_pushList
	(
		Zeitgeist_State* state,
		Zeitgeist_List* listValue
	);

/**
 * @since 0.1
 * @brief Push a Zeitgeist_Map* value on the stack.
 * @param state A pointer to the state.
 * @param listValue The Zeitgeist_Map* value.
 */
void
Zeitgeist_Stack_pushMap
	(
		Zeitgeist_State* state,
		Zeitgeist_Map* mapValue
	);

/**
 * @since 0.1
 * @brief Push a Zeitgeist_Object* value on the stack.
 * @param state A pointer to the state.
 * @param listValue The Zeitgeist_Object* value.
 */
void
Zeitgeist_Stack_pushObject
	(
		Zeitgeist_State* state,
		Zeitgeist_Object* objectValue
	);

/**
 * @since 0.1
 * @brief Push a Zeitgeist_Real32 value on the stack.
 * @param state A pointer to the state.
 * @param real32Value The Zeitgeist_Real32 value.
 */
void
Zeitgeist_Stack_pushReal32
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 real32Value
	);

/**
 * @since 0.1
 * @brief Push a Zeitgeist_String* value on the stack.
 * @param state A pointer to the state.
 * @param stringValue The Zeitgeist_String* value.

 */
void
Zeitgeist_Stack_pushString
	(
		Zeitgeist_State* state,
		Zeitgeist_String* stringValue
	);

/**
 * @since 0.1
 * @brief Push a Zeitgeist_Void value on the stack.
 * @param state A pointer to the state.
 * @param voidValue The Zeitgeist_Void value.
 * @remarks Nothing is pushed on the stack.
 */
void
Zeitgeist_Stack_pushVoid
	(
		Zeitgeist_State* state,
		Zeitgeist_Void voidValue
	);

void
Zeitgeist_Stack_pop
	(
		Zeitgeist_State* state
	);

#endif // ZEITGEIST_H_INCLUDED
