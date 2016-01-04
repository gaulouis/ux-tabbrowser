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

#ifndef UX_STYLE_CONTEXT_H
#define UX_STYLE_CONTEXT_H

//#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS


typedef enum   _UxStylePaintType            UxStylePaintType;
typedef enum   _UxStyleLengthType           UxStyleLengthType;
typedef struct _UxStyleLength               UxStyleLength;
typedef enum   _UxStyleSideType             UxStyleSideType;
typedef enum   _UxStyleDirectionType        UxStyleDirectionType;
typedef struct _UxStyleDirection            UxStyleDirection;
typedef enum   _UxStylePositionType         UxStylePositionType;
typedef enum   _UxStyleColorType            UxStyleColorType;
typedef struct _UxStyleColor                UxStyleColor;
typedef struct _UxStyleColorStop            UxStyleColorStop;
typedef enum   _UxStyleImageType            UxStyleImageType;
typedef struct _UxStyleImage                UxStyleImage;
typedef struct _UxStyleImageLinearGradient  UxStyleImageLinearGradient;
typedef struct _UxStyleBackground           UxStyleBackground;
typedef struct _UxStyleBorder               UxStyleBorder;
typedef struct _UxStyleBorderImage          UxStyleBorderImage;
typedef struct _UxStylePathElement          UxStylePathElement;
typedef struct _UxStylePath                 UxStylePath;
typedef enum   _UxStyleBackgroundRepeatType UxStyleBackgroundRepeatType;
typedef struct _UxStyleBackgroundRepeat     UxStyleBackgroundRepeat;
typedef struct _UxStyleBackgroundPosition   UxStyleBackgroundPosition;
typedef struct _UxStylePadding              UxStylePadding;
typedef struct _UxStyleBorderWidth          UxStyleBorderWidth;
typedef struct _UxStyleBorderColor          UxStyleBorderColor;
typedef struct _UxStyleBorderPath           UxStyleBorderPath;
typedef struct _UxStyleMatrix               UxStyleMatrix;


#define UX_TYPE_STYLE_COLOR               (ux_style_color_get_type ())
#define UX_TYPE_STYLE_BACKGROUND          (ux_style_background_get_type ())
#define UX_TYPE_STYLE_PATH                (ux_style_path_get_type ())


enum _UxStyleLengthType {
    UX_STYLE_LENGTH_TYPE_UNKNOW = 0,
    UX_STYLE_LENGTH_TYPE_NUMBER,
    UX_STYLE_LENGTH_TYPE_PERCENTAGE,
    UX_STYLE_LENGTH_TYPE_PX
};
struct _UxStyleLength
{
    gdouble            value;
    UxStyleLengthType unit;
    void              *context;/* the context for relative length %percentage : UxDisplayViewport(GtkWidget) */
};
gdouble ux_style_length_get_value(UxStyleLength *length, gdouble context);

enum _UxStyleColorType
{
    UX_STYLE_COLOR_UNKNOWN = 0,
    UX_STYLE_COLOR_NONE,
    UX_STYLE_COLOR_TRANSPARENT,
    UX_STYLE_COLOR_RGB,
    UX_STYLE_COLOR_RGBA,
    UX_STYLE_COLOR_ICC
};

typedef struct _MurrineRGB MurrineRGB;
struct _MurrineRGB {
    gdouble r;
    gdouble g;
    gdouble b;
};

#define UX_STYLE_COLOR_INIT { UX_STYLE_COLOR_NONE, 0xFF000000 }
void ux_style_color_init(UxStyleColor *color);

void ux_style_color_get_rgb(guint32 *color, MurrineRGB *rgb);
void ux_style_color_get_rgba(guint32 *color, MurrineRGB *rgb, gdouble *alpha);
void ux_style_color_get_uint(MurrineRGB *rgb, gdouble a, guint32 *color);
void ux_style_color_get_gdk(UxStyleColor *color, GdkColor *gdk);

void mwb_murrine_rgb_to_hls (gdouble *r, gdouble *g, gdouble *b);
void mwb_murrine_hls_to_rgb (gdouble *h, gdouble *l, gdouble *s);
void mwb_murrine_shade (const MurrineRGB *a, float k, MurrineRGB *b);

enum _UxStylePaintType
{
    UX_STYLE_PAINT_TYPE_STROKE_AND_FILL,
    UX_STYLE_PAINT_TYPE_FILL_AND_STROKE
};

struct _UxStyleColor
{
    UxStyleColorType type;
    guint32 value;
};

struct _UxStyleColorStop {
    UxStyleLength offset;
    UxStyleColor  color;
};

enum _UxStyleImageType
{
    UX_STYLE_IMAGE_UNKNOWN = 0,
    UX_STYLE_IMAGE_NONE,
    UX_STYLE_IMAGE_EMBED,// data_uri
    UX_STYLE_IMAGE_URI,
    UX_STYLE_IMAGE_LINEAR_GRADIENT,
};

