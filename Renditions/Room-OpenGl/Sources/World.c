#include "World.h"

#include "Zeitgeist/List.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void
Player_visit
	(
		Zeitgeist_State* state,
		Player* self
	)
{
	if (self->position) {
		Zeitgeist_Gc_visitForeignObject(state, (Zeitgeist_ForeignObject*)self->position);
	}
}

Player*
Player_create
	(
		Zeitgeist_State* state
	)
{
	Player* self = Zeitgeist_allocateForeignObject(state, sizeof(Player), NULL, NULL);
	self->position = Vector3R32_create(state, 0.f, 0.f, 0.f);
	self->rotationY = 0.f;
	((Zeitgeist_ForeignObject*)self)->visit = (void(*)(Zeitgeist_State*, Zeitgeist_ForeignObject*)) & Player_visit;
	return self;
}

void
Player_onKeyboardKeyMessage
	(
		Zeitgeist_State* state,
		Player* self,
		KeyboardKeyMessage* message
	)
{ }


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void
StaticGeometryGl_finalize
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	)
{
	StaticGeometryGl_unmaterialize(state, self);
}

void
StaticGeometryGl_unmaterialize
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	)
{
	if (self->vertexArrayId) {
		glDeleteVertexArrays(1, &self->vertexArrayId);
		self->vertexArrayId = 0;
	}
	if (self->bufferId) {
		glDeleteBuffers(1, &self->bufferId);
		self->bufferId = 0;
	}
}

