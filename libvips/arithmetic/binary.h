/* base class for all binary operations
 */

/*

    Copyright (C) 1991-2005 The National Gallery

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 */

/*

    These files are distributed with VIPS - http://www.vips.ecs.soton.ac.uk

 */

#ifndef VIPS_BINARY_H
#define VIPS_BINARY_H

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

#define VIPS_TYPE_BINARY (vips_binary_get_type())
#define VIPS_BINARY( obj ) \
	(G_TYPE_CHECK_INSTANCE_CAST( (obj), VIPS_TYPE_BINARY, VipsBinary ))
#define VIPS_BINARY_CLASS( klass ) \
	(G_TYPE_CHECK_CLASS_CAST( (klass), VIPS_TYPE_BINARY, VipsBinaryClass))
#define VIPS_IS_BINARY( obj ) \
	(G_TYPE_CHECK_INSTANCE_TYPE( (obj), VIPS_TYPE_BINARY ))
#define VIPS_IS_BINARY_CLASS( klass ) \
	(G_TYPE_CHECK_CLASS_TYPE( (klass), VIPS_TYPE_BINARY ))
#define VIPS_BINARY_GET_CLASS( obj ) \
	(G_TYPE_INSTANCE_GET_CLASS( (obj), VIPS_TYPE_BINARY, VipsBinaryClass ))

typedef void (*VipsBinaryProcessFn)( VipsBinary *binary, 
	void *out, void *left, void *right, int width );

typedef struct _VipsBinary {
	VipsArithmetic parent_instance;

	/* Original left and right image args.
	 */
	VipsImage *left;
	VipsImage *right;

	/* Some intermediates.
	 */
	VipsImage *t[5];

	/* Processed images ready for the line function.
	 */
	VipsImage *left_processed;
	VipsImage *right_processed;

	/* The line processor, plus some client data.
	 */
	VipsBinaryProcessFn process_line;
	void *a;
	void *b;
} VipsBinary;

typedef struct _VipsBinaryClass {
	VipsArithmeticClass parent_class;

} VipsBinaryClass;

GType vips_binary_get_type( void );

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*VIPS_BINARY_H*/


