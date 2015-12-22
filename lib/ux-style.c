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

#include "ux-style.h"

#include "ux-graphics.h"
#include "ux-display.h"

#include <math.h>


GType
ux_style_background_repeat_type_get_type (void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {

            { UX_STYLE_BACKGROUND_REPEAT_REPEAT,    "UX_STYLE_BACKGROUND_REPEAT_REPEAT", "repeat" },
            { UX_STYLE_BACKGROUND_REPEAT_REPEAT_X,  "UX_STYLE_BACKGROUND_REPEAT_REPEAT_X", "repeat-x" },
            { UX_STYLE_BACKGROUND_REPEAT_REPEAT_Y,  "UX_STYLE_BACKGROUND_REPEAT_REPEAT_Y", "repeat-y" },
            { UX_STYLE_BACKGROUND_REPEAT_SPACE,     "UX_STYLE_BACKGROUND_REPEAT_SPACE", "space" },
            { UX_STYLE_BACKGROUND_REPEAT_ROUND,     "UX_STYLE_BACKGROUND_REPEAT_ROUND", "round" },
            { UX_STYLE_BACKGROUND_REPEAT_NO_REPEAT, "UX_STYLE_BACKGROUND_REPEAT_NO_REPEAT", "no-repeat" },

            { 0, NULL, NULL }
        };
        etype = g_enum_register_static (g_intern_static_string ("UxStyleBackgroundRepeatType"), values);
    }
    return etype;
}



void
ux_style_color_init(UxStyleColor *color) {
    color->type = UX_STYLE_COLOR_NONE;
    color->value = 0xFF000000;
}
void
ux_style_color_get_rgb(guint32 *color, MurrineRGB *rgb) {
    rgb->r = (gdouble) (((*color)&0xFF0000) >> 16)/G_MAXUINT8;
    rgb->g = (gdouble) (((*color)&0x00FF00) >>  8)/G_MAXUINT8;
    rgb->b = (gdouble) (((*color)&0x0000FF) >>  0)/G_MAXUINT8;
}
void
ux_style_color_get_rgba(guint32 *color, MurrineRGB *rgb, gdouble *alpha) {
    *alpha = (gdouble) (((*color)&0xFF000000) >> 24)/G_MAXUINT8;
    rgb->r = (gdouble) (((*color)&0xFF0000) >> 16)/G_MAXUINT8;
    rgb->g = (gdouble) (((*color)&0x00FF00) >>  8)/G_MAXUINT8;
    rgb->b = (gdouble) (((*color)&0x0000FF) >>  0)/G_MAXUINT8;
}
void
ux_style_color_get_uint(MurrineRGB *rgb, gdouble a, guint32 *color) {
    *color = 0x000000;
    *color |= (int) (     a*G_MAXUINT8) << 24;
    *color |= (int) (rgb->r*G_MAXUINT8) << 16;
    *color |= (int) (rgb->g*G_MAXUINT8) <<  8;
    *color |= (int) (rgb->b*G_MAXUINT8) <<  0;
}
void
ux_style_color_get_gdk(UxStyleColor *color, GdkColor *gdk)
{
    guint8 red   = (color->value&0xFF0000) >> 16;
    guint8 green = (color->value&0x00FF00) >> 8;
    guint8 blue  = (color->value&0x0000FF) >> 0;
    gdk->red   = red << 8 | red;
    gdk->green = green << 8 | green;
    gdk->blue  = blue << 8 | blue;
}
void
ux_style_color_set_from_gdk(UxStyleColor *color, GdkColor *gdk)
{
    gdouble maxuint16 = (gdouble) G_MAXUINT16;

    guint red   = gdk->red/maxuint16*G_MAXUINT8;
    guint green = gdk->green/maxuint16*G_MAXUINT8;
    guint blue  = gdk->blue/maxuint16*G_MAXUINT8;

    color->type = UX_STYLE_COLOR_RGB;
    color->value = 0x000000;
    color->value |= red << 16;
    color->value |= green << 8;
    color->value |= blue << 0;
}

