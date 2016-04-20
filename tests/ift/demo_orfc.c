#include <at/gui.h>
#include <at/ift.h>
#include <stdlib.h>


// PACKED SET -----------------------------------------------------------------
typedef struct _AtSeedPackedSet{
  uint64_t  num_elements;
  uint64_t  num_allocated;
  uint64_t* indices;
  uint64_t* labels;
}AtSeedPackedSet;

void
at_seeds_init(AtSeedPackedSet* seeds){
  seeds->num_elements  = 0;
  seeds->indices       = NULL;
  seeds->labels        = NULL;
  seeds->num_allocated = 0;
}
void
at_seeds_destroy(AtSeedPackedSet* seeds){
  g_free(seeds->indices);
  g_free(seeds->labels);
  g_free(seeds);
}

void
at_seeds_add_seed(AtSeedPackedSet* seeds, uint64_t index, uint64_t label){
  if(seeds->num_allocated == 0)
    seeds->num_allocated = 1;
  else if(seeds->num_elements == seeds->num_allocated)
    seeds->num_allocated <<= 1;

  seeds->indices = g_realloc(seeds->indices, sizeof(uint64_t) * seeds->num_allocated);
  seeds->labels  = g_realloc(seeds->labels , sizeof(uint64_t) * seeds->num_allocated);
  seeds->indices[seeds->num_elements] = index;
  seeds->labels [seeds->num_elements] = label;
  seeds->num_elements++;
}

AtSeedPackedSet*
at_seeds_pack(AtArray_uint8_t* seeds){
  AtSeedPackedSet* seeds_packed = g_malloc(sizeof(AtSeedPackedSet));
  at_seeds_init(seeds_packed);
  uint64_t num_elements = at_array_get_num_elements(seeds);
  uint64_t seed_index;
  uint8_t seed_label;
  for(seed_index = 0; seed_index < num_elements; seed_index++){
    seed_label = at_array_get(seeds,seed_index);

    if(seed_label != 0){
      if(seed_label != 1)
        at_seeds_add_seed(seeds_packed, seed_index, 0);
      else
        at_seeds_add_seed(seeds_packed, seed_index, seed_label);
    }
  }
  return seeds_packed;
}
AtArray_uint64_t*
at_seeds_packed_to_array(AtSeedPackedSet* seeds){
  AtArray(uint64_t)* array = NULL;
  uint64_t size[2] = {seeds->num_elements, 2};
  at_array_new(&array, 2, size);
  uint64_t offset;
  uint64_t i;
  for(i = 0; i < seeds->num_elements; i++){
    offset = i<<1;
    at_array_set(array, offset  , seeds->indices[i]);
    at_array_set(array, offset+1, seeds->labels[i]);
  }
  return array;
}

// END PACKED SET -------------------------------------------------------------


typedef struct AtORFCDemoInfo{
  AtSeedPackedSet    * seeds_packed;
  AtArray(uint64_t)  * seeds_packed_array;
  AtArray(uint16_t)  * component;
  AtArray(uint8_t)   * original;
  AtArray(uint8_t)   * original_rgb;
  AtArray(uint8_t)   * buffer;
  AtArray(uint8_t)   * component_rgb;
  AtArray(uint8_t)   * seeds;
  AtIFTArray(uint8_t)* ift;
  AtImageViewer      * imageviewer;

  uint8_t              object_color    [3];
  uint8_t              background_color[3];

  gboolean             mouse_pressed;

}AtORFCDemoInfo;

void
init_info(AtORFCDemoInfo* info){
  info->original           = NULL;
  info->original_rgb       = NULL;
  info->buffer             = NULL;
  info->seeds              = NULL;
  info->seeds_packed       = NULL;
  info->ift                = NULL;
  info->imageviewer        = NULL;
  info->seeds_packed_array = NULL;
  info->component          = NULL;
  info->component_rgb      = NULL;

  info->mouse_pressed      = FALSE;
}

