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

#include "ux-parser.h"

#include "ux-style.h"

#include <math.h>


typedef enum _UxRcNumberFlag
{
    UX_RC_NUMBER_NULL            = 0,
    UX_RC_NUMBER_SIGNED          = 1 << 0,
    UX_RC_NUMBER_SIGNED_POSITIVE = 1 << 1,
    UX_RC_NUMBER_SIGNED_NEGATIVE = 1 << 2,
    UX_RC_NUMBER_FLOATING        = 1 << 3,
    UX_RC_NUMBER_EXPONENTED      = 1 << 4
} UxRcNumberFlag;

static GtkWidget *ux_context = NULL;/// \FIXME : static is not great
void
ux_rc_parser_set_context(GtkWidget *widget)
{
    ux_context = widget;
}

#define UX_RC_PARSER_MAX_EXPONENT 300
/**
 * \brief ux_rc_parser_parse_number
 * integer ::= [+-]? [0-9]+
 * number ::= integer | [+-]? [0-9]* "." [0-9]+
 * example:
 * .0
 * -.0
 * +.1
 * -.97
 *
 * \todo use a temporary flag befor assignement to *flag
 *
 * \param value
 * \param ptr
 * \param end
 * \return
 */
static gchar*
ux_rc_parser_parse_double(gdouble *number, UxRcNumberFlag *flag, gchar* ptr, gchar* end/*, gboolean skip*/)
{
    gdouble integer, decimal, frac, exponent;
    int sign, expsign;
    const gchar* start = ptr;

    exponent = 0;
    integer = 0;
    frac = 1;
    decimal = 0;
    sign = 1;
    expsign = 1;
    *flag = UX_RC_NUMBER_NULL;

    // read the sign
    if (ptr < end && *ptr == '+') {
        ptr++;
        *flag |= UX_RC_NUMBER_SIGNED;
        *flag |= UX_RC_NUMBER_SIGNED_POSITIVE;
    }
    else if (ptr < end && *ptr == '-') {
        ptr++;
        sign = -1;
        *flag |= UX_RC_NUMBER_SIGNED;
        *flag |= UX_RC_NUMBER_SIGNED_NEGATIVE;
    }

    /* The first character of a number must be one of [0-9+-.] */
    if (ptr == end || ((*ptr < '0' || '9' < *ptr) && *ptr != '.'))
        return NULL;

    /* read the integer part, build right-to-left */
    const gchar* ptrStartIntPart = ptr;
    while (ptr < end && '0' <= *ptr && *ptr <= '9')
        ++ptr; /* Advance to first non-digit. */

    if (ptr != ptrStartIntPart) {
        const gchar* ptrScanIntPart = ptr - 1;
        double multiplier = 1;
        while (ptrScanIntPart >= ptrStartIntPart) {
            integer += multiplier * (double)(*(ptrScanIntPart--) - '0');
            multiplier *= 10;
        }
        /* Bail out early if this overflows. */
        if (integer < -G_MAXDOUBLE || G_MAXDOUBLE < integer)
            return NULL;
    }


    if (ptr < end && *ptr == '.') { /* read the decimals */
        ptr++;

        /* There must be a least one digit following the */
        if (ptr >= end)
            return NULL;

        while (ptr < end && *ptr >= '0' && *ptr <= '9')
            decimal += (*(ptr++) - '0') * (frac *= 0.1);

        *flag |= UX_RC_NUMBER_FLOATING;
    }

    /* read the exponent part */
    if (ptr != start && ptr + 1 < end && (*ptr == 'e' || *ptr == 'E')
        && (ptr[1] != 'x' && ptr[1] != 'm')) {
        ptr++;

        // read the sign of the exponent
        if (*ptr == '+')
            ptr++;
        else if (*ptr == '-') {
            ptr++;
            expsign = -1;
        }

        /* There must be an exponent */
        if (ptr >= end || *ptr < '0' || *ptr > '9')
            return NULL;

        while (ptr < end && *ptr >= '0' && *ptr <= '9') {
            exponent *= 10.0;
            exponent += *ptr - '0';
            ptr++;
        }
        /* Make sure exponent is valid */
        if (exponent < -G_MAXDOUBLE || G_MAXDOUBLE < exponent || exponent > UX_RC_PARSER_MAX_EXPONENT)
            return NULL;
        *flag |= UX_RC_NUMBER_EXPONENTED;
    }

    *number = integer + decimal;
    *number *= sign;

    if (exponent)
        *number *= pow(10.0, expsign * (int)exponent);

    /* Don't return Infinity() or NaN() */
    if ((*number) < -G_MAXDOUBLE || G_MAXDOUBLE < (*number))
        return NULL;

    if (start == ptr)
        return NULL;

    /*
    if (skip)
    {
        if (ptr < end && !svg_parser_is_space(*ptr) && *ptr != ',')
            return ptr;
        while (ptr < end && svg_parser_is_space(*ptr))
            ptr++;
        if (ptr < end && *ptr == ',') {
            ptr++;
            while (ptr < end && svg_parser_is_space(*ptr))
                ptr++;
        }
    }
    */

    return ptr;
}

