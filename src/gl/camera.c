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
#include <at/core.h>
#include <string.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtGLCameraPrivate{
  AtMat4 projection_matrix;
  AtMat4 view_matrix;
  AtVec3 right;
  AtVec3 forward;
  AtVec3 up;
  AtVec3 origin;

}AtGLCameraPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(AtGLCamera, at_gl_camera, G_TYPE_OBJECT)
static void
at_gl_camera_class_init(AtGLCameraClass *klass){

}
static void
at_gl_camera_init(AtGLCamera *self){
  AtGLCameraPrivate* priv = at_gl_camera_get_instance_private(self);
  memset(priv, 0, sizeof(AtGLCameraPrivate));
  AtMat4 eye = at_mat4_eye();
  priv->projection_matrix = eye;
  at_gl_camera_reset_viewmatrix(self);
}

/**
 * @brief Rotate vectors `n1` and `n2` `units` degrees
 * @param n1
 * @param n2
 * @param units
 */
static void
at_gl_rotate_plane(double* n1, double* n2, double units){
  // Rotação sobre o eixo up
  double cosV = cos(units/180.0*M_PI);
  double sinV = sin(units/180.0*M_PI);

  // O pitch rotaciona o up e o forward
  uint8_t i;
  AtVec4 new_n;
  double* n = new_n.data;
  for(i = 0; i < 3; i++){
    n[i]  = n1[i]*cosV + n2[i]*sinV;
    n2[i] = n2[i]*cosV - n1[i]*sinV;
    n1[i] = n[i];
  }
}
/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
void
at_gl_camera_reset_viewmatrix(AtGLCamera* camera){
  AtGLCameraPrivate* priv = at_gl_camera_get_instance_private(camera);
  priv->right       = at_vec3_x();
  priv->forward     = at_vec3_y();
  priv->up          = at_vec3_z();
  priv->view_matrix = at_mat4_eye();
  at_vec3_negate(&priv->forward);
}
void
at_gl_camera_update_viewmatrix(AtGLCamera* camera){
  AtGLCameraPrivate* priv = at_gl_camera_get_instance_private(camera);
  double* v = priv->view_matrix.data;
  double* r = priv->right.data;
  double* o = priv->origin.data;
  double* f = priv->forward.data;
  double* u = priv->up.data;
  // Inverse of rotational component (which is just
  // a transpose
  v[0 ] = r[0]; v[1 ] = u[0]; v[2 ] =-f[0]; v[3 ] = 0;
  v[4 ] = r[1]; v[5 ] = u[1]; v[6 ] =-f[1]; v[7 ] = 0;
  v[8 ] = r[2]; v[9 ] = u[2]; v[10] =-f[2]; v[11] = 0;
  // Inverse of rotated translational component (dot product)
  v[12] = -at_vec3_dot(&priv->right  , &priv->origin);
  v[13] = -at_vec3_dot(&priv->up     , &priv->origin);
  v[14] =  at_vec3_dot(&priv->forward, &priv->origin);
  v[15] = 1;
}

void
at_gl_camera_move_right(AtGLCamera* camera, double units){
  AtGLCameraPrivate* priv = at_gl_camera_get_instance_private(camera);
  double* o = priv->origin.data;
  double* r = priv->right.data;
  o[0] += r[0]*units;
  o[1] += r[1]*units;
  o[2] += r[2]*units;
}

void
at_gl_camera_move_left(AtGLCamera* camera, double units){
  at_gl_camera_move_right(camera, -units);
}

void
at_gl_camera_move_forward(AtGLCamera* camera, double units){
  AtGLCameraPrivate* priv = at_gl_camera_get_instance_private(camera);
  double* o = priv->origin.data;
  double* f = priv->forward.data;
  o[0] += f[0]*units;
  o[1] += f[1]*units;
  o[2] += f[2]*units;
}
void
at_gl_camera_move_backwards(AtGLCamera* camera, double units){
  at_gl_camera_move_forward(camera, -units);
}

void
at_gl_camera_move_up(AtGLCamera* camera, double units){
  AtGLCameraPrivate* priv = at_gl_camera_get_instance_private(camera);
  double* o = priv->origin.data;
  double* u = priv->up.data;
  o[0] += u[0]*units;
  o[1] += u[1]*units;
  o[2] += u[2]*units;
}

void
at_gl_camera_move_bottom(AtGLCamera* camera, double units){
  at_gl_camera_move_up(camera, -units);
}

void
at_gl_camera_yaw(AtGLCamera* camera, double units){
  AtGLCameraPrivate* priv = at_gl_camera_get_instance_private(camera);
  at_gl_rotate_plane(priv->right.data, priv->forward.data, units);
}

void
at_gl_camera_pitch(AtGLCamera* camera, double units){
  AtGLCameraPrivate* priv = at_gl_camera_get_instance_private(camera);
  at_gl_rotate_plane(priv->forward.data, priv->up.data, units);
}

void
at_gl_camera_roll(AtGLCamera* camera, double units){
  AtGLCameraPrivate* priv = at_gl_camera_get_instance_private(camera);
  at_gl_rotate_plane(priv->right.data, priv->up.data, units);
}


void
at_gl_camera_lookat(AtGLCamera *camera,
                    AtVec3* position,
                    AtVec3* to,
                    AtVec3* up){
  AtGLCameraPrivate* priv = at_gl_camera_get_instance_private(camera);
  double* f = priv->forward.data;
  double* r = priv->right.data;
  double* u = priv->up.data;
  double* o = priv->origin.data;
  double* t = to->data;

  priv->origin = *position;

  f[0] = t[0] - o[0];
  f[1] = t[1] - o[1];
  f[2] = t[2] - o[2];

  at_vec3_normalize(&priv->forward);
  priv->up    = *up;
  priv->right =  at_vec3_cross(&priv->forward, &priv->up);
  priv->up    =  at_vec3_cross(&priv->right  , &priv->forward);
  at_vec3_normalize(&priv->right);
  at_vec3_normalize(&priv->up);
  at_gl_camera_update_viewmatrix(camera);
}

AtMat4*
at_gl_camera_get_projectionmatrix_ptr(AtGLCamera* camera){
  AtGLCameraPrivate* priv = at_gl_camera_get_instance_private(camera);
  return &priv->projection_matrix;
}
AtMat4*
at_gl_camera_get_viewmatrix_ptr(AtGLCamera* camera){
  AtGLCameraPrivate* priv = at_gl_camera_get_instance_private(camera);
  return &priv->view_matrix;
}

