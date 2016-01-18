#include <gtk/gtk.h>
#include <at/studio/app.h>
int main(int argc, char ** argv){
  g_autoptr(AtStudioApp) studio_app = at_studio_app_new();
  return at_studio_app_run(studio_app, argc, argv);
}