void
mwb_murrine_rgb_to_hls (gdouble *r,
                    gdouble *g,
                    gdouble *b)
{
    gdouble min;
    gdouble max;
    gdouble red;
    gdouble green;
    gdouble blue;
    gdouble h, l, s;
    gdouble delta;

    red = *r;
    green = *g;
    blue = *b;

    if (red > green)
    {
        if (red > blue)
            max = red;
        else
            max = blue;

        if (green < blue)
            min = green;
        else
            min = blue;
    }
    else
    {
        if (green > blue)
            max = green;
        else
            max = blue;

        if (red < blue)
            min = red;
        else
            min = blue;
    }

    l = (max+min)/2;
    if (fabs (max-min) < 0.0001)
    {
        h = 0;
        s = 0;
    }
    else
    {
        if (l <= 0.5)
            s = (max-min)/(max+min);
        else
            s = (max-min)/(2-max-min);

        delta = max -min;
        if (red == max)
            h = (green-blue)/delta;
        else if (green == max)
            h = 2+(blue-red)/delta;
        else if (blue == max)
            h = 4+(red-green)/delta;

        h *= 60;
        if (h < 0.0)
            h += 360;
    }

    *r = h;
    *g = l;
    *b = s;
}

void
mwb_murrine_hls_to_rgb (gdouble *h,
                    gdouble *l,
                    gdouble *s)
{
    gdouble hue;
    gdouble lightness;
    gdouble saturation;
    gdouble m1, m2;
    gdouble r, g, b;

    lightness = *l;
    saturation = *s;

    if (lightness <= 0.5)
        m2 = lightness*(1+saturation);
    else
        m2 = lightness+saturation-lightness*saturation;

    m1 = 2*lightness-m2;

    if (saturation == 0)
    {
        *h = lightness;
        *l = lightness;
        *s = lightness;
    }
    else
    {
        hue = *h+120;
        while (hue > 360)
            hue -= 360;
        while (hue < 0)
            hue += 360;

        if (hue < 60)
            r = m1+(m2-m1)*hue/60;
        else if (hue < 180)
            r = m2;
        else if (hue < 240)
            r = m1+(m2-m1)*(240-hue)/60;
        else
            r = m1;

        hue = *h;
        while (hue > 360)
            hue -= 360;
        while (hue < 0)
            hue += 360;

        if (hue < 60)
            g = m1+(m2-m1)*hue/60;
        else if (hue < 180)
            g = m2;
        else if (hue < 240)
            g = m1+(m2-m1)*(240-hue)/60;
        else
            g = m1;

        hue = *h-120;
        while (hue > 360)
            hue -= 360;
        while (hue < 0)
            hue += 360;

        if (hue < 60)
            b = m1+(m2-m1)*hue/60;
        else if (hue < 180)
            b = m2;
        else if (hue < 240)
            b = m1+(m2-m1)*(240-hue)/60;
        else
            b = m1;

        *h = r;
        *l = g;
        *s = b;
    }
}

void
mwb_murrine_shade (const MurrineRGB *a, float k, MurrineRGB *b)
{
    double red;
    double green;
    double blue;

    red   = a->r;
    green = a->g;
    blue  = a->b;

    if (k == 1.0)
    {
        b->r = red;
        b->g = green;
        b->b = blue;
        return;
    }

    mwb_murrine_rgb_to_hls (&red, &green, &blue);

    green *= k;
    if (green > 1.0)
        green = 1.0;
    else if (green < 0.0)
        green = 0.0;

    blue *= k;
    if (blue > 1.0)
        blue = 1.0;
    else if (blue < 0.0)
        blue = 0.0;

    mwb_murrine_hls_to_rgb (&red, &green, &blue);

    b->r = red;
    b->g = green;
    b->b = blue;
}

static gpointer
ux_rc_color_copy (gpointer boxed)
{
    UxStyleColor *org = (UxStyleColor*) boxed;
    UxStyleColor *object = g_new(UxStyleColor, 1);

    object->type = org->type;
    object->value = org->value;

    return (gpointer) object;
}

