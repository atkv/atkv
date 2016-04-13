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

#ifndef AT_GL_MATERIALCOLLECTION_H
#define AT_GL_MATERIALCOLLECTION_H

#include <at/gl.h>
G_BEGIN_DECLS

/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define AT_TYPE_GL_MATERIALCOLLECTION at_gl_materialcollection_get_type()
G_DECLARE_DERIVABLE_TYPE(AtGLMaterialCollection, at_gl_materialcollection,
                         AT, GL_MATERIALCOLLECTION,GObject)
typedef struct _AtGLMaterialCollectionClass{
  GObjectClass parent_class;
}AtGLMaterialCollectionClass;

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtGLMaterialCollection*
at_gl_materialcollection_new();

void
at_gl_materialcollection_add(AtGLMaterialCollection* collection,
                             AtGLMaterial* material);

void
at_gl_materialcollection_remove(AtGLMaterialCollection* collection,
                                AtGLMaterial* material);

void
at_gl_materialcollection_remove_by_name(AtGLMaterialCollection* collection,
                                        char* name);

AtGLMaterial*
at_gl_materialcollection_find(AtGLMaterialCollection* collection, char* name);

AtGLMaterial*
at_gl_materialcollection_get_by_index(AtGLMaterialCollection* collection, uint8_t index);

AtGLMaterialCollection*
at_gl_materialcollection_join(AtGLMaterialCollection* collection, AtGLMaterialCollection* subcollection);

G_END_DECLS
#endif
