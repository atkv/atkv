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

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
// SOME MACROS TO FILL LIST
// ----------------------------
#define COMMAND_IS(type,n) strncmp(line, type, n) == 0
#define FILL_LIST(item,size)\
for(vi = 0; vi < size; vi++){\
  item.data[vi] = strtod(p1, &p2);\
  p1 = p2;\
}\
at_gl_trianglegeometry_add_##item(cur_geometry,&item);

#define FILL_COLOR_MATERIAL(item,size)\
for(vi = 0; vi < size; vi++){\
  item.data[vi] = strtod(p1, &p2);\
  p1 = p2;\
}\
at_gl_colormaterial_set_##item(AT_GL_COLORMATERIAL(cur_material),&item);

#define LOADER_READ(type) \
if(g_str_has_suffix(filename,#type)){ \
  at_gl_loader_read_##type(filename, container, materials,error); \
}
#define COLOR_TO_TEXTURE_MATERIAL() \
if(AT_IS_GL_COLORMATERIAL(cur_material)){ \
  g_set_object(&old_material, cur_material); \
  cur_material = AT_GL_MATERIAL(at_gl_texturematerial_new_from_material(cur_material)); \
  at_gl_materialcollection_add(materials,cur_material); \
  g_clear_object(&old_material); \
}
static AtGLTextureMaterial*
at_gl_texturematerial_new_from_material(AtGLMaterial* material){
  AtGLTextureMaterial* texturematerial = at_gl_texturematerial_new();
  at_gl_material_set_name(AT_GL_MATERIAL(texturematerial),
                          at_gl_material_get_name(material));
  return texturematerial;
}
#define FILL_TEXTURE_MATERIAL(item, mapname) \
p2 = strchr(p1,'\n'); \
memcpy(map_##mapname,p1,p2-p1); \
map_##mapname[p2-p1] = 0; \
at_gl_texturematerial_set_##item(AT_GL_TEXTUREMATERIAL(cur_material),map_##mapname);

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
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

  AtGLMesh*             cur_mesh     = NULL;
  AtGLTriangleGeometry* cur_geometry = NULL;

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
  char* path = g_path_get_dirname(filename);
  g_autofree char* material_path_filename = NULL;


  *container_ptr = at_gl_container_new();


  AtGLContainer* container = *container_ptr;
  AtGLMaterialCollection* materials = NULL;
  AtVec3 position;
  AtVec3 normal;
  AtVec2 uv;
  AtVec3i index2;

  at_gl_object_set_name(AT_GL_OBJECT(container), filename);

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
      material_path_filename = g_build_filename(path,material_filename, NULL);
      at_gl_loader_read_mtl(material_path_filename, materials_ptr, error);
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
      if(cur_geometry)
        at_gl_trianglegeometry_pack(cur_geometry);

      cur_mesh     = at_gl_mesh_new();
      cur_geometry = at_gl_trianglegeometry_new();
      at_gl_mesh_set_geometry(cur_mesh, AT_GL_GEOMETRY(cur_geometry));

      at_gl_object_set_name(AT_GL_OBJECT(cur_mesh), object_name);
      at_gl_container_add_object(container,AT_GL_OBJECT(cur_mesh));
    }else

    // It's an a vertex position
    if(COMMAND_IS("v ",2)){
      FILL_LIST(position, 3);
      //FILL_LIST(vertices, 3, sizeof(float), num_vertices, num_vertices_alloc);
    }else

    // It's a vertex normal
    if(COMMAND_IS("vn",2)){ // It's a vertex normal data
      FILL_LIST(normal, 3);
      //FILL_LIST(normals, 3, sizeof(float), num_normals, num_normals_alloc);
    }else

    // It's a vertex texture
    if(COMMAND_IS("vt",2)){  // If it's a vertex texture uv coord
      FILL_LIST(uv, 2);
      //FILL_LIST(texcoords, 2, sizeof(float), num_texcoords, num_texcoords_alloc);
      has_uv = TRUE;
    }else

    // It's a material name used by the current object
    if(COMMAND_IS("usemtl",6)){ // It's a material name
      p2 = strchr(p1,'\n');
      memcpy(material_name,p1,p2-p1);
      material_name[p2-p1] = 0;
      AtGLMaterial* material = at_gl_materialcollection_find(materials,material_name);
      at_gl_mesh_set_material(cur_mesh,material);
    }else

    // It's a shadow parameter
    if(COMMAND_IS("s",1)){

    }else

    // It's a face
    if(COMMAND_IS("f",1)){ // It's a face (9 numbers)
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

          index2.data[k] = strtoul(index, NULL, 10);
          if(index2.data[k] != 0) index2.data[k]--;
        }
        at_gl_trianglegeometry_add_index(cur_geometry,&index2);
      }
    }
  }
  if(cur_geometry)
    at_gl_trianglegeometry_pack(cur_geometry);
  setlocale(LC_NUMERIC,oldLocale);
}

