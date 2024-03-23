#include "Vector3R32.h"

Vector3R32*
Vector3R32_create
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 x,
		Zeitgeist_Real32 y,
		Zeitgeist_Real32 z
	)
{
	Vector3R32* self = Zeitgeist_allocateForeignObject(state, sizeof(Vector3R32), NULL, NULL);
	idlib_vector_3_f32_set(&self->v, x, y, z);
	return self;
}
