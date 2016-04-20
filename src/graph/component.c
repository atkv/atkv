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

typedef struct AtTarjanInfo{
  uint64_t  num_neighbors;
  uint64_t  num_elements;
  uint64_t* n_size;
  uint64_t  count;
  uint64_t  pre;
  int64_t   si;
  int64_t   sdi;
  uint64_t* stack;
  uint64_t* stack_v;
  uint64_t* stack_ew;
  uint64_t* stack_ewi;
  uint64_t* n;
  uint64_t* stack_lower;
  uint64_t* id;
  uint64_t* low;
  uint8_t * marked;
  uint8_t * ne;
  uint16_t  dim;
  uint8_t   recursive;
}AtTarjanInfo;

void
at_tarjan_info_init(AtTarjanInfo* info, AtGraphArray* grapharray){
  AtArray_uint64_t* neighbors       = at_grapharray_get_neighbors(grapharray);
  AtArray_uint8_t*  neighbors_edges = at_grapharray_get_neighbors_edges(grapharray);
  info->count         = 0;
  info->pre           = 0;
  info->si            = -1;
  info->sdi           = -1;
  info->recursive     = TRUE;
  info->n             = at_array_get(neighbors);
  info->ne            = at_array_get(neighbors_edges);

  info->dim           = at_array_get_dim(neighbors)-1;

  info->num_neighbors = at_grapharray_get_num_neighbors(grapharray);
  info->num_elements  = at_array_get_num_elements(neighbors)/info->num_neighbors;
  size_t size = info->num_elements * sizeof(uint8_t);
  info->marked        = g_malloc(size) ;//new boolean[G.V()];
  info->stack         = g_malloc(size << 3);//new Stack<Integer>();
  info->stack_v       = g_malloc(size << 3);//new Stack<Integer>();
  info->stack_ew      = g_malloc(size << 3);//new Stack<Integer>();
  info->stack_ewi     = g_malloc(size << 3);//new Stack<Integer>();
  info->stack_lower   = g_malloc(size << 3);//new Stack<Integer>();
  info->id            = g_malloc(size << 3);//new int[G.V()];
  info->low           = g_malloc(size << 3);//new int[G.V()];
  info->n_size        = at_array_get_size(neighbors);
  memset(info->marked, 0, size);
}

void
at_tarjan_info_destroy(AtTarjanInfo* info){
  g_free(info->stack);
  g_free(info->id);
  g_free(info->low);
  g_free(info->marked);
  g_free(info->n_size);
  g_free(info->stack_v);
  g_free(info->stack_ew);
  g_free(info->stack_ewi);
  g_free(info->stack_lower);
}
void
at_tarjan_info_push(AtTarjanInfo* info, uint64_t v){
  info->stack[++info->si] = v;
}
uint64_t
at_tarjan_info_pop(AtTarjanInfo* info){
  return info->stack[info->si--];
}

void
at_dfs_push(AtTarjanInfo* info, uint64_t v, uint64_t ew, uint64_t ewi, uint64_t lower){
  info->sdi++;
  info->stack_v    [info->sdi] = v;
  info->stack_ew   [info->sdi] = ew;
  info->stack_ewi  [info->sdi] = ewi;
  info->stack_lower[info->sdi] = lower;
  info->recursive = TRUE;
}
void
at_dfs_pop(AtTarjanInfo* info, uint64_t* v, uint64_t* ew, uint64_t* ewi, uint64_t* lower){
  if(info->sdi >= 0){
    *v     = info->stack_v    [info->sdi];
    *ew    = info->stack_ew   [info->sdi];
    *ewi   = info->stack_ewi  [info->sdi];
    *lower = info->stack_lower[info->sdi];
  }
  info->sdi--;
  info->recursive = FALSE;
}

