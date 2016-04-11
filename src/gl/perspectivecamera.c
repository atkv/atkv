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

typedef struct _AtGLPerspectiveCameraPrivate{
  double fov;
  double aspect;
  double near;
  double far;
}AtGLPerspectiveCameraPrivate;
G_DEFINE_TYPE_WITH_PRIVATE(AtGLPerspectiveCamera, at_gl_perspectivecamera, AT_TYPE_GL_CAMERA)

static void
at_gl_perspectivecamera_class_init(AtGLPerspectiveCameraClass *klass){

}

static void
at_gl_perspectivecamera_init(AtGLPerspectiveCamera *self){
  AtGLPerspectiveCameraPrivate* priv = at_gl_perspectivecamera_get_instance_private(self);
  priv->aspect = 0.0;
  priv->far    = 0.0;
  priv->fov    = 0.0;
  priv->near   = 0.0;
}

static void
at_gl_perspectivecamera_frustrum(AtGLPerspectiveCamera* camera,
                                 double left  , double right,
                                 double bottom, double top,
                                 double near  , double far){
  AtGLPerspectiveCameraPrivate* priv = at_gl_perspectivecamera_get_instance_private(camera);

  double* te = at_gl_camera_get_projectionmatrix_ptr(AT_GL_CAMERA(camera))->data;
  double x = 2 * near / ( right - left );
  double y = 2 * near / ( top - bottom );

  double a = ( right + left ) / ( right - left );
  double b = ( top + bottom ) / ( top - bottom );
  double c = - ( far + near ) / ( far - near );
  double d = - 2 * far * near / ( far - near );

  te[ 0 ] = x;  te[ 4 ] = 0;  te[ 8 ] = a;  te[ 12 ] = 0;
  te[ 1 ] = 0;  te[ 5 ] = y;  te[ 9 ] = b;  te[ 13 ] = 0;
  te[ 2 ] = 0;  te[ 6 ] = 0;  te[ 10 ] = c; te[ 14 ] = d;
  te[ 3 ] = 0;  te[ 7 ] = 0;  te[ 11 ] = - 1; te[ 15 ] = 0;
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

void
at_gl_perspectivecamera_set(AtGLPerspectiveCamera* camera,
                            double fov, double aspect, double near, double far){
  double zoom     = 1;
  double new_fov_r= atan(tan(fov/360.0 * M_PI)/zoom);
  double ymax     = near * tan(new_fov_r);
  double ymin     = - ymax;
  double xmin     = ymin * aspect;
  double xmax     = ymax * aspect;
  return at_gl_perspectivecamera_frustrum(camera, xmin, xmax, ymin, ymax, near, far);
}

