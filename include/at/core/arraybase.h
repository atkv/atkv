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

#ifndef AT_ARRAY_BASE_H
#define AT_ARRAY_BASE_H
#include <stdint.h>
#include <glib-object.h>
#include <at/core.h>

/*===========================================================================
 * MACROS, ENUMS AND TYPEDEFS
 *===========================================================================*/
#define AT_TYPE_ARRAY_BASE at_array_base_get_type()
/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
G_DECLARE_DERIVABLE_TYPE(AtArray_base, at_array_base, AT, ARRAY_BASE, GObject)
typedef struct _AtArray_baseClass{
  GObjectClass parent_class;
}AtArray_baseClass;

/*===========================================================================
 * FUNCTIONS
 *===========================================================================*/
/**
 * @brief at_array_base_set_size
 * @param array
 * @param dim
 * @param size
 */
void
at_array_base_set_size(AtArray_base* array, uint16_t dim, uint64_t* size);
/**
 * @brief at_array_base_set_type
 * @param array
 * @param elemsize
 * @param data_type
 */
void
at_array_base_set_type(AtArray_base* array, size_t elemsize, AtDataType data_type);

/**
 * @brief Get the dimensions of the array
 * @param array the array
 * @return the dimensions of the array (2 for 2D, 3 for 3D, N for ND)
 */ 
uint16_t
at_array_base_get_dim(AtArray_base* array);

uint64_t
at_array_base_get_num_elements(AtArray_base* array);

size_t
at_array_base_get_num_bytes(AtArray_base* array);

size_t
at_array_base_get_elemsize(AtArray_base* array);

uint64_t*
at_array_base_get_size(AtArray_base* array);

uint64_t
at_array_base_get_size_at(AtArray_base* array, uint16_t index);

uint64_t*
at_array_base_get_step(AtArray_base* array);

uint64_t
at_array_base_get_step_at(AtArray_base* array, uint16_t index);

AtArray_base*
at_array_base_get_parent(AtArray_base* array);

gboolean
at_array_base_get_contiguous(AtArray_base* array);

gboolean
at_array_base_broadcast_is_valid(AtArray_base* array1, AtArray_base* array2);

uint64_t*
at_array_base_broadcast_get_size(AtArray_base* array1, AtArray_base* array2);

uint16_t
at_array_base_broadcast_get_dim(AtArray_base* array1, AtArray_base* array2);

void
at_array_base_get_indices(AtArray_base* array, uint64_t index, uint64_t* indices);

void
at_array_base_get_index(AtArray_base* array, uint64_t* indices, uint64_t* index);

void
at_array_base_mod_indices(AtArray_base* array, uint64_t* indices, uint64_t* output);

uint64_t
at_array_base_sub(AtArray_base* array, AtRange* ranges, AtArray_base* subarray);

/**
 * @brief Get absolute 1D offset, from array beginning
 *
 * This is used for subarrays.
 *
 * @param array
 * @param index the relative 1D index
 * @param indices_relative the buffer for ND relative indices
 * @return
 */
uint64_t
at_array_base_get_index_absolute(AtArray_base* array, uint64_t index, uint64_t* indices_relative);
/**
 * @brief at_array_base_squeeze
 * @param array
 */
void
at_array_base_squeeze(AtArray_base* array);
/**
 * @brief at_array_base_squeeze_axes
 * @param array
 * @param num_axes
 * @param axes
 */
void
at_array_base_squeeze_axes(AtArray_base* array, uint16_t num_axes, uint16_t* axes);

#endif
