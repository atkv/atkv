#include <at/imgproc.h>
#include <string.h>
#include <stdlib.h>

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
enum { XY_SHIFT = 16, XY_ONE = 1 << XY_SHIFT, DRAWING_STORAGE_BLOCK = (1<<12) - 256 };
static const float
SinTable[] =
    { 0.0000000f, 0.0174524f, 0.0348995f, 0.0523360f, 0.0697565f, 0.0871557f,
    0.1045285f, 0.1218693f, 0.1391731f, 0.1564345f, 0.1736482f, 0.1908090f,
    0.2079117f, 0.2249511f, 0.2419219f, 0.2588190f, 0.2756374f, 0.2923717f,
    0.3090170f, 0.3255682f, 0.3420201f, 0.3583679f, 0.3746066f, 0.3907311f,
    0.4067366f, 0.4226183f, 0.4383711f, 0.4539905f, 0.4694716f, 0.4848096f,
    0.5000000f, 0.5150381f, 0.5299193f, 0.5446390f, 0.5591929f, 0.5735764f,
    0.5877853f, 0.6018150f, 0.6156615f, 0.6293204f, 0.6427876f, 0.6560590f,
    0.6691306f, 0.6819984f, 0.6946584f, 0.7071068f, 0.7193398f, 0.7313537f,
    0.7431448f, 0.7547096f, 0.7660444f, 0.7771460f, 0.7880108f, 0.7986355f,
    0.8090170f, 0.8191520f, 0.8290376f, 0.8386706f, 0.8480481f, 0.8571673f,
    0.8660254f, 0.8746197f, 0.8829476f, 0.8910065f, 0.8987940f, 0.9063078f,
    0.9135455f, 0.9205049f, 0.9271839f, 0.9335804f, 0.9396926f, 0.9455186f,
    0.9510565f, 0.9563048f, 0.9612617f, 0.9659258f, 0.9702957f, 0.9743701f,
    0.9781476f, 0.9816272f, 0.9848078f, 0.9876883f, 0.9902681f, 0.9925462f,
    0.9945219f, 0.9961947f, 0.9975641f, 0.9986295f, 0.9993908f, 0.9998477f,
    1.0000000f, 0.9998477f, 0.9993908f, 0.9986295f, 0.9975641f, 0.9961947f,
    0.9945219f, 0.9925462f, 0.9902681f, 0.9876883f, 0.9848078f, 0.9816272f,
    0.9781476f, 0.9743701f, 0.9702957f, 0.9659258f, 0.9612617f, 0.9563048f,
    0.9510565f, 0.9455186f, 0.9396926f, 0.9335804f, 0.9271839f, 0.9205049f,
    0.9135455f, 0.9063078f, 0.8987940f, 0.8910065f, 0.8829476f, 0.8746197f,
    0.8660254f, 0.8571673f, 0.8480481f, 0.8386706f, 0.8290376f, 0.8191520f,
    0.8090170f, 0.7986355f, 0.7880108f, 0.7771460f, 0.7660444f, 0.7547096f,
    0.7431448f, 0.7313537f, 0.7193398f, 0.7071068f, 0.6946584f, 0.6819984f,
    0.6691306f, 0.6560590f, 0.6427876f, 0.6293204f, 0.6156615f, 0.6018150f,
    0.5877853f, 0.5735764f, 0.5591929f, 0.5446390f, 0.5299193f, 0.5150381f,
    0.5000000f, 0.4848096f, 0.4694716f, 0.4539905f, 0.4383711f, 0.4226183f,
    0.4067366f, 0.3907311f, 0.3746066f, 0.3583679f, 0.3420201f, 0.3255682f,
    0.3090170f, 0.2923717f, 0.2756374f, 0.2588190f, 0.2419219f, 0.2249511f,
    0.2079117f, 0.1908090f, 0.1736482f, 0.1564345f, 0.1391731f, 0.1218693f,
    0.1045285f, 0.0871557f, 0.0697565f, 0.0523360f, 0.0348995f, 0.0174524f,
    0.0000000f, -0.0174524f, -0.0348995f, -0.0523360f, -0.0697565f, -0.0871557f,
    -0.1045285f, -0.1218693f, -0.1391731f, -0.1564345f, -0.1736482f, -0.1908090f,
    -0.2079117f, -0.2249511f, -0.2419219f, -0.2588190f, -0.2756374f, -0.2923717f,
    -0.3090170f, -0.3255682f, -0.3420201f, -0.3583679f, -0.3746066f, -0.3907311f,
    -0.4067366f, -0.4226183f, -0.4383711f, -0.4539905f, -0.4694716f, -0.4848096f,
    -0.5000000f, -0.5150381f, -0.5299193f, -0.5446390f, -0.5591929f, -0.5735764f,
    -0.5877853f, -0.6018150f, -0.6156615f, -0.6293204f, -0.6427876f, -0.6560590f,
    -0.6691306f, -0.6819984f, -0.6946584f, -0.7071068f, -0.7193398f, -0.7313537f,
    -0.7431448f, -0.7547096f, -0.7660444f, -0.7771460f, -0.7880108f, -0.7986355f,
    -0.8090170f, -0.8191520f, -0.8290376f, -0.8386706f, -0.8480481f, -0.8571673f,
    -0.8660254f, -0.8746197f, -0.8829476f, -0.8910065f, -0.8987940f, -0.9063078f,
    -0.9135455f, -0.9205049f, -0.9271839f, -0.9335804f, -0.9396926f, -0.9455186f,
    -0.9510565f, -0.9563048f, -0.9612617f, -0.9659258f, -0.9702957f, -0.9743701f,
    -0.9781476f, -0.9816272f, -0.9848078f, -0.9876883f, -0.9902681f, -0.9925462f,
    -0.9945219f, -0.9961947f, -0.9975641f, -0.9986295f, -0.9993908f, -0.9998477f,
    -1.0000000f, -0.9998477f, -0.9993908f, -0.9986295f, -0.9975641f, -0.9961947f,
    -0.9945219f, -0.9925462f, -0.9902681f, -0.9876883f, -0.9848078f, -0.9816272f,
    -0.9781476f, -0.9743701f, -0.9702957f, -0.9659258f, -0.9612617f, -0.9563048f,
    -0.9510565f, -0.9455186f, -0.9396926f, -0.9335804f, -0.9271839f, -0.9205049f,
    -0.9135455f, -0.9063078f, -0.8987940f, -0.8910065f, -0.8829476f, -0.8746197f,
    -0.8660254f, -0.8571673f, -0.8480481f, -0.8386706f, -0.8290376f, -0.8191520f,
    -0.8090170f, -0.7986355f, -0.7880108f, -0.7771460f, -0.7660444f, -0.7547096f,
    -0.7431448f, -0.7313537f, -0.7193398f, -0.7071068f, -0.6946584f, -0.6819984f,
    -0.6691306f, -0.6560590f, -0.6427876f, -0.6293204f, -0.6156615f, -0.6018150f,
    -0.5877853f, -0.5735764f, -0.5591929f, -0.5446390f, -0.5299193f, -0.5150381f,
    -0.5000000f, -0.4848096f, -0.4694716f, -0.4539905f, -0.4383711f, -0.4226183f,
    -0.4067366f, -0.3907311f, -0.3746066f, -0.3583679f, -0.3420201f, -0.3255682f,
    -0.3090170f, -0.2923717f, -0.2756374f, -0.2588190f, -0.2419219f, -0.2249511f,
    -0.2079117f, -0.1908090f, -0.1736482f, -0.1564345f, -0.1391731f, -0.1218693f,
    -0.1045285f, -0.0871557f, -0.0697565f, -0.0523360f, -0.0348995f, -0.0174524f,
    -0.0000000f, 0.0174524f, 0.0348995f, 0.0523360f, 0.0697565f, 0.0871557f,
    0.1045285f, 0.1218693f, 0.1391731f, 0.1564345f, 0.1736482f, 0.1908090f,
    0.2079117f, 0.2249511f, 0.2419219f, 0.2588190f, 0.2756374f, 0.2923717f,
    0.3090170f, 0.3255682f, 0.3420201f, 0.3583679f, 0.3746066f, 0.3907311f,
    0.4067366f, 0.4226183f, 0.4383711f, 0.4539905f, 0.4694716f, 0.4848096f,
    0.5000000f, 0.5150381f, 0.5299193f, 0.5446390f, 0.5591929f, 0.5735764f,
    0.5877853f, 0.6018150f, 0.6156615f, 0.6293204f, 0.6427876f, 0.6560590f,
    0.6691306f, 0.6819984f, 0.6946584f, 0.7071068f, 0.7193398f, 0.7313537f,
    0.7431448f, 0.7547096f, 0.7660444f, 0.7771460f, 0.7880108f, 0.7986355f,
    0.8090170f, 0.8191520f, 0.8290376f, 0.8386706f, 0.8480481f, 0.8571673f,
    0.8660254f, 0.8746197f, 0.8829476f, 0.8910065f, 0.8987940f, 0.9063078f,
    0.9135455f, 0.9205049f, 0.9271839f, 0.9335804f, 0.9396926f, 0.9455186f,
    0.9510565f, 0.9563048f, 0.9612617f, 0.9659258f, 0.9702957f, 0.9743701f,
    0.9781476f, 0.9816272f, 0.9848078f, 0.9876883f, 0.9902681f, 0.9925462f,
    0.9945219f, 0.9961947f, 0.9975641f, 0.9986295f, 0.9993908f, 0.9998477f,
    1.0000000f
};
static void
sincos_custom( int angle, float* cosval, float* sinval )
{
  angle += (angle < 0 ? 360 : 0);
  *sinval = SinTable[angle];
  *cosval = SinTable[450 - angle];
}