typedef enum _UxRcNumberType
{
    UX_RC_NUMBER_UNKNOW,

    UX_RC_NUMBER_UINT,/* 0<->255 UX_RC_NUMBER_BYTE WORD */
    UX_RC_NUMBER_PERCENTAGE,/*0.0<->1.0*/

    UX_RC_NUMBER_INT,/* -17319<->17319*/
    UX_RC_NUMBER_FLOAT,/* -356350*.0<->356350*.0 */
    UX_RC_NUMBER_SCALE/*-1.0<->1.0  COSINUS SINUS TRIGO */
} UxRcNumberType;
#define UX_RC_NUMBER_IS(flags, flag) (flags & ~flag)

/**
 * \todo refactor *number
 */
gchar*
ux_rc_parser_parse_number(gdouble *number, UxRcNumberType *type, gchar* ptr, gchar* end/*, gboolean skip*/)
{
    UxRcNumberFlag flags;

    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;
    if (ptr>=end)
        return NULL;

    ptr = ux_rc_parser_parse_double(number, &flags, ptr, end);

    if (!ptr) {
        return NULL;
    }

    *type = UX_RC_NUMBER_UNKNOW;

    if (   !UX_RC_NUMBER_IS(flags, UX_RC_NUMBER_SIGNED)
        && !UX_RC_NUMBER_IS(flags, UX_RC_NUMBER_FLOATING)
        && !UX_RC_NUMBER_IS(flags, UX_RC_NUMBER_EXPONENTED)
        && *number < 256
    ) {
        *type = UX_RC_NUMBER_UINT;
    } else if(
           !UX_RC_NUMBER_IS(flags, UX_RC_NUMBER_SIGNED)
        &&  UX_RC_NUMBER_IS(flags, UX_RC_NUMBER_FLOATING)
        && *number >= 0.0
        && *number <= 1.0
    ){
        *type = UX_RC_NUMBER_PERCENTAGE;
    } else if (
            UX_RC_NUMBER_IS(flags, UX_RC_NUMBER_FLOATING)
    ) {
        *type = UX_RC_NUMBER_FLOAT;
    }

    return ptr;
}

static gchar*
ux_rc_parser_parse_color_hex(UxStyleColor *color, gchar* ptr, gchar* end)
{
    gint num = 0;
    guint8 rgb[6];//0-FF * 6

    while (ptr < end) {
      if (ux_parser_is_digit(*ptr)) {
          rgb[num] = *ptr - '0';
          ptr++;
      } else if ('A' <= *ptr && *ptr <= 'F') {
          rgb[num] = *ptr - 'A' + 10;
          ptr++;
      } else if ('a' <= *ptr && *ptr <= 'f') {
          rgb[num] = *ptr - 'a' + 10;
          ptr++;
      } else {
          //return NULL; /*ERROR_XDIGIT_EXPECTED*/
          break;
      }
      num++;
    }

    if (num==3) {
        color->value = 0xFF;// Alpha
        color->value <<= 8;
        color->value |= rgb[0] * 0x11;// red
        color->value <<= 8;
        color->value |= rgb[1] * 0x11;// green
        color->value <<= 8;
        color->value |= rgb[2] * 0x11;// blue
        return ptr;
    } else if(num==6) {
        color->value = 0xFF;// Alpha
        color->value <<= 8;
        color->value |= rgb[0]<<4 | rgb[1];// red
        color->value <<= 8;
        color->value |= rgb[2]<<4 | rgb[3];// green
        color->value <<= 8;
        color->value |= rgb[4]<<4 | rgb[5];// blue
        return ptr;
    }

    return NULL;
}

static gchar*
ux_rc_parser_parse_color_rgb(UxStyleColor *color, gchar* ptr, gchar* end)
{
    double red, green, blue, alpha;
    UxRcNumberType red_type, green_type, blue_type, alpha_type;
    gchar *token;
    ptr += 3;

    ux_parser_skip_space(ptr, end);
    if (ptr>=end) return NULL;/*ERROR_END_REACHED */
    if (*ptr!='(') return NULL;/*ERROR_CHAR_UNEXCEPTED */
    ptr++;
    token = ux_rc_parser_parse_number(&red, &red_type, ptr, end);
    if (!token)
        return NULL;/* ERROR_NUMBER_EXCEPTED */

    ptr = token;
    ux_parser_skip_space(ptr, end);
    if (ptr>=end) return NULL;/*ERROR_END_REACHED */
    if (*ptr!=',') return NULL;/*ERROR_CHAR_UNEXCEPTED */
    ptr++;
    token = ux_rc_parser_parse_number(&green, &green_type, ptr, end);
    if (!token)
        return NULL;/* ERROR_NUMBER_EXCEPTED */

    ptr = token;
    ux_parser_skip_space(ptr, end);
    if (ptr>=end) return NULL;/*ERROR_END_REACHED */
    if (*ptr!=',') return NULL;/*ERROR_CHAR_UNEXCEPTED */
    ptr++;
    token = ux_rc_parser_parse_number(&blue, &blue_type, ptr, end);
    if (!token)
        return NULL;/* ERROR_NUMBER_EXCEPTED */

    ptr = token;
    ux_parser_skip_space(ptr, end);
    if (ptr>=end) return NULL;/*ERROR_END_REACHED */
    if (*ptr!=')') return NULL;/*ERROR_CHAR_UNEXCEPTED */
    ptr++;

    if (red_type==green_type && red_type==blue_type) {
        if (red_type==UX_RC_NUMBER_UINT) {
            color->value = 0xFF;
            color->value <<= 8;
            color->value |= (guint8)red;
            color->value <<= 8;
            color->value |= (guint8)green;
            color->value <<= 8;
            color->value |= (guint8)blue;
            color->type = UX_STYLE_COLOR_RGB;
        } else {
            color->value = 0xFF;
            color->value <<= 8;
            color->value |= (guint8) round(red * 0xFF);
            color->value <<= 8;
            color->value |= (guint8) round(green * 0xFF);
            color->value <<= 8;
            color->value |= (guint8) round(blue * 0xFF);
            color->type = UX_STYLE_COLOR_RGB;
        }
        return ptr;
    } else {
        return NULL; /*ERROR_MIXED_UNIT*/
    }

    return NULL;
}

