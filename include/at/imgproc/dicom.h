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

#ifndef AT_IMGPROC_DICOM_H
#define AT_IMGPROC_DICOM_H
#include <at/imgproc.h>
G_BEGIN_DECLS

/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define AT_TYPE_DICOM_base     at_dicom_base_get_type()
#define AT_TYPE_DICOM_uint8_t  at_dicom_uint8_t_get_type()
#define AT_TYPE_DICOM_uint16_t at_dicom_uint16_t_get_type()

G_DECLARE_DERIVABLE_TYPE(AtDicom_base, at_dicom_base, AT, DICOM_base, GObject)
G_DECLARE_DERIVABLE_TYPE(AtDicom_uint8_t,  at_dicom_uint8_t,  AT, DICOM_uint8_t,  AtDicom_base)
G_DECLARE_DERIVABLE_TYPE(AtDicom_uint16_t, at_dicom_uint16_t, AT, DICOM_uint16_t, AtDicom_base)
typedef struct _AtDicom_baseClass{
  GObjectClass parent_class;
}AtDicom_baseClass;
typedef struct _AtDicom_uint8_tClass{
  struct _AtDicom_baseClass parent_class;
}AtDicom_uint8_tClass;
typedef struct _AtDicom_uint16_tClass{
  struct _AtDicom_baseClass parent_class;
}AtDicom_uint16_tClass;


/*===========================================================================
 * FUNCTIONS
 *===========================================================================*/
#define AtDicom(type) AtDicom_##type


void
at_dicom_uint8_t_read(AtDicom_uint8_t** dicom, const char* filename);
void
at_dicom_uint16_t_read(AtDicom_uint16_t** dicom, const char* filename);
#define at_dicom_read(dicom, filename) _Generic((dicom), \
  AtDicom_uint8_t**:  at_dicom_uint8_t_read, \
  AtDicom_uint16_t**: at_dicom_uint16_t_read \
)(dicom, filename)

char*
at_dicom_base_get_filename(AtDicom_base* dicom);
#define at_dicom_get_filename(dicom) at_dicom_base_get_filename(AT_DICOM_base(dicom))

char*
at_dicom_base_get_modality(AtDicom_base* dicom);
#define at_dicom_get_modality(dicom) at_dicom_base_get_modality(AT_DICOM_base(dicom))

uint8_t
at_dicom_base_get_pixel_data_tag_found(AtDicom_base* dicom);
#define at_dicom_get_pixel_data_tag_found(dicom) at_dicom_base_get_pixel_data_tag_found(AT_DICOM_base(dicom))

char*
at_dicom_base_get_unit(AtDicom_base* dicom);
#define at_dicom_get_unit(dicom) at_dicom_base_get_unit(AT_DICOM_base(dicom))

uint16_t
at_dicom_uint8_t_get_dim(AtDicom_uint8_t* dicom);
uint16_t
at_dicom_uint16_t_get_dim(AtDicom_uint16_t* dicom);

#define at_dicom_get_dim(dicom) _Generic((dicom), \
  AtDicom_uint8_t*: at_dicom_uint8_t_get_dim, \
  AtDicom_uint16_t*: at_dicom_uint16_t_get_dim \
)(dicom)

size_t
at_dicom_uint8_t_get_elemsize(AtDicom_uint8_t* dicom);
size_t
at_dicom_uint16_t_get_elemsize(AtDicom_uint16_t* dicom);
#define at_dicom_get_elemsize(dicom) _Generic((dicom), \
  AtDicom_uint8_t*: at_dicom_uint8_t_get_elemsize, \
  AtDicom_uint16_t*: at_dicom_uint16_t_get_elemsize \
)(dicom)

uint64_t*
at_dicom_uint8_t_get_size(AtDicom_uint8_t* dicom);
uint64_t*
at_dicom_uint16_t_get_size(AtDicom_uint16_t* dicom);
#define at_dicom_get_size(dicom) _Generic((dicom), \
  AtDicom_uint8_t*: at_dicom_uint8_t_get_size, \
  AtDicom_uint16_t*: at_dicom_uint16_t_get_size \
)(dicom)

G_END_DECLS
#endif