static void
ux_rc_color_free (gpointer boxed)
{
#if defined(USE_TRACE) && USE_TRACE
    g_print("::%s\n", G_STRFUNC);
#endif

    g_print("%s() not implemented\n", G_STRFUNC);// g_error
}

static gpointer
ux_rc_background_copy (gpointer boxed)
{
#if defined(USE_TRACE) && USE_TRACE
    g_print("::%s\n", G_STRFUNC);
#endif
    UxStyleBackground *org = (UxStyleBackground*) boxed;
    UxStyleBackground *object = g_new(UxStyleBackground, 1);
    /// \todo ux_style_background_init(object);

    object->color = org->color;

    object->image.type = org->image.type;
    if (org->image.value) { // some time
        object->image.value = g_strdup(org->image.value);
    } else {
        object->image.value = NULL;
    }
    object->image.data = org->image.data;

    object->position = org->position;

    object->repeat = org->repeat;
    //object->size = org->size;

    return (gpointer) object;
}

static void
ux_rc_background_free (gpointer boxed)
{
#if defined(USE_TRACE) && USE_TRACE
    g_print("::%s\n", G_STRFUNC);
#endif

    g_print("%s() not implemented\n", G_STRFUNC);// g_error
    /*
    FIXME: UxStyleBackground *background = *boxed;
    UxStyleBackground *background = (UxStyleBackground*) boxed;
    ux_style_background_free(background);
    g_free(boxed);
    */
}

static gpointer
ux_rc_path_copy (gpointer boxed)
{
#if defined(USE_TRACE) && USE_TRACE
    g_print("::%s\n", G_STRFUNC);
#endif

    UxStylePath *org = (UxStylePath*) boxed;
    UxStylePath *object = g_new(UxStylePath, 1);

    object->segments = org->segments;

    return (gpointer) object;
}

static void
ux_rc_path_free (gpointer boxed)
{
#if defined(USE_TRACE) && USE_TRACE
    g_print("::%s\n", G_STRFUNC);
#endif
    g_print("%s() not implemented\n", G_STRFUNC);// g_error

    /*
    FIXME: UxStyleBackground *background = *boxed;
    UxStyleBackground *background = (UxStyleBackground*) boxed;
    ux_style_background_free(background);
    g_free(boxed);
    */
}

GType
ux_style_color_get_type ()
{
  static GType ux_style_color_type = 0;

  if (!ux_style_color_type)
    {
      ux_style_color_type = g_boxed_type_register_static ("UxStyleColor",
                                                           ux_rc_color_copy,
                                                           ux_rc_color_free);
    }

  return ux_style_color_type;
}

GType
ux_style_background_get_type ()
{
  static GType ux_style_background_type = 0;

  if (!ux_style_background_type)
    {
      ux_style_background_type = g_boxed_type_register_static ("UxStyleBackground",
                                                                ux_rc_background_copy,
                                                                ux_rc_background_free);
    }

  return ux_style_background_type;
}

GType
ux_style_path_get_type ()
{
  static GType ux_style_path_type = 0;

  if (!ux_style_path_type)
    {
      ux_style_path_type = g_boxed_type_register_static ("UxStylePath",
                                                                ux_rc_path_copy,
                                                                ux_rc_path_free);
    }

  return ux_style_path_type;
}

static void
ux_style_image_linear_gradient_free(UxStyleImageLinearGradient *gradient)
{
    if (gradient->stops)
      {
        g_array_free(gradient->stops, FALSE);
      }

    g_free(gradient);
}

// ux_style_background_new()
void
ux_style_background_free(UxStyleBackground *background)
{
    switch (background->image.type)
      {
        case UX_STYLE_IMAGE_LINEAR_GRADIENT:
            if (background->image.value)
              {
                g_free(background->image.value);
              }
            ux_style_image_linear_gradient_free(background->image.data.linear_gradient);
            break;
      }

    g_free(background);
}


