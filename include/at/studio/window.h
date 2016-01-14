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

#ifndef AT_STUDIO_APP_WINDOW_H
#define AT_STUDIO_APP_WINDOW_H
#include <gtk/gtk.h>
/* ============================================================================
 * STRUCTURE
 * ==========================================================================*/
#define AT_TYPE_STUDIO_APP_WINDOW at_studio_app_window_get_type()
G_DECLARE_DERIVABLE_TYPE(AtStudioAppWindow, at_studio_app_window, AT, STUDIO_APP_WINDOW, GObject)
typedef struct _AtStudioAppWindowClass{
  GObjectClass parent_class;
}AtStudioAppWindowClass;
/* ============================================================================
 * FUNCTIONS
 * ==========================================================================*/
AtStudioAppWindow*
at_studio_app_window_new(GtkApplication* app);

void
at_studio_app_window_show(AtStudioAppWindow* window);

void
at_studio_app_window_load_image_view();

void
at_studio_app_window_load_slice_view();

void
at_studio_app_window_load_graph_view();

#endif
