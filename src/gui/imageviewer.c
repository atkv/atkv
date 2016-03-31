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

typedef enum{
  AT_INTERPOLATION_NEAREST,
  AT_INTERPOLATION_LINEAR,
  AT_INTERPOLATION_AREA,
  AT_INTERPOLATION_CUBIC,
  AT_INTERPOLATION_LANCZOS4,
}AtInterpolationMode;

/**
 * @brief Convert an image from a color format to another
 * @param image
 * @param from
 * @param to
 * @return
 */
AtArray_uint8_t*
at_cvt_color(AtArray_uint8_t* image, AtColorFormat from, AtColorFormat to){
  AtArray_uint8_t* image_cvt = NULL;
  // Get some image info
  uint8_t* data = at_array_uint8_t_get_data(image);
  g_autofree uint64_t* size = at_array_get_size(image);

  uint64_t sizeimage[3] = {size[0],size[1],4};
  switch(to){
  case AT_COLOR_GRAY: sizeimage[2] = 1;break;
  case AT_COLOR_RGB:  sizeimage[2] = 3;break;
  case AT_COLOR_ARGB:
  case AT_COLOR_BGRA: sizeimage[2] = 4;break;
  }

  at_array_zeros(&image_cvt, 3, sizeimage);

  uint8_t* data_image = at_array_uint8_t_get_data(image_cvt);
  uint64_t n, k, offset_image, offset;

  if(from == AT_COLOR_GRAY && to == AT_COLOR_BGRA){
    for(n = 0; n < size[0]*size[1]; n++){
      offset_image = n*sizeimage[2];
      offset = n*size[2];
      for(k = 0; k < 3; k++){
        data_image[offset_image+k] = data[offset];
      }
      data_image[offset_image+k] = 255;
    }
  }
  else if(from == AT_COLOR_RGB && to == AT_COLOR_BGRA){
    for(n = 0; n < size[0]*size[1]; n++){
      offset_image = n*sizeimage[2];
      offset = n*size[2];
      for(k = 0; k < size[2]; k++){
        data_image[offset_image+k] = data[offset+(2-k)];
      }
    }
  }
  else if(from == AT_COLOR_RGB && to == AT_COLOR_GRAY){
    for(n = 0; n < size[0]*size[1]; n++){
      offset_image = n*sizeimage[2];
      offset = n*size[2];
      data_image[offset_image] = (int)((double)data[offset]*0.299 + (double)data[offset+1]*0.587 + (double)data[offset+2]*0.114);
    }
  }
  return image_cvt;
}

AtArray_uint8_t*
at_array_uint8_t_scale(AtArray_uint8_t* image, double scale, AtInterpolationMode interpolation){
  AtArray_uint8_t* output = NULL;
  g_autofree uint64_t* size = at_array_get_size(image);
  uint64_t new_size[3] = {size[0]*scale,size[1]*scale,size[2]};
  at_array_new(&output, 3, new_size);
  uint8_t* new_data = at_array_uint8_t_get_data(output);
  uint8_t* data = at_array_uint8_t_get_data(image);
  uint64_t x,y,pos_x,pos_y,offset,new_offset;
  for(y = 0; y < new_size[0]; y++){
    for(x = 0; x < new_size[1]; x++){
      pos_y = (uint64_t)(y/scale);
      pos_x = (uint64_t)(x/scale);
      offset = (pos_y * size[1] + pos_x) << 2;
      new_offset = (y * new_size[1] + x) << 2;

      // The ARGB colors of the pixel
      new_data[new_offset] = data[offset];
      new_data[new_offset+1] = data[offset+1];
      new_data[new_offset+2] = data[offset+2];
      new_data[new_offset+3] = data[offset+3];
    }
  }
  return output;
}

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtImageViewerPrivate{

  // State
  char*               name;
  AtArray_uint8_t*    image;
  uint64_t*           image_size;
  uint64_t*           image_step;
  AtMouseCallbackFunc mouse_cb;
  AtKeyCallbackFunc   key_cb;
  double              scale;
  void*               mouse_cb_data;
  void*               key_cb_data;
  AtArray_uint8_t*    image_scaled;

  // Window controls and params
  gulong              destroy_id;
  GtkWidget*          window;
  GtkWidget*          toolbar;
  GtkWidget*          statusbar;
  GtkWidget*          vbox;
  GtkToolItem*        btn_save;
  GtkToolItem*        btn_zoom_in;
  GtkToolItem*        btn_zoom_out;
  GtkToolItem*        btn_zoom_reset;
  GtkToolItem*        btn_resize_to_fit;
  GtkToolItem*        btn_translate_left;
  GtkToolItem*        btn_translate_right;
  GtkToolItem*        btn_translate_up;
  GtkToolItem*        btn_translate_down;
  char                lbl_text[256];

  GtkWidget*          statusbar_box;
  GtkWidget*          statusbar_lbl;

  GtkWidget*          viewport;
  GtkWidget*          drawing_area;
  GtkWidget*          scrolled_window;

  cairo_surface_t*    image_surface;

  int                 last_x;
  int                 last_y;
  guint               key;

}AtImageViewerPrivate;
static guint key;

