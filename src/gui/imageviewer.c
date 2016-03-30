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

typedef struct _AtImageViewerPrivate{
  GtkWindow* window;
  char* name;
  AtMouseCallbackFunc mouse_cb;
  AtKeyCallbackFunc key_cb;
}AtImageViewerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtImageViewer, at_imageviewer, G_TYPE_OBJECT)
static void
at_imageviewer_dispose(GObject* object){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(AT_IMAGEVIEWER(object));
  g_free(priv->name);
  G_OBJECT_CLASS(at_imageviewer_parent_class)->dispose(object);
}

static void
at_imageviewer_class_init(AtImageViewerClass *klass){
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = at_imageviewer_dispose;
}
static void
at_imageviewer_init(AtImageViewer *self){

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
}

void
at_imageviewer_set_name(AtImageViewer* imageviewer, char* name){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  priv->name = name;
}
void
at_imageviewer_set_mouse_callback(AtImageViewer* imageviewer, AtMouseCallbackFunc mouse_cb){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  priv->mouse_cb = mouse_cb;
}

void
at_imageviewer_set_key_callback(AtImageViewer* imageviewer, AtKeyCallbackFunc key_cb){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
  priv->key_cb = key_cb;
}

void
at_imageviewer_waitkey(){

}

void
at_imageviewer_show_uint8_t(AtImageViewer* imageviewer, AtArray_uint8_t* array){

}
