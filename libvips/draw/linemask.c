/* draw a mask along a line
 *
 * Copyright: J. Cupitt
 * Written: 15/06/1992
 * Modified : 22/10/92 - clipping constraints changed
 * 22/7/93 JC
 *	- im_incheck() added
 * 16/8/94 JC
 *	- im_incheck() changed to im_makerw()
 * 5/12/06
 * 	- im_invalidate() after paint
 * 1/3/10
 * 	- oops, lineset needs to ask for WIO of mask and ink
 * 6/3/10
 * 	- don't im_invalidate() after paint, this now needs to be at a higher
 * 	  level
 * 27/9/10
 * 	- gtk-doc
 * 	- use draw.c base class
 * 	- do pointwise clipping
 * 	- rename as im_draw_line() for consistency
 * 	- cleanups!
 * 6/2/14
 * 	- redo as a class
 */

/*

    This file is part of VIPS.
    
    VIPS is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA

 */

/*

    These files are distributed with VIPS - http://www.vips.ecs.soton.ac.uk

 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /*HAVE_CONFIG_H*/
#include <vips/intl.h>

#include <stdio.h>
#include <stdlib.h>

#include <vips/vips.h>

#include "pdraw.h"
#include "line.h"

typedef struct _VipsLineMask {
	VipsLine parent_object;

} VipsLineMask;

typedef VipsLineClass VipsLineMaskClass;

G_DEFINE_TYPE( VipsLineMask, vips_line_mask, VIPS_TYPE_LINE );

static int
vips_line_mask_build( VipsObject *object )
{
	VipsDraw *draw = VIPS_DRAW( object );
	VipsLine *line = (VipsLine *) object;

	if( VIPS_OBJECT_CLASS( vips_line_mask_parent_class )->build( object ) )
		return( -1 );

	return( 0 );
}

static int
vips_line_mask_plot_point( VipsLine *line, int x, int y ) 
{
	VipsDraw *draw = (VipsDraw *) line;

	if( draw->noclip )
		vips__draw_pel( draw, VIPS_IMAGE_ADDR( draw->image, x, y ) );
	else
		vips__draw_pel_clip( draw, x, y );

	return( 0 );
}

static void
vips_line_mask_class_init( VipsLineClass *class )
{
	GObjectClass *gobject_class = G_OBJECT_CLASS( class );
	VipsObjectClass *vobject_class = VIPS_OBJECT_CLASS( class );

	gobject_class->set_property = vips_object_set_property;
	gobject_class->get_property = vips_object_get_property;

	vobject_class->nickname = "line_mask";
	vobject_class->description = _( "draw a line_mask on an image" );
	vobject_class->build = vips_line_mask_build;

	class->plot_point = vips_line_mask_plot_point; 

	VIPS_ARG_IMAGE( class, "y2", 6, 
		_( "y2" ), 
		_( "End of line" ),
		VIPS_ARGUMENT_REQUIRED_INPUT,
		G_STRUCT_OFFSET( VipsLine, y2 ),
		-1000000000, 1000000000, 0 );

}

static void
vips_line_mask_init( VipsLine *line )
{
}

static int
vips_line_maskv( VipsImage *image, 
	double *ink, int n, int x1, int y1, int x2, int y2, va_list ap )
{
	VipsArea *area_ink;
	int result;

	area_ink = (VipsArea *) vips_array_double_new( ink, n );
	result = vips_call_split( "line_mask", ap, 
		image, area_ink, x1, y1, x2, y2 );
	vips_area_unref( area_ink );

	return( result );
}

/**
 * vips_line_mask:
 * @image: image to draw on
 * @ink: (array length=n): value to draw
 * @n length of ink array
 *
 * Draws a 1-pixel-wide line on an image. Subclass and override ::plot to draw
 * lines made of other objects. See vips_draw_mask(). 
 *
 * @ink is an array of double containing values to draw. 
 *
 * See also: vips_line_mask1(), vips_circle(), vips_draw_mask(). 
 *
 * Returns: 0 on success, or -1 on error.
 */
int
vips_line_mask( VipsImage *image, 
	double *ink, int n, int x1, int y1, int x2, int y2, ... )
{
	va_list ap;
	int result;

	va_start( ap, y2 );
	result = vips_line_maskv( image, ink, n, x1, y1, x2, y2, ap );
	va_end( ap );

	return( result );
}

/**
 * vips_line_mask1:
 * @image: image to draw on
 * @ink: value to draw
 *
 * As vips_line_mask(), but just takes a single double for @ink. 
 *
 * See also: vips_line_mask(), vips_circle().
 *
 * Returns: 0 on success, or -1 on error.
 */
int
vips_line_mask1( VipsImage *image, double ink, int x1, int y1, int x2, int y2, ... )
{
	double array_ink[1];
	va_list ap;
	int result;

	array_ink[0] = ink; 

	va_start( ap, y2 );
	result = vips_line_maskv( image, array_ink, 1, x1, y1, x2, y2, ap );
	va_end( ap );

	return( result );
}