// graphin.cpp : Defines the entry point for the DLL application.
//

#include "primitives.h"
#include "objects.h"
#include "graphin.h"

#include <malloc.h>

#if defined(__GNUC__)
  #include <alloca.h>
#endif

#undef GRAPHIN_API
#define GRAPHIN_API


// this is an example of an exported function.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL image_create( unsigned int width, unsigned int height, HIMG* pout_img )
{
  if( !pout_img )
    return GRAPHIN_BAD_PARAM;
  image* p = new image(width, height);
  if( p )
  {
    p->add_ref();
    *pout_img = p;
    return GRAPHIN_OK;
  }
  return GRAPHIN_PANIC;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL image_release( HIMG himg )
{
  if(himg) himg->release();
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL image_get_info(
   HIMG himg,
   BYTE** data,
   unsigned int* width,
   unsigned int* height,
   int* stride,
   unsigned int* pixel_format)
{
  if( !himg ) return GRAPHIN_BAD_PARAM;
  if( data ) *data = himg->pmap.buf();
  if( width ) *width = himg->pmap.width();
  if( height ) *height = himg->pmap.height();
  if( stride ) *stride = himg->pmap.stride();
  if( pixel_format ) *pixel_format = 0; // TODODODODO
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT  GRAPHIN_CALL image_blit( HPLATFORMGFX dst, int dst_x, int dst_y,
    HIMG src, int src_x, int src_y,
    int width, int height, bool blend )
{
  if( !src || !dst) return GRAPHIN_BAD_PARAM;
#if defined(_WINDOWS)
  RECT dstr = { dst_x, dst_y, dst_x + width, dst_y + height };
  RECT srcr = { src_x, src_y, src_x + width, src_y + height };
  if( blend )
    src->pmap.blend(dst, &dstr, &srcr);
  else
    src->pmap.draw(dst, &dstr, &srcr);
#elif defined(XWINDOW)
  agg::rect_i dstr( dst_x, dst_y, dst_x + width, dst_y + height );
  agg::rect_i srcr( src_x, src_y, src_x + width, src_y + height );
  //if( blend )
  //  src->pmap.blend(dst->d, dst->w, dst->gc, &dstr, &srcr);
  //else
  //  src->pmap.draw(dst->d, dst->w, dst->gc, &dstr, &srcr);
#endif

  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL graphics_create(HIMG img, HGFX* pout_gfx )
{
  if( !pout_gfx )
    return GRAPHIN_BAD_PARAM;
  graphics* p = new graphics(img);
  if(p)
  {
    p->add_ref();
    *pout_gfx = p;
    return GRAPHIN_OK;
  }
  return GRAPHIN_PANIC;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL graphics_blend_mode( HGFX gfx, GRAPHIN_BLEND_MODE mode )
{
  if(gfx == 0 || mode < 0 || mode >= GBlend_count)
  {
    return GRAPHIN_BAD_PARAM;
  }

  gfx->blendMode( (Agg2D::BlendMode)(int)mode );

  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_BLEND_MODE GRAPHIN_CALL graphics_blend_mode_get( HGFX gfx )
{
  return gfx ? (GRAPHIN_BLEND_MODE)(int)gfx->blendMode(): GBlend_count;
}


GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL graphics_gamma( HGFX gfx, REAL gamma_value )
{
  if(gfx)
  {
    gfx->antiAliasGamma(gamma_value);
    return GRAPHIN_OK;
  }

  return GRAPHIN_BAD_PARAM;
}

GRAPHIN_API REAL GRAPHIN_CALL graphics_gamma_get( HGFX gfx )
{
  if(gfx)
  {
    return gfx->antiAliasGamma();
  }

  return 0.0;
}

// dahed lines

// add one dash
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL graphics_dashes_add( HGFX gfx, POS dash_len, POS gap_len )
{
  if(gfx)
  {
    gfx->dashAdd( dash_len, gap_len );
    return GRAPHIN_OK;
  }

  return GRAPHIN_BAD_PARAM;
}

// remove all dashes, now line-style is solid
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL graphics_dashes_clear( HGFX gfx )
{
  if(gfx)
  {
    gfx->dashClear();
    return GRAPHIN_OK;
  }

  return GRAPHIN_BAD_PARAM;
}


// set dash start offset
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL graphics_dashes_start( HGFX gfx, POS dash_start )
{
  if( gfx )
  {
    gfx->dashStart(dash_start);
    return GRAPHIN_OK;
  }

  return GRAPHIN_BAD_PARAM;
}

// set dash start offset
GRAPHIN_API POS GRAPHIN_CALL graphics_dashes_start_get( HGFX gfx )
{
  return gfx ? gfx->dashStart() : 0;
}



GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL graphics_release( HGFX hgfx )
{
  if(hgfx) hgfx->release();
  return GRAPHIN_OK;
}

// Draws line from x1,y1 to x2,y2 using current lineColor and lineGradient.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_line ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2 )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->line(x1,y1,x2,y2);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_arrow_bone ( HGFX hgfx, POS x_from, POS y_from, POS x_to, POS y_to, POS sz, ANGLE angle )
{
  bool res = hgfx ? hgfx->arrow_bone( x_from, y_from, x_to, y_to, sz, angle ) : false;

  return res ? GRAPHIN_OK : GRAPHIN_BAD_PARAM;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_arrow ( HGFX hgfx, POS x_from, POS y_from, POS x_to, POS y_to, POS sz, ANGLE angle, REAL offset )
{
  bool res = hgfx ? hgfx->arrow( x_from, y_from, x_to, y_to, sz, angle, offset ) : false;

  return res ? GRAPHIN_OK : GRAPHIN_BAD_PARAM;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_triangle ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2, POS x3, POS y3 )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->triangle(x1,y1,x2,y2,x3,y3);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_rectangle ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2 )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->rectangle(x1,y1,x2,y2);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_rounded_rectangle ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2, POS rx, POS ry)
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->roundedRect(x1,y1,x2,y2,rx,ry);
  return GRAPHIN_OK;
}

inline COLOR rgbt( unsigned r, unsigned g, unsigned b, unsigned a ) { return ((a & 0xff) << 24) | ((b & 0xff) << 16) | ((g & 0xff) << 8) | (r & 0xff); }
inline unsigned r( COLOR c ) { return c & 0xff; }
inline unsigned g( COLOR c ) { return (c >> 8)  & 0xff; }
inline unsigned b( COLOR c ) { return (c >> 16) & 0xff; }
inline unsigned a( COLOR c ) { return (c >> 24) & 0xff; } // alpha
inline unsigned t( COLOR c ) { return 255 - a(c); }       // transparency

inline Agg2D::Color AGG_COLOR(COLOR c)
{
  return Agg2D::Color( r(c), g(c), b(c), a(c) );
}

GRAPHIN_API COLOR GRAPHIN_CALL
        graphics_rgbt(unsigned int red, unsigned int green, unsigned int blue, unsigned int transparency)
{
  return rgbt(red,green,blue,255-transparency);
}


// Draws circle or ellipse using current lineColor/lineGradient and fillColor/fillGradient.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_ellipse ( HGFX hgfx, POS x, POS y, POS rx, POS ry )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->ellipse(x,y,rx,ry);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_arc ( HGFX hgfx, POS x, POS y, POS rx, POS ry, ANGLE start, ANGLE sweep )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->arc(x, y, rx, ry, start, sweep);
  return GRAPHIN_OK;
}


// Draws star.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_star ( HGFX hgfx, POS x, POS y, POS r1, POS r2, ANGLE start, unsigned rays )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->star(x, y, r1, r2, start, rays);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_polygon ( HGFX hgfx, POS* xy, unsigned int num_points )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->polygon(xy, num_points);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_polyline ( HGFX hgfx, POS* xy, unsigned int num_points )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->polyline(xy, num_points);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_open_path ( HGFX hgfx )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->resetPath();
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_move_to ( HGFX hgfx, POS x, POS y, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if( rel )
    hgfx->moveRel(x, y);
  else
    hgfx->moveTo(x, y);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_line_to ( HGFX hgfx, POS x, POS y, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if( rel )
    hgfx->lineRel(x, y);
  else
    hgfx->lineTo(x, y);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_hline_to ( HGFX hgfx, POS x, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if( rel )
    hgfx->horLineRel(x);
  else
    hgfx->horLineTo(x);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_vline_to ( HGFX hgfx, POS y, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if( rel )
    hgfx->verLineRel(y);
  else
    hgfx->verLineTo(y);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_arc_to ( HGFX hgfx, POS x, POS y, ANGLE angle, POS rx, POS ry, bool is_large_arc, bool sweep_flag, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if( rel )
    hgfx->arcRel(rx, ry, angle, is_large_arc, sweep_flag, x, y);
  else
    hgfx->arcTo(rx, ry, angle, is_large_arc, sweep_flag, x, y);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_ellipse_to ( HGFX hgfx, POS x, POS y, POS rx, POS ry, bool clockwise )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->addEllipse(x,y,rx,ry, clockwise? Agg2D::CW: Agg2D::CCW);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_quadratic_curve_to ( HGFX hgfx, POS xc, POS yc, POS x, POS y, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if(rel)
    hgfx->quadricCurveRel(xc, yc, x, y);
  else
    hgfx->quadricCurveTo(xc, yc, x, y);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_bezier_curve_to ( HGFX hgfx, POS xc1, POS yc1, POS xc2, POS yc2, POS x, POS y, bool rel )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if(rel)
    hgfx->cubicCurveRel(xc1, yc1, xc2, yc2, x, y);
  else
    hgfx->cubicCurveTo(xc1, yc1, xc2, yc2, x, y);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_close_path ( HGFX hgfx )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->closePolygon();
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_draw_path ( HGFX hgfx, DRAW_PATH_MODE dpm )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->drawPath(Agg2D::DrawPathFlag(dpm));
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_rotate ( HGFX hgfx, ANGLE radians, POS* cx, POS* cy )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if(cx && cy)
    hgfx->rotate(radians, *cx, *cy);
  else
    hgfx->rotate(radians);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_translate ( HGFX hgfx, POS cx, POS cy )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->translate(cx, cy);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_scale ( HGFX hgfx, double x, double y )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->scale(x, y);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_skew ( HGFX hgfx, double sx, double sy )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->skew(sx, sy);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_transform ( HGFX hgfx, POS m11, POS m12, POS m21, POS m22, POS dx, POS dy )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;

  Agg2D::Affine m(m11, m12, m21, m22, dx, dy);
  hgfx->matrix(m);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_state_save ( HGFX hgfx )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->save_state();
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        graphics_state_restore ( HGFX hgfx )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  return hgfx->restore_state()?
    GRAPHIN_OK:
    GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_line_width ( HGFX hgfx, DIM width )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->lineWidth(width);
  if( width == 0.0 )
    hgfx->noLine();
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_line_color ( HGFX hgfx, COLOR color )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  //if( t(c) == 0xFF000000 )
  //  hgfx->noLine();
  //else
  //  hgfx->lineColor(AGG_COLOR(c));
  hgfx->lineColor(AGG_COLOR(color));
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_fill_color ( HGFX hgfx, COLOR color )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if( t(color) == 0xFF )
    hgfx->noFill();
  else
    hgfx->fillColor(AGG_COLOR(color));
  return GRAPHIN_OK;
}

// image for pattern fills
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_fill_pattern ( HGFX hgfx, HIMG pattern, unsigned opacity, int offset_x, int offset_y )
{
    if(!hgfx||!pattern)
      return GRAPHIN_BAD_PARAM;

    hgfx->fillPattern(*pattern, opacity, offset_x, offset_y);

    return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_line_linear_gradient( HGFX hgfx,
       POS x1, POS y1, POS x2, POS y2, COLOR color1, COLOR color2, POS profile )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;

  hgfx->lineLinearGradient(x1, y1, x2, y2, AGG_COLOR(color1), AGG_COLOR(color2), profile);

  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_fill_linear_gradient( HGFX hgfx,
       POS x1, POS y1, POS x2, POS y2, COLOR color1, COLOR color2, POS profile )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;

  hgfx->fillLinearGradient(x1, y1, x2, y2, AGG_COLOR(color1), AGG_COLOR(color2), profile);

  return GRAPHIN_OK;
}

// setup parameters of line gradient radial fills.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_line_radial_gradient( HGFX hgfx, POS x, POS y, DIM r, COLOR color1, COLOR color2, double profile )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;

  hgfx->lineRadialGradient(x, y, r, AGG_COLOR(color1), AGG_COLOR(color2), profile);

  return GRAPHIN_OK;
}

// setup parameters of gradient radial fills.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_fill_radial_gradient( HGFX hgfx, POS x, POS y, DIM r, COLOR color1, COLOR color2, double profile )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;

  hgfx->fillRadialGradient(x, y, r, AGG_COLOR(color1), AGG_COLOR(color2), profile);

  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_fill_mode( HGFX hgfx, bool even_odd /* false - fill_non_zero */ )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;

  hgfx->fillEvenOdd( even_odd );

  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_font( HGFX hgfx, const char* name, DIM size, bool bold, bool italic, ANGLE angle)
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->font( name, size, bold, italic, Agg2D::VectorFontCache, angle);
  return GRAPHIN_OK;

}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_text( HGFX hgfx, POS x, POS y, const wchar_t* text, unsigned int text_length)
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->text( x, y, text, text_length );
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_text_width( HGFX hgfx, const wchar_t* text, unsigned int text_length, DIM* out_width)
{
  if(!hgfx || !out_width)
    return GRAPHIN_BAD_PARAM;
  *out_width = hgfx->textWidth( text, text_length );
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_text_alignment( HGFX hgfx, TEXT_ALIGNMENT x, TEXT_ALIGNMENT y)
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  hgfx->textAlignment( Agg2D::TextAlignment(x), Agg2D::TextAlignment(y) );
  return GRAPHIN_OK;
}

// returns height and ascent of the font.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_font_metrics( HGFX hgfx, DIM* out_height, DIM* out_ascent )
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;
  if(out_height)
    *out_height = hgfx->fontHeight();
  if(out_ascent)
    *out_ascent = hgfx->fontAscent();
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_world_to_screen ( HGFX hgfx, POS* x, POS* y)
{
  if(!hgfx || !x)
    return GRAPHIN_BAD_PARAM;
  if(y)
    hgfx->worldToScreen(*x,*y);
  else
    *x = hgfx->worldToScreen(*x);
  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_screen_to_world ( HGFX hgfx, POS* x, POS* y)
{
  if(!hgfx || !x)
    return GRAPHIN_BAD_PARAM;
  if(y)
    hgfx->screenToWorld(*x,*y);
  else
    *x = hgfx->screenToWorld(*x);
  return GRAPHIN_OK;
}


#include "imageio.h"

bool image_ctor(void* pctorPrm, unsigned int width, unsigned int height, BYTE** rowPtrs)
{
  image* pim = new image(width, height);
  if(!pim) return 0;

  int   stride = pim->pmap.stride();
  BYTE* ptr    = pim->pmap.buf();
  if( stride < 0 )
    ptr += abs(stride) * (height - 1);  

  for(unsigned n = 0; n < height; ++n, ptr += stride)
    rowPtrs[n] = ptr;

  handle<image>* him = (handle<image>*)pctorPrm;
  *him = pim;
  return true;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        image_load( const BYTE* bytes, unsigned int num_bytes, HIMG* pout_img ) // load png/jpeg/etc. image from stream of bytes
{
  handle<image> img;
  if( !DecodeImage(&image_ctor, &img, const_cast<BYTE*>(bytes), num_bytes) )
    return GRAPHIN_FAILURE;

  *pout_img = img.detach();

  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL // save png/jpeg/etc. image to stream of bytes
        image_save( HIMG himg,
        image_write_function* pfn, void* prm, /* function and its param passed "as is" */
        unsigned bpp /*24,32 if alpha needed*/,
        unsigned type /* 0 - png, 1 - jpg*/,
        unsigned quality /*  only for jpeg, 10 - 100 */ )
{
  if(!himg || !pfn)
    return GRAPHIN_BAD_PARAM;

  int   stride = himg->pmap.stride();
  BYTE* ptr    = himg->pmap.buf();
  unsigned width = himg->pmap.width();
  unsigned height = himg->pmap.height();

  BYTE** rows = (BYTE**)alloca( sizeof(BYTE*) * height );
  if( !rows )
    return GRAPHIN_FAILURE;

  for(unsigned n = 0; n < height; ++n, ptr += stride)
    rows[n] = ptr;

  return EncodeImage(pfn,prm,width,height,rows,bpp,quality,type)?
    GRAPHIN_OK: GRAPHIN_FAILURE;
}


GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_draw_image ( HGFX hgfx, HIMG himg, POS x, POS y,
                            DIM* w, DIM* h, unsigned* ix, unsigned* iy, unsigned* iw, unsigned* ih )
{
  if(!hgfx || !himg)
    return GRAPHIN_BAD_PARAM;

  double unit = 0;

  if(!w && !h)
    unit = hgfx->screenToWorld(1.0);

  unsigned src_x1 = ix? *ix : 0;
  unsigned src_y1 = iy? *iy : 0;
  unsigned src_x2 = src_x1 + iw? *iw : himg->pmap.width();
  unsigned src_y2 = src_y1 + ih? *ih : himg->pmap.height();

  DIM dst_width = w? *w : unit * (src_x2 - src_x1);
  DIM dst_height = h? *h : unit * (src_y2 - src_y1);

  hgfx->transformImage(*himg,
                   limit(src_x1, (unsigned)0, (unsigned)himg->pmap.width()),
                   limit(src_y1, (unsigned)0, (unsigned)himg->pmap.height()),
                   limit(src_x2, (unsigned)0, (unsigned)himg->pmap.width()),
                   limit(src_y2, (unsigned)0, (unsigned)himg->pmap.height()),
                   x,y, x + dst_width, y + dst_height);

  return GRAPHIN_OK;

}

// blits image bits onto underlying pixel buffer.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_blit_image ( HGFX hgfx, HIMG himg, POS x, POS y, unsigned* ix, unsigned* iy, unsigned* iw, unsigned* ih )
{
  if(!hgfx || !himg)
    return GRAPHIN_BAD_PARAM;

  unsigned src_x1 = ix? *ix : 0;
  unsigned src_y1 = iy? *iy : 0;
  unsigned src_x2 = src_x1 + iw? *iw : himg->pmap.width();
  unsigned src_y2 = src_y1 + ih? *ih : himg->pmap.height();

  hgfx->copyImage(*himg,
                   limit(src_x1, (unsigned)0, (unsigned)himg->pmap.width()),
                   limit(src_y1, (unsigned)0, (unsigned)himg->pmap.height()),
                   limit(src_x2, (unsigned)0, (unsigned)himg->pmap.width()),
                   limit(src_y2, (unsigned)0, (unsigned)himg->pmap.height()),
                   x, y);

  return GRAPHIN_OK;
}

// blits image bits onto underlying pixel buffer.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_blend_image ( HGFX hgfx, HIMG himg, POS x, POS y, unsigned opacity, unsigned* ix, unsigned* iy, unsigned* iw, unsigned* ih)
{
  if(!hgfx || !himg)
    return GRAPHIN_BAD_PARAM;

  unsigned src_x1 = ix? *ix : 0;
  unsigned src_y1 = iy? *iy : 0;
  unsigned src_x2 = src_x1 + iw? *iw : himg->pmap.width();
  unsigned src_y2 = src_y1 + ih? *ih : himg->pmap.height();

  hgfx->blendImage(*himg,
                   limit(src_x1, (unsigned)0, (unsigned)himg->pmap.width()),
                   limit(src_y1, (unsigned)0, (unsigned)himg->pmap.height()),
                   limit(src_x2, (unsigned)0, (unsigned)himg->pmap.width()),
                   limit(src_y2, (unsigned)0, (unsigned)himg->pmap.height()),
                   x, y, opacity);

  return GRAPHIN_OK;
}


GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_set_clip_box ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2)
{
  if(!hgfx)
    return GRAPHIN_BAD_PARAM;

  hgfx->clipBox(x1,y1,x2,y2);

  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_get_clip_box ( HGFX hgfx, POS* x1, POS* y1, POS* x2, POS* y2)
{
  if(!hgfx || !x1 || !y1 || !x2 || !y2 )
    return GRAPHIN_BAD_PARAM;

  Agg2D::RectD rc = hgfx->clipBox();
  *x1 = rc.x1;
  *x2 = rc.x2;
  *y1 = rc.y1;
  *y2 = rc.y2;

  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_is_in_clip_box ( HGFX hgfx, POS x, POS y, bool* out_yes)
{
  if(!hgfx || !out_yes)
    return GRAPHIN_BAD_PARAM;

  *out_yes = hgfx->inBox(x, y);

  return GRAPHIN_OK;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
      graphics_rect_is_visible ( HGFX hgfx, POS x1, POS y1, POS x2, POS y2, bool *out_yes)
{
  if(!hgfx || !out_yes)
    return GRAPHIN_BAD_PARAM;

  Agg2D::RectD rc = hgfx->clipBox();

  hgfx->worldToScreen(x1,y1);
  hgfx->worldToScreen(x2,y2);

  Agg2D::RectD rr(x1,y1,x2,y2);

  *out_yes =
        ( max( rr.x1, rc.x1 ) <= min( rr.x2, rc.x2 ) )
     && ( max( rr.y1, rc.y1 ) <= min( rr.y2, rc.y2 ) );

  return GRAPHIN_OK;
}


