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

typedef enum{
  AT_NORM_MIN_MAX,
  AT_NORM_L1,
  AT_NORM_L2,
  AT_NORM_INF,
}AtNormType;

typedef struct _AtVolumeViewerSliceConf{
  AtRotationMode      rotation_mode;
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

  cairo_surface_t*    image_surface;

  uint16_t            slice_index;
}AtVolumeViewerSliceConf;

typedef struct _AtVolumeViewerPrivate{
  AtNiftiImage*       nifti_image;

  AtVolumeViewerSliceConf slices[3];

  GtkWidget*          window;
  char*               name;
  AtMouseCallbackFunc mouse_cb;
  AtKeyCallbackFunc   key_cb;
  gpointer            mouse_cb_data;
  gpointer            key_cb_data;

  GtkWidget*          grid;
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

static void
at_volumeviewer_create_slice(AtVolumeViewer* volumeviewer, uint8_t i){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  AtVolumeViewerSliceConf* current_slice = &priv->slices[i];
  current_slice->scrolled_window = gtk_scrolled_window_new(NULL,NULL);
  current_slice->viewport        = gtk_viewport_new(NULL,NULL);
  current_slice->drawing_area    = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(current_slice->viewport), current_slice->drawing_area);
  gtk_container_add(GTK_CONTAINER(current_slice->scrolled_window), current_slice->viewport);
  current_slice->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  gtk_grid_attach(GTK_GRID(priv->grid), current_slice->main_box, i % 2, i >> 1, 1, 1);
}

static void
at_volumeviewer_create_window(AtVolumeViewer* volumeviewer){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  priv->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(priv->window, "destroy", G_CALLBACK(gtk_main_quit), volumeviewer);
  priv->grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(priv->window), priv->grid);

  // Create slices
  uint8_t i;
  for(i = 0; i < 3; i++)
    at_volumeviewer_create_slice(volumeviewer, i);
}

static void
at_volumeviewer_init(AtVolumeViewer *self){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(self);
  priv->nifti_image = NULL;
  memset(priv->slices,0,sizeof(AtVolumeViewerSliceConf)*3);
  at_volumeviewer_create_window(self);
}

static void
at_volumeviewer_update_drawing_areas(AtVolumeViewer* volumeviewer){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);

  AtArray(uint16_t)* nifti_array = AT_ARRAY_uint16_t(priv->nifti_image);
  AtRange ranges[3] = {at_range_all(), at_range_from_to(102,103),at_range_all()};

  AtArray(uint16_t)* slice_array            = NULL; // referenced pixels (not contiguous)
  AtArray(uint16_t)* slice_array_copy       = NULL; // to get contiguous pixels
  AtArray(uint16_t)* slice_array_normalized = NULL; // to fit to uint8_t (max = 255)
  AtArray(uint8_t)*  slice_array_final      = NULL; // converted to uint8_t

  // Get slice (not contiguous)
  at_array_sub(nifti_array, ranges, &slice_array);

  // Get contiguous pixels
  at_array_copy(slice_array, &slice_array_copy);

  // Normalize pixels
  at_array_normalize(slice_array_copy, &slice_array_normalized, 0, 255, AT_NORM_MIN_MAX, NULL);

  // Convert to uint8_t
  at_array_copy(slice_array_normalized, &slice_array_final);

  // Convert to ARGB (for Cairo)
  at_cvt_color(slice_array_final, AT_COLOR_GRAY, AT_COLOR_ARGB);

  // Create cairo surface based on slice
  g_autofree uint64_t* size = at_array_get_size(slice_array_final);
  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24,size[1]);
  priv->slices[0].image_surface = cairo_image_surface_create_for_data(at_array_get(slice_array_final),CAIRO_FORMAT_RGB24,size[1],size[0],stride);
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

  // Create slices from nifti data array
  at_volumeviewer_update_drawing_areas(volumeviewer);

  // Show everything
  gtk_widget_show_all(priv->window);
  uint8_t i;
  for(i = 0; i < 3; i++)
    gtk_widget_queue_draw(priv->slices[i].drawing_area);

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
