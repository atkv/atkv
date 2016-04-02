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
#include <string.h>
#include <epoxy/gl.h>

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef enum{
  AT_ROTATION_0,
  AT_ROTATION_90,
  AT_ROTATION_180,
  AT_ROTATION_270,
  AT_ROTATION_CUSTOM,
}AtRotationMode;

typedef struct _AtVolumeViewerSliceConf{
  AtArray_uint8_t*    image;
  double              scale;
  GtkWidget*          toolbar;
  GtkWidget*          btn_zoom_in;
  GtkWidget*          btn_zoom_out;
  GtkWidget*          btn_zoom_reset;
  GtkWidget*          drawing_area;
  GtkWidget*          viewport;
  GtkWidget*          scrolled_window;
  GtkWidget*          main_box;
  GtkWidget*          index_slider;
  cairo_surface_t*    image_surface;

  uint16_t            slice_index;
  AtSliceSide         slice_side;
  AtRotationMode      rotation_mode;
}AtVolumeViewerSliceConf;

typedef struct _AtVolumeViewerPrivate{
  AtNiftiImage*       nifti_image;
  AtArray_uint8_t*    nifti_array;
  AtVolumeViewerSliceConf slices[3];

  GtkWidget*          window;
  char*               name;
  AtMouseCallbackFunc mouse_cb;
  AtKeyCallbackFunc   key_cb;
  gpointer            mouse_cb_data;
  gpointer            key_cb_data;

  GtkWidget*          grid;
  GtkWidget*          main_hbox;
  GtkWidget*          left_vbox;

  GtkWidget*          gl_area;
}AtVolumeViewerPrivate;
static guint key;

G_DEFINE_TYPE_WITH_PRIVATE(AtVolumeViewer, at_volumeviewer, G_TYPE_OBJECT)

static void
at_volumeviewer_dispose(GObject* object){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(AT_VOLUMEVIEWER(object));
  G_OBJECT_CLASS(at_volumeviewer_parent_class)->dispose(object);
}

static void
at_volumeviewer_finalize(GObject* object){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(AT_VOLUMEVIEWER(object));
  G_OBJECT_CLASS(at_volumeviewer_parent_class)->finalize(object);
}

static void
at_volumeviewer_class_init(AtVolumeViewerClass *klass){
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = at_volumeviewer_finalize;
  object_class->dispose  = at_volumeviewer_dispose;
}

static gboolean
at_volumeviewer_drawing_area_draw_event(GtkWidget* widget,
                                         cairo_t* cr,
                                         gpointer user_data){

  AtVolumeViewer* volumeviewer = AT_VOLUMEVIEWER(user_data);
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  uint8_t i;
  for(i = 0; i < 3; i++){
    uint64_t* size = at_array_get_size(priv->slices[i].image);
    if(widget == priv->slices[i].drawing_area){
      uint8_t* data = at_array_get(priv->slices[i].image);
      gtk_widget_set_size_request(priv->slices[i].drawing_area, size[1],size[0]);
      cairo_set_source_surface(cr, priv->slices[i].image_surface,0,0);
      cairo_paint(cr);
      break;
    }
    g_free(size);
  }

  return FALSE;
}

static uint8_t
at_volumeviewer_get_side_axis(AtSliceSide side){
  switch(side){
  case AT_SLICE_BACK:
  case AT_SLICE_FRONT:
    return 2;
  case AT_SLICE_LEFT:
  case AT_SLICE_RIGHT:
    return 1;
  case AT_SLICE_UP:
  case AT_SLICE_DOWN:
    return 0;
  }
}

