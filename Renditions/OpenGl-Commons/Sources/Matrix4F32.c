#include "Matrix4F32.h"

#include "Vector3F32.h"

#if Zeitgeist_Configuration_CompilerC_Msvc == Zeitgeist_Configuration_CompilerC

  #pragma push_macro("near")
  #undef near

  #pragma push_macro("far")
  #undef far

#endif

Shizu_TypeDescriptor const Matrix4F32_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Matrix4F32),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Matrix4F32_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Matrix4F32, Shizu_Object);

Matrix4F32*
Matrix4F32_createIdentity
  (
    Shizu_State* state
  )
{
  Shizu_Type* type = Matrix4F32_getType(state);
  Matrix4F32* self = (Matrix4F32*)Shizu_Gc_allocateObject(state, sizeof(Matrix4F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_matrix_4x4_f32_set_identity(&self->m);
  ((Shizu_Object*)self)->type = type;
  return self;
}

Matrix4F32*
Matrix4F32_createTranslate
  (
    Shizu_State* state,
    Vector3F32* translate
  )
{
  Shizu_Type* type = Matrix4F32_getType(state);
  Matrix4F32* self = (Matrix4F32*)Shizu_Gc_allocateObject(state, sizeof(Matrix4F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_matrix_4x4_f32_set_translate(&self->m, &translate->v);
  ((Shizu_Object*)self)->type = type;
  return self;
}

Matrix4F32*
Matrix4F32_createRotateY
  (
    Shizu_State* state,
    Shizu_Float32 degrees
  )
{
  Shizu_Type* type = Matrix4F32_getType(state);
  Matrix4F32* self = (Matrix4F32*)Shizu_Gc_allocateObject(state, sizeof(Matrix4F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_matrix_4x4_f32_set_rotation_y(&self->m, degrees);
  ((Shizu_Object*)self)->type = type;
  return self;
}

Matrix4F32*
Matrix4F32_createPerspective
  (
    Shizu_State* state,
    Shizu_Float32 fieldOfViewY,
    Shizu_Float32 aspectRatio,
    Shizu_Float32 near,
    Shizu_Float32 far
  )
{
  Shizu_Type* type = Matrix4F32_getType(state);
  Matrix4F32* self = (Matrix4F32*)Shizu_Gc_allocateObject(state, sizeof(Matrix4F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_matrix_4x4_f32_set_perspective(&self->m, fieldOfViewY, aspectRatio, near, far);
  ((Shizu_Object*)self)->type = type;
  return self;
}

Matrix4F32*
Matrix4F32_createOrthographic
  (
    Shizu_State* state,
    Shizu_Float32 left,
    Shizu_Float32 right,
    Shizu_Float32 bottom,
    Shizu_Float32 top,
    Shizu_Float32 near,
    Shizu_Float32 far
  )
{
  Shizu_Type* type = Matrix4F32_getType(state);
  Matrix4F32* self = (Matrix4F32*)Shizu_Gc_allocateObject(state, sizeof(Matrix4F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_matrix_4x4_f32_set_orthographic(&self->m, left, right, bottom, top, near, far);
  ((Shizu_Object*)self)->type = type;
  return self;
}

Matrix4F32*
Matrix4F32_createLookAt
  (
    Shizu_State* state,
    Vector3F32* source,
    Vector3F32* target,
    Vector3F32* up
  )
{
  Shizu_Type* type = Matrix4F32_getType(state);
  Matrix4F32* self = (Matrix4F32*)Shizu_Gc_allocateObject(state, sizeof(Matrix4F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_matrix_4x4_f32_set_look_at(&self->m, &source->v, &target->v, &up->v);
  ((Shizu_Object*)self)->type = type;
  return self;
}

Matrix4F32*
Matrix4F32_createScale
  (
    Shizu_State* state,
    Vector3F32* scale
  )
{
  Shizu_Type* type = Matrix4F32_getType(state);
  Matrix4F32* self = (Matrix4F32*)Shizu_Gc_allocateObject(state, sizeof(Matrix4F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_matrix_4x4_f32_set_scale(&self->m, &scale->v);
  ((Shizu_Object*)self)->type = type;
  return self;
}

Matrix4F32*
Matrix4F32_negate
  (
    Shizu_State* state,
    Matrix4F32* matrix
  )
{
  Shizu_Type* type = Matrix4F32_getType(state);
  Matrix4F32* self = (Matrix4F32*)Shizu_Gc_allocateObject(state, sizeof(Matrix4F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_matrix_4x4_f32_negate(&self->m, &matrix->m);
  ((Shizu_Object*)self)->type = type;
  return self;
}

Matrix4F32*
Matrix4F32_multiply
  (
    Shizu_State* state,
    Matrix4F32* operand1,
    Matrix4F32* operand2
  )
{
  Shizu_Type* type = Matrix4F32_getType(state);
  Matrix4F32* self = (Matrix4F32*)Shizu_Gc_allocateObject(state, sizeof(Matrix4F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_matrix_4x4_f32_multiply(&self->m, &operand1->m, &operand2->m);
  ((Shizu_Object*)self)->type = type;
  return self;
}

#if Zeitgeist_Configuration_CompilerC_Msvc == Zeitgeist_Configuration_CompilerC
  #pragma pop_macro("far")
  #pragma pop_macro("near")
#endif