void
ux_style_background_init(UxStyleBackground *background)
{
    background->color.type  = UX_STYLE_COLOR_NONE;
    background->color.value = 0x000000;

    background->image.type = UX_STYLE_IMAGE_NONE;
    background->image.value = g_strdup("none");
    background->image.data.linear_gradient = NULL;

    background->position.x = UX_STYLE_POSITION_CENTER;
    background->position.y = UX_STYLE_POSITION_CENTER;

    background->repeat.x = UX_STYLE_BACKGROUND_REPEAT_REPEAT_X;
    background->repeat.y = UX_STYLE_BACKGROUND_REPEAT_REPEAT_Y;

}

gdouble ux_style_path_stroke_hint_none(gdouble value) { return value;}
gdouble ux_style_path_stroke_hint_ceil(gdouble value) { return ceil(value)+05;}
gdouble ux_style_path_stroke_hint_floor(gdouble value) { return floor(value)+0.5;}
gdouble ux_style_path_stroke_hint_round(gdouble value) { return round(value)+0.5;}

void
ux_style_path_init(UxStylePath *path)
{
    path->segments  = NULL;
}

#define UX_STYLE_LENGTH_INIT {0.0, UX_STYLE_LENGTH_TYPE_PX, NULL}
gdouble
ux_style_length_get_value(UxStyleLength *length, gdouble context) {
    gdouble value;
    switch(length->unit)
    {
    case UX_STYLE_LENGTH_TYPE_PERCENTAGE:
    case UX_STYLE_LENGTH_TYPE_NUMBER:
        value = length->value*context;
        break;
    case UX_STYLE_LENGTH_TYPE_PX:
        value = length->value;
        break;
    case UX_STYLE_LENGTH_TYPE_UNKNOW:
    default:
        g_print("Error: Can not compute UxStyleLength without unit");
        break;
    }
    return value;
}

void
ux_style_border_init(UxStyleBorder *border)
{
    UxStyleColor none = UX_STYLE_COLOR_INIT;
    UxStyleLength zero = UX_STYLE_LENGTH_INIT;

    border->color.top = none;
    border->color.right = none;
    border->color.bottom = none;
    border->color.left = none;

    border->width.top = zero;
    border->width.right = zero;
    border->width.bottom = zero;
    border->width.left = zero;

    border->path.top.segments = NULL;
    border->path.right.segments = NULL;
    border->path.bottom.segments = NULL;
    border->path.top.segments = NULL;
}

UxStyleBorder*
ux_style_border_new()
{
    UxStyleBorder *border = g_new(UxStyleBorder, 1);
    ux_style_border_init(border);
    return border;
}

void
ux_style_border_free(UxStyleBorder *border)
{
    /// \FIXME
    /// g_list_free_full(border->path.top.segments, ux_style_path_segment_free);

    if (border->path.top.segments) {
        g_list_free(border->path.top.segments);
    }
    if (border->path.right.segments) {
        g_list_free(border->path.right.segments);
    }
    if (border->path.bottom.segments) {
        g_list_free(border->path.bottom.segments);
    }
    if (border->path.left.segments) {
        g_list_free(border->path.left.segments);
    }
}


void
ux_style_padding_init(UxStylePadding *padding)
{
    UxStyleLength zero = UX_STYLE_LENGTH_INIT;

    padding->top = zero;
    padding->right = zero;
    padding->bottom = zero;
    padding->left = zero;
}

UxStylePadding*
ux_style_padding_new()
{
    UxStylePadding *padding = g_new(UxStylePadding, 1);
    ux_style_padding_init(padding);
    return padding;
}

void
ux_style_padding_free(UxStylePadding* padding)
{
    if (padding) {
        g_free(padding);
    }
}

void ux_style_init(UxStyle *style)
{
    style->background = NULL;
    style->border = NULL;
    style->paint = UX_STYLE_PAINT_TYPE_FILL_AND_STROKE;
    style->color = NULL;
    style->padding = NULL;
    //...
}

