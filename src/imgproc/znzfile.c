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

#include <at/imgproc.h>
#include <stdio.h>

/**
 * @file  znzfile.c
 * @brief Low level i/o interface to compressed and noncompressed files.
 *
 * This library provides an interface to both compressed (gzip/zlib) and
 * uncompressed (normal) file IO.  The functions are written to have the
 * same interface as the standard file IO functions.
 *
 * To use this library instead of normal file IO, the following changes
 * are required:
 *  - replace all instances of FILE* with znzFile
 *  - change the name of all function calls, replacing the initial character
 *    f with the znz  (e.g. fseek becomes znzseek)
 *    one exception is rewind() -> znzrewind()
 *  - add a third parameter to all calls to znzopen (previously fopen)
 *    that specifies whether to use compression (1) or not (0)
 *  - use znz_isnull rather than any (pointer == NULL) comparisons in the code
 *    for znzfile types (normally done after a return from znzopen)
 *
 * NB: seeks for writable files with compression are quite restricted
 */
 /*===========================================================================
 * PRIVATE API
 *===========================================================================*/
 typedef struct AtZnzFilePrivate{
  gboolean withz;/**< TRUE = compressed file */
  FILE* nzfptr;  /**< pointer to file */
#ifdef HAVE_ZLIB
  gzFile zfptr;  /**< pointer to zlib file */
#endif
}AtZnzFilePrivate;
G_DEFINE_TYPE_WITH_PRIVATE(AtZnzFile, at_znzfile, G_TYPE_OBJECT)

/**
 * @brief Close a ZLib file
 * @param znzfile
 * @return
 */
static int
at_znzfile_close(AtZnzFile* znzfile){
  int retval;
  AtZnzFilePrivate *priv = at_znzfile_get_instance_private(znzfile);
#ifdef HAVE_ZLIB
  if(priv->zfptr != NULL) {retval = gzclose(priv->zfptr);}
#endif
  if(priv->nzfptr != NULL){retval = fclose(priv->nzfptr);}
  return retval;
}

/**
 * @brief Dispose referenced objects of the zlib file
 * @param object
 */
static void
at_znzfile_dispose(GObject* object){
  G_OBJECT_CLASS(at_znzfile_parent_class)->dispose(object);
}
/**
 * @brief at_znzfile_finalize
 * @param object
 */
static void
at_znzfile_finalize(GObject* object){
  AtZnzFile        *znzfile = AT_ZNZFILE(object);

  at_znzfile_close(znzfile);

  G_OBJECT_CLASS(at_znzfile_parent_class)->finalize(object);
}

/**
 * @brief at_znzfile_init
 * @param self
 */
static void
at_znzfile_init(AtZnzFile *self){
  AtZnzFilePrivate *priv    = at_znzfile_get_instance_private(self);
  priv->nzfptr = NULL;
  priv->withz  = FALSE;
#ifdef HAVE_ZLIB
  priv->zfptr  = NULL;
#endif
}

/**
 * @brief at_znzfile_class_init
 * @param klass
 */
static void
at_znzfile_class_init(AtZnzFileClass *klass){
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = at_znzfile_finalize;
  object_class->dispose = at_znzfile_dispose;
}

/**
 * @brief Return a new instance of AtZnzFile
 * @return
 */
static AtZnzFile*
at_znzfile_new(){
  return g_object_new(AT_TYPE_ZNZFILE, NULL);
}

/* we already assume ints are 4 bytes */
#undef  ZNZ_MAX_BLOCK_SIZE
#define ZNZ_MAX_BLOCK_SIZE (1<<30)

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtZnzFile*
at_znzfile_open(const char *path, const char *mode, gboolean use_compression){
  AtZnzFile* znzfile     = at_znzfile_new();
  AtZnzFilePrivate *priv = at_znzfile_get_instance_private(znzfile);
#ifdef HAVE_ZLIB
  if(use_compression){
    priv->withz = TRUE;
    priv->zfptr = gzopen(path,mode);
    if(priv->zfptr == NULL){
      g_clear_object(&znzfile);
      return NULL;
    }
  } else {
#endif
    priv->withz = FALSE;
    priv->nzfptr = fopen(path,mode);
    if(priv->nzfptr == NULL){
      g_clear_object(&znzfile);
      return NULL;
    }
#ifdef HAVE_ZLIB
  }
#endif
  return znzfile;
}

