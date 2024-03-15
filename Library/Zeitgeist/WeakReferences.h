#if !defined(ZEITGEIST_WEAKREFERENCES_H_INCLUDED)
#define ZEITGEIST_WEAKREFERENCES_H_INCLUDED

#include "Zeitgeist.h"

/// @ingroup state
typedef struct WeakReferences WeakReferences;

/// @ingroup state
void
WeakReferences_initialize
	(
		Zeitgeist_State* state
	);

/// @ingroup state 
void
WeakReferences_uninitialize
	(
		Zeitgeist_State* state
	);

struct Zeitgeist_WeakReference {
	Zeitgeist_Gc_Object _parent;
	Zeitgeist_Gc_Object* reference;
};

void
Zeitgeist_WeakReference_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_WeakReference* weakReference
	);

void
Zeitgeist_WeakReference_finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_WeakReference* weakReference
	);

void
WeakReferences_notifyDestroy
	(
		Zeitgeist_State* state,
		void* address
	);

/**
 * @since 0.1
 * @brief Create a Zeitgeist_WeakReference object.
 * @param reference A pointer to a Zeitgeist_Gc_Object or a null pointer.
 * @return A pointer to the Zeitgeist_WeakReference object.
 */
Zeitgeist_WeakReference*
Zeitgeist_WeakReference_create
	(
		Zeitgeist_State* state,
		Zeitgeist_Gc_Object* reference
	);

#endif // ZEITGEIST_WEAKREFERENCES_H_INCLUDED
