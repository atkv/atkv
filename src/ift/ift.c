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

#include <at/ift.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/

// These are relative indices used for calculating the absolute
// indices of neighbors
static int64_t
neighbors_idx_rel_2D[16] = { 0,-1,  0, 1, -1, 0,  1, 0, // neighbor-4
                            -1, 1,  1, 1, -1,-1,  1,-1};// neighbor-8
static int64_t
neighbors_idx_rel_3D[78] = { 0, 0,-1,  0, 0, 1, // neighbor-6
                             0,-1, 0,  0, 1, 0,
                            -1, 0, 0,  1, 0, 0,

                             0,-1,-1,  0,-1, 1, // neighbor-18
                             0, 1,-1,  0, 1, 1,
                            -1, 0,-1, -1, 0, 1,
                             1, 0,-1,  1, 0, 1,
                            -1,-1, 0, -1, 1, 0,
                             1,-1, 0,  1, 1, 0,

                            -1,-1,-1, -1,-1, 1, // neighbor-26
                            -1, 1,-1, -1, 1, 1,
                             1,-1,-1,  1,-1, 1,
                             1, 1,-1,  1, 1, 1};

typedef struct _AtIFTArray_uint8_tPrivate{
  // Will be received
  AtArray_uint8_t*           original;
  AtArray_uint64_t*          seeds;
  AtAdjacency                adjacency;
  AtOptimization             optimization;
  AtConnectivityFunc_uint8_t connectivity_func;
  AtWeightingFunc_uint8_t    weighting_func;
  uint16_t                   map_dimension;

  // Will be calculated
  AtArray_uint8_t*           labels;
  AtArray_uint64_t*          roots;
  AtArray_uint64_t*          predecessors;
  AtArray_double*            connectivity;

  // Map of neighbors
  AtArray_uint64_t*          neighbors;
  AtArray_uint8_t*           neighbors_active; // if we cut arc between neighbors
                                               // we set this flag as false
  // Map of weights
  AtArray_double*            weights;
  uint8_t                    max_value;
  uint8_t                    min_value;

  // Same as original, but values = True|False
  // True if seed
  // False if not
  AtArray_double*            original_is_seed;

  AtBucketQueue_uint64_t*    bucket_queue;
}AtIFTArray_uint8_tPrivate;




G_DEFINE_TYPE_WITH_PRIVATE(AtIFTArray_uint8_t, at_ift_array_uint8_t, G_TYPE_OBJECT)
static void
at_ift_array_uint8_t_finalize(GObject* object){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(AT_IFT_ARRAY_uint8_t(object));
  at_bucketqueue_destroy(&priv->bucket_queue);
  G_OBJECT_CLASS(at_ift_array_uint8_t_parent_class)->finalize(object);
}

static void
at_ift_array_uint8_t_dispose(GObject* object){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(AT_IFT_ARRAY_uint8_t(object));
  g_clear_object(&priv->connectivity);
  g_clear_object(&priv->labels);
  g_clear_object(&priv->neighbors);
  g_clear_object(&priv->neighbors_active);
  g_clear_object(&priv->original);
  g_clear_object(&priv->original_is_seed);
  g_clear_object(&priv->predecessors);
  g_clear_object(&priv->roots);
  g_clear_object(&priv->seeds);
  g_clear_object(&priv->weights);
  G_OBJECT_CLASS(at_ift_array_uint8_t_parent_class)->dispose(object);
}

static void
at_ift_array_uint8_t_init(AtIFTArray_uint8_t *self){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(self);

  priv->original          = NULL;
  priv->seeds             = NULL;
  priv->adjacency         = AT_ADJACENCY_4;
  priv->optimization      = AT_OPTIMIZATION_MIN;
  priv->connectivity_func = at_connectivity_max_uint8_t;
  priv->weighting_func    = at_weighting_abs_diff_uint8_t;

  priv->labels            = NULL;
  priv->roots             = NULL;
  priv->predecessors      = NULL;
  priv->connectivity      = NULL;

  priv->neighbors         = NULL;
  priv->original_is_seed  = NULL;
  priv->weights           = NULL;
  priv->bucket_queue      = NULL;
}

