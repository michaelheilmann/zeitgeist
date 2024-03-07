#if !defined(ZEITGEIST_OBJECT_H_INCLUDED)
#define ZEITGEIST_OBJECT_H_INCLUDED

#include "Zeitgeist/Value.h"

struct Zeitgeist_Object {
  Zeitgeist_Object* next;
  void (*finalize)(Zeitgeist_State* state, Zeitgeist_Object* object);
};

#endif // ZEITGEIST_OBJECT_H_INCLUDED
