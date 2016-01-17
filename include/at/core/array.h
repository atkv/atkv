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

#ifndef AT_CORE_ARRAY_H
#define AT_CORE_ARRAY_H
#include <at/core.h>
/*===========================================================================
 * MACROS, ENUMS AND TYPEDEFS
 *===========================================================================*/
#define AtArray(T)      AtArray_##T
#define AtMapFunc(T)    AtMapFunc_##T
#define AtReduceFunc(T) AtReduceFunc_##T
/*===========================================================================
 * FUNCTIONS
 *===========================================================================*/

/* Dispatchers
 * ----------------------------------
 * These macros count the number of
 * arguments and call appropriate
 * variation (below)
 *-----------------------------------*/
#define at_array_new(...)              macro_dispatcher(at_array_new,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_append(...)           macro_dispatcher(at_array_append,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_is_empty(...)         macro_dispatcher(at_array_is_empty,         __VA_ARGS__)(__VA_ARGS__)
#define at_array_get_dim(...)          macro_dispatcher(at_array_get_dim,          __VA_ARGS__)(__VA_ARGS__)
#define at_array_get_num_elements(...) macro_dispatcher(at_array_get_num_elements, __VA_ARGS__)(__VA_ARGS__)
#define at_array_get_elemsize(...)     macro_dispatcher(at_array_get_elemsize,     __VA_ARGS__)(__VA_ARGS__)
#define at_array_get_size(...)         macro_dispatcher(at_array_get_size,         __VA_ARGS__)(__VA_ARGS__)
#define at_array_get_step(...)         macro_dispatcher(at_array_get_step,         __VA_ARGS__)(__VA_ARGS__)
#define at_array_get_num_bytes(...)    macro_dispatcher(at_array_get_num_bytes,    __VA_ARGS__)(__VA_ARGS__)
#define at_array_get_contiguous(...)   macro_dispatcher(at_array_get_contiguous,   __VA_ARGS__)(__VA_ARGS__)
#define at_array_get_index(...)        macro_dispatcher(at_array_get_index,        __VA_ARGS__)(__VA_ARGS__)
#define at_array_get_parent(...)       macro_dispatcher(at_array_get_parent,       __VA_ARGS__)(__VA_ARGS__)
#define at_array_get(...)              macro_dispatcher(at_array_get,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_add(...)              macro_dispatcher(at_array_add,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_subtract(...)         macro_dispatcher(at_array_subtract,         __VA_ARGS__)(__VA_ARGS__)
#define at_array_mult(...)             macro_dispatcher(at_array_mult,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_divide(...)           macro_dispatcher(at_array_divide,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_mod(...)              macro_dispatcher(at_array_mod,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_set(...)              macro_dispatcher(at_array_set,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_copy(...)             macro_dispatcher(at_array_copy,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_sub(...)              macro_dispatcher(at_array_sub,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_map(...)              macro_dispatcher(at_array_map,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_reduce(...)           macro_dispatcher(at_array_reduce,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_zeros(...)            macro_dispatcher(at_array_zeros,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_ones(...)             macro_dispatcher(at_array_ones,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_fill(...)             macro_dispatcher(at_array_fill,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_range(...)            macro_dispatcher(at_array_range,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_sort(...)             macro_dispatcher(at_array_sort,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_argsort(...)          macro_dispatcher(at_array_argsort,          __VA_ARGS__)(__VA_ARGS__)
#define at_array_argmin(...)           macro_dispatcher(at_array_argmin,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_argmax(...)           macro_dispatcher(at_array_argmax,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_max(...)              macro_dispatcher(at_array_max,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_min(...)              macro_dispatcher(at_array_min,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_std(...)              macro_dispatcher(at_array_std,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_prod(...)             macro_dispatcher(at_array_prod,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_var(...)              macro_dispatcher(at_array_var,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_avg(...)              macro_dispatcher(at_array_avg,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_clip(...)             macro_dispatcher(at_array_clip,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_cumsum(...)           macro_dispatcher(at_array_cumsum,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_join(...)             macro_dispatcher(at_array_join,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_convolve(...)         macro_dispatcher(at_array_convolve,         __VA_ARGS__)(__VA_ARGS__)
#define at_array_fftconvolve(...)      macro_dispatcher(at_array_fftconvolve,      __VA_ARGS__)(__VA_ARGS__)
#define at_array_squeeze(...)          macro_dispatcher(at_array_squeeze,          __VA_ARGS__)(__VA_ARGS__)
#define at_array_filter(...)           macro_dispatcher(at_array_filter,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_read(...)             macro_dispatcher(at_array_read,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_write(...)            macro_dispatcher(at_array_write,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_any(...)              macro_dispatcher(at_array_any,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_all(...)              macro_dispatcher(at_array_all,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_equal(...)            macro_dispatcher(at_array_equal,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_not_equal(...)        macro_dispatcher(at_array_not_equal,        __VA_ARGS__)(__VA_ARGS__)
#define at_array_greater(...)          macro_dispatcher(at_array_greater,          __VA_ARGS__)(__VA_ARGS__)
#define at_array_less(...)             macro_dispatcher(at_array_less,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_greater_equal(...)    macro_dispatcher(at_array_greater_equal,    __VA_ARGS__)(__VA_ARGS__)
#define at_array_less_equal(...)       macro_dispatcher(at_array_less_equal,       __VA_ARGS__)(__VA_ARGS__)
#define at_array_sin(...)              macro_dispatcher(at_array_sin,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_cos(...)              macro_dispatcher(at_array_cos,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_tan(...)              macro_dispatcher(at_array_tan,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_arcsin(...)           macro_dispatcher(at_array_arcsin,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_arccos(...)           macro_dispatcher(at_array_arccos,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_arctan(...)           macro_dispatcher(at_array_arctan,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_degrees(...)          macro_dispatcher(at_array_degrees,          __VA_ARGS__)(__VA_ARGS__)
#define at_array_radians(...)          macro_dispatcher(at_array_radians,          __VA_ARGS__)(__VA_ARGS__)
#define at_array_hypot(...)            macro_dispatcher(at_array_hypot,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_sinh(...)             macro_dispatcher(at_array_sinh,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_cosh(...)             macro_dispatcher(at_array_cosh,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_tanh(...)             macro_dispatcher(at_array_tanh,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_arcsinh(...)          macro_dispatcher(at_array_arcsinh,          __VA_ARGS__)(__VA_ARGS__)
#define at_array_arccosh(...)          macro_dispatcher(at_array_arccosh,          __VA_ARGS__)(__VA_ARGS__)
#define at_array_arctanh(...)          macro_dispatcher(at_array_arctanh,          __VA_ARGS__)(__VA_ARGS__)
#define at_array_floor(...)            macro_dispatcher(at_array_floor,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_ceil(...)             macro_dispatcher(at_array_ceil,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_trunc(...)            macro_dispatcher(at_array_trunc,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_exp(...)              macro_dispatcher(at_array_exp,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_expm1(...)            macro_dispatcher(at_array_expm1,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_exp2(...)             macro_dispatcher(at_array_exp2,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_log(...)              macro_dispatcher(at_array_log,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_log10(...)            macro_dispatcher(at_array_log10,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_log2(...)             macro_dispatcher(at_array_log2,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_sinc(...)             macro_dispatcher(at_array_sinc,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_recriprocal(...)      macro_dispatcher(at_array_recriprocal,      __VA_ARGS__)(__VA_ARGS__)
#define at_array_negative(...)         macro_dispatcher(at_array_negative,         __VA_ARGS__)(__VA_ARGS__)
#define at_array_power(...)            macro_dispatcher(at_array_power,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_rem(...)              macro_dispatcher(at_array_rem,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_mod(...)              macro_dispatcher(at_array_mod,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_frac(...)             macro_dispatcher(at_array_frac,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_abs(...)              macro_dispatcher(at_array_abs,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_sign(...)             macro_dispatcher(at_array_sign,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_angle(...)            macro_dispatcher(at_array_angle,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_real(...)             macro_dispatcher(at_array_real,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_imag(...)             macro_dispatcher(at_array_imag,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_conj(...)             macro_dispatcher(at_array_conj,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_signbit(...)          macro_dispatcher(at_array_signbit,          __VA_ARGS__)(__VA_ARGS__)
#define at_array_eye(...)              macro_dispatcher(at_array_eye,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_linspace(...)         macro_dispatcher(at_array_linspace,         __VA_ARGS__)(__VA_ARGS__)
#define at_array_meshgrid(...)         macro_dispatcher(at_array_meshgrid,         __VA_ARGS__)(__VA_ARGS__)
#define at_array_tri(...)              macro_dispatcher(at_array_tri,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_diag(...)             macro_dispatcher(at_array_diag,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_ravel(...)            macro_dispatcher(at_array_ravel,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_unravel(...)          macro_dispatcher(at_array_unravel,          __VA_ARGS__)(__VA_ARGS__)
#define at_array_swapaxes(...)         macro_dispatcher(at_array_swapaxes,         __VA_ARGS__)(__VA_ARGS__)
#define at_array_ravel(...)            macro_dispatcher(at_array_ravel,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_transpose(...)        macro_dispatcher(at_array_transpose,        __VA_ARGS__)(__VA_ARGS__)
#define at_array_rollaxis(...)         macro_dispatcher(at_array_rollaxis,         __VA_ARGS__)(__VA_ARGS__)
#define at_array_expand_dims(...)      macro_dispatcher(at_array_expand_dims,      __VA_ARGS__)(__VA_ARGS__)
#define at_array_as_contiguous(...)    macro_dispatcher(at_array_as_contiguous,    __VA_ARGS__)(__VA_ARGS__)
#define at_array_delete(...)           macro_dispatcher(at_array_delete,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_insert(...)           macro_dispatcher(at_array_insert,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_append(...)           macro_dispatcher(at_array_append,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_resize(...)           macro_dispatcher(at_array_resize,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_unique(...)           macro_dispatcher(at_array_unique,           __VA_ARGS__)(__VA_ARGS__)
#define at_array_flip(...)             macro_dispatcher(at_array_flip,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_roll(...)             macro_dispatcher(at_array_roll,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_bitwise_and(...)      macro_dispatcher(at_array_bitwise_and,      __VA_ARGS__)(__VA_ARGS__)
#define at_array_bitwise_or(...)       macro_dispatcher(at_array_bitwise_or,       __VA_ARGS__)(__VA_ARGS__)
#define at_array_bitwise_xor(...)      macro_dispatcher(at_array_bitwise_xor,      __VA_ARGS__)(__VA_ARGS__)
#define at_array_bitwise_not(...)      macro_dispatcher(at_array_bitwise_not,      __VA_ARGS__)(__VA_ARGS__)
#define at_array_left_shift(...)       macro_dispatcher(at_array_left_shift,       __VA_ARGS__)(__VA_ARGS__)
#define at_array_right_shift(...)      macro_dispatcher(at_array_right_shift,      __VA_ARGS__)(__VA_ARGS__)
#define at_array_packbits(...)         macro_dispatcher(at_array_packbits,         __VA_ARGS__)(__VA_ARGS__)
#define at_array_unpackbits(...)       macro_dispatcher(at_array_unpackbits,       __VA_ARGS__)(__VA_ARGS__)
#define at_array_dot(...)              macro_dispatcher(at_array_dot,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_kron(...)             macro_dispatcher(at_array_kron,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_cholesky(...)         macro_dispatcher(at_array_cholesky,         __VA_ARGS__)(__VA_ARGS__)
#define at_array_qr(...)               macro_dispatcher(at_array_qr,               __VA_ARGS__)(__VA_ARGS__)
#define at_array_svd(...)              macro_dispatcher(at_array_svd,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_eig(...)              macro_dispatcher(at_array_eig,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_norm(...)             macro_dispatcher(at_array_norm,             __VA_ARGS__)(__VA_ARGS__)
#define at_array_det(...)              macro_dispatcher(at_array_det,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_solve(...)            macro_dispatcher(at_array_solve,            __VA_ARGS__)(__VA_ARGS__)
#define at_array_inv(...)              macro_dispatcher(at_array_inv,              __VA_ARGS__)(__VA_ARGS__)
#define at_array_least_square(...)     macro_dispatcher(at_array_least_square,     __VA_ARGS__)(__VA_ARGS__)
#define at_array_alloc_data(...)       macro_dispatcher(at_array_alloc_data,     __VA_ARGS__)(__VA_ARGS__)
#define at_array_dealloc_data(...)     macro_dispatcher(at_array_dealloc_data,     __VA_ARGS__)(__VA_ARGS__)