G_DEFINE_TYPE_WITH_PRIVATE(AtImageViewer, at_imageviewer, G_TYPE_OBJECT)

static void
at_imageviewer_dispose(GObject* object){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(AT_IMAGEVIEWER(object));
  cairo_surface_destroy(priv->image_surface);
  G_OBJECT_CLASS(at_imageviewer_parent_class)->dispose(object);
}
static void
at_imageviewer_finalize(GObject* object){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(AT_IMAGEVIEWER(object));
  if(priv->name) g_free(priv->name);
  if(priv->image_size) g_free(priv->image_size);
  if(priv->image_step) g_free(priv->image_step);
  G_OBJECT_CLASS(at_imageviewer_parent_class)->dispose(object);
}

static void
at_imageviewer_class_init(AtImageViewerClass *klass){
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = at_imageviewer_dispose;
}

/* EVENTS
 * =======*/
static gboolean
at_imageviewer_drawing_area_draw_event(GtkWidget* widget,
                                       cairo_t* cr,
                                       gpointer user_data){
  // Get ImageViewer
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  g_autofree uint64_t* size = at_array_get_size(priv->image_scaled);

  GtkAllocation allocation;
  gtk_widget_get_allocation(priv->viewport, &allocation);

  // Draw the image into imageviewer drawing area
  cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
  gtk_widget_set_size_request(priv->drawing_area, size[1],size[0]);
  cairo_set_source_surface(cr, priv->image_surface,0,0);

  cairo_paint(cr);

  return FALSE;
}
static AtMouseEventFlags
at_get_mouse_flag_button(guint button){
  AtMouseEventFlags flags = AT_MOUSE_FLAG_NONE;
  if(button == 1) flags |= AT_MOUSE_FLAG_LBUTTON;
  if(button == 2) flags |= AT_MOUSE_FLAG_MBUTTON;
  if(button == 3) flags |= AT_MOUSE_FLAG_RBUTTON;
  return flags;
}
static AtMouseEventFlags
at_get_mouse_flag_state(guint state){
  AtMouseEventFlags flags = AT_MOUSE_FLAG_NONE;
  if(state & GDK_CONTROL_MASK) flags |= AT_MOUSE_FLAG_CTRLKEY;
  if(state & GDK_SHIFT_MASK) flags |= AT_MOUSE_FLAG_SHIFTKEY;
  if(state & GDK_MOD1_MASK) flags |= AT_MOUSE_FLAG_ALTKEY;
  return flags;
}

static AtMouseEventFlags
at_get_mouse_flag(guint button, guint state){
  return at_get_mouse_flag_button(button) | at_get_mouse_flag_state(state);
}

static void
at_send_mouse_event(AtImageViewer* imageviewer, AtMouseEventType mouse_event, GdkEventButton* event, int x, int y){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  priv->last_x = x;
  priv->last_y = y;
  if(priv->mouse_cb)
    priv->mouse_cb(mouse_event, x, y, at_get_mouse_flag(event->button, event->state), priv->mouse_cb_data);
}

static void
at_mouse_to_image_coords(AtImageViewer* imageviewer, int oldx, int oldy, int* x, int* y){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);

  *x = (int)(oldx/priv->scale);
  *y = (int)(oldy/priv->scale);
}

