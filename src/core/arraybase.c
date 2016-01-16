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
#include <at/core.h>
#include <stdlib.h>
#include <string.h>

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
 typedef struct _AtArray_basePrivate AtArray_basePrivate;
struct _AtArray_basePrivate{
  uint16_t      dim;
  uint64_t*     size;
  uint64_t*     step;
  uint64_t      num_elements;
  size_t        elemsize;
  AtDataType   datatype;
  AtArray_base* parent;
  gboolean      contiguous;
  uint8_t       alignment; // to align the structure
};

G_DEFINE_TYPE_WITH_PRIVATE(AtArray_base, at_array_base, G_TYPE_OBJECT)

static void
at_array_base_dispose(GObject* object){
  AtArray_base* array = AT_ARRAY_BASE(object);
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  g_clear_object(&priv->parent);
  G_OBJECT_CLASS(at_array_base_parent_class)->dispose(object);
}

static void
at_array_base_finalize(GObject* object){
  AtArray_base* array = AT_ARRAY_BASE(object);
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  g_free(priv->size);
  g_free(priv->step);
  G_OBJECT_CLASS(at_array_base_parent_class)->finalize(object);
}

static void
at_array_base_init(AtArray_base *self){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(self);
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
at_array_base_class_init(AtArray_baseClass *klass){
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = at_array_base_finalize;
  object_class->dispose  = at_array_base_dispose;
}

static void
at_array_base_set_step(AtArray_base* array, uint64_t* step){
  uint64_t i;
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  priv->step = realloc(priv->step,priv->dim * sizeof(uint64_t));
  for(i = 0; i < priv->dim; i++){
    priv->step[i] = step[i];
  }
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
void
at_array_base_set_size(AtArray_base* array, uint16_t dim, uint64_t* size){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
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
at_array_base_set_type(AtArray_base* array, size_t elemsize, AtDataType data_type){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  priv->elemsize = elemsize;
  priv->datatype = data_type;
}

uint16_t
at_array_base_get_dim(AtArray_base* array){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  return priv->dim;
}
uint64_t
at_array_base_get_num_elements(AtArray_base* array){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  return priv->num_elements;
}

size_t
at_array_base_get_num_bytes(AtArray_base* array){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  return priv->elemsize * priv->num_elements;
}

size_t
at_array_base_get_elemsize(AtArray_base* array){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  return priv->elemsize;
}

uint64_t*
at_array_base_get_size(AtArray_base* array){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  if(priv->size == NULL) return NULL;
  size_t size_bytes = priv->dim * sizeof(uint64_t);
  uint64_t* size = (uint64_t*)malloc(size_bytes);
  memcpy(size, priv->size, size_bytes);
  return size;
}

uint64_t
at_array_base_get_size_at(AtArray_base* array, uint16_t index){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  return (priv->dim == 0 || priv->size == NULL)?0:priv->size[index];
}

uint64_t*
at_array_base_get_step(AtArray_base* array){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  if(priv->step == NULL) return NULL;
  size_t step_bytes = priv->dim * sizeof(uint64_t);
  uint64_t* step = (uint64_t*)malloc(step_bytes);
  memcpy(step, priv->step, step_bytes);
  return step;
}

uint64_t
at_array_base_get_step_at(AtArray_base* array, uint16_t index){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  return (priv->dim == 0 || priv->step == NULL)?0:priv->step[index];
}

AtArray_base*
at_array_base_get_parent(AtArray_base* array){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  return priv->parent;
}

gboolean
at_array_base_get_contiguous(AtArray_base* array){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  return priv->contiguous;
}


gboolean
at_array_base_broadcast_is_valid(AtArray_base* array1, AtArray_base* array2){
  uint16_t dim1 = at_array_get_dim(array1);
  uint16_t dim2 = at_array_get_dim(array2);
  uint16_t min_dim = min(dim1,dim2);
  uint16_t i, i1, i2;
  uint64_t size1, size2;
  for(i = 0, i1 = dim1-1, i2 = dim2-1; i < min_dim; i++, i1--, i2--){
    size1 = at_array_get_size(array1,i1);
    size2 = at_array_get_size(array2,i2);
    if(size1 != size2 && size1 != 1 && size2 != 1) return FALSE;
  }
  return TRUE;
}

uint64_t*
at_array_base_broadcast_get_size(AtArray_base* array1, AtArray_base* array2){
  uint16_t  dim1    = at_array_get_dim(array1);
  uint16_t  dim2    = at_array_get_dim(array2);
  uint16_t  max_dim = max(dim1, dim2);
  uint64_t* size    = g_malloc(max_dim * sizeof(uint64_t));
  uint16_t  i;
  for(i = 0; i < max_dim; i++){
    if(i < max_dim - dim1) size[i] = at_array_get_size(array2,i);
    else if(i < max_dim - dim2) size[i] = at_array_get_size(array1,i);
    else size[i] = max(at_array_get_size(array1,i - (max_dim - dim1)),at_array_get_size(array2,i - (max_dim - dim2)));
  }
  return size;
}

uint16_t
at_array_base_broadcast_get_dim(AtArray_base* array1, AtArray_base* array2){
  return max(at_array_get_dim(array1), at_array_get_dim(array2));
}

void
at_array_base_get_indices(AtArray_base* array, uint64_t index, uint64_t* indices){
  uint16_t i;
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  for(i = 0; i < priv->dim; i++){
    indices[i] = index / priv->step[i];
    index     %= priv->step[i];
  }
}

void
at_array_base_get_index(AtArray_base* array, uint64_t* indices, uint64_t* index){
  uint16_t i;
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  *index = 0;
  for(i = 0; i < priv->dim; i++){
    *index += indices[i] * priv->step[i];
  }
}

void
at_array_base_mod_indices(AtArray_base* array, uint64_t* indices, uint64_t* output){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
  uint16_t i;
  for(i = 0; i < priv->dim; i++){
    output[i] = indices[i] % priv->size[i];
  }
}

uint64_t
at_array_base_sub(AtArray_base* array, AtRange* ranges, AtArray_base* subarray){
  AtArray_basePrivate* priv    = at_array_base_get_instance_private(array);
  AtArray_basePrivate* subpriv = at_array_base_get_instance_private(subarray);
  g_autofree uint64_t* size = malloc(sizeof(uint64_t) * priv->dim);
  uint64_t i, offset = 0, from, to;
  for(i = 0; i < at_array_base_get_dim(array); i++){
    // Convert negative values to positive ones
    if(ranges[i].to < 0)   ranges[i].to   += priv->size[i];
    if(ranges[i].from < 0) ranges[i].from += priv->size[i];

    // Shrink them to fit to array
    to   = min(ranges[i].to, priv->size[i]);
    from = max(ranges[i].from, 0);

    size[i] = to-from;
    offset += from * priv->step[i];
  }
  at_array_base_set_size(subarray,priv->dim,size);
  at_array_base_set_step(subarray,priv->step);
  subpriv->contiguous = FALSE;
  g_set_object(&subpriv->parent, array);
  return offset;
}

uint64_t
at_array_base_get_index_absolute(AtArray_base* array, uint64_t index, uint64_t* indices_relative){
  AtArray_basePrivate* priv    = at_array_base_get_instance_private(array);

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
at_array_base_squeeze(AtArray_base* array){
  AtArray_basePrivate* priv    = at_array_base_get_instance_private(array);
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
at_array_base_squeeze_axes(AtArray_base* array, uint16_t num_axes, uint16_t* axes){
  AtArray_basePrivate* priv = at_array_base_get_instance_private(array);
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

