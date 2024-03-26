#if !defined(MATRIX4R32_H_INCLUDED)
#define MATRIX4R32_H_INCLUDED

#include "Zeitgeist.h"
#include "idlib-math.h"
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

Matrix4R32*
Matrix4R32_createLookAt
	(
		Zeitgeist_State* state,
		Vector3R32* source,
		Vector3R32* target,
		Vector3R32* up
	);

Matrix4R32*
Matrix4R32_createScale
	(
		Zeitgeist_State* state,
		Vector3R32* scale
	);

Matrix4R32*
Matrix4R32_negate
	(
		Zeitgeist_State* state,
		Matrix4R32* matrix
	);

Matrix4R32*
Matrix4R32_multiply
	(
		Zeitgeist_State* state,
		Matrix4R32* operand1,
		Matrix4R32* operand2
	);

#endif // MATRIX4R32_H_INCLUDED