// Draw Line without shift
static void
at_array_uint8_t_draw_line_no_shift(AtArray_uint8_t* array, AtVec2i p1, AtVec2i p2, AtVec4i* _color, AtLineType connectivity){
  int err, count;
  uint8_t* ptr;
  int minusDelta, plusDelta;
  int minusStep, plusStep;

  uint8_t left_to_right = 1;

  // BEGIN
  count = -1;

  g_autofree uint64_t* size = at_array_get_size(array);

  if( (unsigned)p1.data[0] >= (unsigned)(size[1]) ||
      (unsigned)p2.data[0] >= (unsigned)(size[1]) ||
      (unsigned)p1.data[1] >= (unsigned)(size[0]) ||
      (unsigned)p2.data[1] >= (unsigned)(size[0]) )
  {
    AtVec2i size2 = {{size[0],size[1]}};
      if( !at_clip_line( size2, &p1, &p2 ) ){
          ptr = at_array_get(array);
          err = plusDelta = minusDelta = plusStep = minusStep = count = 0;

          return;
      }
  }

  int bt_pix0 = at_array_get_dim(array) == 2?1:(int)at_array_get_size(array,2), bt_pix = bt_pix0;
  size_t istep = at_array_get_step(array,0);

  int dx = p2.data[0] - p1.data[0];
  int dy = p2.data[1] - p1.data[1];
  int s = dx < 0 ? -1 : 0;

  if( left_to_right )  {
      dx = (dx ^ s) - s;
      dy = (dy ^ s) - s;
      p1.data[0] ^= (p1.data[0] ^ p2.data[0]) & s;
      p1.data[1] ^= (p1.data[1] ^ p2.data[1]) & s;
  }  else  {
      dx = (dx ^ s) - s;
      bt_pix = (bt_pix ^ s) - s;
  }

  ptr = (uint8_t*)(at_array_get(array) + p1.data[1] * istep + p1.data[0] * bt_pix0);

  s = dy < 0 ? -1 : 0;
  dy = (dy ^ s) - s;
  istep = (istep ^ s) - s;

  s = dy > dx ? -1 : 0;

  /* conditional swaps */
  dx ^= dy & s;
  dy ^= dx & s;
  dx ^= dy & s;

  bt_pix ^= istep & s;
  istep ^= bt_pix & s;
  bt_pix ^= istep & s;

  if( connectivity == AT_LINE_TYPE_8)  {

      err = dx - (dy + dy);
      plusDelta = dx + dx;
      minusDelta = -(dy + dy);
      plusStep = (int)istep;
      minusStep = bt_pix;
      count = dx + 1;
  }  else  { /* connectivity == 4 */

      err = 0;
      plusDelta = (dx + dx) + (dy + dy);
      minusDelta = -(dy + dy);
      plusStep = (int)istep - bt_pix;
      minusStep = bt_pix;
      count = dx + dy + 1;
  }
  // END

  int i,j;
  int pix_size       = at_array_get_dim(array) == 2?1:at_array_get_size(array,2);
  int* color         = ((int*)_color);

  for( i = 0; i < count; i++)  {
    for(j = 0; j < pix_size; j++) ptr[j] = (uint8_t)color[j];

    int mask = err < 0 ? -1 : 0;
    err += minusDelta + (plusDelta & mask);
    ptr += minusStep + (plusStep & mask);
  }
}

