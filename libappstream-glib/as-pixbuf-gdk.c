/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2015 Richard Hughes <richard@hughsie.com>
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

/**
 * SECTION:as-pixbufs
 * @short_description: Helper functions that are used for images
 * @include: appstream-glib.h
 * @stability: Stable
 *
 * These functions are used to avoid using GdPixbuf symbols internally.
 */

#include "as-pixbuf.h"

/**
 * as_pixbuf_new_from_file:
 **/
AsPixbuf *
as_pixbuf_new_from_file (const gchar *fn, GError **error)
{
	return gdk_pixbuf_new_from_file (fn, error);
}

/**
 * as_pixbuf_new_from_stream:
 **/
AsPixbuf *
as_pixbuf_new_from_stream (GInputStream *stream, GCancellable *cancellable, GError **error)
{
	return gdk_pixbuf_new_from_stream (stream, cancellable, error);
}

/**
 * as_pixbuf_copy:
 **/
AsPixbuf *
as_pixbuf_copy (const AsPixbuf *pixbuf)
{
	return gdk_pixbuf_copy (pixbuf);
}

/**
 * as_pixbuf_get_width:
 **/
gint
as_pixbuf_get_width (const AsPixbuf *pixbuf)
{
	return gdk_pixbuf_get_width (pixbuf);
}

/**
 * as_pixbuf_get_height:
 **/
gint
as_pixbuf_get_height (const AsPixbuf *pixbuf)
{
	return gdk_pixbuf_get_height (pixbuf);
}

/**
 * as_pixbuf_get_n_channels:
 **/
gint
as_pixbuf_get_n_channels (const AsPixbuf *pixbuf)
{
	return gdk_pixbuf_get_n_channels (pixbuf);
}

/**
 * as_pixbuf_get_rowstride:
 **/
gint
as_pixbuf_get_rowstride (const AsPixbuf *pixbuf)
{
	return gdk_pixbuf_get_rowstride (pixbuf);
}

/**
 * as_pixbuf_get_colorspace:
 **/
gint
as_pixbuf_get_colorspace (const AsPixbuf *pixbuf)
{
	return gdk_pixbuf_get_colorspace (pixbuf);
}

/**
 * as_pixbuf_get_bits_per_sample:
 **/
gint
as_pixbuf_get_bits_per_sample (const AsPixbuf *pixbuf)
{
	return gdk_pixbuf_get_bits_per_sample (pixbuf);
}

/**
 * as_pixbuf_get_has_alpha:
 **/
gboolean
as_pixbuf_get_has_alpha (const AsPixbuf *pixbuf)
{
	return gdk_pixbuf_get_has_alpha (pixbuf);
}

/**
 * as_pixbuf_get_pixels:
 **/
guchar *
as_pixbuf_get_pixels (const AsPixbuf *pixbuf, guint *length)
{
	return gdk_pixbuf_get_pixels_with_length (pixbuf, length);
}

/**
 * as_pixbuf_save:
 **/
gboolean
as_pixbuf_save (AsPixbuf *pixbuf, const gchar *fn, const gchar *type, GError **error)
{
	return gdk_pixbuf_save (pixbuf, fn, type, error, NULL);
}

/**
 * as_pixbuf_new_from_file_at_size:
 **/
AsPixbuf *
as_pixbuf_new_from_file_at_size (const gchar *fn, gint width, gint height, GError **error)
{
	return gdk_pixbuf_new_from_file_at_size (fn, width, height, error);
}

/**
 * as_pixbuf_fill:
 **/
void
as_pixbuf_fill (AsPixbuf *pixbuf, guint32 pixel)
{
	return gdk_pixbuf_fill (pixbuf, pixel);
}

/**
 * as_pixbuf_new:
 **/
AsPixbuf *
as_pixbuf_new (gboolean has_alpha, gint bbs, gint width, gint height)
{
	return gdk_pixbuf_new (GDK_COLORSPACE_RGB, has_alpha, bbs, width, height);
}

/**
 * as_pixbuf_save_to_buffer:
 **/
gboolean
as_pixbuf_save_to_buffer (AsPixbuf *pixbuf, gchar **buf, gsize *buf_len,
			  const gchar *type, GError **error)
{
	return gdk_pixbuf_save_to_buffer (pixbuf, buf, buf_len, type,
					  error, NULL);
}

/**
 * as_pixbuf_scale_simple:
 **/
AsPixbuf *
as_pixbuf_scale_simple (const AsPixbuf *src, gint width, gint height)
{
	return gdk_pixbuf_scale_simple (src, width, height, GDK_INTERP_HYPER);
}

/**
 * as_pixbuf_copy_area:
 **/
void
as_pixbuf_copy_area (const AsPixbuf *src, gint src_x, gint src_y,
		     gint width, gint height,
		     AsPixbuf *dest, gint dest_x, gint dest_y)
{
	gdk_pixbuf_copy_area (src, src_x, src_y, width, height, dest, dest_x, dest_y);
}
