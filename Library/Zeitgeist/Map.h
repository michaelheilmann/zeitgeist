// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_MAP_H_INCLUDED)
#define ZEITGEIST_MAP_H_INCLUDED

#include "Zeitgeist/Map.h"
#include "Zeitgeist/Value.h"

/**
 * @since 0.1
 * @brief A node of a map. 
 * @unmanaged
 */
typedef struct Zeitgeist_Map_Node Zeitgeist_Map_Node;

/**
 * @since 0.1
 * @brief A map.
 */
typedef struct Zeitgeist_Map Zeitgeist_Map;

struct Zeitgeist_Map_Node {
  Zeitgeist_Map_Node* next;
  Zeitgeist_Value key;
  Zeitgeist_Value value;
};

struct Zeitgeist_Map {
  Zeitgeist_Gc_Object _parent;
  Zeitgeist_Map_Node** buckets;
  size_t size;
  size_t capacity;
};

/**
 * @since 0.1
 * @brief Create a Zeitgeist_Map object.
 * @param state A pointer to the Zeitgeist_State object.
 * @return A pointer to the Zeitgeist_List object.
 * @undefined state does not point to a Zeitgeist_State object.
 */
Zeitgeist_Map*
Zeitgeist_State_createMap
  (
    Zeitgeist_State* state
  );

#endif // ZEITGEIST_MAP_H_INCLUDED