static void
at_array_uint8_t_draw_line_neighbor_8(AtArray_uint8_t* array, AtVec2i p1, AtVec2i p2, AtVec4i* color){
  int dx, dy; // Manhattan distance between points
  int ecount; // Number of elements
  int ax, ay;
  int x,y,i,j;
  int x_step, y_step;
  int pix_size = at_array_get_dim(array) == 2? 1:at_array_get_size(array,2);
  uint8_t *tptr, *ptr = at_array_get(array);
  g_autofree uint64_t* array_size = at_array_get_size(array);
  AtVec2i  size_scaled = {array_size[1] * XY_ONE, array_size[0] * XY_ONE};
  uint64_t step = at_array_get_step(array,0);

  // We need to clip the line
  // If the line is completely outside rectangle, exit
  if(!at_clip_line(size_scaled, &p1, &p2)) return;

  // Manhattan distance
  dx = p2.data[0] - p1.data[0];
  dy = p2.data[1] - p1.data[1];

  //
  j  =  dx < 0? -1:0;
  ax = (dx ^ j) - j;

  i  =  dy < 0? -1:0;
  ay = (dy ^ i) - i;

  // Change axis
#define CHANGE
  if(ax > ay){
    dx    = ax;
    dy    = (dy ^ j) - j;
    p1.data[0] ^= p2.data[0] & j;// Change p1 and p2, if necessary,
    p2.data[0] ^= p1.data[0] & j;// to get p1.data[0] < p2.data[0]
    p1.data[0] ^= p2.data[0] & j;
    p1.data[0] ^= p2.data[1] & j;
    p2.data[0] ^= p1.data[1] & j;
    p1.data[0] ^= p2.data[1] & j;

    x_step = XY_ONE;
    y_step = (int)(((int64_t) dy << XY_SHIFT) / (ax | 1));
    ecount = (p2.data[0] - p1.data[0]) >> XY_SHIFT;
  }else{
    dy    = ay;
    dx    = (dx ^ i) - i;
    p1.data[0] ^= p2.data[0] & i;
    p2.data[0] ^= p1.data[0] & i;
    p1.data[0] ^= p2.data[0] & i;
    p1.data[0] ^= p2.data[1] & i;
    p2.data[0] ^= p1.data[1] & i;
    p1.data[0] ^= p2.data[1] & i;

    x_step = (int)(((int64_t) dx << XY_SHIFT) / (ay | 1));
    y_step = XY_ONE;
    ecount = (p2.data[1] - p1.data[1]) >> XY_SHIFT;
  }

  p1.data[0] += (XY_ONE >> 1);
  p1.data[1] += (XY_ONE >> 1);

  #define  IGRF_PUT_POINT(_x,_y)   \
  x = (_x); y = (_y);             \
  if( 0 <= x && x < array_size[1] && \
      0 <= y && y < array_size[0]) \
  {                               \
      tptr = ptr + y*step + x*pix_size;\
      for( j = 0; j < pix_size; j++ ) \
          tptr[j] = (uint8_t)((int*)color)[j]; \
  }

  IGRF_PUT_POINT((p2.data[0] + (XY_ONE >> 1)) >> XY_SHIFT,
                (p2.data[1] + (XY_ONE >> 1)) >> XY_SHIFT);

  if( ax > ay )
  {
    p1.data[0] >>= XY_SHIFT;

    while( ecount >= 0 )
    {
        IGRF_PUT_POINT(p1.data[0], p1.data[1] >> XY_SHIFT);
        p1.data[0]++;
        p1.data[1] += y_step;
        ecount--;
    }
  }
  else
  {
    p1.data[1] >>= XY_SHIFT;

    while( ecount >= 0 )
    {
        IGRF_PUT_POINT(p1.data[0] >> XY_SHIFT, p1.data[1]);
        p1.data[0] += x_step;
        p1.data[1]++;
        ecount--;
    }
  }

  #undef IGRF_PUT_POINT
}

static void
at_array_uint8_t_draw_line_antialiased(AtArray_uint8_t* array, AtVec2i p1, AtVec2i p2, AtVec4i* color){
  (void) array; (void) p1; (void) p2; (void) color;
}