static gboolean
at_imageviewer_drawing_area_button_press_event(GtkWidget* widget,
                                               GdkEventButton *event,
                                               gpointer user_data){
  // Get ImageViewer
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  int x,y;
  at_mouse_to_image_coords(imageviewer,event->x, event->y, &x, &y);

  if(x >= 0 && x < priv->image_size[1] &&
     y >= 0 && y < priv->image_size[0]){
    at_send_mouse_event(imageviewer, AT_MOUSE_EVENT_PRESS, event, x, y);
  }

  return TRUE;
}
static gboolean
at_imageviewer_drawing_area_button_release_event(GtkWidget* widget,
                                                 GdkEventButton *event,
                                                 gpointer user_data){
  // Get ImageViewer
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  int x,y;
  at_mouse_to_image_coords(imageviewer,event->x, event->y, &x, &y);
  if(x >= 0 && x < priv->image_size[1] &&
     y >= 0 && y < priv->image_size[0]){
    at_send_mouse_event(imageviewer, AT_MOUSE_EVENT_RELEASE, event, x, y);
  }
  return TRUE;
}
static gboolean
at_imageviewer_drawing_area_scroll_event(GtkWidget* widget,
                                         GdkEventScroll* event,
                                         gpointer user_data){
  // Get ImageViewer
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);

  if(event->state & GDK_CONTROL_MASK){
    if(event->direction == GDK_SCROLL_UP){
      at_imageviewer_zoom_in(imageviewer, 1);
    }else if(event->direction == GDK_SCROLL_DOWN){
      at_imageviewer_zoom_out(imageviewer, 1);
    }
    return TRUE;
  }
  return FALSE;
}
static gboolean
at_imageviewer_drawing_area_motion_event(GtkWidget* widget,
                                         GdkEventMotion* event,
                                         gpointer user_data){
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);

  int x,y;
  at_mouse_to_image_coords(imageviewer,event->x, event->y, &x, &y);

  if(x >= 0 && x < priv->image_size[1] &&
     y >= 0 && y < priv->image_size[0]){
    uint8_t* data       = at_array_get(priv->image) + y*priv->image_step[0] + x*priv->image_step[1];
    sprintf(priv->lbl_text, "(%03d, %03d). RGB(<span color=\"red\">%03d</span>,<span color=\"green\">%03d</span>,<span color=\"blue\">%03d</span>)=#%02x%02x%02x",
            x, y,
            data[2],data[1],data[0],
            data[2],data[1],data[0]);

    gtk_label_set_markup(GTK_LABEL(priv->statusbar_lbl), priv->lbl_text);
    if(priv->mouse_cb)
      priv->mouse_cb(AT_MOUSE_EVENT_MOVE, x,y,at_get_mouse_flag_state(event->state), priv->mouse_cb_data);
  }

}
static gboolean
at_imageviewer_key_press_event(GtkWidget* widget,
                               GdkEventKey* event,
                               gpointer user_data){
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  if(priv->key_cb)
    priv->key_cb(AT_KEY_EVENT_PRESS,event->keyval,priv->key_cb_data);
}
static gboolean
at_imageviewer_key_release_event(GtkWidget* widget,
                               GdkEventKey* event,
                               gpointer user_data){

  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  priv->key = event->keyval;
  key = event->keyval;
  if(!event->is_modifier) gtk_main_quit();
  if(priv->key_cb)
    priv->key_cb(AT_KEY_EVENT_RELEASE,event->keyval,priv->key_cb_data);
}

static void
btn_zoom_in_clicked_event(GtkWidget* widget,
                          gpointer user_data){
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  at_imageviewer_zoom_in(imageviewer,1);
}
static void
btn_zoom_out_clicked_event(GtkWidget* widget,
                          gpointer user_data){
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  at_imageviewer_zoom_out(imageviewer,1);
}

static void
btn_zoom_reset_clicked_event(GtkWidget* widget,
                          gpointer user_data){
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  at_imageviewer_zoom_to_fit(imageviewer);
}
static void
btn_resize_to_fit_clicked_event(GtkWidget* widget,
                                gpointer user_data){
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);

}
static void
btn_translate_down_clicked_event(GtkWidget* widget,
                                gpointer user_data){
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);

}
static void
btn_translate_left_clicked_event(GtkWidget* widget,
                                gpointer user_data){
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);

}
static void
btn_translate_right_clicked_event(GtkWidget* widget,
                                gpointer user_data){
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);

}
static void
btn_translate_up_clicked_event(GtkWidget* widget,
                                gpointer user_data){
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);

}
static void
btn_save_clicked_event(GtkWidget* widget,
                                gpointer user_data){
  AtImageViewer* imageviewer = AT_IMAGEVIEWER(user_data);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);

}

