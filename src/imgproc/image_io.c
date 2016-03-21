#include <at/imgproc.h>
#include <string.h>
#include <stdlib.h>
#include <png.h>
#include <jpeglib.h>

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
struct my_error_mgr{
  struct jpeg_error_mgr pub;
  jmp_buf               setjmp_buffer;
};
typedef struct my_error_mgr* my_error_ptr;

METHODDEF(void)
my_error_exit(j_common_ptr cinfo){
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
  longjmp(myerr->setjmp_buffer, 1);
}



static void abort_(const char * s, ...)
{
  va_list args;
  va_start(args, s);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
  abort();
}

static void at_image_read_pgm_skip_comments(char* s, size_t m, FILE* fp){
  while(fgets(s,m,fp) != NULL)
    if(s[0]!='#' && s[0]!='\n') break;
}


static void at_image_read_png(AtArray_uint8_t** array, const char* filename){
  png_structp png_ptr;
  png_infop info_ptr;
  unsigned char header[8];
  FILE* infile = fopen(filename, "rb");
  if(!infile)
    abort_("[read_png_file] File %s could not be opened for reading", filename);

  fread(header, 1, 8, infile);
  if(png_sig_cmp(header, 0, 8))
    abort_("[read_png_file] File %s is not recognized as a PNG file", filename);

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png_ptr)
    abort_("[read_png_file] png_create_read_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr)
    abort_("[read_png_file] png_create_info_struct failed");

  if(setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during init_io");
  png_init_io(png_ptr, infile);

  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);

  uint64_t size[3];
  size[0] = png_get_image_height(png_ptr, info_ptr);
  size[1] = png_get_image_width(png_ptr, info_ptr);
  size[2] = png_get_channels(png_ptr, info_ptr);
  //uint8_t bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  at_array_new(array, 3, size);

  png_read_update_info(png_ptr, info_ptr);

  // Read file
  if(setjmp(png_jmpbuf(png_ptr))) abort_("[read_png_file] Error during read_image");
  uint8_t**buffer   = (uint8_t**)malloc(sizeof(uint8_t*) * size[0]);
  size_t row_stride = png_get_rowbytes(png_ptr, info_ptr);
  uint32_t i;
  uint8_t* data = at_array_uint8_t_get_data(*array);
  for(i = 0; i < size[0]; i++) buffer[i] = data + row_stride * i;
  png_read_image(png_ptr, buffer);
  fclose(infile);
  free(buffer);
}

void at_image_read_jpg(AtArray_uint8_t** array, const char* filename){
  struct    jpeg_decompress_struct cinfo;
  struct    my_error_mgr jerr;
  FILE*     infile;
  uint8_t** buffer; // 2D Pointers to rows
  int       row_stride;
  if((infile = fopen(filename, "rb")) == NULL){
    abort_("[read_png_file] File %s could not be opened for reading", filename);
  }
  cinfo.err           = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if(setjmp(jerr.setjmp_buffer)){
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return;
  }
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);
  uint64_t size[3] = {cinfo.output_height, cinfo.output_width, cinfo.output_components};
  at_array_new(array, 3, size);
  row_stride       = size[1]*size[2];
  buffer           = (uint8_t**)malloc(sizeof(uint8_t*) * size[0]);
  uint32_t i;
  uint8_t* data = at_array_uint8_t_get_data(*array);
  for(i = 0; i < size[0]; i++) buffer[i] = data + row_stride * i;
  i = 0;
  while(cinfo.output_scanline < cinfo.output_height){
    jpeg_read_scanlines(&cinfo, &buffer[i++], 1);
  }
  jpeg_finish_decompress(&cinfo);
  fclose(infile);
  free(buffer);
}

static void at_image_read_ppm_pgm(AtArray_uint8_t** array, const char* filename){
  char version[4];
  char line[256];
  uint64_t i;
  uint32_t width, height;
  uint16_t dim;
  uint64_t max_gray;
  FILE* fp     = fopen(filename,"rb");

  fgets(version, sizeof(version), fp);
  if(version[0] == 'P' && (version[1] == '5' || version[1] == '2' || version[1] == '6')){
    if(version[1] == '6') dim = 3;
    else                  dim = 2;
    at_image_read_pgm_skip_comments(line,256,fp);
    sscanf(line, "%d %d\n", &width, &height);
    at_image_read_pgm_skip_comments(line,256,fp);
    sscanf(line, "%lu", &max_gray);
    uint64_t size[3] = {height, width,3};
    at_array_new(array,dim, size);
    uint8_t* data = at_array_uint8_t_get_data(*array);
    uint64_t num_elements = at_array_get_num_elements(*array);
    if(version[1] == '2')
      for(i = 0; i < num_elements; i++)
        fscanf(fp, "%cu", &data[i]);
    else
      fread(data, sizeof(uint8_t), num_elements,fp);
  }
  fclose(fp);
}

