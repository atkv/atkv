#include <at/graph.h>
#include <string.h>
#include <stdlib.h>
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


typedef struct _AtGraphArrayPrivate{
  AtArray_uint64_t* neighbors;
  AtArray_uint8_t*  neighbors_edges;
  uint64_t          num_neighbors;
}AtGraphArrayPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtGraphArray, at_grapharray, G_TYPE_OBJECT)

static void
at_grapharray_class_init(AtGraphArrayClass *klass){

}
static void
at_grapharray_init(AtGraphArray *self){

}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

/* GRAPHARRAY CREATION
 * ======================*/
void
at_grapharray_create(AtGraphArray** grapharray,
                     uint16_t       dim,
                     uint64_t*      size,
                     AtAdjacency    adjacency){
  // Get proper size
  g_autofree uint64_t* size_grapharray = malloc((dim + 1) * sizeof(uint64_t));
  memcpy(size_grapharray, size, dim * sizeof(uint64_t));
  size_grapharray[dim] = at_adjacency_to_int(adjacency);

  // Create the grapharray object
  *grapharray = g_object_new(AT_TYPE_GRAPHARRAY, NULL);
  AtGraphArrayPrivate *priv = at_grapharray_get_instance_private(*grapharray);

  // Create the arrays
  at_array_new(&priv->neighbors, dim + 1, size_grapharray);

  at_array_zeros(&priv->neighbors_edges, dim+1, size_grapharray);

  // Fill neighbors map
  // TODO: Generalize for several predefined adjacencies and dimensions
  uint64_t* neighbors_data = at_array_uint64_t_get_data(priv->neighbors);
  uint8_t*  neighbors_edges_data = at_array_uint8_t_get_data(priv->neighbors_edges);
  uint64_t  i,j,k,j2,c;
  int64_t   neighbor_abs_index;
  uint64_t  num_neighbors = size_grapharray[dim];
  uint64_t  i_coords[3];
  int64_t   neighbor_coords[3];
  uint64_t  i_remaining;
  uint8_t   out_of_range;
  int64_t*  neighbors_idx_rel;
  uint64_t  num_elements = at_array_get_num_elements(priv->neighbors)/num_neighbors;
  g_autofree uint64_t* step = malloc(dim * sizeof(uint64_t));
  priv->num_neighbors = num_neighbors;
  g_autofree uint64_t* neighbors_step = at_array_get_step(priv->neighbors);
  for(i = 0; i < dim; i++)
    step[i] = neighbors_step[i]/num_neighbors;


  if(dim == 3)
    neighbors_idx_rel = neighbors_idx_rel_3D;
  else
    neighbors_idx_rel = neighbors_idx_rel_2D;

  for(i = 0,k = 0; i < num_elements; i++){
    i_remaining = i;
    // convert 1D index to ND index
    for(c = 0; c < dim; c++){
      i_coords[c] = i_remaining / step[c];
      i_remaining = i_remaining % step[c];
    }

    for(j = 0; j < num_neighbors; j++, k++){
      out_of_range = FALSE;
      // Get proper index of first relative coordinate
      j2 = j<<1;
      if(dim == 3) j2 += j;

      // Get ND coords of neighbor
      // Because we test boundary in each axis
      neighbor_abs_index = 0;
      for(c = 0; c < dim; c++){
        neighbor_coords[c] = i_coords[c] + neighbors_idx_rel[j2 + c];
        // Is it outside of array? (test for every axis)
        if(neighbor_coords[c] < 0 || neighbor_coords[c] >= size[c]){
          out_of_range = TRUE;
          break;
        }
        // Convert back to 1D
        neighbor_abs_index += neighbor_coords[c] * step[c];
      }

      // If neighbor index is inside valid range
      if(!out_of_range){
        neighbors_data[k] = neighbor_abs_index;
        neighbors_edges_data[k] = TRUE;
      }
      else{
        neighbors_data[k] = UINT64_MAX;
      }
    }
  }
}

void
at_grapharray_create_with_arcs(AtGraphArray** grapharray,
                               uint16_t       dim,
                               uint64_t*      size,
                               AtAdjacency    adjacency,
                               uint64_t*      arcs,
                               uint64_t       num_arcs){
  at_grapharray_create(grapharray, dim, size, adjacency);
  at_grapharray_set_arcs(*grapharray, arcs, num_arcs);
}


/* SETTING ARCS AND EDGES
 * ======================*/
void
at_grapharray_set_arcs(AtGraphArray* grapharray,
                       uint64_t*     arcs,
                       uint64_t      num_arcs){
  AtGraphArrayPrivate* priv = at_grapharray_get_instance_private(grapharray);
  uint64_t i, i2, v, w, vn, n;

  at_array_fill(priv->neighbors_edges, 0);
  uint64_t num_neighbors = at_grapharray_get_num_neighbors(grapharray);

  for(i = 0; i < num_arcs; i++){
    // Get indices v and w (i2 because arcs have [v,w,v,w,...]
    i2 = i << 1;
    v = arcs[i2];
    w = arcs[i2 + 1];

    vn = v * num_neighbors;

    for(n = 0; n < num_neighbors; n++){
      if(at_array_get(priv->neighbors,vn+n) == w){
        at_array_set(priv->neighbors_edges, vn+n, TRUE);
        break;
      }
    }
  }
}

void
at_grapharray_set_edges(AtGraphArray* grapharray,
                        uint64_t*     edges,
                        uint64_t      num_edges){

}

/* ADDING ARCS AND EDGES
 * ======================*/

// ARCS
void
at_grapharray_add_arc(AtGraphArray* grapharray, uint64_t node1, uint64_t node2){
  AtGraphArrayPrivate* priv = at_grapharray_get_instance_private(grapharray);
  uint64_t vn = node1 * priv->num_neighbors;
  uint64_t i;
  for(i = 0; i < priv->num_neighbors; i++)
    if(at_array_get(priv->neighbors,vn+i) == node2){
      at_array_set(priv->neighbors_edges, vn+i, TRUE);
      break;
    }
}

AtArray_uint64_t*
at_grapharray_get_neighbors(AtGraphArray* grapharray){
  AtGraphArrayPrivate* priv = at_grapharray_get_instance_private(grapharray);
  return priv->neighbors;
}

AtArray_uint8_t*
at_grapharray_get_neighbors_edges(AtGraphArray* grapharray){
  AtGraphArrayPrivate* priv = at_grapharray_get_instance_private(grapharray);
  return priv->neighbors_edges;
}
uint64_t
at_grapharray_get_num_neighbors(AtGraphArray* grapharray){
  AtGraphArrayPrivate* priv = at_grapharray_get_instance_private(grapharray);
  return priv->num_neighbors;
}
