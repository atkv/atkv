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

#ifndef AT_TYPE_H
#define AT_TYPE_H
typedef enum
{
  AT_UINT8,
  AT_UINT16,
  AT_UINT32,
  AT_UINT64,
  AT_INT8,
  AT_INT16,
  AT_INT32,
  AT_INT64,
  AT_FLOAT,
  AT_DOUBLE,
}AtDataType;
typedef struct _AtArray_uint8_t  AtArray_uint8_t;
typedef struct _AtArray_uint16_t AtArray_uint16_t;
typedef struct _AtArray_uint32_t AtArray_uint32_t;
typedef struct _AtArray_uint64_t AtArray_uint64_t;
typedef struct _AtArray_int8_t   AtArray_int8_t;
typedef struct _AtArray_int16_t  AtArray_int16_t;
typedef struct _AtArray_int32_t  AtArray_int32_t;
typedef struct _AtArray_int64_t  AtArray_int64_t;
typedef struct _AtArray_float    AtArray_float;
typedef struct _AtArray_double   AtArray_double;
#endif