void
ux_paint_box(GtkStyle           *style,
             GdkWindow          *window,
             GtkStateType        state_type,
             GtkShadowType       shadow_type,
             const GdkRectangle *area,
             GtkWidget          *widget,
             const gchar        *detail,
             gint                x,
             gint                y,
             gint                width,
             gint                height)
{
    /// \todo ajouter les fichier .css

    UxStyleBackground *bg_property = NULL;
    UxStyleBorder *border = ux_style_border_new();
    UxStylePadding *padding = ux_style_padding_new();

    UxStyleColor *color_property = NULL;
    gint width_property;


    ux_rc_parser_set_context(widget);
    {
        if (state_type==GTK_STATE_INSENSITIVE) {
            gtk_widget_style_get (widget,
                                  "background-insensitive", &bg_property,
                                  NULL);
        } else {

            gtk_widget_style_get (widget,
                                  "bar-border-top-color", &color_property,
                                  NULL);
            if (color_property) {
                border->color.top = *color_property;
                //ux_style_color_set_from_gdk(&border->color.top, color_property);
                //g_slice_free(GdkColor, color_property);
                color_property = NULL;
            }
            gtk_widget_style_get (widget,
                                  "bar-border-bottom-color", &color_property,
                                  NULL);
            if (color_property) {
                border->color.bottom = *color_property;
                //ux_style_color_set_from_gdk(&border->color.bottom, color_property);
                //g_slice_free(GdkColor, color_property);
                color_property = NULL;
            }
            gtk_widget_style_get (widget,
                                  "bar-border-top-width", &width_property,
                                  NULL);
            if (width_property) {
                border->width.top.value = (gdouble) width_property;
            }
            gtk_widget_style_get (widget,
                                  "bar-border-bottom-width", &width_property,
                                  NULL);
            if (width_property) {
                border->width.bottom.value = width_property;
            }

            gtk_widget_style_get (widget,
                                  "background", &bg_property,
                                  NULL);
        }

        gint padding_top_property = 0;
        gint padding_right_property = 0;
        gint padding_bottom_property = 0;
        gint padding_left_property = 0;

        gtk_widget_style_get (widget,
                              "bar-padding-top", &padding_top_property,
//                              "bar-padding-right", &padding_right_property,
//                              "bar-padding-bottom", &padding_bottom_property,
//                              "bar-padding-left", &padding_left_property,
                              NULL);
        if (padding_top_property)
            padding->top.value = padding_top_property;
        if (padding_right_property)
            padding->right.value = padding_right_property;
        if (padding_bottom_property)
            padding->bottom.value = padding_bottom_property;
        if (padding_left_property)
            padding->left.value = padding_left_property;


    }
    ux_rc_parser_set_context(NULL);

    UxDisplayContext *context = ux_display_context_new(widget);
    context->window = window;
    UxDisplayViewport *viewport = ux_display_viewport_new(context);
    viewport->x = x;
    viewport->y = y;
    viewport->width = width;
    viewport->height = height;

    UxStyle ux_style;
    ux_style_init(&ux_style);
    ux_style.background = bg_property;
    ux_style.border = border;
    ux_style.padding = padding;

    UxDisplayShape *box = ux_display_shape_new(viewport, NULL/* UxDisplayObject *parent */);
    ux_display_shape_set_style(box, &ux_style);

    ux_display_viewport_set_root(viewport, (UxDisplayObject *)box);

    //ux_display_object_render(box, &ux_style);
    ux_display_shape_render(box);



    //gtk_paint_box(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);
}

