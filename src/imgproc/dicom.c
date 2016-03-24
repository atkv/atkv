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

#include <at/imgproc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/

#define AT_DICM_OFFSET        0x80 // 128 bits
#define AT_DICOM_IMPLICIT_VR 0x2D2D
#define AT_READ_BITS(variable,num_bits) fread(&variable,sizeof(uint ## num_bits ## _t),1,file)

typedef enum _AtDicomValueRepresentation{
  AT_DICOM_VR_AE = 0x4145,
  AT_DICOM_VR_AS = 0x4153,
  AT_DICOM_VR_AT = 0x4154,
  AT_DICOM_VR_CS = 0x4353,
  AT_DICOM_VR_DA = 0x4441,
  AT_DICOM_VR_DS = 0x4453,
  AT_DICOM_VR_DT = 0x4454,
  AT_DICOM_VR_FD = 0x4644,
  AT_DICOM_VR_FL = 0x464C,
  AT_DICOM_VR_IS = 0x4953,
  AT_DICOM_VR_LO = 0x4C4F,
  AT_DICOM_VR_LT = 0x4C54,
  AT_DICOM_VR_PN = 0x504E,
  AT_DICOM_VR_SH = 0x5348,
  AT_DICOM_VR_SL = 0x534C,
  AT_DICOM_VR_SS = 0x5353,
  AT_DICOM_VR_ST = 0x5354,
  AT_DICOM_VR_TM = 0x544D,
  AT_DICOM_VR_UI = 0x5549,
  AT_DICOM_VR_UL = 0x554C,
  AT_DICOM_VR_US = 0x5553,
  AT_DICOM_VR_UT = 0x5554,
  AT_DICOM_VR_OB = 0x4F42,
  AT_DICOM_VR_OW = 0x4F57,
  AT_DICOM_VR_SQ = 0x5351,
  AT_DICOM_VR_UN = 0x554E,
  AT_DICOM_VR_QQ = 0x3F3F,
  AT_DICOM_VR_RT = 0x5254,
} AtDicomValueRepresentation;

typedef enum _AtDicomTag{
  AT_DICOM_TAG_PIXEL_REPRESENTATION       = 0x00280103,
  AT_DICOM_TAG_TRANSFER_SYNTAX_UID        = 0x00020010,
  AT_DICOM_TAG_MODALITY                   = 0x00080060,
  AT_DICOM_TAG_SLICE_THICKNESS            = 0x00180050,
  AT_DICOM_TAG_SLICE_SPACING              = 0x00180088,
  AT_DICOM_TAG_SAMPLES_PER_PIXEL          = 0x00280002,
  AT_DICOM_TAG_PHOTOMETRIC_INTERPRETATION = 0x00280004,
  AT_DICOM_TAG_PLANAR_CONFIGURATION       = 0x00280006,
  AT_DICOM_TAG_NUMBER_OF_FRAMES           = 0x00280008,
  AT_DICOM_TAG_ROWS                       = 0x00280010,
  AT_DICOM_TAG_COLUMNS                    = 0x00280011,
  AT_DICOM_TAG_PIXEL_SPACING              = 0x00280030,
  AT_DICOM_TAG_BITS_ALLOCATED             = 0x00280100,
  AT_DICOM_TAG_WINDOW_CENTER              = 0x00281050,
  AT_DICOM_TAG_WINDOW_WIDTH               = 0x00281051,
  AT_DICOM_TAG_RESCALE_INTERCEPT          = 0x00281052,
  AT_DICOM_TAG_RESCALE_SLOPE              = 0x00281053,
  AT_DICOM_TAG_RED_PALETTE                = 0x00281201,
  AT_DICOM_TAG_GREEN_PALETTE              = 0x00281202,
  AT_DICOM_TAG_BLUE_PALETTE               = 0x00281203,
  AT_DICOM_TAG_ICON_IMAGE_SEQUENCE        = 0x00880200,
  AT_DICOM_TAG_PIXEL_DATA                 = 0x7FE00010,
} AtDicomTag;

