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

#ifndef AT_IMGPROC_NIFTI_H
#define AT_IMGPROC_NIFTI_H
#include <at/imgproc.h>
G_BEGIN_DECLS

/*===========================================================================
 * MACROS AND ENUMS
 *===========================================================================*/
typedef enum{
  AT_NIFTI_TYPE_ANALYZE  = 0,
  AT_NIFTI_TYPE_NIFTI1_1 = 1,
  AT_NIFTI_TYPE_NIFTI1_2 = 2,
  AT_NIFTI_TYPE_ASCII    = 3,
  AT_NIFTI_TYPE_MAX      = 3,
} AtNiftiFileType;

/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define AT_TYPE_NIFTI_IMAGE at_nifti_image_get_type()
G_DECLARE_DERIVABLE_TYPE(AtNiftiImage, at_nifti_image, AT, NIFTI_IMAGE, AtArray_uint16_t)
typedef struct _AtNiftiImageClass{
  AtArray_uint16_tClass parent_class;
}AtNiftiImageClass;
/*===========================================================================
 * FUNCTIONS
 *===========================================================================*/

/**
 * @brief Reads a Nifti file and optionally the data, creating a
 *        AtNiftiImage object.
 *
 * - The data buffer will be byteswapped if necessary.
 * - The data buffer will not be scaled.
 * - The data buffer is allocated with calloc().
 *
 * @param hname filename of the nifti dataset
 * @param read_data Flag, true=read data blob, false=don't read blob.
 * @return A pointer to the AtNiftiImage data structure.
 */
AtNiftiImage*
at_nifti_image_read(const char *hname, gboolean read_data);
/**
 * @brief at_nifti_image_get_nsize
 * @param image
 * @return
 */
AtVec7
at_nifti_image_get_nsize(AtNiftiImage* image);
/**
 * @brief at_nifti_image_get_dsize
 * @param image
 * @return
 */
AtVec7
at_nifti_image_get_dsize(AtNiftiImage* image);
/**
 * @brief at_nifti_image_get_qfac
 * @param image
 * @return
 */
float
at_nifti_image_get_qfac(AtNiftiImage* image);
/**
 * @brief at_nifti_image_get_byteorder
 * @param image
 * @return
 */
AtEndianess
at_nifti_image_get_byteorder(AtNiftiImage* image);
/**
 * @brief at_nifti_image_get_filetype
 * @param image
 * @return
 */
AtNiftiFileType
at_nifti_image_get_filetype(AtNiftiImage* image);
/**
 * @brief at_nifti_image_get_filename
 * @param image
 * @return
 */
char*
at_nifti_image_get_filename(AtNiftiImage* image);
/**
 * @brief at_nifti_image_get_iname
 * @param image
 * @return
 */
char*
at_nifti_image_get_iname(AtNiftiImage* image);
/**
 * @brief at_nifti_image_get_ndim
 * @param image
 * @return
 */
int
at_nifti_image_get_ndim(AtNiftiImage* image);
/**
 * @brief return the total volume size, in bytes
 *
 * This is computed as nvox * nbyper.
 *
 * @param nim
 * @return
 */
size_t
at_nifti_image_get_volsize(AtNiftiImage *nim);

G_END_DECLS
#endif