void
ux_paint_extension(GtkStyle           *style,
                   GdkWindow          *window,
                   GtkStateType        state_type,
                   GtkShadowType       shadow_type,
                   const GdkRectangle *area,
                   GtkWidget          *widget,
                   const gchar        *detail,
                   gint                x,
                   gint                y,
                   gint                width,
                   gint                height,
                   GtkPositionType     gap_side)
{

    ux_rc_parser_set_context(widget);
    UxStyleBackground *bg_active_property=NULL;
    UxStyleBackground *bg_property = NULL;
    UxStylePath *path_top_property = NULL;
    UxStylePath *path_right_property = NULL;
    UxStylePath *path_bottom_property = NULL;
    UxStylePath *path_left_property = NULL;


    UxStyleColor *border_top_color_property = NULL;
    UxStyleColor *border_right_color_property = NULL;
    UxStyleColor *border_bottom_color_property = NULL;
    UxStyleColor *border_left_color_property = NULL;

    gint border_top_width_property = 0;
    gint border_right_width_property = 0;
    gint border_bottom_width_property = 0;
    gint border_left_width_property = 0;

    gtk_widget_style_get (widget,
                          "tab-background-active", &bg_active_property,
                          NULL
    );
    gtk_widget_style_get (widget,
                          "tab-background", &bg_property,
                          "tab-border-top-path", &path_top_property,
                          "tab-border-right-path", &path_right_property,
                          "tab-border-bottom-path", &path_bottom_property,
                          "tab-border-left-path", &path_left_property,

                          "tab-border-top-color", &border_top_color_property,
                          "tab-border-right-color", &border_right_color_property,
                          "tab-border-bottom-color", &border_bottom_color_property,
                          "tab-border-left-color", &border_left_color_property,

                          NULL);
    if (state_type!=GTK_STATE_ACTIVE) {
        gtk_widget_style_get (widget,
                              "tab-border-top-width", &border_top_width_property,
                              "tab-border-right-width", &border_right_width_property,
                              "tab-border-bottom-width", &border_bottom_width_property,
                              "tab-border-left-width", &border_left_width_property,
                              NULL
        );
    }

    ux_rc_parser_set_context(NULL);

    UxDisplayContext *context = ux_display_context_new(widget);
    context->window = window;
    UxDisplayViewport *viewport = ux_display_viewport_new(context);
    viewport->x = x;
    viewport->y = y;
    viewport->width = width;
    viewport->height = height;

    UxStyle ux_style;
    ux_style_init(&ux_style);
    ux_style.paint = UX_STYLE_PAINT_TYPE_STROKE_AND_FILL;
    ux_style.border = ux_style_border_new();
    if (path_top_property)
      ux_style.border->path.top = *path_top_property;
    if (path_right_property)
      ux_style.border->path.right = *path_right_property;
    if (path_bottom_property)
      ux_style.border->path.bottom = *path_bottom_property;
    if (path_left_property)
      ux_style.border->path.left = *path_left_property;

    if (border_top_color_property)
      ux_style.border->color.top = *border_top_color_property;
    if (border_right_color_property)
      ux_style.border->color.right = *border_right_color_property;
    if (border_bottom_color_property)
      ux_style.border->color.bottom = *border_bottom_color_property;
    if (border_left_color_property)
      ux_style.border->color.left = *border_left_color_property;


    if (border_top_width_property)
      ux_style.border->width.top.value = border_top_width_property;
    if (border_right_width_property)
      ux_style.border->width.right.value = border_right_width_property;
    if (border_bottom_width_property)
      ux_style.border->width.bottom.value = border_bottom_width_property;
    if (border_left_width_property)
      ux_style.border->width.left.value = border_left_width_property;

    UxStylePadding *padding = ux_style_padding_new();
    gint padding_top_property = 0;
//          gint padding_right_property = 0;
//          gint padding_bottom_property = 0;
//          gint padding_left_property = 0;

    gtk_widget_style_get (widget,
                          "bar-padding-top", &padding_top_property,
//                                "bar-padding-right", &padding_right_property,
//                                "bar-padding-bottom", &padding_bottom_property,
//                                "bar-padding-left", &padding_left_property,
                          NULL);
    if (padding_top_property)
        padding->top.value = padding_top_property;
//          if (padding_right_property)
//              padding->right.value = padding_right_property;
//          if (padding_bottom_property)
//              padding->bottom.value = padding_bottom_property;
//          if (padding_left_property)
//              padding->left.value = padding_left_property;


    if (state_type==GTK_STATE_ACTIVE) {
        //ux_style.background = bg_active_property;
    } else {
        ux_style.background = bg_property;
    }
    ux_style.padding = padding;
  //ux_style...
    UxDisplayShape *box = ux_display_shape_new(viewport, NULL);// UxDisplayObject *parent
    ux_display_shape_set_style(box, &ux_style);

    ux_display_viewport_set_root(viewport, (UxDisplayObject *)box);

    //ux_display_object_render(box, &ux_style);
    ux_display_shape_render(box);
    // ----------------

}
