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

#include "ux-display.h"

#include <math.h>
#include<stdlib.h>
#include <string.h>

//UxDisplayObject   *ux_display_object_new(void);
void
ux_display_object_init(UxDisplayObject *object, gchar *name, UxDisplayViewport *viewport, UxDisplayContainer *parent)
{
    if (object->name) {
        g_free(object->name);
    }
    if (name) {
        object->name = g_strdup(name);
    }
    object->viewport = viewport;// use MwbRef/MwbRefOwner/MwbShared;
    object->parent = parent;
}
/*
void
ux_display_object_set_viewport(UxDisplayObject *object, UxDisplayViewport *viewport)
{
    object->viewport = viewport;
}
*/
UxDisplayViewport *
ux_display_object_get_viewport(UxDisplayObject *object)
{
    return object->viewport;
}
void ux_display_object_render(UxDisplayObject *object)
{
    /*
    cairo_t *cr = ux_display_context_get_cairo(object->viewport->context);
    switch (object->type) {
    case UX_TYPE_DISPLAY_SHAPE:
        ux_graphics_shape_draw(cr, datas);
        break;
    }
    */
}


// ----------------------------------------------------------------------------

/*
UxDisplayContainer *ux_display_container_new(UxDisplayViewport *viewport, UxDisplayContainer *parent)
{

}

void ux_display_container_init(UxDisplayContainer *container)
{

}

void ux_display_container_free(UxDisplayContainer *container)
{

}
*/

// ----------------------------------------------------------------------------

UxDisplayShape *ux_display_shape_new(UxDisplayViewport *viewport, UxDisplayContainer *parent)
{
    UxDisplayShape *shape = g_new0(UxDisplayShape, 1);

    ux_display_object_init((UxDisplayObject *) shape, "shape", viewport, parent);
    ux_display_shape_init(shape);

    return shape;
}
void
ux_display_shape_init(UxDisplayShape *shape)
{
    shape->datas = g_array_new(FALSE, FALSE, sizeof(UxGraphicsData));
}

void ux_display_shape_free(UxDisplayShape *shape)
{
    if (shape) {
        g_free(shape);
    }
}

void
mwb_cairo_path_merge_cairo_path(cairo_path_t *path, cairo_path_t *result_path)
{
    /*
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0,0);
    cairo_t *cr = cairo_create(surface);

    if (result_path)
    {

    } else {

    }

    cairo_path_t *cr_path = cairo_copy_path(cr);
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    */
}

cairo_path_t*
ux_style_path_get_graphics_path(GList *segments, UxDisplayViewport *viewport)
{
    UxStylePathElement *segment;
//    gint data_num_length = 0;
//    for(segments = style->border->path.left.segments; segments; segments = segments->next) {
//        segment = (UxStylePathElement *) segment->data;
//        data_num_length += segment->length + 2;
//    }
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0,0);
    cairo_t *cr = cairo_create(surface);
//    path = g_new(cairo_path_t, 1);
//    path->status   = CAIRO_STATUS_SUCCESS;
//    path->num_data = svg_path->cairo_path_num_data;
//    path->data     = (cairo_path_data_t*) g_new(cairo_path_data_t, svg_path->cairo_path_num_data);
#define MARGIN_TOP 0.0 /*4.0*/
#define FIX_TOP 0.0
    cairo_matrix_t matrix;
    cairo_matrix_init_identity(&matrix);
    cairo_matrix_translate(&matrix, viewport->x, viewport->y+MARGIN_TOP+FIX_TOP);
    //cairo_matrix_scale(&matrix, viewport->x, viewport->y);
    cairo_set_matrix(cr, &matrix);

    gdouble width = viewport->width;
    gdouble height = viewport->height - MARGIN_TOP;
    struct {
        gdouble x1;
        gdouble y1;
        gdouble x2;
        gdouble y2;
        gdouble x;
        gdouble y;
    } coordonnee;