StaticGeometryGl*
StaticGeometryGl_create
	(
		Zeitgeist_State* state
	)
{
	StaticGeometryGl* self = Zeitgeist_allocateForeignObject(state, sizeof(StaticGeometryGl), NULL, NULL);

	self->numberOfVertices = 0;
	self->bufferId = 0;
	self->vertexArrayId = 0;

	while (glGetError()) { }
	glGenBuffers(1, &self->bufferId);
	if (glGetError()) {
		fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenBuffers");
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	glGenVertexArrays(1, &self->vertexArrayId);
	if (glGetError()) {
		glDeleteBuffers(1, &self->bufferId);
		self->bufferId = 0;
		fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenVertexArrays");
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}

	((Zeitgeist_ForeignObject*)self)->finalize = (void(*)(Zeitgeist_State*, Zeitgeist_ForeignObject*)) & StaticGeometryGl_finalize;
	return self;
}

void
StaticGeometryGl_setData
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		size_t numberOfVertices,
		size_t numberOfBytes,
		void const* bytes
	)
{
	static const GLint POSITION_INDEX = 0;
	
	static const GLint AMBIENT_COLOR_INDEX = 1;

	// Compute vertex size.
	size_t vertexSize = numberOfBytes / numberOfVertices;
	
	// Warn if this does not hold. 
	if (numberOfBytes % vertexSize) {
		fprintf(stderr, "%s:%d: warning: additional %zu Bytes at the end of the data\n", __FILE__, __LINE__, numberOfBytes % vertexSize);
	}

	// Store the data in the buffer.
	glBindBuffer(GL_ARRAY_BUFFER, self->bufferId);
	self->numberOfVertices = numberOfVertices;
	self->numberOfBytes = numberOfBytes;
	glBufferData(GL_ARRAY_BUFFER, numberOfBytes, bytes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(self->vertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, self->bufferId);

	glEnableVertexAttribArray(POSITION_INDEX);
	glVertexAttribPointer(POSITION_INDEX,
		                    3,
		                    GL_FLOAT,
		                    GL_FALSE,
		                    vertexSize,
		                    (void*)(uintptr_t)0);

	glEnableVertexAttribArray(AMBIENT_COLOR_INDEX);
	glVertexAttribPointer(AMBIENT_COLOR_INDEX,
		                    4,
		                    GL_FLOAT,
		                    GL_TRUE,
		                    vertexSize,
		                    (void*)(uintptr_t)(sizeof(float)*3));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void
StaticGeometryGl_setDataSouthWall
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	)
{
	idlib_color_4_f32 ambientColor;
	idlib_color_convert_3_u8_to_4_f32(&ambientColor, &idlib_colors_lightgray_3_u8, 1.f);
	struct VERTEX {
		idlib_vector_3_f32 position;
		idlib_color_4_f32 ambientColor;
	};
	struct VERTEX vertices[] = {
		{.position = { -1.f,  1.f, -1.f, }, .ambientColor = ambientColor },
		{.position = { -1.f, -1.f, -1.f, }, .ambientColor = ambientColor, },
		{.position = {  1.f,  1.f, -1.f, }, .ambientColor = ambientColor, },
		{.position = {  1.f, -1.f, -1.f, }, .ambientColor = ambientColor, },
	};

#if 0
	idlib_vector_3_f32 x, y;
	idlib_vector_3_f32_subtract(&x, &vertices[1].position, &vertices[0].position);
	idlib_vector_3_f32_subtract(&y, &vertices[2].position, &vertices[0].position);
	idlib_vector_3_f32_cross(&x, &x, &y);

	idlib_vector_3_f32_subtract(&x, &vertices[2].position, &vertices[1].position);
	idlib_vector_3_f32_subtract(&y, &vertices[3].position, &vertices[1].position);
	idlib_vector_3_f32_cross(&x, &x, &y);
#endif

	size_t numberOfVertices = sizeof(vertices) / sizeof(struct VERTEX);
	StaticGeometryGl_setData(state, self, numberOfVertices, sizeof(vertices), vertices);
	self->flags = SOUTH_FACING_WALL;
	self->left = -1;
	self->right = +1;
	self->bottom = -1;
	self->top = +1;
}

void
StaticGeometryGl_setDataNorthWall
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	)
{
	struct VERTEX {
		idlib_vector_3_f32 position;
		idlib_color_4_f32 ambientColor;
	};
	struct VERTEX vertices[] = {
		{.position = {  1.f,  1.f, +1.f, }, .ambientColor = { 1.f, 0.f, 0.f, 1.f }, },
		{.position = {  1.f, -1.f, +1.f, }, .ambientColor = { 1.f, 1.f, 0.f, 1.f }, },
		{.position = { -1.f,  1.f, +1.f, }, .ambientColor = { 1.f, 0.f, 1.f, 1.f }, },
		{.position = { -1.f, -1.f, +1.f, }, .ambientColor = { 1.f, 1.f, 1.f, 1.f }, },
	};

#if 0
	idlib_vector_3_f32 x, y;
	idlib_vector_3_f32_subtract(&x, &vertices[1].position, &vertices[0].position);
	idlib_vector_3_f32_subtract(&y, &vertices[2].position, &vertices[0].position);
	idlib_vector_3_f32_cross(&x, &x, &y);

	idlib_vector_3_f32_subtract(&x, &vertices[2].position, &vertices[1].position);
	idlib_vector_3_f32_subtract(&y, &vertices[3].position, &vertices[1].position);
	idlib_vector_3_f32_cross(&x, &x, &y);
#endif

	size_t numberOfVertices = sizeof(vertices) / sizeof(struct VERTEX);
	StaticGeometryGl_setData(state, self, numberOfVertices, sizeof(vertices), vertices);
	self->flags = NORTH_FACING_WALL;
	self->left = -1;
	self->right = +1;
	self->bottom = -1;
	self->top = +1;
}

