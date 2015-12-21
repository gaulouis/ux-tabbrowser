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

#ifndef UX_PARSER_H
#define UX_PARSER_H

#include <glib-object.h>

#include "ux-style.h"

#define ux_parser_is_alpha(c)  ( ('a' <= (c) && (c) <= 'z') || ('A' <= (c) && (c) <= 'Z') )
#define ux_parser_is_digit(c)  ( '0' <= (c) && (c) <= '9' )
#define ux_parser_is_space(c)  ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define ux_parser_skip_space(ptr, end) while (ptr<=end && ux_parser_is_space(*ptr)) ptr++
#define ux_parser_skip_separator(ptr, end) while (ptr<=end && (ux_parser_is_space(*ptr) || *ptr == ',' || *ptr == ';')) ptr++

gchar* ux_parser_parse_color(UxStyleColor *color, gchar *ptr, gchar *end);
gchar* ux_parser_parse_image(UxStyleImage *image, gchar *ptr, gchar *end);
gchar* ux_parser_parse_background_position(UxStyleBackgroundPosition *position, gchar *ptr, gchar *end);
gchar* ux_parser_parse_background_repeat(UxStyleBackgroundRepeat *repeat, gchar *ptr, gchar *end);
gchar* ux_parser_parse_path(UxStylePath *path, gchar *ptr, gchar *end);


#endif // UX_PARSER_H
