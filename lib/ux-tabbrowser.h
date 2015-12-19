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


#ifndef __UX_TABBROWSER_H__
#define __UX_TABBROWSER_H__


#include <gtk/gtk.h>

G_BEGIN_DECLS

#define UX_TYPE_TABBROWSER                  (ux_tabbrowser_get_type ())
#define UX_TABBROWSER(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), UX_TYPE_TABBROWSER, UxTabbrowser))
#define UX_TABBROWSER_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), UX_TYPE_TABBROWSER, UxTabbrowserClass))
#define UX_IS_TABBROWSER(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), UX_TYPE_TABBROWSER))
#define UX_IS_TABBROWSER_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), UX_TYPE_TABBROWSER))
#define UX_TABBROWSER_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), UX_TYPE_TABBROWSER, UxTabbrowserClass))



typedef struct _UxTabbrowser       UxTabbrowser;
typedef struct _UxTabbrowserClass  UxTabbrowserClass;

struct _UxTabbrowser
{
  GtkNotebook parent_instance;
};

struct _UxTabbrowserClass
{
  GtkNotebookClass parent_class;
};

GType       ux_tabbrowser_get_type (void) G_GNUC_CONST;
GtkWidget  *ux_tabbrowser_new      (void);

G_END_DECLS

#endif /* __UX_TABBROWSER_H__ */
