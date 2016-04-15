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

#ifndef AT_GL_TRIANGLEGEOMETRY_H
#define AT_GL_TRIANGLEGEOMETRY_H
#include <at/gl.h>
#include <string.h>

G_BEGIN_DECLS
/*===========================================================================
 * CLASS STRUCTURE
 *===========================================================================*/
#define AT_TYPE_GL_TRIANGLEGEOMETRY at_gl_trianglegeometry_get_type()
G_DECLARE_DERIVABLE_TYPE(AtGLTriangleGeometry, at_gl_trianglegeometry, AT,
                         GL_TRIANGLEGEOMETRY, GObject)
typedef struct _AtGLTriangleGeometryClass{
  GObjectClass parent_class;
}AtGLTriangleGeometryClass;

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
/**
 * @brief at_gl_trianglegeometry_new
 * @return
 */
AtGLTriangleGeometry*
at_gl_trianglegeometry_new();

/**
 * @brief at_gl_trianglegeometry_get_positions
 * @param geometry
 * @return
 */
AtVec3*
at_gl_trianglegeometry_get_positions(AtGLTriangleGeometry* geometry);
/**
 * @brief at_gl_trianglegeometry_get_normals
 * @param geometry
 * @return
 */
AtVec3*
at_gl_trianglegeometry_get_normals(AtGLTriangleGeometry* geometry);
/**
 * @brief at_gl_trianglegeometry_get_uvs
 * @param geometry
 * @return
 */
AtVec2*
at_gl_trianglegeometry_get_uvs(AtGLTriangleGeometry* geometry);
/**
 * @brief at_gl_trianglegeometry_get_indices
 * @param geometry
 * @return
 */
AtVec3i*
at_gl_trianglegeometry_get_indices(AtGLTriangleGeometry* geometry);
/**
 * @brief at_gl_trianglegeometry_get_positions_pkd
 * @param geometry
 * @return
 */
AtVec3*
at_gl_trianglegeometry_get_positions_pkd(AtGLTriangleGeometry* geometry);
/**
 * @brief at_gl_trianglegeometry_get_normals_pkd
 * @param geometry
 * @return
 */
AtVec3*
at_gl_trianglegeometry_get_normals_pkd(AtGLTriangleGeometry* geometry);
/**
 * @brief at_gl_trianglegeometry_get_uvs_pkd
 * @param geometry
 * @return
 */
AtVec2*
at_gl_trianglegeometry_get_uvs_pkd(AtGLTriangleGeometry* geometry);
/**
 * @brief at_gl_trianglegeometry_get_indices_pkd
 * @param geometry
 * @return
 */
uint32_t*
at_gl_trianglegeometry_get_indices_pkd(AtGLTriangleGeometry* geometry);

/**
 * @brief at_gl_trianglegeometry_get_num_positions
 * @param geometry
 * @return
 */
uint32_t
at_gl_trianglegeometry_get_num_positions(AtGLTriangleGeometry* geometry);
/**
 * @brief at_gl_trianglegeometry_get_num_normals
 * @param geometry
 * @return
 */
uint32_t
at_gl_trianglegeometry_get_num_normals(AtGLTriangleGeometry* geometry);
/**
 * @brief at_gl_trianglegeometry_get_num_uvs
 * @param geometry
 * @return
 */
uint32_t
at_gl_trianglegeometry_get_num_uvs(AtGLTriangleGeometry* geometry);
/**
 * @brief at_gl_trianglegeometry_get_num_indices
 * @param geometry
 * @return
 */
uint32_t
at_gl_trianglegeometry_get_num_indices(AtGLTriangleGeometry* geometry);
/**
 * @brief at_gl_trianglegeometry_get_num_packed
 * @param geometry
 * @return
 */
uint32_t
at_gl_trianglegeometry_get_num_packed(AtGLTriangleGeometry* geometry);
/**
 * @brief at_gl_trianglegeometry_set_positions
 * @param geometry
 * @param positions
 * @param num_positions
 */
void
at_gl_trianglegeometry_set_positions(AtGLTriangleGeometry* geometry,
                                     AtVec3* positions, uint32_t num_positions);
/**
 * @brief at_gl_trianglegeometry_set_normals
 * @param geometry
 * @param normals
 * @param num_normals
 */
void
at_gl_trianglegeometry_set_normals(AtGLTriangleGeometry* geometry,
                                     AtVec3* normals, uint32_t num_normals);
/**
 * @brief at_gl_trianglegeometry_set_uvs
 * @param geometry
 * @param uvs
 * @param num_uvs
 */
void
at_gl_trianglegeometry_set_uvs(AtGLTriangleGeometry* geometry,
                                     AtVec2* uvs, uint32_t num_uvs);
/**
 * @brief at_gl_trianglegeometry_set_indices
 * @param geometry
 * @param indices
 * @param num_indices
 */
void
at_gl_trianglegeometry_set_indices(AtGLTriangleGeometry* geometry,
                                   AtVec3i* indices, uint32_t num_indices);
/**
 * @brief at_gl_trianglegeometry_add_position
 * @param geometry
 * @param position
 */
void
at_gl_trianglegeometry_add_position(AtGLTriangleGeometry* geometry, AtVec3* position);

/**
 * @brief at_gl_trianglegeometry_add_normal
 * @param geometry
 * @param normal
 */
void
at_gl_trianglegeometry_add_normal(AtGLTriangleGeometry* geometry, AtVec3* normal);

/**
 * @brief at_gl_trianglegeometry_add_uv
 * @param geometry
 * @param uv
 */
void
at_gl_trianglegeometry_add_uv(AtGLTriangleGeometry* geometry, AtVec2* uv);
/**
 * @brief at_gl_trianglegeometry_add_index
 * @param geometry
 * @param indice
 */
void
at_gl_trianglegeometry_add_index(AtGLTriangleGeometry* geometry, AtVec3i* indice);



/**
 * @brief at_gl_trianglegeometry_pack
 * @param geometry
 */
void
at_gl_trianglegeometry_pack(AtGLTriangleGeometry* geometry);

/**
 * @brief at_gl_trianglegeometry_get_num_triangles
 * @param geometry
 */
uint32_t
at_gl_trianglegeometry_get_num_triangles(AtGLTriangleGeometry* geometry);

G_END_DECLS
#endif
