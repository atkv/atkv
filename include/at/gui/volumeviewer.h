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

#ifndef AT_GUI_VOLUMEVIEWER_H
#define AT_GUI_VOLUMEVIEWER_H
#include <at/gui.h>
G_BEGIN_DECLS

typedef enum _AtSliceSide{
  AT_SLICE_LEFT,
  AT_SLICE_RIGHT,
  AT_SLICE_FRONT,
  AT_SLICE_BACK,
  AT_SLICE_UP,
  AT_SLICE_DOWN
}AtSliceSide;

#define AT_TYPE_VOLUMEVIEWER at_volumeviewer_get_type()
G_DECLARE_DERIVABLE_TYPE(AtVolumeViewer, at_volumeviewer, AT, VOLUMEVIEWER, GObject)

typedef struct _AtVolumeViewerClass{
  GObjectClass parent_class;
}AtVolumeViewerClass;

AtVolumeViewer*
at_volumeviewer_new();

/**
 * @brief at_volumeviewer_new_with_name
 * @param name
 * @return
 */
AtVolumeViewer*
at_volumeviewer_new_with_name(char* name);

/**
 * @brief at_volumeviewer_show_nifti
 * @param nifti_image
 * @return
 */
AtVolumeViewer*
at_volumeviewer_show_nifti(AtVolumeViewer* volumeviewer, AtNiftiImage* nifti_image);

/**
 * @brief at_volumeviewer_waitkey
 * @return
 */
guint
at_volumeviewer_waitkey();

/**
 * @brief at_volumeviewer_set_mouse_callback
 * @param volumeviewer
 * @param mouse_cb
 */
void
at_volumeviewer_set_mouse_callback(AtVolumeViewer* volumeviewer, AtMouseCallbackFunc mouse_cb, gpointer user_data);

/**
 * @brief at_volumeviewer_set_key_callback
 * @param volumeviewer
 * @param key_cb
 */
void
at_volumeviewer_set_key_callback(AtVolumeViewer* volumeviewer, AtKeyCallbackFunc key_cb, gpointer user_data);

/**
 * @brief at_volumeviewer_hide
 * @param volumeviewer
 */
void
at_volumeviewer_hide(AtVolumeViewer* volumeviewer);

/**
 * @brief at_volumeviewer_get_name
 * @param volumeviewer
 * @return
 */
char*
at_volumeviewer_get_name(AtVolumeViewer* volumeviewer);

/**
 * @brief at_volumeviewer_get_zoom
 * @param volumeviewer
 * @param slice_side
 * @return
 */
double
at_volumeviewer_get_zoom(AtVolumeViewer* volumeviewer, AtSliceSide slice_side);

/**
 * @brief at_volumeviewer_move
 * @param volumeviewer
 * @param x
 * @param y
 */
void
at_volumeviewer_move(AtVolumeViewer* volumeviewer, int x, int y);

/**
 * @brief at_volumeviewer_zoom_in
 * @param volumeviewer
 * @param increment
 * @param slice_side
 */
void
at_volumeviewer_zoom_in(AtVolumeViewer* volumeviewer, double increment, AtSliceSide slice_side);

/**
 * @brief at_volumeviewer_zoom_out
 * @param volumeviewer
 * @param increment
 * @param slice_side
 */
void
at_volumeviewer_zoom_out(AtVolumeViewer* volumeviewer, double increment, AtSliceSide slice_side);

/**
 * @brief at_volumeviewer_zoom_reset
 * @param volumeviewer
 * @param slice_side
 */
void
at_volumeviewer_zoom_reset(AtVolumeViewer* volumeviewer, AtSliceSide slice_side);




G_END_DECLS
#endif