void
at_gl_loader_read_mtl(char* filename, AtGLMaterialCollection **materials_ptr, GError **error){
  g_autofree char* line = g_malloc(256 * sizeof(char)); // string of line of MTL file
  size_t   len;                    // number of chars in `line`
  ssize_t  read;                   // read
  FILE* fp = fopen(filename, "r");
  if(!fp){
    g_set_error(error,AT_ERROR_GL_LOADER, AT_ERROR_FILE_NOT_FOUND,
                "Couldn't load OBJ mesh: could not open file %s for reading",
                filename);
    return;
  }
  uint8_t  vi;
  char    *p1, *p2; // start and end of a token
  char     material_name[128];     // newmtl <material_name>
  *materials_ptr = at_gl_materialcollection_new();
  AtGLMaterialCollection* materials = *materials_ptr;
  AtGLMaterial* cur_material, *old_material;
  AtVec3 ambient, diffuse, specular, emissive;

  char map_Ka[128];
  char map_Kd[128];
  char map_Ks[128];
  char map_Ke[128];
  char map_d[128];
  char map_Ns[128];
  char map_disp[128];
  char map_bump[128];

  // PARSING THE OBJ FILE
  // ----------------------------
  while((read = getline(&line,&len,fp)) != -1){
    p1 = line;
    p2 = strchr(p1,' ');p1 = p2 + 1;

    // It's a material filename
    if(COMMAND_IS("newmtl",6)){
      p2 = strchr(p1,'\n');
      memcpy(material_name,p1,p2-p1);
      material_name[p2-p1] = 0;
      cur_material = AT_GL_MATERIAL(at_gl_colormaterial_new());
      at_gl_material_set_name(cur_material,material_name);
      at_gl_materialcollection_add(materials,cur_material);
    }else

    if(COMMAND_IS("Ka", 2)){
      FILL_COLOR_MATERIAL(ambient, 3);
    }else

    if(COMMAND_IS("Kd", 2)){
      FILL_COLOR_MATERIAL(diffuse, 3);
    }else

    if(COMMAND_IS("Ks", 2)){
      FILL_COLOR_MATERIAL(specular, 3);
    }else

    if(COMMAND_IS("Ke", 2)){
      FILL_COLOR_MATERIAL(emissive, 3);
    }else

    if(COMMAND_IS("Ns", 2)){
    }else

    if(COMMAND_IS("d", 1)){
    }else

    if(COMMAND_IS("illum", 5)){
    }else

    if(COMMAND_IS("map_Ka", 6)){
      COLOR_TO_TEXTURE_MATERIAL();
      FILL_TEXTURE_MATERIAL(ambient, Ka);
    }else

    if(COMMAND_IS("map_Kd", 6)){
      COLOR_TO_TEXTURE_MATERIAL();
      FILL_TEXTURE_MATERIAL(diffuse, Kd);
    }else

    if(COMMAND_IS("map_Ks", 6)){
      COLOR_TO_TEXTURE_MATERIAL();
      FILL_TEXTURE_MATERIAL(specular, Ks);
    }else

    if(COMMAND_IS("map_Ke", 6)){
      COLOR_TO_TEXTURE_MATERIAL();
      FILL_TEXTURE_MATERIAL(emissive, Ke);
    }else

    if(COMMAND_IS("map_Ns", 6)){
      COLOR_TO_TEXTURE_MATERIAL();

    }else

    if(COMMAND_IS("disp", 4)){
      COLOR_TO_TEXTURE_MATERIAL();
      FILL_TEXTURE_MATERIAL(displacement, disp);
    }else

    if(COMMAND_IS("map_d", 5)){
      COLOR_TO_TEXTURE_MATERIAL();
      FILL_TEXTURE_MATERIAL(alpha, d);
    }else

    if(COMMAND_IS("map_bump", 5) || COMMAND_IS("bump", 4)){
      COLOR_TO_TEXTURE_MATERIAL();
      FILL_TEXTURE_MATERIAL(bump, bump);
    }
  }
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

GQuark
at_error_gl_loader_quark(void){
  return g_quark_from_static_string("at-error-gl-loader-quark");
}
