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
/**
 * @brief Test if we can load an OBJ file, with materials
 */
static void
test_at_gl_loader_obj(){
  g_autoptr(AtGLContainer)          container = NULL;
  g_autoptr(AtGLMaterialCollection) materials = NULL;
  GError** error = NULL;

  at_gl_loader_read_obj("cube.obj", &container,&materials, error);

  // Without error and container/materials filled
  g_assert_null(error);
  g_assert_nonnull(container);
  g_assert_nonnull(materials);

  // Correct name and Just 1 child: the cube mesh
  g_assert_cmpstr(at_gl_object_get_name(AT_GL_OBJECT(container)),=,"cube.obj");
  g_assert_cmpint(at_gl_container_get_num_children(container),=,1);

  // Its child should be a mesh
  AtGLObject* child = at_gl_container_get_by_index(container, 0);
  g_assert_true(AT_IS_GL_MESH(child));

  // Its name should be "Cube"
  AtGLMesh* mesh = AT_GL_MESH(child);
  g_assert_cmpstr(at_gl_object_get_name(child),==,"Cube");

  // It should be a collection of triangles
  AtGLGeometry* geometry = at_gl_get_geometry(mesh);
  g_assert_true(AT_IS_TRIANGLE_GEOMETRY(geometry));

  // It should have 12 triangles
  AtGLTriangleGeometry* triangles = AT_GL_TRIANGLEGEOMETRY(geometry);
  g_assert_cmpint(at_gl_trianglegeometry_get_num_triangles(triangles),==,12);

  // It should have 1 material with name "Material"
  g_assert_cmpint(at_gl_materialcollection_get_num_materials(materials), ==, 1);
  AtGLMaterial* material = at_gl_materialcollection_find(materials, "Material");
  g_assert_nonnull(material);

  // It should be a Color Material
  g_assert_true(AT_IS_GL_COLORMATERIAL(material));
  AtGLColorMaterial* colormat = AT_GL_COLORMATERIAL(material);

  // It should have correct Ambient Color
  AtVec3 ambient = at_gl_colormaterial_get_ambient(colormat);
  AtVec3 diffuse = at_gl_colormaterial_get_diffuse(colormat);
  AtVec3 specular = at_gl_colormaterial_get_specular(colormat);
  AtVec3 emissive = at_gl_colormaterial_get_emissive(colormat);
  double* a      = ambient.data;
  double* d      = diffuse.data;
  double* s      = specular.data;
  double* e      = emissive.data;
  double a_gt[3] = {1.000000, 1.000000, 1.000000};
  double d_gt[3] = {0.640000, 0.640000, 0.640000};
  double s_gt[3] = {0.500000, 0.500000, 0.500000};
  double e_gt[3] = {0.000000, 0.000000, 0.000000};

  uint8_t i;
  for(i = 0; i < 3; i++){
    g_assert_cmpfloat(a[i],==,a_gt[i]);
    g_assert_cmpfloat(d[i],==,d_gt[i]);
    g_assert_cmpfloat(s[i],==,s_gt[i]);
    g_assert_cmpfloat(e[i],==,e_gt[i]);
  }
}

/*===========================================================================
 * MAIN FUNCTION
 *===========================================================================*/
int
main(int argc, char** argv){
  setlocale(LC_ALL, "");
  g_test_init(&argc, &argv, NULL);
  g_test_bug_base("http://github.com/atkv/atkv/issues/");

  g_test_add_func("/at_gl/test_at_gl_loader_obj",
                  test_at_gl_loader_obj);

  g_test_add("/at_gl/test_at_scene",
             TestAtGLFixture,
             NULL,
             test_at_gl_fixture_set_up,
             test_at_gl_scene,
             test_at_gl_fixture_tear_down);
  return g_test_run();
}