/* helper macros: filling horizontal row */
#define IGRF_HLINE( ptr, xl, xr, color, pix_size )            \
{                                                            \
    uint8_t* hline_ptr = (uint8_t*)(ptr) + (xl)*(pix_size);      \
    uint8_t* hline_max_ptr = (uint8_t*)(ptr) + (xr)*(pix_size);  \
                                                             \
    for( ; hline_ptr <= hline_max_ptr; hline_ptr += (pix_size))\
    {                                                        \
        int hline_j;                                         \
        for( hline_j = 0; hline_j < (pix_size); hline_j++ )  \
        {                                                    \
            hline_ptr[hline_j] = (uint8_t)((int*)color)[hline_j];   \
        }                                                    \
    }                                                        \
}
static void
at_array_uint8_t_fill_convex_poly(AtArray_uint8_t* array, AtVec2i* v, int npts, AtVec4i* color, AtLineType line_type, int shift){
  struct
  {
    int idx, di;
    int x, dx, ye;
  }
  edge[2];

  int xmin, xmax, ymin, ymax;
  int delta1, delta2;
  AtVec2i p0;
  int i, y, imin  = 0, left = 0, right = 1, x1, x2;
  int delta       = shift ? 1 << (shift - 1) : 0;
  int edges       = npts;
  uint8_t* ptr    = at_array_get(array);
  g_autofree uint64_t* array_size = at_array_get_size(array);
  g_autofree uint64_t* array_step = at_array_get_step(array);
  int pix_size    = at_array_get_dim(array) < 3? 1:array_size[2];

  if( line_type != AT_LINE_TYPE_AA)
      delta1 = delta2 = XY_ONE >> 1;
  else
      delta1 = XY_ONE - 1, delta2 = 0;

  p0 = v[npts - 1];
  p0.data[0] <<= XY_SHIFT - shift;
  p0.data[1] <<= XY_SHIFT - shift;

  xmin = xmax = v[0].data[0];
  ymin = ymax = v[0].data[1];

  for( i = 0; i < npts; i++ )
  {
      AtVec2i p = v[i];
      if( p.data[1] < ymin )
      {
          ymin = p.data[1];
          imin = i;
      }

      ymax = max( ymax, p.data[1] );
      xmax = max( xmax, p.data[0] );
      xmin = min( xmin, p.data[0] );

      p.data[0] <<= XY_SHIFT - shift;
      p.data[1] <<= XY_SHIFT - shift;

      if( line_type <= 8 )
      {
          if( shift == 0 )
          {
              AtVec2i pt0, pt1;
              pt0.data[0] = p0.data[0] >> XY_SHIFT;
              pt0.data[1] = p0.data[1] >> XY_SHIFT;
              pt1.data[0] = p.data[0] >> XY_SHIFT;
              pt1.data[1] = p.data[1] >> XY_SHIFT;
              at_array_uint8_t_draw_line_no_shift(array, pt0, pt1, color, line_type );
          }
          else
              at_array_uint8_t_draw_line_neighbor_8(array, p0, p, color );
      }
      else
          at_array_uint8_t_draw_line_antialiased(array, p0, p, color );
      p0 = p;
  }

  xmin = (xmin + delta) >> shift;
  xmax = (xmax + delta) >> shift;
  ymin = (ymin + delta) >> shift;
  ymax = (ymax + delta) >> shift;

  if( npts < 3 || xmax < 0 || ymax < 0 || xmin >= array_size[1] || ymin >= array_size[0])
      return;

  ymax = min( ymax, array_size[0] - 1 );
  edge[0].idx = edge[1].idx = imin;

  edge[0].ye = edge[1].ye = y = ymin;
  edge[0].di = 1;
  edge[1].di = npts - 1;

  ptr += array_step[0]*y;

  do
  {
      if( line_type != AT_LINE_TYPE_AA || y < ymax || y == ymin )
      {
          for( i = 0; i < 2; i++ )
          {
              if( y >= edge[i].ye )
              {
                  int idx = edge[i].idx, di = edge[i].di;
                  int xs = 0, xe, ye, ty = 0;

                  for(;;)
                  {
                      ty = (v[idx].data[1] + delta) >> shift;
                      if( ty > y || edges == 0 )
                          break;
                      xs = v[idx].data[0];
                      idx += di;
                      idx -= ((idx < npts) - 1) & npts;   /* idx -= idx >= npts ? npts : 0 */
                      edges--;
                  }

                  ye = ty;
                  xs <<= XY_SHIFT - shift;
                  xe = v[idx].data[0] << (XY_SHIFT - shift);

                  /* no more edges */
                  if( y >= ye )
                      return;

                  edge[i].ye = ye;
                  edge[i].dx = ((xe - xs)*2 + (ye - y)) / (2 * (ye - y));
                  edge[i].x = xs;
                  edge[i].idx = idx;
              }
          }
      }

      if( edge[left].x > edge[right].x )
      {
          left ^= 1;
          right ^= 1;
      }

      x1 = edge[left].x;
      x2 = edge[right].x;

      if( y >= 0 )
      {
          int xx1 = (x1 + delta1) >> XY_SHIFT;
          int xx2 = (x2 + delta2) >> XY_SHIFT;

          if( xx2 >= 0 && xx1 < array_size[1])
          {
              if( xx1 < 0 )
                  xx1 = 0;
              if( xx2 >= array_size[1] )
                  xx2 = array_size[1] - 1;
              IGRF_HLINE( ptr, xx1, xx2, color, pix_size );
          }
      }

      x1 += edge[left].dx;
      x2 += edge[right].dx;

      edge[left].x = x1;
      edge[right].x = x2;
      ptr += array_step[0];
  }
  while( ++y <= ymax );
}

static void
at_array_uint8_t_draw_circle_direct   (AtArray_uint8_t* array, AtVec2i center, int radius, AtVec4i* _color, int fill){
  g_autofree uint64_t* array_size = at_array_get_size(array);
  g_autofree uint64_t* array_step = at_array_get_step(array);
  size_t     step     = array_step[0];
  int        pix_size = at_array_get_dim(array) == 2?1:array_size[2];
  uint8_t*   ptr      = at_array_get(array);
  uint8_t    color[3] = {_color->data[0], _color->data[1], _color->data[2]};
  int err = 0, dx = radius, dy = 0, plus = 1, minus = (radius << 1) - 1;
  int inside = center.data[0] >= radius && center.data[0] < array_size[1] - radius &&
               center.data[1] >= radius && center.data[1] < array_size[0] - radius;

  #define IGRF_PUT_POINT( ptr, x )     \
    memcpy( ptr + (x)*pix_size, color, pix_size );

  while( dx >= dy )
  {
    int mask;
    int y11 = center.data[1] - dy, y12 = center.data[1] + dy, y21 = center.data[1] - dx, y22 = center.data[1] + dx;
    int x11 = center.data[0] - dx, x12 = center.data[0] + dx, x21 = center.data[0] - dy, x22 = center.data[0] + dy;

    if( inside ){
      uint8_t *tptr0 = ptr + y11 * step;
      uint8_t *tptr1 = ptr + y12 * step;

      if( !fill ){
        IGRF_PUT_POINT( tptr0, x11 );
        IGRF_PUT_POINT( tptr1, x11 );
        IGRF_PUT_POINT( tptr0, x12 );
        IGRF_PUT_POINT( tptr1, x12 );
      }else{
        IGRF_HLINE( tptr0, x11, x12, _color, pix_size );
        IGRF_HLINE( tptr1, x11, x12, _color, pix_size );
      }

      tptr0 = ptr + y21 * step;
      tptr1 = ptr + y22 * step;

      if(!fill){
        IGRF_PUT_POINT( tptr0, x21 );
        IGRF_PUT_POINT( tptr1, x21 );
        IGRF_PUT_POINT( tptr0, x22 );
        IGRF_PUT_POINT( tptr1, x22 );
      }else{
        IGRF_HLINE( tptr0, x21, x22, _color, pix_size );
        IGRF_HLINE( tptr1, x21, x22, _color, pix_size );
      }
    }
    else if( x11 < array_size[1] && x12 >= 0 && y21 < array_size[0] && y22 >= 0 ){
      if( fill ){
        x11 = max( x11, 0 );
        x12 = min( x12, array_size[1] - 1 );
      }

      if( (unsigned)y11 < (unsigned)array_size[0]){
        uint8_t *tptr = ptr + y11 * step;

        if( !fill ){
          if( x11 >= 0 )
            IGRF_PUT_POINT( tptr, x11 );
          if( x12 < array_size[1] )
            IGRF_PUT_POINT( tptr, x12 );
        }
        else
          IGRF_HLINE( tptr, x11, x12, _color, pix_size );
      }

      if( (unsigned)y12 < (unsigned)array_size[0] ){
        uint8_t *tptr = ptr + y12 * step;

        if( !fill ){
          if( x11 >= 0 )
            IGRF_PUT_POINT( tptr, x11 );
          if( x12 < array_size[1] )
            IGRF_PUT_POINT( tptr, x12 );
        }
        else
          IGRF_HLINE( tptr, x11, x12, _color, pix_size );
      }

      if( x21 < array_size[1] && x22 >= 0 ){
        if( fill ){
          x21 = max( x21, 0 );
          x22 = min( x22, array_size[1] - 1 );
        }

        if( (unsigned)y21 < (unsigned)array_size[0] ){
          uint8_t *tptr = ptr + y21 * step;

          if( !fill ){
            if( x21 >= 0 )
              IGRF_PUT_POINT( tptr, x21 );
            if( x22 < array_size[1] )
              IGRF_PUT_POINT( tptr, x22 );
          }
          else
            IGRF_HLINE( tptr, x21, x22, _color, pix_size );
        }

        if( (unsigned)y22 < (unsigned)array_size[0] )
        {
          uint8_t *tptr = ptr + y22 * step;

          if( !fill ){
            if( x21 >= 0 )
              IGRF_PUT_POINT( tptr, x21 );
            if( x22 < array_size[1] )
              IGRF_PUT_POINT( tptr, x22 );
          }
          else
            IGRF_HLINE( tptr, x21, x22, _color, pix_size );
        }
      }
    }
    dy++;
    err += plus;
    plus += 2;

    mask = (err <= 0) - 1;

    err -= minus & mask;
    dx += mask;
    minus -= mask & 2;
  }

  #undef  IGRF_PUT_POINT
}

