// Copyright (c) 2023-2024 Michael Heilmann.All rights reserved.
#include "colors.h"

#define DEFINE(NAME, R, G, B) \
  _Static_assert(0 <= R && R <= 255, "<internal error>"); \
  _Static_assert(0 <= G && G <= 255, "<internal error>"); \
  _Static_assert(0 <= B && B <= 255, "<internal error>"); \
  idlib_color_3_u8 const idlib_colors_##NAME##_3_u8 = { .r = R, .g = G, .b = B };

#include "colors.i"

#undef DEFINE
