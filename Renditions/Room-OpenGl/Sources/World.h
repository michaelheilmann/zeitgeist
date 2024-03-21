#if !defined(WORLD_H_INCLUDED)
#define WORLD_H_INCLUDED

#include "Zeitgeist.h"

#include "Matrix4R32.h"
#include "Vector3R32.h"
#include "ServiceGl.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since
 * 0.1
 * @brief
 * Retained mode representation of the building's geometry.
 */
typedef struct StaticGeometryGl StaticGeometryGl;

struct StaticGeometryGl {
	Zeitgeist_ForeignObject parent;
	// The OpenGL ID of the vertex buffer.
	GLuint bufferId;
	// The OpenGL ID of the vertex array.
	GLuint vertexArrayId;
};

void
StaticGeometryGl_unmaterialize
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	);

StaticGeometryGl*
StaticGeometryGl_create
	(
		Zeitgeist_State* state
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct World World;

struct World {
	Zeitgeist_ForeignObject _parent;
	/** @brief Information on the player. */
	struct {
		/** A position vector indicating the position. */
		Vector3R32* position;
		/** A direction vector indicating the direction. */
		Vector3R32* direction;
	} player;
};

World*
World_create
	(
		Zeitgeist_State* state
	);

void
World_render
	(
		Zeitgeist_State* state
	);

#endif // WORLD_H_INCLUDED
