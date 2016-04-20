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

int main(int argc, char** argv){

  // Criar a estrutura
  AtORFCDemoInfo info;
  init_info(&info);

  // Ler o círculo
  //at_image_read(&info.original, "trekkie-nerdbw.png");
  //at_image_read(&info.original, "MRI_blackandwhite.png");
  at_image_read(&info.original, "circleborder.png");

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
    uint16_t k_index_si = at_array_get(info.component, info.seeds_packed->indices[0]);
    uint16_t k_index;
    uint64_t* component_size = at_array_get_size(info.component);
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

    at_array_mult(labels,255);
    at_imageviewer_show_uint8_t(info.imageviewer, labels, AT_COLOR_GRAY);
    at_imageviewer_waitkey();

    uint8_t k_index_si = at_array_get(labels, info.seeds_packed->indices[0]);
    g_autofree uint64_t* component_size = at_array_get_size(labels);
    uint8_t k_index;
#endif

    // Get component index of object seed
    //uint64_t k_index_si = at_array_get(info.component, info.seeds_packed->indices[0]);

    // Creating an image of the component
    uint64_t x, y, k, k_rgb;
    uint8_t component_color[4] = {255,255,255,255};
    uint64_t component_rgb_size[3] = {component_size[0], component_size[1], 4};

    at_array_zeros(&info.component_rgb, 3, component_rgb_size);

    for(y = 0, k = 0, k_rgb = 0; y < component_size[0]; y++){
      for(x = 0; x < component_size[1]; x++, k++){
#ifdef COMPONENT
       k_index = at_array_get(info.component, k);
#else
       k_index = at_array_get(labels, k);
#endif
        for(i = 0; i < 4; i++, k_rgb++){
          if(k_index == k_index_si){
            at_array_set(info.component_rgb, k_rgb, component_color[i]);
          }
        }
      }
    }

    at_imageviewer_show_uint8_t(info.imageviewer, info.component_rgb, AT_COLOR_BGRA);
    at_imageviewer_waitkey();

  }

  destroy_info(&info);
  return 0;
}
