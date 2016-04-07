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

#ifndef AT_GL_SHADER_H
#define AT_GL_SHADER_H

#include <at/gl.h>
#include <glib-object.h>
G_BEGIN_DECLS

/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/

#define AT_TYPE_GL_SHADER at_gl_shader_get_type()
G_DECLARE_DERIVABLE_TYPE(AtGLShader, at_gl_shader, AT, GL_SHADER, GObject)

typedef struct _AtGLShaderClass{
  GObjectClass parent_class;
}AtGLShaderClass;


/*===========================================================================
 * FUNCTIONS
 *===========================================================================*/

/**
 * @brief at_gl_shader_new_from_file
 * @param file
 * @return
 */
AtGLShader*
at_gl_shader_new_from_file(char* file);

/**
 * @brief at_gl_shader_new_from_text
 * @param text
 * @return
 */
AtGLShader*
at_gl_shader_new_from_text(char* text);

/**
 * @brief at_gl_shader_new
 * @return
 */
AtGLShader*
at_gl_shader_new();

/**
 * @brief at_gl_shader_set_text
 * @param shader
 * @param text
 */
void
at_gl_shader_set_text(AtGLShader* shader, char* text);

/**
 * @brief at_gl_shader_compile
 * @param shader
 */
void
at_gl_shader_compile(AtGLShader* shader);
G_END_DECLS
#endif
