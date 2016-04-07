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
#include <at/core.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtGLMeshPrivate{
  AtGLGeometry* geometry;
  AtGLMaterial* material;
}AtGLMeshPrivate;

static void
at_gl_mesh_object_interface_init(AtGLObject *iface);

G_DEFINE_TYPE_WITH_PRIVATE_AND_CODE(AtGLMesh, at_gl_mesh, G_TYPE_OBJECT,G_IMPLEMENT_INTERFACE(AT_TYPE_GL_OBJECT,at_gl_mesh_object_interface_init))
static void
at_gl_mesh_class_init(AtGLMeshClass *klass){

}
static void
at_gl_mesh_init(AtGLMesh *self){
  AtGLMeshPrivate* priv = at_gl_mesh_get_instance_private(self);
  priv->geometry = NULL;
  priv->material = NULL;
}
static void
at_gl_mesh_object_interface_init(AtGLObject *iface){

}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtGLMesh*
at_gl_mesh_new(){
  return g_object_new(AT_TYPE_GL_MESH, NULL);
}

AtGLMesh*
at_gl_mesh_new_from_file(char* file){
  AtGLMesh* mesh = at_gl_mesh_new();
  return mesh;
}