void
destroy_info(AtORFCDemoInfo *info){
  g_clear_object(&info->original);
  g_clear_object(&info->original_rgb);
  g_clear_object(&info->buffer);
  g_clear_object(&info->seeds);
  at_seeds_destroy(info->seeds_packed);
  g_clear_object(&info->ift);
  g_clear_object(&info->seeds_packed_array);
  g_clear_object(&info->component);
  g_clear_object(&info->component_rgb);
}
static void
show_info(AtORFCDemoInfo* info){
  at_imageviewer_show_uint8_t(info->imageviewer, info->buffer, AT_COLOR_BGRA);
}

static void
at_image_set(AtArray_uint8_t* image, int x, int y, AtVec4i value){
  uint64_t indices[3] = {y, x, 0};
  uint64_t offset;
  AtVec2i center = {x,y};
  at_array_uint8_t_draw_circle(image,center,8,&value,-1,AT_LINE_TYPE_4,0);
  at_array_get_index(image, indices, &offset);


  // Get number of channels
  uint16_t dim = at_array_get_dim(image);
  uint64_t depth;
  if(dim == 2) depth = 1;
  else{
    g_autofree uint64_t* size = at_array_get_size(image);
    depth = size[2];
  }



  // Fill channels
  uint64_t i;
  for(i = 0; i < depth; i++)
    at_array_set(image, offset+i, value.data[i]);
}

static void
set_seed(AtORFCDemoInfo* info, AtMouseEventFlags flags, int x, int y){
  AtVec4i label_color;
  AtVec4i label;
  AtVec4i label_color_obj = {255,0,0,255};
  AtVec4i label_color_bkg = {0,255,0,255};
  AtVec4i label_obj = {1,1,1,1};
  AtVec4i label_bkg = {2,2,2,2};
  if(flags & AT_MOUSE_FLAG_CTRLKEY){
    label_color = label_color_bkg;
    label = label_bkg;
  }else{
    label_color = label_color_obj;
    label = label_obj;
  }

  at_image_set(info->seeds , x, y, label);
  at_image_set(info->buffer, x, y, label_color);
  show_info(info);
}



static void
imageviewer_mouse_cb(AtMouseEventType event, int x, int y, AtMouseEventFlags flags, void* user_data){
  AtORFCDemoInfo* info = (AtORFCDemoInfo*) user_data;
  switch(event){
  case AT_MOUSE_EVENT_PRESS:
    info->mouse_pressed = TRUE;
    set_seed(info, flags, x, y);
    break;
  case AT_MOUSE_EVENT_RELEASE:
    info->mouse_pressed = FALSE;
    break;
  case AT_MOUSE_EVENT_MOVE:
    if(info->mouse_pressed){
      set_seed(info, flags, x, y);
    }
    break;
  }
}

void
at_seeds_filter_by_label(AtArray_uint64_t** seeds_background,
                         AtArray_uint64_t* seeds, uint64_t label){
  g_autofree uint64_t* seeds_size = at_array_get_size(seeds);
  g_autofree uint64_t* seeds_background_data = malloc(sizeof(uint64_t) * seeds_size[0] * 2);
  uint64_t* data = at_array_uint64_t_get_data(seeds);
  uint64_t  index = 0;
  uint64_t  i;
  uint64_t num_elements = seeds_size[0] << 1;
  for(i = 0; i < num_elements; i += 2){
    if(data[i+1] == label)
    {
      seeds_background_data[index] = data[i];
      seeds_background_data[index+1] = data[i+1];
      index += 2;
    }
  }
  seeds_background_data = realloc(seeds_background_data,sizeof(uint64_t) * index);
  seeds_size[0]         = index >> 1;
  at_array_new(seeds_background,2,seeds_size,seeds_background_data);
}


static uint8_t
reduce_sum(uint8_t data1, uint8_t data2){
  return data1 + data2;
}

#define at_array_colorize(array, format) _Generic((array), \
 AtArray_uint8_t*: at_array_uint8_t_colorize, \
 AtArray_uint16_t*: at_array_uint16_t_colorize \
)(array, format)

