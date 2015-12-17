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

#include "ux-tabbrowser.h"
#include "glade-ux.h"

//#include "fixed_bg.xpm"

#include <gladeui/glade-editor-property.h>
#include <gladeui/glade-base-editor.h>

#include <gtk/gtk.h>
#include <glib/gi18n-lib.h>
#include <string.h>
#include <stdlib.h>

/* --------------------------------- Constants ------------------------------ */
#define FIXED_DEFAULT_CHILD_WIDTH  100
#define FIXED_DEFAULT_CHILD_HEIGHT 60

/*
void GLADE_UX_API
// GladeSetPropertyFunc
glade_ux_set_dimension (
    GObject      *object,
    const GValue *value)
{
    if (glade_ux_verify_dimension (object, value)) {

    }
}

gboolean GLADE_UX_API
// GladeVerifyPropertyFunc
glade_ux_verify_dimension (
    GObject      *object,
    const GValue *value)
{
    // g_return_val_if_fail (
}




static GType
glade_gtk_gnome_ui_info_get_type (void)
{
    static GType etype = 0;
    if (etype == 0) {
        static const GEnumValue values[] = {
            { GNOMEUIINFO_MENU_NONE, "GNOMEUIINFO_MENU_NONE", NULL},
            { GNOMEUIINFO_MENU_NEW_ITEM, "GNOMEUIINFO_MENU_NEW_ITEM", "gtk-new"},
            { GNOMEUIINFO_MENU_OPEN_ITEM, "GNOMEUIINFO_MENU_OPEN_ITEM", "gtk-open"},
            { GNOMEUIINFO_MENU_SAVE_ITEM, "GNOMEUIINFO_MENU_SAVE_ITEM", "gtk-save"},
            { 0, NULL, NULL }
        };
        etype = g_enum_register_static ("GladeGtkGnomeUIInfo", values);
    }
    return etype;
}

GParamSpec *
glade_ux_gnome_ui_info_spec (void)
{
    return g_param_spec_enum ("uxgnomeuiinfo", _("UxGnomeUIInfo"),
                  _("Choose the UxGnomeUIInfo stock item"),
                  glade_ux_gnome_ui_info_get_type (),
                  0, G_PARAM_READWRITE);
}
*/

GType glade_ux_tabbrowser_type_get_type (void) {
    GType type = UX_TYPE_TABBROWSER;

    return type;
}

/* Track project selection to set the notebook pages to display
 * the selected widget.
 */
static void
glade_gtk_notebook_selection_changed (GladeProject *project,
                                      GladeWidget  *gwidget)
{
    GladeWidget *selected;
    GList       *list;
    gint         i;
    GtkWidget   *page;

    if ((list = glade_project_selection_get (project)) != NULL &&
        g_list_length (list) == 1)
    {
        selected = glade_widget_get_from_gobject (list->data);

        /* Check if selected widget is inside the notebook */
        if (GTK_IS_WIDGET (selected->object) &&
            gtk_widget_is_ancestor (GTK_WIDGET (selected->object),
                        GTK_WIDGET (gwidget->object)))
        {
            /* Find and activate the page */
            for (i = 0; i < gtk_notebook_get_n_pages (GTK_NOTEBOOK (gwidget->object)); i++)
            {
                page = gtk_notebook_get_nth_page (GTK_NOTEBOOK (gwidget->object), i);
                if (GTK_WIDGET (selected->object) == page ||
                    gtk_widget_is_ancestor (GTK_WIDGET (selected->object),
                                GTK_WIDGET (page)))
                {
                    glade_widget_property_set (gwidget, "page", i);
                    return;
                }
            }
        }
    }
}


static void
glade_gtk_notebook_project_changed (GladeWidget *gwidget,
                                    GParamSpec  *pspec,
                                    gpointer     userdata)
{
    GladeProject
        *project = glade_widget_get_project (gwidget),
        *old_project = g_object_get_data (G_OBJECT (gwidget), "notebook-project-ptr");

    if (old_project)
        g_signal_handlers_disconnect_by_func (G_OBJECT (old_project),
                              G_CALLBACK (glade_gtk_notebook_selection_changed),
                              gwidget);

    if (project)
        g_signal_connect (G_OBJECT (project), "selection-changed",
                  G_CALLBACK (glade_gtk_notebook_selection_changed), gwidget);

    g_object_set_data (G_OBJECT (gwidget), "notebook-project-ptr", project);
}


static void
glade_ux_tabbrowser_switch_page (UxTabbrowser    *tabbrowser,
                                 GtkWidget       *page,
                                 guint            page_num,
                                 gpointer         user_data)
{
    GladeWidget *gtabbrowser = glade_widget_get_from_gobject (tabbrowser);

    glade_widget_property_set (gtabbrowser, "page", page_num);

}


void
glade_ux_tabbrowser_post_create (GladeWidgetAdaptor *adaptor,
                                 GObject            *tabbrowser,
                                 GladeCreateReason   reason)
{
    GladeWidget *gwidget = glade_widget_get_from_gobject (tabbrowser);

    gtk_notebook_popup_disable (GTK_NOTEBOOK (tabbrowser));

    g_signal_connect (G_OBJECT (gwidget), "notify::project",
              G_CALLBACK (glade_gtk_notebook_project_changed), NULL);

    glade_gtk_notebook_project_changed (gwidget, NULL, NULL);

    g_signal_connect (G_OBJECT (tabbrowser), "switch-page",
              G_CALLBACK (glade_ux_tabbrowser_switch_page), NULL);
}
