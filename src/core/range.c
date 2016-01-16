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

#include <at/core.h>

AtRange
at_range_from(int64_t from){
  return (AtRange){from,INT64_MAX};
}

AtRange
at_range_to(int64_t to){
  return (AtRange){0,to};
}

AtRange
at_range_from_to(int64_t from, int64_t to){
  return (AtRange){from, to};
}

AtRange
at_range_all(){
  return (AtRange){0, INT64_MAX};
}
