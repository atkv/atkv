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

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <at/imgproc.h>
#include <stdio.h>
static void test_drawing_line(void** state){
  (void) state;
  g_autoptr(AtArray(uint8_t)) array = NULL;
  at_array_new(&array, 10, 10, 3);

  uint8_t* data = at_array_get(array);
  g_autofree uint64_t* step = at_array_get_step(array);

  at_array_fill(array,0);

  // Line Properties
  AtVec2i p0        = {-5, 0},
              p1        = {8,8};
  AtVec4i color     = {255,100,50,255};
  int         thickness = 2;
  AtLineType  line_type = AT_LINE_TYPE_8;
  int         shift     = 0;

  // Draw Line
  at_array_uint8_t_draw_line(array, p0, p1, &color, thickness, line_type, shift);

  uint64_t x,y,c;
  // Print line
  for(y = 0; y < 10; y++){
    for(x = 0; x < 10; x++){
      printf("(");
      for(c = 0; c < 3; c++)
        printf("%3d ", data[y*step[0]+x*step[1]+c*step[2]]);
      printf(")");
    }
    printf("\n");
  }
}

static void test_drawing_circle(void** state){
  (void) state;
  g_autoptr(AtArray(uint8_t)) array = NULL;
  at_array_new(&array, 10, 10, 3);
  at_array_fill(array,0);

  uint8_t* data = at_array_get(array);
  g_autofree uint64_t* step = at_array_get_step(array);

  // Circle Properties
  AtVec2i center    = {5, 5};
  int radius            = 2;
  AtVec4i color     = {255,100,50,255};
  int         thickness = -1;
  AtLineType line_type = AT_LINE_TYPE_8;
  int         shift     = 0;

  // Draw Line
  at_array_uint8_t_draw_circle(array, center, radius, &color, thickness, line_type, shift);

  uint64_t x,y,c;
  // Print line
  for(y = 0; y < 10; y++){
    for(x = 0; x < 10; x++){
      printf("(");
      for(c = 0; c < 3; c++)
        printf("%3d ", data[y*step[0]+x*step[1]+c*step[2]]);
      printf(")");
    }
    printf("\n");
  }
}

static void test_drawing_polyline(void** state){
  (void) state;
}

static void test_drawing_line_arrow(void** state){
  (void) state;
}
static void test_drawing_rectangle(void** state){
  (void) state;
  g_autoptr(AtArray(uint8_t)) array = NULL;
  at_array_new(&array, 10, 10, 3);
  at_array_fill(array,0);

  uint8_t* data = at_array_get(array);
  g_autofree uint64_t* step = at_array_get_step(array);


  // Rectangle Properties
  AtVec4i rect     = {3,3,5,4};
  AtVec4i color     = {255,100,50,255};
  int         thickness = 1;
  AtLineType line_type = AT_LINE_TYPE_8;
  int         shift     = 0;

  // Draw Rectangle
  at_array_uint8_t_draw_rectangle(array,rect, &color, thickness, line_type, shift);

  uint64_t x,y,c;
  // Print line
  for(y = 0; y < 10; y++){
    for(x = 0; x < 10; x++){
      printf("(");
      for(c = 0; c < 3; c++)
        printf("%3d ", data[y*step[0]+x*step[1]+c*step[2]]);
      printf(")");
    }
    printf("\n");
  }


}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[5]={
    cmocka_unit_test(test_drawing_line),
    cmocka_unit_test(test_drawing_circle),
    cmocka_unit_test(test_drawing_polyline),
    cmocka_unit_test(test_drawing_line_arrow),
    cmocka_unit_test(test_drawing_rectangle),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
