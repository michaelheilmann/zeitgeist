#if !defined(VECTOR3R32_H_INCLUDED)
#define VECTOR3R32_H_INCLUDED

#include "Zeitgeist.h"
#include "idlib.h"

typedef struct Vector3R32 {
	Zeitgeist_ForeignObject _parent;
	idlib_vector_3_f32 v;
} Vector3R32;

Vector3R32*
Vector3R32_create
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 x,
		Zeitgeist_Real32 y,
		Zeitgeist_Real32 z
	);

Vector3R32*
Vector3R32_add
	(
		Zeitgeist_State* state,
		Vector3R32* left,
		Vector3R32* right
	);

Vector3R32*
Vector3R32_subtract
	(
		Zeitgeist_State* state,
		Vector3R32* left,
		Vector3R32* right
	);

#endif // VECTOR3R32_H_INCLUDED
