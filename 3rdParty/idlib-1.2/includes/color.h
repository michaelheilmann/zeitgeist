// Copyright (c) 2023-2024 Michael Heilmann.All rights reserved.
#if !defined(IDLIB_COLOR_H_INCLUDED)
#define IDLIB_COLOR_H_INCLUDED

#include "scalar.h"

/**
 * @since 1.2
 * @brief
 * Representation of a color consisting of three components, red, green, and blue.
 * The component values are layed out consecutively in memory and are of type idlib_u8 where
 * 0 denotes the minimum intensity and 255 denotes the maximum intensity.
 */
typedef struct idlib_color_3_u8 {
	union {
		struct {
			idlib_u8 r, g, b;
		};
		idlib_u8 components[3];
	};
} idlib_color_3_u8;

/** 
 * @since 1.2
 * @brief Assign an idlib_color_3_u8 object the specified component values.
 * @param target Pointer to the idlib_matrix_4x4_f32 object.
 * @param operand1 The value to be assigned to the "red" component.
 * @param operand2 The value to be assigned to the "green" component.
 * @param operand3 The value to be assigned to the "blue" component.
 */
static inline void
idlib_color_3_u8_set
	(
		idlib_color_3_u8 *target,
		idlib_u8 operand1,
		idlib_u8 operand2,
		idlib_u8 operand3
	);

static inline void
idlib_color_3_u8_set
	(
		idlib_color_3_u8 *target,
		idlib_u8 operand1,
		idlib_u8 operand2,
		idlib_u8 operand3
	)
{
	IDLIB_DEBUG_ASSERT(NULL != target);
	target->r = operand1;
	target->g = operand2;
	target->b = operand3;
}

/**
 * @since 1.2
 * @brief
 * Representation of a color consisting of three components, red, green, blue, and alpha.
 * The component values are layed out consecutively in memory and are of type idlib_f32 where
 * 0 denotes the minimum intensity and 1 denotes the maximum intensity.
 */
typedef struct idlib_color_4_f32 {
	union {
		struct {
			idlib_f32 r, g, b, a;
		};
		idlib_f32 components[4];
	};
} idlib_color_4_f32;

/**
 * @since 1.2
 * @brief Assign an idlib_color_4_f32 object the specified component values.
 * @param target Pointer to the idlib_matrix_4x4_f32 object.
 * @param r The value to be assigned to the "red" component.
 * @param g The value to be assigned to the "green" component.
 * @param b The value to be assigned to the "blue" component.
 * @param a The value to be assigned to the "alpha" component.
 */
static inline void
idlib_color_4_f32_set
	(
		idlib_color_4_f32* target,
		idlib_f32 operand1,
		idlib_f32 operand2,
		idlib_f32 operand3,
		idlib_f32 operand4
	);

static inline void
idlib_color_4_f32_set
	(
		idlib_color_4_f32* target,
		idlib_f32 operand1,
		idlib_f32 operand2,
		idlib_f32 operand3,
		idlib_f32 operand4
	)
{
	IDLIB_DEBUG_ASSERT(NULL != target);
	target->r = operand1;
	target->g = operand2;
	target->b = operand3;
	target->a = operand4;
}

/**
 * @since 1.3
 * @brief Convert an RGB U8 to a RGBA F32 color.
 * @param target A pointer to the idlib_color_4_f32 receiving the result.
 * @param operand1 A pointer to the idlib_color_3_u8 color.
 * @param operand2 The alpha component value.
 */
static inline void
idlib_color_convert_3_u8_to_4_f32
	(
		idlib_color_4_f32* target,
		idlib_color_3_u8 const* operand1,
		idlib_f32 operand2
	);

static inline void
idlib_color_convert_3_u8_to_4_f32
	(
		idlib_color_4_f32* target,
		idlib_color_3_u8 const* operand1,
		idlib_f32 operand2
	)
{
	for (size_t i = 0; i < 3; ++i) {
		target->components[i] = ((idlib_f32)operand1->components[i]) / 255.f;
	}
	target->components[3] = operand2;
}

#endif // IDLIB_COLOR_H_INCLUDED