/* Variations
 * ----------------------------------
 * The number at suffix of the macro
 * represents the number of parameters
 * (function overloading by number of
 * parameters).
 * The _Generic operator provides the
 * function overloading by type.
 * We can simulate optional arguments
 * by these techniques
 *-----------------------------------*/
#define SEVERAL_OP1I(OP,x) \
  OP(uint8_t ,x), OP(uint16_t,x), OP(uint32_t,x), OP(uint64_t,x),\
  OP(int8_t  ,x), OP(int16_t ,x), OP(int32_t ,x), OP(int64_t ,x)
#define SEVERAL_OP1F(OP,x) OP(float   ,x), OP(double  ,x)
#define SEVERAL_OP1(OP,x) SEVERAL_OP1I(OP,x), SEVERAL_OP1F(OP,x)

#define SEVERAL_OP1R(OP,x) \
  OP(uint8_t ,x), OP(uint16_t,x), OP(uint32_t,x), OP(uint64_t,x),\
  OP(int8_t  ,x), OP(int16_t ,x), OP(int32_t ,x), OP(int64_t ,x),\
  OP(float   ,x), OP(double  ,x)
#define SEVERAL_OP2(OP,x,y) \
  OP(uint8_t ,x,y), OP(uint16_t,x,y), OP(uint32_t,x,y), OP(uint64_t,x,y),\
  OP(int8_t  ,x,y), OP(int16_t ,x,y), OP(int32_t ,x,y), OP(int64_t ,x,y),\
  OP(float   ,x,y), OP(double  ,x,y)