//    struct {
//        gdouble x;
//        gdouble y;
//    } current;
    UxStyleLength length;
    for(segments = segments; segments; segments = segments->next) {
        segment = (UxStylePathElement *) segments->data;
        switch (segment->type) {
        case UX_STYLE_PATH_COMMAND_TYPE_MOVE_TO:
            coordonnee.x = ux_style_length_get_value(&(segment->data[0].length), width);
            coordonnee.y = ux_style_length_get_value(&(segment->data[1].length), height);
            cairo_move_to(cr, coordonnee.x, coordonnee.y);
        break;
        case UX_STYLE_PATH_COMMAND_TYPE_LINE_TO:
            coordonnee.x = ux_style_length_get_value(&(segment->data[0].length), width);
            coordonnee.y = ux_style_length_get_value(&(segment->data[1].length), height);
            cairo_line_to(cr, coordonnee.x, coordonnee.y);
        break;
        case UX_STYLE_PATH_COMMAND_TYPE_CURVE_TO:
            coordonnee.x1 = ux_style_length_get_value(&(segment->data[0].length), width);
            coordonnee.y1 = ux_style_length_get_value(&(segment->data[1].length), height);
            coordonnee.x2 = ux_style_length_get_value(&(segment->data[2].length), width);
            coordonnee.y2 = ux_style_length_get_value(&(segment->data[3].length), height);
            coordonnee.x = ux_style_length_get_value(&(segment->data[4].length), width);
            coordonnee.y = ux_style_length_get_value(&(segment->data[5].length), height);
            cairo_curve_to(cr, coordonnee.x1, coordonnee.y1
                             , coordonnee.x2, coordonnee.y2
                             , coordonnee.x, coordonnee.y);
        break;
        case UX_STYLE_PATH_COMMAND_TYPE_CLOSE:
            cairo_close_path(cr);
        break;
        case UX_STYLE_PATH_COMMAND_TYPE_REL_MOVE_TO:
            coordonnee.x = ux_style_length_get_value(&(segment->data[0].length), width);
            coordonnee.y = ux_style_length_get_value(&(segment->data[1].length), height);
            if (cairo_has_current_point(cr)) {
                cairo_rel_move_to(cr, coordonnee.x, coordonnee.y);
            } else {
                cairo_move_to(cr, coordonnee.x, coordonnee.y);
            }
        break;
        case UX_STYLE_PATH_COMMAND_TYPE_REL_LINE_TO:
            coordonnee.x = ux_style_length_get_value(&(segment->data[0].length), width);
            coordonnee.y = ux_style_length_get_value(&(segment->data[1].length), height);
            cairo_rel_line_to(cr, coordonnee.x, coordonnee.y);
        break;
        case UX_STYLE_PATH_COMMAND_TYPE_REL_CURVE_TO:
            coordonnee.x1 = ux_style_length_get_value(&(segment->data[0].length), width);
            coordonnee.y1 = ux_style_length_get_value(&(segment->data[1].length), height);
            coordonnee.x2 = ux_style_length_get_value(&(segment->data[2].length), width);
            coordonnee.y2 = ux_style_length_get_value(&(segment->data[3].length), height);
            coordonnee.x = ux_style_length_get_value(&(segment->data[4].length), width);
            coordonnee.y = ux_style_length_get_value(&(segment->data[5].length), height);
            cairo_rel_curve_to(cr, coordonnee.x1, coordonnee.y1
                                 , coordonnee.x2, coordonnee.y2
                                 , coordonnee.x, coordonnee.y);
        break;
        case UX_STYLE_PATH_COMMAND_TYPE_REL_CLOSE:
            cairo_close_path(cr);
        break;
        }
//        current.x = coordonnee.x;
//        current.y = coordonnee.y;
    }
    cairo_matrix_t m;
    cairo_matrix_init_identity(&m);
    cairo_set_matrix(cr, &m);

    cairo_path_t *cr_path = cairo_copy_path(cr);
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    return cr_path;
}

