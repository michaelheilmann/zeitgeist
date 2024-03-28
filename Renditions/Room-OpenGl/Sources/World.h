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
	Vector3R32* positionSpeed;
	/**
	 * @brief A scalar indicating the rotation of the player around the Y axis.
	 * @remarks The default direction is (0,0,-1), the default rotation is 0.
	 */
	Zeitgeist_Real32 rotationY;
	Zeitgeist_Real32 rotationYSpeed;

	/**
	 * @brief If "strate left" is down.
	 */
	bool strafeLeftDown;
	/**
	 * @brief If "strafe right" is down.
	 */
	bool strafeRightDown;

	/**
	 * @brief If "move forward" is down.
	 */
	bool moveForwardDown;

	/**
   * @brief If "move backward" is down.
	 */
	bool moveBackwardDown;

	/**
   * @brief If "turn left" is down.
	 */
	bool turnLeftDown;

	/**
	 * @brief If "turn right is down.
	 */
	bool turnRightDown;
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

void
Player_update
	(
		Zeitgeist_State* state,
		Player* self,
		Zeitgeist_Real32 tick
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.2
 * Indicates the geometry of the south wall (6 o'clock) of a room.
 * The south wall has the normal (0,0,-1).
 */
#define SOUTH_WALL (1)

/**
 * @since 0.2
 * Indicates the geometry of the east wall (3 o'clock) of a room.
 * The east wall has the the normal (-1,0,0).
 */
#define EAST_WALL (2)

/**
 * @since 0.2
 * Indicates the geometry of the north wall (12 o'clock) of a room.
 * The north wall has the normal (0,0,+1).
 */
#define NORTH_WALL (3)

/**
 * @since 0.2
 * Indicates the geometry of the west wall (9 o'clock) of a room.
 * The west wall has the normal (+1,0,0).
 */
#define WEST_WALL (4)

/**
 * @since 2.0
 * Indicates the geometry of the floor of a room.
 * The floor has the normal (0,+1,0).
 */
#define FLOOR (5)

/**
 * @since 2.0
 * Indicates geometry of the ceiling of a room.
 * The ceiling has the normal (0,-1,0).
 */
#define CEILING (6)

/**
 * @since 0.1
 * @brief
 * The retained mode representation of a part of a building's geometry.
 * A StaticGeometryGl object represents one of the following:
 * - a floor
 * - a ceiling
 * - a west, north, east, or south walls
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
	
	/// @brief Either FLOOR or CEILING, WEST_WALL, NORTH_WALL, EAST_WALL, or SOUT_WALL.
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
 * @brief Set the vertex data for the geometry of the north wall (12 o'clock).
 * The north wall has the normal (0,0,+1).
 * @param breadth The extend along the x-axis. Must be positive.
 * @param height The extend along the y-axis. Must be positive.
 */
void
StaticGeometryGl_setDataNorthWall
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		Zeitgeist_Real32 breadth,
		Zeitgeist_Real32 height
	);

/**
 * @brief Set the vertex data for the geometry of the south wall (6 o'clock).
 * The south wall has the normal (0,0,-1).
 * @param breadth The extend along the x-axis. Must be positive.
 * @param height The extend along the y-axis. Must be positive.
 */
void
StaticGeometryGl_setDataSouthWall
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		Zeitgeist_Real32 breadth,
		Zeitgeist_Real32 height
	);

/**
 * @brief Set the vertex data for the geometry of the east wall (3 o'clock).
 * The east wall has the normal (-1,0,0).
 * @param breadth The extend along the z-axis. Must be positive.
 * @param height The extend along the y-axis. Must be positive.
 */
void
StaticGeometryGl_setDataEastWall
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		Zeitgeist_Real32 breadth,
		Zeitgeist_Real32 height
	);

/**
 * @brief Set the vertex data for the geometry of the west wall (9 o'clock).
 * The west wall has the normal (+1,0,0).
 * @param breadth The extend along the z-axis. Must be positive.
 * @param height The extend along the y-axis. Must be positive.
 */
void
StaticGeometryGl_setDataWestWall
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		Zeitgeist_Real32 breadth,
		Zeitgeist_Real32 height
	);

/**
 * @brief Set the vertex data for the geometry of the floor.
 * The floor has the normal (0,+1,0).
 * @param breadth The breadth (the extend along the x-axis). Must be positive.
 * @param length The length (the extend along the z-axis). Must be positive.
 */
void
StaticGeometryGl_setDataFloor
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		Zeitgeist_Real32 breadth,
		Zeitgeist_Real32 length
	);

/**
 * @brief Set the vertex data for the geometry of the ceiling.
 * The ceiling has the normal (0,+1,0).
 * @param breadth The breadth (the extend along the x-axis). Must be positive.
 * @param length The length (the extend along the z-axis). Must be positive.
 */
void
StaticGeometryGl_setDataCeiling
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		Zeitgeist_Real32 breadth,
		Zeitgeist_Real32 length
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

void
World_update
	(
		Zeitgeist_State* state,
		World* self,
		Zeitgeist_Real32 tick
	);

#endif // WORLD_H_INCLUDED
