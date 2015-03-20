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

#if !defined (__APPSTREAM_GLIB_H) && !defined (AS_COMPILATION)
#error "Only <appstream-glib.h> can be included directly."
#endif

#ifndef __AS_PIXBUF_H
#define __AS_PIXBUF_H

#include <glib-object.h>
#include <gio/gio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

G_BEGIN_DECLS

typedef GdkPixbuf AsPixbuf;

AsPixbuf	*as_pixbuf_new_from_stream	(GInputStream	*stream,
						 GCancellable	*cancellable,
						 GError		**error);
AsPixbuf	*as_pixbuf_new_from_file	(const gchar	*fn,
						 GError		**error);
gint		 as_pixbuf_get_width		(const AsPixbuf	*pixbuf);
gint		 as_pixbuf_get_height		(const AsPixbuf	*pixbuf);
gint		 as_pixbuf_get_n_channels	(const AsPixbuf	*pixbuf);
gint		 as_pixbuf_get_rowstride	(const AsPixbuf	*pixbuf);
gint		 as_pixbuf_get_colorspace	(const AsPixbuf	*pixbuf);
gint		 as_pixbuf_get_bits_per_sample	(const AsPixbuf	*pixbuf);
gboolean	 as_pixbuf_get_has_alpha	(const AsPixbuf	*pixbuf);
AsPixbuf	*as_pixbuf_copy			(const AsPixbuf	*pixbuf);
guchar		*as_pixbuf_get_pixels		(const AsPixbuf	*pixbuf,
						 guint		*length);
gboolean	 as_pixbuf_save			(AsPixbuf	*pixbuf,
						 const gchar	*fn,
						 const gchar	*type,
						 GError		**error);
AsPixbuf	*as_pixbuf_new_from_file_at_size (const gchar	*fn,
						 gint		 width,
						 gint		 height,
						 GError		**error);
void		 as_pixbuf_fill			(AsPixbuf	*pixbuf,
						 guint32	 pixel);
AsPixbuf	*as_pixbuf_new			(gboolean	 has_alpha,
						 gint		 bbs,
						 gint		 width,
						 gint		 height);
gboolean	 as_pixbuf_save_to_buffer	(AsPixbuf	*pixbuf,
						 gchar		**buf,
						 gsize		*buf_len,
						 const gchar	*type,
						 GError		**error);
AsPixbuf	*as_pixbuf_scale_simple		(const AsPixbuf	*src,
						 gint		 width,
						 gint		 height);
void		 as_pixbuf_copy_area		(const AsPixbuf *src,
						 gint		 src_x,
						 gint		 src_y,
						 gint		 width,
						 gint		 height,
						 AsPixbuf	*dest,
						 gint		 dest_x,
						 gint		 dest_y);

G_END_DECLS

#endif /* __AS_PIXBUF_H */
