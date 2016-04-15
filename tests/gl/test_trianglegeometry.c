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
test_at_gl_trianglegeometry(){
  g_autoptr(AtGLTriangleGeometry) trianglegeometry = at_gl_trianglegeometry_new();
  AtVec3  positions[4] = {{-1.0, -1.0, 0.0},{1.0,-1.0,0.0},{0.0,1.0,0.0},{1.0,1.0,0.0}};
  AtVec3  normals[4]   = {{-1.0, -1.0, 0.0},{1.0,-1.0,0.0},{0.0,1.0,0.0},{1.0,1.0,0.0}};
  AtVec2  uvs[4]       = {{0.0, 1.0},       {1.0,1.0},     {0.5,0.0},    {1.0,0.0}};
  AtVec3i indices[6]   = {{0,0,0},{1,1,1},{2,2,2},
                          {2,1,0},{1,1,1},{3,3,3}};
  at_gl_trianglegeometry_set_positions(trianglegeometry,positions,3);
  at_gl_trianglegeometry_set_normals(trianglegeometry  ,normals,3);
  at_gl_trianglegeometry_set_uvs(trianglegeometry      ,uvs,3);
  at_gl_trianglegeometry_set_indices(trianglegeometry  ,indices, 3);
  g_assert_cmpint(at_gl_trianglegeometry_get_num_positions(trianglegeometry),==,3);
  g_assert_cmpint(at_gl_trianglegeometry_get_num_normals(trianglegeometry),==,3);
  g_assert_cmpint(at_gl_trianglegeometry_get_num_uvs(trianglegeometry),==,3);
  g_assert_cmpint(at_gl_trianglegeometry_get_num_indices(trianglegeometry),==,3);

  AtVec3*  positions_t = at_gl_trianglegeometry_get_positions(trianglegeometry);
  AtVec3*  normals_t   = at_gl_trianglegeometry_get_normals(trianglegeometry);
  AtVec2*  uvs_t       = at_gl_trianglegeometry_get_uvs(trianglegeometry);
  AtVec3i* indices_t   = at_gl_trianglegeometry_get_indices(trianglegeometry);
  uint8_t i,k;
  for(i = 0; i < 3; i++)
  {
    for(k = 0; k < 3; k++){
      g_assert_cmpfloat(positions_t[i].data[k],==,positions[i].data[k]);
      g_assert_cmpfloat(normals_t[i].data[k],==,normals[i].data[k]);
      g_assert_cmpfloat(indices_t[i].data[k],==,indices[i].data[k]);
      if(k < 2)
        g_assert_cmpfloat(uvs_t[i].data[k],==,uvs[i].data[k]);
    }
  }

  g_autoptr(AtGLTriangleGeometry) triangle2 = at_gl_trianglegeometry_new();
  for(i = 0; i < 4; i++){
    at_gl_trianglegeometry_add_position(triangle2,&positions[i]);
    at_gl_trianglegeometry_add_normal  (triangle2,&normals[i]  );
    at_gl_trianglegeometry_add_uv      (triangle2,&uvs[i]      );
  }
  for(i = 0; i < 6; i++){
    at_gl_trianglegeometry_add_index   (triangle2,&indices[i]  );
  }

  positions_t = at_gl_trianglegeometry_get_positions(triangle2);
  normals_t   = at_gl_trianglegeometry_get_normals  (triangle2);
  uvs_t       = at_gl_trianglegeometry_get_uvs      (triangle2);
  indices_t   = at_gl_trianglegeometry_get_indices  (triangle2);

  for(i = 0; i < 4; i++)
  {
    for(k = 0; k < 3; k++){
      g_assert_cmpfloat(positions_t[i].data[k],==,positions[i].data[k]);
      g_assert_cmpfloat(normals_t[i].data[k],==,normals[i].data[k]);
      if(k < 2)
        g_assert_cmpfloat(uvs_t[i].data[k],==,uvs[i].data[k]);
    }
  }
  for(i = 0; i < 6; i++){
    for(k = 0; k < 3; k++){
      g_assert_cmpfloat(indices_t[i].data[k],==,indices[i].data[k]);
    }
  }


  AtVec3  positions_p[5] = {{-1.0, -1.0, 0.0},{1.0,-1.0,0.0},{0.0,1.0,0.0},
                            {0.0,1.0,0.0},{1.0,1.0,0.0}};
  AtVec3  normals_p[5]   = {{-1.0, -1.0, 0.0},{1.0,-1.0,0.0},{0.0,1.0,0.0},
                            {1.0,-1.0,0.0},{1.0,1.0,0.0}};
  AtVec2  uvs_p[5]       = {{0.0, 1.0},       {1.0,1.0},     {0.5,0.0},
                            {0.0, 1.0},       {1.0,0.0}};
  uint32_t indices_p[6]  = {0,1,2,3,1,4};
  at_gl_trianglegeometry_pack(triangle2);
  g_assert_cmpint(at_gl_trianglegeometry_get_num_packed(triangle2),==, 5);

  AtVec3*   positions_t_p = at_gl_trianglegeometry_get_positions_pkd(triangle2);
  AtVec3*   normals_t_p   = at_gl_trianglegeometry_get_normals_pkd  (triangle2);
  AtVec2*   uvs_t_p       = at_gl_trianglegeometry_get_uvs_pkd      (triangle2);
  uint32_t* indices_t_p   = at_gl_trianglegeometry_get_indices_pkd  (triangle2);

  for(i = 0; i < 5; i++)
  {
    for(k = 0; k < 3; k++){
      g_assert_cmpfloat(positions_t_p[i].data[k],==,positions_p[i].data[k]);
      g_assert_cmpfloat(normals_t_p[i].data[k]  ,==,normals_p[i].data[k]);
      if(k < 2)
        g_assert_cmpfloat(uvs_t_p[i].data[k]    ,==,uvs_p[i].data[k]);
    }
  }
  for(i = 0; i < 6; i++){
    g_assert_cmpfloat(indices_t_p[i],==,indices_p[i]);
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

  g_test_add_func("/at_gl/test_at_gl_trianglegeometry",
                  test_at_gl_trianglegeometry);

  return g_test_run();
}