enum _UxStyleDirectionType {
    UX_STYLE_DIRECTION_UNKNOW,
    UX_STYLE_DIRECTION_NONE,
    UX_STYLE_DIRECTION_ANGLE,
    UX_STYLE_DIRECTION_SIDE,
    UX_STYLE_DIRECTION_CORNER
};

enum _UxStyleSideType {
    UX_STYLE_SIDE_UNKNOW,
    UX_STYLE_SIDE_TOP,
    UX_STYLE_SIDE_BOTTOM,
    UX_STYLE_SIDE_LEFT,
    UX_STYLE_SIDE_RIGHT
};

enum _UxStylePositionType {
    UX_STYLE_POSITION_UNKNOW,
    UX_STYLE_POSITION_TOP,
    UX_STYLE_POSITION_BOTTOM,
    UX_STYLE_POSITION_CENTER,
    UX_STYLE_POSITION_LEFT,
    UX_STYLE_POSITION_RIGHT
};

struct _UxStyleDirection {
    UxStyleDirectionType type;
    UxStyleSideType side;
    UxStyleSideType corner;
    gdouble angle;
};

struct _UxStyleImageLinearGradient
{
    UxStyleDirection direction;
    GArray *stops;
};

struct _UxStyleImage
{
    UxStyleImageType type;
    gchar *value;
    union {
        UxStyleImageLinearGradient *linear_gradient;
    } data;
};

enum _UxStyleBackgroundRepeatType
{
    UX_STYLE_BACKGROUND_REPEAT_REPEAT,
    UX_STYLE_BACKGROUND_REPEAT_REPEAT_X,
    UX_STYLE_BACKGROUND_REPEAT_REPEAT_Y,
    UX_STYLE_BACKGROUND_REPEAT_SPACE,
    UX_STYLE_BACKGROUND_REPEAT_ROUND,
    UX_STYLE_BACKGROUND_REPEAT_NO_REPEAT
};

GType ux_style_background_repeat_type_get_type (void) G_GNUC_CONST;
#define UX_TYPE_STYLE_BACKGROUND_REPEAT_TYPE (ux_style_background_repeat_type_get_type ())

struct _UxStyleBackgroundRepeat
{
    UxStyleBackgroundRepeatType x;
    UxStyleBackgroundRepeatType y;
};

struct _UxStyleBackgroundPosition
{
    UxStylePositionType x;
    UxStylePositionType y;
};

/**
 * Boxed
 * @brief The _UxStyleBackground struct
 */
struct _UxStyleBackground
{
    UxStyleColor color;
    UxStyleImage image;
    UxStyleBackgroundPosition position;
    UxStyleBackgroundRepeat repeat;
    //UxStyleBackgroundSize size;// width height, contain, cover, auto
    //UxStyleBackgroundOrigin origin;// border-box, padding-box, content-box
    //UxStyleBackgroundClip clip;// border-box, padding-box, content-box
};

typedef enum _UxStylePathCommandType {
    UX_STYLE_PATH_COMMAND_TYPE_MOVE_TO,
    UX_STYLE_PATH_COMMAND_TYPE_LINE_TO,
//    UX_STYLE_PATH_COMMAND_TYPE_QUAD_TO,
    UX_STYLE_PATH_COMMAND_TYPE_CURVE_TO,
    UX_STYLE_PATH_COMMAND_TYPE_CLOSE,
    UX_STYLE_PATH_COMMAND_TYPE_REL_MOVE_TO,
    UX_STYLE_PATH_COMMAND_TYPE_REL_LINE_TO,
//    UX_STYLE_PATH_COMMAND_TYPE_REL_QUAD_TO,
    UX_STYLE_PATH_COMMAND_TYPE_REL_CURVE_TO,
    UX_STYLE_PATH_COMMAND_TYPE_REL_CLOSE,

    UX_STYLE_PATH_COMMAND_LENGTH,
    UX_STYLE_PATH_COMMAND_TYPE_UNKNOW
} UxStylePathCommandType;

typedef gdouble (*UxStylePathDataHint)(gdouble);
gdouble ux_style_path_stroke_hint_none(gdouble value);
gdouble ux_style_path_stroke_hint_ceil(gdouble value);
gdouble ux_style_path_stroke_hint_floor(gdouble value);
gdouble ux_style_path_stroke_hint_round(gdouble value);
#define ux_style_path_fill_hint_none ux_style_path_stroke_hint_none
#define ux_style_path_fill_hint_ceil ceil
#define ux_style_path_fill_hint_floor floor
#define ux_style_path_fill_hint_round round

typedef struct _UxStylePathData {
    UxStyleLength length;
    UxStylePathDataHint hint;
} UxStylePathData;
struct _UxStylePathElement {
    UxStylePathCommandType type;
    gint length;
    UxStylePathData data[1];
};
typedef struct _UxStylePathElementMoveTo {
    UxStylePathElement element;
    UxStylePathData _data[2-1];
} UxStylePathElementMoveTo;

typedef struct _UxStylePathElementLineTo {
    UxStylePathElement element;
    UxStylePathData _data[2-1];
} UxStylePathElementLineTo;
typedef struct _UxStylePathElementCurveTo {
    UxStylePathElement element;
    UxStylePathData _data[6-1];
} UxStylePathElementCurveTo;

