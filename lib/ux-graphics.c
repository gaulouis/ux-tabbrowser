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

#include "ux-graphics.h"

#define UX_GRAPHICS_PATH_INIT {UX_GRAPHICS_DATA_PATH, NULL}
UxGraphicsData *ux_graphics_path_new()
{
    UxGraphicsData *graphics = g_new(UxGraphicsData, 1);
    ux_graphics_path_init(graphics, NULL);
    return graphics;
}
void ux_graphics_path_init(UxGraphicsData *graphics, cairo_path_t *path)
{
    graphics->type = UX_GRAPHICS_DATA_PATH;
    graphics->data.path.cr_path = path;
}
void ux_graphics_path_free(UxGraphicsData *graphics)
{
    if (graphics->data.path.cr_path) {
        cairo_path_destroy(graphics->data.path.cr_path);
    }
    if (graphics) {
        g_free(graphics);
    }
}
void ux_graphics_path_set_cr_path(UxGraphicsData *graphics, cairo_path_t *cr_path)
{
    if (graphics->data.path.cr_path) {
        cairo_path_destroy(graphics->data.path.cr_path);
    }
    if (cr_path) {
        graphics->data.path.cr_path = cr_path;
    }
}

#define UX_GRAPHICS_STROKE_INIT {UX_GRAPHICS_DATA_STROKE, {NULL, 1.0, CAIRO_LINE_CAP_ROUND, CAIRO_LINE_JOIN_MITER, 1.0}}
UxGraphicsData *ux_graphics_stroke_new()
{
    UxGraphicsData *graphics = g_new(UxGraphicsData, 1);
    ux_graphics_stroke_init(graphics, NULL, 1.0, CAIRO_LINE_CAP_BUTT, CAIRO_LINE_JOIN_MITER, 1.0);
    return graphics;
}
void ux_graphics_stroke_init(UxGraphicsData *graphics,
                              cairo_pattern_t *pattern,
                              double width,
                              cairo_line_cap_t cap,
                              cairo_line_join_t join,
                              double miter_limit)
{
    graphics->type = UX_GRAPHICS_DATA_STROKE;
    graphics->data.stroke.pattern = pattern;
    graphics->data.stroke.width = width;
    graphics->data.stroke.cap = cap;
    graphics->data.stroke.join = join;
    graphics->data.stroke.miter_limit = miter_limit;
}
void ux_graphics_stroke_free(UxGraphicsData *graphics)
{
    if (graphics->data.stroke.pattern) {
        cairo_pattern_destroy(graphics->data.stroke.pattern);
    }
    if (graphics) {
        g_free(graphics);
    }
}

#define UX_GRAPHICS_FILL_INIT {UX_GRAPHICS_DATA_FILL, {{NULL, CAIRO_FILL_RULE_EVEN_ODD}}}
UxGraphicsData *ux_graphics_fill_new()
{
    UxGraphicsData *graphics = g_new(UxGraphicsData, 1);

    graphics->type = UX_GRAPHICS_DATA_FILL;
    ux_graphics_fill_init(graphics, NULL, CAIRO_FILL_RULE_EVEN_ODD);

    return graphics;
}
void ux_graphics_fill_init(UxGraphicsData *graphics,
                              cairo_pattern_t *pattern,
                              cairo_fill_rule_t rule)
{
    graphics->type = UX_GRAPHICS_DATA_FILL;
    graphics->data.fill.pattern = pattern;
    graphics->data.fill.rule = rule;
}
void ux_graphics_fill_free(UxGraphicsData *graphics)
{
    if (graphics->data.fill.pattern) {
        cairo_pattern_destroy(graphics->data.fill.pattern);
    }
    if (graphics) {
        g_free(graphics);
    }
}

