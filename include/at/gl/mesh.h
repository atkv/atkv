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
G_DECLARE_DERIVABLE_TYPE(AtGLMesh, at_gl_mesh, AT, GL_MESH, GObject)
typedef struct _AtGLMeshClass{
  GObjectClass parent_class;
}AtGLMeshClass;

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtGLMesh*
at_gl_mesh_new_from_file(char* file);

G_END_DECLS
#endif
