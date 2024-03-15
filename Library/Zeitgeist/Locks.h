#if !defined(ZEITGEIST_LOCKS_H_INCLUDED)
#define ZEITGEIST_LOCKS_H_INCLUDED

#include "Zeitgeist.h"

/// @ingroup state
typedef struct Locks Locks;

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

#endif // ZEITGEIST_LOCKS_H_INCLUDED
