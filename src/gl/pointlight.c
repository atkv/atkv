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
#include <string.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtGLPointLightPrivate{
  AtVec4 position;
}AtGLPointLightPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtGLPointLight, at_gl_pointlight, AT_TYPE_GL_LIGHT)

static void
at_gl_pointlight_class_init(AtGLPointLightClass *klass){

}

static void
at_gl_pointlight_init(AtGLPointLight *self){
  AtGLPointLightPrivate* priv = at_gl_pointlight_get_instance_private(self);
  memset(&priv->position,0,sizeof(priv->position));
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtGLPointLight*
at_gl_pointlight_new(){
  return g_object_new(AT_TYPE_GL_POINTLIGHT, NULL);
}

AtGLPointLight*
at_gl_pointlight_new_with_position(AtVec4* position){
  AtGLPointLight* pointlight = at_gl_pointlight_new();
  AtGLPointLightPrivate* priv = at_gl_pointlight_get_instance_private(pointlight);
  priv->position = *position;
  return pointlight;
}

AtVec4
at_gl_pointlight_get_position(AtGLPointLight* pointlight){
  AtGLPointLightPrivate* priv = at_gl_pointlight_get_instance_private(pointlight);
  return priv->position;
}

void
at_gl_pointlight_set_position(AtGLPointLight* pointlight, AtVec4* position){
  AtGLPointLightPrivate* priv = at_gl_pointlight_get_instance_private(pointlight);
  priv->position = *position;
}
