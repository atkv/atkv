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

#ifndef AT_VEC2i_H
#define AT_VEC2i_H
#include <math.h>

typedef struct _AtVec2i{
  int data[2];
}AtVec2i;

/**
 * @brief at_vec2_add
 * @param vec1
 * @param vec2
 */
void
at_vec2i_add(AtVec2i* vec1, AtVec2i* vec2);

/**
 * @brief at_vec2_subtract
 * @param vec1
 * @param vec2
 */
void
at_vec2i_subtract(AtVec2i* vec1, AtVec2i* vec2);

/**
 * @brief at_vec2_multiply
 * @param vec1
 * @param vec2
 */
void
at_vec2i_multiply(AtVec2i* vec1, AtVec2i* vec2);

/**
 * @brief at_vec2_divide
 * @param vec1
 * @param vec2
 */
void
at_vec2i_divide(AtVec2i* vec1, AtVec2i* vec2);

/**
 * @brief at_vec2_dot
 * @param vec1
 * @param vec2
 * @return
 */
int
at_vec2i_dot(AtVec2i* vec1, AtVec2i* vec2);

/**
 * @brief at_vec2_multiply_scalar
 * @param vec1
 * @param scalar
 */
void
at_vec2i_multiply_scalar(AtVec2i* vec1, double scalar);
/**
 * @brief at_vec2_normalize
 * @param vec
 */
void
at_vec2i_normalize(AtVec2i* vec);
/**
 * @brief at_vec2_get_magnitude
 * @param vec
 * @return
 */
double
at_vec2i_get_magnitude(AtVec2i* vec);

/**
 * @brief negate a vertex
 */
void
at_vec2i_negate(AtVec2i* vec);

#if 2 == 3
/**
 * @brief Cross product between two vectors
 */
AtVec3i
at_vec2i_cross(AtVec2i* vec1, AtVec2i* vec2);
AtVec3i
at_vec3i_x();
AtVec3i
at_vec3i_y();
AtVec3i
at_vec3i_z();
#endif

AtVec2
AtVec2i_to_double(AtVec2i* vec);

#endif
