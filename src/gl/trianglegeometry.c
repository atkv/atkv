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

#include <at/gl.h>

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
#define GET_PRIV(obj) AtGLTriangleGeometryPrivate* priv = \
                      at_gl_trianglegeometry_get_instance_private(obj);
typedef struct _AtGLTriangleGeometryPrivate{

  AtVec3  * positions;               /**< List of positions of the vertices */
  AtVec3  * normals;                 /**< List of normals of the vertices */
  AtVec3i * indices;                 /**< List of indices of the vertices */

  AtVec3  * positions_pkd;           /**< Packed List of positions of the vertices */
  AtVec3  * normals_pkd;             /**< Packed List of positions of the vertices */

  AtVec2  * uvs;                     /**< List of uvs of the vertices */
  AtVec2  * uvs_pkd;                 /**< Packed List of uvs of the vertices */

  uint32_t* indices_pkd;             /**< Packed List of indices of the vertices */

  uint32_t  num_positions;           /**< Number of positions in unpacked list */
  uint32_t  num_normals;             /**< Number of normals in unpacked list */
  uint32_t  num_uvs;                 /**< Number of uvs in unpacked list */
  uint32_t  num_indices;             /**< Number of indices in unpacked list
                                          and packed ones */
  uint32_t  num_packed;              /**< Number of packed items (positions,
                                          uvs and normals) */

  uint32_t  num_positions_allocated; /**< Number of positions in unpacked list */
  uint32_t  num_normals_allocated;   /**< Number of normals in unpacked list */
  uint32_t  num_uvs_allocated;       /**< Number of uvs in unpacked list */
  uint32_t  num_indices_allocated;   /**< Number of indices in unpacked list
                                         and packed ones */
  uint8_t   padding[4]; // For data alignment

}AtGLTriangleGeometryPrivate;

static void
at_gl_trianglegeometry_geometry_interface_init (AtGLGeometryInterface *iface);

G_DEFINE_TYPE_WITH_PRIVATE_AND_CODE(
  AtGLTriangleGeometry, at_gl_trianglegeometry, G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(
     AT_TYPE_GL_GEOMETRY,
     at_gl_trianglegeometry_geometry_interface_init
  )
)

static void
at_gl_trianglegeometry_geometry_interface_init (AtGLGeometryInterface *iface){

}

static void
at_gl_trianglegeometry_finalize(GObject* object){
  GET_PRIV(AT_GL_TRIANGLEGEOMETRY(object));
  if(priv->indices)       g_free(priv->indices);
  if(priv->indices_pkd)   g_free(priv->indices_pkd);
  if(priv->normals)       g_free(priv->normals);
  if(priv->normals_pkd)   g_free(priv->normals_pkd);
  if(priv->positions)     g_free(priv->positions);
  if(priv->positions_pkd) g_free(priv->positions_pkd);
  if(priv->uvs)           g_free(priv->uvs);
  if(priv->uvs_pkd)       g_free(priv->uvs_pkd);
  G_OBJECT_CLASS(at_gl_trianglegeometry_parent_class)->finalize(object);
}

static void
at_gl_trianglegeometry_class_init(AtGLTriangleGeometryClass *klass){
  GObjectClass * object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = at_gl_trianglegeometry_finalize;
}

