/**
 ** This file is part of the atkv project.
 ** Copyright 2016 Anderson Tavares <nocturne.pe@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef AT_IMGPROC_ZNZFILE_H_
#define AT_IMGPROC_ZNZFILE_H_

#include <at/core.h>

#ifdef HAVE_ZLIB
#include "zlib.h"
#endif

G_BEGIN_DECLS

/**
 * This library provides an interface to both compressed (gzip/zlib) and
 * uncompressed (normal) file IO.  The functions are written to have the
 * same interface as the standard file IO functions.
 *
 * To use this library instead of normal file IO, the following changes
 * are required:
 *  - replace all instances of FILE* with znzFile
 *  - change the name of all function calls, replacing the initial character
 *    f with the znz  (e.g. fseek becomes znzseek)
 *  - add a third parameter to all calls to znzopen (previously fopen)
 *    that specifies whether to use compression (1) or not (0)
 *  - use znz_isnull rather than any (pointer == NULL) comparisons in the code
 * NB: seeks for writable files with compression are quite restricted
 * include optional check for HAVE_FDOPEN here, from deleted config.h:
 * uncomment the following line if fdopen() exists for your compiler and
 * compiler options
 * #define HAVE_FDOPEN
 */
 
/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define AT_TYPE_ZNZFILE at_znzfile_get_type()
G_DECLARE_DERIVABLE_TYPE(AtZnzFile, at_znzfile, AT, ZNZFILE, GObject)
typedef struct _AtZnzFileClass{
  GObjectClass parent_class;
}AtZnzFileClass;

/*===========================================================================
 * MEMBERS
 *===========================================================================*/
/**
 * @brief Open a zlib file (with or without compression)
 * @param path
 * @param mode
 * @param use_compression TRUE => with gzip compression
 * @return AtZnzFile representing a zlib object
 */
AtZnzFile*
at_znzfile_open(const char *path, const char *mode, gboolean use_compression);
/**
 * @brief Open a zlib file by id (with or without compression)
 * @param fd
 * @param mode
 * @param use_compression
 * @return
 */
AtZnzFile*
at_znzfile_dopen(int fd, const char *mode, gboolean use_compression);
/**
 * @brief at_znzfile_read_header
 * @param znzfile
 * @param buf
 * @param size
 * @param nmemb
 * @return
 */
size_t
at_znzfile_read_header(AtZnzFile* znzfile, void* buf, size_t size, size_t nmemb);
/**
 * @brief at_znzfile_rewind
 * @param znzfile
 * @return
 */
int
at_znzfile_rewind(AtZnzFile* znzfile);
/**
 * @brief at_znzfile_seek
 * @param file
 * @param offset
 * @param whence
 * @return
 */
long
at_znzfile_seek(AtZnzFile* file, long offset, int whence);
/**
 * @brief at_znzfile_tell
 * @param file
 * @return
 */
long
at_znzfile_tell(AtZnzFile* file);
/**
 * @brief at_znzread
 * @param file
 * @param buf
 * @param size
 * @param nmemb
 * @return
 */
size_t
at_znzfile_read(AtZnzFile* file, void* buf, size_t size, size_t nmemb);

G_END_DECLS
#endif
