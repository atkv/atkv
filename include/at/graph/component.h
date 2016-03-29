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

#ifndef AT_GRAPH_COMPONENT_H
#define AT_GRAPH_COMPONENT_H

#include <at/core.h>
#include <at/graph.h>
G_BEGIN_DECLS

void
at_graph_component_from_grapharray(AtArray_uint16_t** component_label_ptr, AtGraphArray* grapharray);

#undef at_stack_push

G_END_DECLS
#endif
