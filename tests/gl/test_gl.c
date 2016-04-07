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
#include <cmocka.h>
#include <at/gl.h>
#include <epoxy/gl.h>
#include <gtk/gtk.h>

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
  g_autoptr(AtGLShader)  vshader = at_gl_shader_new_from_file("test_gl_vertex.glsl");
  g_autoptr(AtGLShader)  fshader = at_gl_shader_new_from_file("test_gl_frag.glsl");
  g_autoptr(AtGLProgram) program = at_gl_program_new_from_shaders(vshader, fshader);

  g_autoptr(AtGLMesh)   object = at_gl_mesh_new_from_file("cube.obj");
  g_autoptr(AtGLScene)  scene  = at_gl_scene_new();
  at_gl_scene_add_mesh(scene, object);
  assert_int_equal(at_gl_container_get_num_children(AT_GL_CONTAINER(scene)), 1);
}

static void
test_at_gl_init(void** state){
  // Obter o contexto WebGL
  gtk_init(NULL,NULL);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget* gl_area = gtk_gl_area_new();
  g_signal_connect(gl_area, "realize", G_CALLBACK(gl_area_realize_event), NULL);
  g_signal_connect(gl_area, "render", G_CALLBACK(gl_area_render_event), NULL);
  gtk_container_add(GTK_CONTAINER(window), gl_area);
  gtk_widget_show_all(window);
  gtk_main();
}

int
main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_at_gl_init),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
