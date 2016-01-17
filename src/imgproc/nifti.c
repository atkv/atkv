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

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
/*! \struct nifti_1_header
    \brief Data structure defining the fields in the nifti1 header.
           This binary header should be found at the beginning of a valid
           NIFTI-1 header file.
 */
                        /*************************/  /************************/
struct AtNifti1Header { /* NIFTI-1 usage         */  /* ANALYZE 7.5 field(s) */
                        /*************************/  /************************/

                                           /*--- was header_key substruct ---*/
 int   sizeof_hdr;    /*!< MUST be 348           */  /* int sizeof_hdr;      */
 char  data_type[10]; /*!< ++UNUSED++            */  /* char data_type[10];  */
 char  db_name[18];   /*!< ++UNUSED++            */  /* char db_name[18];    */
 int   extents;       /*!< ++UNUSED++            */  /* int extents;         */
 short session_error; /*!< ++UNUSED++            */  /* short session_error; */
 char  regular;       /*!< ++UNUSED++            */  /* char regular;        */
 char  dim_info;      /*!< MRI slice ordering.   */  /* char hkey_un0;       */

                                      /*--- was image_dimension substruct ---*/
 short dim[8];        /*!< Data array dimensions.*/  /* short dim[8];        */
 float intent_p1 ;    /*!< 1st intent parameter. */  /* short unused8;       */
                                                     /* short unused9;       */
 float intent_p2 ;    /*!< 2nd intent parameter. */  /* short unused10;      */
                                                     /* short unused11;      */
 float intent_p3 ;    /*!< 3rd intent parameter. */  /* short unused12;      */
                                                     /* short unused13;      */
 short intent_code ;  /*!< NIFTI_INTENT_* code.  */  /* short unused14;      */
 short datatype;      /*!< Defines data type!    */  /* short datatype;      */
 short bitpix;        /*!< Number bits/voxel.    */  /* short bitpix;        */
 short slice_start;   /*!< First slice index.    */  /* short dim_un0;       */
 float pixdim[8];     /*!< Grid spacings.        */  /* float pixdim[8];     */
 float vox_offset;    /*!< Offset into .nii file */  /* float vox_offset;    */
 float scl_slope ;    /*!< Data scaling: slope.  */  /* float funused1;      */
 float scl_inter ;    /*!< Data scaling: offset. */  /* float funused2;      */
 short slice_end;     /*!< Last slice index.     */  /* float funused3;      */
 char  slice_code ;   /*!< Slice timing order.   */
 char  xyzt_units ;   /*!< Units of pixdim[1..4] */
 float cal_max;       /*!< Max display intensity */  /* float cal_max;       */
 float cal_min;       /*!< Min display intensity */  /* float cal_min;       */
 float slice_duration;/*!< Time for 1 slice.     */  /* float compressed;    */
 float toffset;       /*!< Time axis shift.      */  /* float verified;      */
 int   glmax;         /*!< ++UNUSED++            */  /* int glmax;           */
 int   glmin;         /*!< ++UNUSED++            */  /* int glmin;           */

                                         /*--- was data_history substruct ---*/
 char  descrip[80];   /*!< any text you like.    */  /* char descrip[80];    */
 char  aux_file[24];  /*!< auxiliary filename.   */  /* char aux_file[24];   */

 short qform_code ;   /*!< NIFTI_XFORM_* code.   */  /*-- all ANALYZE 7.5 ---*/
 short sform_code ;   /*!< NIFTI_XFORM_* code.   */  /*   fields below here  */
                                                     /*   are replaced       */
 float quatern_b ;    /*!< Quaternion b param.   */
 float quatern_c ;    /*!< Quaternion c param.   */
 float quatern_d ;    /*!< Quaternion d param.   */
 float qoffset_x ;    /*!< Quaternion x shift.   */
 float qoffset_y ;    /*!< Quaternion y shift.   */
 float qoffset_z ;    /*!< Quaternion z shift.   */

 float srow_x[4] ;    /*!< 1st row affine transform.   */
 float srow_y[4] ;    /*!< 2nd row affine transform.   */
 float srow_z[4] ;    /*!< 3rd row affine transform.   */