static void at_image_read_pgm(AtArray_uint8_t** array, const char* filename){
  at_image_read_ppm_pgm(array, filename);
}
static void at_image_read_ppm(AtArray_uint8_t** array, const char* filename){
  at_image_read_ppm_pgm(array, filename);
}


void at_image_write_png(AtArray_uint8_t* array, const char* filename){
  /* create file */
  FILE *outfile = fopen(filename, "wb");
  if (!outfile)
    abort_("[write_png_file] File %s could not be opened for writing", filename);
  /* initialize stuff */
  png_structp png_ptr;
  png_infop info_ptr;
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
    abort_("[write_png_file] png_create_write_struct failed");
  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    abort_("[write_png_file] png_create_info_struct failed");
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during init_io");
  png_init_io(png_ptr, outfile);

  /* write header */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during writing header");

  uint16_t dim = at_array_get_dim(array);
  g_autofree uint64_t* size = at_array_get_size(array);
  g_autofree uint64_t* step = at_array_get_step(array);
  size_t elemsize = at_array_get_elemsize(array);
  uint8_t* data = at_array_uint8_t_get_data(array);

  int color_type = (dim > 2 && size[2] == 3)?PNG_COLOR_TYPE_RGB:PNG_COLOR_TYPE_GRAY;
  png_set_IHDR(png_ptr, info_ptr, size[1], size[0],
               elemsize << 3, color_type, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  /* write bytes */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during writing bytes");

  g_autofree uint8_t**buffer   = (uint8_t**)malloc(sizeof(uint8_t*) * size[0]);
  uint32_t i;
  for(i = 0; i < size[0]; i++) buffer[i] = data + step[0] * i * elemsize;
  png_write_image(png_ptr, buffer);

  /* end write */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during end of write");

  png_write_end(png_ptr, NULL);

  /* cleanup heap allocation */
  fclose(outfile);
}
void at_image_write_jpg(AtArray_uint8_t* array, const char* filename){
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE* outfile;
  uint8_t* row_pointer[1];
  uint8_t* data = at_array_uint8_t_get_data(array);
  g_autofree uint64_t* size = at_array_get_size(array);
  g_autofree uint64_t* step = at_array_get_step(array);
  uint16_t dim = at_array_get_dim(array);

  row_pointer[0] = data;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  if((outfile = fopen(filename, "wb"))==NULL){
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }
  jpeg_stdio_dest(&cinfo, outfile);
  cinfo.image_width = size[1];
  cinfo.image_height = size[0];
  cinfo.input_components = (dim > 2)?size[2]:1;
  cinfo.in_color_space = (dim > 2 && size[2] == 3)?JCS_RGB:JCS_GRAYSCALE;
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, 90, TRUE);
  jpeg_start_compress(&cinfo, TRUE);
  while(cinfo.next_scanline < size[0]){
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
    row_pointer[0] += step[0];
  }
  jpeg_finish_compress(&cinfo);
  fclose(outfile);
  jpeg_destroy_compress(&cinfo);
}

static void _at_image_write_ppm_pgm(AtArray_uint8_t* array, const char* filename, char format){
  FILE* fp;
  fp = fopen(filename, "wb");
  uint8_t* data = at_array_uint8_t_get_data(array);
  g_autofree uint64_t* size = at_array_get_size(array);
  uint64_t num_elements = at_array_get_num_elements(array);
  uint8_t array_max = 0;
  at_array_max(array, &array_max);
  fprintf(fp, "P%d\n",format);
  fprintf(fp,"%d %d\n", size[1], size[0]);
  fprintf(fp, "%d\n",array_max);
  fwrite(data,sizeof(uint8_t),num_elements,fp);
  fclose(fp);
}

static void   at_image_write_pgm(AtArray_uint8_t* array, const char* filename){
  _at_image_write_ppm_pgm(array, filename, 5);
}

static void   at_image_write_ppm(AtArray_uint8_t* array, const char* filename){
  _at_image_write_ppm_pgm(array, filename, 6);
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
void at_image_read(AtArray_uint8_t** array, const char* filename){
  const char* ext = strrchr(filename, '.') + 1;
  if     (strcasecmp(ext, "png") == 0) at_image_read_png(array, filename);
  else if(strcasecmp(ext, "jpg") == 0) at_image_read_jpg(array, filename);
  else if(strcasecmp(ext, "pgm") == 0) at_image_read_pgm(array, filename);
  else if(strcasecmp(ext, "ppm") == 0) at_image_read_ppm(array, filename);
}

void at_image_write(AtArray_uint8_t* array, const char* filename){
  const char* ext = strrchr(filename, '.') + 1;
  if     (strcasecmp(ext, "png") == 0) at_image_write_png(array,filename);
  else if(strcasecmp(ext, "jpg") == 0) at_image_write_jpg(array,filename);
  else if(strcasecmp(ext, "pgm") == 0) at_image_write_pgm(array,filename);
  else if(strcasecmp(ext, "ppm") == 0) at_image_write_ppm(array,filename);
}