void
StaticGeometryGl_setDataWestWall
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	)
{
	idlib_color_4_f32 ambientColor;
	idlib_color_convert_3_u8_to_4_f32(&ambientColor, &idlib_colors_magenta_3_u8, 1.f);
	struct VERTEX {
		idlib_vector_3_f32 position;
		idlib_color_4_f32 ambientColor;
	};
	struct VERTEX vertices[] = {
		{.position = { +1.f,  1.f, -1.f, }, .ambientColor = ambientColor, },
		{.position = { +1.f, -1.f, -1.f, }, .ambientColor = ambientColor, },
		{.position = { +1.f,  1.f, +1.f, }, .ambientColor = ambientColor, },
		{.position = { +1.f, -1.f, +1.f, }, .ambientColor = ambientColor, },
	};

#if 0
	idlib_vector_3_f32 x, y;
	idlib_vector_3_f32_subtract(&x, &vertices[1].position, &vertices[0].position);
	idlib_vector_3_f32_subtract(&y, &vertices[2].position, &vertices[0].position);
	idlib_vector_3_f32_cross(&x, &x, &y);

	idlib_vector_3_f32_subtract(&x, &vertices[2].position, &vertices[1].position);
	idlib_vector_3_f32_subtract(&y, &vertices[3].position, &vertices[1].position);
	idlib_vector_3_f32_cross(&x, &x, &y);
#endif

	size_t numberOfVertices = sizeof(vertices) / sizeof(struct VERTEX);
	StaticGeometryGl_setData(state, self, numberOfVertices, sizeof(vertices), vertices);
	self->flags = WEST_FACING_WALL;
	self->left = -1;
	self->right = +1;
	self->bottom = -1;
	self->top = +1;
}

void
StaticGeometryGl_setDataEastWall
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	)
{
	idlib_color_4_f32 ambientColor;
	idlib_color_convert_3_u8_to_4_f32(&ambientColor, &idlib_colors_lightgray_3_u8, 1.f);
	struct VERTEX {
		idlib_vector_3_f32 position;
		idlib_color_4_f32 ambientColor;
	};
	struct VERTEX vertices[] = {
		{.position = { -1.f,  1.f, +1.f, }, .ambientColor = ambientColor },
		{.position = { -1.f, -1.f, +1.f, }, .ambientColor = ambientColor, },
		{.position = { -1.f,  1.f, -1.f, }, .ambientColor = ambientColor, },
		{.position = { -1.f, -1.f, -1.f, }, .ambientColor = ambientColor, },
	};

#if 0
	idlib_vector_3_f32 x, y;
	idlib_vector_3_f32_subtract(&x, &vertices[1].position, &vertices[0].position);
	idlib_vector_3_f32_subtract(&y, &vertices[2].position, &vertices[0].position);
	idlib_vector_3_f32_cross(&x, &x, &y);

	idlib_vector_3_f32_subtract(&x, &vertices[2].position, &vertices[1].position);
	idlib_vector_3_f32_subtract(&y, &vertices[3].position, &vertices[1].position);
	idlib_vector_3_f32_cross(&x, &x, &y);
#endif

	size_t numberOfVertices = sizeof(vertices) / sizeof(struct VERTEX);
	StaticGeometryGl_setData(state, self, numberOfVertices, sizeof(vertices), vertices);
	self->flags = EAST_FACING_WALL;
	self->left = -1;
	self->right = +1;
	self->bottom = -1;
	self->top = +1;
}

void
StaticGeometryGl_setDataFloor
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	)
{
	idlib_color_4_f32 ambientColor;
	idlib_color_convert_3_u8_to_4_f32(&ambientColor, &idlib_colors_lightgray_3_u8, 1.f);
	struct VERTEX {
		idlib_vector_3_f32 position;
		idlib_color_4_f32 ambientColor;
	};
	struct VERTEX vertices[] = {
		{.position = { -1.f, -1.f, -1.f, }, .ambientColor = ambientColor, },
		{.position = { -1.f, -1.f,  1.f, }, .ambientColor = ambientColor, },
		{.position = {  1.f, -1.f, -1.f, }, .ambientColor = ambientColor, },
		{.position = {  1.f, -1.f,  1.f, }, .ambientColor = ambientColor, },
	};

#if 0
	idlib_vector_3_f32 x, y;
	idlib_vector_3_f32_subtract(&x, &vertices[1].position, &vertices[0].position);
	idlib_vector_3_f32_subtract(&y, &vertices[2].position, &vertices[0].position);
	idlib_vector_3_f32_cross(&x, &x, &y);

	idlib_vector_3_f32_subtract(&x, &vertices[2].position, &vertices[1].position);
	idlib_vector_3_f32_subtract(&y, &vertices[3].position, &vertices[1].position);
	idlib_vector_3_f32_cross(&x, &x, &y);
#endif

	size_t numberOfVertices = sizeof(vertices) / sizeof(struct VERTEX);
	StaticGeometryGl_setData(state, self, numberOfVertices, sizeof(vertices), vertices);
	self->flags = FLOOR;
	self->left = -1;
	self->right = +1;
	self->bottom = -1;
	self->top = +1;
}