static gchar*
ux_rc_parser_parse_color_rgba(UxStyleColor *color, gchar* ptr, gchar* end)
{
    double red, green, blue, alpha;
    UxRcNumberType red_type, green_type, blue_type, alpha_type;/*FIXME more elegant*/
    gchar *token;
    ptr += 4;

    ux_parser_skip_space(ptr, end);
    if (ptr>=end) return NULL;/*ERROR_END_REACHED */
    if (*ptr!='(') return NULL;/*ERROR_CHAR_UNEXCEPTED */
    ptr++;
    token = ux_rc_parser_parse_number(&red, &red_type, ptr, end);
    if (!token)
        return NULL;/* ERROR_NUMBER_EXCEPTED */

    ptr = token;
    ux_parser_skip_space(ptr, end);
    if (ptr>=end) return NULL;/*ERROR_END_REACHED */
    if (*ptr!=',') return NULL;/*ERROR_CHAR_UNEXCEPTED */
    ptr++;
    token = ux_rc_parser_parse_number(&green, &green_type, ptr, end);
    if (!token)
        return NULL;/* ERROR_NUMBER_EXCEPTED */

    ptr = token;
    ux_parser_skip_space(ptr, end);
    if (ptr>=end) return NULL;/*ERROR_END_REACHED */
    if (*ptr!=',') return NULL;/*ERROR_CHAR_UNEXCEPTED */
    ptr++;
    token = ux_rc_parser_parse_number(&blue, &blue_type, ptr, end);
    if (!token)
        return NULL;/* ERROR_NUMBER_EXCEPTED */

    ptr = token;
    ux_parser_skip_space(ptr, end);
    if (ptr>=end) return NULL;/*ERROR_END_REACHED */
    if (*ptr!=',') return NULL;/*ERROR_CHAR_UNEXCEPTED */
    ptr++;
    token = ux_rc_parser_parse_number(&alpha, &alpha_type, ptr, end);
    if (!token)
        return NULL;/* ERROR_NUMBER_EXCEPTED */
    ptr = token;

    ux_parser_skip_space(ptr, end);
    if (ptr>=end) return NULL;/*ERROR_END_REACHED */
    if (*ptr!=')') return NULL;/*ERROR_CHAR_UNEXCEPTED */
    ptr++;

    if (red_type==green_type && red_type==blue_type && red_type==alpha_type) {
        if (red_type==UX_RC_NUMBER_UINT) {
            color->value = (guint8)alpha;
            color->value <<= 8;
            color->value |= (guint8)red;
            color->value <<= 8;
            color->value |= (guint8)green;
            color->value <<= 8;
            color->value |= (guint8)blue;
            color->type = UX_STYLE_COLOR_RGBA;
        } else {
            color->value = (guint8) round(alpha * 0xFF);
            color->value <<= 8;
            color->value |= (guint8) round(red * 0xFF);
            color->value <<= 8;
            color->value |= (guint8) round(green * 0xFF);
            color->value <<= 8;
            color->value |= (guint8) round(blue * 0xFF);
            color->type = UX_STYLE_COLOR_RGBA;
        }
        return ptr;
    } else {
        return NULL; /*ERROR_MIXED_UNIT*/
    }

    return NULL;
}

static gchar*
ux_parser_parse_color_map(UxStyleColor *color, gchar* ptr, gchar* end)
{
    /// TODO
    GtkWidget *widget = ux_context;
    GtkStyle *style = gtk_rc_get_style (widget);
    gchar color_name[1024];

    int i = 0;
    while (ux_parser_is_alpha(*ptr) || *ptr=='_') {
        color_name[i] = *ptr;
        ptr++;
        i++;
    }
    color_name[i] = '\0';

    GdkColor gdk_color;
    gboolean success = gtk_style_lookup_color (style, color_name, &gdk_color);
    if (!success) {
        // if color_name==bg_color then use ux_context->style->bg[NORMAL]
        return NULL;
    }

    color->type = UX_STYLE_COLOR_RGB;
    color->value = 0xFF000000;
    color->value |= (gdk_color.red&G_MAXUINT8) << 16;
    color->value |= (gdk_color.green&G_MAXUINT8) << 8;
    color->value |= (gdk_color.blue&G_MAXUINT8) << 0;

    return ptr;
}


static gchar*
ux_rc_parser_parse_shade(UxStyleColor *color, gchar* ptr, gchar* end)
{
    UxStyleColor style_color = UX_STYLE_COLOR_INIT;
    gchar *token = ux_parser_parse_color(&style_color, ptr, end);
    if (!token) {
        return NULL;
    }
    ptr = token;

    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;
    if (ptr>=end)
        return NULL;
    if (*ptr!=',')
        return NULL;
    ptr++;
    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;

    gdouble number;
    UxRcNumberType type;
    token = ux_rc_parser_parse_number(&number, &type, ptr, end);
    if (!token) {
        return NULL;
    }
    ptr = token;

    // compute shade
    MurrineRGB rgb;
    gdouble a;
    ux_style_color_get_rgba(&style_color.value, &rgb, &a);// FIXME &

    MurrineRGB result_rgb;
    mwb_murrine_shade(&rgb, (float) number, &result_rgb);

    ux_style_color_get_uint(&result_rgb, a, &style_color.value);

    color->type = style_color.type;
    color->value = style_color.value;

    return ptr;
}