/*
static cairo_path_t*
cairo_dup_path(cairo_path_t *path)
{
    cairo_path_t *copy = NULL;
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
    cairo_t *cr = cairo_create(surface);

    cairo_new_path(cr);
    cairo_append_path(cr, path);
    copy = cairo_copy_path(cr);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    return copy;
}
*/

typedef struct _PathBorder {
    cairo_path_t *top;
    cairo_path_t *right;
    cairo_path_t *bottom;
    cairo_path_t *left;
} PathBorder;

static void
ux_display_shape_add_graphics_border(UxDisplayShape *shape, PathBorder *cr_path_border, UxStyle *style)
{
    if (cr_path_border->top
     && style->border->width.top.value
     && style->border->color.top.type > UX_STYLE_COLOR_TRANSPARENT
    ) {
        UxGraphicsData *graphics_path = ux_graphics_path_new();
        graphics_path->data.path.cr_path = cr_path_border->top;
        ux_display_shape_add_graphics(shape, graphics_path);

        UxGraphicsData *graphics_stroke = ux_graphics_stroke_new();
        MurrineRGB rgb;
        gdouble a;
        ux_style_color_get_rgba(&style->border->color.top.value, &rgb, &a);
        graphics_stroke->data.stroke.pattern = cairo_pattern_create_rgba(rgb.r, rgb.g, rgb.b, a);
        graphics_stroke->data.stroke.width = style->border->width.top.value;
        ux_display_shape_add_graphics(shape, graphics_stroke);
    }

    if (cr_path_border->right
     && style->border->width.right.value
     && style->border->color.right.type > UX_STYLE_COLOR_TRANSPARENT
    ) {
        UxGraphicsData *graphics_path = ux_graphics_path_new();
        graphics_path->data.path.cr_path = cr_path_border->right;
        ux_display_shape_add_graphics(shape, graphics_path);

        UxGraphicsData *graphics_stroke = ux_graphics_stroke_new();
        MurrineRGB rgb;
        gdouble a;
        ux_style_color_get_rgba(&style->border->color.right.value, &rgb, &a);
        graphics_stroke->data.stroke.pattern = cairo_pattern_create_rgba(rgb.r, rgb.g, rgb.b, a);
        graphics_stroke->data.stroke.width = style->border->width.right.value;
        ux_display_shape_add_graphics(shape, graphics_stroke);
    }

    if (cr_path_border->bottom
     && style->border->width.bottom.value
     && style->border->color.bottom.type > UX_STYLE_COLOR_TRANSPARENT
    ) {
        UxGraphicsData *graphics_path = ux_graphics_path_new();
        graphics_path->data.path.cr_path = cr_path_border->bottom;
        ux_display_shape_add_graphics(shape, graphics_path);

        UxGraphicsData *graphics_stroke = ux_graphics_stroke_new();
        MurrineRGB rgb;
        gdouble a;
        ux_style_color_get_rgba(&style->border->color.bottom.value, &rgb, &a);
        graphics_stroke->data.stroke.pattern = cairo_pattern_create_rgba(rgb.r, rgb.g, rgb.b, a);
        graphics_stroke->data.stroke.width = style->border->width.bottom.value;
        ux_display_shape_add_graphics(shape, graphics_stroke);
    }

    if (cr_path_border->left
     && style->border->width.left.value
     && style->border->color.left.type > UX_STYLE_COLOR_TRANSPARENT
    ) {
        UxGraphicsData *graphics_path = ux_graphics_path_new();
        graphics_path->data.path.cr_path = cr_path_border->left;
        ux_display_shape_add_graphics(shape, graphics_path);

        UxGraphicsData *graphics_stroke = ux_graphics_stroke_new();
        MurrineRGB rgb;
        gdouble a;
        ux_style_color_get_rgba(&style->border->color.left.value, &rgb, &a);
        graphics_stroke->data.stroke.pattern = cairo_pattern_create_rgba(rgb.r, rgb.g, rgb.b, a);
        graphics_stroke->data.stroke.width = style->border->width.left.value;
        ux_display_shape_add_graphics(shape, graphics_stroke);
    }

}