 char intent_name[16];/*!< 'name' or meaning of data.  */

 char magic[4] ;      /*!< MUST be "ni1\0" or "n+1\0". */

} ;                   /**** 348 bytes total ****/
typedef struct AtNifti1Header AtNifti1Header ;


typedef struct AtNiftiImagePrivate{
  int     ndim ;                  /*!< last dimension greater than 1 (1..7) */
  AtVec7 nsize;                 /*!< array of sizes for each dimension    */
  int     dim[8] ;                /*!< dim[0]=ndim, dim[1]=nx, etc.         */
  size_t  nvox ;                  /*!< number of voxels = nx*ny*nz*...*nw   */
  int     nbyper ;                /*!< bytes per voxel, matches datatype    */
  int     datatype ;              /*!< type of data in voxels: DT_* code    */

  AtVec7 dsize;                 /*!< array of grid spacings for each dim  */
  float   pixdim[8] ;             /*!< pixdim[1]=dx, etc. */

  float   scl_slope ;             /*!< scaling parameter - slope        */
  float   scl_inter ;             /*!< scaling parameter - intercept    */

  float   cal_min ;               /*!< calibration parameter, minimum   */
  float   cal_max ;               /*!< calibration parameter, maximum   */

  int     qform_code ;            /*!< codes for (x,y,z) space meaning  */
  int     sform_code ;            /*!< codes for (x,y,z) space meaning  */

  int     freq_dim  ;             /*!< indexes (1,2,3, or 0) for MRI    */
  int     phase_dim ;             /*!< directions in dim[]/pixdim[]     */
  int     slice_dim ;             /*!< directions in dim[]/pixdim[]     */

  int     slice_code  ;           /*!< code for slice timing pattern    */
  int     slice_start ;           /*!< index for start of slices        */
  int     slice_end   ;           /*!< index for end of slices          */
  float   slice_duration ;        /*!< time between individual slices   */

  /*! quaternion transform parameters
    [when writing a dataset, these are used for qform, NOT qto_xyz]   */
  float   quatern_b , quatern_c , quatern_d ,
          qoffset_x , qoffset_y , qoffset_z ,
          qfac      ;

  AtMat4 qto_xyz ;               /*!< qform: transform (i,j,k) to (x,y,z) */
  AtMat4 qto_ijk ;               /*!< qform: transform (x,y,z) to (i,j,k) */

  AtMat4 sto_xyz ;               /*!< sform: transform (i,j,k) to (x,y,z) */
  AtMat4 sto_ijk ;               /*!< sform: transform (x,y,z) to (i,j,k) */

  float   toffset ;               /*!< time coordinate offset */

  int     xyz_units  ;            /*!< dx,dy,dz units: NIFTI_UNITS_* code  */
  int     time_units ;            /*!< dt       units: NIFTI_UNITS_* code  */

  int     nifti_type ;            /*!< 0==ANALYZE, 1==NIFTI-1 (1 file),
                                                 2==NIFTI-1 (2 files),
                                                 3==NIFTI-ASCII (1 file) */
  int     intent_code ;           /*!< statistic type (or something)       */
  float   intent_p1 ;             /*!< intent parameters                   */
  float   intent_p2 ;             /*!< intent parameters                   */
  float   intent_p3 ;             /*!< intent parameters                   */
  char    intent_name[16] ;       /*!< optional description of intent data */

  char    descrip[80]  ;          /*!< optional text to describe dataset   */
  char    aux_file[24] ;          /*!< auxiliary filename                  */

  char *  fname ;                 /*!< header filename (.hdr or .nii)         */
  char *  iname ;                 /*!< image filename  (.img or .nii)         */
  int     iname_offset ;          /*!< offset into iname where data starts    */
  int     swapsize ;              /*!< swap unit in image data (might be 0)   */
  int     byteorder ;             /*!< byte order on disk (MSB_ or LSB_FIRST) */
  int                    num_ext ;  /*!< number of extensions in ext_list       */
  AtNifti1Extension   * ext_list ; /*!< array of extension structs (with data) */
  AtAnalyze75OrientCode analyze75_orient; /*!< for old analyze files, orient */
}AtNiftiImagePrivate;