gchar*
ux_parser_parse_color(UxStyleColor *color, gchar *ptr, gchar *end)
{
    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;
    if (ptr>=end)
        return NULL;

    if (*ptr=='@') {
        ptr++;
        gchar *token = ux_parser_parse_color_map(color, ptr, end);
        if (!token) {
            return NULL;
        }
        ptr = token;
        return ptr;
    } else if (ptr+5<end
         && ptr[0]=='s'
         && ptr[1]=='h'
         && ptr[2]=='a'
         && ptr[3]=='d'
         && ptr[4]=='e'
         && (ptr[5]=='(' || ptr[5]==' ')
    ) {
        ptr += 5;
        ux_parser_skip_space(ptr, end);
        if (ptr>=end) return NULL;/*ERROR_END_REACHED */
        if (*ptr!='(') return NULL;/*ERROR_CHAR_UNEXCEPTED */
        ptr++;
        ux_parser_skip_space(ptr, end);
        if (ptr>=end) return NULL;/*ERROR_END_REACHED */
        gchar *token = ux_rc_parser_parse_shade(color, ptr, end);
        if (!token) {
            return NULL;
        }
        ptr = token;
        ux_parser_skip_space(ptr, end);
        if (*ptr!=')') return NULL;/*ERROR_CHAR_UNEXCEPTED */
        ptr++;
        return ptr;
    } else if (*ptr=='#') {
        /* try #RRGGBB / #RGB */
        ptr++;
        gchar *token = ux_rc_parser_parse_color_hex(color, ptr, end);
        color->type = UX_STYLE_COLOR_RGB;
        return token;
    } else if (ptr+3<end
           && ptr[0]=='r'
           && ptr[1]=='g'
           && ptr[2]=='b'
           && ptr[3]=='a'
    ) {
        /* try rgba() */
        gchar *token = ux_rc_parser_parse_color_rgba(color, ptr, end);
        //if (!token) // ERROR remove color object ?
        return token;
    } else if (ptr+2<end
           && ptr[0]=='r'
           && ptr[1]=='g'
           && ptr[2]=='b'
    ) {
        /* try rgb() */
        gchar *token = ux_rc_parser_parse_color_rgb(color, ptr, end);
        //if (!token) // ERROR remove color object ?
        return token;
    } else if (ptr+3<end
            && (ptr[0]=='n' || ptr[0]=='N')
            && (ptr[1]=='o' || ptr[1]=='O')
            && (ptr[2]=='n' || ptr[2]=='N')
            && (ptr[3]=='e' || ptr[3]=='E')
    ) {
        /* try none */
        if (ptr+4<end && ux_parser_is_space(ptr[4])) {
            return NULL;
        }
        color->type  = UX_STYLE_COLOR_NONE;
        color->value = 0;
        return ptr+4;
    } else if (ptr+10<end
               && ( ptr[0]=='t' ||  ptr[0]=='T')
               && ( ptr[1]=='r' ||  ptr[1]=='R')
               && ( ptr[2]=='a' ||  ptr[2]=='A')
               && ( ptr[3]=='n' ||  ptr[3]=='N')
               && ( ptr[4]=='s' ||  ptr[4]=='S')
               && ( ptr[5]=='p' ||  ptr[5]=='P')
               && ( ptr[6]=='a' ||  ptr[6]=='A')
               && ( ptr[7]=='r' ||  ptr[7]=='R')
               && ( ptr[8]=='e' ||  ptr[8]=='E')
               && ( ptr[9]=='n' ||  ptr[9]=='N')
               && (ptr[10]=='t' || ptr[10]=='T')
    ) {
        /* try transparent */
        if (ptr+11>=end || !ux_parser_is_space(ptr[11])) {
            return NULL;
        }
        color->type  = UX_STYLE_COLOR_TRANSPARENT;
        color->value = 0xFF000000;
        return ptr+11;
    } else {
        /* try ICC */
        /* use g hash table
         *
        ux_rc_parser_skip_space(ptr, end);
        if (ptr>=end) return NULL;//ERROR_END_REACHED
        char* string = ptr;
        while (string<=end && 'a' <= *ptr && *ptr <= 'z') {
            string++;
        }
        string--;
        string = g_strndup(ptr, string-ptr);

        UxStyleColor* icc = ux_style_color_icc_lookup(string);
        if (icc) {
            color->type  = icc->type;
            color->value = icc->value;
        } else {
            color->type  = UX_STYLE_COLOR_UNKNOWN;
            color->value = 0;
        }

        ptr += strlen(string);
        g_free(string);

        return ptr;
        */
    }
    return NULL;
}
gchar*
ux_rc_parser_parse_percentage(gdouble *number, gchar* ptr, gchar* end)
{
    // 50%
    return NULL;
}

