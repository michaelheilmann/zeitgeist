#if !defined(ZEITGEIST_STACK_H_INCLUDED)
#define ZEITGEIST_STACK_H_INCLUDED

#include "Zeitgeist.h"

/// @ingroup state
typedef struct Stack Stack;

struct Stack {
	Zeitgeist_Value* elements;
	size_t size;
	size_t capacity;
	size_t maximalCapacity;
};

/// @debug
/// @brief Assert the stack is empty.
/// @ingroup state
void
Stack_preLastGcCheck
	(
		Zeitgeist_State* state
	);

/// @ingroup state 
void
Stack_premark
	(
		Zeitgeist_State* state
	);

/// @ingroup state
void
Stack_initialize
	(
		Zeitgeist_State* state
	);

/// @ingroup state 
void
Stack_uninitialize
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

/**
 * @since 0.1
 * @brief Push a Zeitgeist_WeakReference* value on the stack.
 * @param state A pointer to the state.
 * @param weakReferenceValue The Zeitgeist_WeakReference* value.
 */
void
Zeitgeist_Stack_pushWeakReference
	(
		Zeitgeist_State* state,
		Zeitgeist_WeakReference* weakReference
	);

void
Zeitgeist_Stack_pop
	(
		Zeitgeist_State* state
	);

#endif // ZEITGEIST_STACK_H_INCLUDED