/*----------------------------------------------------------------------*/
/*! check the end of the filename for a valid nifti extension

    Valid extensions are currently .nii, .hdr, .img, .nia,
    or any of them followed by .gz.  Note that '.' is part of
    the extension.

    \return a pointer to the extension (within the filename), or NULL
*//*--------------------------------------------------------------------*/
static char *
at_nifti_find_file_extension( const char * name )
{
   char * ext;
   int    len;

   if ( ! name ) return NULL;

   len = (int)strlen(name);
   if ( len < 4 ) return NULL;

   ext = (char *)name + len - 4;

   if ( (strcmp(ext, ".hdr") == 0) || (strcmp(ext, ".img") == 0) ||
        (strcmp(ext, ".nia") == 0) || (strcmp(ext, ".nii") == 0) )
      return ext;

#ifdef HAVE_ZLIB
   if ( len < 7 ) return NULL;

   ext = (char *)name + len - 7;

   if ( (strcmp(ext, ".hdr.gz") == 0) || (strcmp(ext, ".img.gz") == 0) ||
        (strcmp(ext, ".nii.gz") == 0) )
      return ext;
#endif
   return NULL;
}
/*----------------------------------------------------------------------*/
/*! return whether the filename is valid

    The name is considered valid if its length is positive, excluding
    any nifti filename extension.
    fname input         |  return | result of nifti_makebasename
    ====================================================================
    "myimage"           |  1      | "myimage"
    "myimage.tif"       |  1      | "myimage.tif"
    "myimage.tif.gz"    |  1      | "myimage.tif"
    "myimage.nii"       |  1      | "myimage"
    ".nii"              |  0      | <ERROR - basename has zero length>
    ".myhiddenimage"    |  1      | ".myhiddenimage"
    ".myhiddenimage.nii |  1      | ".myhiddenimage"
*//*--------------------------------------------------------------------*/
static gboolean
at_nifti_validfilename(const char* fname)
{
   char * ext;

   /* check input file(s) for sanity */
   if( fname == NULL || *fname == '\0' ){
      return FALSE;
   }

   ext = at_nifti_find_file_extension(fname);

   if ( ext && ext == fname ) {   /* then no filename prefix */
      return FALSE;
   }

   return TRUE;
}
/*----------------------------------------------------------------------*/
/*! duplicate the filename, while clearing any extension

    This allocates memory for basename which should eventually be freed.
*//*--------------------------------------------------------------------*/
static char *
at_nifti_makebasename(const char* fname)
{
   char *basename, *ext;

   basename = g_strdup(fname);

   ext = at_nifti_find_file_extension(basename);
   if ( ext ) *ext = '\0';  /* clear out extension */

   return basename;  /* in either case */
}
/*----------------------------------------------------------------------*/
/*! simple check for file existence

    \return 1 on existence, 0 otherwise
*//*--------------------------------------------------------------------*/
static gboolean
at_nifti_fileexists(const char* fname)
{
  AtZnzFile* fp = at_znzfile_open(fname, "rb", TRUE);
  if(fp){
    g_clear_object(&fp);
    return TRUE;
  }
  return FALSE;
}
/*----------------------------------------------------------------------*/
/*! check current directory for existing header file

    \return filename of header on success and NULL if no appropriate file
            could be found

    NB: it allocates memory for hdrname which should be freed
        when no longer required
*//*-------------------------------------------------------------------*/
static char *
at_nifti_findhdrname(const char* fname)
{
   char *basename, *hdrname, *ext;
   char  elist[2][5] = { ".hdr", ".nii" };
   int   efirst;

   /**- check input file(s) for sanity */
   if( !at_nifti_validfilename(fname) ) return NULL;

   basename = at_nifti_makebasename(fname);
   if( !basename ) return NULL;   /* only on string alloc failure */

   /**- return filename if it has a valid extension and exists
         (except if it is an .img file (and maybe .gz)) */
   ext = at_nifti_find_file_extension(fname);
   if ( ext && at_nifti_fileexists(fname) ) {
     if ( strncmp(ext,".img",4) != 0 ){
        hdrname = g_strdup(fname);
        free(basename);
        return hdrname;
     }
   }

   /* So the requested name is a basename, contains .img, or does not exist. */
   /* In any case, use basename. */

   /**- if .img, look for .hdr, .hdr.gz, .nii, .nii.gz, in that order */
   /**- else,    look for .nii, .nii.gz, .hdr, .hdr.gz, in that order */

   /* if we get more extension choices, this could be a loop */

   if ( ext && strncmp(ext,".img",4) == 0 ) efirst = 0;
   else                                     efirst = 1;

   hdrname = (char *)calloc(sizeof(char),strlen(basename)+8);
   if( !hdrname ){
      fprintf(stderr,"** nifti_findhdrname: failed to alloc hdrname\n");
      free(basename);
      return NULL;
   }

   strcpy(hdrname,basename);
   strcat(hdrname,elist[efirst]);
   if (at_nifti_fileexists(hdrname)) { free(basename); return hdrname; }
#ifdef HAVE_ZLIB
   strcat(hdrname,".gz");
   if (at_nifti_fileexists(hdrname)) { free(basename); return hdrname; }
#endif

   /* okay, try the other possibility */

   efirst = 1 - efirst;

   strcpy(hdrname,basename);
   strcat(hdrname,elist[efirst]);
   if (at_nifti_fileexists(hdrname)) { free(basename); return hdrname; }
#ifdef HAVE_ZLIB
   strcat(hdrname,".gz");
   if (at_nifti_fileexists(hdrname)) { free(basename); return hdrname; }
#endif

   /**- if nothing has been found, return NULL */
   free(basename);
   free(hdrname);
   return NULL;
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtNiftiImage *at_nifti_image_read( const char *hname , gboolean read_data )
{
   struct AtNifti1Header  nhdr;
   AtNiftiImage          *nim;
   AtZnzFile*             fp = NULL;
   int                     rv, ii , filesize, remaining;
   g_autofree char        *hfile=NULL;

   /**- determine filename to use for header */
   hfile = at_nifti_findhdrname(hname);
   if( hfile == NULL ){
      return NULL;  /* check return */
   }

   if( at_nifti_is_gzfile(hfile) ) filesize = -1;  /* unknown */
   else                         filesize = at_nifti_get_filesize(hfile);

   fp = at_znzfile_open(hfile, "rb", at_nifti_is_gzfile(hfile));
   if(fp == NULL) return NULL;

   rv = at_nifti_has_ascii_header( fp );
   if(rv < 0){
     g_clear_object(&fp);
     return NULL;
   }
   else if ( rv == 1 )  /* process special file type */
      return at_nifti_read_ascii_image( fp, hfile, filesize, read_data );

   /* else, just process normally */

   /**- read binary header */

   ii = (int)at_znzfile_read(fp, &nhdr , 1 , sizeof(nhdr) ) ;       /* read the thing */

   /* keep file open so we can check for exts. after nifti_convert_nhdr2nim() */

   if( ii < (int) sizeof(nhdr) ){
      g_clear_object(&fp);
      return NULL;
   }

   /* create output image struct and set it up */

   /**- convert all nhdr fields to nifti_image fields */
   nim = at_nifti_convert_nhdr2nim(nhdr,hfile);

   if( nim == NULL ){
      g_clear_object(&fp);                                   /* close the file */
      return NULL;
   }
   AtNiftiImagePrivate* priv = at_nifti_image_get_instance_private(nim);
   /**- check for extensions (any errors here means no extensions) */
   if( AT_NIFTI_ONEFILE(nhdr) ) remaining = priv->iname_offset - sizeof(nhdr);
   else                      remaining = filesize - sizeof(nhdr);

   (void)at_nifti_read_extensions(nim, fp, remaining);

   g_clear_object(&fp);                                      /* close the file */

   /**- read the data if desired, then bug out */
   if( read_data ){
      if( at_nifti_image_load( nim ) < 0 ){
        g_clear_object(&nim);
        return NULL;
      }
   }
   else {
     at_ndarray_dealloc_data(AT_NDARRAY(nim));
   }

   return nim ;
}