// Dicom Private structure
typedef struct _AtDicomPrivate{
  const char* filename;
  char*       modality;
  char*       unit;
  float       intercept;
  float       slope;
  uint8_t     pixel_data_tag_found;
  uint16_t    samples_per_pixel;
  uint16_t    bits_allocated;
  uint8_t*    photo_interpretation;
  uint16_t    pixel_representation;
}AtDicom_basePrivate;

typedef struct _AtDicom_uint8_tPrivate{
  AtArray(uint8_t)* luts;
  AtArray(uint8_t)* image;
}AtDicom_uint8_tPrivate;

typedef struct _AtDicom_uint16Private{
  AtArray(uint16_t)* luts;
  AtArray(uint16_t)* image;
}AtDicom_uint16_tPrivate;


// Dicom Class Definition
G_DEFINE_TYPE_WITH_PRIVATE(AtDicom_base    , at_dicom_base    , G_TYPE_OBJECT)
G_DEFINE_TYPE_WITH_PRIVATE(AtDicom_uint8_t , at_dicom_uint8_t , AT_TYPE_DICOM_base)
G_DEFINE_TYPE_WITH_PRIVATE(AtDicom_uint16_t, at_dicom_uint16_t, AT_TYPE_DICOM_base)

// Dicom Dispose
static void
at_dicom_base_dispose(GObject* object){
  G_OBJECT_CLASS(at_dicom_base_parent_class)->dispose(object);
}

#define AT_DEFINE_DICOM_DISPOSE(type) \
static void \
at_dicom_##type##_dispose(GObject* object){\
  AtDicom_##type##Private* priv = at_dicom_##type##_get_instance_private((AtDicom_##type *)(object));\
  if(priv->image) g_clear_object(&priv->image);\
  if(priv->luts)  g_clear_object(&priv->image);\
  G_OBJECT_CLASS(at_dicom_##type##_parent_class)->dispose(object);\
}\

AT_DEFINE_DICOM_DISPOSE(uint8_t)
AT_DEFINE_DICOM_DISPOSE(uint16_t)

// Dicom Finalize
static void
at_dicom_base_finalize(GObject* object){
  AtDicom_basePrivate* priv = at_dicom_base_get_instance_private(AT_DICOM_base(object));
  g_free(priv->modality);
  g_free(priv->unit);
  g_free(priv->photo_interpretation);
  G_OBJECT_CLASS(at_dicom_base_parent_class)->dispose(object);
}

// Dicom Init
static void
at_dicom_base_init(AtDicom_base *self){
  AtDicom_basePrivate* priv = at_dicom_base_get_instance_private(self);
  priv->modality = NULL;
  priv->filename = NULL;
  priv->unit = NULL;
  priv->intercept = 0;
  priv->slope = 0;
  priv->pixel_data_tag_found = 0;
  priv->samples_per_pixel = 0;
  priv->bits_allocated = 0;
  priv->photo_interpretation = NULL;
  priv->pixel_representation = 0;\
}

#define AT_DEFINE_DICOM_INIT(type) \
static void \
at_dicom_##type##_init(AtDicom_##type *self){\
  AtDicom_##type##Private* priv = at_dicom_##type##_get_instance_private(self);\
  priv->luts = NULL;\
  priv->image = NULL;\
}

AT_DEFINE_DICOM_INIT(uint8_t)
AT_DEFINE_DICOM_INIT(uint16_t)


#define AT_DEFINE_DICOM_CLASS_INIT(type) \
static void \
at_dicom_##type##_class_init(AtDicom_##type##Class *klass){\
  GObjectClass* object_class = G_OBJECT_CLASS(klass);\
  object_class->dispose = at_dicom_##type##_dispose;\
}
AT_DEFINE_DICOM_CLASS_INIT(uint8_t)
AT_DEFINE_DICOM_CLASS_INIT(uint16_t)

// Dicom Class Init
static void
at_dicom_base_class_init(AtDicom_baseClass *klass){
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = at_dicom_base_finalize;
  object_class->dispose = at_dicom_base_dispose;
}

