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

#ifndef AT_GL_COLORMATERIAL_H
#define AT_GL_COLORMATERIAL_H
#include <at/gl.h>
#include <at/core.h>
G_BEGIN_DECLS

/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define AT_TYPE_GL_COLORMATERIAL at_gl_colormaterial_get_type()
G_DECLARE_DERIVABLE_TYPE(AtGLColorMaterial, at_gl_colormaterial, AT, GL_COLORMATERIAL, AtGLMaterial)

typedef struct _AtGLColorMaterialClass{
  GObjectClass parent_class;
}AtGLColorMaterialClass;

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

/**
 * @brief at_gl_colormaterial_new
 * @return
 */
AtGLColorMaterial*
at_gl_colormaterial_new();

/**
 * @brief at_gl_colormaterial_set_ambient
 * @param colormaterial
 * @param ambient
 */
void
at_gl_colormaterial_set_ambient(AtGLColorMaterial* colormaterial, AtVec3* ambient);

/**
 * @brief at_gl_colormaterial_set_diffuse
 * @param colormaterial
 * @param diffuse
 */
void
at_gl_colormaterial_set_diffuse(AtGLColorMaterial* colormaterial, AtVec3* diffuse);

/**
 * @brief at_gl_colormaterial_set_specular
 * @param colormaterial
 * @param specular
 */
void
at_gl_colormaterial_set_specular(AtGLColorMaterial* colormaterial, AtVec3* specular);

/**
 * @brief at_gl_colormaterial_set_emissive
 * @param colormaterial
 * @param emissive
 */
void
at_gl_colormaterial_set_emissive(AtGLColorMaterial* colormaterial, AtVec3* emissive);

/**
 * @brief at_gl_colormaterial_get_ambient
 * @param colormaterial
 * @return
 */
AtVec3
at_gl_colormaterial_get_ambient(AtGLColorMaterial* colormaterial);

/**
 * @brief at_gl_colormaterial_get_diffuse
 * @param colormaterial
 * @return
 */
AtVec3
at_gl_colormaterial_get_diffuse(AtGLColorMaterial* colormaterial);

/**
 * @brief at_gl_colormaterial_get_specular
 * @param colormaterial
 * @return
 */
AtVec3
at_gl_colormaterial_get_specular(AtGLColorMaterial* colormaterial);

/**
 * @brief at_gl_colormaterial_get_emissive
 * @param colormaterial
 * @return
 */
AtVec3
at_gl_colormaterial_get_emissive(AtGLColorMaterial* colormaterial);

G_END_DECLS
#endif