void
at_dfs(AtTarjanInfo* info, uint64_t v){
  uint64_t w, ew, ewi;
  uint64_t lower;
  do{
    if(info->recursive){
      info->recursive = FALSE;
      info->marked[v] = TRUE;
      info->low[v]    = info->pre++;
      lower           = info->low[v];
      at_tarjan_info_push(info, v);
      ewi             = v*info->num_neighbors;
      ew              = ewi;
    }

    for(; ew < ewi + info->num_neighbors; ew++){
      if(info->ne[ew]){
        w = info->n[ew];
        if(!info->marked[w]){
          at_dfs_push(info, v, ew, ewi, lower);
          v = w;
          break;
        }
        lower = min(lower, info->low[w]);
      }
    }
    if(!info->recursive){
      if(lower < info->low[v])
        info->low[v] = lower;
      else{
        do{
          w            = at_tarjan_info_pop(info);
          info->id[w]  = info->count;
          info->low[w] = info->num_elements;
        }while(w != v);
        info->count++;
      }
      at_dfs_pop(info, &v, &ew, &ewi, &lower);
    }
  }while(info->sdi >= -1);
  info->recursive = TRUE;
  info->sdi       = -1;
  info->si        = -1;
}
void
at_tarjan_info_sort(AtTarjanInfo* info){
  memset(info->stack, 0, sizeof(uint64_t) * info->num_elements);
  info->count = 0;
  uint64_t i, v;
  for(i = 0; i < info->num_elements; i++){
    v = info->id[i];
    if(info->stack[v] == 0)
      info->stack[v] = ++info->count;

    info->id[i] = info->stack[v];
  }
}

static AtTarjanInfo* info = NULL;

void
at_graph_component_from_grapharray_interactive(AtArray_uint16_t** component_label_ptr, AtGraphArray* grapharray, uint64_t* next_v){
  if(info == NULL){
    info = malloc(sizeof(AtTarjanInfo));
    at_tarjan_info_init(info, grapharray);
  }
  uint64_t v;
  for(v = *next_v; v < info->num_elements; v++){
    if(!info->marked[v]){
      at_dfs(info,v);
      *next_v = v+1;
      break;
    }
  }

  at_tarjan_info_sort(info);

  // Creating the component
  if(*component_label_ptr == NULL)
    at_array_new(component_label_ptr, info->dim, info->n_size);
  uint16_t* c_data = at_array_get(*component_label_ptr);
  memcpy(c_data, info->id, sizeof(uint16_t)*info->num_elements);
}

void
at_graph_component_from_grapharray(AtArray_uint16_t** component_label_ptr, AtGraphArray* grapharray){
  AtTarjanInfo info;
  at_tarjan_info_init(&info, grapharray);
  uint64_t v;
  for(v = 0; v < info.num_elements; v++){
    if(!info.marked[v]) at_dfs(&info,v);
  }

  at_tarjan_info_sort(&info);

  // Creating the component
  at_array_new(component_label_ptr, info.dim, info.n_size);
  uint16_t* c_data = at_array_get(*component_label_ptr);
  for(v = 0; v < info.num_elements; v++){
    c_data[v] = (uint16_t) info.id[v];
  }

  at_tarjan_info_destroy(&info);
}

//void
//at_graph_component_from_grapharray(AtArray_uint16_t** component_label_ptr, AtGraphArray* grapharray){
//  // Get neighbors info (indices and enabled arcs)
//  AtArray(uint64_t)* neighbors        = at_grapharray_get_neighbors(grapharray);
//  AtArray(uint8_t)*  neighbors_edges  = at_grapharray_get_neighbors_edges(grapharray);
//  g_autofree uint64_t* neighbors_size = at_array_get_size(neighbors);
//  // Get size (without number of neighbors) and create the output
//  at_array_zeros(component_label_ptr, at_array_get_dim(neighbors)-1, neighbors_size);

//  AtArray(uint16_t)* component_label = *component_label_ptr;

//  // More info
//  uint64_t num_elements  = at_array_get_num_elements(component_label);
//  uint64_t num_neighbors = at_array_get_num_elements(neighbors)/num_elements;

