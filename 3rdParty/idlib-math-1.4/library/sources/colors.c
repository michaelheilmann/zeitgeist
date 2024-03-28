// Copyright (c) 2023-2024 Michael Heilmann.All rights reserved.
#include "colors.h"

#if IDLIB_C_COMPILER == IDLIB_C_COMPILER_MSVC
  #define STATIC_ASSERT(CONDITION, MESSAGE) static_assert(CONDITION, MESSAGE)
#else
  #define STATIC_ASSERT(CONDITION, MESSAGE) _Static_assert(CONDITION, MESSAGE)
#endif

#define DEFINE(NAME, R, G, B) \
  STATIC_ASSERT(0 <= R && R <= 255, "<internal error>"); \
  STATIC_ASSERT(0 <= G && G <= 255, "<internal error>"); \
  STATIC_ASSERT(0 <= B && B <= 255, "<internal error>"); \
  idlib_color_3_u8 const idlib_colors_##NAME##_3_u8 = { .r = R, .g = G, .b = B };

#include "colors.i"

#undef DEFINE
