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

#ifndef AT_VEC2_H
#define AT_VEC2_H
#include <math.h>

typedef struct _AtVec2{
  double data[2];
}AtVec2;

/**
 * @brief at_vec2_add
 * @param vec1
 * @param vec2
 */
void
at_vec2_add(AtVec2* vec1, AtVec2* vec2);

/**
 * @brief at_vec2_subtract
 * @param vec1
 * @param vec2
 */
void
at_vec2_subtract(AtVec2* vec1, AtVec2* vec2);

/**
 * @brief at_vec2_multiply
 * @param vec1
 * @param vec2
 */
void
at_vec2_multiply(AtVec2* vec1, AtVec2* vec2);

/**
 * @brief at_vec2_divide
 * @param vec1
 * @param vec2
 */
void
at_vec2_divide(AtVec2* vec1, AtVec2* vec2);

/**
 * @brief at_vec2_dot
 * @param vec1
 * @param vec2
 * @return
 */
double
at_vec2_dot(AtVec2* vec1, AtVec2* vec2);

/**
 * @brief at_vec2_multiply_scalar
 * @param vec1
 * @param scalar
 */
void
at_vec2_multiply_scalar(AtVec2* vec1, double scalar);
/**
 * @brief at_vec2_normalize
 * @param vec
 */
void
at_vec2_normalize(AtVec2* vec);
/**
 * @brief at_vec2_get_magnitude
 * @param vec
 * @return
 */
double
at_vec2_get_magnitude(AtVec2* vec);

#if 2 == 3
/**
 * @brief Cross product between two vectors
 */
AtVec3
at_vec2_cross(AtVec2* vec1, AtVec2* vec2);
#endif

#endif