#define GENERIC_SEVERAL_OP1(item, OP, x)   _Generic((item), SEVERAL_OP1(OP, x))
#define GENERIC_SEVERAL_OP1I(item, OP, x)  _Generic((item), SEVERAL_OP1I(OP, x))
#define GENERIC_SEVERAL_OP2(item, OP, x,y) _Generic((item), SEVERAL_OP2(OP, x,y))
#define GENERIC_SEVERAL_OP1R(item, OP, x)  _Generic((item), SEVERAL_OP1R(OP, x))


#define EMPTY(...)
#define DEFER(...) __VA_ARGS__ EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
#define EXPAND(...) __VA_ARGS__

#define LINE_OP(type, op)   AtArray_##type **: at_array_##type##_##op
#define LINE_OP2(type, op)   AtArray_##type *: at_array_##type##_##op
#define LINE_NEW3(type, x, y) AtArray_##type **: _Generic((y),\
  int: at_array_##type##_##x##_2D,\
  uint64_t*: at_array_##type##_##x##_with_size\
)
#define LINE_NEW4(type, x, y) AtArray_##type **: _Generic((y),\
  int: at_array_##type##_##x##_3D,\
  uint64_t*: at_array_##type##_##x##_with_data\
)
#define LINE_NEW42(type, x, y) AtArray_##type **: _Generic((y),\
  int: at_array_##type##_##x##_3D\
)
#define LINE_INDEX2(type, x) AtArray_##type *: _Generic((x),\
  uint64_t*: at_array_##type##_get_index,\
  uint64_t:  at_array_##type##_get_indices\
)
#define LINE_MOD(type, input) AtArray_##type *: _Generic((input), uint64_t*: at_array_##type##_mod_indices)
#define LINE_SET(type, x) AtArray_##type *: _Generic((x),\
  uint64_t*: at_array_##type##_set_by_indices,\
  uint64_t:  at_array_##type##_set_by_index\
)
#define LINE_SET2(type,input) AtArray_##type*: _Generic((input),\
  AtArray_##type*: at_array_##type##_set_array_##type##_to_##type,\
  default: at_array_##type##_set_##type##_to_##type\
)
#define LINE_GET(type,x) AtArray_##type*: _Generic((x),\
  uint64_t*: at_array_##type##_get_by_indices,\
  uint64_t:  at_array_##type##_get_by_index,\
  int:       at_array_##type##_get_by_index\
)
#define LINE_COPY2(type1,type2) AtArray_##type1 *: at_array_##type1##_copy_to_##type2
#define LINE_COPY(type2,array) AtArray_##type2 **: GENERIC_SEVERAL_OP1R(array,LINE_COPY2,type2)
#define LINE_OP22(type, input, op) AtArray_##type *: _Generic((input),\
  AtArray_##type*: at_array_##type##_##op##_array_##type##_to_##type,\
  default: at_array_##type##_##op##_##type##_to_##type\
)
#define LINE_OP23(input1, input2, op, output) _Generic((input1),\
  AtArray_uint8_t*: _Generic((input2),\
    AtArray_uint8_t*: _Generic((output),\
      AtArray_uint8_t**: at_array_uint8_t_##op##_array_uint8_t_to_uint8_t\
    ),\
    uint8_t: _Generic((output),\
      AtArray_uint8_t**: at_array_uint8_t_##op##_uint8_t_to_uint8_t\
    ),\
    default: _Generic((output),\
      AtArray_uint8_t**: at_array_uint8_t_##op##_uint8_t_to_uint8_t\
    )\
  )\
)(input1, input2, output)
#define LINE_BITWISE3(input1, input2, output, op) _Generic((input1),\
  AtArray_uint8_t*: _Generic((input2),\
    AtArray_uint8_t*: _Generic((output),\
      AtArray_uint8_t**: at_array_uint8_t_bitwise_##op##_array_uint8_t_to_uint8_t\
    ),\
    uint8_t: _Generic((output),\
      AtArray_uint8_t**: at_array_uint8_t_bitwise_##op##_uint8_t_to_uint8_t\
    ),\
    default: _Generic((output),\
      AtArray_uint8_t**: at_array_uint8_t_bitwise_##op##_uint8_t_to_uint8_t\
    )\
  )\
)(input1, input2, output)
#define LINE_GET_DATA(type, array) AtArray_##type* : at_array_##type##_get_data
#define LINE_ALLOC_DATA(type, array) AtArray_##type* : at_array_##type##_alloc_data
#define LINE_DEALLOC_DATA(type, array) AtArray_##type* : at_array_##type##_dealloc_data

