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

#ifndef AT_GL_PROGRAM_H
#define AT_GL_PROGRAM_H

#include <at/gl.h>
G_BEGIN_DECLS

/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/

#define AT_TYPE_GL_PROGRAM at_gl_program_get_type()
G_DECLARE_DERIVABLE_TYPE(AtGLProgram, at_gl_program, AT, GL_PROGRAM, GObject)
typedef struct _AtGLProgramClass{
  GObjectClass parent_class;
}AtGLProgramClass;

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
/**
 * @brief at_gl_program_new
 * @return
 */
AtGLProgram*
at_gl_program_new();

/**
 * @brief at_gl_program_new_from_shaders
 * @param vertexShader
 * @param fragmentShader
 */
AtGLProgram*
at_gl_program_new_from_shaders(AtGLShader* vertexShader,
                               AtGLShader* fragmentShader, GError** error);

/**
 * @brief at_gl_program_link
 * @param program
 * @param error
 */
void
at_gl_program_link(AtGLProgram* program, GError** error);

/**
 * @brief at_gl_program_is_linked
 * @param program
 * @return
 */
gboolean
at_gl_program_is_linked(AtGLProgram* program);

/*===========================================================================
 * ERROR HANDLING
 *===========================================================================*/

#define AT_GL_PROGRAM_ERROR at_gl_program_error_quark()

typedef enum {
  AT_GL_PROGRAM_ERROR_LINK   = 102,
}AtGLProgramError;

GQuark
at_gl_program_error_quark(void);


G_END_DECLS
#endif
