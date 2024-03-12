#include "Vector3R32.h"

// malloc
#include <malloc.h>

Vector3R32*
Vector3R32_create
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 x,
		Zeitgeist_Real32 y,
		Zeitgeist_Real32 z
	)
{
	Vector3R32* self = malloc(sizeof(Vector3R32));
	if (!self) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	self->e[0] = x;
	self->e[1] = y;
	self->e[2] = z;

	((Zeitgeist_Object*)self)->finalize = NULL;
	((Zeitgeist_Object*)self)->visit = NULL;

	((Zeitgeist_Gc_Object*)self)->typeTag = Zeitgeist_Gc_TypeTag_Object;
	((Zeitgeist_Gc_Object*)self)->next = state->gc.all;
	state->gc.all = (Zeitgeist_Gc_Object*)self;
	((Zeitgeist_Gc_Object*)self)->color = Zeitgeist_Gc_Color_White;
	((Zeitgeist_Object*)self)->gclist = NULL;

	return self;
}
