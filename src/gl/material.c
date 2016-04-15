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
typedef struct _AtGLMaterialPrivate{
  char* name;
}AtGLMaterialPrivate;

#define GET_PRIV(material) AtGLMaterialPrivate* priv = \
                           at_gl_material_get_instance_private(material);
G_DEFINE_TYPE_WITH_PRIVATE(AtGLMaterial, at_gl_material, G_TYPE_OBJECT)

static void
at_gl_material_finalize(GObject* object){
  GET_PRIV(AT_GL_MATERIAL(object));
  if(priv->name) g_free(priv->name);
  G_OBJECT_CLASS(at_gl_material_parent_class)->finalize(object);
}

static void
at_gl_material_class_init(AtGLMaterialClass *klass){
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = at_gl_material_finalize;
}
static void
at_gl_material_init(AtGLMaterial *self){
  GET_PRIV(self)
  priv->name       = NULL;
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

AtGLMaterial*
at_gl_material_new(){
  return g_object_new(AT_TYPE_GL_MATERIAL, NULL);
}

char*
at_gl_material_get_name(AtGLMaterial* material){
  GET_PRIV(material);
  return priv->name;
}

void
at_gl_material_set_name(AtGLMaterial* material, char* name){
  GET_PRIV(material);
  priv->name = g_strdup(name);
}