static void
at_volumeviewer_update_drawing_area(AtVolumeViewer* volumeviewer, uint8_t i){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  g_autoptr(AtArray(uint8_t)) slice_array            = NULL; // referenced pixels (not contiguous)
  g_autoptr(AtArray(uint8_t)) slice_array_copy       = NULL; // to get contiguous pixels

  uint64_t* size = NULL;
  uint8_t* data;
  g_autofree uint64_t* nifti_size = at_array_get_size(priv->nifti_array);

  // Calculate proper range
  AtVolumeViewerSliceConf* current_slice = &priv->slices[i];
  AtRange ranges[3] = {at_range_all(), at_range_all(), at_range_all()};
  AtRange range = at_range_from_to(current_slice->slice_index,current_slice->slice_index+1);
  uint8_t index = at_volumeviewer_get_side_axis(current_slice->slice_side);
  ranges[index] = range;

  // Get slice (not contiguous)
  g_clear_object(&slice_array);
  at_array_sub(priv->nifti_array, ranges, &slice_array);

  // Get contiguous pixels
  g_clear_object(&slice_array_copy);
  at_array_copy(slice_array, &slice_array_copy);
  at_array_squeeze(slice_array_copy);

  g_clear_object(&current_slice->image);
  current_slice->image = at_cvt_color(slice_array_copy, AT_COLOR_GRAY, AT_COLOR_BGRA);

  // Create cairo surface based on slice
  data = at_array_get(current_slice->image);
  size = at_array_get_size(current_slice->image);
  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24,size[1]);


  if(current_slice->image_surface){
    uint8_t* cairo_surface_data = cairo_image_surface_get_data(current_slice->image_surface);
    memcpy(cairo_surface_data,data,at_array_get_num_elements(current_slice->image));
  }else{
    current_slice->image_surface = cairo_image_surface_create_for_data(data,CAIRO_FORMAT_RGB24,size[1],size[0],stride);
  }
  gtk_range_set_range(GTK_RANGE(current_slice->index_slider), 0, nifti_size[index]-1);
  g_free(size);
}

static void
at_volumeviewer_index_slider_changed_event(GtkRange* range, gpointer user_data){
  AtVolumeViewer* volumeviewer = AT_VOLUMEVIEWER(user_data);
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  GtkWidget* scale = GTK_WIDGET(range);
  uint8_t i;
  for(i = 0; i < 3; i++){
    if(priv->slices[i].index_slider == scale){
      priv->slices[i].slice_index = gtk_range_get_value(range);
      at_volumeviewer_update_drawing_area(volumeviewer,i);
      gtk_widget_queue_draw(priv->window);
      break;
    }
  }
}

static void
at_volumeviewer_gl_area_init_buffers(AtVolumeViewer* volumeviewer){
//  glGenVertexArrays(1, &vao);
//  glBindVertexArray(vao);
//  glGenBuffers(1, &buffer);

}

static void
at_volumeviewer_gl_area_realize_event(GtkGLArea* area, AtVolumeViewer* volumeviewer){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  gtk_gl_area_make_current(GTK_GL_AREA(priv->gl_area));
  if(gtk_gl_area_get_error(area)!=NULL) return;
  at_volumeviewer_gl_area_init_buffers(volumeviewer);
  glClearColor(0.0, 0.0, 0.2, 1.0);

}
static gboolean
at_volumeviewer_gl_area_render_event(GtkGLArea* area, GdkGLContext *context, AtVolumeViewer* volumeviewer){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  return TRUE;
}

static void
at_volumeviewer_create_gl_area(AtVolumeViewer* volumeviewer){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  priv->gl_area = gtk_gl_area_new();
  g_signal_connect(priv->gl_area, "realize", G_CALLBACK(at_volumeviewer_gl_area_realize_event),volumeviewer);
  g_signal_connect(priv->gl_area, "render", G_CALLBACK(at_volumeviewer_gl_area_render_event),volumeviewer);
  gtk_box_pack_start(GTK_BOX(priv->main_hbox),priv->gl_area, TRUE, TRUE, 0);
  gtk_widget_set_size_request(priv->gl_area,640,480);
}

