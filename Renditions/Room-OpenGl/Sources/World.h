#if !defined(WORLD_H_INCLUDED)
#define WORLD_H_INCLUDED

#include "Zeitgeist.h"

#include "Matrix4R32.h"
#include "Vector3R32.h"
#include "ServiceGl.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @brief The state of the player.
 */
typedef struct Player Player;

struct Player {
	Zeitgeist_ForeignObject parent;
	/**
	 * @brief A position vector indicating the position of the player in world coordinates.
	 * @remarks The default position is (0,0,0).
	 */
	Vector3R32* position;
	/**
	 * @brief A scalar indicating the rotation of the player around the Y axis.
	 * @remarks The default direction is (0,0,-1), the default rotation is 0.
	 */
	Zeitgeist_Real32 rotationY;
};

Player*
Player_create
	(
		Zeitgeist_State* state
	);

void
Player_onKeyboardKeyMessage
	(
		Zeitgeist_State* state,
		Player* self,
		KeyboardKeyMessage* message
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.2
 * Indicates "north-facing" wall geometry.
 * "north-facing wall" geometry has the normal (0,0,-1).
 */
#define NORTH_FACING_WALL (1)

/**
 * @since 0.2
 * Indicates "east-facing wall" geometry.
 * "east-facing wall" geometry has the normal (-1,0,0).
 */
#define EAST_FACING_WALL (2)

/**
 * @since 0.2
 * Indicates "south facing wall" geometry.
 * "south-facing wall" geometry has the normal (0,0,+1).
 */
#define SOUTH_FACING_WALL (3)

/**
 * @since 0.2
 * Indicates a west-facing wall geometry.
 * "west-facing wall" geometry has the normal (+1,0,0).
 */
#define WEST_FACING_WALL (4)

/**
 * @since 2.0
 * Indicates "floor" geometry.
 * "floor" geometry has the normal (0,+1,0).
 */
#define FLOOR (5)

/**
 * @since 2.0
 * Indicates "ceiling geometry.
 * "ceiling" geometry has the normal (0,-1,0).
 */
#define CEILING (6)

/**
 * @since
 * 0.1
 * @brief
 * Retained mode representation of the building's geometry.
 */
typedef struct StaticGeometryGl StaticGeometryGl;

struct StaticGeometryGl {
	Zeitgeist_ForeignObject parent;
	
	/// @brief The OpenGL ID of the vertex buffer.
	GLuint bufferId;
	
	/// @brief The OpenGL ID of the vertex array.
	GLuint vertexArrayId;
	
	/// @brief The number of vertices of this wall.
	size_t numberOfVertices;
	
	/// @brief The number of Bytes of this wall.
	size_t numberOfBytes;
	
	/// @brief Either WALL_DIRECTION_NORTH, WALL_DIRECTION_EAST, WALL_DIRECTION_SOUTH, or WALL_DIRECTION_WEST.
	uint8_t flags;

	/// @brief The coordinate of the bottom side of the wall in the plane of the wall
	/// using a coordinate system of x going right, y going up, negative z going into.
	Zeitgeist_Real32 bottom;
	
	/// @brief The coordinate of the top side of the wall in the plane of the wall
	/// using a coordinate system of x going right, y going up, negative z going into.
	Zeitgeist_Real32 top;

	/// @brief The coordinate of the left side of the wall in the plane of the wall.
	/// using a coordinate system of x going right, y going up, negative z going into.
	Zeitgeist_Real32 left;

	/// @brief The coordinate of the right side of the wall in the plane of the wall
	/// using a coordinate system of x going right, y going up, negative z going into.
	Zeitgeist_Real32 right;

};

void
StaticGeometryGl_unmaterialize
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	);

/**
 * @brief Set the vertex data.
 * @param state A pointer to the state.
 * @param self A pointer to to this StaticGeometryGl object.
 * @param numberOfVertices The size, in vertices, of the data.
 * @param numberOfBytes The size, in Bytes, of the data.
 * @param bytes A pointer to the data.
 */
void
StaticGeometryGl_setData
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		size_t numberOfVertices,
		size_t numberOfBytes,
		void const* bytes
	);

/**
 * @brief Set the vertex data to represent a south-facing wall.
 * "south-facing" means the normal is (0,0,+1).
 */
void
StaticGeometryGl_setDataSouthWall
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	);

/**
 * @brief Set the vertex data to represent a north-facing wall.
 * "north-facing" means the normal is (0,0,-1).
 */
void
StaticGeometryGl_setDataNorthWall
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	);

/**
 * @brief Set the vertex data to represent a west-facing wall.
 * "west-facing" the normal is (-1,0,0).
 */
void
StaticGeometryGl_setDataWestWall
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	);

/**
 * @brief Set the vertex data to represent a east-facing wall.
 * "east-facing" means the normal is (+1,0,0).
 */
void
StaticGeometryGl_setDataEastWall
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	);

/**
 * @brief Set the vertex data to represent a floor.
 * "floor" means the normal is (0,+1,0).
 */
void
StaticGeometryGl_setDataFloor
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	);

/**
 * @brief Set the vertex data to represent a ceiling.
 * "ceiling" means the normal is (0,+1,0).
 */
void
StaticGeometryGl_setDataCeiling
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
	/// @brief Pointer to the list of StaticGeometryGl objects.
	Zeitgeist_List* geometries;
	/// @brief Information on the player.
	Player* player;
};

World*
World_create
	(
		Zeitgeist_State* state
	);

#endif // WORLD_H_INCLUDED