static gchar*
ux_rc_parser_parse_length_type(UxStyleLengthType *type, gchar *ptr, gchar *end)
{
    /// \todo skip space

    const gchar firstChar = *ptr;
    ptr++;

    if (firstChar == '%') {
        *type = UX_STYLE_LENGTH_TYPE_PERCENTAGE;
        return ptr;
    }

    const gchar secondChar = *ptr;

    if (ptr >= end) {
        *type = UX_STYLE_LENGTH_TYPE_NUMBER;
        return --ptr;
    }

    if (firstChar == 'p' && secondChar == 'x') {
        *type =  UX_STYLE_LENGTH_TYPE_PX;
        return ++ptr;
    } // else if em, in, pt, ...

    *type = UX_STYLE_LENGTH_TYPE_NUMBER;
    return --ptr;
}

static gchar*
ux_rc_parser_parse_length(UxStyleLength *length, gchar* ptr, gchar* end)
{
    gchar *token;
    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;
    if (ptr>=end)
        return NULL;

    gdouble number;
    UxRcNumberType type;
    token = ux_rc_parser_parse_number(&number, &type, ptr, end);
    if (!token)
        return NULL;

    ptr = token;

    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;
    //if (ptr>=end)
    //    return NULL;

    UxStyleLengthType length_type;
    token = ux_rc_parser_parse_length_type(&length_type, ptr, end);
    if (!token)
        return NULL;
    ptr = token;

    length->unit = length_type;
    length->value = number;

    return ptr;
}

static gchar*
ux_rc_parser_parse_color_stop(UxStyleColorStop *color_stop, gchar* ptr, gchar* end)
{
    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;
    if (ptr>=end)
        return NULL;

    gchar *token;
    UxStyleColor color;
    token = ux_parser_parse_color(&color, ptr, end);
    if (!token)
        return NULL;

    ptr = token;

    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;
    if (ptr>=end)
        return NULL;

    UxStyleLength length;
    token = ux_rc_parser_parse_length(&length, ptr, end);
    if (!token)
        return NULL;

    ptr = token;

    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;
    if (ptr>=end)
        return NULL;

    color_stop->color = color;
    color_stop->offset = length;

    return ptr;
}
// ----------------------------------------------------------------------------
// background
// ----------------------------------------------------------------------------

static gchar*
ux_rc_parser_parse_linear_gradient(/*UxStyleGradient *gradient, */gchar *ptr, gchar *end)
{

}

static gchar*
ux_rc_parser_parse_side(UxStyleSideType *direction, gboolean *finish, gchar *ptr, gchar *end)
{
    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;

    if (ptr+3<end
        && (ptr[0] == 't' || ptr[0] == 'T')
        && (ptr[1] == 'o' || ptr[1] == 'O')
        && (ptr[2] == 'p' || ptr[2] == 'P')
        && (ux_parser_is_space(ptr[3]) || ptr[3] == ',')
    ) {
        *direction = UX_STYLE_SIDE_TOP;
        ptr += 3;
    } else if (ptr+6<end
        && (ptr[0] == 'b' || ptr[0] == 'B')
        && (ptr[1] == 'o' || ptr[1] == 'O')
        && (ptr[2] == 't' || ptr[2] == 'T')
        && (ptr[3] == 't' || ptr[3] == 'T')
        && (ptr[4] == 'o' || ptr[4] == 'O')
        && (ptr[5] == 'm' || ptr[5] == 'M')
        && (ux_parser_is_space(ptr[6]) || ptr[6] == ',')
    ) {
        *direction = UX_STYLE_SIDE_TOP;
        ptr += 6;
    } else if (ptr+4<end
        && (ptr[0] == 'l' || ptr[0] == 'L')
        && (ptr[1] == 'e' || ptr[1] == 'E')
        && (ptr[2] == 'f' || ptr[2] == 'F')
        && (ptr[3] == 't' || ptr[3] == 'T')
        && (ux_parser_is_space(ptr[4]) || ptr[4] == ',')
    ) {
        *direction = UX_STYLE_SIDE_LEFT;
        ptr += 4;
    } else if (ptr+5<end
        && (ptr[0] == 'r' || ptr[0] == 'R')
        && (ptr[1] == 'i' || ptr[1] == 'I')
        && (ptr[2] == 'g' || ptr[2] == 'G')
        && (ptr[3] == 'h' || ptr[3] == 'H')
        && (ptr[4] == 't' || ptr[4] == 'T')
        && (ux_parser_is_space(ptr[5]) || ptr[5] == ',')
    ) {
        *direction = UX_STYLE_SIDE_RIGHT;
        ptr += 5;
    } else {
        *direction = UX_STYLE_SIDE_UNKNOW;
        return NULL;
    }

    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;
    *finish = FALSE;
    if (*ptr==',') {
        *finish = TRUE;
        ptr++;
    }

    return ptr;
}

