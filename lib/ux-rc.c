/* UX - TabBrowser
 * Copyright (C) 2015 {Aka} Gaulouis
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "ux-rc.h"
#include "ux-style.h"
#include "ux-parser.h"


gboolean ux_rc_parse_color(const GParamSpec *pspec,
                           const GString    *rc_string,
                           GValue           *property_value)
{
    gchar *ptr = rc_string->str;
    gchar *end = rc_string->str + rc_string->len;
    while (ptr < end && (ux_parser_is_space(*ptr)||*ptr=='"') )
        ptr++;
    while (end > ptr && (ux_parser_is_space(*end)||*end=='"') )
        end--;

    UxStyleColor *color = g_new(UxStyleColor, 1);
    ux_style_color_init(color);
    ptr = ux_parser_parse_color(color, ptr, end);

    g_value_set_boxed(property_value, color);

    return TRUE;
}

gboolean ux_rc_parse_background(const GParamSpec *pspec,
                                const GString    *rc_string,
                                GValue           *property_value)
{
    gchar *ptr = rc_string->str;
    gchar *end = rc_string->str + rc_string->len;
    while (ptr < end && (ux_parser_is_space(*ptr)||*ptr=='"') )
        ptr++;
    while (end > ptr && (ux_parser_is_space(*end)||*end=='"') )
        end--;

    UxStyleBackground *background = g_new(UxStyleBackground, 1);
    ux_style_background_init(background);

    UxStyleColor *color = &background->color;
    ptr = ux_parser_parse_color(color, ptr, end);

    UxStyleImage *image = &background->image;
    ptr = ux_parser_parse_image(image, ptr, end);

    UxStyleBackgroundPosition *position = &background->position;
    ptr = ux_parser_parse_background_position(position, ptr, end);

    UxStyleBackgroundRepeat *repeat = &background->repeat;
    ptr = ux_parser_parse_background_repeat(repeat, ptr, end);

    //UxStyleImage *image = &background->repeat;// repeat
    //UxStyleImage *image = &background->size;// cover
    //UxStyleImage *image = &background->hint = bit pixel;


    g_value_set_boxed(property_value, background);
//    UxStyleBackground *bg;
//    g_value_take_boxed(property_value, bg);// take_owner_ship

    return TRUE;
}

/*
gboolean ux_rc_parse_border_image(const GParamSpec *pspec,
                           const GString    *rc_string,
                           GValue           *property_value)
{

    gchar *ptr = rc_string->str;
    gchar *end = rc_string->str + rc_string->len;
    while (ptr < end && (ux_parser_is_space(*ptr)||*ptr=='"') )
        ptr++;
    while (end > ptr && (ux_parser_is_space(*end)||*end=='"') )
        end--;

    UxStyleBorderImage *border_image = g_new(UxStyleBorderImage, 1);
    ux_style_background_init(border_image);

    UxStyleColor *color = &background->color;
    ptr = ux_parser_parse_color(color, ptr, end);

    UxStyleImage *image = &background->image;
    ptr = ux_parser_parse_image(image, ptr, end);

    g_value_set_boxed(property_value, background);

    return TRUE;
}
*/

gboolean ux_rc_parse_path(const GParamSpec *pspec,
                          const GString    *rc_string,
                          GValue           *property_value)
{
    gchar *ptr = rc_string->str;
    gchar *end = rc_string->str + rc_string->len;
    while (ptr < end && (ux_parser_is_space(*ptr)||*ptr=='"') )
        ptr++;
    //while (end > ptr && (ux_rc_parser_is_space(*end)||*end=='"') )
        end--;

    UxStylePath *path = g_new(UxStylePath, 1);
    ux_style_path_init(path);

    ptr = ux_parser_parse_path(path, ptr, end);


    g_value_set_boxed(property_value, path);

    return TRUE;
}
