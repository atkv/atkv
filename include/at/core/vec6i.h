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

#ifndef AT_VEC6i_H
#define AT_VEC6i_H
#include <math.h>

typedef struct _AtVec6i{
  int data[6];
}AtVec6i;

/**
 * @brief at_vec6_add
 * @param vec1
 * @param vec2
 */
void
at_vec6i_add(AtVec6i* vec1, AtVec6i* vec2);

/**
 * @brief at_vec6_subtract
 * @param vec1
 * @param vec2
 */
void
at_vec6i_subtract(AtVec6i* vec1, AtVec6i* vec2);

/**
 * @brief at_vec6_multiply
 * @param vec1
 * @param vec2
 */
void
at_vec6i_multiply(AtVec6i* vec1, AtVec6i* vec2);

/**
 * @brief at_vec6_divide
 * @param vec1
 * @param vec2
 */
void
at_vec6i_divide(AtVec6i* vec1, AtVec6i* vec2);

/**
 * @brief at_vec6_dot
 * @param vec1
 * @param vec2
 * @return
 */
int
at_vec6i_dot(AtVec6i* vec1, AtVec6i* vec2);

/**
 * @brief at_vec6_multiply_scalar
 * @param vec1
 * @param scalar
 */
void
at_vec6i_multiply_scalar(AtVec6i* vec1, double scalar);
/**
 * @brief at_vec6_normalize
 * @param vec
 */
void
at_vec6i_normalize(AtVec6i* vec);
/**
 * @brief at_vec6_get_magnitude
 * @param vec
 * @return
 */
double
at_vec6i_get_magnitude(AtVec6i* vec);

#if 6 == 3
/**
 * @brief Cross product between two vectors
 */
AtVec3i
at_vec6i_cross(AtVec6i* vec1, AtVec6i* vec2);
#endif

AtVec6i
AtVec6i_to_double(AtVec6i* vec);

#endif
