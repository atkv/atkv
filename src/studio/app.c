#include <at/studio/app.h>
#include <at/studio/window.h>
/* ============================================================================
 * PRIVATE API
 * ==========================================================================*/
typedef struct _AtStudioAppPrivate{
  GtkApplication*   app;
  AtStudioAppWindow* window;
}AtStudioAppPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtStudioApp, at_studio_app, G_TYPE_OBJECT)

static void
at_studio_app_activate(GtkApplication* app, gpointer user_data)
{
  AtStudioApp* studio_app = user_data;
  AtStudioAppPrivate* priv = at_studio_app_get_instance_private(studio_app);
  priv->window = at_studio_app_window_new(app);
  at_studio_app_window_show(priv->window);
}

static void
at_studio_app_init(AtStudioApp *self){
  AtStudioAppPrivate* priv = at_studio_app_get_instance_private(self);
  priv->app = gtk_application_new("io.at.studio",G_APPLICATION_FLAGS_NONE);
  priv->window = NULL;
  g_signal_connect(priv->app, "activate", G_CALLBACK(at_studio_app_activate), self);
}

static void
at_studio_app_class_init(AtStudioAppClass *klass){

}

/* ============================================================================
 * PUBLIC API
 * ==========================================================================*/
AtStudioApp*
at_studio_app_new(){
  return g_object_new(AT_TYPE_STUDIO_APP, NULL);
}

int
at_studio_app_run(AtStudioApp* app, int argc, char** argv){
  AtStudioAppPrivate* priv = at_studio_app_get_instance_private(app);
  g_application_run(G_APPLICATION(priv->app),argc, argv);
}
