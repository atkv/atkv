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

#include <at/gui.h>
#include <at/ift.h>

gboolean pressed;
AtArray(uint8_t)* image = NULL;
AtArray(uint8_t)* image_seeded = NULL;
uint64_t width, height, depth;
AtImageViewer* imageviewer;
uint64_t seeds_data[1024];
uint64_t seeds_index;
uint64_t seed_type;

void
draw_point(int x, int y){
  uint64_t offset = (y*width + x)*depth;
  uint8_t r,g,b;
  if(seed_type == 0){
    r = 0; g = 255; b = 255;
  }else{
    r = 255; g = 255; b = 255;
  }
  at_array_set(image_seeded,offset,r);

  at_array_set(image_seeded,offset+1,g);

  at_array_set(image_seeded,offset+2,b);
  at_imageviewer_show_uint8_t(imageviewer,image_seeded,AT_COLOR_RGB);
}

void
add_seed(uint64_t offset, AtMouseEventFlags flags){
  seeds_data[seeds_index] = offset;
  if(flags & AT_MOUSE_FLAG_CTRLKEY){
    seeds_data[seeds_index+1] = 0;
    seed_type = 0;
  }
  else{
    seeds_data[seeds_index+1] = 255;
    seed_type = 1;
  }
  seeds_index += 2;
}

void
mouse_cb(AtMouseEventType event, int x, int y, AtMouseEventFlags flags, void* user_data){
  uint64_t offset = (y*width + x);
  if(event == AT_MOUSE_EVENT_PRESS){
    // add seed
    add_seed(offset,flags);
    draw_point(x,y);
    pressed = TRUE;
  }
  if(event == AT_MOUSE_EVENT_RELEASE){
    pressed = FALSE;
  }
  if(event == AT_MOUSE_EVENT_MOVE && pressed){
    add_seed(offset,flags);
    draw_point(x,y);
  }
}

void
key_cb(AtKeyEventType event, int key_val, void* user_data){

}

int main(int argc, char** argv){
  pressed = FALSE;
  gtk_init(&argc,&argv);
  seeds_index = 0;

  char* name = "circle_color.png";
  if(argc >= 2)
    name = argv[1];

  // Loading an image
  at_image_read(&image, name);
  g_autoptr(AtArray(uint8_t)) image_gray = at_cvt_color(image,AT_COLOR_RGB,AT_COLOR_GRAY);
  at_array_copy(image, &image_seeded);

  // Get image info
  g_autofree uint64_t* size = at_array_get_size(image);
  width = size[1];
  height = size[0];
  depth = size[2];

  // Creating a window
  imageviewer = at_imageviewer_new_with_name("teste");

  // Getting seeds
  at_imageviewer_set_mouse_callback(imageviewer, mouse_cb,NULL);
  at_imageviewer_set_key_callback(imageviewer, key_cb,NULL);
  at_imageviewer_show_uint8_t(imageviewer, image_seeded, AT_COLOR_RGB);
  guint k = 0;
  while(k != GDK_KEY_Escape)
    k = at_imageviewer_waitkey();

  // Apply IFT
  g_autoptr(AtArray(uint64_t)) seeds = NULL;
  uint64_t seeds_size[2] = {seeds_index >> 1,2};
  at_array_new(&seeds, 2, seeds_size, seeds_data);
  g_autoptr(AtIFTArray(uint8_t)) ift = NULL;
  at_ift_apply(&ift,
               image_gray,
               2,
               AT_ADJACENCY_4,
               AT_OPTIMIZATION_MIN,
               at_connectivity(max, ift),
               at_weighting(abs_diff, ift),
               seeds);
  // Show Result
  AtArray(uint8_t)* labels = at_ift_get_labels(ift);
  at_imageviewer_show_uint8_t(imageviewer,labels,AT_COLOR_GRAY);
  at_imageviewer_waitkey();

  // Finish
  g_clear_object(&image);
  g_clear_object(&imageviewer);
  return 0;
}
