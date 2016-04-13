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

#include <at/gl.h>

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
#define GET_PRIV(self) AtGLMaterialCollectionPrivate* priv = \
                       at_gl_materialcollection_get_instance_private(self);
typedef struct _AtGLMaterialCollectionPrivate{
  GHashTable* hash_table;
}AtGLMaterialCollectionPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtGLMaterialCollection, at_gl_materialcollection,
                           G_TYPE_OBJECT)

static void
at_gl_materialcollection_finalize(GObject* object){
  GET_PRIV(AT_GL_MATERIALCOLLECTION(object));
  g_hash_table_destroy(priv->hash_table);
  G_OBJECT_CLASS(at_gl_materialcollection_parent_class)->finalize(object);
}

static void
at_gl_materialcollection_class_init(AtGLMaterialCollectionClass *klass){

}

static void
at_gl_materialcollection_init(AtGLMaterialCollection *self){
  GET_PRIV(self);
  priv->hash_table = g_hash_table_new(g_str_hash, g_str_equal);
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/


AtGLMaterialCollection*
at_gl_materialcollection_new(){
  return g_object_new(AT_TYPE_GL_MATERIALCOLLECTION, NULL);
}

void
at_gl_materialcollection_add(AtGLMaterialCollection* collection,
                             AtGLMaterial* material){
  GET_PRIV(collection);
  g_hash_table_insert(priv->hash_table,at_gl_material_get_name(material), material);
}

void
at_gl_materialcollection_remove(AtGLMaterialCollection* collection,
                                AtGLMaterial* material){
  at_gl_materialcollection_remove_by_name(collection,
                                          at_gl_material_get_name(material));

}

void
at_gl_materialcollection_remove_by_name(AtGLMaterialCollection* collection,
                                        char* name){
  GET_PRIV(collection);
  g_hash_table_remove(priv->hash_table,name);
}

AtGLMaterial*
at_gl_materialcollection_find(AtGLMaterialCollection* collection, char* name){
  GET_PRIV(collection);
  if(g_hash_table_contains(priv->hash_table,name))
    return g_hash_table_lookup(priv->hash_table,name);
  return NULL;
}


AtGLMaterial*
at_gl_materialcollection_foreach(AtGLMaterialCollection* collection, uint8_t index){

}

AtGLMaterialCollection*
at_gl_materialcollection_join(AtGLMaterialCollection* collection, AtGLMaterialCollection* subcollection){

}