AtArray_uint8_t*
at_array_uint8_t_colorize(AtArray_uint8_t* array, AtColorFormat format){
  uint8_t max_value        = 0;
  at_array_max(array, &max_value);

  uint8_t* colors           = g_malloc(max_value);
  uint16_t dim              = at_array_get_dim(array);
  dim = 2;
  g_autofree uint64_t* size = at_array_get_size(array);
  uint64_t num_elements     = at_array_get_num_elements(array);

  AtArray(uint8_t)* output  = NULL;
  at_array_new(&output, dim, size);

  uint8_t*  output_data     = at_array_get(output);
  uint8_t* array_data      = at_array_get(array);
  uint64_t  i;
  for(i = 0; i < max_value; i++)
  {
    colors[i] = rand() % 256;
    printf("%d ", colors[i]);
  }
  printf("\n");

  for(i = 0; i < num_elements; i++){
    output_data[i] = colors[array_data[i]];
  }
  g_free(colors);
  return output;
}
uint8_t colors[] = {
194, 181, 123, 138, 182,  59, 162,  66,  27, 179, 223, 242, 165, 243, 169, 203, 188, 189,  42,  31, 131, 232, 170,  73,  76,  95, 137, 115, 212,  25, 177, 150, 207,  44,  32, 133, 103, 194, 199, 130, 118, 167, 117,  27, 154,  30, 230,  87, 219,  17,
118,  94, 249,  33, 167,  70, 128,  49, 185,  85,  74, 107, 235,  25, 151,  12, 159, 255, 206, 102, 129,  68,  13, 246,  96, 168,  20,  70, 255, 239,  87, 117,  78,  81, 150, 245, 151,  23,  38,  80, 108, 113, 187,  87, 138,  83,  99,  41,  82,  50,
144, 211, 118, 157, 202, 214,  69, 222,  29,  68, 206, 116, 186,  28, 197,  80,  17,  92, 103,  56, 173, 211, 169, 104,  43,  51, 187, 142,  93,  13, 192, 237, 225,  55, 138, 171,  13, 208, 137,  42,  20,  87, 159, 206, 115, 100,  31, 133, 193, 134,
189, 110,  90, 102, 214, 133, 153, 146,  19, 246, 159, 212, 227, 128,  11, 110,  43,  24,  62, 181,  67,  82,  12, 226,  33, 128,  70,  64,   5,   7, 198, 194, 117,  32,  40,  76, 165, 193, 222, 185, 184, 125, 141, 155, 254, 152,   9,  41, 176,  71,
222, 243, 154, 235, 213, 187, 107,  28, 251, 112,  35, 193,  50, 153, 226,  90, 229, 135,  27, 195,  64, 211,  64, 205, 111,  62, 101, 120, 104,  22, 192,  70,   9,  90,  49, 223,  21, 156, 251,  16,  12,  30, 209,  62, 183, 179, 152, 156,  59, 180,
 95, 123, 135, 160,  73, 246, 222, 174, 111,  70, 196,  47, 141, 206, 137, 190, 173, 158,  91, 168, 174, 103, 198, 127, 166, 126,  51,  62,  26, 110, 242, 122, 233, 122,  26,  50, 112, 248, 225, 223,  63, 165,  14, 204, 115, 151, 138,  32,  53, 229,
200, 227,  77, 143,  99, 243,  13, 150,  49,  39,   4,  36, 161, 237, 158, 187,  32,  14, 180,   1, 238, 243, 166, 252, 191,  26, 148,  73,  58, 201,  47,   3, 173, 124, 146,  16, 111, 159, 166, 160, 198, 170, 196, 104, 151,  98,  35, 183, 113, 215,
184,  95, 202,  95,  91, 137, 121, 239, 211, 179, 185,   2, 182, 102, 126,  72, 118, 237, 231,  28, 141, 174, 198,  82,  22,  93, 180,  57,  21,  37,  17, 205, 132, 219,  44, 224, 101, 165, 207,  56,  89, 136,  58,  15, 238, 184,  88, 100, 165,  63,
128,  50, 237,  70, 132,   3, 164,  57,  61, 185,  94,  78, 134, 227,  41, 179, 195, 142,  88, 146, 198, 177,  27,   0, 193,   9, 184,  25, 110,  93,  88, 238, 144,  70,  53,  20,  73, 217,  77, 134, 146, 172, 212,  24, 143, 254, 203,  82, 140,  36,
228,  83, 213, 255,  83, 150,   9,  12, 175, 119, 105,   8, 101, 249,  78, 154,  14, 151, 115,  91,  30,   5,   7, 242,  30, 150, 240, 233, 232, 125,  13, 205, 208, 227, 204,  35, 121, 213,  47,  41,  76, 153,  49, 178, 146, 127,  76, 160,  22, 192,
252,  52, 197,   3,  39, 227, 154,  23, 205, 130, 148, 218,  79, 100, 189,  28, 136,  55, 241, 183,  96,  62,  80, 145, 240, 227,  16,  60, 131,  38, 252, 127,  91, 194, 131, 130, 165,  29, 153, 114, 159,  46,  77, 239, 146,  10,  11,  26,  65, 252,
210, 161,  58,  34,  50,  42,   5,  66, 103, 137, 105,  99,   8, 196,  37, 139,  70, 203, 168, 223,  61,  72,  13, 138,  55, 160, 149,  66, 186, 214,  62, 140, 120, 121, 175, 170, 163, 180, 237,  10,  61,  86, 110,  70,  26, 147, 209,  96,  94, 122,
 63, 156, 194,  77,  38, 249, 237, 187,  59, 167, 146, 121,  52,  10, 242, 227, 180, 150, 151, 161, 160, 213, 247,  14,  27,  17, 162, 236, 113,   0, 102, 177, 156,  40, 254, 195,  33, 235, 126,  92, 146,  16, 214, 198,  26, 200, 169, 207,  94,  65,
112, 255,  22, 104,  13,  49, 121, 175,  29, 235, 176, 132, 156,  76, 172, 154,  15, 206, 133, 142,  42,  23, 158,   0, 222, 185, 201, 135, 136,  39, 200, 248,  38, 222,  96,  52,  15, 218, 227,  45, 197, 147, 177,  97, 224,  93, 251, 239,  43, 128,
125,  86, 151,  28,  86, 117, 213,  31, 253,  93,  71, 197,  85, 109, 164, 182, 161, 179, 144, 133, 224,  85,  24, 145, 182, 248, 239, 177, 232,  26,  49, 101, 112, 200, 129, 199,  62,  86, 230,  59, 179,  45,   0,   9, 155, 164, 191,  60,  88,  79,
193,  56, 164, 218, 202,  90, 210, 185,  11, 186, 211,  60,  32,  68,   4, 161,  11,  66, 248, 241, 125, 171,  31, 126, 180, 186,  34, 115, 246, 122, 194, 184, 179, 102, 146, 125, 192, 100,  54, 203,  31,   9,   7,  63,  77,  12, 224,  88,  78, 216,
 74, 204, 132, 105,  74,  56,  35, 108, 172,  25, 231, 110, 209, 154, 213,  99,  23, 149, 200,  77,  97, 231,  86, 104,  38, 164, 116,   6, 252, 195, 223,  70, 143,  99, 175, 217, 155, 210,  69,  71, 236,  44, 182, 189, 198, 139,  33, 221,  32, 233,
 42, 129, 208, 129, 234, 246,  37,  94, 252,  33,  33, 219, 104, 176,  62,  23, 137, 218, 234, 207,  33, 214, 251, 215, 147, 194,  98, 180, 159, 131, 157, 202,   4, 109,  75, 238,  99, 112,  77,  96, 145, 110,  59, 249,  31, 122,  17, 168,  84, 251,
119, 117, 209, 115,  77, 100,  53, 175,  25, 212,  50, 182, 158,  55,  36, 233,  37, 135,  89, 114, 231, 235, 225,  35, 228,   0, 157, 245, 168, 241, 240,  32, 102, 193, 147, 179,  38, 200,  99,  63, 156, 149, 245,  59, 204,  25,  36, 242, 161, 126};

