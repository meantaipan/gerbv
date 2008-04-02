/*
 * gEDA - GNU Electronic Design Automation
 * This file is a part of gerbv.
 *
 *   Copyright (C) 2008 Julian Lamb
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glib.h>
#include <math.h>

#include <glib/gstdio.h>
#include "gerber.h"
#include "gerb_image.h"
#include "export-drill.h"

/* DEBUG printing.  #define DEBUG 1 in config.h to use this fcn. */
#define dprintf if(DEBUG) printf
#define round(x) floor(x+0.5)

gboolean
export_drill_file_from_image (gchar *filename, gerb_image_t *image) {
	FILE *fd;
	GArray *apertureTable = g_array_new(FALSE,FALSE,sizeof(int));
	gerb_net_t *currentNet;
	
	if ((fd = g_fopen(filename, "w")) == NULL) {
		GERB_MESSAGE("Can't open file for writing: %s\n", filename);
		return FALSE;
	}
	/* write header info */
	fprintf(fd, "M48\n");
	fprintf(fd, "INCH,TZ\n");

	/* define all apertures */
	gerb_aperture_t *currentAperture;
	gint i;
		
	for (i=APERTURE_MIN; i<APERTURE_MAX; i++) {
		currentAperture = image->aperture[i];
		
		if (!currentAperture)
			continue;
		
		switch (currentAperture->type) {
			case CIRCLE:
				fprintf(fd, "T%dC%1.3f\n",i,currentAperture->parameter[0]);
				/* add the "approved" aperture to our valid list */
	  			g_array_append_val (apertureTable, i);
				break;  
			default:
				break;
		}
	}
	
	fprintf(fd, "%%\n");
	/* write rest of image */
	
	for (i=0; i<apertureTable->len; i++) {
		int currentAperture=g_array_index (apertureTable, int, i);
		
		/* write tool change */
		fprintf(fd, "T%d\n",currentAperture);
		
		/* run through all nets and look for drills using this aperture */
		for (currentNet = image->netlist; currentNet; currentNet = currentNet->next){
			if (currentNet->aperture == currentAperture) {
				long xVal,yVal;
				xVal = (long) round(currentNet->stop_x * 10000.0);
				yVal = (long) round(currentNet->stop_y * 10000.0);
				fprintf(fd, "X%06ldY%06ld\n",xVal,yVal);
			}
		}
	}
	g_array_free (apertureTable, TRUE);
	/* write footer */
	fprintf(fd, "M30\n\n");

	fclose(fd);
	return TRUE;
}