void ux_display_shape_set_style(UxDisplayShape *shape, UxStyle *style)
{
    UxDisplayViewport *viewport = ux_display_object_get_viewport((UxDisplayObject *)shape);
    //UxDisplayContext *context = ux_display_viewport_get_context(viewport);
    UxDisplayContext *context = viewport->context;

    PathBorder cr_path_border = {NULL, NULL, NULL, NULL};
    cairo_path_t *cr_path_shape = NULL;
    gboolean used_path_shape = FALSE;

    gboolean use_viewport_border = TRUE;
    gint padding_top = 0;
    if (style->padding)
        padding_top = style->padding->top.value;

    if (style->border) {
        // create shape
        if (style->border->path.top.segments
         && style->border->path.right.segments
         && style->border->path.bottom.segments
         && style->border->path.left.segments
        ) {
            cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
            cairo_t *cr = cairo_create(surface);
            UxDisplayViewport _viewport;

            // create stroke path
            {
                _viewport.x = viewport->x+viewport->height;
                _viewport.y = viewport->y;
                _viewport.width = viewport->width-2*viewport->height+1;
                _viewport.height = viewport->height;
                cr_path_border.top = ux_style_path_get_graphics_path(style->border->path.top.segments, &_viewport);

                _viewport.x = viewport->x+viewport->width-viewport->height;
                _viewport.y = viewport->y;
                _viewport.width = viewport->height;
                _viewport.height = viewport->height;
                cr_path_border.right = ux_style_path_get_graphics_path(style->border->path.right.segments, &_viewport);

                _viewport.x = viewport->x+viewport->height;
                _viewport.y = viewport->y;
                _viewport.width = viewport->width-2*viewport->height;
                _viewport.height = viewport->height;
                cr_path_border.bottom = ux_style_path_get_graphics_path(style->border->path.bottom.segments, &_viewport);

                _viewport.x = viewport->x;
                _viewport.y = viewport->y;
                _viewport.width = viewport->height;
                _viewport.height = viewport->height;
                cr_path_border.left = ux_style_path_get_graphics_path(style->border->path.left.segments, &_viewport);
            }

            // create fill path
            {
                cairo_path_t *cr_path;

                _viewport.x = viewport->x+viewport->height;
                _viewport.y = viewport->y;
                _viewport.width = viewport->width-2*viewport->height;
                _viewport.height = viewport->height;
                cr_path = ux_style_path_get_graphics_path(style->border->path.top.segments, &_viewport);
                cr_path->data->header.type = CAIRO_PATH_LINE_TO;
                cairo_append_path(cr, cr_path);
                cairo_path_destroy(cr_path);

                _viewport.x = viewport->x+viewport->width-viewport->height;
                _viewport.y = viewport->y;
                _viewport.width = viewport->height;
                _viewport.height = viewport->height;
                cr_path = ux_style_path_get_graphics_path(style->border->path.right.segments, &_viewport);
                cr_path->data->header.type = CAIRO_PATH_LINE_TO;
                cairo_append_path(cr, cr_path);
                cairo_path_destroy(cr_path);

                _viewport.x = viewport->x+viewport->height;
                _viewport.y = viewport->y;
                _viewport.width = viewport->width-2*viewport->height;
                _viewport.height = viewport->height;
                cr_path = ux_style_path_get_graphics_path(style->border->path.bottom.segments, &_viewport);
                cr_path->data->header.type = CAIRO_PATH_LINE_TO;
                cairo_append_path(cr, cr_path);
                cairo_path_destroy(cr_path);

                _viewport.x = viewport->x;
                _viewport.y = viewport->y;
                _viewport.width = viewport->height;
                _viewport.height = viewport->height;
                cr_path = ux_style_path_get_graphics_path(style->border->path.left.segments, &_viewport);
                cr_path->data->header.type = CAIRO_PATH_LINE_TO;
                cairo_append_path(cr, cr_path);
                cr_path_shape = cairo_copy_path(cr);
                cairo_path_destroy(cr_path);

            }

            cairo_destroy(cr);
            cairo_surface_destroy(surface);

            use_viewport_border = FALSE;
        }
    }

    // create box-path for fill if background exist
    if (use_viewport_border) {
        gdouble fix_top = 0.0;//style->border->width.top.value;
        gdouble fix_height = 0.0;//style->border->width.bottom.value;

        cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
        cairo_t *cr = cairo_create(surface);

        cairo_new_path(cr);
        cairo_move_to(cr, viewport->x, viewport->y+fix_top);
        cairo_line_to(cr, viewport->x+viewport->width, viewport->y+fix_top);
        cairo_line_to(cr, viewport->x+viewport->width, viewport->y+viewport->height-fix_height);
        cairo_line_to(cr, viewport->x, viewport->y+viewport->height-fix_height);
        cairo_line_to(cr, viewport->x, viewport->y+fix_top);
        cr_path_shape = cairo_copy_path(cr);

        cairo_destroy(cr);
        cairo_surface_destroy(surface);
    }


    // create border-path for stroke if border exist
    if (use_viewport_border) {
        gdouble fix_top = style->border->width.top.value;
        gdouble fix_height = style->border->width.bottom.value;
        gdouble hint_a = fix_top/2.0;// todo %2 ? +0.5 : 0.0;
        gdouble hint_s = fix_height/2.0;
        cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
        cairo_t *cr = cairo_create(surface);

        cairo_new_path(cr);
        cairo_move_to(cr, viewport->x, viewport->y+hint_a);
        cairo_line_to(cr, viewport->x+viewport->width, viewport->y+hint_a);
        cr_path_border.top = cairo_copy_path(cr);

        cairo_new_path(cr);
        cairo_move_to(cr, viewport->x+viewport->width+hint_s, viewport->y+hint_a);
        cairo_line_to(cr, viewport->x+viewport->width+hint_s, viewport->y+viewport->height+hint_s);
        cr_path_border.right = cairo_copy_path(cr);

        cairo_new_path(cr);
        cairo_move_to(cr, viewport->x+viewport->width, viewport->y+viewport->height-hint_s);
        cairo_line_to(cr, viewport->x, viewport->y+viewport->height-hint_s);
        cr_path_border.bottom = cairo_copy_path(cr);

        cairo_new_path(cr);
        cairo_move_to(cr, viewport->x+hint_a, viewport->y+viewport->height+hint_s);
        cairo_line_to(cr, viewport->x+hint_a, viewport->y+hint_a);
        cr_path_border.left = cairo_copy_path(cr);

        cairo_destroy(cr);
        cairo_surface_destroy(surface);
    }

    if (UX_STYLE_PAINT_TYPE_STROKE_AND_FILL==style->paint && style->border) {
        ux_display_shape_add_graphics_border(shape, &cr_path_border, style);
    }

    if (style->background) {
        //if (style->background->color) {
            switch (style->background->color.type) {
            case UX_STYLE_COLOR_RGB:
                {
                    if (!used_path_shape) {
                        UxGraphicsData *graphics_path = ux_graphics_path_new();
                        ux_graphics_path_set_cr_path(graphics_path, cr_path_shape);
                        ux_display_shape_add_graphics(shape, graphics_path);
                        used_path_shape = TRUE;
                    }

                    MurrineRGB rgb;
                    UxGraphicsData *graphics_fill = ux_graphics_fill_new();
                    ux_style_color_get_rgb(&style->background->color.value, &rgb);
                    graphics_fill->data.fill.pattern = cairo_pattern_create_rgb(rgb.r, rgb.g, rgb.b);
                    ux_display_shape_add_graphics(shape, graphics_fill);
                }
                break;
            case UX_STYLE_COLOR_RGBA:
                {
                    if (!used_path_shape) {
                        UxGraphicsData *graphics_path = ux_graphics_path_new();
                        ux_graphics_path_set_cr_path(graphics_path, cr_path_shape);
                        ux_display_shape_add_graphics(shape, graphics_path);
                        used_path_shape = TRUE;
                    }

                    gdouble a;
                    MurrineRGB rgb;
                    UxGraphicsData *graphics_fill = ux_graphics_fill_new();
                    ux_style_color_get_rgba(&style->background->color.value, &rgb, &a);
                    graphics_fill->data.fill.pattern = cairo_pattern_create_rgba(rgb.r, rgb.g, rgb.b, a);
                    ux_display_shape_add_graphics(shape, graphics_fill);
                }
                break;
            case UX_STYLE_COLOR_TRANSPARENT:
            case UX_STYLE_COLOR_NONE:
            case UX_STYLE_COLOR_UNKNOWN:
            default:
                break;
            }
        //}
        //if (style->background->image) {
            switch (style->background->image.type) {
            case UX_STYLE_IMAGE_LINEAR_GRADIENT:
                {
                    if (!used_path_shape) {
                        UxGraphicsData *graphics_path = ux_graphics_path_new();
                        ux_graphics_path_set_cr_path(graphics_path, cr_path_shape);
                        ux_display_shape_add_graphics(shape, graphics_path);
                        used_path_shape = TRUE;
                    }

                    double module = 1.0;
                    MurrineRGB rgb;
                    UxGraphicsData *graphics_fill = ux_graphics_fill_new();
                    cairo_pattern_t *pattern;
                    UxStyleImageLinearGradient *linear_gradient = style->background->image.data.linear_gradient;

                    switch (linear_gradient->direction.type)
                    {
                    case UX_STYLE_DIRECTION_SIDE:
                        if (UX_STYLE_SIDE_TOP==linear_gradient->direction.side) {
                            pattern = cairo_pattern_create_linear (viewport->x, viewport->y,
                                          viewport->x, viewport->y+viewport->height);
                            module = viewport->height;
                        } else if (UX_STYLE_SIDE_BOTTOM==linear_gradient->direction.side) {
                        } else if (UX_STYLE_SIDE_LEFT==linear_gradient->direction.side) {
                        } else if (UX_STYLE_SIDE_RIGHT==linear_gradient->direction.side) {
                        }
                        break;
                    case UX_STYLE_DIRECTION_CORNER:
                        // compute angle
                        if (UX_STYLE_SIDE_TOP==linear_gradient->direction.side) {
                            if (UX_STYLE_SIDE_LEFT==linear_gradient->direction.corner) {
                                pattern = cairo_pattern_create_linear (viewport->x, viewport->y,
                                              viewport->x+viewport->width, viewport->y+viewport->height);
                                module = sqrt(viewport->width*viewport->width+viewport->height*viewport->height);
                            } else /** UX_STYLE_SIDE_RIGHT */ {
                            }
                        } else /** UX_STYLE_SIDE_BOTTOM */ {
                            if (UX_STYLE_SIDE_LEFT==linear_gradient->direction.corner) {
                            } else /** UX_STYLE_SIDE_RIGHT */ {
                            }
                        }
                        break;
                    case UX_STYLE_DIRECTION_ANGLE:
                        break;
                    case UX_STYLE_DIRECTION_NONE:
                    case UX_STYLE_DIRECTION_UNKNOW:
                    default:
                        break;
                    }

                    int i;
                    for (i=0; i<linear_gradient->stops->len; i++) {
                        double offset;
                        MurrineRGB rgb;
                        UxStyleColorStop color_stop = g_array_index(linear_gradient->stops, UxStyleColorStop, i);
                        switch (color_stop.color.type) {
                        case UX_STYLE_COLOR_RGB:
                        case UX_STYLE_COLOR_RGBA:
                            {
                                ux_style_color_get_rgb(&color_stop.color.value, &rgb);
                                switch (color_stop.offset.unit) {
                                case UX_STYLE_LENGTH_TYPE_PERCENTAGE:
                                    offset = color_stop.offset.value;
                                    break;
                                case UX_STYLE_LENGTH_TYPE_PX:
                                    //offset = ux_style_length_compute(&color_stop.offset, context);
                                    offset = color_stop.offset.value / module;
                                    break;
                                case UX_STYLE_LENGTH_TYPE_NUMBER:
                                case UX_STYLE_LENGTH_TYPE_UNKNOW:
                                default:
                                    g_print("Error: color_stop not implemented\n");
                                    break;
                                }

                            }
                            break;
                        case UX_STYLE_COLOR_TRANSPARENT:
                        case UX_STYLE_COLOR_NONE:
                        case UX_STYLE_COLOR_UNKNOWN:
                        default:
                            break;
                        }
                        cairo_pattern_add_color_stop_rgb(pattern, offset, rgb.r, rgb.g, rgb.b);
                    }
                    //ux_graphics_fill_set_pattern(graphics_fill, pattern);
                    graphics_fill->data.fill.pattern = pattern;

                    ux_display_shape_add_graphics(shape, graphics_fill);
                }
                break;
            case UX_STYLE_IMAGE_NONE:
            case UX_STYLE_IMAGE_UNKNOWN:
            default:
                break;
            }
        //}
    }


    if (UX_STYLE_PAINT_TYPE_FILL_AND_STROKE==style->paint && style->border) {
        ux_display_shape_add_graphics_border(shape, &cr_path_border, style);
    }

}

