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

#include <at/graph.h>
#include <setjmp.h>
#include <cmocka.h>

static void
_test_component(AtGraphArray* grapharray, uint16_t* label_gt){
  // Create the components
  g_autoptr(AtArray(uint16_t)) component_label = NULL;
  at_graph_component_from_grapharray(&component_label, grapharray);

  uint16_t*component_label_data  = at_array_get(component_label);
  uint64_t i;

  for(i = 0; i < 9; i++){
    assert_int_equal(component_label_data[i], label_gt[i]);
  }
}

static void
test_at_component_from_array(void** state){
  uint64_t size[2]  = {3,3};
  uint64_t num_arcs = 9;
  uint64_t arcs[20] = {0,1, 1,4, 4,3, 3,0, 1,2, 6,7, 8,7, 5,8, 8,5, 2,1}; // 10 arcs

  // Create a grapharray
  g_autoptr(AtGraphArray) grapharray = NULL;
  at_grapharray_new(&grapharray, 2, size, AT_ADJACENCY_4, arcs, num_arcs); // a 3x3 grid w/10 arcs
  assert_non_null(grapharray);

  // Groundtruths
  uint16_t component_label_gt_9[9] = {1,1,2,1,1,3,4,5,3}; // for 9 arcs
  uint16_t component_label_gt_10[9] = {1,1,1,1,1,2,3,4,2};   // for 10 arcs

  // For 9 arcs
  _test_component(grapharray, component_label_gt_9);

  // For 10 arcs
  at_grapharray_add_arc(grapharray,2,1);
  _test_component(grapharray, component_label_gt_10);

}

int
main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_at_component_from_array),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
