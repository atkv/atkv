/* ===================================================================
#   Copyright (C) 2015-2015
#   Anderson Tavares <nocturne.pe at gmail.com> PK 0x38e7bfc5c2def8ff
#   Lucy Mansilla    <lucyacm at gmail.com>
#   Caio de Braz     <caiobraz at gmail.com>
#   Hans Harley      <hansbecc at gmail.com>
#   Paulo Miranda    <pavmbr at yahoo.com.br>
#
#   Institute of Mathematics and Statistics - IME
#   University of Sao Paulo - USP
#
#   This file is part of Grafeo.
#
#   Grafeo is free software: you can redistribute it and/or
#   modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation, either version
#   3 of the License, or (at your option) any later version.
#
#   Grafeo is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty
#   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#   See the GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public
#   License along with Grafeo.  If not, see
#   <http://www.gnu.org/licenses/>.
# ===================================================================*/
#include <grf/core.h>
#include <stdlib.h>
#include <string.h>

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
 typedef struct _GrfArray_basePrivate GrfArray_basePrivate;
struct _GrfArray_basePrivate{
  uint16_t      dim;
  uint64_t*     size;
  uint64_t*     step;
  uint64_t      num_elements;
  size_t        elemsize;
  GrfDataType   datatype;
  GrfArray_base* parent;
  gboolean      contiguous;
  uint8_t       alignment; // to align the structure
};

G_DEFINE_TYPE_WITH_PRIVATE(GrfArray_base, grf_array_base, G_TYPE_OBJECT)

static void
grf_array_base_dispose(GObject* object){
  GrfArray_base* array = GRF_ARRAY_BASE(object);
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  g_clear_object(&priv->parent);
  G_OBJECT_CLASS(grf_array_base_parent_class)->dispose(object);
}

static void
grf_array_base_finalize(GObject* object){
  GrfArray_base* array = GRF_ARRAY_BASE(object);
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  g_free(priv->size);
  g_free(priv->step);
  G_OBJECT_CLASS(grf_array_base_parent_class)->finalize(object);
}

static void
grf_array_base_init(GrfArray_base *self){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(self);
  priv->dim        = 0;
  priv->size       = NULL;
  priv->step       = NULL;
  priv->elemsize   = 0;
  priv->datatype   = -1;
  priv->parent     = NULL;
  priv->contiguous = TRUE;
  priv->num_elements = 0;
}
static void
grf_array_base_class_init(GrfArray_baseClass *klass){
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = grf_array_base_finalize;
  object_class->dispose  = grf_array_base_dispose;
}

