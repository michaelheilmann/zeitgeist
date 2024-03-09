// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEISTE_H_INCLUDED)
#define ZEITGEIST_H_INCLUDED

#if Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
  #include <stddef.h>
#endif
#include "Zeitgeist/ArrayList.h"
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
  Zeitgeist_JumpTarget* jumpTarget;
  Zeitgeist_ArrayList* arrayLists;
  Zeitgeist_Object* objects;
  Zeitgeist_String *strings;
};

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
