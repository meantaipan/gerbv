/*
 * gEDA - GNU Electronic Design Automation
 * This file is a part of gerbv.
 *
 *   Copyright (C) 2000-2002 Stefan Petersen (spe@stacken.kth.se)
 *
 * $Id$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA
 */

#ifndef GERBV_SCREEN_H
#define GERBV_SCREEN_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#define INITIAL_SCALE 200
#define MAX_FILES 20
#define MAX_ERRMSGLEN 45
#define MAX_COORDLEN 50
#define MAX_DISTLEN 75
#define MAX_STATUSMSGLEN (MAX_ERRMSGLEN+MAX_COORDLEN+MAX_DISTLEN)
#define GERBV_DISTFONTNAME "-*-helvetica-bold-r-normal--*-120-*-*-*-*-iso8859-1"
#define GERBV_STATUSFONTNAME "-*-fixed-*-*-normal--*-100-*-*-*-*-iso8859-1"

/* Macros to convert between unscaled gerber coordinates and other units */
/* XXX NOTE: Currently unscaled units are assumed as inch, this is not
   XXX necessarily true for all files */
#define COORD2MILS(c) ((c)*1000.0)
#define COORD2MMS(c) ((c)*25.4)

typedef enum {NORMAL, MOVE, ZOOM_OUTLINE, MEASURE, ALT_PRESSED} gerbv_state_t;

typedef struct {
    gerb_image_t *image;
    GdkColor *color;
    char *name;
} gerbv_fileinfo_t;


typedef struct {
    GtkWidget *drawing_area;
    GdkPixmap *pixmap;
    GdkColor  *background;
    GdkFunction si_func; /* Function used for superimposing layers */
    GdkColor  *zoom_outline_color;
    GdkColor  *dist_measure_color;
    
    GtkWidget *load_file_popup;
    GtkWidget *color_selection_popup;
    GtkWidget *export_png_popup;

    gerbv_fileinfo_t *file[MAX_FILES];
    int curr_index;
    char *path;
    struct {			/* Bounding box for all gerber images loaded */
	double x1, y1;		/* Initialized by autoscale() */
	double x2, y2;
    } gerber_bbox;

    GtkTooltips *tooltips;
    GtkWidget *layer_button[MAX_FILES];
    GtkWidget *popup_menu;
    struct {
	GtkWidget *msg;
	char msgstr[MAX_ERRMSGLEN];
	char coordstr[MAX_COORDLEN];
	char diststr[MAX_DISTLEN];
    } statusbar;

    gerbv_state_t state;
    gboolean centered_outline_zoom;

    int scale;

    int selected_layer;         /* Selected layer by Alt+keypad */

    gint last_x;
    gint last_y;
    gint start_x;		/* Zoom box/measure start coordinates */
    gint start_y;

    int trans_x; /* Translate offset */
    int trans_y;
} gerbv_screen_t;

extern gerbv_screen_t screen;

#endif /* GERBV_SCREEN_H */