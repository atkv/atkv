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
void
at_vec5i_add(AtVec5i* vec1, AtVec5i* vec2){
  uint8_t i;
  for(i = 0; i < 5; i++)
    vec1->data[i] += vec2->data[i];
}

void
at_vec5i_subtract(AtVec5i* vec1, AtVec5i* vec2){
  uint8_t i;
  for(i = 0; i < 5; i++)
    vec1->data[i] -= vec2->data[i];
}

void
at_vec5i_multiply(AtVec5i* vec1, AtVec5i* vec2){
  uint8_t i;
  for(i = 0; i < 5; i++)
    vec1->data[i] *= vec2->data[i];
}

void
at_vec5i_divide(AtVec5i* vec1, AtVec5i* vec2){
  uint8_t i;
  for(i = 0; i < 5; i++)
    vec1->data[i] /= vec2->data[i];
}

int
at_vec5i_dot(AtVec5i* vec1, AtVec5i* vec2){
  uint8_t i;
  double sum = 0;
  for(i = 0; i < 5; i++)
    sum += vec1->data[i]*vec2->data[i];
  return sum;
}
void
at_vec5i_multiply_scalar(AtVec5i* vec1, double scalar){
  uint8_t i;
  for(i = 0; i < 5; i++)
    vec1->data[i] /= scalar;
}
void
at_vec5i_normalize(AtVec5i* vec){
  double magnitude = at_vec5i_get_magnitude(vec);
  at_vec5i_multiply_scalar(vec,1.0/magnitude);
}
double
at_vec5i_get_magnitude(AtVec5i* vec){
  return sqrt((double)at_vec5i_dot(vec,vec));
}
#if 5 == 3
AtVec3i
at_vec3i_cross(AtVec3i* vec1, AtVec3i* vec2){
  int* d1 = vec1->data;
  int* d2 = vec2->data;
  AtVec3i result;
  int* d = result.data;
  d[0] = d1[1] * d2[2] - d2[1] * d1[2];
  d[1] = d1[2] * d2[0] - d2[2] * d1[0];
  d[2] = d1[0] * d2[1] - d2[0] * d1[1];
  return result;
}
#endif

AtVec5i
AtVec5i_to_double(AtVec5i* vec){
  AtVec5i vecd;
  uint8_t i;
  for(i = 0; i < 5; i++)
    vecd.data[i] = vec->data[i];
  return vecd;
}