/* WINDOW CREATION
 * ===============*/

static void
at_imageviewer_create_toolbar(AtImageViewer* self){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);
  priv->toolbar = gtk_toolbar_new();
  GtkWidget* btn_save_img             = gtk_image_new_from_icon_name("document-save",GTK_ICON_SIZE_SMALL_TOOLBAR);
  GtkWidget* btn_resize_to_fit_img    = gtk_image_new_from_icon_name("zoom-fit-best",GTK_ICON_SIZE_SMALL_TOOLBAR);
  GtkWidget* btn_translate_down_img   = gtk_image_new_from_icon_name("go-down",GTK_ICON_SIZE_SMALL_TOOLBAR);
  GtkWidget* btn_translate_left_img   = gtk_image_new_from_icon_name("go-previous",GTK_ICON_SIZE_SMALL_TOOLBAR);
  GtkWidget* btn_translate_right_img  = gtk_image_new_from_icon_name("go-next",GTK_ICON_SIZE_SMALL_TOOLBAR);
  GtkWidget* btn_translate_up_img     = gtk_image_new_from_icon_name("go-up",GTK_ICON_SIZE_SMALL_TOOLBAR);
  GtkWidget* btn_zoom_in_img          = gtk_image_new_from_icon_name("zoom-in",GTK_ICON_SIZE_SMALL_TOOLBAR);
  GtkWidget* btn_zoom_out_img         = gtk_image_new_from_icon_name("zoom-out",GTK_ICON_SIZE_SMALL_TOOLBAR);
  GtkWidget* btn_zoom_reset_img       = gtk_image_new_from_icon_name("zoom-original",GTK_ICON_SIZE_SMALL_TOOLBAR);

  priv->btn_save            = gtk_tool_button_new(btn_save_img,"Save");
  priv->btn_resize_to_fit   = gtk_tool_button_new(btn_resize_to_fit_img,"Resize to fit");
  priv->btn_translate_down  = gtk_tool_button_new(btn_translate_down_img,"Translate down");
  priv->btn_translate_left  = gtk_tool_button_new(btn_translate_left_img,"Translate left");
  priv->btn_translate_right = gtk_tool_button_new(btn_translate_right_img,"Translate right");
  priv->btn_translate_up    = gtk_tool_button_new(btn_translate_up_img,"Translate up");
  priv->btn_zoom_in         = gtk_tool_button_new(btn_zoom_in_img,"Zoom in");
  priv->btn_zoom_out        = gtk_tool_button_new(btn_zoom_out_img,"Zoom out");
  priv->btn_zoom_reset      = gtk_tool_button_new(btn_zoom_reset_img,"Reset zoom");

  gtk_tool_item_set_tooltip_text(priv->btn_resize_to_fit,"Resize window to fit");
  gtk_tool_item_set_tooltip_text(priv->btn_save,"Save to file");
  gtk_tool_item_set_tooltip_text(priv->btn_translate_down,"Translate Down");
  gtk_tool_item_set_tooltip_text(priv->btn_translate_left,"Translate Left");
  gtk_tool_item_set_tooltip_text(priv->btn_translate_right,"Translate Right");
  gtk_tool_item_set_tooltip_text(priv->btn_translate_up,"Translate Up");
  gtk_tool_item_set_tooltip_text(priv->btn_zoom_in,"Zoom In");
  gtk_tool_item_set_tooltip_text(priv->btn_zoom_out,"Zoom Out");
  gtk_tool_item_set_tooltip_text(priv->btn_zoom_reset,"Zoom to Original");

  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),priv->btn_save,0);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),priv->btn_resize_to_fit,1);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),priv->btn_translate_down,2);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),priv->btn_translate_up,3);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),priv->btn_translate_left,4);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),priv->btn_translate_right,5);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),priv->btn_zoom_in,6);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),priv->btn_zoom_out,7);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),priv->btn_zoom_reset,8);
  gtk_toolbar_set_icon_size(GTK_TOOLBAR(priv->toolbar), GTK_ICON_SIZE_SMALL_TOOLBAR);
  gtk_box_pack_start(GTK_BOX(priv->vbox), priv->toolbar, FALSE,FALSE, 0);

  g_signal_connect(priv->btn_zoom_in, "clicked",
                   G_CALLBACK(btn_zoom_in_clicked_event), self);
  g_signal_connect(priv->btn_zoom_out, "clicked",
                   G_CALLBACK(btn_zoom_out_clicked_event), self);
  g_signal_connect(priv->btn_zoom_reset, "clicked",
                   G_CALLBACK(btn_zoom_reset_clicked_event), self);
  g_signal_connect(priv->btn_resize_to_fit, "clicked",
                   G_CALLBACK(btn_resize_to_fit_clicked_event), self);
  g_signal_connect(priv->btn_translate_down, "clicked",
                   G_CALLBACK(btn_translate_down_clicked_event), self);
  g_signal_connect(priv->btn_translate_left, "clicked",
                   G_CALLBACK(btn_translate_left_clicked_event), self);
  g_signal_connect(priv->btn_translate_right, "clicked",
                   G_CALLBACK(btn_translate_right_clicked_event), self);
  g_signal_connect(priv->btn_translate_up, "clicked",
                   G_CALLBACK(btn_translate_up_clicked_event), self);
  g_signal_connect(priv->btn_save, "clicked",
                   G_CALLBACK(btn_save_clicked_event), self);
}
static void
at_imageviewer_create_statusbar(AtImageViewer* self){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);
  priv->statusbar = gtk_statusbar_new();
  gtk_box_pack_end(GTK_BOX(priv->vbox), priv->statusbar, FALSE,FALSE, 0);
  //gtk_statusbar_push(GTK_STATUSBAR(priv->statusbar),0,"Teste");

  priv->statusbar_lbl = gtk_label_new("teste");
  priv->statusbar_box = gtk_statusbar_get_message_area(GTK_STATUSBAR(priv->statusbar));
  gtk_box_pack_start(GTK_BOX(priv->statusbar_box),priv->statusbar_lbl,FALSE, FALSE, 0);
}