static gchar*
ux_rc_parser_parse_direction(UxStyleDirection *direction, gchar *ptr, gchar *end)
{
    gchar * token;
    UxStyleSideType side;
    UxStyleDirectionType type;
    UxStyleSideType v_side = UX_STYLE_SIDE_UNKNOW;
    UxStyleSideType h_side = UX_STYLE_SIDE_UNKNOW;
    gdouble angle = 0.0;
    gboolean finish;
    token = ux_rc_parser_parse_side(&side, &finish, ptr, end);
    if (token) {
        ptr = token;
        if  (finish) {
            // side
            type = UX_STYLE_DIRECTION_SIDE;
            v_side = side;
            ptr = token;
        } else if (side==UX_STYLE_SIDE_TOP || side==UX_STYLE_SIDE_BOTTOM) {
            // corner
            v_side = side;
            token = ux_rc_parser_parse_side(&side, &finish, ptr, end);
            if (token && finish && (side==UX_STYLE_SIDE_LEFT || side==UX_STYLE_SIDE_RIGHT)) {
                h_side = side;
                type = UX_STYLE_DIRECTION_CORNER;
                ptr = token;
            } else {
                // ERROR: ([left|right],) not found after top|bottom
                type = UX_STYLE_DIRECTION_UNKNOW;
                return NULL;
            }
        } else {
            // ERROR: Don't start by top|bottom
            type = UX_STYLE_DIRECTION_UNKNOW;
            return NULL;
        }
    } else {
        // angle
        // there are no sides; check angle, else use left to right
        gdouble value;
        UxRcNumberType type;
        token = ux_rc_parser_parse_number(&value, &type, ptr, end);
        if (token && type==UX_RC_NUMBER_UINT) {
            ptr = token;
            if (ptr+3<end && ptr[0]=='d' && ptr[1]=='e' && ptr[2]=='g' && (ux_parser_is_space(ptr[3]) || ptr[3]==',')) {
                angle = value;
            } else {
                // ERROR no "deg" specified
                return NULL;
            }
        } else {
            // ERROR expected number
            return NULL;
        }
    }
    direction->type = type;
    direction->side = v_side;
    direction->corner = h_side;
    direction->angle = angle;

    return ptr;
}

gchar*
ux_parser_parse_image(UxStyleImage *image, gchar *ptr, gchar *end)
{
    gchar *token;
    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;
    if (ptr>=end)
        return NULL;

    if (ptr+2<end
           && ptr[0]=='u'
           && ptr[1]=='r'
           && ptr[2]=='l'
    ) {
        /*try url(img.png) */
        ptr += 3;
        if (*ptr!='(') {
            return NULL;
        }

        // start quote
        gboolean use_quote = FALSE;
        ptr++;
        while (ptr < end && ux_parser_is_space(*ptr))
            ptr++;
        if (ptr>=end)
            return NULL;
        if (*ptr=='\'') {
            use_quote = TRUE;
        }

        // parse url(data:image/gif;base64,R0lGODlhEAAQAMQAAORHHOVSKudfOulrSOp3WOyDZu6QdvCchPGolfO0o/XBs/fNwfjZ0frl3/zy7////wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAkAABAALAAAAAAQABAAAAVVICSOZGlCQAosJ6mu7fiyZeKqNKToQGDsM8hBADgUXoGAiqhSvp5QAnQKGIgUhwFUYLCVDFCrKUE1lBavAViFIDlTImbKC5Gm2hB0SlBCBMQiB0UjIQA7)
        // parse url(http://www.example.com/image.png)
        // parse url(//path/image.png)
        // parse url(image/background.png) relative to data_dir
        if (ptr+10<end
           && ptr[0]=='d'
           && ptr[1]=='a'
           && ptr[2]=='t'
           && ptr[3]=='a'
           && ptr[4]==':'
           && ptr[5]=='i'
           && ptr[6]=='m'
           && ptr[7]=='a'
           && ptr[8]=='g'
           && ptr[9]=='e'
           && ptr[10]=='/'
        ) {
            // not supported
            return NULL;
        } else if(ptr+7<end
            && ptr[0]=='h'
            && ptr[1]=='t'
            && ptr[2]=='t'
            && ptr[3]=='p'
            && ptr[4]==':'
            && ptr[5]=='/'
            && ptr[6]=='/'
        ) {
            // http://
            // https://
            // //
            // ftp://
            // not supported
            return NULL;
        } else {
            // file:///home/user
            // C:\home\user
            /// \todo
            return NULL;
        }

        // end quote
        while (ptr < end && ux_parser_is_space(*ptr))
            ptr++;
        if (ptr>=end)
            return NULL;
        if (use_quote && *ptr!='\'') {
            return NULL;
        }

        while (ptr < end && ux_parser_is_space(*ptr))
            ptr++;
        if (ptr>=end)
            return NULL;
        if (*ptr!=')') {
            return NULL;// EXCEPT CLOSE bracket
        }

    } else if (ptr+15<end
           &&  ptr[0]=='l'
           &&  ptr[1]=='i'
           &&  ptr[2]=='n'
           &&  ptr[3]=='e'
           &&  ptr[4]=='a'
           &&  ptr[5]=='r'
           &&  ptr[6]=='-'
           &&  ptr[7]=='g'
           &&  ptr[8]=='r'
           &&  ptr[9]=='a'
           && ptr[10]=='d'
           && ptr[11]=='i'
           && ptr[12]=='e'
           && ptr[13]=='n'
           && ptr[14]=='t'
    ) {
        /* try linear-gradient(direction, color-stop1, color-stop2, ...) */
        // background-image: linear-gradient( [ [ [top | bottom] || [left | right] ] | <angle> ,]? <color-stop>[, <color-stop>]+)
        // <color-stop> = <color> [ <percentage> | <length> ]?
        gchar *start = ptr;
        ptr += 15;
        while (ptr < end && ux_parser_is_space(*ptr))
            ptr++;
        if (ptr>=end || *ptr!='(') {
            return NULL;
        }
        ptr++;

        UxStyleDirection direction;
        token = ux_rc_parser_parse_direction(&direction, ptr, end);
        if (!token) {
            direction.type = UX_STYLE_DIRECTION_NONE;
            direction.angle = 0.0;
        }
        ptr = token;// a revoir

        gboolean end_reached = FALSE;
        UxStyleColorStop color_stop;
        GArray *stops = g_array_new(FALSE, FALSE, sizeof(UxStyleColorStop));
        do {
            token = ux_rc_parser_parse_color_stop(&color_stop, ptr, end);
            if(token) {
                ptr = token;
                g_array_append_val(stops, color_stop);

                /// \todo skip space
                if (*ptr==',') {
                    ptr++;
                }
                if (*ptr==')') {
                    end_reached = TRUE;
                    ptr++;
                }
            }
        } while(token && !end_reached);

        UxStyleImageLinearGradient *linear_gradient = (UxStyleImageLinearGradient*) g_new(UxStyleImageLinearGradient, 1);
        linear_gradient->direction = direction;
        linear_gradient->stops = stops;
        image->type = UX_STYLE_IMAGE_LINEAR_GRADIENT;
        image->data.linear_gradient = linear_gradient;
        image->value = g_strndup(start, ptr-start);// n+1, null-termined
    }

    return ptr;
}

