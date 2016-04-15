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

#ifndef AT_GL_MESH_H
#define AT_GL_MESH_H

#include <at/gl.h>
G_BEGIN_DECLS
/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define AT_TYPE_GL_MESH at_gl_mesh_get_type()
G_DECLARE_DERIVABLE_TYPE(AtGLMesh, at_gl_mesh, AT, GL_MESH, AtGLMovableObject)
typedef struct _AtGLMeshClass{
  AtGLMovableObjectClass parent_class;
}AtGLMeshClass;

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
/**
 * @brief at_gl_mesh_new
 * @return
 */
AtGLMesh*
at_gl_mesh_new();

/**
 * @brief at_gl_mesh_get_geometry
 * @param mesh
 * @return
 */
AtGLGeometry*
at_gl_mesh_get_geometry(AtGLMesh* mesh);

/**
 * @brief at_gl_material_get_material
 * @param mesh
 * @return
 */
AtGLMaterial*
at_gl_material_get_material(AtGLMesh* mesh);

/**
 * @brief at_gl_mesh_set_geometry
 * @param mesh
 * @param geometry
 */
void
at_gl_mesh_set_geometry(AtGLMesh* mesh, AtGLGeometry* geometry);

/**
 * @brief at_gl_mesh_set_material
 * @param mesh
 * @param material
 */
void
at_gl_mesh_set_material(AtGLMesh* mesh, AtGLMaterial* material);

/**
 * @brief at_gl_mesh_calculate_normalmatrix
 * @param mesh
 * @param viewmatrix
 */
void
at_gl_mesh_calculate_normalmatrix(AtGLMesh* mesh, AtMat4* viewmatrix);

/*===========================================================================
 * ERROR HANDLING
 *===========================================================================*/

#define AT_GL_MESH_ERROR at_gl_mesh_error_quark()

typedef enum {
  AT_ERROR_GL_MESH_FILE_OPEN   = 103,
}AtGLMeshError;

GQuark
at_gl_mesh_error_quark(void);

G_END_DECLS
#endif
