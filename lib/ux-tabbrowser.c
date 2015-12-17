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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

//#include "mwb-types.h"
//#include "mwb-enums.h"
//#include "mwb-marshalers.h"
//#include "mwb-style.h"
//#include "mwb-parser.h"
//#include "mwb-rc.h"
//#include "mwb-graphics.h"
//#include "mwb-display.h"
//#include "mwb-notebook_p.h"
#include "ux-tabbrowser.h"


G_DEFINE_TYPE (UxTabbrowser, ux_tabbrowser, GTK_TYPE_NOTEBOOK)

static void
ux_tabbrowser_class_init (UxTabbrowserClass *klass)
{
  GObjectClass   *gobject_class = G_OBJECT_CLASS (klass);
  GtkObjectClass *object_class = GTK_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
  GtkContainerClass *container_class = GTK_CONTAINER_CLASS (klass);
  GtkNotebookClass *notebook_class = GTK_NOTEBOOK_CLASS(klass);

}

static void
ux_tabbrowser_init (UxTabbrowser *tabbrowser)
{

}
