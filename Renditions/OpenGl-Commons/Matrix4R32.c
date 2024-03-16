#include "Matrix4R32.h"

// malloc
#include <malloc.h>

#include "Vector3R32.h"


Matrix4R32*
Matrix4R32_createIdentity
	(
		Zeitgeist_State* state
	)
{
	Matrix4R32* self = Zeitgeist_allocateObject(state, sizeof(Matrix4R32), NULL, NULL);

	self->e[0][0] = 1.f;
	self->e[0][1] = 0.f;
	self->e[0][2] = 0.f;
	self->e[0][3] = 0.f;

	self->e[1][0] = 0.f;
	self->e[1][1] = 1.f;
	self->e[1][2] = 0.f;
	self->e[1][3] = 0.f;

	self->e[2][0] = 0.f;
	self->e[2][1] = 0.f;
	self->e[2][2] = 1.f;
	self->e[2][3] = 0.f;

	self->e[3][0] = 0.f;
	self->e[3][1] = 0.f;
	self->e[3][2] = 0.f;
	self->e[3][3] = 1.f;

	return self;
}

Matrix4R32*
Matrix4R32_createTranslate
	(
		Zeitgeist_State* state,
		Vector3R32* translate
	)
{
	Matrix4R32* self = malloc(sizeof(Matrix4R32));
	if (!self) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}

	self->e[0][0] = 1.f;
	self->e[0][1] = 0.f;
	self->e[0][2] = 0.f;
	self->e[0][3] = 0.f;

	self->e[1][0] = 0.f;
	self->e[1][1] = 1.f;
	self->e[1][2] = 0.f;
	self->e[1][3] = 0.f;

	self->e[2][0] = 0.f;
	self->e[2][1] = 0.f;
	self->e[2][2] = 1.f;
	self->e[2][3] = 0.f;

	self->e[3][0] = translate->e[0];
	self->e[3][1] = translate->e[1];
	self->e[3][2] = translate->e[2];
	self->e[3][3] = 1.f;

	((Zeitgeist_Object*)self)->finalize = NULL;
	((Zeitgeist_Object*)self)->visit = NULL;

	((Zeitgeist_Gc_Object*)self)->typeTag = Zeitgeist_Gc_TypeTag_Object;
	((Zeitgeist_Gc_Object*)self)->next = state->gc.all;
	state->gc.all = (Zeitgeist_Gc_Object*)self;
	((Zeitgeist_Gc_Object*)self)->color = Zeitgeist_Gc_Color_White;
	((Zeitgeist_Object*)self)->gclist = NULL;

	return self;
}
