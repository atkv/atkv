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


#include <locale.h>
#include <glib-object.h>
#include <at/gl.h>

/*===========================================================================
 * TEST CASES
 *===========================================================================*/
static void
test_at_gl_camera(){
  AtGLPerspectiveCamera* perspectivecamera = at_gl_perspectivecamera_new();
  AtGLCamera* camera = AT_GL_CAMERA(perspectivecamera);
  AtMat4* viewmatrix = at_gl_camera_get_viewmatrix_ptr(camera);
  at_gl_camera_reset_viewmatrix(camera);
  uint8_t i;

  at_gl_camera_move_left(camera, 1.0);
  at_gl_camera_update_viewmatrix(camera);
  double viewmatrix_left[16]    = {1,0,0,0,0,1,0,0,0,0,1,0,-1.0,0,0,1};
  for(i = 0; i < 16; i++)
    g_assert_cmpfloat(viewmatrix->data[i],==,viewmatrix_left[i]);

  at_gl_camera_move_right(camera, 2.0);
  at_gl_camera_update_viewmatrix(camera);
  double viewmatrix_right[16]   = {1,0,0,0,0,1,0,0,0,0,1,0,2.0,0,0,1};
  for(i = 0; i < 16; i++)
    g_assert_cmpfloat(viewmatrix->data[i],==,viewmatrix_right[i]);

  at_gl_camera_move_forward(camera, 1.0);
  at_gl_camera_update_viewmatrix(camera);
  double viewmatrix_forward[16] = {1,0,0,0,0,1,0,0,0,0,1,0,2.0,0,-1,1};
  for(i = 0; i < 16; i++)
    g_assert_cmpfloat(viewmatrix->data[i],==,viewmatrix_forward[i]);

  at_gl_camera_move_backwards(camera, 2.0);
  at_gl_camera_update_viewmatrix(camera);
  double viewmatrix_backwards[16] = {1,0,0,0,0,1,0,0,0,0,1,0,2.0,0,2,1};
  for(i = 0; i < 16; i++)
    g_assert_cmpfloat(viewmatrix->data[i],==,viewmatrix_backwards[i]);

  at_gl_camera_reset_viewmatrix(camera);
  at_gl_camera_yaw(camera,90.0);
  at_gl_camera_update_viewmatrix(camera);
  double viewmatrix_yaw[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
  for(i = 0; i < 16; i++)
    g_assert_cmpfloat(viewmatrix->data[i],==,viewmatrix_yaw[i]);

  at_gl_camera_reset_viewmatrix(camera);
  at_gl_camera_pitch(camera,90.0);
  at_gl_camera_update_viewmatrix(camera);
  double viewmatrix_pitch[16] = {1,0,0,0,0,1,0,0,0,0,1,0,2.0,0,2,1};
  for(i = 0; i < 16; i++)
    g_assert_cmpfloat(viewmatrix->data[i],==,viewmatrix_pitch[i]);

  at_gl_camera_reset_viewmatrix(camera);
  at_gl_camera_roll(camera,90.0);
  at_gl_camera_update_viewmatrix(camera);
  double viewmatrix_roll[16] = {1,0,0,0,0,1,0,0,0,0,1,0,2.0,0,2,1};
  for(i = 0; i < 16; i++)
    g_assert_cmpfloat(viewmatrix->data[i],==,viewmatrix_roll[i]);

  at_gl_camera_reset_viewmatrix(camera);
  at_gl_camera_lookat(camera);
  double viewmatrix_lookat[16] = {1,0,0,0,0,1,0,0,0,0,1,0,2.0,0,2,1};
  for(i = 0; i < 16; i++)
    g_assert_cmpfloat(viewmatrix->data[i],==,viewmatrix_lookat[i]);

  at_gl_perspectivecamera_set(camera, 45.0, 1.4, 0.1, 100.0);
  AtMat4* projectionmatrix = at_gl_camera_get_projectionmatrix_ptr(camera);
  double projectionmatrix_lookat[16] = {1,0,0,0,0,1,0,0,0,0,1,0,2.0,0,2,1};
  for(i = 0; i < 16; i++)
    g_assert_cmpfloat(projectionmatrix->data[i],==,projectionmatrix_lookat[i]);
}

/*===========================================================================
 * MAIN FUNCTION
 *===========================================================================*/
int
main(int argc, char** argv){
  setlocale(LC_ALL, "");
  g_test_init(&argc, &argv, NULL);
  g_test_bug_base("http://github.com/atkv/atkv/issues/");

  g_test_add_func("/at_gl/test_at_gl_camera",
                  test_at_gl_camera);

  return g_test_run();
}

