#include "Matrix4R32.h"

#include "Vector3R32.h"

Matrix4R32*
Matrix4R32_createIdentity
	(
		Zeitgeist_State* state
	)
{
	Matrix4R32* self = Zeitgeist_allocateForeignObject(state, sizeof(Matrix4R32), NULL, NULL);
	idlib_matrix_4x4_f32_set_identity(&self->m);
	return self;
}

Matrix4R32*
Matrix4R32_createTranslate
	(
		Zeitgeist_State* state,
		Vector3R32* translate
	)
{
	Matrix4R32* self = Zeitgeist_allocateForeignObject(state, sizeof(Matrix4R32), NULL, NULL);
	idlib_matrix_4x4_f32_set_translate(&self->m, &translate->v);
	return self;
}

Matrix4R32*
Matrix4R32_createRotateY
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 degrees
	)
{
	Matrix4R32* self = Zeitgeist_allocateForeignObject(state, sizeof(Matrix4R32), NULL, NULL);
	idlib_matrix_4x4_f32_set_rotation_y(&self->m, degrees);
	return self;
}

Matrix4R32*
Matrix4R32_createPerspective
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 fieldOfViewY,
		Zeitgeist_Real32 aspectRatio,
		Zeitgeist_Real32 near,
		Zeitgeist_Real32 far
	)
{
	Matrix4R32* self = Zeitgeist_allocateForeignObject(state, sizeof(Matrix4R32), NULL, NULL);
	idlib_matrix_4x4_f32_set_perspective(&self->m, fieldOfViewY, aspectRatio, near, far);
	return self;
}

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
	)
{
	Matrix4R32* self = Zeitgeist_allocateForeignObject(state, sizeof(Matrix4R32), NULL, NULL);
	idlib_matrix_4x4_f32_set_orthographic(&self->m, left, right, bottom, top, near, far);
	return self;
}

Matrix4R32*
Matrix4R32_createLookAt
	(
		Zeitgeist_State* state,
		Vector3R32* source,
		Vector3R32* target,
		Vector3R32* up
	)
{
	Matrix4R32* self = Zeitgeist_allocateForeignObject(state, sizeof(Matrix4R32), NULL, NULL);
	idlib_matrix_4x4_f32_set_look_at(&self->m, &source->v, &target->v, &up->v);
	return self;
}

Matrix4R32*
Matrix4R32_createScale
	(
		Zeitgeist_State* state,
		Vector3R32* scale
	)
{
	Matrix4R32* self = Zeitgeist_allocateForeignObject(state, sizeof(Matrix4R32), NULL, NULL);
	idlib_matrix_4x4_f32_set_scale(&self->m, &scale->v);
	return self;
}

Matrix4R32*
Matrix4R32_negate
	(
		Zeitgeist_State* state,
		Matrix4R32* matrix
	)
{
	Matrix4R32* self = Zeitgeist_allocateForeignObject(state, sizeof(Matrix4R32), NULL, NULL);
	idlib_matrix_4x4_f32_negate(&self->m, &matrix->m);
	return self;
}

Matrix4R32*
Matrix4R32_multiply
	(
		Zeitgeist_State* state,
		Matrix4R32* operand1,
		Matrix4R32* operand2
	)
{
	Matrix4R32* self = Zeitgeist_allocateForeignObject(state, sizeof(Matrix4R32), NULL, NULL);
	idlib_matrix_4x4_f32_multiply(&self->m, &operand1->m, &operand2->m);
	return self;
}
