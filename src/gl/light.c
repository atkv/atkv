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
#include <string.h>

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/

typedef struct _AtGLLightPrivate{
  AtVec4 ambient_color;
  AtVec4 diffuse_color;
  AtVec4 specular_color;
  double power;
}AtGLLightPrivate;
G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(AtGLLight, at_gl_light, G_TYPE_OBJECT)
static void
at_gl_light_class_init(AtGLLightClass *klass){

}

static void
at_gl_light_init(AtGLLight *self){
  AtGLLightPrivate* priv = at_gl_light_get_instance_private(self);

  memset(priv, 0, sizeof(AtGLLightPrivate));
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

AtVec4
at_gl_light_get_ambient(AtGLLight* light){
  AtGLLightPrivate* priv = at_gl_light_get_instance_private(light);
  return priv->ambient_color;
}

AtVec4
at_gl_light_get_diffuse(AtGLLight* light){
  AtGLLightPrivate* priv = at_gl_light_get_instance_private(light);
  return priv->diffuse_color;
}

AtVec4
at_gl_light_get_specular(AtGLLight* light){
  AtGLLightPrivate* priv = at_gl_light_get_instance_private(light);
  return priv->specular_color;
}

double
at_gl_light_get_power(AtGLLight* light){
  AtGLLightPrivate* priv = at_gl_light_get_instance_private(light);
  return priv->power;
}

void
at_gl_light_set_ambient(AtGLLight* light, AtVec4* ambient){
  AtGLLightPrivate* priv = at_gl_light_get_instance_private(light);
  priv->ambient_color = *ambient;
}

void
at_gl_light_set_diffuse(AtGLLight* light, AtVec4* diffuse){
  AtGLLightPrivate* priv = at_gl_light_get_instance_private(light);
  priv->diffuse_color = *diffuse;
}

void
at_gl_light_set_specular(AtGLLight* light, AtVec4* specular){
  AtGLLightPrivate* priv = at_gl_light_get_instance_private(light);
  priv->specular_color = *specular;
}

void
at_gl_light_set_power(AtGLLight* light, double power){
  AtGLLightPrivate* priv = at_gl_light_get_instance_private(light);
  priv->power = power;
}