#define at_array_new1(output)                             GENERIC_SEVERAL_OP1(output,LINE_OP,  new)   (output)
#define at_array_new2(output,size1)                       GENERIC_SEVERAL_OP1(output,LINE_OP,  new_1D)(output,size1)
#define at_array_new3(output,x, y)                        GENERIC_SEVERAL_OP2(output,LINE_NEW3,new,y) (output, x, y)
#define at_array_new4(output,x, y, z)                     GENERIC_SEVERAL_OP2(output,LINE_NEW4,new,y) (output, x, y, z)
#define at_array_new5(output,x, y, z, k)                  GENERIC_SEVERAL_OP1(output,LINE_OP,  new_4D)(output, x, y, z, k)
#define at_array_get_dim1(array)                          at_array_base_get_dim(AT_ARRAY_BASE(array))
#define at_array_get_num_elements1(array)                 at_array_base_get_num_elements(AT_ARRAY_BASE(array))
#define at_array_get_elemsize1(array)                     at_array_base_get_elemsize(AT_ARRAY_BASE(array))
#define at_array_get_num_bytes1(array)                    at_array_base_get_num_bytes(AT_ARRAY_BASE(array))
#define at_array_get_contiguous1(array)                   at_array_base_get_contiguous(AT_ARRAY_BASE(array))
#define at_array_get_size1(array)                         at_array_base_get_size(AT_ARRAY_BASE(array))
#define at_array_get_size2(array,x)                       at_array_base_get_size_at(AT_ARRAY_BASE(array),x)
#define at_array_get_step1(array)                         at_array_base_get_step(AT_ARRAY_BASE(array))
#define at_array_get_step2(array,x)                       at_array_base_get_step_at(AT_ARRAY_BASE(array),x)
#define at_array_is_empty1(array)                         GENERIC_SEVERAL_OP1(array, LINE_OP2, is_empty)(array)
#define at_array_get_index3(array, i1, i2)                GENERIC_SEVERAL_OP1(array, LINE_INDEX2,i1)(array, i1, i2)
#define at_array_mod3(array, input, output)               GENERIC_SEVERAL_OP1(array, LINE_MOD,input)(array, input, output)
#define at_array_set3(array, x, y)                        GENERIC_SEVERAL_OP1(array, LINE_SET,x)(array, x, y)
#define at_array_get1(array)                              GENERIC_SEVERAL_OP1(array, LINE_GET_DATA, array)(array)
#define at_array_get2(array, x)                           GENERIC_SEVERAL_OP1(array, LINE_GET, x)(array,x)
#define at_array_copy2(array, output)                     GENERIC_SEVERAL_OP1(output,LINE_COPY,array)(array,output)
#define at_array_add2(array,input)                        GENERIC_SEVERAL_OP2(array, LINE_OP22,input,add) (array,input,&array)
#define at_array_subtract2(array,input)                   GENERIC_SEVERAL_OP2(array, LINE_OP22,input,subtract) (array,input,&array)
#define at_array_mult2(array,input)                       GENERIC_SEVERAL_OP2(array, LINE_OP22,input,mult) (array,input,&array)
#define at_array_divide2(array,input)                     GENERIC_SEVERAL_OP2(array, LINE_OP22,input,divide) (array,input,&array)
#define at_array_add3(input1, input2, output)             LINE_OP23(input1, input2, add, output)
#define at_array_subtract3(input1, input2, output)        LINE_OP23(input1, input2, subtract, output)
#define at_array_mult3(input1, input2, output)            LINE_OP23(input1, input2, mult, output)
#define at_array_divide3(input1, input2, output)          LINE_OP23(input1, input2, divide, output)
#define at_array_set2(array, input)                       GENERIC_SEVERAL_OP1(array, LINE_SET2,input)(array,input,&array)
#define at_array_sub3(array, ranges, output)              GENERIC_SEVERAL_OP1(array, LINE_OP2, sub) (array, ranges, output)
#define at_array_get_parent1(array)                       at_array_base_get_parent(AT_ARRAY_BASE(array))
#define at_array_squeeze1(array)                          GENERIC_SEVERAL_OP1(array, LINE_OP2,squeeze)(array)
#define at_array_map2(array, func)                        GENERIC_SEVERAL_OP1(array, LINE_OP2,map_inplace)(array,func)
#define at_array_map3(array, func, output)                GENERIC_SEVERAL_OP1(array, LINE_OP2,map)(array,func,output)
#define at_array_reduce5(array, func, axis, output,value) GENERIC_SEVERAL_OP1(array, LINE_OP2,reduce_1)(array,func,axis,output,value)
#define at_array_reduce6(array, func, num_axes, axes, output,value) GENERIC_SEVERAL_OP1(array,LINE_OP2,reduce_N)(array,func,num_axes,axes, output,value)
#define at_array_zeros2(output,size1)                     GENERIC_SEVERAL_OP1(output, LINE_OP,zeros_1D)(output,size1)
#define at_array_zeros3(output,x, y)                      GENERIC_SEVERAL_OP2(output, LINE_NEW3,zeros,y)(output,x,y)
#define at_array_zeros4(output,x,y,z)                     GENERIC_SEVERAL_OP2(output, LINE_NEW42,zeros,y)(output,x,y,z)
#define at_array_zeros5(output,x,y,z,k)                   GENERIC_SEVERAL_OP1(output, LINE_OP, zeros_4D)(output,x,y,z,k)
#define at_array_ones2(output,size1)                      GENERIC_SEVERAL_OP1(output, LINE_OP,    ones_1D)(output,size1)
#define at_array_ones3(output,x, y)                       GENERIC_SEVERAL_OP2(output, LINE_NEW3,  ones,y)(output,x,y)
#define at_array_ones4(output,x,y,z)                      GENERIC_SEVERAL_OP2(output, LINE_NEW42, ones,y)(output,x,y,z)
#define at_array_ones5(output,x,y,z,k)                    GENERIC_SEVERAL_OP1(output, LINE_OP, ones_4D)(output,x,y,z,k)
#define at_array_max2(array, output)                      GENERIC_SEVERAL_OP1(array,  LINE_OP2, max)(array,output)
#define at_array_max3(array, axis, output)                GENERIC_SEVERAL_OP1(array,  LINE_OP2,max_axis)(array, axis, output)
#define at_array_max4(array, num_axes, axes, output)      GENERIC_SEVERAL_OP1(array,  LINE_OP2,max_axes)(array, num_axes, axes, output)
#define at_array_min2(array, output)                      GENERIC_SEVERAL_OP1(array,  LINE_OP2, min)(array,output)
#define at_array_min3(array, axis, output)                GENERIC_SEVERAL_OP1(array,  LINE_OP2, min_axis)(array, axis, output)
#define at_array_min4(array, num_axes, axes, output)      GENERIC_SEVERAL_OP1(array,  LINE_OP2, min_axes)(array, num_axes, axes, output)
#define at_array_prod2(array, output)                     GENERIC_SEVERAL_OP1(array,  LINE_OP2, prod)(array,output)
#define at_array_prod3(array, axis, output)               GENERIC_SEVERAL_OP1(array,  LINE_OP2, prod_axis)(array, axis, output)
#define at_array_prod4(array, num_axes, axes, output)     GENERIC_SEVERAL_OP1(array,  LINE_OP2, prod_axes)(array, num_axes, axes, output)
#define at_array_range2(output, to)                       GENERIC_SEVERAL_OP1(output, LINE_OP, range_to)(output, to)
#define at_array_range3(output, x, y)                     GENERIC_SEVERAL_OP1(output, LINE_OP, range_from_to)(output, x,y)
#define at_array_range4(output, x, y, z)                  GENERIC_SEVERAL_OP1(output, LINE_OP, range_from_to_step)(output, x,y,z)
#define at_array_bitwise_and3(input1, input2, output)     LINE_BITWISE3(input1, input2, output, and)
#define at_array_bitwise_or3(input1, input2, output)      LINE_BITWISE3(input1, input2, output, or)
#define at_array_bitwise_xor3(input1, input2, output)     LINE_BITWISE3(input1, input2, output, xor)
#define at_array_bitwise_not2(array1, output)             GENERIC_SEVERAL_OP1I(array1,LINE_OP2,bitwise_not)(array1, output)
#define at_array_alloc_data3(array, dim, size)            GENERIC_SEVERAL_OP1(array, LINE_ALLOC_DATA, array)(array, dim, size)
#define at_array_dealloc_data1(array)                     GENERIC_SEVERAL_OP1(array, LINE_DEALLOC_DATA, array)(array)
#endif
