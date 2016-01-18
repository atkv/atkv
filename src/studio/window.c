#include <at/studio/window.h>
#include <at/core.h>
#include <glib/gi18n.h>
/* ============================================================================
 * PRIVATE API
 * ==========================================================================*/
typedef struct _AtStudioAppWindowPrivate{
  GtkApplicationWindow* window;
  GtkApplication* app;
  GtkCssProvider* css_provider;
  GtkBuilder* builder;
  GtkNotebook* views;
  GdkPixbuf* logo;
  gchar* authors[2];
  gchar* artists[2];
}AtStudioAppWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtStudioAppWindow, at_studio_app_window, G_TYPE_OBJECT)

void
mnu_about_activate_cb(GtkMenuItem* item, gpointer user_data){
  AtStudioAppWindow* app_window = user_data;
  AtStudioAppWindowPrivate* priv = at_studio_app_window_get_instance_private(app_window);


  gtk_show_about_dialog(GTK_WINDOW(priv->window),
                        "program-name","@Studio",
                        "title",_("About @Studio"),
                        "website","http://studio.atkv.io",
                        "website-label","http://studio.atkv.io",
                        "copyright","All rights reserved",
                        "license-type",GTK_LICENSE_GPL_3_0,
                        "version",AT_VERSION_STRING,
                        "logo",priv->logo,
                        "artists",priv->artists,
                        "authors",priv->authors,
                        NULL);
}

static void
at_studio_app_window_load_interface(AtStudioAppWindow* app_window,
                                    char* ui_filename,
                                    char* style_filename){
  AtStudioAppWindowPrivate* priv = at_studio_app_window_get_instance_private(app_window);
  priv->css_provider = gtk_css_provider_new();
  priv->builder = gtk_builder_new_from_file(ui_filename);
  gtk_builder_connect_signals(priv->builder, app_window);
  priv->logo = gdk_pixbuf_new_from_file("../data/icons/studio_logo.png",NULL);
  gtk_css_provider_load_from_path(priv->css_provider,style_filename,NULL);
  priv->window = GTK_APPLICATION_WINDOW(gtk_builder_get_object(priv->builder, "at_studio_app_window"));
  priv->views = GTK_NOTEBOOK(gtk_notebook_new());
  priv->artists[0] = "Anderson Tavares";
  priv->authors[0] = "Anderson Tavares";
  priv->authors[1] = NULL;
  priv->artists[1] = NULL;

  GtkBox* box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0));
  gtk_box_pack_start(box, gtk_image_new_from_icon_name("document-open",GTK_ICON_SIZE_SMALL_TOOLBAR), TRUE ,TRUE  , 0);
  gtk_box_pack_end(box, gtk_button_new_from_icon_name("document-close",GTK_ICON_SIZE_SMALL_TOOLBAR), FALSE, FALSE, 0);
  gtk_widget_show_all(GTK_WIDGET(box));
  gtk_notebook_append_page(priv->views,gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0),GTK_WIDGET(box));
  gtk_notebook_append_page(priv->views,gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0),gtk_label_new("Carlos"));
  GtkPaned* pnl_main = GTK_PANED(gtk_builder_get_object(priv->builder,"pnl_main"));
  gtk_paned_add1(pnl_main,GTK_WIDGET(priv->views));

  const char* components[5] = {"tlb_main","pnl_main","btn_selection","btn_segmentation","btn_transformation"};
  GtkWidget* component;
  GtkStyleContext *context;
  u_int8_t i;
  for(i = 0; i < 5; i++){
    component = GTK_WIDGET(gtk_builder_get_object(priv->builder, components[i]));
    context = gtk_widget_get_style_context(component);
    gtk_style_context_add_provider(context,GTK_STYLE_PROVIDER(priv->css_provider),GTK_STYLE_PROVIDER_PRIORITY_USER);
  }

  //GtkImageMenuItem* mnu_about = gtk_builder_get_object(priv->builder, "mnu_about");

}

static void
at_studio_app_window_init(AtStudioAppWindow *self){
  at_studio_app_window_load_interface(self, "../data/studio_main.ui", "../data/studio.css");
}

static void
at_studio_app_window_class_init(AtStudioAppWindowClass *klass){

}
/* ============================================================================
 * PUBLIC API
 * ==========================================================================*/
AtStudioAppWindow*
at_studio_app_window_new(GtkApplication* app){
  AtStudioAppWindow* app_window = g_object_new(AT_TYPE_STUDIO_APP_WINDOW,NULL);
  AtStudioAppWindowPrivate* priv = at_studio_app_window_get_instance_private(app_window);
  priv->app = app;
  gtk_window_set_application(GTK_WINDOW(priv->window),priv->app);
  return app_window;
}

void
at_studio_app_window_show(AtStudioAppWindow* window){
  AtStudioAppWindowPrivate* priv = at_studio_app_window_get_instance_private(window);
  gtk_widget_show_all(GTK_WIDGET(priv->window));
}
