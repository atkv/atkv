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

#ifndef AT_GL_CONTAINER_H
#define AT_GL_CONTAINER_H
#include <at/gl.h>
#include <stdint.h>
G_BEGIN_DECLS
/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define AT_TYPE_GL_CONTAINER at_gl_container_get_type()
G_DECLARE_DERIVABLE_TYPE(AtGLContainer, at_gl_container, AT, GL_CONTAINER, GObject)
typedef struct _AtGLContainerClass{
  GObjectClass parent_class;
}AtGLContainerClass;
/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
/**
 * @brief at_gl_container_get_num_children
 * @param container
 * @return
 */
uint8_t
at_gl_container_get_num_children(AtGLContainer* container);
/**
 * @brief at_gl_container_index_of
 * @param container
 * @param object
 * @return
 */
int32_t
at_gl_container_index_of(AtGLContainer* container, AtGLObject* object);
/**
 * @brief at_gl_container_add_object
 * @param container
 * @param object
 */
void
at_gl_container_add_object(AtGLContainer* container, AtGLObject* object);

G_END_DECLS
#endif
