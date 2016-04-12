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

#include <at/core.h>
#include <string.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
#define at_to_rad(angle) (angle / 180.0 * M_PI  )
#define at_to_deg(angle) (angle * 180.0 * M_1_PI)
static void
at_gl_create_rotation_matrix(AtMat4* rotation, double angle, AtVec3* axis){
  double a            = at_to_rad(angle);
  double c            = cos(a);
  double cc           = 1-c;
  double s            = sin(a);

  at_vec3_normalize(axis);

  //uint32_t  size     = 3;
  //uint32_t  sizes[2] = {4,4};
  //double     ve[3]    = {x,y,z};
  //AtNDArray* axisn    = at_ndarray_from_data(ve,1,&size,AT_double);
  //AtNDArray* axis     = at_ndarray_normalize(axisn);

  double u  = axis->data[0];
  double v  = axis->data[1];
  double w  = axis->data[2];
  double u2 = u*u;
  double v2 = v*v;
  double w2 = w*w;
  double uv = u*v;
  double uw = u*w;
  double vw = v*w;

  rotation->data[0]  = u2*cc + c;
  rotation->data[1]  = uv*cc + w*s;
  rotation->data[2]  = uw*cc - v*s;

  rotation->data[4]  = uv*cc - w*s;
  rotation->data[5]  = v2*cc + c;
  rotation->data[6]  = vw*cc + u*s;

  rotation->data[8]  = uw*cc + v*s;
  rotation->data[9]  = vw*cc - u*s;
  rotation->data[10] = w2*cc + c;
}
/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtMat4
at_mat4_eye(){
  return (AtMat4){{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}};
}

void
at_mat4_rotate_vec3(AtVec3* vec, double angle, AtVec3* axis){
  AtMat4 rotation = at_mat4_eye();
  at_gl_create_rotation_matrix(&rotation,angle,axis);
  at_mat4_mult_vec3(&rotation,vec);
}

void
at_mat4_rotate_mat4(AtMat4* mat, double angle, AtVec3* axis){
  AtMat4 rotation = at_mat4_eye();
  at_gl_create_rotation_matrix(&rotation,angle,axis);
  at_mat4_mult_mat4(&rotation,mat,mat);
}

void
at_mat4_mult_vec3(AtMat4* mat, AtVec3* vec){
  AtVec3 res = *vec;
  u_int8_t i;
  AtVec3 rows[3] = {{{mat->data[0],mat->data[4],mat->data[8]}},
                       {{mat->data[1],mat->data[5],mat->data[9]}},
                       {{mat->data[2],mat->data[6],mat->data[10]}}};
  for(i = 0; i < 3; i++)
    vec->data[i] = at_vec3_dot(&res,&rows[i]) + mat->data[12+i];
}
void
at_mat4_mult_mat4(AtMat4* mat1, AtMat4* mat2, AtMat4* output){
  AtVec4 rows1[4] = {{{mat1->data[0],mat1->data[4],mat1->data[8] ,mat1->data[12]}},
                        {{mat1->data[1],mat1->data[5],mat1->data[9] ,mat1->data[13]}},
                        {{mat1->data[2],mat1->data[6],mat1->data[10],mat1->data[14]}},
                        {{mat1->data[3],mat1->data[7],mat1->data[11],mat1->data[15]}}};
  AtVec4 columns2[4] = {*(AtVec4*)&mat2->data[0],
                           *(AtVec4*)&mat2->data[4],
                           *(AtVec4*)&mat2->data[8],
                           *(AtVec4*)&mat2->data[12]};
  u_int8_t i, j, k = 0;
  for(j = 0; j < 4; j++)
    for(i = 0; i < 4; i++, k++)
      output->data[k] = at_vec4_dot(&rows1[i],&columns2[j]);
}

void
at_mat4_set(AtMat4* mat, AtMat4* output){
  memcpy(output, mat, sizeof(AtMat4));
}