// Dicom New
#define AT_DEFINE_DICOM_NEW(type) \
static AtDicom_##type *\
at_dicom_##type##_new(){\
  return g_object_new(at_dicom_##type##_get_type(), NULL);\
}
AT_DEFINE_DICOM_NEW(uint8_t)
AT_DEFINE_DICOM_NEW(uint16_t)


// Dicom get data
/**
 * @brief Read `num_bytes` bytes from `file` and put to `variable`, and update
 *             byte counter (`location`)
 * @param num_bytes
 * @param variable
 * @param location
 * @param file
 */
static void
at_dicom_get_bytes(size_t num_bytes, void* variable, size_t* location, FILE* file){
  fread(variable,1,num_bytes,file);
  (*location) += num_bytes;
}

/**
 * @brief particular version of at_dicom_get_bytes for 2 bytes (little or big
 *        endian)
 * @param variable
 * @param location
 * @param file
 * @param little_endian
 */
static void
at_dicom_get_uint16(uint16_t* variable, size_t* location, FILE* file, uint8_t little_endian){
  uint8_t b[2];
  at_dicom_get_bytes(2, b, location, file);
  uint16_t b1 = (uint16_t)b[1];
  uint16_t b0 = b[0];
  if(little_endian)
    *variable = b1 << 8 | b0;
  else
    *variable = b0 << 8 | b1;
}

/**
 * @brief particular version of at_dicom_get_bytes for 4 bytes (little or big
 *        endian)
 * @param variable
 * @param location
 * @param file
 * @param little_endian
 */
static void
at_dicom_get_uint32(uint32_t* variable, size_t* location, FILE* file, uint8_t little_endian){
  uint8_t b[4];
  at_dicom_get_bytes(4, b, location, file);
  if(little_endian)
    *variable = (uint32_t)(b[3] << 24)|(uint32_t)(b[2] << 16) | (uint32_t)(b[1] << 8) | b[0];
  else
    *variable = (uint32_t)(b[0] << 24)|(uint32_t)(b[1] << 16) | (uint32_t)(b[2] << 8) | b[3];
}


/**
 * @brief The tags have variable length, but defined at the file. Let's get how
 *        the tag value is
 * @param variable
 * @param location
 * @param file
 * @param little_endian
 */
static void
at_dicom_get_length(uint32_t* variable, size_t* location, FILE* file, uint8_t little_endian){
  uint8_t b[4];
  uint32_t b3, b2;
  uint16_t vr;
  at_dicom_get_bytes(4, b, location, file);
  vr = ((uint16_t)b[0] << 8) | b[1];
  switch(vr){
  case AT_DICOM_VR_OB:
  case AT_DICOM_VR_OW:
  case AT_DICOM_VR_SQ:
  case AT_DICOM_VR_UN:
  case AT_DICOM_VR_UT:
    if((b[2] == 0) || (b[3] == 0)){
      at_dicom_get_uint32(variable, location, file, little_endian);
    }else{
      vr = AT_DICOM_IMPLICIT_VR;
      if(little_endian)
        *variable = (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0];
      else
        *variable = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
    }
    break;
  case AT_DICOM_VR_AE:
  case AT_DICOM_VR_AS:
  case AT_DICOM_VR_AT:
  case AT_DICOM_VR_CS:
  case AT_DICOM_VR_DA:
  case AT_DICOM_VR_DS:
  case AT_DICOM_VR_DT:
  case AT_DICOM_VR_FD:
  case AT_DICOM_VR_FL:
  case AT_DICOM_VR_IS:
  case AT_DICOM_VR_LO:
  case AT_DICOM_VR_LT:
  case AT_DICOM_VR_PN:
  case AT_DICOM_VR_SH:
  case AT_DICOM_VR_SL:
  case AT_DICOM_VR_SS:
  case AT_DICOM_VR_ST:
  case AT_DICOM_VR_TM:
  case AT_DICOM_VR_UI:
  case AT_DICOM_VR_UL:
  case AT_DICOM_VR_US:
  case AT_DICOM_VR_QQ:
  case AT_DICOM_VR_RT:
    b3 = (uint32_t)b[3];
    b2 = (uint32_t)b[2];
    if(little_endian)
      *variable = (b3 << 8) | b2;
    else
      *variable = (b2 << 8) | b3;
    break;
  default:
    vr = AT_DICOM_IMPLICIT_VR;
    if(little_endian)
      *variable = (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0];
    else
      *variable = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
    break;
  }
}

