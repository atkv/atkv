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

#ifndef AT_IFT_IFT_H
#define AT_IFT_IFT_H
#include <at/ift.h>
#include <at/graph.h>
G_BEGIN_DECLS

/*===========================================================================
 * CONSTANTS, ENUMS AND MACROS
 *===========================================================================*/

/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/

#define AT_TYPE_IFT_ARRAY_uint8_t at_ift_array_uint8_t_get_type()
G_DECLARE_DERIVABLE_TYPE(AtIFTArray_uint8_t, at_ift_array_uint8_t, AT, IFT_ARRAY_uint8_t, GObject)

typedef struct _AtIFTArray_uint8_tClass{
  GObjectClass parent_class;
}AtIFTArray_uint8_tClass;

#define AtIFTArray(type) AtIFTArray_##type


/*===========================================================================
 * FUNCTIONS
 *===========================================================================*/
/**
 * @brief: Connectivity function format
 */
typedef double
(*AtConnectivityFunc_uint8_t)(AtIFTArray_uint8_t* ift,
                              uint64_t index_s, uint64_t index_t);

/**
 * @brief: return a weight value between two nodes
 */
typedef double
(*AtWeightingFunc_uint8_t)(AtIFTArray_uint8_t* ift,
                           uint64_t index_s, uint64_t index_t);

/**
 * @brief at_connectivity_min_uint8_t
 * @param ift
 * @param index_s
 * @param index
 * @return
 */
double
at_connectivity_min_uint8_t  (AtIFTArray_uint8_t* ift,
                              uint64_t index_s, uint64_t index_t);

double
at_connectivity_max_uint8_t  (AtIFTArray_uint8_t* ift,
                              uint64_t index_s, uint64_t index_t);

double
at_connectivity_sum_uint8_t  (AtIFTArray_uint8_t* ift,
                              uint64_t index_s, uint64_t index_t);

double
at_connectivity_norm_l1_uint8_t  (AtIFTArray_uint8_t* ift,
                                  uint64_t index_s, uint64_t index_t);

double
at_connectivity_norm_l2_uint8_t  (AtIFTArray_uint8_t* ift,
                                  uint64_t index_s, uint64_t index_t);

double
at_connectivity_norm_l2sqr_uint8_t  (AtIFTArray_uint8_t* ift,
                                  uint64_t index_s, uint64_t index_t);

double
at_connectivity_norm_inf_uint8_t  (AtIFTArray_uint8_t* ift,
                                  uint64_t index_s, uint64_t index_t);

double
at_weighting_abs_diff_uint8_t     (AtIFTArray_uint8_t* ift,
                                   uint64_t index_s, uint64_t index_t);
double
at_weighting_abs_diff_r_uint8_t   (AtIFTArray_uint8_t* ift,
                                   uint64_t index_s, uint64_t index_t);


/**
 * @brief Apply IFT to an Array with predefined adjacency and connectivity
 *        function
 * @param ift structure of results of IFT
 * @param image
 * @param map_dimension
 * @param adjacency
 * @param optimization
 * @param connectivity_func
 */
void
at_ift_apply_array_uint8_t(AtIFTArray_uint8_t** ift,
                           AtArray_uint8_t* image,
                           uint16_t map_dimension,
                           AtAdjacency adjacency,
                           AtOptimization optimization,
                           AtConnectivityFunc_uint8_t connectivity_func,
                           AtWeightingFunc_uint8_t weighting_func,
                           AtArray_uint64_t* seeds);


AtArray_uint8_t*
at_ift_get_labels_uint8_t(AtIFTArray_uint8_t* ift);

AtArray_uint64_t*
at_ift_get_predecessors_uint8_t(AtIFTArray_uint8_t* ift);

AtArray_uint64_t*
at_ift_get_roots_uint8_t(AtIFTArray_uint8_t* ift);

AtArray_double*
at_ift_get_connectivities_uint8_t(AtIFTArray_uint8_t* ift);


#define at_ift_get_labels(ift) _Generic( (ift), \
  AtIFTArray_uint8_t*: at_ift_get_labels_uint8_t \
)(ift)

#define at_ift_get_predecessors(ift) _Generic( (ift), \
  AtIFTArray_uint8_t*: at_ift_get_predecessors_uint8_t \
)(ift)

#define at_ift_get_roots(ift) _Generic( (ift), \
  AtIFTArray_uint8_t*: at_ift_get_roots_uint8_t \
)(ift)

#define at_ift_get_connectivities(ift) _Generic( (ift), \
  AtIFTArray_uint8_t*: at_ift_get_connectivities_uint8_t \
)(ift)

#define at_connectivity(type,ift) _Generic((ift), \
  AtIFTArray_uint8_t*: at_connectivity_##type##_uint8_t \
)
#define at_weighting(type,ift) _Generic((ift), \
  AtIFTArray_uint8_t*: at_weighting_##type##_uint8_t \
)

#define at_ift_apply(ift, image, map_dimension, adjacency, optimization, connectivity_func, weighting_func, seeds) _Generic((ift), \
  AtIFTArray_uint8_t**: at_ift_apply_array_uint8_t \
)(ift, image, map_dimension, adjacency, optimization, connectivity_func, weighting_func, seeds)


G_END_DECLS
#endif
