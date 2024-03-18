#if !defined(MATRIX4R32_H_INCLUDED)
#define MATRIX4R32_H_INCLUDED

#include "Zeitgeist.h"
typedef struct Vector3R32 Vector3R32;

typedef struct Matrix4R32 {
	Zeitgeist_ForeignObject _parent;
	Zeitgeist_Real32 e[4][4];
} Matrix4R32;

Matrix4R32*
Matrix4R32_createIdentity
	(
		Zeitgeist_State* state
	);

Matrix4R32*
Matrix4R32_createTranslate
	(
		Zeitgeist_State* state,
		Vector3R32* translation
	);

#endif // MATRIX4R32_H_INCLUDED