static void
at_dicom_uint8_t_read_pixels(AtDicom_uint8_t* dicom, size_t offset, FILE* file){
  AtDicom_uint8_tPrivate* priv         = at_dicom_uint8_t_get_instance_private(dicom);
  AtDicom_basePrivate*    priv_parent  = at_dicom_base_get_instance_private(AT_DICOM_base(dicom));
  uint8_t*                data         = at_array_uint8_t_get_data(priv->image);
  size_t                  num_bytes    = at_array_get_num_bytes(priv->image);
  uint64_t                num_elements = at_array_get_num_elements(priv->image);

  uint64_t                i;
  uint32_t                pixel32;

  // Read all file data and put to array
  fseek(file, offset, SEEK_SET);
  fread(data, 1, num_bytes, file);

  if(priv_parent->samples_per_pixel == 1){
    for(i = 0; i < num_elements; i++){
      pixel32 = (uint32_t)data[i] * priv_parent->slope + priv_parent->intercept;
      if(strstr(priv_parent->photo_interpretation,"MONOCHROME1"))
        pixel32 = 255 - pixel32;
      data[i] = (uint8_t)pixel32;
    }
  }
}

static void
at_dicom_uint16_t_read_pixels(AtDicom_uint16_t* dicom, size_t offset, FILE* file){
  AtDicom_uint16_tPrivate* priv         = at_dicom_uint16_t_get_instance_private(dicom);
  AtDicom_basePrivate*     priv_parent  = at_dicom_base_get_instance_private(AT_DICOM_base(dicom));
  uint16_t*                data         = at_array_uint16_t_get_data(priv->image);
  int16_t*                 data_s       = (uint16_t*) data;
  uint8_t*                 data_8       = (uint8_t*)  data;
  size_t                   num_bytes    = at_array_get_num_bytes(priv->image);
  uint64_t                 num_elements = at_array_get_num_elements(priv->image);

  uint64_t                i , i2;
  uint8_t                 b0, b1;
  uint32_t                pixel32;
  uint16_t                unsigned_s;
  uint8_t                 signed_image = FALSE;
  int16_t                 signed_s;

  // Read all file data and put to array
  fseek(file, offset, SEEK_SET);
  fread(data, 1, num_bytes, file);

  if(priv_parent->samples_per_pixel == 1){
    for(i = 0; i < num_elements; i++){
      // If unsigned image
      if(!priv_parent->pixel_representation){
        i2 = i << 1;
        b0 = data_8[i2];
        b1 = data_8[i2+1];
        unsigned_s = ((uint16_t) b1 << 8) | b0;
        pixel32 = (uint32_t) unsigned_s * priv_parent->slope + priv_parent->intercept;
        if(strstr(priv_parent->photo_interpretation, "MONOCHROME1"))
          pixel32 = 0xFFFF - pixel32;
      // If signed image
      }else{
        signed_s   = data_s[i];
        pixel32 = (uint32_t) signed_s * priv_parent->slope + priv_parent->intercept;
        if(strstr(priv_parent->photo_interpretation, "MONOCHROME1"))
          pixel32 = 0xFFFF - pixel32;
      }
      // As we use 12 bits, we don't worry to stack overflows
      // So, we use int16.
      data_s[i] = (int16_t) pixel32;

      // We flag the need for normalization if it's signed
      if(data_s[i] < 0)
        signed_image = TRUE;
    }
    // Turn everything to positive if signed image
    if(signed_image){
      for(i = 0; i < num_elements; i++){
        data[i] = (uint16_t)(data_s[i] - INT16_MIN);
      }
    }
  }
}

