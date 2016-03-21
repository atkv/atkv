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

#include <at/imgproc.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_at_image_read(void** state){
  g_autoptr(AtArray(uint8_t)) image_png = NULL;
  g_autoptr(AtArray(uint8_t)) image_jpg = NULL;
  g_autoptr(AtArray(uint8_t)) image_pgm = NULL;
  g_autoptr(AtArray(uint8_t)) image_ppm = NULL;
  uint64_t i;
  uint8_t data[48] = {   0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255};
  uint8_t data_gray[16] = {   0,   255,
                         0,   255,
                         0,   255,
                         0,   255,
                         0,   255,
                         0,   255,
                         0,   255,
                         0,   255};
  at_image_read(&image_png, "stripe.png");
  for(i = 0; i < 48; i++) assert_int_equal(at_array_get(image_png, i),data[i]);

  at_image_read(&image_jpg, "stripe.jpg");
  for(i = 0; i < 48; i++) assert_int_equal(at_array_get(image_jpg, i),data[i]);

  // Gray Scale
  at_image_read(&image_pgm, "stripe.pgm");
  for(i = 0; i < 16; i++) assert_int_equal(at_array_get(image_pgm, i),data_gray[i]);

  at_image_read(&image_ppm, "stripe.ppm");
  for(i = 0; i < 48; i++) assert_int_equal(at_array_get(image_ppm, i),data[i]);


}

#define TEST_WRITE(type,num,name) \
  g_autoptr(AtArray(uint8_t)) array_##type##_write = NULL;\
  g_autoptr(AtArray(uint8_t)) array_##type##_read = NULL;\
  at_array_new(&array_##type##_write, (num==1)?2:3, size, data_##name);\
  at_image_write(array_##type##_write,"stripe_write."#type);\
  at_image_read(&array_##type##_read,"stripe_write."#type);\
  for(i = 0; i < 16*num; i++) assert_int_equal(at_array_get(array_##type##_read, i),data_##name[i]);

static void test_at_image_write(void** state){
  uint8_t data_color[48] = {   0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255,
                         0,   0,   0, 255, 255, 255};
  uint8_t data_gray[16] = {   0,   255,
                         0,   255,
                         0,   255,
                         0,   255,
                         0,   255,
                         0,   255,
                         0,   255,
                         0,   255};
  uint64_t size[3] = {4,4,3};
  uint64_t i;

  TEST_WRITE(png, 3,color);
  TEST_WRITE(jpg, 3,color);
  TEST_WRITE(pgm, 1,gray);
  TEST_WRITE(ppm, 3,color);
}


int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[2]={
    cmocka_unit_test(test_at_image_read),
    cmocka_unit_test(test_at_image_write),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}