static void
at_ift_array_uint8_t_class_init(AtIFTArray_uint8_tClass *klass){
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = at_ift_array_uint8_t_dispose;
  object_class->finalize = at_ift_array_uint8_t_finalize;
}

static AtIFTArray_uint8_t*
at_ift_array_uint8_t_new(){
  return g_object_new(AT_TYPE_IFT_ARRAY_uint8_t, NULL);
}

static void
at_ift_array_uint8_t_create_neighbors_map(AtIFTArray_uint8_t* ift){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  // Create neighbors map
  //------------------------------------------------------------------------------------
  // dim_neighbors = dim + 1
  // size_neighbors = {size[0],...,size[dim-1], 4|6|8|18|26};
  uint16_t  original_dim  = at_array_get_dim (priv->original);
  uint64_t* original_size = at_array_get_size(priv->original);
  uint64_t* original_step = at_array_get_step(priv->original);
  uint64_t  num_elements  = at_array_get_num_elements(priv->original);
  g_autofree uint64_t* size_neighbors = malloc(sizeof(uint64_t) * (priv->map_dimension + 1));

  // Copy original size
  memcpy(size_neighbors, original_size, sizeof(uint64_t)*priv->map_dimension);

  // Add number of neighbors
  size_neighbors[priv->map_dimension] = at_adjacency_to_int(priv->adjacency);

  at_array_zeros(&priv->neighbors,        priv->map_dimension + 1, size_neighbors);

  // Add arcs flags
  at_array_zeros(&priv->neighbors_active, priv->map_dimension + 1, size_neighbors);

  // Fill neighbors map
  // TODO: Generalize for several predefined adjacencies and dimensions
  uint64_t* neighbors_data = at_array_uint64_t_get_data(priv->neighbors);
  uint8_t*  neighbors_active_data = at_array_uint8_t_get_data(priv->neighbors_active);
  uint64_t  i,j,k,j2,c;
  int64_t   neighbor_abs_index;
  uint64_t  num_neighbors = size_neighbors[priv->map_dimension];
  uint64_t  i_coords[3];
  int64_t  neighbor_coords[3];
  uint64_t  i_remaining;
  uint8_t   out_of_range;
  int64_t  *neighbors_idx_rel;

  if(priv->map_dimension == 3)
    neighbors_idx_rel = neighbors_idx_rel_3D;
  else
    neighbors_idx_rel = neighbors_idx_rel_2D;

  for(i = 0,k = 0; i < num_elements; i++){
    i_remaining = i;
    // convert 1D index to ND index
    for(c = 0; c < priv->map_dimension; c++){
      i_coords[c] = i_remaining / original_step[c];
      i_remaining = i_remaining % original_step[c];
    }

    for(j = 0; j < num_neighbors; j++, k++){
      out_of_range = FALSE;
      // Get proper index of first relative coordinate
      j2 = j<<1;
      if(priv->map_dimension == 3) j2 += j;

      // Get ND coords of neighbor
      // Because we test boundary in each axis
      neighbor_abs_index = 0;
      for(c = 0; c < priv->map_dimension; c++){
        neighbor_coords[c] = i_coords[c] + neighbors_idx_rel[j2 + c];
        // Is it outside of array? (test for every axis)
        if(neighbor_coords[c] < 0 || neighbor_coords[c] >= original_size[c]){
          out_of_range = TRUE;
          break;
        }
        // Convert back to 1D
        neighbor_abs_index += neighbor_coords[c] * original_step[c];
      }

      // If neighbor index is inside valid range
      if(!out_of_range){
        neighbors_data[k] = neighbor_abs_index;
        neighbors_active_data[k] = TRUE;
      }
    }
  }
}