static void
grf_array_base_set_step(GrfArray_base* array, uint64_t* step){
  uint64_t i;
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  priv->step = realloc(priv->step,priv->dim * sizeof(uint64_t));
  for(i = 0; i < priv->dim; i++){
    priv->step[i] = step[i];
  }
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
void
grf_array_base_set_size(GrfArray_base* array, uint16_t dim, uint64_t* size){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  size_t dim_bytes = dim * sizeof(uint64_t);   // dim
  priv->dim = dim;                             // -
  priv->size = realloc(priv->size, dim_bytes); // size
  memcpy(priv->size, size, dim_bytes);         // -
  priv->step = realloc(priv->step, dim_bytes); // step
  priv->step[dim-1] = 1;                       // -
  priv->num_elements = priv->size[dim-1];
  uint16_t i, ir;
  for(i = 0,ir = dim-2; i < dim-1; i++, ir--){ // -
    priv->step[ir]      = priv->step[ir+1] * priv->size[ir+1]; // -
    priv->num_elements *= priv->size[i];
  }
}

void
grf_array_base_set_type(GrfArray_base* array, size_t elemsize, GrfDataType data_type){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  priv->elemsize = elemsize;
  priv->datatype = data_type;
}

uint16_t
grf_array_base_get_dim(GrfArray_base* array){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  return priv->dim;
}
uint64_t
grf_array_base_get_num_elements(GrfArray_base* array){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  return priv->num_elements;
}

size_t
grf_array_base_get_num_bytes(GrfArray_base* array){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  return priv->elemsize * priv->num_elements;
}

size_t
grf_array_base_get_elemsize(GrfArray_base* array){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  return priv->elemsize;
}

uint64_t*
grf_array_base_get_size(GrfArray_base* array){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  if(priv->size == NULL) return NULL;
  size_t size_bytes = priv->dim * sizeof(uint64_t);
  uint64_t* size = (uint64_t*)malloc(size_bytes);
  memcpy(size, priv->size, size_bytes);
  return size;
}

uint64_t
grf_array_base_get_size_at(GrfArray_base* array, uint16_t index){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  return (priv->dim == 0 || priv->size == NULL)?0:priv->size[index];
}

uint64_t*
grf_array_base_get_step(GrfArray_base* array){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  if(priv->step == NULL) return NULL;
  size_t step_bytes = priv->dim * sizeof(uint64_t);
  uint64_t* step = (uint64_t*)malloc(step_bytes);
  memcpy(step, priv->step, step_bytes);
  return step;
}

uint64_t
grf_array_base_get_step_at(GrfArray_base* array, uint16_t index){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  return (priv->dim == 0 || priv->step == NULL)?0:priv->step[index];
}

GrfArray_base*
grf_array_base_get_parent(GrfArray_base* array){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  return priv->parent;
}

gboolean
grf_array_base_get_contiguous(GrfArray_base* array){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  return priv->contiguous;
}


gboolean
grf_array_base_broadcast_is_valid(GrfArray_base* array1, GrfArray_base* array2){
  uint16_t dim1 = grf_array_get_dim(array1);
  uint16_t dim2 = grf_array_get_dim(array2);
  uint16_t min_dim = min(dim1,dim2);
  uint16_t i, i1, i2;
  uint64_t size1, size2;
  for(i = 0, i1 = dim1-1, i2 = dim2-1; i < min_dim; i++, i1--, i2--){
    size1 = grf_array_get_size(array1,i1);
    size2 = grf_array_get_size(array2,i2);
    if(size1 != size2 && size1 != 1 && size2 != 1) return FALSE;
  }
  return TRUE;
}

uint64_t*
grf_array_base_broadcast_get_size(GrfArray_base* array1, GrfArray_base* array2){
  uint16_t  dim1    = grf_array_get_dim(array1);
  uint16_t  dim2    = grf_array_get_dim(array2);
  uint16_t  max_dim = max(dim1, dim2);
  uint64_t* size    = g_malloc(max_dim * sizeof(uint64_t));
  uint16_t  i;
  for(i = 0; i < max_dim; i++){
    if(i < max_dim - dim1) size[i] = grf_array_get_size(array2,i);
    else if(i < max_dim - dim2) size[i] = grf_array_get_size(array1,i);
    else size[i] = max(grf_array_get_size(array1,i - (max_dim - dim1)),grf_array_get_size(array2,i - (max_dim - dim2)));
  }
  return size;
}

uint16_t
grf_array_base_broadcast_get_dim(GrfArray_base* array1, GrfArray_base* array2){
  return max(grf_array_get_dim(array1), grf_array_get_dim(array2));
}

void
grf_array_base_get_indices(GrfArray_base* array, uint64_t index, uint64_t* indices){
  uint16_t i;
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  for(i = 0; i < priv->dim; i++){
    indices[i] = index / priv->step[i];
    index     %= priv->step[i];
  }
}

void
grf_array_base_get_index(GrfArray_base* array, uint64_t* indices, uint64_t* index){
  uint16_t i;
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  *index = 0;
  for(i = 0; i < priv->dim; i++){
    *index += indices[i] * priv->step[i];
  }
}

void
grf_array_base_mod_indices(GrfArray_base* array, uint64_t* indices, uint64_t* output){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  uint16_t i;
  for(i = 0; i < priv->dim; i++){
    output[i] = indices[i] % priv->size[i];
  }
}

uint64_t
grf_array_base_sub(GrfArray_base* array, GrfRange* ranges, GrfArray_base* subarray){
  GrfArray_basePrivate* priv    = grf_array_base_get_instance_private(array);
  GrfArray_basePrivate* subpriv = grf_array_base_get_instance_private(subarray);
  g_autofree uint64_t* size = malloc(sizeof(uint64_t) * priv->dim);
  uint64_t i, offset = 0, from, to;
  for(i = 0; i < grf_array_base_get_dim(array); i++){
    // Convert negative values to positive ones
    if(ranges[i].to < 0)   ranges[i].to   += priv->size[i];
    if(ranges[i].from < 0) ranges[i].from += priv->size[i];

    // Shrink them to fit to array
    to   = min(ranges[i].to, priv->size[i]);
    from = max(ranges[i].from, 0);

    size[i] = to-from;
    offset += from * priv->step[i];
  }
  grf_array_base_set_size(subarray,priv->dim,size);
  grf_array_base_set_step(subarray,priv->step);
  subpriv->contiguous = FALSE;
  g_set_object(&subpriv->parent, array);
  return offset;
}

uint64_t
grf_array_base_get_index_absolute(GrfArray_base* array, uint64_t index, uint64_t* indices_relative){
  GrfArray_basePrivate* priv    = grf_array_base_get_instance_private(array);

  uint64_t i, ir, index_absolute = 0;

  for(i = 0, ir = priv->dim-1; i < priv->dim; i++, ir--){
    // Convert 1D-Relative to ND-Relative
    indices_relative[ir]     = index % priv->size[ir];
    index /= priv->size[ir];

    // Convert ND-Relative to 1D-Absolute
    index_absolute += indices_relative[ir]*priv->step[ir];
  }
  return index_absolute;
}

void
grf_array_base_squeeze(GrfArray_base* array){
  GrfArray_basePrivate* priv    = grf_array_base_get_instance_private(array);
  uint16_t k = 0;
  uint64_t i;
  for(i = 0; i < priv->dim; i++){
    if(priv->size[i] != 1 || (k == 0 && i == (priv->dim - 1))){
      priv->size[k]   = priv->size[i];
      priv->step[k++] = priv->step[i];
    }
  }
  priv->dim  = k;
  priv->size = realloc(priv->size,k*sizeof(uint64_t));
  priv->step = realloc(priv->step,k*sizeof(uint64_t));
}

void
grf_array_base_squeeze_axes(GrfArray_base* array, uint16_t num_axes, uint16_t* axes){
  GrfArray_basePrivate* priv = grf_array_base_get_instance_private(array);
  uint16_t current_pos = axes[0], current_axis_pos = 0,i;
  for(i = current_pos; i < priv->dim; i++){
    // Copy whether non single-dimension or not indicated by axis
    if(current_axis_pos       == num_axes || // Traversed all indicated axis?
       priv->size[i]         != 1 ||        // Non single-dimension?
       axes[current_axis_pos] != i)          // Is not an indicated axis?
      priv->size[current_pos++] = priv->size[i];
    // If indicated, don't copy, just see next indicated axis
    else if(i == axes[current_axis_pos])
      current_axis_pos++;
  }
  priv->dim = current_pos;
  priv->size = realloc(priv->size, current_pos * sizeof(uint64_t));
}