static void
at_array_uint8_t_draw_polyline(AtArray_uint8_t* array, AtVec2i* v, int count, uint8_t is_closed, AtVec4i* color, int thickness, AtLineType line_type, int shift);
static void
at_array_uint8_t_draw_line_thick(AtArray_uint8_t* array,
                                 AtVec2i p1,
                                 AtVec2i p2,
                                 AtVec4i* color,
                                 int thickness,
                                 AtLineType line_type,
                                 int flags,
                                 int shift);
static void
at_array_uint8_t_draw_ellipse_ex      (AtArray_uint8_t* array, AtVec2i center, AtVec2i axes, int angle, int arc_start, int arc_end, AtVec4i* color, int thickness, AtLineType line_type);

static void
at_array_uint8_t_draw_line_thick(AtArray_uint8_t* array,
                                 AtVec2i p1,
                                 AtVec2i p2,
                                 AtVec4i* color,
                                 int thickness,
                                 AtLineType line_type,
                                 int flags,
                                 int shift){
  static const double INV_XY_ONE = 1./XY_ONE;

  p1.data[0] <<= XY_SHIFT - shift;
  p1.data[1] <<= XY_SHIFT - shift;
  p2.data[0] <<= XY_SHIFT - shift;
  p2.data[1] <<= XY_SHIFT - shift;

  if(thickness <= 1){
    if(line_type != AT_LINE_TYPE_AA){
      if(line_type == AT_LINE_TYPE_4 || shift == 0){
        p1.data[0] = (p1.data[0] + (XY_ONE>>1)) >> XY_SHIFT;
        p1.data[1] = (p1.data[1] + (XY_ONE>>1)) >> XY_SHIFT;
        p2.data[0] = (p2.data[0] + (XY_ONE>>1)) >> XY_SHIFT;
        p2.data[1] = (p2.data[1] + (XY_ONE>>1)) >> XY_SHIFT;
        at_array_uint8_t_draw_line_no_shift(array, p1, p2, color, line_type);
      }
      else
        at_array_uint8_t_draw_line_neighbor_8(array, p1, p2, color);
    }
    else
      at_array_uint8_t_draw_line_antialiased(array, p1, p2, color);
  }else{ // Thickness > 1
    AtVec2i pt[4], dp = {0,0};
    double dx = (p1.data[0] - p2.data[0])*INV_XY_ONE, dy = (p2.data[1] - p1.data[1])*INV_XY_ONE;
    double r = dx * dx + dy * dy;
    int i, oddThickness = thickness & 1;
    thickness <<= XY_SHIFT - 1;

    if( fabs(r) > DBL_EPSILON )
    {
        r = (thickness + oddThickness*XY_ONE*0.5)/sqrt(r);
        dp.data[0] = round(dy * r );
        dp.data[1] = round(dx * r );

        pt[0].data[0] = p1.data[0] + dp.data[0];
        pt[0].data[1] = p1.data[1] + dp.data[1];
        pt[1].data[0] = p1.data[0] - dp.data[0];
        pt[1].data[1] = p1.data[1] - dp.data[1];
        pt[2].data[0] = p2.data[0] - dp.data[0];
        pt[2].data[1] = p2.data[1] - dp.data[1];
        pt[3].data[0] = p2.data[0] + dp.data[0];
        pt[3].data[1] = p2.data[1] + dp.data[1];

        at_array_uint8_t_fill_convex_poly(array, pt, 4, color, line_type, XY_SHIFT );
    }

    for( i = 0; i < 2; i++ )
    {
        if( flags & (i+1) )
        {
            if( line_type < AT_LINE_TYPE_AA )
            {
                AtVec2i center;
                center.data[0] = (p1.data[0] + (XY_ONE>>1)) >> XY_SHIFT;
                center.data[1] = (p1.data[1] + (XY_ONE>>1)) >> XY_SHIFT;
                at_array_uint8_t_draw_circle_direct(array, center, (thickness + (XY_ONE>>1)) >> XY_SHIFT, color, 1 );
            }
            else
            {
                AtVec2i size = {thickness, thickness};
                at_array_uint8_t_draw_ellipse_ex( array, p1, size, 0, 0, 360, color, -1, line_type );
            }
        }
        p1 = p2;
    }
  }
}
static void
at_array_uint8_t_draw_polyline(AtArray_uint8_t* array, AtVec2i* v, int count, uint8_t is_closed, AtVec4i* color, int thickness, AtLineType line_type, int shift){
  if(!v || count <= 0) return;
  int i     = is_closed ? count - 1: 0;
  int flags = 2 + !is_closed;
  AtVec2i p0;
  p0 = v[i];
  for(i = !is_closed; i < count; i++){
    AtVec2i p = v[i];
    at_array_uint8_t_draw_line_thick(array, p0, p, color, thickness, line_type, flags, shift);
    p0            = p;
    flags         = 2;
  }
}