//  // We have two stacks (one for the recursive process and another
//  // for the processing nodes). This is for the processing nodes
//  // (TRUE if node is on processing stack)
//  g_autoptr(AtArray(uint8_t)) on_stack = NULL;
//  // Where in the processing stack is the node?
//  g_autoptr(AtArray(uint8_t)) indices  = NULL;

//  // Create the above arrays
//  g_autofree uint64_t* component_label_size = at_array_get_size(component_label);
//  at_array_zeros(&on_stack, at_array_get_dim(component_label), component_label_size);
//  at_array_zeros(&indices, at_array_get_dim(component_label), component_label_size);

//  // Create the stacks
//  // v: the proper indices
//  // n: the component label for the node
//  // rec: the recursive stack
//  g_autofree uint64_t* stack_v   = malloc(sizeof(uint64_t) * (num_elements+1));
//  g_autofree uint64_t* stack_rec = malloc(sizeof(uint64_t) * (num_elements+1));
//  g_autofree uint64_t* stack_n   = malloc(sizeof(uint64_t) * (num_elements+1));
//  int64_t stack_pointer     = -1;
//  int64_t on_stack_pointer  = -1;
//  int64_t stack_rec_pointer = -1;

//  uint16_t index = 1;
//  uint64_t vn, v, n, i, ni;
//  uint16_t label;
//  uint64_t w;
//  uint8_t recursive;

//  uint16_t* data_c  = at_array_get(component_label);
//  uint8_t*  data_i  = at_array_get(indices);
//  uint8_t*  data_o  = at_array_get(on_stack);
//  uint8_t*  data_ne = at_array_get(neighbors_edges);
//  uint64_t* data_n  = at_array_get(neighbors);

//  for(i = 0; i < num_elements; i++){
//    if(at_array_get(indices,i) == 0){
//      v             = i;
//      recursive     = TRUE;
//      ni            = 0;
//      stack_pointer = -1;
//      while(1){
//        // strongconnect(v)
//        if(recursive){
//          data_i[v] = index;
//          data_c[v] = index;
//          index++;
//          stack_pointer++;
//          stack_v[stack_pointer] = v;
//          on_stack_pointer       = stack_pointer;
//          data_o[v] = TRUE;
//          recursive = FALSE;
//        }

//        // Consider successors of i
//        vn = v * num_neighbors;
//        for(n = ni; n < num_neighbors; n++){
//          if(data_ne[vn+n] == TRUE){
//            w = data_n[vn + n];
//            if(data_i[w] == 0){
//              // strongconnect(v)
//              recursive = TRUE;
//              stack_rec_pointer++;
//              stack_rec[stack_rec_pointer] = v;
//              stack_n[stack_rec_pointer] = n+1;
//              v  = w;
//              ni = 0;
//              break;
//            }else if(data_o[w] == TRUE){
//              data_c[v] = min(data_c[v],data_i[w]);
//            }
//          }
//        }
//        if(!recursive){
//          if(n == num_neighbors){
//            if(data_c[v] == data_i[v]){
//              label = data_c[v];
//              do{
//                w = stack_v[stack_pointer];
//                data_o[w] = FALSE;
//                data_c[w] = label;
//                stack_pointer--;
//              }while(w != v);
//            }
//            if(stack_rec_pointer >= 0){
//              w  = v;
//              v  = stack_rec[stack_rec_pointer];
//              ni = stack_n[stack_rec_pointer];
//              stack_rec_pointer--;

//              data_c[v] = min(data_c[v], data_c[w]);
//            }
//            else break;
//          }
//        }
//      }
//    }
//  }

//  // incremental label
//  memset(stack_v, 0, sizeof(uint64_t) * (num_elements+1));
//  index = 0;
//  for(i = 0; i < num_elements; i++){
//    v = data_c[i];
//    if(stack_v[v] == 0){
//      stack_v[v] = ++index;
//    }
//    data_c[i] = (uint16_t)stack_v[v];
//  }
//}