AtArray_uint8_t*
at_array_uint16_t_colorize(AtArray_uint16_t* array, AtColorFormat format){
  uint16_t max_value        = 0;
  at_array_max(array, &max_value);

  //uint8_t* colors           = g_malloc(max_value);
  uint16_t dim              = at_array_get_dim(array);
  g_autofree uint64_t* size = at_array_get_size(array);
  uint64_t num_elements     = at_array_get_num_elements(array);

  AtArray(uint8_t)* output  = NULL;
  at_array_new(&output, dim, size);

  uint8_t*  output_data     = at_array_get(output);
  uint16_t* array_data      = at_array_get(array);
  uint64_t  i;
//  for(i = 0; i < max_value; i++)
//  {
//    colors[i] = rand() % 256;
//    printf("%3d ", colors[i]);
//  }
//  printf("\n");

  for(i = 0; i < num_elements; i++){
    output_data[i] = colors[array_data[i] % 300];
  }
  //g_free(colors);
  return output;
}

int main(int argc, char** argv){

  // Criar a estrutura
  AtORFCDemoInfo info;
  init_info(&info);

  // Ler o círculo
  g_autoptr(AtArray(uint8_t)) array = NULL;
  //at_image_read(&array, "trekkie-nerdbw.png");
  at_image_read(&array, "MRI_blackandwhite2.png");
  //at_image_read(&array, "comics_pb.png");
  //at_image_read(&array, "monster.png");
  //at_image_read(&array, "circleborder.png");
  info.original = at_cvt_color(array, AT_COLOR_GRAY, AT_COLOR_GRAY);
  uint64_t* original_size = at_array_get_size(info.original);
  original_size[2] = 1;
  at_array_base_set_size(AT_ARRAY_BASE(info.original),3,original_size);

  // Converter para RGB
  info.original_rgb = at_cvt_color(info.original,AT_COLOR_GRAY, AT_COLOR_BGRA);

  // Duplicar para manter o original
  at_array_copy(info.original_rgb, &info.buffer);

  // Criar o mapa de sementes
  g_autofree uint64_t* image_size = at_array_get_size(info.original);
  at_array_zeros(&info.seeds, 2, image_size);

  // Abrir a janela
  gtk_init(&argc,&argv);
  info.imageviewer = at_imageviewer_new_with_name("Circle ORFC");
  at_imageviewer_set_mouse_callback(info.imageviewer, imageviewer_mouse_cb, &info);

  show_info(&info);
  at_imageviewer_waitkey();

  // Achar o núcleo
  info.seeds_packed       = at_seeds_pack(info.seeds);
  if(info.seeds_packed->indices){

    info.seeds_packed_array = at_seeds_packed_to_array(info.seeds_packed);

    uint64_t i;
    g_autofree uint64_t* component_size = NULL;

#define COMPONENT
#ifdef COMPONENT
    info.component = at_orfc_out_cut_core_array_uint8_t(
           &info.ift,
           info.original,
           2,
           AT_ADJACENCY_4,
           AT_OPTIMIZATION_MIN,
           at_connectivity(max, info.ift),
           at_weighting(abs_diff, info.ift),
           info.seeds_packed_array);
    component_size = at_array_get_size(info.component);

    AtArray(uint8_t)* component_image = at_array_uint16_t_colorize(info.component, AT_COLOR_GRAY);
    at_imageviewer_show_uint8_t(info.imageviewer,component_image,AT_COLOR_GRAY);
    at_imageviewer_waitkey();
#else
    at_ift_apply_array_uint8_t(
           &info.ift,
           info.original,
           2,
           AT_ADJACENCY_4,
           AT_OPTIMIZATION_MIN,
           at_connectivity(max, info.ift),
           at_weighting(abs_diff, info.ift),
           info.seeds_packed_array);
    AtArray_uint8_t* labels = at_ift_get_labels_uint8_t(info.ift);
    // Get component index of object seed

    g_autoptr(AtArray(uint8_t)) labels_image = at_array_colorize(labels, AT_COLOR_GRAY);
    //at_array_mult(labels,255);
    at_imageviewer_show_uint8_t(info.imageviewer, labels_image, AT_COLOR_GRAY);
    at_imageviewer_waitkey();

    uint8_t k_index_si = at_array_get(labels, info.seeds_packed->indices[0]);
    component_size = at_array_get_size(labels);
    uint8_t k_index;
#endif
  }

  destroy_info(&info);
  return 0;
}
