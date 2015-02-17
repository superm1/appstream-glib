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

/**
 * SECTION:as-utils
 * @short_description: Helper functions that are used inside libappstream-glib
 * @include: appstream-glib.h
 * @stability: Stable
 *
 * These functions are used internally to libappstream-glib, and some may be
 * useful to user-applications.
 */

#include "config.h"

#include <archive_entry.h>
#include <archive.h>

#include "as-cleanup.h"
#include "as-store.h"
#include "as-utils-helper.h"

/**
 * as_utils_error_quark:
 *
 * Return value: An error quark.
 *
 * Since: 0.3.7
 **/
GQuark
as_utils_error_quark (void)
{
	static GQuark quark = 0;
	if (!quark)
		quark = g_quark_from_static_string ("AsUtilsError");
	return quark;
}

/**
 * as_utils_location_get_prefix:
 **/
static const gchar *
as_utils_location_get_prefix (AsUtilsLocation location)
{
	if (location == AS_UTILS_LOCATION_SHARED)
		return "/usr/share";
	if (location == AS_UTILS_LOCATION_CACHE)
		return "/var/cache";
	if (location == AS_UTILS_LOCATION_USER)
		return "~/.local/share";
	return NULL;
}

/**
 * as_utils_install_icon:
 **/
static gboolean
as_utils_install_icon (AsUtilsLocation location,
		       const gchar *filename,
		       const gchar *origin,
		       const gchar *destdir,
		       GError **error)
{
	const gchar *pathname;
	const gchar *tmp;
	gboolean ret = TRUE;
	gsize len;
	int r;
	struct archive *arch = NULL;
	struct archive_entry *entry;
	_cleanup_free_ gchar *data = NULL;
	_cleanup_free_ gchar *dir = NULL;

	dir = g_strdup_printf ("%s%s/app-info/icons/%s",
			       destdir,
			       as_utils_location_get_prefix (location),
			       origin);

	/* load file at once to avoid seeking */
	ret = g_file_get_contents (filename, &data, &len, error);
	if (!ret)
		goto out;

	/* read anything */
	arch = archive_read_new ();
	archive_read_support_format_all (arch);
	archive_read_support_filter_all (arch);
	r = archive_read_open_memory (arch, data, len);
	if (r) {
		ret = FALSE;
		g_set_error (error,
			     AS_UTILS_ERROR,
			     AS_UTILS_ERROR_FAILED,
			     "Cannot open: %s",
			     archive_error_string (arch));
		goto out;
	}

	/* decompress each file */
	for (;;) {
		_cleanup_free_ gchar *buf = NULL;

		r = archive_read_next_header (arch, &entry);
		if (r == ARCHIVE_EOF)
			break;
		if (r != ARCHIVE_OK) {
			ret = FALSE;
			g_set_error (error,
				     AS_UTILS_ERROR,
				     AS_UTILS_ERROR_FAILED,
				     "Cannot read header: %s",
				     archive_error_string (arch));
			goto out;
		}

		/* no output file */
		pathname = archive_entry_pathname (entry);
		if (pathname == NULL)
			continue;

		/* update output path */
		buf = g_build_filename (dir, pathname, NULL);
		archive_entry_update_pathname_utf8 (entry, buf);

		/* update hardlinks */
		tmp = archive_entry_hardlink (entry);
		if (tmp != NULL) {
			_cleanup_free_ gchar *buf_link = NULL;
			buf_link = g_build_filename (dir, tmp, NULL);
			archive_entry_update_hardlink_utf8 (entry, buf_link);
		}

		/* update symlinks */
		tmp = archive_entry_symlink (entry);
		if (tmp != NULL) {
			_cleanup_free_ gchar *buf_link = NULL;
			buf_link = g_build_filename (dir, tmp, NULL);
			archive_entry_update_symlink_utf8 (entry, buf_link);
		}

		r = archive_read_extract (arch, entry, 0);
		if (r != ARCHIVE_OK) {
			ret = FALSE;
			g_set_error (error,
				     AS_UTILS_ERROR,
				     AS_UTILS_ERROR_FAILED,
				     "Cannot extract: %s",
				     archive_error_string (arch));
			goto out;
		}
	}
out:
	if (arch != NULL) {
		archive_read_close (arch);
		archive_read_free (arch);
	}
	return ret;
}

/**
 * as_utils_install_xml:
 **/
