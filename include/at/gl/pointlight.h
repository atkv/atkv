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

#ifndef AT_GL_POINTLIGHT_H
#define AT_GL_POINTLIGHT_H
#include <at/gl.h>
#include <at/core.h>

/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define AT_TYPE_GL_POINTLIGHT at_gl_pointlight_get_type()
G_DECLARE_DERIVABLE_TYPE(AtGLPointLight, at_gl_pointlight, AT, GL_POINTLIGHT, AtGLLight)
typedef struct _AtGLPointLightClass{
  AtGLLightClass parent_class;
}AtGLPointLightClass;

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
/**
 * @brief at_gl_pointlight_new
 * @return
 */
AtGLPointLight*
at_gl_pointlight_new();

/**
 * @brief at_gl_pointlight_new_with_position
 * @param position
 * @return
 */
AtGLPointLight*
at_gl_pointlight_new_with_position(AtVec4* position);

/**
 * @brief at_gl_pointlight_get_position
 * @param pointlight
 * @return
 */
AtVec4
at_gl_pointlight_get_position(AtGLPointLight* pointlight);

/**
 * @brief at_gl_pointlight_set_position
 * @param pointlight
 * @param position
 */
void
at_gl_pointlight_set_position(AtGLPointLight* pointlight, AtVec4* position);

#endif
