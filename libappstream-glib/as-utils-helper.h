/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2014-2015 Richard Hughes <richard@hughsie.com>
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

#ifndef __AS_UTILS_HELPER_H
#define __AS_UTILS_HELPER_H

#include <glib.h>

G_BEGIN_DECLS

/**
 * AsUtilsError:
 * @AS_UTILS_ERROR_FAILED:			Generic failure
 * @AS_UTILS_ERROR_INVALID_TYPE:		Invalid type
 *
 * The error type.
 **/
typedef enum {
	AS_UTILS_ERROR_FAILED,
	AS_UTILS_ERROR_INVALID_TYPE,
	/*< private >*/
	AS_UTILS_ERROR_LAST
} AsUtilsError;

#define	AS_UTILS_ERROR				as_utils_error_quark ()

/**
 * AsUtilsLocation:
 * @AS_UTILS_LOCATION_SHARED:			Installed by the vendor, shared
 * @AS_UTILS_LOCATION_CACHE:			Installed as metadata, shared
 * @AS_UTILS_LOCATION_USER:			Installed by the user
 *
 * The flags used when installing and removing metadata files.
 **/
typedef enum {
	AS_UTILS_LOCATION_SHARED,
	AS_UTILS_LOCATION_CACHE,
	AS_UTILS_LOCATION_USER,
	/*< private >*/
	AS_UTILS_LOCATION_LAST
} AsUtilsLocation;

GQuark		 as_utils_error_quark		(void);
gboolean	 as_utils_install_filename	(AsUtilsLocation location,
						 const gchar	*filename,
						 const gchar	*origin,
						 const gchar	*destdir,
						 GError		**error);

G_END_DECLS

#endif /* __AS_UTILS_HELPER_H */
