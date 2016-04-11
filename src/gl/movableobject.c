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
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtGLMovableObjectPrivate{
  AtMat4 model_matrix;
}AtGLMovableObjectPrivate;
/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
void
at_gl_object_reset_matrix(AtGLMovableObject* object){
  AtGLMovableObjectPrivate* priv = at_gl_movableobject_get_instance_private(object);
  at_mat4_to_eye(&priv->model_matrix);
}

void
at_gl_object_translate(AtGLMovableObject* object, AtVec3* vector){
  AtGLMovableObjectPrivate* priv = at_gl_movableobject_get_instance_private(object);
  at_mat4_translate(&priv->model_matrix, vector);
}

void
at_gl_object_rotate(AtGLMovableObject* object, double angle, AtVec3* axis){
  AtGLMovableObjectPrivate* priv = at_gl_movableobject_get_instance_private(object);
  at_mat4_rotate(&priv->model_matrix, angle, axis);
}

void
at_gl_object_scale(AtGLMovableObject* object, AtVec3* vector){
  AtGLMovableObjectPrivate* priv = at_gl_movableobject_get_instance_private(object);
  at_mat4_scale(&priv->model_matrix, vector);
}