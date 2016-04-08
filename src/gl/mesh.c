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
#include <at/core.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtGLMeshPrivate{
  AtGLGeometry* geometry;
  AtGLMaterial* material;
}AtGLMeshPrivate;

static void
at_gl_mesh_object_interface_init(AtGLObject *iface);

G_DEFINE_TYPE_WITH_PRIVATE_AND_CODE(AtGLMesh, at_gl_mesh, G_TYPE_OBJECT,G_IMPLEMENT_INTERFACE(AT_TYPE_GL_OBJECT,at_gl_mesh_object_interface_init))
static void
at_gl_mesh_class_init(AtGLMeshClass *klass){

}
static void
at_gl_mesh_init(AtGLMesh *self){
  AtGLMeshPrivate* priv = at_gl_mesh_get_instance_private(self);
  priv->geometry = NULL;
  priv->material = NULL;
}
static void
at_gl_mesh_object_interface_init(AtGLObject *iface){

}

static void
at_gl_mesh_pack_indices(uint32_t **indices, uint8_t num_components, uint32_t *num_indices, uint32_t* num_indices_alloc){

}

static void
at_gl_mesh_read_mtl(char* material_filename){

}

static AtGLMesh*
at_gl_mesh_new_from_file_obj(char* filename, GError** error){
  // LISTS AND VARIABLES
  // ----------------------------
  g_autofree char* line = g_malloc(256 * sizeof(char)); // string of line of OBJ file
  size_t   len;  // number of chars in `line`
  ssize_t  read; // read
  uint8_t  has_uv = FALSE;

  char     object_name[256]; // o <object_name>
  char     material_name[128]; // usemtl <material_name>
  char     material_filename[128]; // mtllib <material_name>

  uint8_t  vi,k;
  char    *p1, *p2; // start and end of a token

  // Mesh data
  float   *vertices = NULL       , *texcoords=NULL      , *normals=NULL;
  uint32_t num_vertices       = 0, num_texcoords      =0, num_normals      =0;
  uint32_t num_vertices_alloc = 0, num_texcoords_alloc=0, num_normals_alloc=0;
  uint32_t*indices=NULL, num_indices=0, num_indices_alloc=0;

  char     index[8]; // index in string
  char     seps[3] = "// ";

  char    *oldLocale = setlocale(LC_NUMERIC, NULL);

  // SOME MACROS TO FILL LIST
  // ----------------------------
#define COMMAND_IS(type,n) strncmp(line, type, n) == 0
#define FIX_LIST_SIZE(list, size, typesize, num_vertices, num_vertices_alloc) \
 if(vertices == NULL){ \
   vertices = malloc(size * typesize); \
   num_vertices_alloc = 1; \
 }else if(num_vertices == num_vertices_alloc){ \
   num_vertices_alloc <<= 1; \
   list = realloc(list, num_vertices_alloc * size * typesize); \
 }
#define FILL_LIST(list,size,typesize,num_vertices,num_vertices_alloc) \
 FIX_LIST_SIZE(list,size,typesize,num_vertices,num_vertices_alloc) \
 for(vi = 0; vi < size; vi++){ \
   vertices[size*num_vertices+vi] = strtof(p1, &p2); \
   p1 = p2; \
 } \
 num_vertices++;


  // LOCALE FOR DOT AS DECIMAL POINT
  // ----------------------------
  setlocale(LC_NUMERIC,"C");

  // OPENING FILE
  // ----------------------------
  g_return_val_if_fail(error == NULL || *error == NULL, NULL);

  FILE* fp = fopen(filename, "r");
  if(!fp){
    g_set_error(error,AT_GL_MESH_ERROR, AT_GL_MESH_ERROR_FILE_OPEN,
                "Couldn't load OBJ mesh: could not open file %s for reading",
                filename);
    return NULL;
  }

  // PARSING THE OBJ FILE
  // ----------------------------
  while((read = getline(&line,&len,fp)) != -1){
    p1 = line;
    p2 = strchr(p1,' ');p1 = p2 + 1;

    // It's a material filename
    if(COMMAND_IS("mtllib",6)){
      p2 = strchr(p1,'\n');
      memcpy(material_filename,p1,p2-p1);
      material_filename[p2-p1] = 0;
    }else

    // It's an object name
    if(COMMAND_IS("o",1)){
      p2 = strchr(p1,'\n');
      memcpy(object_name,p1,p2-p1);
      object_name[p2-p1] = 0;
    }else

    // It's an a vertex position
    if(COMMAND_IS("v ",2)){
      FILL_LIST(vertices, 3, sizeof(float), num_vertices, num_vertices_alloc);
    }else

    // It's a vertex texture
    if(COMMAND_IS("vt",2)){  // If it's a vertex texture uv coord
      FILL_LIST(texcoords, 2, sizeof(float), num_texcoords, num_texcoords_alloc);
      has_uv = TRUE;
    }else

    // It's a vertex normal
    if(COMMAND_IS("vn",2)){ // It's a vertex normal data
      FILL_LIST(normals, 3, sizeof(float), num_normals, num_normals_alloc);
    }else

    // It's a material name used by the current object
    if(COMMAND_IS("usemtl",6)){ // It's a material name
      p2 = strchr(p1,'\n');
      memcpy(material_name,p1,p2-p1);
      material_name[p2-p1] = 0;
    }else

    // It's a shadow parameter
    if(COMMAND_IS("s",1)){

    }else

    // It's a face
    if(COMMAND_IS("f",1)){ // It's a face (9 numbers)
      // Fix list
      if(indices == NULL){
        num_indices_alloc = 3;
        indices = g_malloc(num_indices_alloc * 3 *sizeof(uint32_t));
      }
      else if(num_indices == num_indices_alloc){
        num_indices_alloc <<= 1;
        indices = g_realloc(indices,num_indices_alloc * 3 * sizeof(uint32_t));
      }

      for(vi = 0; vi < 3; vi++){
        for(k = 0; k < 3; k++){
          if(vi == 2 && k == 2)
            p2 = strchr(p1,'\n');
          else
            p2 = strchr(p1,seps[k]);

          memcpy(index, p1, p2-p1);
          index[p2-p1] = 0;
          if(p1 == p2) p1+=2;
          else p1 = p2;

          indices[3*num_indices+k] = strtoul(index, NULL,10);
        }
        num_indices++;
      }
    }
  }
#undef COMMAND_IS
#undef FILL_LIST
#undef FIX_LIST_SIZE

  at_gl_mesh_pack_indices(&indices, 3, &num_indices, &num_indices_alloc);
  at_gl_mesh_read_mtl(material_filename);
  setlocale(LC_NUMERIC,oldLocale);
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtGLMesh*
at_gl_mesh_new(){
  return g_object_new(AT_TYPE_GL_MESH, NULL);
}
AtGLMesh*
at_gl_mesh_new_from_file(char* file, GError** error){
  g_return_val_if_fail(error == NULL || *error == NULL, NULL);
  return at_gl_mesh_new_from_file_obj(file, error);
}
uint32_t
at_gl_mesh_get_num_vertices(AtGLMesh* mesh){
  return 0;
}

uint32_t
at_gl_mesh_get_num_faces(AtGLMesh* mesh){
  return 0;
}

uint32_t
at_gl_mesh_get_num_normals(AtGLMesh* mesh){
  return 0;
}

uint32_t
at_gl_mesh_get_num_uvs(AtGLMesh* mesh){
  return 0;
}

GQuark
at_gl_mesh_error_quark(void){
  return g_quark_from_static_string("at-gl-mesh-error-quark");
}