static void
at_volumeviewer_create_slice(AtVolumeViewer* volumeviewer, uint8_t i){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  AtVolumeViewerSliceConf* current_slice = &priv->slices[i];
  current_slice->scrolled_window = gtk_scrolled_window_new(NULL,NULL);
  current_slice->viewport        = gtk_viewport_new(NULL,NULL);
  current_slice->drawing_area    = gtk_drawing_area_new();
  current_slice->index_slider    = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,0,255,1);
  g_signal_connect(current_slice->index_slider, "value-changed",G_CALLBACK(at_volumeviewer_index_slider_changed_event),volumeviewer);

  //gtk_container_add(GTK_CONTAINER(current_slice->viewport), current_slice->drawing_area);
  //gtk_container_add(GTK_CONTAINER(current_slice->scrolled_window), current_slice->viewport);
  current_slice->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_pack_start(GTK_BOX(current_slice->main_box),current_slice->drawing_area,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(current_slice->main_box),current_slice->index_slider,FALSE,FALSE,0);

  gtk_widget_add_events(current_slice->drawing_area, GDK_BUTTON_PRESS_MASK|GDK_SCROLL_MASK|GDK_POINTER_MOTION_MASK|GDK_BUTTON_RELEASE_MASK);
  g_signal_connect(current_slice->drawing_area, "draw",G_CALLBACK(at_volumeviewer_drawing_area_draw_event),volumeviewer);

  gtk_box_pack_start(GTK_BOX(priv->left_vbox), current_slice->main_box,FALSE,FALSE,0);
}

static void
at_volumeviewer_create_window(AtVolumeViewer* volumeviewer){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  priv->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(priv->window, "destroy", G_CALLBACK(gtk_main_quit), volumeviewer);
  priv->grid = gtk_grid_new();
  priv->main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  priv->left_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  gtk_container_add(GTK_CONTAINER(priv->window), priv->main_hbox);
  gtk_container_add(GTK_CONTAINER(priv->main_hbox), priv->left_vbox);

  // Create slices
  uint8_t i;
  AtSliceSide slices_default[3] = {AT_SLICE_FRONT, AT_SLICE_LEFT, AT_SLICE_UP};
  for(i = 0; i < 3; i++){
    at_volumeviewer_create_slice(volumeviewer, i);
    priv->slices[i].slice_side = slices_default[i];
    priv->slices[i].slice_index = 0;
  }

  // Create gl_area
  at_volumeviewer_create_gl_area(volumeviewer);
}

static void
at_volumeviewer_init(AtVolumeViewer *self){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(self);
  priv->nifti_image = NULL;
  priv->nifti_array = NULL;
  memset(priv->slices,0,sizeof(AtVolumeViewerSliceConf)*3);
  at_volumeviewer_create_window(self);
}



static void
at_volumeviewer_update_drawing_areas(AtVolumeViewer* volumeviewer){
  uint8_t i;
  for(i = 0; i < 3; i++)
    at_volumeviewer_update_drawing_area(volumeviewer,i);

}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtVolumeViewer*
at_volumeviewer_new(){
  return g_object_new(AT_TYPE_VOLUMEVIEWER, NULL);
}

AtVolumeViewer*
at_volumeviewer_new_with_name(char* name){
  AtVolumeViewer* volumeviewer = at_volumeviewer_new();
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  priv->name = g_strdup(name);
  return volumeviewer;
}

AtVolumeViewer*
at_volumeviewer_show_nifti(AtVolumeViewer* volumeviewer, AtNiftiImage* nifti_image){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  g_set_object(&priv->nifti_image, nifti_image);

  AtArray(uint16_t)* nifti_array = AT_ARRAY_uint16_t(priv->nifti_image);
  g_autoptr(AtArray(uint16_t)) nifti_normalized = NULL;

  at_array_normalize(nifti_array, &nifti_normalized, 0, 255, AT_NORM_MINMAX, NULL);
  at_array_copy(nifti_array, &priv->nifti_array);

  // Create slices from nifti data array
  at_volumeviewer_update_drawing_areas(volumeviewer);

  // Show everything

  gtk_widget_show_all(priv->window);
}

guint
at_volumeviewer_waitkey(){
  gtk_main();
  return key;
}

void
at_volumeviewer_set_mouse_callback(AtVolumeViewer* volumeviewer, AtMouseCallbackFunc mouse_cb, gpointer user_data){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  priv->mouse_cb = mouse_cb;
  priv->mouse_cb_data = user_data;
}

void
at_volumeviewer_set_key_callback(AtVolumeViewer* volumeviewer, AtKeyCallbackFunc key_cb, gpointer user_data){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  priv->key_cb = key_cb;
  priv->key_cb_data = user_data;
}