/**
 * @brief Remove initial and final space.
 *
 * Get the inital location with proper content and the end.
 * Move these content to start of pointer and delete the remaining memory
 *
 * [_,_,a,b,c,d,e,_,_,_]
 * =>
 * [a,b,c,d,e]
 *
 * @param str
 * @return
 */
static char*
trim_in_place(char* str){
  char* beg = str;
  char* end = beg + strlen(beg) - 1;

  while(isspace(*beg)) ++beg;
  while(end >= beg && isspace(*end)) --end;
  end[1] = 0;
  return memmove(str,beg, end - beg + 2);
}

static int
at_dicom_read_common(AtDicom_basePrivate* data, size_t* offset, uint64_t* size, FILE** file_external, const char* filename){
  char     dicm_id[5];
  char*    s                          = NULL;
  char*    scale                      = NULL;
  char*    scale_index                = NULL;
  char*    spacing                    = NULL;
  char*    center                     = NULL;
  char*    center_index               = NULL;
  char*    window_width_str           = NULL;
  char*    window_width_index         = NULL;
  char*    slope_str                  = NULL;
  char*    intercept_str              = NULL;
  uint8_t  decoding_tags              = TRUE;
  uint8_t  little_endian              = 1;
  uint8_t  big_endian_transfer_syntax = 0;
  uint8_t  odd_locations;
  uint8_t  in_sequence                = 0;
  uint16_t group_word;
  uint16_t planar_configuration;
  uint16_t element_word;
  uint16_t width;
  uint16_t height;
  uint32_t tag;
  uint32_t element_length;
  uint32_t n_images;
  size_t   location = 0;
  float    window_center;
  float    window_width;
  double   xscale;
  double   yscale;
  double   pixel_width;
  double   pixel_height;
  double   pixel_depth;


  *file_external = fopen(filename, "rb");
  FILE* file = *file_external;
  if(file){
    data->filename = filename;
    data->slope = 1;
    data->intercept = 0;
    data->unit = "mm";

    fseek(file, AT_DICM_OFFSET, SEEK_SET);
    data->bits_allocated = 16;
    location += AT_DICM_OFFSET;
    at_dicom_get_bytes(4, dicm_id, &location, file);
    dicm_id[4] = 0;
    if(strcmp(dicm_id, "DICM") == 0){
      while(decoding_tags){
        at_dicom_get_uint16(&group_word, &location, file, little_endian);
        if(group_word == 0x0800 && big_endian_transfer_syntax){
          little_endian = 0;
          group_word = 0x0008;
        }
        at_dicom_get_uint16(&element_word, &location, file, little_endian);
        tag = (group_word << 16) | element_word;
        at_dicom_get_length(&element_length, &location, file, little_endian);
        // Hack to read some GE files
        if(element_length == 13 && !odd_locations)
          element_length = 10;

        // "Undefined" element length
        // This is a sort of bracket that encloses a sequence of
        // elements
        if(element_length == -1){
          element_length = 0;
          in_sequence    = 1;
        }

        if(location & 1) odd_locations = 1;
        if(in_sequence){
          //at_dicom_add_tag_info(tag,NULL);
          in_sequence = 0;
          continue;
        }

        // Analyse tag
        switch(tag){
        case AT_DICOM_TAG_TRANSFER_SYNTAX_UID:
          s = malloc(element_length + 1);
          at_dicom_get_bytes(element_length, s, &location, file);
          if(strstr(s,"1.2.4") || strstr(s,"1.2.5")){
            fclose(file);
            free(s);
            return FALSE;
          }
          if(strstr(s,"1.2.840.10008.1.2.2"))
            big_endian_transfer_syntax = 1;
          free(s);
          break;
        case AT_DICOM_TAG_MODALITY:
          data->modality = malloc(element_length + 1);
          at_dicom_get_bytes(element_length, data->modality, &location, file);
          break;
        case AT_DICOM_TAG_NUMBER_OF_FRAMES:
          s = malloc(element_length + 1);
          at_dicom_get_bytes(element_length, s, &location, file);
          double frames = strtof(s, NULL);
          if(frames > 1.0)
            n_images = (uint32_t) frames;
          free(s);
          break;
        case AT_DICOM_TAG_SAMPLES_PER_PIXEL:
          at_dicom_get_uint16(&data->samples_per_pixel, &location, file, little_endian);
          break;
        case AT_DICOM_TAG_PHOTOMETRIC_INTERPRETATION:
          data->photo_interpretation = malloc(element_length + 1);
          at_dicom_get_bytes(element_length, data->photo_interpretation, &location, file);
          data->photo_interpretation = trim_in_place(data->photo_interpretation);
          break;
        case AT_DICOM_TAG_PLANAR_CONFIGURATION:
          at_dicom_get_uint16(&planar_configuration, &location, file, little_endian);
          break;
        case AT_DICOM_TAG_ROWS:
          at_dicom_get_uint16(&height, &location, file, little_endian);
          break;
        case AT_DICOM_TAG_COLUMNS:
          at_dicom_get_uint16(&width, &location, file, little_endian);
          break;
        case AT_DICOM_TAG_PIXEL_SPACING:
          scale = malloc(element_length + 1);
          at_dicom_get_bytes(element_length, scale, &location, file);
          xscale = yscale = 0;
          scale_index = strstr(scale, "\\");
          if(scale_index - scale > 1){
            yscale = strtof(scale, NULL);
            xscale = strtof(scale_index+1,NULL);
          }
          if(xscale != 0.0 && yscale != 0.0){
            pixel_width = xscale;
            pixel_height = yscale;
            data->unit = "mm";
          }
          free(scale);
          break;
        case AT_DICOM_TAG_SLICE_THICKNESS:
        case AT_DICOM_TAG_SLICE_SPACING:
          spacing = malloc(element_length + 1);
          at_dicom_get_bytes(element_length, spacing, &location, file);
          pixel_depth = strtof(spacing, NULL);
          free(spacing);
          break;
        case AT_DICOM_TAG_BITS_ALLOCATED:
          at_dicom_get_uint16(&data->bits_allocated, &location, file, little_endian);
          break;
        case AT_DICOM_TAG_PIXEL_REPRESENTATION:
          at_dicom_get_uint16(&data->pixel_representation, &location, file, little_endian);
          break;
        case AT_DICOM_TAG_WINDOW_CENTER:
          center = malloc(element_length + 1);
          at_dicom_get_bytes(element_length, center, &location, file);
          center_index = strstr(center, "\\");
          if(center_index) center = center_index;
          window_center = strtof(center, NULL);
          free(center);
          break;
        case AT_DICOM_TAG_WINDOW_WIDTH:
          window_width_str = malloc(element_length + 1);
          at_dicom_get_bytes(element_length, window_width_str, &location, file);
          if(window_width_index) window_width_str = window_width_index;
          window_width = strtof(window_width_str, NULL);
          free(window_width_str);
          break;
        case AT_DICOM_TAG_RESCALE_INTERCEPT:
          intercept_str = malloc(element_length + 1);
          at_dicom_get_bytes(element_length, intercept_str, &location, file);
          data->intercept = strtof(intercept_str, NULL);
          free(intercept_str);
          break;
        case AT_DICOM_TAG_RESCALE_SLOPE:
          slope_str = malloc(element_length + 1);
          at_dicom_get_bytes(element_length, slope_str, &location, file);
          data->intercept = strtof(slope_str, NULL);
          free(slope_str);
          break;
        case AT_DICOM_TAG_RED_PALETTE:
          break;
        case AT_DICOM_TAG_GREEN_PALETTE:
          break;
        case AT_DICOM_TAG_BLUE_PALETTE:
          break;
        case AT_DICOM_TAG_PIXEL_DATA:
          if(element_length){
            *offset = location;
            decoding_tags = 0;
          }
          data->pixel_data_tag_found = 1;
          size[0] = height;
          size[1] = width;
          size[2] = data->samples_per_pixel;
          break;
         default:
          fseek(file, element_length, SEEK_CUR);
          location += element_length;
          break;
        }
      }
      return TRUE;
    }
  }
  return FALSE;
}