void ux_display_shape_add_graphics(UxDisplayShape *shape, UxGraphicsData *data)
{
    g_array_append_val(shape->datas, *data);
}

void ux_display_shape_render(UxDisplayShape *shape)
{
    UxDisplayObject *object = (UxDisplayObject *) shape;
    cairo_t *cr = ux_display_context_create_cairo(object->viewport->context);
    cairo_rectangle(cr, object->viewport->x, object->viewport->y, object->viewport->width, object->viewport->height);
    cairo_clip(cr);
    cairo_new_path(cr);
    ux_graphics_draw(cr, shape->datas);
    cairo_destroy(cr);
}

// ----------------------------------------------------------------------------

UxDisplayViewport *ux_display_viewport_new(UxDisplayContext *context)
{
    UxDisplayViewport *viewport = g_new(UxDisplayViewport, 1);

    ux_display_viewport_init(viewport, context);

    return viewport;
}

void ux_display_viewport_init(UxDisplayViewport *viewport, UxDisplayContext *context)
{
    viewport->context = context;
}

void ux_display_viewport_free(UxDisplayViewport *viewport)
{
    if (viewport) {
        g_free(viewport);
    }
}

void ux_display_viewport_set_root(UxDisplayViewport *viewport, UxDisplayObject *root)
{
    viewport->root = root;
}

// ----------------------------------------------------------------------------

UxDisplayContext *ux_display_context_new(GtkWidget *widget)
{
    UxDisplayContext *context = g_new(UxDisplayContext, 1);

    context->widget = widget;
    context->window = NULL;
    context->cr = NULL;

    return context;
}

void ux_display_context_free(UxDisplayContext *context)
{
//    if (context->cr) {
//        cairo_destroy(context->cr);
//    }
}
/*
void ux_display_context_init(UxDisplayContext *context, GtkWidget *widget)
{
    context->widget = widget;
    context->cr = NULL;
}
*/
cairo_t *ux_display_context_create_cairo(UxDisplayContext *context)
{
    return gdk_cairo_create(context->window);
}