#define SCOOP_RENDER static
SCOOP_RENDER void ux_graphics_path_render(UxGraphicsData *graphics, cairo_t *cr, gboolean preserve)
{
    g_return_if_fail(graphics->type==UX_GRAPHICS_DATA_PATH);

    cairo_append_path(cr, graphics->data.path.cr_path);
}
SCOOP_RENDER void ux_graphics_stroke_render(UxGraphicsData *graphics, cairo_t *cr, gboolean preserve)
{
    g_return_if_fail(graphics->type==UX_GRAPHICS_DATA_STROKE);

    cairo_set_source(cr, graphics->data.stroke.pattern);//cairo_set_source_surface(); ???
    cairo_set_line_width(cr, graphics->data.stroke.width);
    cairo_set_line_cap(cr, graphics->data.stroke.cap);
    cairo_set_line_join(cr, graphics->data.stroke.join);
    cairo_set_miter_limit(cr, graphics->data.stroke.miter_limit);

    preserve ? cairo_stroke_preserve(cr):cairo_stroke(cr);
}
SCOOP_RENDER void ux_graphics_fill_render(UxGraphicsData *graphics, cairo_t *cr, gboolean preserve)
{
    g_return_if_fail(graphics->type==UX_GRAPHICS_DATA_FILL);

    cairo_set_source(cr, graphics->data.fill.pattern);//cairo_set_source_surface(); ???
    cairo_set_fill_rule(cr, graphics->data.fill.rule);

    preserve ? cairo_fill_preserve(cr):cairo_fill(cr);
}
SCOOP_RENDER void ux_graphics_nop_render(UxGraphicsData *graphics, cairo_t *cr, gboolean preserve)
{
}

void ux_graphics_draw(cairo_t *cr, GArray *data)
{
    UxGraphicsData *preview_data = NULL;
    UxGraphicsRender preview_render = &ux_graphics_nop_render;
    int i;
    UxGraphicsData *graphics;
    for (i = 0; i < data->len; i++) {
        graphics = (UxGraphicsData *)&(data->data[i*sizeof(UxGraphicsData)]);

        switch(graphics->type) {
        case UX_GRAPHICS_DATA_PATH:
            preview_render(preview_data, cr, FALSE);

            preview_data = graphics;
            preview_render = &ux_graphics_path_render;
        break;
        case UX_GRAPHICS_DATA_STROKE:
            preview_render(preview_data, cr, TRUE);

            preview_data = graphics;
            preview_render = &ux_graphics_stroke_render;
        break;
        case UX_GRAPHICS_DATA_FILL:
            preview_render(preview_data, cr, TRUE);

            preview_data = graphics;
            preview_render = &ux_graphics_fill_render;
        break;
        default:
            g_print("Error: COMMAND[%d] not defined\n", graphics->type);
        break;
        }
    }
    preview_render(preview_data, cr, FALSE);

}

// -------------------------------------


void test(cairo_t *cr)
{

    // path object
    UxGraphicsData data_path = UX_GRAPHICS_PATH_INIT;
    {
        cairo_new_path(cr);
        cairo_move_to(cr, 0.0, 0.0);
        cairo_line_to(cr, 20.0,  0.0);
        cairo_line_to(cr, 20.0, 20.0);
        cairo_line_to(cr,  0.0, 20.0);
        cairo_close_path(cr);
        cairo_path_t *path = cairo_copy_path(cr);
        data_path.data.path.cr_path = path;
        // ux_graphics_path_set_path(data_path, path);
    }

    // stroke object
    UxGraphicsData data_stroke = UX_GRAPHICS_STROKE_INIT;
    {
        cairo_pattern_t *pattern = cairo_pattern_create_rgb(0.0, 1.0, 0.0);
        data_stroke.data.stroke.pattern = pattern;
        // ux_graphics_stroke_set_pattern(data_stroke, pattern);
    }

    // fill object
    UxGraphicsData data_fill = UX_GRAPHICS_FILL_INIT;
    {
        cairo_pattern_t *pattern = cairo_pattern_create_rgb(0.0, 0.0, 1.0);
        data_fill.data.fill.pattern = pattern;
        // ux_graphics_fill_set_pattern(data_fill, pattern);
    }

    //cairo_fill_preserve(cr);
    //cairo_stroke(cr);

    GArray *data = g_array_new(FALSE, FALSE, sizeof(UxGraphicsData));
    //g_array_append_val(data, begin);
    g_array_append_val(data, data_path);
    g_array_append_val(data, data_stroke);
    g_array_append_val(data, data_fill);
    //g_array_append_val(data, end);

    ux_graphics_draw(cr, data);

}