/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
void
at_dicom_uint8_t_read(AtDicom_uint8_t **dicom, const char *filename){
  AtDicom_basePrivate data;
  uint64_t size[3];
  size_t offset;
  FILE* file = NULL;
  // If Success
  if(at_dicom_read_common(&data,&offset,size,&file, filename) == 1){
    g_set_object(dicom,at_dicom_uint8_t_new());
    AtDicom_uint8_tPrivate* priv = at_dicom_uint8_t_get_instance_private(*dicom);
    at_array_new(&priv->image,3, size);

    at_dicom_uint8_t_read_pixels(*dicom,offset,file);
  }
  fclose(file);
}
void
at_dicom_uint16_t_read(AtDicom_uint16_t **dicom, const char *filename){
  AtDicom_basePrivate data;
  uint64_t size[3];
  size_t offset;
  FILE* file = NULL;
  // If Success
  if(at_dicom_read_common(&data,&offset,size,&file, filename) == 1){
    g_set_object(dicom,at_dicom_uint16_t_new());
    AtDicom_uint16_tPrivate* priv = at_dicom_uint16_t_get_instance_private(*dicom);
    AtDicom_basePrivate* priv_base = at_dicom_base_get_instance_private(AT_DICOM_base(*dicom));
    memcpy(priv_base, &data, sizeof(AtDicom_basePrivate));
    at_array_new(&priv->image,3, size);

    at_dicom_uint16_t_read_pixels(*dicom,offset,file);
  }
  if(file)fclose(file);
}

