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
#define Zeitgeist_Gc_TypeTag_Object (3)

/**
 * @since 0.1
 * Type tag. See Zeitgeist_Gc_Object::typeTag for more information.
 */
#define Zeitgeist_Gc_TypeTag_String (4)

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
 * @brief A garbage collected object.
 */
typedef struct Zeitgeist_Gc_Object Zeitgeist_Gc_Object;

struct Zeitgeist_Gc_Object {

	Zeitgeist_Gc_Object* next;

	/**
	 * @brief The type of the garbage collected object.
	 * Zeitgeist_Gc_TypeTag_List indicates an GC object of type List.
	 * Zeitgeist_Gc_TypeTag_Map indicates an GC object of type Map.
	 * Zeitgeist_Gc_TypeTag_Object indicates a GC object of type Object.
	 * Zeitgeist_Gc_TypeTagString indicates a GC object of type String.
	 */
	uint8_t typeTag;

	/**
	 * @brief The color of the garbage collected object.
	 * Must exactly one of the following values: Zeitgeist_Gc_Color_Black or Zeitgeist_Gc_Color_White.
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

#endif // ZEITGEIST_GC_H_INCLUDED
