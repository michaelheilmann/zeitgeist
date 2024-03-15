#if !defined(ZEITGEIST_LOCKS_H_INCLUDED)
#define ZEITGEIST_LOCKS_H_INCLUDED

#include "Zeitgeist.h"

/// @ingroup state
typedef struct Locks Locks;

/// @debug
/// @brief Assert nothing is locked.
/// @ingroup state
void
Locks_preLastGcCheck
	(
		Zeitgeist_State* state
	);

void
Locks_premark
	(
		Zeitgeist_State* state
	);

void
Locks_initialize
	(
		Zeitgeist_State* state
	);

void
Locks_uninitialize
	(
		Zeitgeist_State* state
	);

void
Locks_notifyDestroy
	(
		Zeitgeist_State* state,
		Zeitgeist_Gc_Object* object
	);

void
Zeitgeist_lock
	(
		Zeitgeist_State* state,
		Zeitgeist_Gc_Object* object
	);

void
Zeitgeist_unlock
	(
		Zeitgeist_State* state,
		Zeitgeist_Gc_Object* object
	);

#endif // ZEITGEIST_LOCKS_H_INCLUDED
