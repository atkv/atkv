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

#ifndef AT_GL_MAT4_H
#define AT_GL_MAT4_H
#include <at/core.h>
typedef struct _AtMat4{
  double data[16];
}AtMat4;
/**
 * @brief at_mat4_eye
 * @return
 */
AtMat4
at_mat4_eye();
/**
 * @brief at_mat4_rotate_vec3
 * @param vec
 * @param angle
 * @param axis
 */
void
at_mat4_rotate_vec3(AtVec3* vec, double angle, AtVec3 axis);
/**
 * @brief at_mat4_rotate_mat4
 * @param mat
 * @param angle
 * @param axis
 */
void
at_mat4_rotate_mat4(AtMat4* mat, double angle, AtVec3 axis);
/**
 * @brief at_mat4_mult_vec3
 * @param mat
 * @param vec
 */
void
at_mat4_mult_vec3(AtMat4* mat, AtVec3* vec);
/**
 * @brief at_mat4_mult_mat4
 * @param mat1
 * @param mat2
 * @param output
 */
void
at_mat4_mult_mat4(AtMat4* mat1, AtMat4* mat2, AtMat4* output);
#endif
