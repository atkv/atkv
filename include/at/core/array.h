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

#endif