static void
at_array_uint8_t_collect_polyedges(AtArray_uint8_t* array, AtVec2i* v, int count, AtVec4i** edges, int* count_edges, AtVec4i* color, AtLineType line_type, int shift, AtVec2i offset){
  int i, delta = offset.data[1] + (shift ? 1 << (shift - 1) : 0);
  AtVec2i pt0 = v[count-1], pt1;
  pt0.data[0] = (pt0.data[0] + offset.data[0]) << (XY_SHIFT - shift);
  pt0.data[1] = (pt0.data[1] + delta) >> shift;

  AtVec4i* edges2 = *edges;
  edges2 = realloc(edges2,(count + (*count_edges) + 1)*sizeof(AtVec4i));
  *edges = edges2;

  for( i = 0; i < count; i++, pt0 = pt1 )
  {
    AtVec2i t0, t1;
    AtVec4i edge;

    pt1 = v[i];
    pt1.data[0] = (pt1.data[0] + offset.data[0]) << (XY_SHIFT - shift);
    pt1.data[1] = (pt1.data[1] + delta) >> shift;

    if( line_type != AT_LINE_TYPE_AA)
    {
      t0.data[1] = pt0.data[1]; t1.data[1] = pt1.data[1];
      t0.data[0] = (pt0.data[0] + (XY_ONE >> 1)) >> XY_SHIFT;
      t1.data[0] = (pt1.data[0] + (XY_ONE >> 1)) >> XY_SHIFT;
      at_array_uint8_t_draw_line_no_shift(array, t0, t1, color, line_type );
    }
    else
    {
      t0.data[0] = pt0.data[0]; t1.data[0] = pt1.data[0];
      t0.data[1] = pt0.data[1] << XY_SHIFT;
      t1.data[1] = pt1.data[1] << XY_SHIFT;
      at_array_uint8_t_draw_line_antialiased(array, t0, t1, color );
    }

    if( pt0.data[1] == pt1.data[1] )
      continue;

    if( pt0.data[1] < pt1.data[1] )
    {
      edge.data[1] = pt0.data[1];
      edge.data[3] = pt1.data[1];
      edge.data[0] = pt0.data[0];
    }
    else
    {
      edge.data[1] = pt1.data[1];
      edge.data[3] = pt0.data[1];
      edge.data[0] = pt1.data[0];
    }
    edge.data[2] = (pt1.data[0] - pt0.data[0]) / (pt1.data[1] - pt0.data[1]);
    edges2[i] = edge;
    (*count_edges)++;
  }
}

static int
at_compare_edges(const void* elem1, const void* elem2){
  AtVec4i* e1 = (AtVec4i*) elem1;
  AtVec4i* e2 = (AtVec4i*) elem2;
  // First compare Y, after X, after X2 (dx)
  if     (e1->data[1]  < e2->data[1] ) return -1;
  else if(e1->data[1]  > e2->data[1] ) return  1;
  else if(e1->data[0]  < e2->data[0] ) return -1;
  else if(e1->data[0]  > e2->data[0] ) return  1;
  else if(e1->data[2] < e2->data[2]) return -1;
  else if(e1->data[2] > e2->data[2]) return  1;
  return 0;
}
static void
at_array_uint8_t_fill_edge_collection(AtArray_uint8_t* array, AtVec4i* edges, int num_edges, AtVec4i* color){
  (void) color;
  g_autofree uint64_t* array_size = at_array_get_size(array);
  AtVec4i tmp;
  int i, total = num_edges;
  //int y;
  //AtVec4i* e;
  int y_max = INT_MIN, x_max = INT_MIN, y_min = INT_MAX, x_min = INT_MAX;
  //int pix_size = array->dim < 3?1:array->size[2];

  if( total < 2 )
      return;

  for( i = 0; i < total; i++ )
  {
    AtVec4i* e1 = edges + i;
    // Determine x-coordinate of the end of the edge.
    // (This is not necessary x-coordinate of any vertex in the array.)
    int x1 = e1->data[0] + (e1->data[3] - e1->data[1]) * e1->data[2];
    y_min = min( y_min, e1->data[1] );
    y_max = max( y_max, e1->data[3] );
    x_min = min( x_min, e1->data[0] );
    x_max = max( x_max, e1->data[0] );
    x_min = min( x_min, x1 );
    x_max = max( x_max, x1 );
  }

  if( y_max < 0 || y_min >= array_size[0] || x_max < 0 || x_min >= (array_size[1]<<XY_SHIFT) )
    return;

  qsort(edges, num_edges, sizeof(AtVec4i), at_compare_edges);

  // start drawing
  tmp.data[1]      = INT_MAX;
  edges[num_edges] = tmp;

  //edges.push_back(tmp); // after this point we do not add
                        // any elements to edges, thus we can use pointers
  i = 0;
//  e = &edges[i];
//  y_max = min( y_max, size->height );

//  for( y = e->y0; y < y_max; y++ )
//  {
//    PolyEdge *last, *prelast, *keep_prelast;
//    int sort_flag = 0;
//    int draw = 0;
//    int clipline = y < 0;

//    prelast = &tmp;
//    last = tmp.next;
//    while( last || e->y0 == y )
//    {
//      if( last && last->y1 == y )
//      {
//        // exclude edge if y reachs its lower point
//        prelast->next = last->next;
//        last = last->next;
//        continue;
//      }
//      keep_prelast = prelast;
//      if( last && (e->y0 > y || last->x < e->x) )
//      {
//        // go to the next edge in active list
//        prelast = last;
//        last = last->next;
//      }
//      else if( i < total )
//      {
//        // insert new edge into active list if y reachs its upper point
//        prelast->next = e;
//        e->next = last;
//        prelast = e;
//        e = &edges[++i];
//      }
//      else
//          break;

//      if( draw )
//      {
//        if( !clipline )
//        {
//          // convert x's from fixed-point to image coordinates
//          uchar *timg = img.ptr(y);
//          int x1 = keep_prelast->x;
//          int x2 = prelast->x;

//          if( x1 > x2 )
//          {
//            int t = x1;

//            x1 = x2;
//            x2 = t;
//          }

//          x1 = (x1 + XY_ONE - 1) >> XY_SHIFT;
//          x2 = x2 >> XY_SHIFT;

//          // clip and draw the line
//          if( x1 < size.width && x2 >= 0 )
//          {
//            if( x1 < 0 )
//                x1 = 0;
//            if( x2 >= size.width )
//                x2 = size.width - 1;
//            IGRF_HLINE( timg, x1, x2, color, pix_size );
//          }
//        }
//        keep_prelast->x += keep_prelast->dx;
//        prelast->x += prelast->dx;
//      }
//      draw ^= 1;
//    }

//    // sort edges (using bubble sort)
//    keep_prelast = 0;

//    do
//    {
//      prelast = &tmp;
//      last = tmp.next;

//      while( last != keep_prelast && last->next != 0 )
//      {
//        PolyEdge *te = last->next;

//        // swap edges
//        if( last->x > te->x )
//        {
//          prelast->next = te;
//          last->next = te->next;
//          te->next = last;
//          prelast = te;
//          sort_flag = 1;
//        }
//        else
//        {
//          prelast = last;
//          last = te;
//        }
//      }
//      keep_prelast = prelast;
//    }
//    while( sort_flag && keep_prelast != tmp.next && keep_prelast != &tmp );
//  }
}

