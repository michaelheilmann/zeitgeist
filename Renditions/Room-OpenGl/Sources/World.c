#include "World.h"

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

World*
World_create
	(
		Zeitgeist_State* state
	)
{
	World* self = Zeitgeist_allocateForeignObject(state, sizeof(World), NULL, NULL);
	return self;
}

void
World_render
	(
		Zeitgeist_State* state
	)
{
	
}
