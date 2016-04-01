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

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtVolumeViewerPrivate{
  AtNiftiImage*       nifti_image;
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
at_volumeviewer_create_window(AtVolumeViewer* volumeviewer){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(volumeviewer);
  priv->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(priv->window, "destroy", G_CALLBACK(gtk_main_quit), volumeviewer);
  priv->grid = gtk_grid_new();
}

static void
at_volumeviewer_init(AtVolumeViewer *self){
  AtVolumeViewerPrivate* priv = at_volumeviewer_get_instance_private(self);
  priv->nifti_image = NULL;
  at_volumeviewer_create_window(self);
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
