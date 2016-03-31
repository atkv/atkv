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

  char*               name;
  AtMouseCallbackFunc mouse_cb;
  AtKeyCallbackFunc   key_cb;
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

  GtkWidget*          statusbar_box;
  GtkWidget*          statusbar_lbl;

  GtkWidget*          viewport;
  GtkWidget*          drawing_area;
  GtkWidget*          scrolled_window;
}AtImageViewerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtImageViewer, at_imageviewer, G_TYPE_OBJECT)
static void
at_imageviewer_dispose(GObject* object){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(AT_IMAGEVIEWER(object));
  g_clear_object(&priv->window);
  G_OBJECT_CLASS(at_imageviewer_parent_class)->dispose(object);
}
static void
at_imageviewer_finalize(GObject* object){
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

static void
at_imageviewer_create_window(AtImageViewer* self){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);
  priv->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  priv->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(priv->window),priv->vbox);
  at_imageviewer_quit_on_destroy(self,TRUE);
  at_imageviewer_create_toolbar(self);
  at_imageviewer_create_statusbar(self);
}

static void
at_imageviewer_init(AtImageViewer *self){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);
  priv->name = NULL;
  priv->key_cb = NULL;
  priv->mouse_cb = NULL;
  at_imageviewer_create_window(self);
}

static void
at_imageviewer_create_drawing_area(AtImageViewer* self){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);
  priv->drawing_area = gtk_drawing_area_new();
  priv->scrolled_window = gtk_scrolled_window_new(NULL,NULL);
  priv->viewport = gtk_viewport_new(NULL,NULL);
  gtk_container_add(GTK_CONTAINER(priv->scrolled_window), priv->viewport);
  gtk_container_add(GTK_CONTAINER(priv->viewport), priv->drawing_area);
  gtk_box_pack_start(GTK_BOX(priv->vbox),priv->scrolled_window,TRUE,TRUE,0);
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
  gtk_main();
}

void
at_imageviewer_show_uint8_t(AtImageViewer* imageviewer, AtArray_uint8_t* array){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imageviewer);
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
