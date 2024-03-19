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
 * @brief Get the size of the stack.
 * @param state A pointer to the state.
 * @return The size of the stack.
 */
size_t
Zeitgeist_Stack_getSize
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
 * @brief Pop an element from the stack.
 * @param state A pointer to the state.
 * @undefined The stack is empty.
 */
void
Zeitgeist_Stack_pop
	(
		Zeitgeist_State* state
	);

/**
 * @since 0.1
 * @brief Push a Zeitgeist_Boolean value on the stack.
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
 * @brief Get if the value at the specified stack index is a Zeitgeist_Boolean value.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return @a true if the value at the specfied stack index is a Zeitgeist_Boolean value. @a false otherwise.
 * @undefined The stack index is not within bounds.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
bool
Zeitgeist_Stack_isBoolean
	(
		Zeitgeist_State* state,
		size_t index
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_Boolean value at the specified stack index.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return The Zeitgeist_Boolean value.
 * @undefined The stack index is not within bounds.
 * @undefined The value at the specified index is not a Zeitgeist_Boolean value.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
Zeitgeist_Boolean
Zeitgeist_Stack_getBoolean
	(
		Zeitgeist_State* state,
		size_t index
	);

/**
 * @since 0.1
 * @brief Push a Zeitgeist_ForeignFunction* value on the stack.
 * @param state A pointer to the state.
 * @param foreignFunctionValue The Zeitgeist_ForeignFunction* value.
 */
void
Zeitgeist_Stack_pushForeignFunction
	(
		Zeitgeist_State* state,
		Zeitgeist_ForeignFunction* foreignFunctionValue
	);

/**
 * @since 0.1
 * @brief Get if the value at the specified stack index is a Zeitgeist_ForeignFunction* value.
 * @param state A pointer to the state.
 * @return @a true if the value at the specfied stack index is a Zeitgeist_ForeignFunction* value. @a false otherwise.
 * @undefined The stack index is not within bounds.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
bool
Zeitgeist_Stack_isForeignFunction
	(
		Zeitgeist_State* state,
		size_t index
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_ForeignFunction* value at the specified stack index.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return The Zeitgeist_ForeignFunction* value.
 * @undefined The stack index is not within bounds.
 * @undefined The value at the specified index is not a Zeitgeist_ForeignFunction* value.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
Zeitgeist_ForeignFunction*
Zeitgeist_Stack_getForeignFunction
	(
		Zeitgeist_State* state,
		size_t index
	);

/**
 * @since 0.1
 * @brief Push a Zeitgeist_ForeignObject* value on the stack.
 * @param state A pointer to the state.
 * @param foreignObjectValue The Zeitgeist_ForeignObject* value.
 */
void
Zeitgeist_Stack_pushForeignObject
	(
		Zeitgeist_State* state,
		Zeitgeist_ForeignObject* foreignObjectValue
	);

