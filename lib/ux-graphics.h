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

#ifndef __UX_GRAPHICS_H__
#define __UX_GRAPHICS_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

/*
typedef struct _MwbRef {
    int count;
    void *ptr;
} MwbRef;

typedef struct _MwbRefOwner {
    MwbRef parent;
    GArray *shared;
} MwbRefOwner;

typedef struct _MwbRefShared {
    MwbRef parent;
    MwbRefOwner *owner;
} MwbRefShared;
*/
//--------------

typedef struct _UxGraphicsPainter {
    GObject object;
} UxGraphicsPainter;

typedef struct _UxGraphicsPainterClass {
    void (*paint) (UxGraphicsPainter *painter, cairo_t *cr, gboolean preserve);
} UxGraphicsPainterClass;

//--------------












typedef enum _UxGraphicsDataType {
    UX_GRAPHICS_DATA_UNKNOW,
    UX_GRAPHICS_DATA_PATH,
    UX_GRAPHICS_DATA_STROKE,
    UX_GRAPHICS_DATA_FILL
} UxGraphicsDataType;

typedef struct _UxGraphicsDataPath {
    cairo_path_t *cr_path;
} UxGraphicsDataPath;

typedef struct _UxGraphicsDataStroke {
    cairo_pattern_t *pattern;
    double width;
    cairo_line_cap_t cap;
    cairo_line_join_t join;
    double miter_limit;
} UxGraphicsDataStroke;

typedef struct _UxGraphicsDataFill {
    cairo_pattern_t *pattern;
    cairo_fill_rule_t rule;
} UxGraphicsDataFill;

typedef struct _UxGraphicsData {
    UxGraphicsDataType type;
    union {
        UxGraphicsDataStroke stroke;
        UxGraphicsDataFill fill;
        UxGraphicsDataPath path;
    } data;
} UxGraphicsData;

UxGraphicsData *ux_graphics_path_new();
void ux_graphics_path_init(UxGraphicsData *graphics, cairo_path_t *path);
void ux_graphics_path_free(UxGraphicsData *graphics);
void ux_graphics_path_set_cr_path(UxGraphicsData *graphics, cairo_path_t *cr_path);

UxGraphicsData *ux_graphics_stroke_new();
void ux_graphics_stroke_init(UxGraphicsData *graphics,
                              cairo_pattern_t *pattern,
                              double width,
                              cairo_line_cap_t cap,
                              cairo_line_join_t join,
                              double miter_limit);
void ux_graphics_stroke_free(UxGraphicsData *graphics);

UxGraphicsData *ux_graphics_fill_new();
void ux_graphics_fill_init(UxGraphicsData *graphics,
                              cairo_pattern_t *pattern,
                              cairo_fill_rule_t rule);
void ux_graphics_fill_free(UxGraphicsData *graphics);

void ux_graphics_draw(cairo_t *cr, GArray *data);

// --------------

typedef void (*UxGraphicsRender)(UxGraphicsData *graphics, cairo_t *cr, gboolean preserve);// ok
typedef struct _UxGraphics {// ok but not/bad used
    cairo_t *cr;
    UxGraphicsData *preview_data;
    UxGraphicsRender preview_render;
} UxGraphics;

struct _UxGraphicsClass {// who use it ???
    void (*draw) (UxGraphics *graphics, cairo_t *cr, gboolean preserve);
};


//--------------
//drawing.push(stroke, fill, path);
//graphics.drawGraphicsData(drawing);
void test(cairo_t *cr);

G_END_DECLS

#endif // __UX_GRAPHICS_H__