AtZnzFile*
at_znzfile_dopen(int fd, const char *mode, gboolean use_compression){
  AtZnzFile* znzfile     = at_znzfile_new();
  AtZnzFilePrivate *priv = at_znzfile_get_instance_private(znzfile);
#ifdef HAVE_ZLIB
  if(use_compression){
    priv->withz = TRUE;
    priv->zfptr = gzdopen(fd,mode);
  } else {
#endif
    priv->withz = FALSE;
#ifndef HAVE_FDOPEN
    priv->nzfptr = fdopen(fd, mode);
#endif
#ifdef HAVE_ZLIB
  }
#endif
  return znzfile;
}

size_t
at_znzfile_read_header(AtZnzFile* znzfile, void* buf, size_t size, size_t nmemb){

  size_t     remain = size*nmemb;
  char     * cbuf = (char *)buf;
  unsigned   n2read;
  int        nread;

  if (znzfile == NULL) { return 0; }
  AtZnzFilePrivate* priv = at_znzfile_get_instance_private(znzfile);
#ifdef HAVE_ZLIB
  if (priv->zfptr!=NULL) {
    /* gzread/write take unsigned int length, so maybe read in int pieces
       (noted by M Hanke, example given by M Adler)   6 July 2010 [rickr] */
    while( remain > 0 ) {
       n2read = (remain < ZNZ_MAX_BLOCK_SIZE) ? remain : ZNZ_MAX_BLOCK_SIZE;
       nread = gzread(priv->zfptr, (void *)cbuf, n2read);
       if( nread < 0 ) return nread; /* returns -1 on error */

       remain -= nread;
       cbuf += nread;

       /* require reading n2read bytes, so we don't get stuck */
       if( nread < (int)n2read ) break;  /* return will be short */
    }

    /* warn of a short read that will seem complete */
    if( remain > 0 && remain < size )
       fprintf(stderr,"** znzread: read short by %u bytes\n",(unsigned)remain);

    return nmemb - remain/size;   /* return number of members processed */
  }
#endif
  return fread(buf,size,nmemb,priv->nzfptr);
}

int
at_znzfile_rewind(AtZnzFile* znzfile)
{
  if (znzfile==NULL) { return 0; }
  AtZnzFilePrivate* priv = at_znzfile_get_instance_private(znzfile);
#ifdef HAVE_ZLIB
  /* On some systems, gzrewind() fails for uncompressed files.
     Use gzseek(), instead.               10, May 2005 [rickr]

     if (stream->zfptr!=NULL) return gzrewind(stream->zfptr);
  */

  if (priv->zfptr!=NULL) return (int)gzseek(priv->zfptr, 0L, SEEK_SET);
#endif
  rewind(priv->nzfptr);
  return 0;
}

long
at_znzfile_seek(AtZnzFile* znzfile, long offset, int whence)
{
  if (znzfile == NULL) { return 0; }
  AtZnzFilePrivate* priv = at_znzfile_get_instance_private(znzfile);
#ifdef HAVE_ZLIB
  if (priv->zfptr!=NULL) return (long) gzseek(priv->zfptr,offset,whence);
#endif
  return fseek(priv->nzfptr,offset,whence);
}

long
at_znzfile_tell(AtZnzFile* znzfile)
{
  if (znzfile==NULL) { return 0; }
  AtZnzFilePrivate* priv = at_znzfile_get_instance_private(znzfile);
#ifdef HAVE_ZLIB
  if (priv->zfptr!=NULL) return (long) gztell(priv->zfptr);
#endif
  return ftell(priv->nzfptr);
}

size_t
at_znzfile_read(AtZnzFile* znzfile, void* buf, size_t size, size_t nmemb)
{
  size_t     remain = size*nmemb;
  char     * cbuf = (char *)buf;
  unsigned   n2read;
  int        nread;

  if (znzfile==NULL) { return 0; }
  AtZnzFilePrivate* priv = at_znzfile_get_instance_private(znzfile);
#ifdef HAVE_ZLIB
  if (priv->zfptr!=NULL) {
    /* gzread/write take unsigned int length, so maybe read in int pieces
       (noted by M Hanke, example given by M Adler)   6 July 2010 [rickr] */
    while( remain > 0 ) {
       n2read = (remain < ZNZ_MAX_BLOCK_SIZE) ? remain : ZNZ_MAX_BLOCK_SIZE;
       nread = gzread(priv->zfptr, (void *)cbuf, n2read);
       if( nread < 0 ) return nread; /* returns -1 on error */

       remain -= nread;
       cbuf += nread;

       /* require reading n2read bytes, so we don't get stuck */
       if( nread < (int)n2read ) break;  /* return will be short */
    }

    /* warn of a short read that will seem complete */
    if( remain > 0 && remain < size )
       fprintf(stderr,"** znzread: read short by %u bytes\n",(unsigned)remain);

    return nmemb - remain/size;   /* return number of members processed */
  }
#endif
  return fread(buf,size,nmemb,priv->nzfptr);
}
