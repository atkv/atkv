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

  // Unpacked data
  AtVec3* positions; /**< List of positions of the vertices */
  AtVec3* normals;   /**< List of normals of the vertices */
  AtVec3* indices;   /**< List of indices of the vertices */

  // Packed data
  AtVec3* positions_pkd; /**< Packed List of positions of the vertices */
  AtVec3* normals_pkd;   /**< Packed List of positions of the vertices */

  // unpacked uv (here for alignment)
  AtVec2* uvs;           /**< List of uvs of the vertices */
  // packed uv (here for alignment)
  AtVec2* uvs_pkd;       /**< Packed List of uvs of the vertices */

  double*  indices_pkd;  /**< Packed List of indices of the vertices */

  uint32_t num_positions; /**< Number of positions in unpacked list */
  uint32_t num_normals;   /**< Number of normals in unpacked list */
  uint32_t num_uvs;       /**< Number of uvs in unpacked list */
  uint32_t num_indices;   /**< Number of indices in unpacked list
                              and packed ones */

  uint32_t num_positions_allocated; /**< Number of positions in unpacked list */
  uint32_t num_normals_allocated;   /**< Number of normals in unpacked list */
  uint32_t num_uvs_allocated;       /**< Number of uvs in unpacked list */
  uint32_t num_indices_allocated;   /**< Number of indices in unpacked list
                                         and packed ones */

}AtGLTriangleGeometryPrivate;


G_DEFINE_TYPE_WITH_PRIVATE(AtGLTriangleGeometry, at_gl_trianglegeometry,
                           AtGLGeometry)
static void
at_gl_trianglegeometry_class_init(AtGLTriangleGeometryClass *klass){

}

static void
at_gl_trianglegeometry_init(AtGLTriangleGeometry *self){
  GET_PRIV(geometry);
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

void
at_gl_trianglegeometry_set_positions(AtGLTriangleGeometry* geometry,
                                     AtVec3* positions, uint32_t num_positions){
  GET_PRIV(geometry);
  priv->positions     = positions;
  priv->num_positions = num_positions;
}


void
at_gl_trianglegeometry_set_normals(AtGLTriangleGeometry* geometry,
                                   AtVec3* normals, uint32_t num_normals){
  GET_PRIV(geometry);
  priv->normals     = normals;
  priv->num_normals = num_normals;
}


void
at_gl_trianglegeometry_set_uvs(AtGLTriangleGeometry* geometry,
                                     AtVec3* uvs, uint32_t num_uvs){
  GET_PRIV(geometry);
  priv->uvs     = uvs;
  priv->num_uvs = num_uvs;
}

#define ADD_ITEM(item, dim) \
GET_PRIV(geometry); \
if(priv->item##s == NULL){ \
  priv->item##s     = malloc(sizeof(AtVec##dim)); \
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
at_gl_trianglegeometry_add_uv(AtGLTriangleGeometry* geometry, AtVec2 uv){
  ADD_ITEM(uv, 2);
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

void
at_gl_trianglegeometry_pack(AtGLTriangleGeometry* geometry){

}
