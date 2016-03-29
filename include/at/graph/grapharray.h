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

#ifndef AT_GRAPH_GRAPHARRAY_H
#define AT_GRAPH_GRAPHARRAY_H

#include <at/graph.h>
#include <at/core.h>
/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define AT_TYPE_GRAPHARRAY at_grapharray_get_type()
G_DECLARE_DERIVABLE_TYPE(AtGraphArray, at_grapharray, AT, GRAPHARRAY, GObject)
typedef struct _AtGraphArrayClass{
  GObjectClass parent_class;
}AtGraphArrayClass;

/*===========================================================================
 * FUNCTIONS
 *===========================================================================*/

/* GRAPHARRAY CREATION
 * ======================*/

/**
 * @brief at_grapharray_new
 * @param grapharray
 * @param dim
 * @param size
 * @param adjacency
 */
void
at_grapharray_create(AtGraphArray** grapharray, uint16_t dim, uint64_t* size, AtAdjacency adjacency);

/**
 * @brief at_grapharray_new_with_arcs
 * @param grapharray
 * @param dim
 * @param size
 * @param adjacency
 * @param arcs
 * @param num_arcs
 */
void
at_grapharray_create_with_arcs(AtGraphArray** grapharray, uint16_t dim, uint64_t* size, AtAdjacency adjacency, uint64_t* arcs, uint64_t num_arcs);

#define at_grapharray_new(...) macro_dispatcher(at_grapharray_new, __VA_ARGS__)(__VA_ARGS__)
#define at_grapharray_new6(grapharray, dim, size, adjacency, arcs, num_arcs) at_grapharray_create_with_arcs(grapharray, dim, size, adjacency, arcs, num_arcs);
#define at_grapharray_new4(grapharray, dim, size, adjacency) at_grapharray_create(grapharray, dim, size, adjacency);

/* SETTING ARCS AND EDGES
 * ======================*/
/**
 * @brief at_grapharray_set_arcs
 * @param grapharray
 * @param arcs
 * @param num_arcs
 */
void
at_grapharray_set_arcs(AtGraphArray* grapharray, uint64_t* arcs, uint64_t num_arcs);
/**
 * @brief at_grapharray_set_edges
 * @param grapharray
 * @param edges
 * @param num_edges
 */
void
at_grapharray_set_edges(AtGraphArray* grapharray, uint64_t* edges, uint64_t num_edges);

/* ADDING ARCS AND EDGES
 * ======================*/

// ARCS
/**
 * @brief at_grapharray_add_arc
 * @param grapharray
 * @param node1
 * @param node2
 */
void
at_grapharray_add_arc(AtGraphArray* grapharray, uint64_t node1, uint64_t node2);
/**
 * @brief at_grapharray_add_arcs
 * @param grapharray
 * @param arcs
 * @param num_arcs
 */
void
at_grapharray_add_arcs(AtGraphArray* grapharray, uint64_t* arcs, uint64_t num_arcs);
/**
 * @brief at_grapharray_add_arc_by_index
 * @param grapharray
 * @param edge_index
 */
void
at_grapharray_add_arc_by_index(AtGraphArray* grapharray, uint64_t arc_index);
/**
 * @brief at_grapharray_add_arcs_by_index
 * @param grapharray
 * @param arc_indices
 * @param num_indices
 */
void
at_grapharray_add_arcs_by_index(AtGraphArray* grapharray, uint64_t* arc_indices, uint64_t num_indices);

// EDGES
/**
 * @brief at_grapharray_add_edge
 * @param grapharray
 * @param node1
 * @param node2
 */
void
at_grapharray_add_edge(AtGraphArray* grapharray, uint64_t node1, uint64_t node2);
/**
 * @brief at_grapharray_add_edges
 * @param grapharray
 * @param edges
 * @param num_edges
 */
void
at_grapharray_add_edges(AtGraphArray* grapharray, uint64_t* edges, uint64_t num_edges);

/**
 * @brief at_grapharray_add_edge_by_index
 * @param grapharray
 * @param edge_index
 */
void
at_grapharray_add_edge_by_index(AtGraphArray* grapharray, uint64_t edge_index);
/**
 * @brief at_grapharray_add_edge_by_index
 * @param grapharray
 * @param edge_index
 */
void
at_grapharray_add_edges_by_indices(AtGraphArray* grapharray, uint64_t* edge_indices, uint64_t num_indices);


/* REMOVING ARCS AND EDGES
 * ========================*/
/**
 * @brief at_grapharray_are_neighbors
 * @param grapharray
 * @param node1
 * @param node2
 */
void
at_grapharray_are_neighbors(AtGraphArray* grapharray, uint64_t node1, uint64_t node2);

/**
 * @brief at_grapharray_remove_arc
 * @param grapharray
 * @param node1
 * @param node2
 */
void
at_grapharray_remove_arc(AtGraphArray* grapharray, uint64_t node1, uint64_t node2);
/**
 * @brief at_grapharray_remove_arcs
 * @param grapharray
 * @param arcs
 * @param num_arcs
 */
void
at_grapharray_remove_arcs(AtGraphArray* grapharray, uint64_t* arcs, uint64_t num_arcs);
/**
 * @brief at_grapharray_remove_edge
 * @param grapharray
 * @param node1
 * @param node2
 */
void
at_grapharray_remove_edge(AtGraphArray* grapharray, uint64_t node1, uint64_t node2);

/**
 * @brief at_grapharray_remove_arc_by_index
 * @param grapharray
 * @param arc_index
 */
void
at_grapharray_remove_arc_by_index(AtGraphArray* grapharray, uint64_t arc_index);

/**
 * @brief at_grapharray_remove_edge_by_index
 * @param grapharray
 * @param edge_index
 */
void
at_grapharray_remove_edge_by_index(AtGraphArray* grapharray, uint64_t edge_index);

/**
 * @brief at_grapharray_get_neighbors
 * @param grapharray
 * @return
 */
AtArray_uint64_t*
at_grapharray_get_neighbors(AtGraphArray* grapharray);
/**
 * @brief at_grapharray_get_neighbors_edges
 * @param grapharray
 * @return
 */
AtArray_uint8_t*
at_grapharray_get_neighbors_edges(AtGraphArray* grapharray);
/**
 * @brief at_grapharray_get_num_neighbors
 * @param grapharray
 * @return
 */
uint64_t
at_grapharray_get_num_neighbors(AtGraphArray* grapharray);

#endif
