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

#ifndef AT_GL_LOADER_H
#define AT_GL_LOADER_H

#include <at/gl.h>
G_BEGIN_DECLS

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
/**
 * @brief at_gl_loader_read_mtl
 * @param filename
 * @param materials
 */
void
at_gl_loader_read_mtl(char* filename, AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read(char* filename, AtGLContainer** container,
                  AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_obj
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_obj(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_md5
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_md5(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_md3
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_md3(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_dae
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_dae(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_fbx
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_fbx(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_ms3d
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_ms3d(char* filename, AtGLContainer** container,
                       AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_bvh
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_bvh(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_lwo
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_lwo(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_kmz
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_kmz(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_ase
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_ase(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_ac3d
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_ac3d(char* filename, AtGLContainer** container,
                       AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_md2
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_md2(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_mdl7
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_mdl7(char* filename, AtGLContainer** container,
                       AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_x3d
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_x3d(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_jmf
 * @param filename
 * @param container
 * @param materialsd
 * @param error
 */
void
at_gl_loader_read_jmf(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error);

/**
 * @brief at_gl_loader_read_jma
 * @param filename
 * @param container
 * @param materials
 * @param error
 */
void
at_gl_loader_read_jma(char* filename, AtGLContainer** container,
                      AtGLMaterialCollection** materials, GError** error);

/*===========================================================================
 * ERROR HANDLING
 *===========================================================================*/
#define AT_ERROR_GL_LOADER at_error_gl_loader_quark()
typedef enum{
  AT_ERROR_GL_LOADER_READ_FORMAT_UNKNOWN     = 104,
  AT_ERROR_GL_LOADER_READ_FORMAT_OBJ_INVALID = 105,
  AT_ERROR_GL_LOADER_READ_FORMAT_MTL_INVALID = 106,
  AT_ERROR_GL_LOADER_READ_FORMAT_MD5_INVALID = 107,
  AT_ERROR_GL_LOADER_READ_FORMAT_MD3_INVALID = 108,
  AT_ERROR_GL_LOADER_READ_FORMAT_DAE_INVALID = 109,
  AT_ERROR_GL_LOADER_READ_FORMAT_FBX_INVALID = 110,
  AT_ERROR_GL_LOADER_READ_FORMAT_MS3D_INVALID= 111,
  AT_ERROR_GL_LOADER_READ_FORMAT_BVH_INVALID = 112,
  AT_ERROR_GL_LOADER_READ_FORMAT_LWO_INVALID = 113,
  AT_ERROR_GL_LOADER_READ_FORMAT_KMZ_INVALID = 114,
  AT_ERROR_GL_LOADER_READ_FORMAT_ASE_INVALID = 115,
  AT_ERROR_GL_LOADER_READ_FORMAT_AC3D_INVALID= 116,
  AT_ERROR_GL_LOADER_READ_FORMAT_MD2_INVALID = 117,
  AT_ERROR_GL_LOADER_READ_FORMAT_MDL7_INVALID= 118,
  AT_ERROR_GL_LOADER_READ_FORMAT_X3D_INVALID = 119,
  AT_ERROR_GL_LOADER_READ_FORMAT_JMF_INVALID = 110,
  AT_ERROR_GL_LOADER_READ_FORMAT_JMA_INVALID = 111,
}AtErrorGLLoader;

GQuark
at_error_gl_loader_quark(void);


G_END_DECLS
#endif
