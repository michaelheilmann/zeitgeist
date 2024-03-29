// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_H_INCLUDED)
#define ZEITGEIST_H_INCLUDED

// malloc
#include <malloc.h>

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

#define cAllocate(size) \
	(malloc(size > 0 ? size : 1))

#define cDeallocate(pointer) \
	(free(pointer))

#define cDebugAssert(expression) \
	if (!(expression)) { \
		fprintf(stderr, "%s:%d: assertion `%s` failed\n", __FILE__, __LINE__, #expression);	\
		exit(EXIT_FAILURE); \
	}

#if Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#include <stddef.h>
#endif

#include "Zeitgeist/List.h"
#include "Zeitgeist/Map.h"
#include "Zeitgeist/Object.h"
#include "Zeitgeist/Stack.h"
#include "Zeitgeist/String.h"
#include "Zeitgeist/Value.h"

/// @unmanaged
struct Zeitgeist_JumpTarget {
	Zeitgeist_JumpTarget* previous;
	jmp_buf environment;
};

/// @ingroup state
typedef struct WeakReferences WeakReferences;
/// @ingroup state
typedef struct Locks Locks;
/// @ingroup state
typedef struct Stack Stack;

struct Zeitgeist_State {

	int lastError;
	/**
	 * @brief Zeitgeist checks the value of this variable each n (usually n = 1) updates.
	 * If this this true by that time, it will exit the process. Otherwise it will do nothing.
	 * @initial @a Zeitgeist_False.
	 */
	bool exitProcessRequested;
	
	Locks* locks;
	WeakReferences* weakReferences;
	Stack* stack;
	
	Zeitgeist_JumpTarget* jumpTarget;
	Zeitgeist_String *strings;

	struct {
		Zeitgeist_Gc_Object* all;
		Zeitgeist_Gc_Object* gray;
	} gc;
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

#endif // ZEITGEIST_H_INCLUDED