gboolean
at_mat4_inverse(AtMat4* mat, AtMat4* output){
  AtMat4 inv_mat;
  double* inv = inv_mat.data;
  double det;
  uint8_t i;
  double* m = mat->data;

  inv[0]  =  m[5]  * m[10] * m[15] - m[5]  * m[11] * m[14] -
             m[9]  * m[6]  * m[15] + m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - m[13] * m[7]  * m[10];

  inv[4]  = -m[4]  * m[10] * m[15] + m[4]  * m[11] * m[14] +
             m[8]  * m[6]  * m[15] - m[8]  * m[7]  * m[14] -
             m[12] * m[6]  * m[11] + m[12] * m[7]  * m[10];

  inv[8]  =  m[4]  * m[9]  * m[15] - m[4]  * m[11] * m[13] -
             m[8]  * m[5]  * m[15] + m[8]  * m[7]  * m[13] +
             m[12] * m[5]  * m[11] - m[12] * m[7]  * m[9];

  inv[12] = -m[4]  * m[9] * m[14]  + m[4]  * m[10] * m[13] +
             m[8]  * m[5] * m[14]  - m[8]  * m[6] * m[13]  -
             m[12] * m[5] * m[10]  + m[12] * m[6] * m[9];

  inv[1]  = -m[1]  * m[10] * m[15] + m[1]  * m[11] * m[14] +
             m[9]  * m[2] * m[15]  - m[9]  * m[3] * m[14]  -
             m[13] * m[2] * m[11]  + m[13] * m[3] * m[10];

  inv[5]  =  m[0]  * m[10] * m[15] - m[0]  * m[11] * m[14] -
             m[8]  * m[2] * m[15]  + m[8]  * m[3] * m[14]  +
             m[12] * m[2] * m[11]  - m[12] * m[3] * m[10];

  inv[9]  = -m[0]  * m[9] * m[15]  + m[0]  * m[11] * m[13] +
             m[8]  * m[1] * m[15]  - m[8]  * m[3] * m[13]  -
             m[12] * m[1] * m[11]  + m[12] * m[3] * m[9];

  inv[13] =  m[0]  * m[9] * m[14]  - m[0]  * m[10] * m[13] -
             m[8]  * m[1] * m[14]  + m[8]  * m[2] * m[13]  +
             m[12] * m[1] * m[10]  - m[12] * m[2] * m[9];

  inv[2]  =  m[1]  * m[6] * m[15]  - m[1]  * m[7] * m[14] -
             m[5]  * m[2] * m[15]  + m[5]  * m[3] * m[14] +
             m[13] * m[2] * m[7]   - m[13] * m[3] * m[6];

  inv[6]  = -m[0]  * m[6] * m[15]  + m[0]  * m[7] * m[14] +
             m[4]  * m[2] * m[15]  - m[4]  * m[3] * m[14] -
             m[12] * m[2] * m[7]   + m[12] * m[3] * m[6];

  inv[10] =  m[0]  * m[5] * m[15]  - m[0]  * m[7] * m[13] -
             m[4]  * m[1] * m[15]  + m[4]  * m[3] * m[13] +
             m[12] * m[1] * m[7]   - m[12] * m[3] * m[5];

  inv[14] = -m[0]  * m[5] * m[14]  + m[0]  * m[6] * m[13] +
             m[4]  * m[1] * m[14]  - m[4]  * m[2] * m[13] -
             m[12] * m[1] * m[6]   + m[12] * m[2] * m[5];

  inv[3]  = -m[1] * m[6] * m[11]   + m[1] * m[7] * m[10] +
             m[5] * m[2] * m[11]   - m[5] * m[3] * m[10] -
             m[9] * m[2] * m[7]    + m[9] * m[3] * m[6];

  inv[7]  =  m[0] * m[6] * m[11]   - m[0] * m[7] * m[10] -
             m[4] * m[2] * m[11]   + m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7]    - m[8] * m[3] * m[6];

  inv[11] = -m[0] * m[5] * m[11]   + m[0] * m[7] * m[9]  +
             m[4] * m[1] * m[11]   - m[4] * m[3] * m[9]  -
             m[8] * m[1] * m[7]    + m[8] * m[3] * m[5];

  inv[15] =  m[0] * m[5] * m[10]   - m[0] * m[6] * m[9]  -
             m[4] * m[1] * m[10]   + m[4] * m[2] * m[9]  +
             m[8] * m[1] * m[6]    - m[8] * m[2] * m[5];

  det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

  if (det == 0)
    return FALSE;

  det = 1.0 / det;

  for (i = 0; i < 16; i++)
    output->data[i] = inv[i] * det;

  return TRUE;
}

void
at_mat4_transpose(AtMat4* mat, AtMat4* output){
  double t;
  double* m = mat->data;
  double* o = output->data;
  if(output != mat){
    o[0]  = m[0];
    o[5]  = m[5];
    o[10] = m[10];
    o[15] = m[15];
  }

  t = m[1 ]; o[1 ] = m[4 ]; o[4 ] = t;
  t = m[2 ]; o[2 ] = m[8 ]; o[8 ] = t;
  t = m[3 ]; o[3 ] = m[12]; o[12] = t;
  t = m[6 ]; o[6 ] = m[9 ]; o[9 ] = t;
  t = m[7 ]; o[7 ] = m[13]; o[13] = t;
  t = m[11]; o[11] = m[14]; o[14] = t;
}

void
at_mat4_translate(AtMat4* mat, AtVec3* vector){
  double* m = mat->data;
  double* v = vector->data;
  m[12] += v[0];
  m[13] += v[1];
  m[14] += v[2];
}

void
at_mat4_to_eye(AtMat4* mat){
  double* m = mat->data;
  memset(mat, 0, sizeof(AtMat4));
  m[0] = m[5] = m[10] = m[15] = 1;
}

void
at_mat4_scale(AtMat4* mat, AtVec3* vector){
  double* m = mat->data;
  double* v = vector->data;
  m[0] *= v[0];
  m[5] *= v[1];
  m[10] *= v[2];
}
