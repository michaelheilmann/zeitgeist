// Copyright (c) 2023-2024 Michael Heilmann.All rights reserved.
#if !defined(IDLIB_COLORS_H_INCLUDED)
#define IDLIB_COLORS_H_INCLUDED

#include "color.h"

#define DEFINE(NAME, R, G, B) \
  extern idlib_color_3_u8 const idlib_colors_##NAME##_3_u8;

#include "colors.i"

#undef DEFINE

#endif // IDLIB_COLORS_H_INCLUDED
