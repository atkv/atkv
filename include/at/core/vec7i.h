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

#ifndef AT_VEC7i_H
#define AT_VEC7i_H
#include <math.h>

typedef struct _AtVec7i{
  int data[7];
}AtVec7i;

/**
 * @brief at_vec7_add
 * @param vec1
 * @param vec2
 */
void
at_vec7i_add(AtVec7i* vec1, AtVec7i* vec2);

/**
 * @brief at_vec7_subtract
 * @param vec1
 * @param vec2
 */
void
at_vec7i_subtract(AtVec7i* vec1, AtVec7i* vec2);

/**
 * @brief at_vec7_multiply
 * @param vec1
 * @param vec2
 */
void
at_vec7i_multiply(AtVec7i* vec1, AtVec7i* vec2);

/**
 * @brief at_vec7_divide
 * @param vec1
 * @param vec2
 */
void
at_vec7i_divide(AtVec7i* vec1, AtVec7i* vec2);

/**
 * @brief at_vec7_dot
 * @param vec1
 * @param vec2
 * @return
 */
int
at_vec7i_dot(AtVec7i* vec1, AtVec7i* vec2);

/**
 * @brief at_vec7_multiply_scalar
 * @param vec1
 * @param scalar
 */
void
at_vec7i_multiply_scalar(AtVec7i* vec1, double scalar);
/**
 * @brief at_vec7_normalize
 * @param vec
 */
void
at_vec7i_normalize(AtVec7i* vec);
/**
 * @brief at_vec7_get_magnitude
 * @param vec
 * @return
 */
double
at_vec7i_get_magnitude(AtVec7i* vec);

#if 7 == 3
/**
 * @brief Cross product between two vectors
 */
AtVec3i
at_vec7i_cross(AtVec7i* vec1, AtVec7i* vec2);
#endif

AtVec7i
AtVec7i_to_double(AtVec7i* vec);

#endif