void
at_array_draw_ellipse(AtArray_uint8_t *array, AtVec2i center, AtVec2i axes, double angle, double start_angle, double end_angle, AtVec4i *color, int thickness, int line_type, int shift){
  if( line_type == AT_LINE_TYPE_AA)
      line_type = AT_LINE_TYPE_8;

  int _angle       = round(angle);
  int _start_angle = round(start_angle);
  int _end_angle   = round(end_angle);
  center.data[0]    <<= XY_SHIFT - shift;
  center.data[1]    <<= XY_SHIFT - shift;
  axes.data[0] <<= XY_SHIFT - shift;
  axes.data[1] <<= XY_SHIFT - shift;

  at_array_uint8_t_draw_ellipse_ex(array, center, axes, _angle, _start_angle,
             _end_angle, color, thickness, line_type );
}

void
at_ellipse_to_poly(AtVec2i center, AtVec2i axes, int angle, int arc_start, int arc_end, int delta, AtVec2i** pts, int *count){
  float       alpha, beta;
  double      size_a = axes.data[0], size_b = axes.data[1];
  double      cx     = center.data[0], cy = center.data[1];
  AtVec2i prevPt = {INT_MIN,INT_MIN};
  int         i;

  while( angle < 0 )
    angle += 360;
  while( angle > 360 )
    angle -= 360;

  if( arc_start > arc_end ){
    i = arc_start;
    arc_start = arc_end;
    arc_end = i;
  }
  while( arc_start < 0 ){
    arc_start += 360;
    arc_end += 360;
  }
  while( arc_end > 360 ){
    arc_end -= 360;
    arc_start -= 360;
  }
  if( arc_end - arc_start > 360 ){
    arc_start = 0;
    arc_end = 360;
  }
  double angle2 = (double) angle;
  sincos_custom(angle2, &alpha, &beta );

  int cur_length = 1024;

  AtVec2i* pts2 = malloc(cur_length * sizeof(AtVec2i));
  *pts = pts2;

  for(i = arc_start; i < arc_end + delta; i += delta ){
    double x, y;
    angle = i;
    if( angle > arc_end )
      angle = arc_end;
    if( angle < 0 )
      angle += 360;

    x = size_a * SinTable[450-angle];
    y = size_b * SinTable[angle];
    AtVec2i pt;
    pt.data[0] = round( cx + x * alpha - y * beta );
    pt.data[1] = round( cy + x * beta + y * alpha );
    if( pt.data[0] != prevPt.data[0] || pt.data[1] != prevPt.data[1] ){
      // Resize list whether not sufficient
      if(*count == cur_length){
        cur_length <<= 1;
        pts2 = realloc(pts,cur_length * sizeof(AtVec2i));
        *pts = pts2;
      }
      pts[*count]->data[0] = pt.data[0];
      pts[*count]->data[1] = pt.data[1];
      (*count)++;
    }
  }


  // If there are no points, it's a zero-size polygon
  if( *count == 1) {
    *count    = 2;
    pts2 = realloc(pts,2 * sizeof(AtVec2i));
    pts2[0].data[0] = center.data[0];pts2[0].data[1] = center.data[1];
    pts2[1].data[0] = center.data[0];pts2[1].data[1] = center.data[1];

  }else{
    pts2 = realloc(pts,*count * sizeof(AtVec2i));
  }
  *pts = pts2;
}

static void
at_array_uint8_t_draw_ellipse_ex      (AtArray_uint8_t* array, AtVec2i center, AtVec2i axes, int angle, int arc_start, int arc_end, AtVec4i* color, int thickness, AtLineType line_type){
  axes.data[0] = abs(axes.data[0]), axes.data[1] = abs(axes.data[1]);
  int delta = (max(axes.data[0],axes.data[1])+(XY_ONE>>1))>>XY_SHIFT;
  delta = delta < 3 ? 90 : delta < 10 ? 30 : delta < 15 ? 18 : 5;

  AtVec2i* v = NULL;
  int count;
  at_ellipse_to_poly( center, axes, angle, arc_start, arc_end, delta, &v, &count );

  if( thickness >= 0 )
    at_array_uint8_t_draw_polyline( array, &v[0], count, 0, color, thickness, line_type, XY_SHIFT );
  else if( arc_end - arc_start >= 360 )
    at_array_uint8_t_fill_convex_poly( array, &v[0], count, color, line_type, XY_SHIFT );
  else
  {
    v = realloc(v, (count + 1) * sizeof(AtVec2i));
    v[count] = center;
    AtVec4i* edges;
    int count_edges = 0;
    AtVec2i offset = {0,0};
    at_array_uint8_t_collect_polyedges( array,  &v[0], count+1, &edges, &count_edges, color, line_type, XY_SHIFT, offset);
    at_array_uint8_t_fill_edge_collection( array, edges, count_edges, color);
  }
  free(v);
}

