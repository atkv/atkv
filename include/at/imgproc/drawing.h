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
#ifndef AT_IMGPROC_DRAWING_H
#define AT_IMGPROC_DRAWING_H

#include <at/imgproc.h>

G_BEGIN_DECLS
typedef enum{
  AT_COLOR_GRAY,
  AT_COLOR_RGB,
  AT_COLOR_ARGB,
  AT_COLOR_BGRA,
}AtColorFormat;

typedef enum{
  AT_INTERPOLATION_NEAREST,
  AT_INTERPOLATION_LINEAR,
  AT_INTERPOLATION_AREA,
  AT_INTERPOLATION_CUBIC,
  AT_INTERPOLATION_LANCZOS4,
}AtInterpolationMode;

typedef enum{
  AT_LINE_TYPE_4,
  AT_LINE_TYPE_8,
  AT_LINE_TYPE_AA
}AtLineType;

/**
 * @brief Convert an image between formats
 * @param image
 * @param from
 * @param to
 * @return
 */
AtArray_uint8_t*
at_cvt_color(AtArray_uint8_t* image, AtColorFormat from, AtColorFormat to);

/**
 * @brief Scale an image based on a interpolation
 * @param image
 * @param scale
 * @param interpolation
 * @return
 */
AtArray_uint8_t*
at_array_uint8_t_scale(AtArray_uint8_t* image, double scale, AtInterpolationMode interpolation);

/**
 * @brief Draw a line from p0 to p1 by using a color, thickness and line_type
 * @param array
 * @param p0
 * @param p1
 * @param color
 * @param thickness
 * @param line_type
 * @param shift
 */
void
at_array_uint8_t_draw_line(AtArray_uint8_t* array,
                           AtVec2i p0,
                           AtVec2i p1,
                           AtVec4i* color,
                           int thickness,
                           AtLineType line_type,
                           int shift);

/**
 * @brief at_clip_line
 * @param size_scaled
 * @param p1
 * @param p2
 * @return
 */
uint8_t
at_clip_line(AtVec2i size_scaled, AtVec2i* p1, AtVec2i* p2);

/**
 * @brief at_array_uint8_t_draw_circle
 * @param array
 * @param center
 * @param radius
 * @param color
 * @param thickness
 * @param line_type
 * @param shift
 */
void
at_array_uint8_t_draw_circle(AtArray_uint8_t* array,
                             AtVec2i center,
                             int radius,
                             AtVec4i* color,
                             int thickness,
                             AtLineType line_type,
                             int shift);

/**
 * @brief at_array_uint8_t_draw_ellipse
 * @param array
 * @param center
 * @param axes
 * @param angle
 * @param start_angle
 * @param end_angle
 * @param color
 * @param thickness
 * @param line_type
 * @param shift
 */
void
at_array_uint8_t_draw_ellipse(AtArray_uint8_t* array,
                              AtVec2 center,
                              AtVec2 axes,
                              double angle,
                              double start_angle,
                              double end_angle,
                              AtVec4i* color,
                              int thickness,
                              AtLineType line_type,
                              int shift);

/**
 * @brief at_array_uint8_t_draw_rectangle
 * @param array
 * @param rect
 * @param color
 * @param thickness
 * @param lineType
 * @param shift
 */
void
at_array_uint8_t_draw_rectangle(AtArray_uint8_t* array,
                                AtVec4i rect,
                                AtVec4i* color,
                                int thickness,
                                AtLineType lineType,
                                int shift);

/**
 * @brief at_ellipse_to_poly
 * @param center
 * @param axes
 * @param angle
 * @param arc_start
 * @param arc_end
 * @param delta
 * @param pts
 * @param count
 */
void
at_ellipse_to_poly(AtVec2i center,
                   AtVec2i axes,
                   int angle,
                   int arc_start,
                   int arc_end,
                   int delta,
                   AtVec2i** pts,
                   int *count);

/**
 * @brief at_array_uint8_t_draw_rectangle_2
 * @param array
 * @param top_left
 * @param bottom_right
 * @param color
 * @param thickness
 * @param line_type
 * @param shift
 */
void
at_array_uint8_t_draw_rectangle_2(AtArray_uint8_t* array,
                                  AtVec2i top_left,
                                  AtVec2i bottom_right,
                                  AtVec4i* color,
                                  int thickness,
                                  AtLineType line_type,
                                  int shift);

G_END_DECLS
#endif
