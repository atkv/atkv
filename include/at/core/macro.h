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

#ifndef AT_MACRO_H
#define AT_MACRO_H
#define VA_NUM_ARGS(...) VA_NUM_ARGS_IMPL(__VA_ARGS__, 6,5,4,3,2,1)
#define VA_NUM_ARGS_IMPL(_1,_2,_3,_4,_5,_6,N,...) N
#define macro_dispatcher(func, ...)     macro_dispatcher_(func, VA_NUM_ARGS(__VA_ARGS__))
#define macro_dispatcher_(func, nargs)  macro_dispatcher__(func, nargs)
#define macro_dispatcher__(func, nargs) func ## nargs
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define AT_DECLARE_BINARY(type, op) \
void \
at_array_##type##_##op##_array_##type##_to_##type (AtArray_##type * array1, \
                                               AtArray_##type * array2, \
                                               AtArray_##type ** output); \
void \
at_array_##type##_##op##_##type##_to_##type (AtArray_##type * array1, \
                                         type scalar, \
                                         AtArray_##type ** output);

#define AT_DECLARE_COPY_TO(type1,type2) \
void \
at_array_##type1##_copy_to_##type2(AtArray_##type1 * array, AtArray_##type2 ** output);

#define AT_DEFINE_COPY_TO(type1,type2) \
void \
at_array_##type1##_copy_to_##type2 (AtArray_##type1 * array, AtArray_##type2 ** output){\
  AtArray_##type1##Private* priv = at_array_##type1##_get_instance_private(array);\
  g_autofree uint64_t*       size = at_array_get_size(array);\
  uint16_t                   dim = at_array_get_dim(array);\
  type2 *                    data = NULL;\
  g_autofree type2 *         data2 = NULL;\
  uint64_t num_elements = at_array_get_num_elements(array);\
  data2 = g_malloc(num_elements * sizeof(type2));\
  uint64_t i;\
  if(at_array_get_contiguous(array)){\
    data = data2;\
    for(i = 0; i < num_elements; i++){\
      data2[i] = priv->data[i];\
    }\
  } else {\
    uint64_t index;\
    data  = data2;\
    g_autofree uint64_t* relative = g_malloc(at_array_get_dim(array)*sizeof(uint64_t));\
    for(i = 0; i < num_elements; i++){\
      index = at_array_base_get_index_absolute(AT_ARRAY_BASE(array),i,relative);\
      data[i] = priv->data[index];\
    }\
  }\
  at_array_##type2##_new_with_data(output,dim,size,data);\
}

#include <glib-object.h>
#if !GLIB_CHECK_VERSION(2,44,0)
  #define G_DECLARE_DERIVABLE_TYPE(ModuleObjName, module_obj_name, MODULE, OBJ_NAME, ParentName) \
  GType module_obj_name##_get_type (void); \
  G_GNUC_BEGIN_IGNORE_DEPRECATIONS \
  typedef struct _##ModuleObjName ModuleObjName; \
  typedef struct _##ModuleObjName##Class ModuleObjName##Class; \
  struct _##ModuleObjName { ParentName parent_instance; }; \
  \
  _GLIB_DEFINE_AUTOPTR_CHAINUP (ModuleObjName, ParentName) \
  \
  static inline ModuleObjName * MODULE##_##OBJ_NAME (gconstpointer ptr) { \
  return G_TYPE_CHECK_INSTANCE_CAST (ptr, module_obj_name##_get_type (), ModuleObjName); } \
  static inline ModuleObjName##Class * MODULE##_##OBJ_NAME##_CLASS (gconstpointer ptr) { \
  return G_TYPE_CHECK_CLASS_CAST (ptr, module_obj_name##_get_type (), ModuleObjName##Class); } \
  static inline gboolean MODULE##_IS_##OBJ_NAME (gconstpointer ptr) { \
  return G_TYPE_CHECK_INSTANCE_TYPE (ptr, module_obj_name##_get_type ()); } \
  static inline gboolean MODULE##_IS_##OBJ_NAME##_CLASS (gconstpointer ptr) { \
  return G_TYPE_CHECK_CLASS_TYPE (ptr, module_obj_name##_get_type ()); } \
  static inline ModuleObjName##Class * MODULE##_##OBJ_NAME##_GET_CLASS (gconstpointer ptr) { \
  return G_TYPE_INSTANCE_GET_CLASS (ptr, module_obj_name##_get_type (), ModuleObjName##Class); } \
  G_GNUC_END_IGNORE_DEPRECATIONS
#endif

#endif