static void
at_gl_trianglegeometry_init(AtGLTriangleGeometry *self){
  GET_PRIV(self);
  priv->positions     = NULL;
  priv->normals       = NULL;
  priv->uvs           = NULL;
  priv->indices       = NULL;

  priv->positions_pkd = NULL;
  priv->normals_pkd   = NULL;
  priv->uvs_pkd       = NULL;
  priv->indices_pkd   = NULL;

  priv->num_positions = 0;
  priv->num_normals   = 0;
  priv->num_uvs       = 0;
  priv->num_indices   = 0;
  priv->num_packed    = 0;

  priv->num_positions_allocated = 0;
  priv->num_normals_allocated   = 0;
  priv->num_uvs_allocated       = 0;
  priv->num_indices_allocated   = 0;
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtGLTriangleGeometry*
at_gl_trianglegeometry_new(){
  return g_object_new(AT_TYPE_GL_TRIANGLEGEOMETRY, NULL);
}

// SETTERS
#define SET_ITEM(list, dim) \
  GET_PRIV(geometry); \
  size_t num_##list##_bytes = num_##list * sizeof(AtVec##dim); \
  priv->list = g_realloc(priv->list,num_##list##_bytes); \
  memcpy(priv->list,list,num_##list##_bytes); \
  priv->num_##list = num_##list;

void
at_gl_trianglegeometry_set_positions(AtGLTriangleGeometry* geometry,
                                     AtVec3* positions, uint32_t num_positions){
  SET_ITEM(positions, 3)
}


void
at_gl_trianglegeometry_set_normals(AtGLTriangleGeometry* geometry,
                                   AtVec3* normals, uint32_t num_normals){
  SET_ITEM(normals, 3)
}


void
at_gl_trianglegeometry_set_uvs(AtGLTriangleGeometry* geometry,
                                     AtVec2* uvs, uint32_t num_uvs){
  SET_ITEM(uvs, 2)
}

void
at_gl_trianglegeometry_set_indices(AtGLTriangleGeometry* geometry,
                                   AtVec3i* indices, uint32_t num_indices){
  SET_ITEM(indices, 3i)
}
#undef SET_ITEM

// GETTERS

AtVec3*
at_gl_trianglegeometry_get_positions(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->positions;
}

AtVec3*
at_gl_trianglegeometry_get_normals(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->normals;
}


AtVec2*
at_gl_trianglegeometry_get_uvs(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->uvs;
}

AtVec3i*
at_gl_trianglegeometry_get_indices(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->indices;
}

AtVec3*
at_gl_trianglegeometry_get_positions_pkd(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->positions_pkd;
}

AtVec3*
at_gl_trianglegeometry_get_normals_pkd(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->normals_pkd;
}

AtVec2*
at_gl_trianglegeometry_get_uvs_pkd(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->uvs_pkd;
}

uint32_t*
at_gl_trianglegeometry_get_indices_pkd(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->indices_pkd;
}

uint32_t
at_gl_trianglegeometry_get_num_positions(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->num_positions;
}

uint32_t
at_gl_trianglegeometry_get_num_normals(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->num_normals;
}

uint32_t
at_gl_trianglegeometry_get_num_uvs(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->num_uvs;
}

uint32_t
at_gl_trianglegeometry_get_num_indices(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->num_indices;
}
uint32_t
at_gl_trianglegeometry_get_num_packed(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->num_packed;
}

uint32_t
at_gl_trianglegeometry_get_num_triangles(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);
  return priv->num_indices/3;
}

// ADDING ITEMS
#define ADD_ITEM(item, dim) \
GET_PRIV(geometry); \
if(priv->item##s == NULL){ \
  priv->item##s     = g_malloc(sizeof(AtVec##dim)); \
  priv->num_##item##s_allocated = 1; \
}else if(priv->num_##item##s_allocated == priv->num_##item##s){ \
  priv->num_##item##s_allocated <<= 1; \
  priv->item##s = g_realloc(priv->item##s, priv->num_##item##s_allocated * sizeof(AtVec##dim)); \
}\
priv->item##s[priv->num_##item##s++] = *item;

void
at_gl_trianglegeometry_add_position(AtGLTriangleGeometry* geometry, AtVec3* position){
  ADD_ITEM(position, 3);
}
void
at_gl_trianglegeometry_add_normal(AtGLTriangleGeometry* geometry, AtVec3* normal){
  ADD_ITEM(normal, 3);
}
void
at_gl_trianglegeometry_add_uv(AtGLTriangleGeometry* geometry, AtVec2* uv){
  ADD_ITEM(uv, 2);
}
void
at_gl_trianglegeometry_add_index(AtGLTriangleGeometry* geometry, AtVec3i* indice){
  ADD_ITEM(indice, 3i)
}


#undef ADD_ITEM


void
at_gl_trianglegeometry_pack(AtGLTriangleGeometry* geometry){
  GET_PRIV(geometry);

  // Clear packed lists
  if(priv->indices_pkd)   g_free(priv->indices_pkd);
  if(priv->positions_pkd) g_free(priv->positions_pkd);
  if(priv->normals_pkd)   g_free(priv->normals_pkd);
  if(priv->uvs_pkd)       g_free(priv->uvs_pkd);

  priv->num_packed  = 0;

  // Preallocate packed lists with maximum size (num_indices)
  priv->positions_pkd = g_malloc(sizeof(AtVec3)  *priv->num_indices);
  priv->normals_pkd   = g_malloc(sizeof(AtVec3)  *priv->num_indices);
  priv->uvs_pkd       = g_malloc(sizeof(AtVec2)  *priv->num_indices);
  priv->indices_pkd   = g_malloc(sizeof(uint32_t)*priv->num_indices);

  // Auxiliary structures
  uint64_t step[3]  = {priv->num_normals*priv->num_uvs,priv->num_normals,1};
  GHashTable* table =  g_hash_table_new(g_direct_hash, g_direct_equal);

  int64_t key;
  uint32_t* ind;
  uint32_t i, k;
  for(i = 0; i < priv->num_indices; i++){
    ind = priv->indices[i].data;

    // Calculate key
    key = 0;
    for(k = 0; k < 3; k++)
      key += ind[k] * step[k];

    // If key is already on hash table, get its value (the index)
    gpointer pointer = GINT_TO_POINTER(key);
    if(g_hash_table_contains(table,pointer)){
      priv->indices_pkd[i] = GPOINTER_TO_UINT(g_hash_table_lookup(table,GINT_TO_POINTER(key)));
    }
    // Otherwise, create a new index and fill the lists
    else{
      g_hash_table_insert(table,GINT_TO_POINTER(key),GUINT_TO_POINTER(priv->num_packed));
      if(ind[0] < priv->num_positions)
        priv->positions_pkd[priv->num_packed] = priv->positions[ind[0]];
      if(ind[1] < priv->num_uvs)
        priv->uvs_pkd      [priv->num_packed] = priv->uvs      [ind[1]];
      if(ind[2] < priv->num_normals)
        priv->normals_pkd  [priv->num_packed] = priv->normals  [ind[2]];
      priv->indices_pkd[i] = priv->num_packed++;
    }
  }

  // Realloc packed lists with correct size (num_packed)
  if(priv->num_positions > 0)
    priv->positions_pkd = g_realloc(priv->positions_pkd, sizeof(AtVec3)  *priv->num_packed);
  else{
    g_free(priv->positions_pkd);
    priv->positions_pkd = NULL;
  }
  if(priv->num_normals > 0)
    priv->normals_pkd   = g_realloc(priv->normals_pkd  , sizeof(AtVec3)  *priv->num_packed);
  else{
    g_free(priv->normals_pkd);
    priv->normals_pkd  = NULL;
  }
  if(priv->num_uvs > 0)
    priv->uvs_pkd       = g_realloc(priv->uvs_pkd      , sizeof(AtVec2)  *priv->num_packed);
  else{
    g_free(priv->uvs_pkd);
    priv->uvs_pkd = NULL;
  }

  g_hash_table_destroy(table);
}

#undef GET_PRIV
