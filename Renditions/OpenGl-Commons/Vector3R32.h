#if !defined(VECTOR3R32_H_INCLUDED)
#define VECTOR3R32_H_INCLUDED

#include "Zeitgeist.h"

typedef struct Vector3R32 {
	Zeitgeist_ForeignObject _parent;
	Zeitgeist_Real32 e[3];
} Vector3R32;

Vector3R32*
Vector3R32_create
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 x,
		Zeitgeist_Real32 y,
		Zeitgeist_Real32 z
	);

#endif // VECTOR3R32_H_INCLUDED
