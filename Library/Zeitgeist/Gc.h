// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_GC_H_INCLUDED)
#define ZEITGEIST_GC_H_INCLUDED

#include "Zeitgeist/Configure.h"

// bool, true, false
#include <stdbool.h>

// int8_t, int64_t, INT64_MAX, INT64_MIN
#include <inttypes.h>


/**
 * @since 0.1
 * Type tag. See Zeitgeist_Gc_Object::typeTag for more information.
 */
#define Zeitgeist_Gc_TypeTag_List (1)

/**
 * @since 0.1
 * Type tag. See Zeitgeist_Gc_Object::typeTag for more information.
 */
#define Zeitgeist_Gc_TypeTag_Map (2)

/**
 * @since 0.1
 * Type tag. See Zeitgeist_Gc_Objectc::typeTag for more information.
 */
#define Zeitgeist_Gc_TypeTag_ForeignObject (3)

/**
 * @since 0.1
 * Type tag. See Zeitgeist_Gc_Object::typeTag for more information.
 */
#define Zeitgeist_Gc_TypeTag_String (4)

/**
 * @since 0.1
 * Type tag. See Zeitgeist_Gc_Object::typeTag for more information.
 */
#define Zeitgeist_Gc_TypeTag_WeakReference (5)

/**
 * @since 0.1
 * Color. See Zeitgeist_Gc_Object::color for more information.
 */
#define Zeitgeist_Gc_Color_Black (1)

/**
 * @since 0.1
 * Color. See Zeitgeist_Gc_Object::color for more information.
 */
#define Zeitgeist_Gc_Color_White (2)

/**
 * @since 0.1
 * Color. See Zeitgeist_Gc_Object::color for more information.
 */
#define Zeitgeist_Gc_Color_Gray (Zeitgeist_Gc_Color_Black|Zeitgeist_Gc_Color_White)

/**
 * @since 0.1
 * @brief A garbage collected object.
 */
typedef struct Zeitgeist_Gc_Object Zeitgeist_Gc_Object;

struct Zeitgeist_Gc_Object {

	Zeitgeist_Gc_Object* next;

	/**
	 * @brief The type of the garbage collected object.
	 * Zeitgeist_Gc_TypeTag_List indicates an GC object of type List.
	 * Zeitgeist_Gc_TypeTag_Map indicates an GC object of type Map.
	 * Zeitgeist_Gc_TypeTag_ForeignObject indicates a GC object of type ForeignObject.
	 * Zeitgeist_Gc_TypeTag_String indicates a GC object of type String.
	 * Zeitgeist_Gc_TypeTag_WeakReference indicates a GC object of type WeakReference.
	 */
	uint8_t typeTag;

	/**
	 * @brief The color of the garbage collected object.
	 * Must exactly one of the following values: Zeitgeist_Gc_Color_Black, Zeitgeist_Gc_Color_White, or Zeitgeist_Gc_Color_Gray.
	 */
	uint8_t color;

};

/**
 * @since 0.1
 * @brief Set the color (Zeitgeist_Gc_Object:color) of a GC object to black (Zeitgeist_Gc_Color_Black).
 * @param object A pointer to the GC object.
 */
void
Zeitgeist_Gc_Object_setBlack
	(
		Zeitgeist_Gc_Object* object
	);

/**
 * @since 0.1
 * @brief Get if a GC object has the color black.
 * @param object A pointer to the GC object.
 * @return true if the GC object has the color white. false otherwise.
 */
bool
Zeitgeist_Gc_Object_isBlack
	(
		Zeitgeist_Gc_Object* object
	);

/**
 * @since 0.1
 * @brief Set the color (Zeitgeist_Gc_Object:color) of a GC object to white (Zeitgeist_Gc_Color_White).
 * @param object A pointer to the GC object.
 */
void
Zeitgeist_Gc_Object_setWhite
	(
		Zeitgeist_Gc_Object* object
	);

/**
 * @since 0.1
 * @brief Get if a GC object has the color white.
 * @param object A pointer to the GC object.
 * @return true if the GC object has the color white. false otherwise.
 */
bool
Zeitgeist_Gc_Object_isWhite
	(
		Zeitgeist_Gc_Object* object
	);

/**
 * @since 0.1
 * @brief Set the color (Zeitgeist_Gc_Object:color) of a GC object to gray (Zeitgeist_Gc_Color_Gray).
 * @param object A pointer to the GC object.
 */
void
Zeitgeist_Gc_Object_setGray
	(
		Zeitgeist_Gc_Object* object
	);

/**
 * @since 0.1
 * @brief Get if a GC object has the color gray.
 * @param object A pointer to the GC object.
 * @return true if the GC object has the color gray. false otherwise.
 */
bool
Zeitgeist_Gc_Object_isGray
	(
		Zeitgeist_Gc_Object* object
	);

typedef struct Zeitgeist_State Zeitgeist_State;
typedef struct Zeitgeist_ForeignObject Zeitgeist_ForeignObject;
typedef struct Zeitgeist_List Zeitgeist_List;
typedef struct Zeitgeist_Map Zeitgeist_Map;
typedef struct Zeitgeist_String Zeitgeist_String;
typedef struct Zeitgeist_Value Zeitgeist_Value;
typedef struct Zeitgeist_WeakReference Zeitgeist_WeakReference;

/**
 * @brief If the foreign object is white, color it gray and put it into the gray list.
 */
void
Zeitgeist_Gc_visitForeignObject
	(
		Zeitgeist_State* state,
		Zeitgeist_ForeignObject* foreignObject
	);

/**
 * @brief If the list is white, color it gray and put it into the gray list.
 */
void
Zeitgeist_Gc_visitList
	(
		Zeitgeist_State* state,
		Zeitgeist_List* list
	);

/**
 * @brief If the map is white, color it gray and put it into the gray list.
 */
void
Zeitgeist_Gc_visitMap
	(
		Zeitgeist_State* state,
		Zeitgeist_Map* map
	);

/**
 * @brief Color the string black.
 */
void
Zeitgeist_Gc_visitString
	(
		Zeitgeist_State* state,
		Zeitgeist_String* string
	);

/**
 * @brief Traverbase the value, calling Zeitgeist_Gc_visit(ForeignObject|List|Map|String|WeakReference) if required.
 */
void
Zeitgeist_Gc_visitValue
	(
		Zeitgeist_State* state,
		Zeitgeist_Value* value
	);

/**
 * @brief Color the weak reference black.
 */
void
Zeitgeist_Gc_visitWeakReference
	(
		Zeitgeist_State* state,
		Zeitgeist_WeakReference* weakReference
	);

/**
 * @brief Ensure that the invariant "a black object may never refer to a white object" holds.
 * @param state A pointer to the state.
 * @param a A pointer to one object.
 * @param b A pointer to another object.
 * @remarks
 * This function ensures the invariant as follows:
 * - if a is black and b is white, then color b gray (or black).
 * - if a is white and b is black, then color a gray (or black).
 */
void
Zeitgeist_Gc_barrier
	(
		Zeitgeist_State* state,
		Zeitgeist_Gc_Object* a,
		Zeitgeist_Gc_Object* b
	);

#endif // ZEITGEIST_GC_H_INCLUDED