static void
at_ift_array_uint8_t_create_weights_map(AtIFTArray_uint8_t* ift){
  uint64_t i,j,k;
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  uint64_t num_elements = at_array_get_num_elements(priv->original);
  at_array_min(priv->original, &priv->min_value);
  at_array_max(priv->original, &priv->max_value);
  at_array_zeros(&priv->weights,at_array_get_dim(priv->neighbors),at_array_get_size(priv->neighbors));
  // Fill weights map
  //------------------------------------------------------------------------------------
  double* weights_data  = at_array_double_get_data(priv->weights);
  uint64_t num_neighbors = at_array_get_size(priv->weights)[at_array_get_dim(priv->weights)-1];
  for(i = 0,k = 0; i < num_elements; i++){
    for(j = 0; j < num_neighbors; j++,k++){
      if(at_array_get(priv->neighbors_active,k)){
        weights_data[k] = priv->weighting_func(ift, i, at_array_get(priv->neighbors,k));
      }
    }
  }
}
static void
at_ift_array_uint8_t_init_maps(AtIFTArray_uint8_t* ift){
  uint64_t i;
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  uint64_t num_elements = at_array_get_num_elements(priv->original);
  at_array_new(&priv->predecessors, at_array_get_dim(priv->original), at_array_get_size(priv->original));
  at_array_new(&priv->roots       , at_array_get_dim(priv->original), at_array_get_size(priv->original));
  at_array_new(&priv->connectivity, at_array_get_dim(priv->original), at_array_get_size(priv->original));

  for(i = 0; i < num_elements; i++){
    at_array_set(priv->predecessors, i, i);
    at_array_set(priv->roots, i, i);
    at_array_set(priv->connectivity, i, priv->connectivity_func(ift,i,i));
  }

  // Inicializar os mapas nas sementes
}

static double
at_ift_get_weight(AtIFTArray_uint8_t* ift, uint64_t index_s, uint64_t index_t){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  uint64_t num_neighbors = at_array_get_size(priv->weights)[at_array_get_dim(priv->weights)-1];
  uint64_t index_t_weights, i;
  for(i = 0, index_t_weights = index_s*num_neighbors; i < num_neighbors; i++, index_t_weights++){
    if(at_array_get(priv->neighbors_active, index_t_weights)){
      if(at_array_get(priv->neighbors,index_t_weights) == index_t){
        return at_array_get(priv->weights, index_t_weights);
      }
    }
  }
  return -1;
}

static void
at_ift_array_uint8_t_init_labels(AtIFTArray_uint8_t *ift){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  at_array_zeros(&priv->labels        , at_array_get_dim(priv->original), at_array_get_size(priv->original));
  at_array_new(&priv->original_is_seed, at_array_get_dim(priv->original), at_array_get_size(priv->original));

  uint64_t* seeds_data = at_array_uint64_t_get_data(priv->seeds);
  uint64_t seeds_num_elements = at_array_get_num_elements(priv->seeds) >> 1;

  uint64_t i, i2;
  for(i = 0; i < seeds_num_elements; i++){
    i2 = i << 1;
    at_array_set(priv->labels, seeds_data[i2], seeds_data[i2+1]);
    at_array_set(priv->original_is_seed, seeds_data[i2], TRUE);
  }
}

static void
at_ift_array_uint8_t_create_bucketqueue(AtIFTArray_uint8_t* ift){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  uint8_t original_max;
  at_array_max(priv->original, &original_max);
  at_bucketqueue_new_with_size(&priv->bucket_queue, original_max+1,at_array_get_num_elements(priv->original), priv->optimization);

  // add seeds to bucket queue
  uint64_t num_seeds = at_array_get_num_elements(priv->seeds) >> 1;
  uint64_t i, i2;
  double seed_connectivity;
  uint64_t seed_connectivity_uint64_t;
  uint64_t bucket;
  uint64_t seed_index;
  for(i = 0; i < num_seeds; i++){
    i2 = i << 1;
    seed_index = at_array_get(priv->seeds, i2);
    seed_connectivity = at_array_get(priv->connectivity, seed_index);
    seed_connectivity_uint64_t = (uint64_t)round(seed_connectivity);
    bucket = seed_connectivity_uint64_t % at_bucketqueue_get_nbuckets(priv->bucket_queue);
    at_bucketqueue_insert(priv->bucket_queue,bucket,seed_index);
  }
}