void
StaticGeometryGl_setDataCeiling
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	)
{
	idlib_color_4_f32 ambientColor;
	idlib_color_convert_3_u8_to_4_f32(&ambientColor, &idlib_colors_lightgray_3_u8, 1.f);
	struct VERTEX {
		idlib_vector_3_f32 position;
		idlib_color_4_f32 ambientColor;
	};
	struct VERTEX vertices[] = {
		{.position = { -1.f, +1.f,  1.f, }, .ambientColor = ambientColor, },
		{.position = { -1.f, +1.f, -1.f, }, .ambientColor = ambientColor, },
		{.position = {  1.f, +1.f,  1.f, }, .ambientColor = ambientColor, },
		{.position = {  1.f, +1.f, -1.f, }, .ambientColor = ambientColor, },
	};

#if 0
	idlib_vector_3_f32 x, y;
	idlib_vector_3_f32_subtract(&x, &vertices[1].position, &vertices[0].position);
	idlib_vector_3_f32_subtract(&y, &vertices[2].position, &vertices[0].position);
	idlib_vector_3_f32_cross(&x, &x, &y);

	idlib_vector_3_f32_subtract(&x, &vertices[2].position, &vertices[1].position);
	idlib_vector_3_f32_subtract(&y, &vertices[3].position, &vertices[1].position);
	idlib_vector_3_f32_cross(&x, &x, &y);
#endif

	size_t numberOfVertices = sizeof(vertices) / sizeof(struct VERTEX);
	StaticGeometryGl_setData(state, self, numberOfVertices, sizeof(vertices), vertices);
	self->flags = CEILING;
	self->left = -1;
	self->right = +1;
	self->bottom = -1;
	self->top = +1;
}

static void
World_visit
	(
		Zeitgeist_State* state,
		World* self
	)
{
	if (self->geometries) {
		Zeitgeist_Gc_visitForeignObject(state, (Zeitgeist_ForeignObject*)self->geometries);
	}
	if (self->player) {
		Zeitgeist_Gc_visitForeignObject(state, (Zeitgeist_ForeignObject*)self->player);
	}
}

World*
World_create
	(
		Zeitgeist_State* state
	)
{
	World* self = Zeitgeist_allocateForeignObject(state, sizeof(World), NULL, NULL);
	self->player = NULL;
	self->geometries = NULL;

	self->geometries = Zeitgeist_List_create(state);
	
	StaticGeometryGl* geometry = NULL;
	
	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataSouthWall(state, geometry);
	Zeitgeist_List_appendForeignObject(state, self->geometries, (Zeitgeist_ForeignObject*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataFloor(state, geometry);
	Zeitgeist_List_appendForeignObject(state, self->geometries, (Zeitgeist_ForeignObject*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataCeiling(state, geometry);
	Zeitgeist_List_appendForeignObject(state, self->geometries, (Zeitgeist_ForeignObject*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataEastWall(state, geometry);
	Zeitgeist_List_appendForeignObject(state, self->geometries, (Zeitgeist_ForeignObject*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataWestWall(state, geometry);
	Zeitgeist_List_appendForeignObject(state, self->geometries, (Zeitgeist_ForeignObject*)geometry);

	self->player = Player_create(state);

	((Zeitgeist_ForeignObject*)self)->visit = (void(*)(Zeitgeist_State*, Zeitgeist_ForeignObject*)) & World_visit;
	return self;
}
