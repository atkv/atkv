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
#include <string.h>
void
at_graph_component_from_grapharray(AtArray_uint16_t** component_label_ptr, AtGraphArray* grapharray){
  AtArray(uint64_t)* neighbors       = at_grapharray_get_neighbors(grapharray);
  AtArray(uint8_t)*  neighbors_edges = at_grapharray_get_neighbors_edges(grapharray);
  g_autofree uint64_t* neighbors_size = at_array_get_size(neighbors);
  at_array_zeros(component_label_ptr, at_array_get_dim(neighbors)-1, neighbors_size);

  AtArray(uint16_t)* component_label = *component_label_ptr;

  uint64_t num_elements = at_array_get_num_elements(component_label);
  uint64_t num_neighbors = at_array_get_num_elements(neighbors)/num_elements;

  g_autoptr(AtArray(uint8_t)) on_stack = NULL;
  g_autoptr(AtArray(uint8_t)) indices = NULL;
  g_autofree uint64_t* component_label_size = at_array_get_size(component_label);
  at_array_zeros(&on_stack, at_array_get_dim(component_label), component_label_size);
  at_array_zeros(&indices, at_array_get_dim(component_label), component_label_size);

  g_autofree uint64_t* stack_v   = malloc(sizeof(uint64_t) * (num_elements+1));
  g_autofree uint64_t* stack_rec = malloc(sizeof(uint64_t) * (num_elements+1));
  g_autofree uint64_t* stack_n   = malloc(sizeof(uint64_t) * (num_elements+1));
  int64_t stack_pointer     = -1;
  int64_t on_stack_pointer  = -1;
  int64_t stack_rec_pointer = -1;

  uint16_t index = 1;
  uint64_t vn, v, n, i, ni;
  uint16_t label;
  uint64_t w;
  uint8_t recursive;
  for(i = 0; i < num_elements; i++){
    if(at_array_get(indices,i) == 0){
      v = i;
      recursive = TRUE;
      ni = 0;
      stack_pointer = -1;
      while(1){
        // strongconnect(v)
        if(recursive){
          at_array_set(indices, v, index);
          at_array_set(component_label, v, index);
          index++;
          stack_pointer++;
          stack_v[stack_pointer] = v;
          on_stack_pointer = stack_pointer;
          at_array_set(on_stack, v, TRUE);
          recursive = FALSE;
        }

        // Consider successors of i
        vn = v * num_neighbors;
        for(n = ni; n < num_neighbors; n++){
          if(at_array_get(neighbors_edges,vn+n) == TRUE){
            w = at_array_get(neighbors,vn + n);
            if(at_array_get(indices, w) == 0){
              // strongconnect(v)
              recursive = TRUE;
              stack_rec_pointer++;
              stack_rec[stack_rec_pointer] = v;
              stack_n[stack_rec_pointer] = n+1;
              v  = w;
              ni = 0;
              break;
            }else if(at_array_get(on_stack, w) == TRUE){
              at_array_set(component_label, v, min(at_array_get(component_label,v),at_array_get(indices,w)));
            }
          }
        }
        if(!recursive){
          if(n == num_neighbors){
            if(at_array_get(component_label, v) == at_array_get(indices, v)){
              label = at_array_get(component_label, v);
              do{
                w = stack_v[stack_pointer];
                at_array_set(on_stack, w, FALSE);
                at_array_set(component_label,w,label);
                stack_pointer--;
              }while(w != v);
            }
            if(stack_rec_pointer >= 0){
              w  = v;
              v  = stack_rec[stack_rec_pointer];
              ni = stack_n[stack_rec_pointer];
              stack_rec_pointer--;

              at_array_set(component_label, v, min(at_array_get(component_label,v),at_array_get(component_label,w)));
            }
            else break;
          }
        }
      }
    }
  }

  // incremental label
  memset(stack_v, 0, sizeof(uint64_t) * (num_elements+1));
  index = 0;
  for(i = 0; i < num_elements; i++){
    v = at_array_get(component_label, i);
    if(stack_v[v] == 0){
      stack_v[v] = ++index;
    }
    at_array_set(component_label, i, (uint16_t)stack_v[v]);
  }
}
