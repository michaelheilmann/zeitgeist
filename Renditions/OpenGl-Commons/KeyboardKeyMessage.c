#include "KeyboardKeyMessage.h"

// malloc
#include <malloc.h>

KeyboardKeyMessage*
KeyboardKeyMessage_create
	(
		Zeitgeist_State* state,
		Zeitgeist_Integer action,
		Zeitgeist_Integer key
	)
{
	KeyboardKeyMessage* self = malloc(sizeof(KeyboardKeyMessage));
	if (!self) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}

	self->action = action;
	self->key = key;

	((Zeitgeist_Object*)self)->finalize = NULL;
	((Zeitgeist_Object*)self)->visit = NULL;

	((Zeitgeist_Gc_Object*)self)->typeTag = Zeitgeist_Gc_TypeTag_Object;
	((Zeitgeist_Gc_Object*)self)->next = state->gc.all;
	state->gc.all = (Zeitgeist_Gc_Object*)self;
	((Zeitgeist_Gc_Object*)self)->color = Zeitgeist_Gc_Color_White;
	((Zeitgeist_Object*)self)->gclist = NULL;

	return self;
}
