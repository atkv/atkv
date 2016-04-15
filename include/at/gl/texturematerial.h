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

#ifndef AT_GL_TEXTUREMATERIAL_H
#define AT_GL_TEXTUREMATERIAL_H
#include <at/gl.h>
#include <string.h>

G_BEGIN_DECLS
/*===========================================================================
 * CLASS STRUCTURE
 *===========================================================================*/
#define AT_TYPE_GL_TEXTUREMATERIAL at_gl_texturematerial_get_type()
G_DECLARE_DERIVABLE_TYPE(AtGLTextureMaterial, at_gl_texturematerial,
                         AT, GL_TEXTUREMATERIAL,AtGLMaterial)
typedef struct _AtGLTextureMaterialClass{
  AtGLMaterialClass parent_class;
}AtGLTextureMaterialClass;

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

/**
 * @brief at_gl_texturematerial_new
 * @return
 */
AtGLTextureMaterial*
at_gl_texturematerial_new();

/**
 * @brief at_gl_texturematerial_set_ambient
 * @param texturematerial
 * @param ambient_map
 */
void
at_gl_texturematerial_set_ambient(AtGLTextureMaterial* texturematerial,
                                  char* ambient_map);

/**
 * @brief at_gl_texturematerial_set_diffuse
 * @param texturematerial
 * @param diffuse_map
 */
void
at_gl_texturematerial_set_diffuse(AtGLTextureMaterial* texturematerial,
                                  char* diffuse_map);

/**
 * @brief at_gl_texturematerial_set_specular
 * @param texturematerial
 * @param specular_map
 */
void
at_gl_texturematerial_set_specular(AtGLTextureMaterial* texturematerial,
                                   char* specular_map);

/**
 * @brief at_gl_texturematerial_set_emissive
 * @param texturematerial
 * @param emissive_map
 */
void
at_gl_texturematerial_set_emissive(AtGLTextureMaterial* texturematerial,
                                   char* emissive_map);

/**
 * @brief at_gl_texturematerial_set_normal
 * @param texturematerial
 * @param normal_map
 */
void
at_gl_texturematerial_set_normal(AtGLTextureMaterial* texturematerial,
                                   char* normal_map);

/**
 * @brief at_gl_texturematerial_set_bump
 * @param texturematerial
 * @param bump_map
 */
void
at_gl_texturematerial_set_bump(AtGLTextureMaterial* texturematerial,
                                   char* bump_map);

/**
 * @brief at_gl_texturematerial_set_displacement
 * @param texturematerial
 * @param disp_map
 */
void
at_gl_texturematerial_set_displacement(AtGLTextureMaterial* texturematerial,
                                   char* disp_map);
/**
 * @brief at_gl_texturematerial_set_alpha
 * @param texturematerial
 * @param alpha_map
 */
void
at_gl_texturematerial_set_alpha(AtGLTextureMaterial* texturematerial,
                                char* alpha_map);

/**
 * @brief at_gl_texturematerial_get_ambient
 * @param texturematerial
 * @return
 */
char*
at_gl_texturematerial_get_ambient(AtGLTextureMaterial* texturematerial);

/**
 * @brief at_gl_texturematerial_get_diffuse
 * @param texturematerial
 * @return
 */
char*
at_gl_texturematerial_get_diffuse(AtGLTextureMaterial* texturematerial);

/**
 * @brief at_gl_texturematerial_get_specular
 * @param texturematerial
 * @return
 */
char*
at_gl_texturematerial_get_specular(AtGLTextureMaterial* texturematerial);

/**
 * @brief at_gl_texturematerial_get_emissive
 * @param texturematerial
 * @return
 */
char*
at_gl_texturematerial_get_emissive(AtGLTextureMaterial* texturematerial);

/**
 * @brief at_gl_texturematerial_get_normal
 * @param texturematerial
 * @return
 */
char*
at_gl_texturematerial_get_normal(AtGLTextureMaterial* texturematerial);

/**
 * @brief at_gl_texturematerial_get_bump
 * @param texturematerial
 * @return
 */
char*
at_gl_texturematerial_get_bump(AtGLTextureMaterial* texturematerial);

/**
 * @brief at_gl_texturematerial_get_displacement
 * @param texturematerial
 * @return
 */
char*
at_gl_texturematerial_get_displacement(AtGLTextureMaterial* texturematerial);

G_END_DECLS
#endif
