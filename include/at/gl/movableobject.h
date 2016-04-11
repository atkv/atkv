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

#ifndef AT_GL_MOVABLEOBJECT_H
#define AT_GL_MOVABLEOBJECT_H
#include <at/gl.h>

G_BEGIN_DECLS
/*===========================================================================
 * CLASS STRUCTURE
 *===========================================================================*/
G_DECLARE_DERIVABLE_TYPE(AtGLMovableObject, at_gl_movableobject, AT, 
                         GL_MOVABLEOBJECT, GObject)
typedef structure _AtGLMovableObjectClass{

}AtGLMovableObjectClass;
/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

void
at_gl_object_reset_matrix(AtGLMovableObject* object);

void
at_gl_object_translate(AtGLMovableObject* object, AtVec3* vector);

void
at_gl_object_rotate(AtGLMovableObject* object, double angle, AtVec3* axis);

void
at_gl_object_scale(AtGLMovableObject* object, AtVec3* vector);

G_END_DECLS 