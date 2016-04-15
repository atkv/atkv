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
#define GET_PRIV(object) AtGLTextureMaterialPrivate* priv = at_gl_texturematerial_get_instance_private(object)
typedef struct _AtGLTextureMaterialPrivate{
  char* ambient_map;
  char* diffuse_map;
  char* specular_map;
  char* emissive_map;
  char* normal_map;
  char* bump_map;
  char* disp_map;
  char* alpha_map;
}AtGLTextureMaterialPrivate;
G_DEFINE_TYPE_WITH_PRIVATE(AtGLTextureMaterial, at_gl_texturematerial, AT_TYPE_GL_MATERIAL)

static void
at_gl_texturematerial_finalize(GObject* object){
  GET_PRIV(AT_GL_TEXTUREMATERIAL(object));
  if(priv->alpha_map)    g_free(priv->alpha_map);
  if(priv->ambient_map)  g_free(priv->ambient_map);
  if(priv->bump_map)     g_free(priv->bump_map);
  if(priv->diffuse_map)  g_free(priv->diffuse_map);
  if(priv->disp_map)     g_free(priv->disp_map);
  if(priv->emissive_map) g_free(priv->emissive_map);
  if(priv->normal_map)   g_free(priv->normal_map);
  if(priv->specular_map) g_free(priv->specular_map);
  G_OBJECT_CLASS(at_gl_texturematerial_parent_class)->finalize(object);
}

static void
at_gl_texturematerial_class_init(AtGLTextureMaterialClass *klass){
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = at_gl_texturematerial_finalize;
}

static void
at_gl_texturematerial_init(AtGLTextureMaterial *self){
  GET_PRIV(self);
  memset(priv, 0, sizeof(AtGLTextureMaterialPrivate));
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

AtGLTextureMaterial*
at_gl_texturematerial_new(){
  return g_object_new(AT_TYPE_GL_TEXTUREMATERIAL, NULL);
}

void
at_gl_texturematerial_set_ambient(AtGLTextureMaterial* texturematerial,
                                  char* ambient_map){
  GET_PRIV(texturematerial);
  priv->ambient_map = g_strdup(ambient_map);
}

void
at_gl_texturematerial_set_diffuse(AtGLTextureMaterial* texturematerial,
                                  char* diffuse_map){
  GET_PRIV(texturematerial);
  priv->diffuse_map = g_strdup(diffuse_map);
}

void
at_gl_texturematerial_set_specular(AtGLTextureMaterial* texturematerial,
                                   char* specular_map){
  GET_PRIV(texturematerial);
  priv->specular_map = g_strdup(specular_map);
}

void
at_gl_texturematerial_set_emissive(AtGLTextureMaterial* texturematerial,
                                   char* emissive_map){
  GET_PRIV(texturematerial);
  priv->emissive_map = g_strdup(emissive_map);
}

void
at_gl_texturematerial_set_normal(AtGLTextureMaterial* texturematerial,
                                   char* normal_map){
  GET_PRIV(texturematerial);
  priv->normal_map = g_strdup(normal_map);
}

void
at_gl_texturematerial_set_bump(AtGLTextureMaterial* texturematerial,
                                   char* bump_map){
  GET_PRIV(texturematerial);
  priv->bump_map = g_strdup(bump_map);
}

void
at_gl_texturematerial_set_displacement(AtGLTextureMaterial* texturematerial,
                                   char* disp_map){
  GET_PRIV(texturematerial);
  priv->disp_map = g_strdup(disp_map);
}

void
at_gl_texturematerial_set_alpha(AtGLTextureMaterial* texturematerial,
                                   char* alpha_map){
  GET_PRIV(texturematerial);
  priv->alpha_map = g_strdup(alpha_map);
}


char*
at_gl_texturematerial_get_ambient(AtGLTextureMaterial* texturematerial){
  GET_PRIV(texturematerial);
  return priv->ambient_map;
}

char*
at_gl_texturematerial_get_diffuse(AtGLTextureMaterial* texturematerial){
  GET_PRIV(texturematerial);
  return priv->diffuse_map;
}

char*
at_gl_texturematerial_get_specular(AtGLTextureMaterial* texturematerial){
  GET_PRIV(texturematerial);
  return priv->specular_map;
}

char*
at_gl_texturematerial_get_emissive(AtGLTextureMaterial* texturematerial){
  GET_PRIV(texturematerial);
  return priv->emissive_map;
}

char*
at_gl_texturematerial_get_normal(AtGLTextureMaterial* texturematerial){
  GET_PRIV(texturematerial);
  return priv->normal_map;
}

char*
at_gl_texturematerial_get_bump(AtGLTextureMaterial* texturematerial){
  GET_PRIV(texturematerial);
  return priv->bump_map;
}

char*
at_gl_texturematerial_get_displacement(AtGLTextureMaterial* texturematerial){
  GET_PRIV(texturematerial);
  return priv->disp_map;
}