char*
at_dicom_base_get_filename(AtDicom_base* dicom){
  AtDicom_basePrivate *priv = at_dicom_base_get_instance_private(dicom);
  return g_strdup(priv->filename);
}
char*
at_dicom_base_get_modality(AtDicom_base* dicom){
  AtDicom_basePrivate *priv = at_dicom_base_get_instance_private(dicom);
  return g_strdup(priv->modality);
}

uint8_t
at_dicom_base_get_pixel_data_tag_found(AtDicom_base* dicom){
  AtDicom_basePrivate *priv = at_dicom_base_get_instance_private(dicom);
  return priv->pixel_data_tag_found;
}

char*
at_dicom_base_get_unit(AtDicom_base* dicom){
  AtDicom_basePrivate *priv = at_dicom_base_get_instance_private(dicom);
  return g_strdup(priv->unit);
}


uint16_t
at_dicom_uint8_t_get_dim(AtDicom_uint8_t* dicom){
  AtDicom_uint8_tPrivate* priv = at_dicom_uint8_t_get_instance_private(dicom);
  return at_array_get_dim(priv->image);
}
uint16_t
at_dicom_uint16_t_get_dim(AtDicom_uint16_t* dicom){
  AtDicom_uint16_tPrivate* priv = at_dicom_uint16_t_get_instance_private(dicom);
  return at_array_get_dim(priv->image);
}

size_t
at_dicom_uint8_t_get_elemsize(AtDicom_uint8_t* dicom){
  AtDicom_uint8_tPrivate* priv = at_dicom_uint8_t_get_instance_private(dicom);
  return at_array_get_elemsize(priv->image);
}
size_t
at_dicom_uint16_t_get_elemsize(AtDicom_uint16_t* dicom){
  AtDicom_uint16_tPrivate* priv = at_dicom_uint16_t_get_instance_private(dicom);
  return at_array_get_elemsize(priv->image);
}

uint64_t*
at_dicom_uint8_t_get_size(AtDicom_uint8_t* dicom){
  AtDicom_uint8_tPrivate* priv = at_dicom_uint8_t_get_instance_private(dicom);
  return at_array_get_size(priv->image);
}
uint64_t*
at_dicom_uint16_t_get_size(AtDicom_uint16_t* dicom){
  AtDicom_uint16_tPrivate* priv = at_dicom_uint16_t_get_instance_private(dicom);
  return at_array_get_size(priv->image);
}