static gchar*
ux_rc_parser_parse_background_position_value(UxStylePositionType *position, gchar *ptr, gchar *end)
{
    gchar *start;
    gsize length;
    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;
    start = ptr;

    while (ptr < end && ux_parser_is_alpha(*ptr))
        ptr++;
    length = ptr-start;

    if (g_ascii_strncasecmp(start, "top", length)==0 && !ux_parser_is_alpha(*ptr)) {
        *position = UX_STYLE_POSITION_TOP;
        return ptr;
    } else if (g_ascii_strncasecmp(start, "bottom", length)==0 && !ux_parser_is_alpha(*ptr)) {
        *position = UX_STYLE_POSITION_BOTTOM;
        return ptr;
    } else if (g_ascii_strncasecmp(start, "center", length)==0 && !ux_parser_is_alpha(*ptr)) {
        *position = UX_STYLE_POSITION_CENTER;
        return ptr;
    } else if (g_ascii_strncasecmp(start, "left", length)==0 && !ux_parser_is_alpha(*ptr)) {
        *position = UX_STYLE_POSITION_LEFT;
        return ptr;
    } else if (g_ascii_strncasecmp(start, "right", length)==0 && !ux_parser_is_alpha(*ptr)) {
        *position = UX_STYLE_POSITION_RIGHT;
        return ptr;
    }

    return NULL;
}

