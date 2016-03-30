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

#ifndef AT_GUI_IMAGEVIEWER_H
#define AT_GUI_IMAGEVIEWER_H
#include <at/gui.h>
#include <gtk/gtk.h>
#include <at/imgproc.h>

G_BEGIN_DECLS
#define AT_TYPE_IMAGEVIEWER at_imageviewer_get_type()
G_DECLARE_DERIVABLE_TYPE(AtImageViewer, at_imageviewer, AT, IMAGEVIEWER, GObject)
typedef struct _AtImageViewerClass{
  GObjectClass parent_class;
}AtImageViewerClass;


typedef enum{
  // Mouse Buttons
  AT_MOUSE_FLAG_LBUTTON  = 0,
  AT_MOUSE_FLAG_RBUTTON  = 1,
  AT_MOUSE_FLAG_MBUTTON  = 2,

  // Modifiers
  AT_MOUSE_FLAG_CTRLKEY  = 4,
  AT_MOUSE_FLAG_SHIFTKEY = 8,
  AT_MOUSE_FLAG_ALTKEY   = 16,
} AtMouseEventFlags;

typedef enum{
  AT_MOUSE_EVENT_MOVE    = 1,
  AT_MOUSE_EVENT_PRESS   = 2,
  AT_MOUSE_EVENT_RELEASE = 4,
  AT_MOUSE_EVENT_DBLCLK  = 8,
  AT_MOUSE_EVENT_WHEEL   = 16
} AtMouseEventType;
#define AT_MOUSE_FLAG_MASK     3

typedef enum{
  AT_KEY_EVENT_PRESS,
  AT_KEY_EVENT_RELEASE,
} AtKeyEventType;

typedef enum{
  AT_POSITION_START,
  AT_POSITION_CENTER,
  AT_POSITION_END
}AtPlacementFlag;

typedef void (*AtMouseCallbackFunc)(AtMouseEventType event, int x, int y, AtMouseEventFlags flags, void* user_data);
typedef void (*AtKeyCallbackFunc)(AtKeyEventType event, int key_val, void* user_data);


/**
 * @brief Create a new AtImageViewer object.
 *
 * Don't forget to use g_clear_object to destroy the object.
 *
 * @return a new AtImageViewer object
 */
AtImageViewer*
at_imageviewer_new();

/**
 * @brief at_imageviewer_new_with_name
 * @param name
 * @return
 */
AtImageViewer*
at_imageviewer_new_with_name(char* name);

/**
 * @brief at_imageviewer_set_name
 * @param imageviewer
 * @param name
 */
void
at_imageviewer_set_name(AtImageViewer* imageviewer, char* name);

/**
 * @brief at_imageviewer_get_name
 * @param imageviewer
 * @return
 */
char*
at_imageviewer_get_name(AtImageViewer* imageviewer);

/**
 * @brief at_imageviewer_show_uint8_t
 * @param imageviewer
 * @param array
 */
void
at_imageviewer_show_uint8_t(AtImageViewer* imageviewer, AtArray_uint8_t* array);

/**
 * @brief at_imageviewer_show_by_name
 * @param name
 * @param array
 */
void
at_imageviewer_show_by_name_uint8_t(char* name, AtArray_uint8_t* array);

/**
 * @brief at_imageviewer_hide
 * @param imageviewer
 */
void
at_imageviewer_hide(AtImageViewer* imageviewer);

/**
 * @brief at_imageviewer_set_mouse_callback
 * @param imageviewer
 * @param mouse_cb
 */
void
at_imageviewer_set_mouse_callback(AtImageViewer* imageviewer, AtMouseCallbackFunc mouse_cb);

/**
 * @brief at_imageviewer_set_key_callback
 * @param imageviewer
 * @param key_cb
 */
void
at_imageviewer_set_key_callback(AtImageViewer* imageviewer, AtKeyCallbackFunc key_cb);
/**
 * @brief at_imageviewer_waitkey
 * @param imageviewer
 */
void
at_imageviewer_waitkey();

/**
 * @brief at_imageviewer_set_zoom
 * @param imageviewer
 * @param scale
 */
void
at_imageviewer_set_zoom(AtImageViewer* imageviewer, double scale);
/**
 * @brief at_imageviewer_get_zoom
 * @param imageviewer
 * @param scale
 * @return
 */
double
at_imageviewer_get_zoom(AtImageViewer* imageviewer, double scale);
/**
 * @brief at_imageviewer_reset_zoom
 * @param imageviewer
 */
void
at_imageviewer_reset_zoom(AtImageViewer* imageviewer);
/**
 * @brief at_imageviewer_zoom_in
 * @param imageviewer
 * @param scale_increments
 */
void
at_imageviewer_zoom_in(AtImageViewer* imageviewer, double scale_increments);
/**
 * @brief at_imageviewer_zoom_out
 * @param imageviewer
 * @param scale_increments
 */
void
at_imageviewer_zoom_out(AtImageViewer* imageviewer, double scale_increments);
/**
 * @brief at_imageviewer_zoom_to_fit
 * @param imageviewer
 */
void
at_imageviewer_zoom_to_fit(AtImageViewer* imageviewer);
/**
 * @brief at_imageviewer_translate
 * @param imageviewer
 * @param x
 * @param y
 */
void
at_imageviewer_translate(AtImageViewer* imageviewer, int x, int y);
/**
 * @brief at_imageviewer_translate_special
 * @param imageviewer
 * @param placement_flag_x
 * @param placement_flag_y
 */
void
at_imageviewer_translate_special(AtImageViewer* imageviewer, AtPlacementFlag placement_flag_x, AtPlacementFlag placement_flag_y);
/**
 * @brief at_imageviewer_resize_to_content
 * @param imageviewer
 */
void
at_imageviewer_resize_to_content(AtImageViewer* imageviewer);
/**
 * @brief at_imageviewer_resize
 * @param imageviewer
 * @param width
 * @param height
 */
void
at_imageviewer_resize(AtImageViewer* imageviewer, int width, int height);
/**
 * @brief at_imageviewer_move
 * @param imageviewer
 * @param x
 * @param y
 */
void
at_imageviewer_move(AtImageViewer* imageviewer, int x, int y);


G_END_DECLS
#endif
