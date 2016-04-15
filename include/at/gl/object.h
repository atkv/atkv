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

#ifndef AT_GL_OBJECT_H
#define AT_GL_OBJECT_H

#include <at/gl.h>
G_BEGIN_DECLS
/*===========================================================================
 * INTERFACE DECLARATION
 *===========================================================================*/
#define AT_TYPE_GL_OBJECT at_gl_object_get_type()
G_DECLARE_INTERFACE(AtGLObject, at_gl_object, AT, GL_OBJECT, GObject)
typedef struct _AtGLObjectInterface{
  GTypeInterface parent_iface;

  void (*set_name) (AtGLObject* self, char* name);
  char*(*get_name) (AtGLObject* self);

}AtGLObjectInterface;

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
/**
 * @brief Set name of the object
 * @param object
 * @param name
 */
void
at_gl_object_set_name(AtGLObject* object, char* name);

/**
 * @brief Get the object name
 * @param object
 * @return
 */
char*
at_gl_object_get_name(AtGLObject* object);

G_END_DECLS
#endif
