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
	KeyboardKeyMessage* self = Zeitgeist_allocateForeignObject(state, sizeof(KeyboardKeyMessage), NULL, NULL);

	self->action = action;
	self->key = key;

	return self;
}
