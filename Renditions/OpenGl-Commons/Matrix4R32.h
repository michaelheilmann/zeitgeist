#if !defined(MATRIX4R32_H_INCLUDED)
#define MATRIX4R32_H_INCLUDED

#include "Zeitgeist.h"
#include "idlib.h"
typedef struct Vector3R32 Vector3R32;

typedef struct Matrix4R32 {
	Zeitgeist_ForeignObject _parent;
	idlib_matrix_4x4_f32 m;
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

Matrix4R32*
Matrix4R32_createRotateY
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 degrees
	);

Matrix4R32*
Matrix4R32_createPerspective
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 fieldOfViewY,
		Zeitgeist_Real32 aspectRatio,
		Zeitgeist_Real32 near,
		Zeitgeist_Real32 far
	);

Matrix4R32*
Matrix4R32_createOrthographic
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 left,
		Zeitgeist_Real32 right,
		Zeitgeist_Real32 bottom,
		Zeitgeist_Real32 top,
		Zeitgeist_Real32 near,
		Zeitgeist_Real32 far
	);

#endif // MATRIX4R32_H_INCLUDED
