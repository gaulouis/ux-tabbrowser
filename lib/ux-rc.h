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

#ifndef UX_RC_PARSER_H
#define UX_RC_PARSER_H

#include <gtk/gtk.h>
//#include <glib-object.h>

gboolean ux_rc_parse_color(const GParamSpec *pspec,
                             const GString    *rc_string,
                             GValue           *property_value);

/**
 * Parse GtkRC rule
 * See CSS3 syntaxe for more explanation.
 * Also, you can superpose backgrounds by adding rules separated by a coma
 * Example:
 * \code
 * UxTabrowser::background = "#FF0000"
 * UxTabrowser::background = "rgba(1.0, 1.0, 1.0, 1.0) none"
 * UxTabrowser::background = "rgba(255, 255, 255, 255) url('img.png') left top repeat no-repeat"
 * UxTabrowser::background = "transparent url('img.png') center no-repeat"
 * UxTabrowser::background = "transparent url('img.png') center no-repeat, transparent url('img.png') center no-repeat"
 * UxTabrowser::background = "transparent linear-gradient(0px #00FF00, 100% #FF0000) left top"
 * \endcode
 *
 * @brief ux_rc_parser_background
 * @param pspec GParamSpec
 * @param rc_string GString
 * @param property_value GValue
 * @return
 */
gboolean ux_rc_parse_background(const GParamSpec *pspec,
                                 const GString    *rc_string,
                                 GValue           *property_value);

/**
 * Parse GtkRC rule
 * See CSS3 syntaxe for more explanation.
 * Also, you can superpose backgrounds by adding rules separated by a coma
 * Example:
 * \code
 * UxTabrowser::tab-border-image = "linear-gradient(0px #00FF00, 100% #FF0000) left top"
 * UxTabrowser::tab-border-image-source = "url()" TODO
 * UxTabrowser::tab-border-image-repeat = "" TODO
 *
 * \endcode
 *
 * @brief ux_rc_parser_background
 * @param pspec GParamSpec
 * @param rc_string GString
 * @param property_value GValue
 * @return
 *
gboolean ux_rc_parse_border_image(const GParamSpec *pspec,
                                  const GString    *rc_string,
                                  GValue           *property_value);
*/

/**
 * Parse GtkRC rule
 * See CSS3 syntaxe for more explanation.
 * Also, you can superpose backgrounds by adding rules separated by a coma
 * Example:
 * \code
 * UxTabrowserPage::border-path = "m 0.0,0.0 l 1.0,0.0 1.0,1.0 0.0,1.0 z"
 * UxTabrowserPage::border-top-path = ""
 * UxTabrowserPage::border-right-path = ""
 * UxTabrowserPage::border-bottom-path = ""
 * UxTabrowserPage::border-left-path = ""
 * \endcode
 *
 * @brief ux_rc_parser_path
 * @param pspec GParamSpec
 * @param rc_string GString
 * @param property_value GValue
 * @return
 */
gboolean ux_rc_parse_path(const GParamSpec *pspec,
                            const GString    *rc_string,
                            GValue           *property_value);

void ux_rc_parser_set_context(GtkWidget *widget);/// \FIXME



#endif // UX_RC_PARSER_H