static gboolean
as_utils_install_xml (const gchar *filename,
		      const gchar *origin,
		      const gchar *dir,
		      const gchar *destdir,
		      GError **error)
{
	gchar *tmp;
	_cleanup_free_ gchar *basename = NULL;
	_cleanup_free_ gchar *path_dest = NULL;
	_cleanup_free_ gchar *path_parent = NULL;
	_cleanup_object_unref_ GFile *file_dest = NULL;
	_cleanup_object_unref_ GFile *file_src = NULL;

	/* create directory structure */
	path_parent = g_strdup_printf ("%s%s", destdir, dir);
	if (g_mkdir_with_parents (path_parent, 0777) != 0) {
		g_set_error (error,
			     AS_UTILS_ERROR,
			     AS_UTILS_ERROR_FAILED,
			     "Failed to create %s", path_parent);
		return FALSE;
	}

	/* calculate the new destination */
	file_src = g_file_new_for_path (filename);
	basename = g_path_get_basename (filename);
	if (origin != NULL) {
		_cleanup_free_ gchar *basename_new = NULL;
		tmp = g_strstr_len (basename, -1, ".");
		if (tmp == NULL) {
			g_set_error (error,
				     AS_UTILS_ERROR,
				     AS_UTILS_ERROR_FAILED,
				     "Name of XML file invalid %s",
				     basename);
			return FALSE;
		}
		basename_new = g_strdup_printf ("%s%s", origin, tmp);
		/* replace the fedora.xml.gz into %{origin}.xml.gz */
		path_dest = g_build_filename (path_parent, basename_new, NULL);
	} else {
		path_dest = g_build_filename (path_parent, basename, NULL);
	}

	/* actually copy file */
	file_dest = g_file_new_for_path (path_dest);
	if (!g_file_copy (file_src, file_dest,
			  G_FILE_COPY_OVERWRITE |
			  G_FILE_COPY_TARGET_DEFAULT_PERMS,
			  NULL, NULL, NULL, error))
		return FALSE;

	/* fix the origin */
	if (origin != NULL) {
		_cleanup_object_unref_ AsStore *store = NULL;
		store = as_store_new ();
		if (!as_store_from_file (store, file_dest, NULL, NULL, error))
			return FALSE;
		as_store_set_origin (store, origin);
		if (!as_store_to_file (store, file_dest,
				       AS_NODE_TO_XML_FLAG_ADD_HEADER |
				       AS_NODE_TO_XML_FLAG_FORMAT_MULTILINE,
				       NULL, error))
			return FALSE;
	}
	return TRUE;
}

/**
 * as_utils_install_filename:
 * @location: the #AsUtilsLocation, e.g. %AS_UTILS_LOCATION_CACHE
 * @filename: the full path of the file to install
 * @origin: the origin to use for the installation, or %NULL
 * @destdir: the destdir to use, or %NULL
 * @error: A #GError or %NULL
 *
 * Installs an AppData, MetaInfo, AppStream XML or AppStream Icon metadata file.
 *
 * Returns: %TRUE for success, %FALSE if error is set
 *
 * Since: 0.3.4
 **/
gboolean
as_utils_install_filename (AsUtilsLocation location,
			  const gchar *filename,
			  const gchar *origin,
			  const gchar *destdir,
			  GError **error)
{
	gboolean ret = FALSE;
	gchar *tmp;
	_cleanup_free_ gchar *basename = NULL;
	_cleanup_free_ gchar *path = NULL;

	/* default value */
	if (destdir == NULL)
		destdir = "";

	switch (as_app_guess_source_kind (filename)) {
	case AS_APP_SOURCE_KIND_APPSTREAM:
		if (g_strstr_len (filename, -1, ".yml.gz") != NULL) {
			path = g_build_filename (as_utils_location_get_prefix (location),
						 "app-info", "yaml", NULL);
			ret = as_utils_install_xml (filename, origin, path, destdir, error);
		} else {
			path = g_build_filename (as_utils_location_get_prefix (location),
						 "app-info", "xmls", NULL);
			ret = as_utils_install_xml (filename, origin, path, destdir, error);
		}
		break;
	case AS_APP_SOURCE_KIND_APPDATA:
	case AS_APP_SOURCE_KIND_METAINFO:
		if (location == AS_UTILS_LOCATION_CACHE) {
			g_set_error_literal (error,
					     AS_UTILS_ERROR,
					     AS_UTILS_ERROR_INVALID_TYPE,
					     "cached location unsupported for "
					     "MetaInfo and AppData files");
			return FALSE;
		}
		path = g_build_filename (as_utils_location_get_prefix (location),
					 "appdata", NULL);
		ret = as_utils_install_xml (filename, NULL, path, destdir, error);
		break;
	default:
		/* icons */
		if (origin != NULL) {
			ret = as_utils_install_icon (location, filename, origin, destdir, error);
			break;
		}
		basename = g_path_get_basename (filename);
		tmp = g_strstr_len (basename, -1, "-icons.tar.gz");
		if (tmp != NULL) {
			*tmp = '\0';
			ret = as_utils_install_icon (location, filename, basename, destdir, error);
			break;
		}

		/* unrecognised */
		g_set_error_literal (error,
				     AS_UTILS_ERROR,
				     AS_UTILS_ERROR_INVALID_TYPE,
				     "No idea how to process files of this type");
		break;
	}
	return ret;
}
