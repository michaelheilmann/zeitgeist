#if !defined(KEYBOARDKEYMESSAGE_H_INCLUDED)
#define KEYBOARDKEYMESSAGE_H_INCLUDED

#include "Zeitgeist.h"

#define KeyboardKey_Action_Released (0)
#define KeyboardKey_Action_Pressed (1)

#define KeyboardKey_Up (1)
#define KeyboardKey_Down (2)
#define KeyboardKey_Left (3)
#define KeyboardKey_Right (4)
#define KeyboardKey_Escape (5)

typedef struct KeyboardKeyMessage {
	Zeitgeist_ForeignObject _parent;
	Zeitgeist_Integer action;
	Zeitgeist_Integer key;
} KeyboardKeyMessage;

KeyboardKeyMessage*
KeyboardKeyMessage_create
	(
		Zeitgeist_State* state,
		Zeitgeist_Integer action,
		Zeitgeist_Integer key
	);

Zeitgeist_Integer
KeyboardKeyMessage_getAction
	(
		Zeitgeist_State* state,
		KeyboardKeyMessage* self
	);

Zeitgeist_Integer
KeyboardKeyMessage_getKey
	(
		Zeitgeist_State* state,
		KeyboardKeyMessage* self
	);

#endif // KEYBOARDKEYMESSAGE_H_INCLUDED
