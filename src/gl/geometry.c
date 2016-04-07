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
typedef struct _AtGLGeometryPrivate{
  double*   vertices_data;
  double*   normals_data;
  double*   uvs_data;
  uint32_t* indices;
}AtGLGeometryPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtGLGeometry, at_gl_geometry, G_TYPE_OBJECT)
static void
at_gl_geometry_class_init(AtGLGeometryClass *klass){

}
static void
at_gl_geometry_init(AtGLGeometry *self){
  AtGLGeometryPrivate* priv = at_gl_geometry_get_instance_private(self);
  priv->indices       = NULL;
  priv->normals_data  = NULL;
  priv->uvs_data      = NULL;
  priv->vertices_data = NULL;
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

AtGLGeometry*
at_gl_geometry_new(){
  return g_object_new(AT_TYPE_GL_GEOMETRY, NULL);
}
