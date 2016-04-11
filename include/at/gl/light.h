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

#ifndef AT_GL_LIGHT_H
#define AT_GL_LIGHT_H

#include <at/gl.h>
G_BEGIN_DECLS
/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/

#define AT_TYPE_GL_LIGHT at_gl_light_get_type()
G_DECLARE_DERIVABLE_TYPE(AtGLLight, at_gl_light, AT, GL_LIGHT, GObject)
typedef struct _AtGLLightClass{
  GObjectClass parent_class;
}AtGLLightClass;

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

AtVec4
at_gl_light_get_ambient(AtGLLight* light);

AtVec4
at_gl_light_get_diffuse(AtGLLight* light);

AtVec4
at_gl_light_get_specular(AtGLLight* light);

double
at_gl_light_get_power(AtGLLight* light);

void
at_gl_light_set_ambient(AtGLLight* light, AtVec4* ambient);

void
at_gl_light_set_diffuse(AtGLLight* light, AtVec4* diffuse);

void
at_gl_light_set_specular(AtGLLight* light, AtVec4* specular);

void
at_gl_light_set_power(AtGLLight* light, double power);

G_END_DECLS
#endif
