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

typedef struct _AtGLOrthographicCameraPrivate{
  double left;
  double right;
  double bottom;
  double top;
  double near;
  double far;
}AtGLOrthographicCameraPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtGLOrthographicCamera, at_gl_orthographiccamera, AT_TYPE_GL_CAMERA)

static void
at_gl_orthographiccamera_class_init(AtGLOrthographicCameraClass *klass){

}

static void
at_gl_orthographiccamera_init(AtGLOrthographicCamera *self){

}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
void
at_gl_orthographiccamera_set(AtGLOrthographicCamera* camera,
                             double left,   double right,
                             double bottom, double top,
                             double near,   double far){
  AtGLOrthographicCameraPrivate* priv = at_gl_orthographiccamera_get_instance_private(camera);
  priv->left = left;
  priv->right = right;
  priv->bottom = bottom;
  priv->top = top;
  priv->near = near;
  priv->far = far;
}
