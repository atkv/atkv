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


static void
test_at_dicom_read(void** state){
  (void) state;
  const char* filename = "MR-MONO2-16-head.dcm";
  g_autoptr(AtDicom(uint16_t)) dicom = NULL;

  at_dicom_read(&dicom,filename);
  assert_non_null(dicom);

  g_autofree char* filename_assert = at_dicom_get_filename(dicom);
  g_autofree char* modality        = at_dicom_get_modality(dicom);
  uint8_t pixel_data_tag_found     = at_dicom_get_pixel_data_tag_found(dicom);
  g_autofree char* unit            = at_dicom_get_unit(dicom);
  uint16_t         dim             = at_dicom_get_dim(dicom);
  size_t           elemsize        = at_dicom_get_elemsize(dicom);
  uint64_t*        size            = at_dicom_get_size(dicom);


  assert_string_equal(filename_assert, filename);
  assert_string_equal(modality, "MR");
  assert_true(pixel_data_tag_found);
  assert_string_equal(unit, "mm");
  assert_int_equal(dim, 3);
  assert_int_equal(elemsize, 2);
  assert_int_equal(size[0], 256);
  assert_int_equal(size[1], 256);
  assert_int_equal(size[2], 1);
}

static void
test_at_dicom_write(void** state){
  (void) state;
}

int
main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[2]={
    cmocka_unit_test(test_at_dicom_read),
    cmocka_unit_test(test_at_dicom_write),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
