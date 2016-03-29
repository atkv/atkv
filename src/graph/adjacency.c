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

uint8_t
at_adjacency_to_int(AtAdjacency adjacency){
  switch(adjacency){
    case AT_ADJACENCY_4:  return 4;
    case AT_ADJACENCY_8:  return 8;
    case AT_ADJACENCY_6:  return 6;
    case AT_ADJACENCY_18: return 18;
    case AT_ADJACENCY_26: return 26;
  }
}