/**
 * @brief Create Area for Rendering Image
 * @param self
 */
static void
at_imageviewer_create_drawing_area(AtImageViewer* self){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);
  priv->drawing_area = gtk_drawing_area_new();
  priv->scrolled_window = gtk_scrolled_window_new(NULL,NULL);
  priv->viewport = gtk_viewport_new(NULL,NULL);
  gtk_container_add(GTK_CONTAINER(priv->scrolled_window), priv->viewport);
  gtk_container_add(GTK_CONTAINER(priv->viewport), priv->drawing_area);
  gtk_box_pack_start(GTK_BOX(priv->vbox),priv->scrolled_window,TRUE,TRUE,0);

  gtk_widget_add_events(priv->drawing_area, GDK_BUTTON_PRESS_MASK|GDK_SCROLL_MASK|GDK_POINTER_MOTION_MASK|GDK_BUTTON_RELEASE_MASK);
  g_signal_connect(G_OBJECT(priv->drawing_area),
                   "draw",
                   G_CALLBACK(at_imageviewer_drawing_area_draw_event),
                   self);
  g_signal_connect(G_OBJECT(priv->drawing_area),
                   "button-press-event",
                   G_CALLBACK(at_imageviewer_drawing_area_button_press_event),
                   self);
  g_signal_connect(G_OBJECT(priv->drawing_area),
                   "button-release-event",
                   G_CALLBACK(at_imageviewer_drawing_area_button_release_event),
                   self);

  g_signal_connect(G_OBJECT(priv->drawing_area),
                   "scroll-event",
                   G_CALLBACK(at_imageviewer_drawing_area_scroll_event),
                   self);
  g_signal_connect(G_OBJECT(priv->drawing_area),
                   "motion-notify-event",
                   G_CALLBACK(at_imageviewer_drawing_area_motion_event),
                   self);
  g_signal_connect(G_OBJECT(priv->window),
                   "key-press-event",
                   G_CALLBACK(at_imageviewer_key_press_event),
                   self);
  g_signal_connect(G_OBJECT(priv->window),
                   "key-release-event",
                   G_CALLBACK(at_imageviewer_key_release_event),
                   self);
}

static void
at_imageviewer_create_window(AtImageViewer* self){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);
  priv->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  priv->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(priv->window),priv->vbox);
  at_imageviewer_quit_on_destroy(self,TRUE);
  at_imageviewer_create_toolbar(self);
  at_imageviewer_create_drawing_area(self);
  at_imageviewer_create_statusbar(self);
}

