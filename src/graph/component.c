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
#include <stdlib.h>
void
at_graph_component_from_grapharray(AtArray_uint16_t** component_label_ptr, AtGraphArray* grapharray){
  AtArray(uint64_t)* neighbors       = at_grapharray_get_neighbors(grapharray);
  AtArray(uint8_t)*  neighbors_edges = at_grapharray_get_neighbors_edges(grapharray);
  at_array_zeros(component_label_ptr, at_array_get_dim(neighbors)-1, at_array_get_size(neighbors));



  AtArray(uint16_t)* component_label = *component_label_ptr;

  uint64_t num_elements = at_array_get_num_elements(component_label);
  uint64_t num_neighbors = at_array_get_num_elements(neighbors)/num_elements;

  g_autoptr(AtArray(uint8_t)) on_stack = NULL;
  g_autoptr(AtArray(uint8_t)) indices = NULL;
  at_array_zeros(&on_stack, at_array_get_dim(component_label), at_array_get_size(component_label));
  at_array_zeros(&indices, at_array_get_dim(component_label), at_array_get_size(component_label));

  g_autofree uint64_t* stack_v = malloc(sizeof(uint64_t) * at_array_get_num_elements(component_label));
  g_autofree uint64_t* stack_w = malloc(sizeof(uint64_t) * at_array_get_num_elements(component_label));
  uint64_t stack_pointer     = 0;

  uint16_t index = 1;
  uint64_t vn, v, n, i;
  uint64_t w;
  uint8_t recursive;
  for(i = 0; i < num_elements; v++){
    if(at_array_get(indices,i) == 0){

      v = i;
      recursive = TRUE;
      while(recursive){
        // strongconnect(v)
        recursive = FALSE;
        at_array_set(indices, v, index);
        at_array_set(component_label, v, index);
        index++;
        at_array_set(on_stack, v, TRUE);

        // Consider successors of i
        vn = v * num_neighbors;
        for(n = 0; n < num_neighbors; n++){
          if(at_array_get(neighbors_edges,vn+n) == TRUE){
            w = at_array_get(neighbors,vn + n);
            if(at_array_get(indices, w) == 0){
              // strongconnect(v)
              recursive = TRUE;
              stack_v[stack_pointer] = v;
              stack_w[stack_pointer] = w;
              stack_pointer++;
              v = w;
              break;
            }else if(at_array_get(on_stack, w) == TRUE){
              at_array_set(component_label, v, min(at_array_get(component_label,v),at_array_get(indices,w)));
            }
          }
        }
        if(!recursive && stack_pointer > 0){
          stack_pointer--;
          v = stack_v[stack_pointer];
          w = stack_w[stack_pointer];
          at_array_set(component_label, v, min(at_array_get(component_label,v),at_array_get(component_label,w)));
          recursive = TRUE;
        }
      }
    }
  }
}
