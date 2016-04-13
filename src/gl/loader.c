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
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


// SOME MACROS TO FILL LIST
// ----------------------------
#define COMMAND_IS(type,n) strncmp(line, type, n) == 0
#define FIX_LIST_SIZE(list, size, typesize, num_vertices, num_vertices_alloc) \
if(vertices == NULL){ \
 vertices = g_malloc(size * typesize); \
 num_vertices_alloc = 1; \
}else if(num_vertices == num_vertices_alloc){ \
 num_vertices_alloc <<= 1; \
 list = g_realloc(list, num_vertices_alloc * size * typesize); \
}
#define FILL_LIST(list,size,typesize,num_vertices,num_vertices_alloc) \
FIX_LIST_SIZE(list,size,typesize,num_vertices,num_vertices_alloc) \
for(vi = 0; vi < size; vi++){ \
 vertices[size*num_vertices+vi] = strtof(p1, &p2); \
 p1 = p2; \
} \
num_vertices++;


#define LOADER_READ(type) \
if(g_str_has_suffix(filename,#type)){ \
  at_gl_loader_read_##type(filename, container, materials,error); \
}

void
at_gl_loader_read(char *filename, AtGLContainer **container, AtGLMaterialCollection **materials, GError **error){
  LOADER_READ(obj)  else
  LOADER_READ(dae)  else
  LOADER_READ(md5)  else
  LOADER_READ(md3)  else
  LOADER_READ(fbx)  else
  LOADER_READ(ms3d)  else
  LOADER_READ(bvh)  else
  LOADER_READ(lwo)  else
  LOADER_READ(kmz)  else
  LOADER_READ(ase)  else
  LOADER_READ(ac3d)  else
  LOADER_READ(md2)  else
  LOADER_READ(mdl7)  else
  LOADER_READ(x3d)  else
  LOADER_READ(jmf)  else
  LOADER_READ(jma)  else{
    g_set_error(error,
                AT_ERROR_GL_LOADER,
                AT_ERROR_GL_LOADER_READ_FORMAT_UNKNOWN,
                "Format unkwnown");
  }
}

void
at_gl_loader_read_obj(char* filename, AtGLContainer** container_ptr, AtGLMaterialCollection **materials_ptr, GError** error){
  g_return_if_fail(container_ptr != NULL &&  materials_ptr != NULL &&  error != NULL && filename != NULL &&
                  *container_ptr == NULL && *materials_ptr == NULL && *error == NULL);

  // LISTS AND VARIABLES
  // ----------------------------
  g_autofree char* line = g_malloc(256 * sizeof(char)); // string of line of OBJ file
  size_t   len;                    // number of chars in `line`
  ssize_t  read;                   // read
  uint8_t  has_uv = FALSE;         // the face is define differently when has uv or not

  char     object_name[256];       // o <object_name>
  char     material_name[128];     // usemtl <material_name>
  char     material_filename[128]; // mtllib <material_name>

  uint8_t  vi;
  uint8_t  k;
  char    *p1, *p2; // start and end of a token

  // Mesh data
  float   *vertices = NULL       , *texcoords=NULL      , *normals=NULL;
  uint32_t num_vertices       = 0, num_texcoords      =0, num_normals      =0;
  uint32_t num_vertices_alloc = 0, num_texcoords_alloc=0, num_normals_alloc=0;
  // indices data
  uint32_t*indices  = NULL, num_indices=0, num_indices_alloc=0;

  AtGLMesh* cur_mesh = NULL;

  char     index[8]; // index in string
  char     seps[3] = "// ";

  // Get locale (ex: portuguese locale
  char    *oldLocale = setlocale(LC_NUMERIC, NULL);

  // LOCALE FOR DOT AS DECIMAL POINT
  // ----------------------------
  setlocale(LC_NUMERIC,"C");

  // OPENING FILE
  // ----------------------------
  FILE* fp = fopen(filename, "r");
  if(!fp){
    g_set_error(error,AT_GL_MESH_ERROR, AT_ERROR_GL_MESH_FILE_OPEN,
                "Couldn't load OBJ mesh: could not open file %s for reading",
                filename);
    return;
  }

  *container_ptr = at_gl_container_new();

  AtGLContainer* container = *container_ptr;
  AtGLMaterialCollection* materials = NULL;

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
      at_gl_loader_read_mtl(material_filename, materials_ptr, error);
      if(*error != NULL){
        fclose(fp);
        g_clear_object(container_ptr);
        return;
      }else{
        materials = *materials_ptr;
      }
    }else

    // It's an object name
    if(COMMAND_IS("o",1)){
      p2 = strchr(p1,'\n');
      memcpy(object_name,p1,p2-p1);
      object_name[p2-p1] = 0;
      if(cur_mesh)
        at_gl_mesh_pack_indices(cur_mesh);
      cur_mesh = at_gl_mesh_new();
      at_gl_object_set_name(AT_GL_OBJECT(cur_mesh), object_name);
      at_gl_container_add_object(container,AT_GL_OBJECT(cur_mesh));
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

  if(cur_mesh) at_gl_mesh_pack_indices(cur_mesh);
  at_gl_mesh_read_mtl(material_filename);
  setlocale(LC_NUMERIC,oldLocale);
}

void
at_gl_loader_read_mtl(char* filename, AtGLMaterialCollection **materials, GError **error){

}

void
at_gl_loader_read_md5(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_md3(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_collada(char* filename, AtGLContainer** container,
                          AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_dae(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_fbx(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_ms3d(char* filename, AtGLContainer** container,
                       AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_bvh(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_lwo(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_kmz(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_ase(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_ac3d(char* filename, AtGLContainer** container,
                       AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_md2(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_mdl7(char* filename, AtGLContainer** container,
                       AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_x3d(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_jmf(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error){

}

void
at_gl_loader_read_jma(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error){

}
