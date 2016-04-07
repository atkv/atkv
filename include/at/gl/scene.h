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

#ifndef AT_GL_SCENE_H
#define AT_GL_SCENE_H

#include <at/gl.h>
G_BEGIN_DECLS
/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define AT_TYPE_GL_SCENE at_gl_scene_get_type()
G_DECLARE_DERIVABLE_TYPE(AtGLScene, at_gl_scene, AT, GL_SCENE, GObject)
typedef struct _AtGLSceneClass{
  GObjectClass parent_class;
}AtGLSceneClass;

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtGLScene*
at_gl_scene_new();

void
at_gl_scene_add_object(AtGLScene* scene, AtGLObject* object);

void
at_gl_scene_add_mesh(AtGLScene* scene, AtGLMesh* mesh);

G_END_DECLS
#endif