static int UxStylePathPointLength[UX_STYLE_PATH_COMMAND_LENGTH] = {
    [UX_STYLE_PATH_COMMAND_TYPE_MOVE_TO] = 2,
    [UX_STYLE_PATH_COMMAND_TYPE_LINE_TO] = 2,
//    [UX_STYLE_PATH_COMMAND_TYPE_QUAD_TO] = 4,
    [UX_STYLE_PATH_COMMAND_TYPE_CURVE_TO] = 6,
    [UX_STYLE_PATH_COMMAND_TYPE_CLOSE] = 0,
    [UX_STYLE_PATH_COMMAND_TYPE_REL_MOVE_TO] = 2,
    [UX_STYLE_PATH_COMMAND_TYPE_REL_LINE_TO] = 2,
//    [UX_STYLE_PATH_COMMAND_TYPE_REL_QUAD_TO] = 4,
    [UX_STYLE_PATH_COMMAND_TYPE_REL_CURVE_TO] = 6,
    [UX_STYLE_PATH_COMMAND_TYPE_REL_CLOSE] = 0
};


/**
 * Boxed
 * @brief The _UxStylePath struct
 */
struct _UxStylePath
{
    GList *segments; // Glist<UxStyleLength*> but UxStyleLength= {UxStyleLength; hint}
    // cairo_path_t *cr_path;
};


struct _UxStylePadding
{
    UxStyleLength top;
    UxStyleLength right;
    UxStyleLength bottom;
    UxStyleLength left;
};

struct _UxStyleBorderImage
{
    UxStyleImage source;
    //UxStyleBorderRepeat repeat; //stretch
};

struct _UxStyleBorderWidth
{
    UxStyleLength top;
    UxStyleLength right;
    UxStyleLength bottom;
    UxStyleLength left;
};

struct _UxStyleBorderColor
{
    UxStyleColor top;
    UxStyleColor right;
    UxStyleColor bottom;
    UxStyleColor left;
};


struct _UxStyleBorderPath
{
    UxStylePath top;
    UxStylePath right;
    UxStylePath bottom;
    UxStylePath left;
};

struct _UxStyleBorder
{
    //UxStyleBorderStyle style;// solid, dashed
    UxStyleBorderColor color;
    UxStyleBorderWidth width;
    UxStyleBorderPath  path;
    UxStyleBorderImage image;
    //UxStyleBorderSlice slice;
};

struct _UxStyleMatrix
{
    double xx; double yx;
    double xy; double yy;
    double x0; double y0;
};

typedef struct _UxStyle UxStyle;
struct _UxStyle
{
    UxStylePaintType   paint;
    UxStyleBackground *background;
    UxStyleColor   *color;
    //  position, column, text-justify, align, font, transform, transition
    UxStyleBorder  *border;
    UxStylePadding *padding;
    UxStyleMatrix  *matrix;
    //UxStyleMargin
    //int width
    //int height

    //UxStyleTextShadow
    //UxStyleBoxShadow
} ;
//UxStyle            *ux_style_new(void);
void                ux_style_init(UxStyle *style);
//void                ux_style_free(UxStyle *style);
void                ux_style_border_init(UxStyleBorder *border);
UxStyleBorder      *ux_style_border_new();
void                ux_style_border_free(UxStyleBorder *border);
void                ux_style_padding_init(UxStylePadding *padding);
UxStylePadding     *ux_style_padding_new();
void                ux_style_padding_free(UxStylePadding *padding);

GType               ux_style_color_get_type              (void);
GType               ux_style_background_get_type         (void);
//UxStyleBackground *ux_style_background_new(void);
void                ux_style_background_init(UxStyleBackground *background);
void                ux_style_background_free(UxStyleBackground *background);

GType               ux_style_path_get_type               (void);
void                ux_style_path_init(UxStylePath *path);
//void                ux_style_path_free(UxStylePath *path);
UxStyleMatrix      *ux_style_matrix_new(void);
void                ux_style_matrix_init_identity(UxStyleMatrix *matrix);
UxStyleMatrix      *ux_style_matrix_new_identity(void);
void                ux_style_matrix_free(UxStyleMatrix *matrix);
void                ux_style_matrix_rotate(UxStyleMatrix *matrix, gdouble a);
void                ux_style_matrix_scale(UxStyleMatrix *matrix, gdouble x, gdouble y);

void                ux_paint_box(GtkStyle           *style,
                                 GdkWindow          *window,
                                 GtkStateType        state_type,
                                 GtkShadowType       shadow_type,
                                 const GdkRectangle *area,
                                 GtkWidget          *widget,
                                 const gchar        *detail,
                                 gint                x,
                                 gint                y,
                                 gint                width,
                                 gint                height);

void                ux_paint_extension(GtkStyle           *style,
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
                                       GtkPositionType     gap_side);


G_END_DECLS

#endif // UX_STYLE_CONTEXT_H
