#include <at/gui.h>

void
mouse_cb(AtMouseEventType event, int x, int y, AtMouseEventFlags flags, void* user_data){

}

void
key_cb(AtKeyEventType event, int key_val, void* user_data){

}

int main(){
  g_autoptr(AtArray(uint8_t)) image = NULL;
  gtk_init(NULL,NULL);
  at_image_read(&image, "circle.png");
  AtImageViewer* imageviewer = at_imageviewer_new_with_name("teste");
  at_imageviewer_set_mouse_callback(imageviewer, mouse_cb);
  at_imageviewer_set_key_callback(imageviewer, key_cb);
  at_imageviewer_show_uint8_t(imageviewer, image);
  at_imageviewer_waitkey();
  return 0;
}
