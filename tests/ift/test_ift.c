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

#include <setjmp.h>
#include <stdlib.h>
#include <stdarg.h>
#include <cmocka.h>
#include <at/ift.h>

static void
test_at_ift_min_f_max_array(void** state){
  // intensities of the array
  uint8_t intensities[16] = {  0,  0,  0,  0,
                               0,  0,  0,255,
                               0,255,255,255,
                             255,255,255,255};
  uint8_t labels[16]      = {  0,  0,  0,  0,
                               0,  0,  0,  1,
                               0,  1,  1,  1,
                               1,  1,  1,  1};
  uint8_t predecessors[16]= {  0,  0,  1,  2,
                               0,  1,  2, 11,
                               4, 13, 14, 15,
                              13, 14, 15, 15};
  uint8_t roots[16]       = {  0,  0,  0,  0,
                               0,  0,  0, 15,
                               0, 15, 15, 15,
                              15, 15, 15, 15};
  double connectivity[16] = {-DBL_MAX,0.0,0.0,0.0,
                                  0.0,0.0,0.0,0.0,
                                  0.0,0.0,0.0,0.0,
                                  0.0,0.0,0.0,-DBL_MAX};
                   //pos, label
  uint64_t seeds_data[4]   = {  0, 0,
                               15, 1};


  // Create the image
  g_autoptr(AtArray(uint8_t)) image = NULL;
  uint64_t size[2] = {4,4};
  at_array_new(&image, 2, size, intensities);

  // Create the seeds
  g_autoptr(AtArray(uint64_t)) seeds = NULL;
  uint64_t seeds_size[2] = {2,2};
  at_array_new(&seeds, 2, seeds_size, seeds_data);

  // Apply arrays
  g_autoptr(AtIFTArray(uint8_t)) ift = NULL;
  at_ift_apply(&ift,
               image,
               at_array_get_dim(image),
               AT_ADJACENCY_4,
               AT_OPTIMIZATION_MIN,
               at_connectivity(max,ift),
               at_weighting(abs_diff, ift),
               seeds);

  assert_non_null(ift);


  AtArray(uint8_t) * labels_ift       = at_ift_get_labels(ift);
  AtArray(uint64_t)* predecessors_ift = at_ift_get_predecessors(ift);
  AtArray(uint64_t)* roots_ift        = at_ift_get_roots(ift);
  AtArray(double)*   connectivity_ift = at_ift_get_connectivities(ift);

  assert_non_null(labels_ift);
  assert_non_null(predecessors_ift);
  assert_non_null(roots_ift);
  assert_non_null(connectivity_ift);

  uint8_t*  labels_ift_data       = at_array_uint8_t_get_data (labels_ift);
  uint64_t* predecessors_ift_data = at_array_uint64_t_get_data(predecessors_ift);
  uint64_t* roots_ift_data        = at_array_uint64_t_get_data(roots_ift);
  double*   connectivity_ift_data = at_array_double_get_data  (connectivity_ift);

  uint64_t i;
  for(i = 0; i < 16; i++){
    assert_int_equal(labels_ift_data[i]      , labels[i]);
    assert_int_equal(predecessors_ift_data[i], predecessors[i]);
    assert_int_equal(roots_ift_data[i]       , roots[i]);
    assert_true     (fabs(connectivity_ift_data[i] - connectivity[i]) <= .1e-4);
  }
}


int
main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_at_ift_min_f_max_array),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