static inline uint64_t at_rectangle_area(AtVec4i rect){
  return rect.data[2] * rect.data[3];
}
static AtVec2i at_rectangle_top_left(AtVec4i rect){
  AtVec2i point = {rect.data[0], rect.data[1]};
  return point;
}
static AtVec2i at_rectangle_bottom_right(AtVec4i rect){
  AtVec2i point = {rect.data[0] + rect.data[2], rect.data[1] + rect.data[3]};
  return point;
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

AtArray_uint8_t*
at_cvt_color(AtArray_uint8_t* image, AtColorFormat from, AtColorFormat to){
  if(from == to) return image;

  AtArray_uint8_t* image_cvt = NULL;
  // Get some image info
  uint8_t* data = at_array_uint8_t_get_data(image);
  g_autofree uint64_t* size = at_array_get_size(image);

  uint64_t sizeimage[3] = {size[0],size[1],4};
  switch(to){
  case AT_COLOR_GRAY: sizeimage[2] = 1;break;
  case AT_COLOR_RGB:  sizeimage[2] = 3;break;
  case AT_COLOR_ARGB:
  case AT_COLOR_BGRA: sizeimage[2] = 4;break;
  }

  at_array_zeros(&image_cvt, 3, sizeimage);

  uint8_t* data_image = at_array_uint8_t_get_data(image_cvt);
  uint64_t n, k, offset_image, offset;

  if(from == AT_COLOR_GRAY && to == AT_COLOR_BGRA){
    for(n = 0; n < size[0]*size[1]; n++){
      offset_image = n*sizeimage[2];
      offset = n*size[2];
      for(k = 0; k < 3; k++){
        data_image[offset_image+k] = data[offset];
      }
      data_image[offset_image+k] = 255;
    }
  }
  else if(from == AT_COLOR_RGB && to == AT_COLOR_BGRA){
    for(n = 0; n < size[0]*size[1]; n++){
      offset_image = n*sizeimage[2];
      offset = n*size[2];
      for(k = 0; k < size[2]; k++){
        data_image[offset_image+k] = data[offset+(2-k)];
      }
    }
  }
  else if(from == AT_COLOR_RGB && to == AT_COLOR_GRAY){
    for(n = 0; n < size[0]*size[1]; n++){
      offset_image = n*sizeimage[2];
      offset = n*size[2];
      data_image[offset_image] = (int)((double)data[offset]*0.299 + (double)data[offset+1]*0.587 + (double)data[offset+2]*0.114);
    }
  }
  return image_cvt;
}

AtArray_uint8_t*
at_array_uint8_t_scale(AtArray_uint8_t* image, double scale, AtInterpolationMode interpolation){
  AtArray_uint8_t* output = NULL;
  g_autofree uint64_t* size = at_array_get_size(image);
  uint64_t new_size[3] = {size[0]*scale,size[1]*scale,size[2]};
  at_array_new(&output, 3, new_size);
  uint8_t* new_data = at_array_uint8_t_get_data(output);
  uint8_t* data = at_array_uint8_t_get_data(image);
  uint64_t x,y,pos_x,pos_y,offset,new_offset;
  for(y = 0; y < new_size[0]; y++){
    for(x = 0; x < new_size[1]; x++){
      pos_y = (uint64_t)(y/scale);
      pos_x = (uint64_t)(x/scale);
      offset = (pos_y * size[1] + pos_x) << 2;
      new_offset = (y * new_size[1] + x) << 2;

      // The ARGB colors of the pixel
      new_data[new_offset] = data[offset];
      new_data[new_offset+1] = data[offset+1];
      new_data[new_offset+2] = data[offset+2];
      new_data[new_offset+3] = data[offset+3];
    }
  }
  return output;
}

void
at_array_uint8_t_draw_line(AtArray_uint8_t* array,
                           AtVec2i p0,
                           AtVec2i p1,
                           AtVec4i* color,
                           int thickness,
                           AtLineType line_type,
                           int shift){
  at_array_uint8_t_draw_line_thick(array,p0,p1,color,thickness,line_type,3,shift);
}

uint8_t
at_clip_line(AtVec2i rectangle, AtVec2i *p1, AtVec2i *p2){
  int64_t x1, y1, x2, y2;
  int c1, c2;
  int64_t right = rectangle.data[0]-1, bottom = rectangle.data[1]-1;

  if( rectangle.data[0] <= 0 || rectangle.data[1] <= 0 )
     return 0;

  x1 = p1->data[0]; y1 = p1->data[1]; x2 = p2->data[0]; y2 = p2->data[1];
  c1 = (x1 < 0) + ((x1 > right) << 1) + ((y1 < 0) << 2) + ((y1 > bottom) << 3);
  c2 = (x2 < 0) + ((x2 > right) << 1) + ((y2 < 0) << 2) + ((y2 > bottom) << 3);

  if( (c1 & c2) == 0 && (c1 | c2) != 0 )
  {
    int64_t a;
    if( c1 & 12 )
    {
      a = c1 < 8 ? 0 : bottom;
      x1 +=  (a - y1) * (x2 - x1) / (y2 - y1);
      y1 = a;
      c1 = (x1 < 0) + ((x1 > right) << 1);
    }
    if( c2 & 12 )
    {
      a = c2 < 8 ? 0 : bottom;
      x2 += (a - y2) * (x2 - x1) / (y2 - y1);
      y2 = a;
      c2 = (x2 < 0) + ((x2 > right) << 1);
    }
    if( (c1 & c2) == 0 && (c1 | c2) != 0 )
    {
      if( c1 )
      {
        a = c1 == 1 ? 0 : right;
        y1 += (a - x1) * (y2 - y1) / (x2 - x1);
        x1 = a;
        c1 = 0;
      }
      if( c2 )
      {
        a = c2 == 1 ? 0 : right;
        y2 += (a - x2) * (y2 - y1) / (x2 - x1);
        x2 = a;
        c2 = 0;
      }
    }

    //assert( (c1 & c2) != 0 || (x1 | y1 | x2 | y2) >= 0 );

    p1->data[0] = (int)x1;
    p1->data[1] = (int)y1;
    p2->data[0] = (int)x2;
    p2->data[1] = (int)y2;
  }

  return (c1 | c2) == 0;
}
void
at_array_uint8_t_draw_circle(AtArray_uint8_t *array, AtVec2i center, int radius, AtVec4i *color, int thickness, AtLineType line_type, int shift){
  if( line_type == AT_LINE_TYPE_AA)
      line_type = AT_LINE_TYPE_8;

  if( thickness > 1 || line_type == AT_LINE_TYPE_AA || shift > 0 )
  {
    center.data[0] <<= XY_SHIFT - shift;
    center.data[1] <<= XY_SHIFT - shift;
    radius   <<= XY_SHIFT - shift;
    AtVec2i radiuses = {radius,radius};
    at_array_uint8_t_draw_ellipse_ex(array, center, radiuses,
               0, 0, 360, color, thickness, line_type );
  }
  else
      at_array_uint8_t_draw_circle_direct(array, center, radius, color, thickness < 0 );
}
void
at_array_uint8_t_draw_rectangle(AtArray_uint8_t *array, AtVec4i rect, AtVec4i *color, int thickness, AtLineType line_type, int shift){
  if(at_rectangle_area(rect) > 0 ){
    AtVec2i top_left     = at_rectangle_top_left(rect);
    AtVec2i point        = {1<<shift,1<<shift};
    AtVec2i bottom_right = at_rectangle_bottom_right(rect);
    bottom_right.data[0] -= point.data[0];
    bottom_right.data[1] -= point.data[1];
    at_array_uint8_t_draw_rectangle_2( array, top_left, bottom_right, color, thickness, line_type, shift );
  }
}
void
at_array_uint8_t_draw_rectangle_2(AtArray_uint8_t* array, AtVec2i top_left, AtVec2i bottom_right, AtVec4i* color, int thickness, AtLineType line_type, int shift){
  if( line_type == AT_LINE_TYPE_AA)
      line_type = AT_LINE_TYPE_8;
  AtVec2i pt[4];
  pt[0]   = top_left;
  pt[1].data[0] = bottom_right.data[0];
  pt[1].data[1] = top_left.data[1];
  pt[2]   = bottom_right;
  pt[3].data[0] = top_left.data[0];
  pt[3].data[1] = bottom_right.data[1];

  if( thickness >= 0 )
    at_array_uint8_t_draw_polyline(array, pt, 4, 1, color, thickness, line_type, shift );
  else
    at_array_uint8_t_fill_convex_poly(array, pt, 4, color, line_type, shift );
}