static void
at_imageviewer_init(AtImageViewer *self){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);
  priv->name = NULL;
  priv->key_cb = NULL;
  priv->mouse_cb = NULL;
  priv->scale = 1;
  priv->image_scaled = NULL;
  priv->image = NULL;
  priv->image_surface = NULL;
  priv->image_size = NULL;
  priv->image_step = NULL;
  at_imageviewer_create_window(self);
}

static void
at_imageviewer_update_image_surface(AtImageViewer* imageviewer){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);

  // Get scale image info
  g_autofree uint64_t* size = at_array_get_size(priv->image_scaled);
  uint8_t* data       = at_array_uint8_t_get_data(priv->image_scaled);

  // Create cairo surface based on scaled image
  int stride          = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24,size[1]);
  if(priv->image_surface) cairo_surface_destroy(priv->image_surface);
  priv->image_surface = cairo_image_surface_create_for_data(data,CAIRO_FORMAT_RGB24,size[1],size[0],stride);
}

static void
at_imageviewer_update_image_scaled(AtImageViewer* imageviewer){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  g_set_object(&priv->image_scaled,at_array_uint8_t_scale(priv->image,priv->scale,AT_INTERPOLATION_NEAREST));
  at_imageviewer_update_image_surface(imageviewer);
}
/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtImageViewer*
at_imageviewer_new(){
  return g_object_new(AT_TYPE_IMAGEVIEWER, NULL);
}

AtImageViewer*
at_imageviewer_new_with_name(char* name){
  AtImageViewer* imageviewer = at_imageviewer_new();
  at_imageviewer_set_name(imageviewer, name);
  return imageviewer;
}

void
at_imageviewer_set_name(AtImageViewer* imageviewer, char* name){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  priv->name = g_strdup(name);
}
void
at_imageviewer_set_mouse_callback(AtImageViewer* imageviewer, AtMouseCallbackFunc mouse_cb, gpointer user_data){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  priv->mouse_cb = mouse_cb;
  priv->mouse_cb_data = user_data;
}

void
at_imageviewer_set_key_callback(AtImageViewer* imageviewer, AtKeyCallbackFunc key_cb, gpointer user_data){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  priv->key_cb = key_cb;
  priv->key_cb_data = user_data;
}

guint
at_imageviewer_waitkey(){
  gtk_main();
  return key;
}

void
at_imageviewer_show_uint8_t(AtImageViewer* imageviewer, AtArray_uint8_t* array, AtColorFormat format){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);

  // Avoid memory leak
  g_clear_object(&priv->image);
  if(priv->image_size) g_free(priv->image_size);
  if(priv->image_step) g_free(priv->image_step);


  // Create the image in proper format
  priv->image = at_cvt_color(array, format, AT_COLOR_BGRA);
  priv->image_size    = at_array_get_size(priv->image);
  priv->image_step    = at_array_get_step(priv->image);

  at_imageviewer_update_image_scaled(imageviewer);

  //gtk_widget_set_size_request(priv->scrolled_window, size[1],size[0]);
  gtk_widget_queue_draw(priv->drawing_area);
  gtk_widget_show_all(GTK_WIDGET(priv->window));
}

void
at_imageviewer_quit_on_destroy(AtImageViewer* display, gboolean value){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(display);
  if(value && priv->destroy_id == 0)
    priv->destroy_id = g_signal_connect(priv->window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
  else{
    g_signal_handler_disconnect(priv->window, priv->destroy_id);
    priv->destroy_id = 0;
  }
}

void
at_imageviewer_zoom_in(AtImageViewer* imageviewer, double scale_increments){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  if(priv->scale <= 1) priv->scale *= 2.0;
  else priv->scale += scale_increments;

  at_imageviewer_update_image_scaled(imageviewer);
  gtk_widget_queue_draw(priv->drawing_area);

}

/**
 * @brief at_imageviewer_zoom_out
 * @param imageviewer
 * @param scale_increments
 */
void
at_imageviewer_zoom_out(AtImageViewer* imageviewer, double scale_increments){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  if(priv->scale <= 1) priv->scale /= 2.0;
  else priv->scale -= scale_increments;

  at_imageviewer_update_image_scaled(imageviewer);
  gtk_widget_queue_draw(priv->drawing_area);
}

void
at_imageviewer_zoom_to_fit(AtImageViewer* imageviewer){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  priv->scale = 1;
  at_imageviewer_update_image_scaled(imageviewer);
  gtk_widget_queue_draw(priv->drawing_area);
}
