// Copyright (c) 2024 Michael Heilmann. All rights reserved.

// strlen
#include <string.h>

// fprintf, stdout
#include <stdio.h>

#include "Zeitgeist/UpstreamRequests.h"
#include "ServiceGl.h"

#include "Visuals/DefaultPrograms.h"
#include "Visuals/Program.h"
#include "Visuals/RenderBuffer.h"
#include "Visuals/VertexBuffer.h"

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #define Shizu_Rendition_Export _declspec(dllexport)
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  #define Shizu_Rendition_Export
#else
  #error("operating system not (yet) supported")
#endif

Shizu_Rendition_Export char const*
Shizu_Module_getName
  (
  )
{
  static char const* NAME = "<Dl:Hello World (OpenGL)>";
  return NAME;
}

Shizu_Rendition_Export Shizu_String*
Zeitgeist_Rendition_getName
  (
    Shizu_State* state
  )
{
  return Shizu_String_create(state, "Hello World (OpenGL)", strlen("Hello World (OpenGL)"));
}

static GLint Positions_Index = 0;

typedef struct VERTEX {
  float x, y, z;
} VERTEX;

static VERTEX const SQUARE[] = {
  { -1.0f,  1.0f, 0.f, },
  { -1.0f, -1.0f, 0.f, },
  {  1.0f,  1.0f, 0.f, },
  {  1.0f, -1.0f, 0.f, },
};

static Visuals_Program* g_program = NULL;
static Visuals_VertexBuffer* g_vertexBuffer = NULL;
static Visuals_RenderBuffer* g_renderBuffer = NULL;

Shizu_Rendition_Export void
Zeitgeist_Rendition_update
  (
    Shizu_State* state
  )
{
  ServiceGl_update(state);

  if (ServiceGl_quitRequested(state)) {
    Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
    Zeitgeist_sendUpstreamRequest(state, request);
  }

  Shizu_Integer32 viewportWidth, viewportHeight;
  ServiceGl_getClientSize(state, &viewportWidth, &viewportHeight);
  ServiceGl_beginFrame(state);

  glViewport(0, 0, viewportWidth, viewportHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(((Visuals_GlProgram*)g_program)->programId);

  GLint location;
  
  location = glGetUniformLocation(((Visuals_GlProgram*)g_program)->programId, "scale");
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of `%s`\n", __FILE__, __LINE__, "scale");
  } else {
    GLfloat matrix[16] = {
      .75f, 0.f,  0.f, 0.f,
      0.f,  .75f, 0.f, 0.f,
      0.f,  0.f,  1.f, 0.f,
      0.f,  0.f,  0.f, 1.f,
    };
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0]);
  }
  
  location = glGetUniformLocation(((Visuals_GlProgram*)g_program)->programId, "inputColor");
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of `%s`\n", __FILE__, __LINE__, "inputColor");
  } else {
    // The color (255, 204, 51) is the websafe color "sunglow".
    GLfloat vector[4] = { 1.0, 0.8, 0.2, 1.0 };
    glUniform4fv(location, 1, &vector[0]);
  }

  glBindVertexArray(((Visuals_GlVertexBuffer*)g_vertexBuffer)->vertexArrayId);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(SQUARE)/ sizeof(VERTEX));
  glBindVertexArray(0);
  glUseProgram(0);

  ServiceGl_endFrame(state);
}

Shizu_Rendition_Export void
Zeitgeist_Rendition_load
  (
    Shizu_State* state
  )
{
  Shizu_JumpTarget jumpTarget;

  ServiceGl_startup(state);

  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    ServiceGl_setTitle(state, Shizu_String_create(state, "Hello World (OpenGL)", strlen("Hello World (OpenGL)")));
    Visuals_Program* program = Visuals_getProgram(state, "simple");
    Visuals_Object_materialize(state, (Visuals_Object*)program);
    Shizu_Object_lock(state, (Shizu_Object*)program);
    g_program = program;
    Visuals_VertexBuffer* vertexBuffer = (Visuals_VertexBuffer*)Visuals_GlVertexBuffer_create(state);
    Visuals_VertexBuffer_setData(state, vertexBuffer, Visuals_VertexSemantics_Position3d | Visuals_VertexSyntactics_Float3, SQUARE, sizeof(SQUARE));
    Shizu_Object_lock(state, (Shizu_Object*)vertexBuffer);
    g_vertexBuffer = vertexBuffer;
    Visuals_RenderBuffer* renderBuffer = (Visuals_RenderBuffer*)Visuals_GlRenderBuffer_create(state);
    Shizu_Object_lock(state, (Shizu_Object*)renderBuffer);
    g_renderBuffer = renderBuffer;
    Shizu_State_popJumpTarget(state);
  } else {
    Shizu_State_popJumpTarget(state);
    if (g_renderBuffer) {
      Visuals_Object_unmaterialize(state, (Visuals_Object*)g_renderBuffer);
      Shizu_Object_unlock(state, (Shizu_Object*)g_renderBuffer);
      g_renderBuffer = NULL;
    }
    if (g_vertexBuffer) {
      Visuals_Object_unmaterialize(state, (Visuals_Object*)g_vertexBuffer);
      Shizu_Object_unlock(state, (Shizu_Object*)g_vertexBuffer);
      g_vertexBuffer = NULL;
    }
    if (g_program) {
      Visuals_Object_unmaterialize(state, (Visuals_Object*)g_program);
      Shizu_Object_unlock(state, (Shizu_Object*)g_program);
      g_program = NULL;
    }
    ServiceGl_shutdown(state);
    Shizu_State_jump(state);
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0f, 0.0f, 0.0f, 0.f);
  glClearDepth(1.f);
}

Shizu_Rendition_Export void
Zeitgeist_Rendition_unload
  (
    Shizu_State* state
  )
{
  if (g_renderBuffer) {
    Visuals_Object_unmaterialize(state, (Visuals_Object*)g_renderBuffer);
    Shizu_Object_unlock(state, (Shizu_Object*)g_renderBuffer);
    g_renderBuffer = NULL;
  }
  if (g_vertexBuffer) {
    Visuals_Object_unmaterialize(state, (Visuals_Object*)g_vertexBuffer);
    Shizu_Object_unlock(state, (Shizu_Object*)g_vertexBuffer);
    g_vertexBuffer = NULL;
  }
  if (g_program) {
    Visuals_Object_unmaterialize(state, (Visuals_Object*)g_program);
    Shizu_Object_unlock(state, (Shizu_Object*)g_program);
    g_program = NULL;
  }
  ServiceGl_shutdown(state);
}
