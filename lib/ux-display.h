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

#ifndef __UX_DISPLAY_H__
#define __UX_DISPLAY_H__


#include "ux-graphics.h" //#include "mwb-types.h"

#include "ux-style.h"

#include <glib-object.h>


typedef struct _UxDisplayViewport  UxDisplayViewport;
typedef struct _UxDisplayObject    UxDisplayObject;
typedef struct _UxDisplayContainer UxDisplayContainer;
typedef struct _UxDisplayShape     UxDisplayShape;
typedef struct _UxDisplayContext   UxDisplayContext;
struct _UxDisplayObject {
    gchar *name;
    UxDisplayContainer *parent;
    UxDisplayViewport *viewport;
};
struct _UxDisplayContainer {
    UxDisplayObject object;
    GList *childs;
};
struct _UxDisplayShape {
    UxDisplayObject object;
    GArray *datas;
};
struct _UxDisplayViewport {
    UxDisplayContext *context;
    UxDisplayObject  *root;
    gdouble x;
    gdouble y;
    gdouble width;
    gdouble height;
};
struct _UxDisplayContext {
    GdkWindow *window;
    GtkWidget *widget;/// TODO remove this !
    cairo_t   *cr;/// TODO remove this !
};

//UxDisplayObject   *ux_display_object_new(void);
void                 ux_display_object_init(UxDisplayObject *object, gchar *name, UxDisplayViewport *viewport, UxDisplayContainer *parent);
/*void                 ux_display_object_set_viewport(UxDisplayObject *object, UxDisplayViewport *viewport);*/
UxDisplayViewport  *ux_display_object_get_viewport(UxDisplayObject *object);
void                 ux_display_object_render(UxDisplayObject *object);

/*
UxDisplayContainer *ux_display_container_new(UxDisplayViewport *viewport, UxDisplayContainer *parent);
void                 ux_display_container_init(UxDisplayContainer *container);
void                 ux_display_container_free(UxDisplayContainer *container);
*/
UxDisplayShape     *ux_display_shape_new(UxDisplayViewport *viewport, UxDisplayContainer *parent);
void                 ux_display_shape_init(UxDisplayShape *shape);
void                 ux_display_shape_free(UxDisplayShape *shape);
void                 ux_display_shape_set_style(UxDisplayShape *shape, UxStyle *style);
//void             ux_display_shape_clear_graphics(UxDisplayShape *shape);
//void             ux_display_shape_set_graphics(UxDisplayShape *shape, GArray *datas);
//void             ux_display_shape_push_graphics(UxDisplayShape *shape, GArray *datas);
//void             ux_display_shape_pop_graphics(UxDisplayShape *shape, GArray *datas);
void                 ux_display_shape_add_graphics(UxDisplayShape *shape, UxGraphicsData *data);
void                 ux_display_shape_render(UxDisplayShape *shape);

UxDisplayViewport  *ux_display_viewport_new(UxDisplayContext *context);
void                 ux_display_viewport_init(UxDisplayViewport *viewport, UxDisplayContext *context);
void                 ux_display_viewport_free(UxDisplayViewport *viewport);
void                 ux_display_viewport_set_root(UxDisplayViewport *viewport, UxDisplayObject *root);

UxDisplayContext   *ux_display_context_new(GtkWidget *widget/*, cairo_t *cr*/);
void                 ux_display_context_free(UxDisplayContext *context);
cairo_t             *ux_display_context_create_cairo(UxDisplayContext *context);

#endif // __UX_DISPLAY_H__
