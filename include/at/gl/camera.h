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

#ifndef AT_GL_CAMERA_H
#define AT_GL_CAMERA_H
#include <at/gl.h>
#include <at/core.h>
G_BEGIN_DECLS

/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/

#define AT_TYPE_GL_CAMERA at_gl_camera_get_type()
G_DECLARE_DERIVABLE_TYPE(AtGLCamera, at_gl_camera, AT, GL_CAMERA, GObject)
typedef struct _AtGLCameraClass{
  GObjectClass object_class;
}AtGLCameraClass;

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
/**
 * @brief at_gl_camera_reset_viewmatrix
 * @param camera
 */
void
at_gl_camera_reset_viewmatrix(AtGLCamera* camera);

/**
 * @brief at_gl_camera_update_viewmatrix
 * @param camera
 */
void
at_gl_camera_update_viewmatrix(AtGLCamera* camera);

/**
 * @brief at_gl_camera_move_right
 * @param camera
 * @param units
 */
void
at_gl_camera_move_right(AtGLCamera* camera, double units);

/**
 * @brief at_gl_camera_move_left
 * @param camera
 * @param units
 */
void
at_gl_camera_move_left(AtGLCamera* camera, double units);

/**
 * @brief at_gl_camera_move_forward
 * @param camera
 * @param units
 */
void
at_gl_camera_move_forward(AtGLCamera* camera, double units);

/**
 * @brief at_gl_camera_move_backwards
 * @param camera
 * @param units
 */
void
at_gl_camera_move_backwards(AtGLCamera* camera, double units);

/**
 * @brief at_gl_camera_move_up
 * @param camera
 * @param units
 */
void
at_gl_camera_move_up(AtGLCamera* camera, double units);

/**
 * @brief at_gl_camera_move_bottom
 * @param camera
 * @param units
 */
void
at_gl_camera_move_bottom(AtGLCamera* camera, double units);

/**
 * @brief at_gl_camera_yaw
 * @param camera
 * @param units
 */
void
at_gl_camera_yaw(AtGLCamera* camera, double units);

/**
 * @brief at_gl_camera_pitch
 * @param camera
 * @param units
 */
void
at_gl_camera_pitch(AtGLCamera* camera, double units);

/**
 * @brief at_gl_camera_roll
 * @param camera
 * @param units
 */
void
at_gl_camera_roll(AtGLCamera* camera, double units);

/**
 * @brief at_gl_camera_lookat
 * @param camera
 * @param position
 * @param to
 * @param up
 */
void
at_gl_camera_lookat(AtGLCamera *camera,
                    AtVec3* position,
                    AtVec3* to,
                    AtVec3* up);

/**
 * @brief at_gl_camera_get_projectionmatrix_ptr
 * @param camera
 * @return
 */
AtMat4*
at_gl_camera_get_projectionmatrix_ptr(AtGLCamera* camera);
/**
 * @brief at_gl_camera_get_viewmatrix_ptr
 * @param camera
 * @return
 */
AtMat4*
at_gl_camera_get_viewmatrix_ptr(AtGLCamera* camera);





G_END_DECLS
#endif