gchar*
ux_parser_parse_background_position(UxStyleBackgroundPosition *position, gchar *ptr, gchar *end)
{
    gchar* token;
    UxStylePositionType x = UX_STYLE_POSITION_CENTER;
    UxStylePositionType y = UX_STYLE_POSITION_CENTER;

    token = ux_rc_parser_parse_background_position_value(&x, ptr, end);
    if (token) {
        position->x = x;
        ptr = token;
        token = ux_rc_parser_parse_background_position_value(&y, ptr, end);
        if (token) {
            position->y = y;
            ptr = token;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }

    return ptr;
}

static gchar*
ux_rc_parser_parse_background_repeat_value(UxStyleBackgroundRepeatType *repeat, gchar *ptr, gchar *end)
{
    gchar *start;
    gsize length;
    while (ptr < end && ux_parser_is_space(*ptr))
        ptr++;
    start = ptr;

    while (ptr < end && ux_parser_is_alpha(*ptr) || *ptr=='-')
        ptr++;
    length = ptr-start;

    if (g_ascii_strncasecmp(start, "no-repeat", length)==0 && !ux_parser_is_alpha(*ptr)) {
        *repeat = UX_STYLE_BACKGROUND_REPEAT_NO_REPEAT;
        return ptr;
    } else if (g_ascii_strncasecmp(start, "repeat", length)==0 && !ux_parser_is_alpha(*ptr)) {
        *repeat = UX_STYLE_BACKGROUND_REPEAT_REPEAT;
        return ptr;
    } else if (g_ascii_strncasecmp(start, "repeat-x", length)==0 && !ux_parser_is_alpha(*ptr)) {
        *repeat = UX_STYLE_BACKGROUND_REPEAT_REPEAT_X;
        return ptr;
    } else if (g_ascii_strncasecmp(start, "repeat-y", length)==0 && !ux_parser_is_alpha(*ptr)) {
        *repeat = UX_STYLE_BACKGROUND_REPEAT_REPEAT_Y;
        return ptr;
    }

    return NULL;
}

gchar*
ux_parser_parse_background_repeat(UxStyleBackgroundRepeat *repeat, gchar *ptr, gchar *end)
{
    gchar* token;
    UxStyleBackgroundRepeatType x = UX_STYLE_BACKGROUND_REPEAT_REPEAT;
    UxStyleBackgroundRepeatType y = UX_STYLE_BACKGROUND_REPEAT_REPEAT;

    token = ux_rc_parser_parse_background_repeat_value(&x, ptr, end);
    if (token) {
        repeat->x = x;
        ptr = token;
        token = ux_rc_parser_parse_background_repeat_value(&y, ptr, end);
        if (token) {
            repeat->y = y;
            ptr = token;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }

    return ptr;
}

// ----------------------------------------------------------------------------
// path
// ----------------------------------------------------------------------------

UxStylePathElement *
ux_style_path_element_rel_move_to_new()
{
    UxStylePathElement *element;
    element = (UxStylePathElement *) g_new(UxStylePathElementMoveTo, 1);
    element->type = UX_STYLE_PATH_COMMAND_TYPE_REL_MOVE_TO;
    element->length = 2;//UxStylePathPointLength[UX_STYLE_PATH_COMMAND_TYPE_REL_MOVE_TO];
    return element;
}
UxStylePathElement *
ux_style_path_element_rel_line_to_new()
{
    UxStylePathElement *element;
    element = (UxStylePathElement *) g_new(UxStylePathElementLineTo, 1);
    element->type = UX_STYLE_PATH_COMMAND_TYPE_REL_LINE_TO;
    element->length = 2;//UxStylePathPointLength[UX_STYLE_PATH_COMMAND_TYPE_REL_LINE_TO];
    return element;
}
UxStylePathElement *
ux_style_path_element_rel_curve_to_new()
{
    UxStylePathElement *element;
    element = (UxStylePathElement *) g_new(UxStylePathElementCurveTo, 1);
    element->type = UX_STYLE_PATH_COMMAND_TYPE_REL_CURVE_TO;
    element->length = 6;//UxStylePathPointLength[UX_STYLE_PATH_COMMAND_TYPE_REL_CURVE_TO];
    return element;
}
UxStylePathElement *
ux_style_path_element_rel_close_new()
{
    UxStylePathElement *element;
    element = (UxStylePathElement *) g_new(UxStylePathElement, 1);
    element->type = UX_STYLE_PATH_COMMAND_TYPE_REL_CLOSE;
    element->length = 0;//UxStylePathPointLength[UX_STYLE_PATH_COMMAND_TYPE_REL_CLOSE];
    return element;
}

UxStylePathElement *
ux_style_path_element_move_to_new()
{
    UxStylePathElement *element;
    element = (UxStylePathElement *) g_new(UxStylePathElementMoveTo, 1);
    element->type = UX_STYLE_PATH_COMMAND_TYPE_MOVE_TO;
    element->length = 2;//UxStylePathPointLength[UX_STYLE_PATH_COMMAND_TYPE_MOVE_TO];
    return element;
}
UxStylePathElement *
ux_style_path_element_line_to_new()
{
    UxStylePathElement *element;
    element = (UxStylePathElement *) g_new(UxStylePathElementLineTo, 1);
    element->type = UX_STYLE_PATH_COMMAND_TYPE_LINE_TO;
    element->length = 2;//UxStylePathPointLength[UX_STYLE_PATH_COMMAND_TYPE_LINE_TO];
    return element;
}
UxStylePathElement *
ux_style_path_element_curve_to_new()
{
    UxStylePathElement *element;
    element = (UxStylePathElement *) g_new(UxStylePathElementCurveTo, 1);
    element->type = UX_STYLE_PATH_COMMAND_TYPE_CURVE_TO;
    element->length = 6;//UxStylePathPointLength[UX_STYLE_PATH_COMMAND_TYPE_CURVE_TO];
    return element;
}
UxStylePathElement *
ux_style_path_element_close_new()
{
    UxStylePathElement *element;
    element = (UxStylePathElement *) g_new(UxStylePathElement, 1);
    element->type = UX_STYLE_PATH_COMMAND_TYPE_CLOSE;
    element->length = 0;//UxStylePathPointLength[UX_STYLE_PATH_COMMAND_TYPE_CLOSE];
    return element;
}

typedef UxStylePathElement* (*UxStylePathSegmentFactory)(void);

/**
 *
 * @brief ux_rc_parser_parse_path
 * @param path
 * @param ptr
 * @param end
 * @return
 */
gchar*
ux_parser_parse_path(UxStylePath *path, gchar *ptr, gchar *end)
{
    gchar *org = ptr;
    ux_parser_skip_space(ptr, end);

    UxStylePathSegmentFactory factory = NULL;
    //gboolean end_reached = FALSE;
    while (ptr<end)
    {
        UxStylePathElement *segment = NULL;

        if (*ptr=='m') {
            factory = &ux_style_path_element_rel_move_to_new;
        } else if (*ptr=='l') {
            factory = &ux_style_path_element_rel_line_to_new;
        } else if (*ptr=='c') {
            factory = &ux_style_path_element_rel_curve_to_new;
        } else if (*ptr=='z') {
            factory = &ux_style_path_element_rel_close_new;
        } else if (*ptr=='M') {
            factory = &ux_style_path_element_move_to_new;
        } else if (*ptr=='L') {
            factory = &ux_style_path_element_line_to_new;
        } else if (*ptr=='C') {
            factory = &ux_style_path_element_curve_to_new;
        } else if (*ptr=='Z') {
            factory = &ux_style_path_element_close_new;
        } else {
            // use previous command
            ptr--;
        }
        ptr++;

        segment = factory();

        ux_parser_skip_space(ptr, end);

        int i;
        int ln = segment->length;
        UxStyleLength length;
        for (i=0; i<ln; i++) {
            ptr = ux_rc_parser_parse_length(&length, ptr, end);// fixme parse_path_data
            segment->data[i].length = length;
            segment->data[i].hint = &ux_style_path_stroke_hint_none;

            ux_parser_skip_separator(ptr, end);
        }
        path->segments = g_list_append(path->segments, segment);

        //end_reached = TRUE;/// \fixme
    }

    return ptr;
}

