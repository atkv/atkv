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

#include <setjmp.h>
#include <stdlib.h>
#include <stdarg.h>
#include <at/gl.h>
#include <epoxy/gl.h>
#include <gtk/gtk.h>
#include <locale.h>

/*===========================================================================
 * HELPERS
 *===========================================================================*/
typedef struct{
  AtGLShader*  vertexShader;
  AtGLShader*  fragmentShader;
  AtGLProgram* programShader;
}TestAtGLFixture;

static void
test_at_gl_fixture_set_up(TestAtGLFixture* fixture,
                          gconstpointer user_data){

}
static void
test_at_gl_fixture_tear_down(TestAtGLFixture* fixture,
                             gconstpointer user_data){

}

static gboolean
gl_area_render_event(GtkGLArea* area, GdkGLContext* context, gpointer user_data){
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  return TRUE;
}
static void
gl_area_realize_event(GtkGLArea* area, gpointer user_data){
  gtk_gl_area_make_current(area);
  if(gtk_gl_area_get_error(area) != NULL) return;
  GError* error = NULL;

  // Compiling a vertex shader
  g_autoptr(AtGLShader)  vshader = at_gl_shader_new_from_file(GL_VERTEX_SHADER,   "test_gl_vertex.glsl", &error);
  g_assert_no_error(error);
  g_assert_nonnull(vshader);
  // Compiling a vertex shader
  g_autoptr(AtGLShader)  fshader = at_gl_shader_new_from_file(GL_FRAGMENT_SHADER, "test_gl_frag.glsl" , &error);
  g_assert_no_error(error);
  g_assert_nonnull(fshader);
  // Compiling a program
  g_autoptr(AtGLProgram) program = at_gl_program_new_from_shaders(vshader, fshader, &error);
  g_assert_no_error(error);
  g_assert_nonnull(program);

  g_assert_true(at_gl_shader_is_compiled(vshader));
  g_assert_true(at_gl_shader_is_compiled(fshader));
  g_assert_true(at_gl_program_is_linked(program));
}
/*===========================================================================
 * TEST CASES
 *===========================================================================*/
/**
 * @brief test_at_gl_shader_program
 */
static void
test_at_gl_shader_program(){
  // Obter o contexto WebGL
  gtk_init(NULL,NULL);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget* gl_area = gtk_gl_area_new();
  g_signal_connect(gl_area, "realize", G_CALLBACK(gl_area_realize_event), NULL);
  g_signal_connect(gl_area, "render",  G_CALLBACK(gl_area_render_event), NULL);
  g_signal_connect(window, "destroy",  G_CALLBACK(gtk_main_quit), NULL);
  gtk_container_add(GTK_CONTAINER(window), gl_area);
  gtk_widget_show_all(window);
  gtk_main();
}

/**
 * @brief test_at_gl_shader_program
 */
static void
test_at_gl_scene(TestAtGLFixture* fixture, gconstpointer user_data){
  GError* error = NULL;

//  g_autoptr(AtGLContainer) cube   = at_gl_loader_read("cube.obj", &error);
//  g_autoptr(AtGLContainer) octa   = at_gl_mesh_new_from_file("octahedron.obj", &error);
//  g_autoptr(AtGLScene)     scene  = at_gl_scene_new();

//  g_assert_cmpuint(at_gl_mesh_get_num_vertices(cube),==,8); // 8 vertices
//  g_assert_cmpuint(at_gl_mesh_get_num_faces(cube)   ,==,12);// 12 triangles (6 squares)
//  g_assert_cmpuint(at_gl_mesh_get_num_normals(cube) ,==,8); // 1 normal per vertex
//  g_assert_cmpuint(at_gl_mesh_get_num_uvs(cube)     ,==,0); // we still don't use texture

//  g_assert_cmpuint(at_gl_mesh_get_num_vertices(octa),==,6); // 4 base + 2 up/down
//  g_assert_cmpuint(at_gl_mesh_get_num_faces(octa)   ,==,8); // 2 pyramids (2 x 4)
//  g_assert_cmpuint(at_gl_mesh_get_num_normals(octa) ,==,6); // 1 normal per vertex
//  g_assert_cmpuint(at_gl_mesh_get_num_uvs(octa)     ,==,6); // we still don't use texture

//  at_gl_scene_add_mesh(scene, cube);
//  g_assert_cmpuint(at_gl_container_get_num_children(AT_GL_CONTAINER(scene)),=, 1);
//  at_gl_scene_add_mesh(scene, octa);
//  g_assert_cmpuint(at_gl_container_get_num_children(AT_GL_CONTAINER(scene)),=, 2);


}

int
main(int argc, char** argv){
  setlocale(LC_ALL, "");
  g_test_init(&argc, &argv, NULL);
  g_test_bug_base("http://github.com/atkv/atkv/issues/");

  g_test_add_func("/at_gl/test_at_gl_shader_program",
                  test_at_gl_shader_program);

  g_test_add("/at_gl/test_at_scene",
             TestAtGLFixture,
             NULL,
             test_at_gl_fixture_set_up,
             test_at_gl_scene,
             test_at_gl_fixture_tear_down);
  return g_test_run();
}
