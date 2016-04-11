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

#ifndef AT_VEC3i_H
#define AT_VEC3i_H
#include <math.h>

typedef struct _AtVec3i{
  int data[3];
}AtVec3i;

/**
 * @brief at_vec3_add
 * @param vec1
 * @param vec2
 */
void
at_vec3i_add(AtVec3i* vec1, AtVec3i* vec2);

/**
 * @brief at_vec3_subtract
 * @param vec1
 * @param vec2
 */
void
at_vec3i_subtract(AtVec3i* vec1, AtVec3i* vec2);

/**
 * @brief at_vec3_multiply
 * @param vec1
 * @param vec2
 */
void
at_vec3i_multiply(AtVec3i* vec1, AtVec3i* vec2);

/**
 * @brief at_vec3_divide
 * @param vec1
 * @param vec2
 */
void
at_vec3i_divide(AtVec3i* vec1, AtVec3i* vec2);

/**
 * @brief at_vec3_dot
 * @param vec1
 * @param vec2
 * @return
 */
int
at_vec3i_dot(AtVec3i* vec1, AtVec3i* vec2);

/**
 * @brief at_vec3_multiply_scalar
 * @param vec1
 * @param scalar
 */
void
at_vec3i_multiply_scalar(AtVec3i* vec1, double scalar);
/**
 * @brief at_vec3_normalize
 * @param vec
 */
void
at_vec3i_normalize(AtVec3i* vec);
/**
 * @brief at_vec3_get_magnitude
 * @param vec
 * @return
 */
double
at_vec3i_get_magnitude(AtVec3i* vec);

/**
 * @brief negate a vertex
 */
void
at_vec3i_negate(AtVec3i* vec);

#if 3 == 3
/**
 * @brief Cross product between two vectors
 */
AtVec3i
at_vec3i_cross(AtVec3i* vec1, AtVec3i* vec2);
AtVec3i
at_vec3i_x();
AtVec3i
at_vec3i_y();
AtVec3i
at_vec3i_z();
#endif

AtVec3
AtVec3i_to_double(AtVec3i* vec);

#endif
