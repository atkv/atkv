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

#ifndef AT_GRAPH_ADJACENCY_H
#define AT_GRAPH_ADJACENCY_H

#include <at/graph.h>
#include <stdint.h>

typedef enum{
  AT_ADJACENCY_4,  // For 2D (d(x,y) <= 1)
  AT_ADJACENCY_6,  // For 3D (d(x,y) <= 1)
  AT_ADJACENCY_8,  // For 2D (d(x,y) <= sqrt(2))
  AT_ADJACENCY_18, // For 3D (d(x,y) <= sqrt(2))
  AT_ADJACENCY_26, // For 3D (d(x,y) <= sqrt(3))
  AT_ADJACENCY_CUSTOM,
}AtAdjacency;

/**
 * @brief at_adjacency_to_int
 * @param adjacency
 * @return
 */
uint8_t
at_adjacency_to_int(AtAdjacency adjacency);

#endif
