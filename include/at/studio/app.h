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

#ifndef AT_STUDIO_APP_H
#define AT_STUDIO_APP_H
#include <gtk/gtk.h>
/* ============================================================================
 * STRUCTURE
 * ==========================================================================*/
#define AT_TYPE_STUDIO_APP at_studio_app_get_type()
G_DECLARE_DERIVABLE_TYPE(AtStudioApp, at_studio_app, AT, STUDIO_APP, GObject)
typedef struct _AtStudioAppClass{
  GObjectClass parent_class;
}AtStudioAppClass;

/* ============================================================================
 * FUNCTIONS
 * ==========================================================================*/
/**
 * @brief at_studio_app_new
 * @return
 */
AtStudioApp*
at_studio_app_new();
/**
 * @brief at_studio_app_run
 * @param app
 * @param argc
 * @param argv
 * @return
 */
int
at_studio_app_run(AtStudioApp* app, int argc, char** argv);
#endif