/**
 * @since 0.1
 * @brief Get if the value at the specified stack index is a ForeignObject* value.
 * @param state A pointer to the state.
 * @return @a true if the value at the specfied stack index is a ForeignObject* value. @a false otherwise.
 * @undefined The stack index is not within bounds.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
bool
Zeitgeist_Stack_isForeignObject
	(
		Zeitgeist_State* state,
		size_t index
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_ForeignObject* value at the specified stack index.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return The Zeitgeist_ForeignObject* value.
 * @undefined The stack index is not within bounds.
 * @undefined The value at the specified index is not a Zeitgeist_ForeignObject* value.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
Zeitgeist_ForeignObject*
Zeitgeist_Stack_getForeignObject
	(
		Zeitgeist_State* state,
		size_t index
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
 * @brief Get if the value at the specified stack index is an Integer value.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return @a true if the value at the specfied stack index is a Integer value. @a false otherwise.
 * @undefined The stack index is not within bounds.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
bool
Zeitgeist_Stack_isInteger
	(
		Zeitgeist_State* state,
		size_t index
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_Integer value at the specified stack index.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return The Zeitgeist_Integer value.
 * @undefined The stack index is not within bounds.
 * @undefined The value at the specified index is not a Zeitgeist_Integer value.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
Zeitgeist_Integer
Zeitgeist_Stack_getInteger
	(
		Zeitgeist_State* state,
		size_t index
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
 * @brief Get if the value at the specified stack index is a List value.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return @a true if the value at the specfied stack index is a List value. @a false otherwise.
 * @undefined The stack index is not within bounds.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
bool
Zeitgeist_Stack_isList
	(
		Zeitgeist_State* state,
		size_t index
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_List* value at the specified stack index.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return The Zeitgeist_List* value.
 * @undefined The stack index is not within bounds.
 * @undefined The value at the specified index is not a Zeitgeist_List* value.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
Zeitgeist_List*
Zeitgeist_Stack_getList
	(
		Zeitgeist_State* state,
		size_t index
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
 * @brief Get if the value at the specified stack index is a Map value.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return @a true if the value at the specfied stack index is a Map value. @a false otherwise.
 * @undefined The stack index is not within bounds.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
bool
Zeitgeist_Stack_isMap
	(
		Zeitgeist_State* state,
		size_t index
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_Map* value at the specified stack index.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return The Zeitgeist_Map* value.
 * @undefined The stack index is not within bounds.
 * @undefined The value at the specified index is not a Zeitgeist_Map* value.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
Zeitgeist_Map*
Zeitgeist_Stack_getMap
	(
		Zeitgeist_State* state,
		size_t index
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
 * @brief Get if the value at the specified stack index is a Real32 value.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return @a true if the value at the specfied stack index is a Real32 value. @a false otherwise.
 * @undefined The stack index is not within bounds.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
bool
Zeitgeist_Stack_isReal32
	(
		Zeitgeist_State* state,
		size_t index
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_Real32 value at the specified stack index.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return The Zeitgeist_Real32 value.
 * @undefined The stack index is not within bounds.
 * @undefined The value at the specified index is not a Zeitgeist_Real32 value.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
Zeitgeist_Real32
Zeitgeist_Stack_getReal32
	(
		Zeitgeist_State* state,
		size_t index
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
 * @brief Get if the value at the specified stack index is a String* value.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return @a true if the value at the specfied stack index is a String* value. @a false otherwise.
 * @undefined The stack index is not within bounds.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
bool
Zeitgeist_Stack_isString
	(
		Zeitgeist_State* state,
		size_t index
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_String* value at the specified stack index.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return The Zeitgeist_String* value.
 * @undefined The stack index is not within bounds.
 * @undefined The value at the specified index is not a Zeitgeist_String* value.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
Zeitgeist_String*
Zeitgeist_Stack_getString
	(
		Zeitgeist_State* state,
		size_t index
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
 * @brief Get if the value at the specified stack index is a Void value.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return @a true if the value at the specfied stack index is a Void value. @a false otherwise.
 * @undefined The stack index is not within bounds.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
bool
Zeitgeist_Stack_isVoid
	(
		Zeitgeist_State* state,
		size_t index
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_Void value at the specified stack index.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return The Zeitgeist_Void value.
 * @undefined The stack index is not within bounds.
 * @undefined The value at the specified index is not a Zeitgeist_Void value.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
Zeitgeist_Void
Zeitgeist_Stack_getVoid
	(
		Zeitgeist_State* state,
		size_t index
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

/**
 * @since 0.1
 * @brief Get if the value at the specified stack index is a WeakReference* value.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return @a true if the value at the specfied stack index is a WeakReference* value. @a false otherwise.
 * @undefined The stack index is not within bounds.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
bool
Zeitgeist_Stack_isWeakReference
	(
		Zeitgeist_State* state,
		size_t index
	);

/**
 * @since 0.1
 * @brief Get the Zeitgeist_WeakReference* value at the specified stack index.
 * @param state A pointer to the state.
 * @param index The stack index.
 * @return The Zeitgeist_WeakReference* value.
 * @undefined The stack index is not within bounds.
 * @undefined The value at the specified index is not a Zeitgeist_WeakReference* value.
 * @remark The stack index must be within the bounds of [0,n) where n is the size of the stack.
 * The stack index @a 0 denotes the element on the top of the stack, the stack index @a 1 denotes the element one below the top of the stack, ...
 */
Zeitgeist_WeakReference*
Zeitgeist_Stack_getWeakReference
	(
		Zeitgeist_State* state,
		size_t index
	);

#endif // ZEITGEIST_STACK_H_INCLUDED
