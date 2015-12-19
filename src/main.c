/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2015 Gaulouis <gaulouis.fr@gmail.com>
 * 
 * ux-tabbrowser is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ux-tabbrowser is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ux-tabbrowser.h"


#include <gtk/gtk.h>
#include <glib/gi18n.h>


static GtkWidget*
create_window (void)
{
	GtkWidget *window;
    GtkWidget *tabbrowser;
    GtkWidget *widget;

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "UX - Tab Browser");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

    tabbrowser = g_object_new(UX_TYPE_TABBROWSER, NULL);
    gtk_widget_set_name(GTK_WIDGET(tabbrowser), "primary");
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(tabbrowser), GTK_POS_TOP);
    gtk_notebook_append_page(GTK_NOTEBOOK(tabbrowser), gtk_label_new("Content #1"), gtk_label_new("Label 1"));
    gtk_notebook_append_page(GTK_NOTEBOOK(tabbrowser), gtk_label_new("Content #2"), gtk_label_new("Label 2"));
    gtk_notebook_append_page(GTK_NOTEBOOK(tabbrowser), gtk_label_new("Content #3"), gtk_label_new("Label 3"));
    widget = gtk_button_new_from_stock(GTK_STOCK_APPLY);
    gtk_widget_show(widget);
    gtk_notebook_set_action_widget (GTK_NOTEBOOK(tabbrowser), widget, GTK_PACK_START);

    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(tabbrowser));

	/* Exit when the window is closed */
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	
	return window;
}

int
main (int argc, char *argv[])
{
 	GtkWidget *window;


#ifdef G_OS_WIN32
	gchar *prefix = g_win32_get_package_installation_directory_of_module (NULL);
	gchar *localedir = g_build_filename (prefix, "share", "locale", NULL);
#endif

#ifdef ENABLE_NLS

# ifndef G_OS_WIN32
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
# else
	bindtextdomain (GETTEXT_PACKAGE, localedir);
# endif
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif
	
	gtk_init (&argc, &argv);
    const char *rc_style = " \
      gtk-color-scheme = \"dark_bg_color:#3c3b37\ndark_fg_color:#dfdbd2\" \
      style \"ux_tabbrowser_style\" { \
            UxTabbrowser::bar-padding-top = 10 \
      } \
      widget_class \"*.<UxTabbrowser>\" style \"ux_tabbrowser_style\"";
      gtk_rc_parse_string (rc_style);

	window = create_window ();
    gtk_widget_show_all (window);

	gtk_main ();



#ifdef G_OS_WIN32
	g_free (prefix);
	g_free (localedir);
#endif

	return 0;
}

