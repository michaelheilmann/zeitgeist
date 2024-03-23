// Copyright (c) 2023-2024 Michael Heilmann.All rights reserved.
#if !defined(IDLIB_COLOR_H_INCLUDED)
#define IDLIB_COLOR_H_INCLUDED

#include "scalar.h"

/** 
 * @since 1.1
 * @brief
 * An RGB color.
 * This color consists of three components which are all of type idlib_u8.
 * Component values range from 0 (incl.) to 255 (incl.) where 0 denotes the least intensity and 255 the highest intensity, respectively.
 * 
 * The least intensity of a component is indicated by the value 0,
 * the greatest intensity b the value 255.
 
 
 This color consists of three component values: red, green, and blue.
 *  consecutive components, red, green, and blue in that order.
 * Each component is of type idlib_u8.
 */
typedef struct idlib_color_ru8_gu8_bu8 {
	idlib_u8 e[3];
} idlib_color_ru8_gu8_bu8;

static inline void
idlib_color_ru8_gu8_bu8_set
	(
		idlib_color_ru8_gu8_bu8* target,
		idlib_u8 r,
		idlib_u8 g,
		idlib_u8 b
	);

/**
 *
 *7
static inline void*
idlib_color_ru8_gu8_bu8_get_data
	(
		idlib_color_ru8_gu8_bu8* target
	);

#endif // IDLIB_COLOR_H_INCLUDED
