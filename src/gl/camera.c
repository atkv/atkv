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
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtGLCameraPrivate{
  AtMat4* projection_matrix;
  AtMat4* view_matrix;
  AtVec3* right;
  AtVec3* forward;
  AtVec3* up;
  AtVec3* origin;

}AtGLCameraPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(AtGLCamera, at_gl_camera, G_TYPE_OBJECT)


/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
void
at_gl_camera_reset_viewmatrix(AtGLCamera* camera){

}
void
at_gl_camera_update_viewmatrix(AtGLCamera* camera){

}

void
at_gl_camera_move_right(AtGLCamera* camera, double units){

}

void
at_gl_camera_move_left(AtGLCamera* camera, double units){

}

void
at_gl_camera_move_forward(AtGLCamera* camera, double units){

}
void
at_gl_camera_move_backwards(AtGLCamera* camera, double units){

}

void
at_gl_camera_move_up(AtGLCamera* camera, double units){

}

void
at_gl_camera_move_bottom(AtGLCamera* camera, double units){

}

void
at_gl_camera_yaw(AtGLCamera* camera, double units){

}

void
at_gl_camera_pitch(AtGLCamera* camera, double units){

}

void
at_gl_camera_roll(AtGLCamera* camera, double units){

}

void
at_gl_camera_lookat(AtGLCamera *camera,
                    AtVec3* position,
                    AtVec3* to,
                    AtVec3* up){

}
