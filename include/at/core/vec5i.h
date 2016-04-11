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

#ifndef AT_VEC5i_H
#define AT_VEC5i_H
#include <math.h>

typedef struct _AtVec5i{
  int data[5];
}AtVec5i;

/**
 * @brief at_vec5_add
 * @param vec1
 * @param vec2
 */
void
at_vec5i_add(AtVec5i* vec1, AtVec5i* vec2);

/**
 * @brief at_vec5_subtract
 * @param vec1
 * @param vec2
 */
void
at_vec5i_subtract(AtVec5i* vec1, AtVec5i* vec2);

/**
 * @brief at_vec5_multiply
 * @param vec1
 * @param vec2
 */
void
at_vec5i_multiply(AtVec5i* vec1, AtVec5i* vec2);

/**
 * @brief at_vec5_divide
 * @param vec1
 * @param vec2
 */
void
at_vec5i_divide(AtVec5i* vec1, AtVec5i* vec2);

/**
 * @brief at_vec5_dot
 * @param vec1
 * @param vec2
 * @return
 */
int
at_vec5i_dot(AtVec5i* vec1, AtVec5i* vec2);

/**
 * @brief at_vec5_multiply_scalar
 * @param vec1
 * @param scalar
 */
void
at_vec5i_multiply_scalar(AtVec5i* vec1, double scalar);
/**
 * @brief at_vec5_normalize
 * @param vec
 */
void
at_vec5i_normalize(AtVec5i* vec);
/**
 * @brief at_vec5_get_magnitude
 * @param vec
 * @return
 */
double
at_vec5i_get_magnitude(AtVec5i* vec);

/**
 * @brief negate a vertex
 */
void
at_vec5i_negate(AtVec5i* vec);

#if 5 == 3
/**
 * @brief Cross product between two vectors
 */
AtVec3i
at_vec5i_cross(AtVec5i* vec1, AtVec5i* vec2);
AtVec3i
at_vec3i_x();
AtVec3i
at_vec3i_y();
AtVec3i
at_vec3i_z();
#endif

AtVec5
AtVec5i_to_double(AtVec5i* vec);

#endif
