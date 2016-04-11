/**
 ** This file is part of the atkv project.
 ** Copyright 2016 Anderson Tavares <nocturne.pe@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef AT_VEC4i_H
#define AT_VEC4i_H
#include <math.h>

typedef struct _AtVec4i{
  int data[4];
}AtVec4i;

/**
 * @brief at_vec4_add
 * @param vec1
 * @param vec2
 */
void
at_vec4i_add(AtVec4i* vec1, AtVec4i* vec2);

/**
 * @brief at_vec4_subtract
 * @param vec1
 * @param vec2
 */
void
at_vec4i_subtract(AtVec4i* vec1, AtVec4i* vec2);

/**
 * @brief at_vec4_multiply
 * @param vec1
 * @param vec2
 */
void
at_vec4i_multiply(AtVec4i* vec1, AtVec4i* vec2);

/**
 * @brief at_vec4_divide
 * @param vec1
 * @param vec2
 */
void
at_vec4i_divide(AtVec4i* vec1, AtVec4i* vec2);

/**
 * @brief at_vec4_dot
 * @param vec1
 * @param vec2
 * @return
 */
int
at_vec4i_dot(AtVec4i* vec1, AtVec4i* vec2);

/**
 * @brief at_vec4_multiply_scalar
 * @param vec1
 * @param scalar
 */
void
at_vec4i_multiply_scalar(AtVec4i* vec1, double scalar);
/**
 * @brief at_vec4_normalize
 * @param vec
 */
void
at_vec4i_normalize(AtVec4i* vec);
/**
 * @brief at_vec4_get_magnitude
 * @param vec
 * @return
 */
double
at_vec4i_get_magnitude(AtVec4i* vec);

/**
 * @brief negate a vertex
 */
void
at_vec4i_negate(AtVec4i* vec);

#if 4 == 3
/**
 * @brief Cross product between two vectors
 */
AtVec3i
at_vec4i_cross(AtVec4i* vec1, AtVec4i* vec2);
AtVec3i
at_vec3i_x();
AtVec3i
at_vec3i_y();
AtVec3i
at_vec3i_z();
#endif

AtVec4
AtVec4i_to_double(AtVec4i* vec);

#endif