static void
at_ift_array_uint8_t_start_loop(AtIFTArray_uint8_t* ift){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  uint64_t index_current;         // node which will offer best path values
  uint64_t index_neighbor;        //   for neighbors
  uint8_t  num_neighbors;         // max number of neighbors in neighbor map
  uint64_t index_current_neighbor;// index of current node in neighbor map
  double   connectivity_extended; // f(π_a . <a,b>)
  double   connectivity_current;  // f(π_b)
  uint64_t connectivity_extended_uint64_t; // f(π_a . <a,b>)
  uint64_t connectivity_current_uint64_t;  // f(π_b)
  AtBucketQueueNodeState neighbor_state; // Controlling bucket flow
  uint64_t nbuckets = at_bucketqueue_get_nbuckets(priv->bucket_queue);
  uint64_t i;
  uint8_t  is_best;

  // while the bucket queue is not empty
  while(!at_bucketqueue_is_empty(priv->bucket_queue)){

    // Get node a with minimum/maximum connectivity value
    index_current = at_bucketqueue_remove(priv->bucket_queue);

    // For each adjacent node
    num_neighbors = at_adjacency_to_int(priv->adjacency);
    index_current_neighbor = index_current * num_neighbors;
    for(i = 0; i < num_neighbors; i++){
      if(at_array_get(priv->neighbors_active,index_current_neighbor + i)){
        index_neighbor = at_array_get(priv->neighbors, index_current_neighbor + i);
        neighbor_state = at_bucketqueue_get_state(priv->bucket_queue, index_neighbor);

        // if it's not removed yet
        if(neighbor_state != AT_BUCKETQUEUE_NODE_REMOVED){

          // get f(π_a . <a,b>)
          connectivity_extended = priv->connectivity_func(ift,index_current, index_neighbor);
          connectivity_current  = at_array_get(priv->connectivity,index_neighbor);
          connectivity_extended_uint64_t = (uint64_t)round(connectivity_extended);
          connectivity_current_uint64_t = (uint64_t)round(connectivity_current);

          if(priv->optimization == AT_OPTIMIZATION_MAX)
            // if f(π_a . <a,b>) > f(π_b) (higher is better)
            is_best = connectivity_extended > connectivity_current;
          else
            // if f(π_a . <a,b>) < f(π_b) (lower is better)
            is_best = connectivity_extended < connectivity_current;

          if(is_best){
            // Insert (if not yet) or move to proper bucket
            if(neighbor_state == AT_BUCKETQUEUE_NODE_NOT_INSERTED)
              at_bucketqueue_insert(priv->bucket_queue, connectivity_extended_uint64_t % nbuckets,index_neighbor);
            else
              at_bucketqueue_update(priv->bucket_queue, connectivity_current_uint64_t % nbuckets, index_neighbor, connectivity_extended_uint64_t % nbuckets);

            // Update maps
            at_array_set(priv->connectivity, index_neighbor, connectivity_extended);
            at_array_set(priv->predecessors, index_neighbor, index_current);
            at_array_set(priv->labels,       index_neighbor, at_array_get(priv->labels, index_current));
            at_array_set(priv->roots,        index_neighbor, at_array_get(priv->roots, index_current));
          }
        }
      }
    }
  }
}
void
at_seeds_filter_by_label(AtArray_uint64_t** seeds_background,
                         AtArray_uint64_t* seeds, uint64_t label){
  g_autofree uint64_t* seeds_size = at_array_get_size(seeds);
  g_autofree uint64_t* seeds_background_data = malloc(sizeof(uint64_t) * seeds_size[0] * 2);
  uint64_t* data = at_array_uint64_t_get_data(seeds);
  uint64_t  index = 0;
  uint64_t  i;
  uint64_t num_elements = seeds_size[0] << 1;
  for(i = 0; i < num_elements; i += 2){
    if(data[i+1] == label)
    {
      seeds_background_data[index] = data[i];
      seeds_background_data[index+1] = data[i+1];
      index += 2;
    }
  }
  seeds_background_data = realloc(seeds_background_data,sizeof(uint64_t) * index);
  seeds_size[0]         = index >> 1;
  at_array_new(seeds_background,2,seeds_size,seeds_background_data);
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

void
at_ift_apply_array_uint8_t(AtIFTArray_uint8_t**       ift,
                           AtArray_uint8_t*           image,
                           uint16_t                   map_dimension,
                           AtAdjacency                adjacency,
                           AtOptimization             optimization,
                           AtConnectivityFunc_uint8_t connectivity_func,
                           AtWeightingFunc_uint8_t    weighting_func,
                           AtArray_uint64_t*          seeds){
  // Create the new structure
  g_set_object(ift, at_ift_array_uint8_t_new());
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(*ift);

  // Save parameters
  g_set_object(&priv->original, image);
  g_set_object(&priv->seeds, seeds);
  priv->adjacency         = adjacency;
  priv->optimization      = optimization;
  priv->connectivity_func = connectivity_func;
  priv->weighting_func    = weighting_func;
  priv->map_dimension     = map_dimension;

  // Init labels and convert list of seeds
  // to array of flags
  at_ift_array_uint8_t_init_labels(*ift);

  // Create map of neighbors
  at_ift_array_uint8_t_create_neighbors_map(*ift);

  // Create map of weight values
  at_ift_array_uint8_t_create_weights_map(*ift);

  // Initialize maps: root, predecessors and connectivity
  at_ift_array_uint8_t_init_maps(*ift);

  // Inicializar o bucket queue
  at_ift_array_uint8_t_create_bucketqueue(*ift);

  // Realizar o loop
  at_ift_array_uint8_t_start_loop(*ift);

  // Destroy bucket queue
  at_bucketqueue_destroy(&priv->bucket_queue);
}

void
at_orfc_in_cut_apply_array_uint8_t(AtIFTArray_uint8_t** ift,
                                   AtArray_uint8_t* image,
                                   uint16_t map_dimension,
                                   AtAdjacency adjacency,
                                   AtOptimization optimization,
                                   AtConnectivityFunc_uint8_t connectivity_func,
                                   AtWeightingFunc_uint8_t weighting_func,
                                   AtArray_uint64_t* seeds){
  g_autoptr(AtArray(uint64_t)) seeds_background = NULL;
  at_seeds_filter_by_label(&seeds_background, seeds, 0);
  at_ift_apply_array_uint8_t(ift,image,map_dimension,adjacency,optimization,connectivity_func,weighting_func,seeds_background);

  // Clonar o grapharray do IFT

  // Inverter o dígrafo

  // Para cada semente de objeto

  // Preencha o DCC da semente com o rótulo

  //
}

void
at_orfc_out_cut_apply_array_uint8_t(AtIFTArray_uint8_t** ift,
                                    AtArray_uint8_t* image,
                                    uint16_t map_dimension,
                                    AtAdjacency adjacency,
                                    AtOptimization optimization,
                                    AtConnectivityFunc_uint8_t connectivity_func,
                                    AtWeightingFunc_uint8_t weighting_func,
                                    AtArray_uint64_t* seeds){

}

AtArray_uint16_t*
at_orfc_out_cut_core_array_uint8_t(AtIFTArray_uint8_t**       ift,
                                   AtArray_uint8_t*           image,
                                   uint16_t                   map_dimension,
                                   AtAdjacency                adjacency,
                                   AtOptimization             optimization,
                                   AtConnectivityFunc_uint8_t connectivity_func,
                                   AtWeightingFunc_uint8_t    weighting_func,
                                   AtArray_uint64_t*          seeds){
  // Apply IFT to find Vb(si)
  g_autoptr(AtArray(uint64_t)) seeds_background = NULL;
  at_seeds_filter_by_label(&seeds_background, seeds, 0);
  at_ift_apply_array_uint8_t(ift,image,map_dimension,adjacency,optimization,connectivity_func,weighting_func,seeds_background);

  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(*ift);

  // Get objects seeds (we use just one in this function)
  g_autoptr(AtArray(uint64_t)) seeds_object = NULL;
  at_seeds_filter_by_label(&seeds_object, seeds, 1);

  // Energy of object seed
  uint64_t si_index = at_array_get(seeds_object, 0);
  double vb_si = at_array_get(priv->connectivity, si_index);

  // Create a grapharray
  g_autoptr(AtGraphArray) grapharray = NULL;
  g_autofree uint64_t* size = at_array_get_size(priv->original);
  at_grapharray_new(&grapharray,map_dimension,size,adjacency);

  uint64_t num_neighbors = at_grapharray_get_num_neighbors(grapharray);
  uint64_t i, n, in,a,b;
  double vb_a, vb_b;
  uint64_t num_elements = at_array_get_num_elements(priv->original);

  // Remove edges with different energies or lower weight values
  uint8_t weight, neighbors_in;
  AtArray(uint64_t)* neighbors = at_grapharray_get_neighbors(grapharray);
  AtArray(uint8_t)* edges = at_grapharray_get_neighbors_edges(grapharray);
  for(i = 0, in = 0; i < num_elements; i++){
    for(n = 0; n < num_neighbors; n++, in++){
      neighbors_in = at_array_get(edges,in);
      if(neighbors_in){
        a = i;
        b = at_array_get(neighbors,in);
        vb_a = at_array_get(priv->connectivity, a);
        vb_b = at_array_get(priv->connectivity, b);
        weight = at_array_get(priv->weights,in);

        if(vb_a != vb_si || vb_b != vb_si || vb_a != vb_b ||
           (optimization == AT_OPTIMIZATION_MIN && weight >= vb_si) ||
           (optimization == AT_OPTIMIZATION_MAX && weight <= vb_si)){
          at_grapharray_remove_arc_by_index(grapharray,in);
        }
      }
    }
  }

  // Find Tarjan Components
  AtArray(uint16_t)* component = NULL;
  at_graph_component_from_grapharray(&component, grapharray);

  return component;
}

double
at_connectivity_min_uint8_t  (AtIFTArray_uint8_t* ift,
                              uint64_t index_s, uint64_t index_t){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  uint8_t is_seed;
  if(index_s == index_t){
    is_seed = at_array_get(priv->original_is_seed,index_s);
    if(is_seed)
      return  DBL_MAX;
    else
      return -DBL_MAX;
  }
  return fmin(at_array_get(priv->connectivity,index_s), at_ift_get_weight(ift, index_s, index_t));
}

double
at_connectivity_max_uint8_t  (AtIFTArray_uint8_t* ift,
                              uint64_t index_s, uint64_t index_t){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  uint8_t is_seed;
  if(index_s == index_t){
    is_seed = at_array_get(priv->original_is_seed,index_s);
    if(is_seed)
      return -DBL_MAX;
    else
      return  DBL_MAX;
  }
  return fmax(at_array_get(priv->connectivity,index_s), at_ift_get_weight(ift, index_s, index_t));
}

double
at_weighting_abs_diff_uint8_t     (AtIFTArray_uint8_t* ift,
                                   uint64_t index_s, uint64_t index_t){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  return fabs((double)at_array_get(priv->original,index_s)-(double)at_array_get(priv->original,index_t));
}
double
at_weighting_abs_diff_r_uint8_t(AtIFTArray_uint8_t* ift, uint64_t index_s, uint64_t index_t){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  return priv->max_value - at_weighting_abs_diff_uint8_t(ift,index_s, index_t);
}

// ------------------------------
// GETTERS
// ------------------------------
AtArray_uint8_t*
at_ift_get_labels_uint8_t(AtIFTArray_uint8_t* ift){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  return priv->labels;
}

AtArray_uint64_t*
at_ift_get_predecessors_uint8_t(AtIFTArray_uint8_t* ift){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  return priv->predecessors;
}

AtArray_uint64_t*
at_ift_get_roots_uint8_t(AtIFTArray_uint8_t* ift){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  return priv->roots;
}

AtArray_double*
at_ift_get_connectivities_uint8_t(AtIFTArray_uint8_t* ift){
  AtIFTArray_uint8_tPrivate* priv = at_ift_array_uint8_t_get_instance_private(ift);
  return priv->connectivity;
}

