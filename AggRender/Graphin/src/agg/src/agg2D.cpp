//----------------------------------------------------------------------------
// Agg2D - Version 1.0
// Based on Anti-Grain Geometry
// Copyright (C) 2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//
//  25 Jan 2007 - Ported to AGG 2.4 Jerry Evans (jerry@novadsp.com)
//
//----------------------------------------------------------------------------

#include "agg2D.h"
#include "agg_bounding_rect.h"
#include "agg_blur.h"
#include <assert.h>
//-----------------------------------------------------------------------------
static const real g_approxScale = 2.0f;
//-----------------------------------------------------------------------------
class gradient_opacity
{
public:
  //---------------------------------------------------------------------
  typedef Agg2D::Color             color_type;
  typedef Agg2D::gradient_lut_type gradient_lut_type;
  //---------------------------------------------------------------------
  gradient_opacity(const gradient_lut_type& lut, real opacity)
    :
        m_lut(lut),
        m_opacity(unsigned(opacity * color_type::base_scale + 0.5f))
    {}
  //---------------------------------------------------------------------
    static unsigned size()
  {
    return gradient_lut_type::size();
  }
  //---------------------------------------------------------------------
  color_type operator[] (unsigned i) const
    {
        color_type c = m_lut[i];
        c.a = (color_type::value_type)
        ((c.a * m_opacity) >> color_type::base_shift);
        return c;
    }
  //---------------------------------------------------------------------
private:
  const gradient_lut_type& m_lut;
    unsigned                 m_opacity;
};
//-----------------------------------------------------------------------------
Agg2D::~Agg2D()
{
#if !defined( UNDER_CE )
#ifndef AGG2D_USE_FREETYPE
    ::ReleaseDC(0, m_fontDC);
#endif
#endif
}

Agg2D::Agg2D() :
    m_rbuf(),
    m_pixFormat(m_rbuf),
    m_pixFormatComp(m_rbuf),
    m_pixFormatPre(m_rbuf),
    m_pixFormatCompPre(m_rbuf),
    m_renBase(m_pixFormat),
    m_renBaseComp(m_pixFormatComp),
    m_renBasePre(m_pixFormatPre),
    m_renBaseCompPre(m_pixFormatCompPre),
    m_renSolid(m_renBase),
    m_renSolidComp(m_renBaseComp),

    m_allocator(),
    m_clipBox(0,0,0,0),

    m_blendMode(BlendAlpha),
    m_imageBlendMode(BlendDst),
    m_imageBlendColor(0,0,0),

    m_scanline(),
    m_rasterizer(),

    m_masterAlpha(1.0f),
    m_antiAliasGamma(1.0f),

    m_fillColor(255, 255, 255),
    m_lineColor(0,   0,   0),
    m_fillGradient(),
    m_lineGradient(),

    m_lineCap(LineCapRound),
    m_lineJoin(RoundJoin),

    m_fillGradientFlag(Solid),
    m_lineGradientFlag(Solid),
    m_fillGradientMatrix(),
    m_lineGradientMatrix(),
    m_fillGradientD1(0.0f),
    m_lineGradientD1(0.0f),
    m_fillGradientD2(100.0f),
    m_lineGradientD2(100.0f),

    m_fillGradientSpreadMethod(spreadMethod_pad),
    m_lineGradientSpreadMethod(spreadMethod_pad),

    m_fillGradientOpacity(1.0f),
    m_lineGradientOpacity(1.0f),

    m_textAngle(0.0f),
    m_textAlignX(AlignLeft),
    m_textAlignY(AlignBottom),
    m_textHints(true),
    m_fontHeight(0.0f),
    m_fontAscent(0.0f),
    m_fontDescent(0.0f),
    m_fontCacheType(RasterFontCache),

    m_imageFilter(Bilinear),
    m_imageResample(NoResample),
    m_imageFilterLut(agg::image_filter_bilinear(), true),

    m_fillGradientInterpolator(m_fillGradientMatrix),
    m_lineGradientInterpolator(m_lineGradientMatrix),

    m_linearGradientFunction(),
    m_radialGradientFunction(),

    m_lineWidth(1),
    m_evenOddFlag(false),

    m_start_x(0.0f),
    m_start_y(0.0f),

    m_path(),
    m_transform(),

    m_convCurve(m_path),
    m_convStroke(m_convCurve),
    m_convDash(m_convCurve),
    m_convDashStroke(m_convDash),

    m_pathTransform(m_convCurve, m_transform),
    m_strokeTransform(m_convStroke, m_transform),
    m_dashStrokeTransform(m_convDashStroke, m_transform),

#ifdef AGG2D_USE_FREETYPE
    m_fontEngine(),
#else
    m_fontDC(::GetDC(0)),
    m_fontEngine(m_fontDC),
#endif
    m_fontCacheManager(m_fontEngine)
{
    lineCap(m_lineCap);
    lineJoin(m_lineJoin);
}

void Agg2D::dashAdd( real dash_len, real gap_len )
{
    m_convDash.add_dash( dash_len, gap_len );
    m_dashes.push_back( Dash( dash_len, gap_len ) );
}

void Agg2D::dashStart( real dash_start )
{
    m_dashStart = dash_start;
    m_convDash.dash_start( dash_start );
}

void Agg2D::dashClear()
{
    m_convDash.remove_all_dashes();
    m_dashes.clear();
}

void Agg2D::saveStateTo(State& st)
{

  st.m_clipBox             = m_clipBox;

  st.m_blendMode           = m_blendMode;
  st.m_imageBlendMode      = m_imageBlendMode;
  st.m_imageBlendColor     = m_imageBlendColor;

  st.m_masterAlpha         = m_masterAlpha;
  st.m_antiAliasGamma      = m_antiAliasGamma;

  st.m_fillColor           = m_fillColor;
  st.m_lineColor           = m_lineColor;
  st.m_fillGradient        = m_fillGradient;
  st.m_lineGradient        = m_lineGradient;

  st.m_lineCap             = m_lineCap;
  st.m_lineJoin            = m_lineJoin;

  st.m_fillGradientFlag    = m_fillGradientFlag;
  st.m_lineGradientFlag    = m_lineGradientFlag;
  st.m_fillGradientMatrix  = m_fillGradientMatrix;
  st.m_lineGradientMatrix  = m_lineGradientMatrix;
  st.m_fillGradientD1      = m_fillGradientD1;
  st.m_lineGradientD1      = m_lineGradientD1;
  st.m_fillGradientD2      = m_fillGradientD2;
  st.m_lineGradientD2      = m_lineGradientD2;

  st.m_textAngle           = m_textAngle;
  st.m_textAlignX          = m_textAlignX;
  st.m_textAlignY          = m_textAlignY;
  st.m_textHints           = m_textHints;
  st.m_fontHeight          = m_fontHeight;
  st.m_fontAscent          = m_fontAscent;
  st.m_fontDescent         = m_fontDescent;
  st.m_fontCacheType       = m_fontCacheType;

  st.m_lineWidth           = m_lineWidth;
  st.m_evenOddFlag         = m_evenOddFlag;

  st.m_transform           = m_transform;
  st.m_affine              = m_affine;

  st.m_dashStart           = m_dashStart;
  st.m_dashes              = m_dashes;

}

void Agg2D::restoreStateFrom(const State& st)
{

  m_clipBox             = st.m_clipBox;

  m_blendMode           = st.m_blendMode;
  m_imageBlendMode      = st.m_imageBlendMode;
  m_imageBlendColor     = st.m_imageBlendColor;

  m_masterAlpha         = st.m_masterAlpha;
  m_antiAliasGamma      = st.m_antiAliasGamma;

  m_fillColor           = st.m_fillColor;
  m_lineColor           = st.m_lineColor;
  m_fillGradient        = st.m_fillGradient;
  m_lineGradient        = st.m_lineGradient;

  m_lineCap             = st.m_lineCap;
  m_lineJoin            = st.m_lineJoin;

  m_fillGradientFlag    = st.m_fillGradientFlag;
  m_lineGradientFlag    = st.m_lineGradientFlag;
  m_fillGradientMatrix  = st.m_fillGradientMatrix;
  m_lineGradientMatrix  = st.m_lineGradientMatrix;
  m_fillGradientD1      = st.m_fillGradientD1;
  m_lineGradientD1      = st.m_lineGradientD1;
  m_fillGradientD2      = st.m_fillGradientD2;
  m_lineGradientD2      = st.m_lineGradientD2;

  m_textAngle           = st.m_textAngle;
  m_textAlignX          = st.m_textAlignX;
  m_textAlignY          = st.m_textAlignY;
  m_textHints           = st.m_textHints;
  m_fontHeight          = st.m_fontHeight;
  m_fontAscent          = st.m_fontAscent;
  m_fontDescent         = st.m_fontDescent;
  m_fontCacheType       = st.m_fontCacheType;

  m_lineWidth           = st.m_lineWidth;
  m_evenOddFlag         = st.m_evenOddFlag;

  m_affine              = st.m_affine;
  m_transform           = st.m_transform;

  m_dashStart           = st.m_dashStart;
  m_dashes              = st.m_dashes;

  m_convDash.dash_start( m_dashStart );
  m_convDash.remove_all_dashes();

  for( unsigned i = 0; i < m_dashes.size(); ++i )
  {
    m_convDash.add_dash( m_dashes[i].dash_len, m_dashes[i].gap_len );
  }
}



//------------------------------------------------------------------------
void Agg2D::attach(unsigned char* buf, unsigned width, unsigned height, int stride)
{
    m_rbuf.attach(buf, width, height, stride);

    m_renBase.reset_clipping(true);
    m_renBaseComp.reset_clipping(true);
    m_renBasePre.reset_clipping(true);
    m_renBaseCompPre.reset_clipping(true);

    resetTransformations();
    lineWidth(1.0f),
    lineColor(0,0,0);
    fillColor(255,255,255);
    textAlignment(AlignLeft, AlignBottom);
    clipBox(0, 0, width, height);
    lineCap(LineCapRound);
    lineJoin(RoundJoin);
    flipText(false);
    imageFilter(Bilinear);
    imageResample(NoResample);
    m_masterAlpha = 1.0f;
    m_antiAliasGamma = 1.0f;
    m_rasterizer.gamma(agg::gamma_none());
    m_blendMode = BlendAlpha;

    m_dashStart = 0;
    m_dashes.clear();
}


//------------------------------------------------------------------------
void Agg2D::attach(Image& img)
{
    attach(img.renBuf.buf(), img.renBuf.width(), img.renBuf.height(), img.renBuf.stride());
}

//------------------------------------------------------------------------
void Agg2D::clipBox(real x1, real y1, real x2, real y2)
{
    m_clipBox = RectD(x1, y1, x2, y2);
    int rx1 = int(x1);
    int ry1 = int(y1);
    int rx2 = int(x2);
    int ry2 = int(y2);

    m_renBase.clip_box(rx1, ry1, rx2, ry2);
    m_renBaseComp.clip_box(rx1, ry1, rx2, ry2);
    m_renBasePre.clip_box(rx1, ry1, rx2, ry2);
    m_renBaseCompPre.clip_box(rx1, ry1, rx2, ry2);

    m_rasterizer.clip_box(x1, y1, x2, y2);
}

//------------------------------------------------------------------------
void Agg2D::blendMode(BlendMode m)
{
    m_blendMode = m;
    m_pixFormatComp.comp_op(m);
    m_pixFormatCompPre.comp_op(m);
}

//------------------------------------------------------------------------
Agg2D::BlendMode Agg2D::blendMode() const
{
    return m_blendMode;
}

//------------------------------------------------------------------------
void Agg2D::imageBlendMode(BlendMode m)
{
    m_imageBlendMode = m;
}

//------------------------------------------------------------------------
Agg2D::BlendMode Agg2D::imageBlendMode() const
{
    return m_imageBlendMode;
}

//------------------------------------------------------------------------
void Agg2D::imageBlendColor(Color c)
{
    m_imageBlendColor = c;
}

//------------------------------------------------------------------------
void Agg2D::imageBlendColor(unsigned r, unsigned g, unsigned b, unsigned a)
{
    imageBlendColor(Color(r, g, b, a));
}

//------------------------------------------------------------------------
Agg2D::Color Agg2D::imageBlendColor() const
{
    return m_imageBlendColor;
}

//------------------------------------------------------------------------
void Agg2D::masterAlpha(real a)
{
    m_masterAlpha = a;
    updateRasterizerGamma();
}

//------------------------------------------------------------------------
real Agg2D::masterAlpha() const
{
    return m_masterAlpha;
}

//------------------------------------------------------------------------
void Agg2D::antiAliasGamma(real g)
{
    m_antiAliasGamma = g;
    updateRasterizerGamma();
}

//------------------------------------------------------------------------
real Agg2D::antiAliasGamma() const
{
    return m_antiAliasGamma;
}

//------------------------------------------------------------------------
Agg2D::RectD Agg2D::clipBox() const
{
    return m_clipBox;
}

//------------------------------------------------------------------------
void Agg2D::clearAll(Color c)
{
    m_renBase.clear(c);
}

//------------------------------------------------------------------------
void Agg2D::clearAll(unsigned r, unsigned g, unsigned b, unsigned a)
{
    clearAll(Color(r, g, b, a));
}

//------------------------------------------------------------------------
void Agg2D::clearClipBox(Color c)
{
    m_renBase.copy_bar(0, 0, m_renBase.width(), m_renBase.height(), c);
}

//------------------------------------------------------------------------
void Agg2D::clearClipBox(unsigned r, unsigned g, unsigned b, unsigned a)
{
    clearClipBox(Color(r, g, b, a));
}

//------------------------------------------------------------------------
void Agg2D::worldToScreen(real& x, real& y) const
{
    m_transform.transform(&x, &y);
}

//------------------------------------------------------------------------
void Agg2D::screenToWorld(real& x, real& y) const
{
    m_transform.inverse_transform(&x, &y);
}


//------------------------------------------------------------------------
real Agg2D::worldToScreen(real scalar) const
{
    real x1 = 0;
    real y1 = 0;
    real x2 = scalar;
    real y2 = scalar;
    worldToScreen(x1, y1);
    worldToScreen(x2, y2);
    return SQRT((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) * 0.7071068f;
}


//------------------------------------------------------------------------
real Agg2D::screenToWorld(real scalar) const
{
    real x1 = 0;
    real y1 = 0;
    real x2 = scalar;
    real y2 = scalar;
    screenToWorld(x1, y1);
    screenToWorld(x2, y2);
    return SQRT((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) * 0.7071068f;
}


//------------------------------------------------------------------------
void Agg2D::alignPoint(real& x, real& y) const
{
    worldToScreen(x, y);
    x = FLOOR(x) + 0.5f;
    y = FLOOR(y) + 0.5f;
    screenToWorld(x, y);
}


//------------------------------------------------------------------------
bool Agg2D::inBox(real worldX, real worldY) const
{
    worldToScreen(worldX, worldY);
    return m_renBase.inbox(int(worldX), int(worldY));
}


//------------------------------------------------------------------------
Agg2D::Transformations Agg2D::transformations() const
{
    Transformations tr;
    m_transform.store_to(tr.affineMatrix);
    return tr;
}
//------------------------------------------------------------------------
const Agg2D::Affine& Agg2D::affine() const
{
    return m_affine;
}
//------------------------------------------------------------------------
void Agg2D::affine(const Affine& af)
{
    m_affine = af;
    updateTransformations();
}
//------------------------------------------------------------------------
void Agg2D::transformations(const Transformations& tr)
{
    real s = worldToScreen(1.0f) * g_approxScale;

    m_transform.load_from(tr.affineMatrix);
    m_convCurve.approximation_scale(s);
    m_convStroke.approximation_scale(s);
    m_convDashStroke.approximation_scale(s);
}
//------------------------------------------------------------------------
void Agg2D::resetTransformations()
{
    m_transform.reset();
}
//------------------------------------------------------------------------
void Agg2D::updateTransformations()
{
   real s = worldToScreen(1.0f) * g_approxScale;

   m_transform  = m_affine;
   m_transform *= m_viewport;
   m_convCurve.approximation_scale(s);
   m_convStroke.approximation_scale(s);
   m_convDashStroke.approximation_scale(s);
}
//------------------------------------------------------------------------
void Agg2D::addStrokePath()
{
    if( m_dashes.size() )
        m_rasterizer.add_path(m_dashStrokeTransform);
    else
        m_rasterizer.add_path(m_strokeTransform);
}
//------------------------------------------------------------------------
void Agg2D::rotate(real angle)
{
    m_affine.premultiply(agg::trans_affine_rotation(angle));
    updateTransformations();
}
//------------------------------------------------------------------------
void Agg2D::rotate(real angle, real cx, real cy)
{
    agg::trans_affine_translation m(-cx, -cy);
    m *= agg::trans_affine_rotation(angle);
    m *= agg::trans_affine_translation(cx, cy);
    m_affine.premultiply(m);
    updateTransformations();
}
//------------------------------------------------------------------------
void Agg2D::skew(real sx, real sy)
{
    m_affine.premultiply(agg::trans_affine_skewing(sx, sy));
    updateTransformations();
}
//------------------------------------------------------------------------
void Agg2D::translate(real x, real y)
{
    m_affine.premultiply(agg::trans_affine_translation(x, y));
    updateTransformations();
}
//------------------------------------------------------------------------
void Agg2D::matrix(const Affine& tr)
{
    m_affine.premultiply(tr);
    updateTransformations();
}
//------------------------------------------------------------------------
void Agg2D::matrix(const Transformations& tr)
{
    matrix(agg::trans_affine(tr.affineMatrix[0], tr.affineMatrix[1], tr.affineMatrix[2],
                             tr.affineMatrix[3], tr.affineMatrix[4], tr.affineMatrix[5]));
}
//------------------------------------------------------------------------
void Agg2D::scale(real s)
{
    m_affine.premultiply(agg::trans_affine_scaling(s));
    updateTransformations();
}
//------------------------------------------------------------------------
void Agg2D::scale(real sx, real sy)
{
    m_affine.premultiply(agg::trans_affine_scaling(sx, sy));
    updateTransformations();
}
//------------------------------------------------------------------------
void Agg2D::parallelogram(real x1, real y1, real x2, real y2, const real* para)
{
    m_affine.premultiply(agg::trans_affine(x1, y1, x2, y2, para));
    updateTransformations();
}
//------------------------------------------------------------------------
void Agg2D::viewport(real worldX1,  real worldY1,  real worldX2,  real worldY2,
                     real screenX1, real screenY1, real screenX2, real screenY2,
                     ViewportOption opt, WindowFitLogic fl)
{
    agg::trans_viewport vp;

    agg::aspect_ratio_e ar =
        (fl == WindowFitLogic_meet) ? agg::aspect_ratio_meet :
                                      agg::aspect_ratio_slice;
    switch(opt)
    {
        case Anisotropic: vp.preserve_aspect_ratio(0.0f, 0.0f, agg::aspect_ratio_stretch); break;
        case XMinYMin:    vp.preserve_aspect_ratio(0.0f, 0.0f, ar);    break;
        case XMidYMin:    vp.preserve_aspect_ratio(0.5f, 0.0f, ar);    break;
        case XMaxYMin:    vp.preserve_aspect_ratio(1.0f, 0.0f, ar);    break;
        case XMinYMid:    vp.preserve_aspect_ratio(0.0f, 0.5f, ar);    break;
        case XMidYMid:    vp.preserve_aspect_ratio(0.5f, 0.5f, ar);    break;
        case XMaxYMid:    vp.preserve_aspect_ratio(1.0f, 0.5f, ar);    break;
        case XMinYMax:    vp.preserve_aspect_ratio(0.0f, 1.0f, ar);    break;
        case XMidYMax:    vp.preserve_aspect_ratio(0.5f, 1.0f, ar);    break;
        case XMaxYMax:    vp.preserve_aspect_ratio(1.0f, 1.0f, ar);    break;
    }
    vp.world_viewport(worldX1,   worldY1,  worldX2,  worldY2);
    vp.device_viewport(screenX1, screenY1, screenX2, screenY2);

    m_viewport = vp.to_affine();
    updateTransformations();
}


//------------------------------------------------------------------------
void Agg2D::fillColor(Color c)
{
    m_fillColor = c;
    m_fillGradientFlag = Solid;
}

//------------------------------------------------------------------------
void Agg2D::fillColor(unsigned r, unsigned g, unsigned b, unsigned a)
{
    fillColor(Color(r, g, b, a));
}

//------------------------------------------------------------------------
void Agg2D::noFill()
{
    fillColor(Color(0, 0, 0, 0));
}

//------------------------------------------------------------------------
void Agg2D::fillPattern(const Image& img, unsigned opacity, int offset_x, int offset_y)
{
    m_fillGradientFlag = Pattern;

    m_fillPattern.pattern = img;
    m_fillPattern.offset_x = offset_x;
    m_fillPattern.offset_y = offset_y;
    m_fillPattern.opacity  = opacity > 255 ? 255 : opacity;

    m_fillColor = Color(0, 255, 255, m_fillPattern.opacity);
}

//------------------------------------------------------------------------
void Agg2D::lineColor(Color c)
{
    m_lineColor = c;
    m_lineGradientFlag = Solid;
}

//------------------------------------------------------------------------
void Agg2D::lineColor(unsigned r, unsigned g, unsigned b, unsigned a)
{
    lineColor(Color(r, g, b, a));
}

//------------------------------------------------------------------------
void Agg2D::noLine()
{
    lineColor(Color(0, 0, 0, 0));
}

//------------------------------------------------------------------------
Agg2D::Color Agg2D::fillColor() const
{
    return m_fillColor;
}

//------------------------------------------------------------------------
Agg2D::Color Agg2D::lineColor() const
{
    return m_lineColor;
}

//------------------------------------------------------------------------
void Agg2D::fillLinearGradient(const gradient_lut_type& lut,
                real x1, real y1, real x2, real y2,
              const agg::trans_affine& gradientTransform,
              spreadMethod_e spreadMethod,
              gradientUnits_e units,
              real opacity)
{
    agg::trans_affine mtx;

  mtx = agg::trans_affine_line_segment(x1, y1, x2, y2, 100.0f);

  if (units == gradientUnits_objectBoundingBox)
  {
    real parl[6] = { 0,0, 1,0, 1,1 };
    real object_bbox_x1 = 0.;
    real object_bbox_y1 = 0.;
    real object_bbox_x2 = 0.;
    real object_bbox_y2 = 0.;

    agg::bounding_rect_single(m_path, 0,
                  &object_bbox_x1, &object_bbox_y1,
                  &object_bbox_x2, &object_bbox_y2);

        if (object_bbox_y1 == object_bbox_y2)
            object_bbox_y2 += .1;

        if (object_bbox_x1 == object_bbox_x2)
            object_bbox_x2 += .1;

    mtx *= agg::trans_affine(parl,
                 object_bbox_x1,
                 object_bbox_y1,
                 object_bbox_x2,
                 object_bbox_y2);
  }

    mtx *= gradientTransform;
    mtx *= m_transform;
    mtx.invert();

  m_fillGradientSpreadMethod = spreadMethod;
  m_fillGradientFlag         = Linear;
  m_fillGradient             = lut;
  m_fillGradientMatrix       = mtx;
    m_fillGradientD1           = 0.0f;
    m_fillGradientD2           = 100.0f;
  m_fillGradientOpacity      = opacity;
  m_fillColor = Color(0,0,0);  // Set some real color
}
//------------------------------------------------------------------------
void Agg2D::lineLinearGradient(const gradient_lut_type& lut,
                real x1, real y1, real x2, real y2,
              const agg::trans_affine& gradientTransform,
              spreadMethod_e spreadMethod,
              gradientUnits_e units,
              real opacity)
{
    agg::trans_affine mtx;

  mtx = agg::trans_affine_line_segment(x1, y1, x2, y2, 100.0f);

  if (units == gradientUnits_objectBoundingBox)
  {
    real parl[6] = { 0,0, 1,0, 1,1 };
    real object_bbox_x1 = 0.;
    real object_bbox_y1 = 0.;
    real object_bbox_x2 = 0.;
    real object_bbox_y2 = 0.;

    agg::bounding_rect_single(m_path, 0,
                  &object_bbox_x1, &object_bbox_y1,
                  &object_bbox_x2, &object_bbox_y2);

        if (object_bbox_y1 == object_bbox_y2)
            object_bbox_y2 += .1;

        if (object_bbox_x1 == object_bbox_x2)
            object_bbox_x2 += .1;

    mtx *= agg::trans_affine(parl,
                 object_bbox_x1,
                 object_bbox_y1,
                 object_bbox_x2,
                 object_bbox_y2);
  }

    mtx *= gradientTransform;
    mtx *= m_transform;
    mtx.invert();

  m_lineGradientSpreadMethod = spreadMethod;
  m_lineGradientFlag         = Linear;
  m_lineGradient             = lut;
  m_lineGradientMatrix       = mtx;
    m_lineGradientD1           = 0.0f;
    m_lineGradientD2           = 100.0f;
  m_lineGradientOpacity      = opacity;
  m_lineColor = Color(0,0,0);  // Set some real color
}
//------------------------------------------------------------------------
void Agg2D::fillRadialGradient(const gradient_lut_type& lut,
                real cx, real cy, real r,
              real fx, real fy,
              const agg::trans_affine& gradientTransform,
              spreadMethod_e spreadMethod,
              gradientUnits_e units,
              real opacity)
{
  agg::trans_affine mtx;
    int gradient_type = 0;

    real fx_ = 0;
    real fy_ = 0;

  m_fillGradientFlag = (fx == cx && fy == cy) ? Radial : RadialWithFocus;

    mtx  = agg::trans_affine_scaling(r / 100.0f);
    mtx *= agg::trans_affine_translation(cx, cy);
    fx_ = fx;
    fy_ = fy;
    mtx.inverse_transform(&fx_, &fy_);

  if (units == gradientUnits_objectBoundingBox)
  {
    real parl[6] = { 0,0, 1,0, 1,1 };
    real object_bbox_x1 = 0.;
    real object_bbox_y1 = 0.;
    real object_bbox_x2 = 0.;
    real object_bbox_y2 = 0.;

    agg::bounding_rect_single(m_path, 0,
                  &object_bbox_x1, &object_bbox_y1,
                  &object_bbox_x2, &object_bbox_y2);

        if (object_bbox_y1 == object_bbox_y2)
            object_bbox_y2 += .1;

        if (object_bbox_x1 == object_bbox_x2)
            object_bbox_x2 += .1;

    mtx *= agg::trans_affine(parl,
                 object_bbox_x1,
                 object_bbox_y1,
                 object_bbox_x2,
                 object_bbox_y2);
  }

    mtx *= gradientTransform;
    mtx *= m_transform;
    mtx.invert();

    m_fillGradientSpreadMethod = spreadMethod;
    m_fillGradient             = lut;
    m_fillGradientMatrix       = mtx;
    m_fillGradientD1           = fx_;
    m_fillGradientD2           = fy_;
    m_fillGradientOpacity      = opacity;
    m_fillColor = Color(0,0,0);  // Set some real color
}
//------------------------------------------------------------------------
void Agg2D::lineRadialGradient(const gradient_lut_type& lut,
                real cx, real cy, real r,
              real fx, real fy,
              const agg::trans_affine& gradientTransform,
              spreadMethod_e spreadMethod,
              gradientUnits_e units,
              real opacity)
{
    agg::trans_affine mtx;
    int gradient_type = 0;

    real fx_ = 0;
    real fy_ = 0;

    m_lineGradientFlag = (fx == cx && fy == cy) ? Radial : RadialWithFocus;

    mtx  = agg::trans_affine_scaling(r / 100.0f);
    mtx *= agg::trans_affine_translation(cx, cy);
    fx_ = fx;
    fy_ = fy;
    mtx.inverse_transform(&fx_, &fy_);

    if (units == gradientUnits_objectBoundingBox)
    {
      real parl[6] = { 0,0, 1,0, 1,1 };
      real object_bbox_x1 = 0.;
      real object_bbox_y1 = 0.;
      real object_bbox_x2 = 0.;
      real object_bbox_y2 = 0.;

      agg::bounding_rect_single(m_path, 0,
                    &object_bbox_x1, &object_bbox_y1,
                    &object_bbox_x2, &object_bbox_y2);

          if (object_bbox_y1 == object_bbox_y2)
              object_bbox_y2 += .1;

          if (object_bbox_x1 == object_bbox_x2)
              object_bbox_x2 += .1;

      mtx *= agg::trans_affine(parl,
                   object_bbox_x1,
                   object_bbox_y1,
                   object_bbox_x2,
                   object_bbox_y2);
    }

    mtx *= gradientTransform;
    mtx *= m_transform;
    mtx.invert();

    m_lineGradientSpreadMethod = spreadMethod;
    m_lineGradient             = lut;
    m_lineGradientMatrix       = mtx;
    m_lineGradientD1           = fx_;
    m_lineGradientD2           = fy_;
    m_lineGradientOpacity      = opacity;
    m_lineColor = Color(0,0,0);  // Set some real color
}
//-------------------------------------------------------------------------
void Agg2D::fillLinearGradient(real x1, real y1, real x2, real y2, Color c1, Color c2, real profile)
{
    int i;
    int startGradient = 128 - int(profile * 127.0f);
    int endGradient   = 128 + int(profile * 127.0f);
    if (endGradient <= startGradient) endGradient = startGradient + 1;
    real k = 1.0f / real(endGradient - startGradient);
    for (i = 0; i < startGradient; i++)
    {
        m_fillGradient[i] = c1;
    }
    for (; i < endGradient; i++)
    {
        m_fillGradient[i] = c1.gradient(c2, real(i - startGradient) * k);
    }
    for (; i < 256; i++)
    {
        m_fillGradient[i] = c2;
    }
    real angle = (real)atan2(y2-y1, x2-x1);
    m_fillGradientMatrix.reset();
    m_fillGradientMatrix *= agg::trans_affine_rotation(angle);
    m_fillGradientMatrix *= agg::trans_affine_translation(x1, y1);
    m_fillGradientMatrix *= m_transform;
    m_fillGradientMatrix.invert();
    m_fillGradientD1 = 0.0f;
    m_fillGradientD2 = SQRT((x2-x1) * (x2-x1) + (y2-y1) * (y2-y1));
    m_fillGradientFlag = Linear;
    m_fillColor = Color(0,0,0);  // Set some real color
    m_fillGradientOpacity = 1.0;
}


//------------------------------------------------------------------------
void Agg2D::lineLinearGradient(real x1, real y1, real x2, real y2, Color c1, Color c2, real profile)
{
    int i;
    int startGradient = 128 - int(profile * 128.0f);
    int endGradient   = 128 + int(profile * 128.0f);
    if (endGradient <= startGradient) endGradient = startGradient + 1;
    real k = 1.0f / real(endGradient - startGradient);
    for (i = 0; i < startGradient; i++)
    {
        m_lineGradient[i] = c1;
    }
    for (; i < endGradient; i++)
    {
        m_lineGradient[i] = c1.gradient(c2, real(i - startGradient) * k);
    }
    for (; i < 256; i++)
    {
        m_lineGradient[i] = c2;
    }
    real angle = (real)atan2(y2-y1, x2-x1);
    m_lineGradientMatrix.reset();
    m_lineGradientMatrix *= agg::trans_affine_rotation(angle);
    m_lineGradientMatrix *= agg::trans_affine_translation(x1, y1);
    m_fillGradientMatrix *= m_transform;
    m_lineGradientMatrix.invert();
    m_lineGradientD1 = 0;
    m_lineGradientD2 = SQRT((x2-x1) * (x2-x1) + (y2-y1) * (y2-y1));
    m_lineGradientFlag = Linear;
    m_lineColor = Color(0,0,0);  // Set some real color
}


//------------------------------------------------------------------------
void Agg2D::fillRadialGradient(real x, real y, real r, Color c1, Color c2, real profile)
{
    int i;
    int startGradient = 128 - int(profile * 127.0f);
    int endGradient   = 128 + int(profile * 127.0f);
    if (endGradient <= startGradient) endGradient = startGradient + 1;
    real k = 1.0f / real(endGradient - startGradient);
    for (i = 0; i < startGradient; i++)
    {
        m_fillGradient[i] = c1;
    }
    for (; i < endGradient; i++)
    {
        m_fillGradient[i] = c1.gradient(c2, real(i - startGradient) * k);
    }
    for (; i < 256; i++)
    {
        m_fillGradient[i] = c2;
    }
    m_fillGradientD2 = worldToScreen(r);
    worldToScreen(x, y);
    m_fillGradientMatrix.reset();
    m_fillGradientMatrix *= agg::trans_affine_translation(x, y);
    m_fillGradientMatrix.invert();
    m_fillGradientD1 = 0;
    m_fillGradientFlag = Radial;
    m_fillColor = Color(0,0,0);  // Set some real color
    m_fillGradientOpacity = 1.0;
}


//------------------------------------------------------------------------
void Agg2D::lineRadialGradient(real x, real y, real r, Color c1, Color c2, real profile)
{
    int i;
    int startGradient = 128 - int(profile * 128.0f);
    int endGradient   = 128 + int(profile * 128.0f);
    if (endGradient <= startGradient) endGradient = startGradient + 1;
    real k = 1.0f / real(endGradient - startGradient);
    for (i = 0; i < startGradient; i++)
    {
        m_lineGradient[i] = c1;
    }
    for (; i < endGradient; i++)
    {
        m_lineGradient[i] = c1.gradient(c2, real(i - startGradient) * k);
    }
    for (; i < 256; i++)
    {
        m_lineGradient[i] = c2;
    }
    m_lineGradientD2 = worldToScreen(r);
    worldToScreen(x, y);
    m_lineGradientMatrix.reset();
    m_lineGradientMatrix *= agg::trans_affine_translation(x, y);
    m_lineGradientMatrix.invert();
    m_lineGradientD1 = 0;
    m_lineGradientFlag = Radial;
    m_lineColor = Color(0,0,0);  // Set some real color
}


//------------------------------------------------------------------------
void Agg2D::fillRadialGradient(real x, real y, real r, Color c1, Color c2, Color c3)
{
    int i;
    for (i = 0; i < 128; i++)
    {
        m_fillGradient[i] = c1.gradient(c2, real(i) / 127.0f);
    }
    for (; i < 256; i++)
    {
        m_fillGradient[i] = c2.gradient(c3, real(i - 128) / 127.0f);
    }
    m_fillGradientD2 = worldToScreen(r);
    worldToScreen(x, y);
    m_fillGradientMatrix.reset();
    m_fillGradientMatrix *= agg::trans_affine_translation(x, y);
    m_fillGradientMatrix.invert();
    m_fillGradientD1 = 0;
    m_fillGradientFlag = Radial;
    m_fillColor = Color(0,0,0);  // Set some real color
    m_fillGradientOpacity = 1.0;
}


//------------------------------------------------------------------------
void Agg2D::lineRadialGradient(real x, real y, real r, Color c1, Color c2, Color c3)
{
    int i;
    for (i = 0; i < 128; i++)
    {
        m_lineGradient[i] = c1.gradient(c2, real(i) / 127.0f);
    }
    for (; i < 256; i++)
    {
        m_lineGradient[i] = c2.gradient(c3, real(i - 128) / 127.0f);
    }
    m_lineGradientD2 = worldToScreen(r);
    worldToScreen(x, y);
    m_lineGradientMatrix.reset();
    m_lineGradientMatrix *= agg::trans_affine_translation(x, y);
    m_lineGradientMatrix.invert();
    m_lineGradientD1 = 0;
    m_lineGradientFlag = Radial;
    m_lineColor = Color(0,0,0);  // Set some real color
}


void Agg2D::fillRadialGradient(real x, real y, real r)
{
    m_fillGradientD2 = worldToScreen(r);
    worldToScreen(x, y);
    m_fillGradientMatrix.reset();
    m_fillGradientMatrix *= agg::trans_affine_translation(x, y);
    m_fillGradientMatrix.invert();
    m_fillGradientD1 = 0;
}


//------------------------------------------------------------------------
void Agg2D::lineRadialGradient(real x, real y, real r)
{
    m_lineGradientD2 = worldToScreen(r);
    worldToScreen(x, y);
    m_lineGradientMatrix.reset();
    m_lineGradientMatrix *= agg::trans_affine_translation(x, y);
    m_lineGradientMatrix.invert();
    m_lineGradientD1 = 0;
}


//------------------------------------------------------------------------
void Agg2D::lineWidth(real w)
{
    m_lineWidth = w;
    m_convStroke.width(w);
    m_convDashStroke.width(w);
}


//------------------------------------------------------------------------
real Agg2D::lineWidth(real w) const
{
    return m_lineWidth;
}


//------------------------------------------------------------------------
void Agg2D::fillEvenOdd(bool evenOddFlag)
{
    m_evenOddFlag = evenOddFlag;
    m_rasterizer.filling_rule(evenOddFlag ? agg::fill_even_odd : agg::fill_non_zero);
}


//------------------------------------------------------------------------
bool Agg2D::fillEvenOdd() const
{
    return m_evenOddFlag;
}


//------------------------------------------------------------------------
void Agg2D::lineCap(LineCap cap)
{
    m_lineCap = cap;
    m_convStroke.line_cap((agg::line_cap_e)cap);
    m_convDashStroke.line_cap((agg::line_cap_e)cap);
}


//------------------------------------------------------------------------
Agg2D::LineCap Agg2D::lineCap() const
{
    return m_lineCap;
}


//------------------------------------------------------------------------
void Agg2D::lineJoin(LineJoin join)
{
    m_lineJoin = join;
    m_convStroke.line_join((agg::line_join_e)join);
    m_convDashStroke.line_join((agg::line_join_e)join);
}


//------------------------------------------------------------------------
Agg2D::LineJoin Agg2D::lineJoin() const
{
    return m_lineJoin;
}


//------------------------------------------------------------------------
void Agg2D::addLine(real x1, real y1, real x2, real y2)
{
    m_path.move_to(x1, y1);
    m_path.line_to(x2, y2);
}


//------------------------------------------------------------------------
void Agg2D::line(real x1, real y1, real x2, real y2)
{
    m_path.remove_all();
    addLine(x1, y1, x2, y2);
    drawPath(StrokeOnly);
}


//------------------------------------------------------------------------
bool Agg2D::arrow_bone(real x_from, real y_from, real x_to, real y_to, real sz, real angle_ext)
{
    const real dx = x_to-x_from;
    const real dy = y_to-y_from;
    const real len = sqrt(dx*dx+dy*dy);

    if( len == 0 )
        return false;

    const real k = sz / len;
    const real dx0 = dx * k;
    const real dy0 = dy * k;

    const real a = angle_ext / 2;

    // turn 

    const real cos_a1 = cos( a);
    const real cos_a2 = cos(-a);
    const real sin_a1 = sin( a);
    const real sin_a2 = -sin_a1;

    const real dx1 = dx0*cos_a1 - dy0*sin_a1;
    const real dy1 = dx0*sin_a1 + dy0*cos_a1;

    const real dx2 = dx0*cos_a2 - dy0*sin_a2;
    const real dy2 = dx0*sin_a2 + dy0*cos_a2;


    //lineJoin(MiterJoin);

    m_path.remove_all();
    m_path.move_to( x_to-dx1, y_to-dy1 );
    m_path.line_to( x_to,     y_to );
    m_path.line_to( x_to-dx2, y_to-dy2 );

    drawPath(StrokeOnly);

    return true;
}

//------------------------------------------------------------------------
bool Agg2D::arrow(real x_from, real y_from, real x_to, real y_to, real sz, real angle_ext, real offset_int)
{
    const real dx = x_to-x_from;
    const real dy = y_to-y_from;
    const real len = sqrt(dx*dx+dy*dy);

    if( len == 0 )
        return false;

    const real k = sz / len;
    const real dx0 = dx * k;
    const real dy0 = dy * k;

    const real a = angle_ext / 2;

    const real sz_int = cos(a)*sz;
    const real k_int = sz_int / len;
    const real dx3 = dx * k_int * offset_int;
    const real dy3 = dy * k_int * offset_int;

    // turn 

    const real cos_a1 = cos( a);
    const real cos_a2 = cos(-a);
    const real sin_a1 = sin( a);
    const real sin_a2 = -sin_a1;

    const real dx1 = dx0*cos_a1 - dy0*sin_a1;
    const real dy1 = dx0*sin_a1 + dy0*cos_a1;

    const real dx2 = dx0*cos_a2 - dy0*sin_a2;
    const real dy2 = dx0*sin_a2 + dy0*cos_a2;



    m_path.remove_all();
    m_path.move_to( x_to-dx1, y_to-dy1 );
    m_path.line_to( x_to,     y_to );
    m_path.line_to( x_to-dx2, y_to-dy2 );
    m_path.line_to( x_to-dx3, y_to-dy3 );

    m_path.close_polygon();
    drawPath(FillAndStroke);

    return false;
}


//------------------------------------------------------------------------
void Agg2D::triangle(real x1, real y1, real x2, real y2, real x3, real y3)
{
    m_path.remove_all();
    m_path.move_to(x1, y1);
    m_path.line_to(x2, y2);
    m_path.line_to(x3, y3);
    m_path.close_polygon();
    drawPath(FillAndStroke);
}


//------------------------------------------------------------------------
void Agg2D::rectangle(real x1, real y1, real x2, real y2)
{
    m_path.remove_all();
    m_path.move_to(x1, y1);
    m_path.line_to(x2, y1);
    m_path.line_to(x2, y2);
    m_path.line_to(x1, y2);
    m_path.close_polygon();
    drawPath(FillAndStroke);
}


//------------------------------------------------------------------------
void Agg2D::roundedRect(real x1, real y1, real x2, real y2, real r)
{
    m_path.remove_all();
    agg::rounded_rect rc(x1, y1, x2, y2, r);
    rc.normalize_radius();
    rc.approximation_scale(worldToScreen(1.0f) * g_approxScale);
    // JME audit
    //m_path.add_path(rc, 0, false);
    m_path.concat_path(rc,0);
    drawPath(FillAndStroke);
}



//------------------------------------------------------------------------
void Agg2D::roundedRect(real x1, real y1, real x2, real y2, real rx, real ry)
{
    m_path.remove_all();
    agg::rounded_rect rc;
    rc.rect(x1, y1, x2, y2);
    rc.radius(rx, ry);
    rc.normalize_radius();
    //m_path.add_path(rc, 0, false);
    m_path.concat_path(rc,0); // JME
    drawPath(FillAndStroke);
}



//------------------------------------------------------------------------
void Agg2D::roundedRect(real x1, real y1, real x2, real y2,
                        real rx_bottom, real ry_bottom,
                        real rx_top,    real ry_top)
{
    m_path.remove_all();
    agg::rounded_rect rc;
    rc.rect(x1, y1, x2, y2);
    rc.radius(rx_bottom, ry_bottom, rx_top, ry_top);
    rc.normalize_radius();
    rc.approximation_scale(worldToScreen(1.0f) * g_approxScale);
    //m_path.add_path(rc, 0, false);
    m_path.concat_path(rc,0); // JME
    drawPath(FillAndStroke);
}

//------------------------------------------------------------------------
void Agg2D::roundedRect(real x1, real y1, real x2, real y2,
                     real rx1, real ry1, real rx2, real ry2,
                     real rx3, real ry3, real rx4, real ry4)
{
    m_path.remove_all();
    agg::rounded_rect rc;
    rc.rect(x1, y1, x2, y2);
    rc.radius(rx1, ry1, rx2, ry2, rx3, ry3, rx4, ry4);
    rc.normalize_radius();
    rc.approximation_scale(worldToScreen(1.0f) * g_approxScale);
    m_path.concat_path(rc,0);
    drawPath(FillAndStroke);
}

//------------------------------------------------------------------------
void Agg2D::ellipse(real cx, real cy, real rx, real ry)
{
    m_path.remove_all();
    agg::bezier_arc arc(cx, cy, rx, ry, 0, 2*pi());
    //m_path.add_path(arc, 0, false);
    m_path.concat_path(arc,0); // JME
    m_path.close_polygon();
    drawPath(FillAndStroke);
}


//------------------------------------------------------------------------
void Agg2D::arc(real cx, real cy, real rx, real ry, real start, real sweep)
{
    m_path.remove_all();
    agg::bezier_arc arc(cx, cy, rx, ry, start, sweep);
    //m_path.add_path(arc, 0, false);
    m_path.concat_path(arc,0); // JME
    drawPath(StrokeOnly);
}


//------------------------------------------------------------------------
void Agg2D::star(real cx, real cy, real r1, real r2, real startAngle, int numRays)
{
    m_path.remove_all();
    real da = agg::pi / real(numRays);
    real a = startAngle;
    int i;
    for (i = 0; i < numRays; i++)
    {
        real x = (real)cos(a) * r2 + cx;
        real y = (real)sin(a) * r2 + cy;
        if (i) m_path.line_to(x, y);
        else   m_path.move_to(x, y);
        a += da;
        m_path.line_to((real)cos(a) * r1 + cx, (real)sin(a) * r1 + cy);
        a += da;
    }
    closePolygon();
    drawPath(FillAndStroke);
}


//------------------------------------------------------------------------
void Agg2D::curve(real x1, real y1, real x2, real y2, real x3, real y3)
{
    m_path.remove_all();
    m_path.move_to(x1, y1);
    m_path.curve3(x2, y2, x3, y3);
    drawPath(StrokeOnly);
}


//------------------------------------------------------------------------
void Agg2D::curve(real x1, real y1, real x2, real y2, real x3, real y3, real x4, real y4)
{
    m_path.remove_all();
    m_path.move_to(x1, y1);
    m_path.curve4(x2, y2, x3, y3, x4, y4);
    drawPath(StrokeOnly);
}


//------------------------------------------------------------------------
void Agg2D::polygon(real* xy, int numPoints)
{
    m_path.remove_all();
    //m_path.add_poly(xy, numPoints);
    m_path.concat_poly(xy,numPoints,true); // JME, AF
    closePolygon();
    drawPath(FillAndStroke);
}


//------------------------------------------------------------------------
void Agg2D::polyline(real* xy, int numPoints)
{
    m_path.remove_all();
    //m_path.add_poly(xy, numPoints);
    m_path.concat_poly(xy,numPoints,true); // JME, AF
    drawPath(StrokeOnly);
}


//------------------------------------------------------------------------
void Agg2D::flipText(bool flip)
{
#if !defined( UNDER_CE )
    m_fontEngine.flip_y(flip);
#endif
}

//------------------------------------------------------------------------
void Agg2D::font(const char* fontName,
                 real height,
                 bool bold,
                 bool italic,
                 FontCacheType ch,
                 real angle)
{
#if !defined( UNDER_CE )

    m_textAngle = angle;
    m_fontHeight = height;
    m_fontCacheType = ch;


#ifdef AGG2D_USE_FREETYPE
    // fontconfig anyone?
    m_fontEngine.load_font(fontName,
                           0,
                           (ch == VectorFontCache) ?
                                agg::glyph_ren_outline :
                                agg::glyph_ren_agg_gray8);
    m_fontEngine.hinting(m_textHints);
    m_fontEngine.height((ch == VectorFontCache) ? height : worldToScreen(height));
#else
    m_fontEngine.hinting(m_textHints);

    m_fontEngine.create_font(fontName,
                             (ch == VectorFontCache) ?
                                agg::glyph_ren_outline :
                                agg::glyph_ren_agg_gray8,
                              (ch == VectorFontCache) ? height : worldToScreen(height),
                              0.0f,
                              bold ? 700 : 400,
                              italic);
#endif

#endif

}


//------------------------------------------------------------------------
real Agg2D::fontHeight() const
{
   return m_fontHeight;
}

real Agg2D::fontAscent() const
{
   return m_fontAscent;
}

//------------------------------------------------------------------------
void Agg2D::textAlignment(TextAlignment alignX, TextAlignment alignY)
{
   m_textAlignX = alignX;
   m_textAlignY = alignY;
}

//------------------------------------------------------------------------
real Agg2D::textWidth(const char* str, unsigned int len)
{
#if defined( UNDER_CE )
  return 0;
#else
    real x = 0;
    real y = 0;
    bool first = true;
    while(*str && len)
    {
        const agg::glyph_cache* glyph = m_fontCacheManager.glyph(*str);
        if(glyph)
        {
            if(!first) m_fontCacheManager.add_kerning(&x, &y);
            x += glyph->advance_x;
            y += glyph->advance_y;
            first = true;
        }
        ++str; --len;
    }
    return (m_fontCacheType == VectorFontCache) ? x : screenToWorld(x);
#endif
}

//------------------------------------------------------------------------
real Agg2D::textWidth(const wchar_t* str, unsigned int len)
{
#if defined( UNDER_CE )
  return 0;
#else
    real x = 0;
    real y = 0;
    bool first = true;
    while(*str && len)
    {
        const agg::glyph_cache* glyph = m_fontCacheManager.glyph(*str);
        if(glyph)
        {
            if(!first) m_fontCacheManager.add_kerning(&x, &y);
            x += glyph->advance_x;
            y += glyph->advance_y;
            first = true;
        }
        ++str; --len;
    }
    return (m_fontCacheType == VectorFontCache) ? x : screenToWorld(x);
#endif
}


//------------------------------------------------------------------------
bool Agg2D::textHints() const
{
   return m_textHints;
}

//------------------------------------------------------------------------
void Agg2D::textHints(bool hints)
{
   m_textHints = hints;
}



//------------------------------------------------------------------------
void Agg2D::text(real x, real y, const char* str, unsigned int len, bool roundOff, real ddx, real ddy)
{
#if !defined( UNDER_CE )

   real dx = 0.0f;
   real dy = 0.0f;

   switch(m_textAlignX)
   {
       case AlignCenter:  dx = -textWidth(str,len) * 0.5f; break;
       case AlignRight:   dx = -textWidth(str,len);       break;
       default: break;
   }


   real asc = fontHeight();
   const agg::glyph_cache* glyph = m_fontCacheManager.glyph('H');
   if(glyph)
   {
       asc = glyph->bounds.y2 - glyph->bounds.y1;
   }

   if(m_fontCacheType == RasterFontCache)
   {
       asc = screenToWorld(asc);
   }

   switch(m_textAlignY)
   {
       case AlignCenter:  dy = -asc * 0.5f; break;
       case AlignTop:     dy = -asc;       break;
       default: break;
   }

   if(m_fontEngine.flip_y()) dy = -dy;

   agg::trans_affine  mtx;

    real start_x = x + dx;
    real start_y = y + dy;

    if (roundOff)
    {
        start_x = int(start_x);
        start_y = int(start_y);
    }
    start_x += ddx;
    start_y += ddy;

    mtx *= agg::trans_affine_translation(-x, -y);
    mtx *= agg::trans_affine_rotation(m_textAngle);
    mtx *= agg::trans_affine_translation(x, y);

    agg::conv_transform<FontCacheManager::path_adaptor_type> tr(m_fontCacheManager.path_adaptor(), mtx);

    if(m_fontCacheType == RasterFontCache)
    {
        worldToScreen(start_x, start_y);
    }

    unsigned int i;
    for (i = 0; i < len && str[i]; i++)
    {
        glyph = m_fontCacheManager.glyph(str[i]);
        if(glyph)
        {
            if(i) m_fontCacheManager.add_kerning(&x, &y);
            m_fontCacheManager.init_embedded_adaptors(glyph, start_x, start_y);

            if(glyph->data_type == agg::glyph_data_outline)
            {
                m_path.remove_all();
                m_path.concat_path(tr, 0);
                drawPath();
            }

            if(glyph->data_type == agg::glyph_data_gray8)
            {
                render(m_fontCacheManager.gray8_adaptor(),
                       m_fontCacheManager.gray8_scanline());
            }
            start_x += glyph->advance_x;
            start_y += glyph->advance_y;
        }
    }
#endif
}

void Agg2D::text(real x, real y, const wchar_t* str, unsigned int len, bool roundOff, real ddx, real ddy)
{
#if !defined( UNDER_CE )

   real dx = 0.0f;
   real dy = 0.0f;

   switch(m_textAlignX)
   {
       case AlignCenter:  dx = -textWidth(str,len) * 0.5f; break;
       case AlignRight:   dx = -textWidth(str,len);       break;
       default: break;
   }

   real asc = fontHeight();
   const agg::glyph_cache* glyph = m_fontCacheManager.glyph('H');
   if(glyph)
   {
       asc = glyph->bounds.y2 - glyph->bounds.y1;
   }

   if(m_fontCacheType == RasterFontCache)
   {
       asc = screenToWorld(asc);
   }

   switch(m_textAlignY)
   {
       case AlignCenter:  dy = -asc * 0.5f; break;
       case AlignTop:     dy = -asc;       break;
       default: break;
   }

   if(m_fontEngine.flip_y()) dy = -dy;

   agg::trans_affine  mtx;

    real start_x = x + dx;
    real start_y = y + dy;

    if (roundOff)
    {
        start_x = int(start_x);
        start_y = int(start_y);
    }
    start_x += ddx;
    start_y += ddy;

    mtx *= agg::trans_affine_translation(-x, -y);
    mtx *= agg::trans_affine_rotation(m_textAngle);
    mtx *= agg::trans_affine_translation(x, y);

    agg::conv_transform<FontCacheManager::path_adaptor_type> tr(m_fontCacheManager.path_adaptor(), mtx);

    if(m_fontCacheType == RasterFontCache)
    {
        worldToScreen(start_x, start_y);
    }

    unsigned int i;
    for (i = 0; i < len && str[i]; i++)
    {
        glyph = m_fontCacheManager.glyph(str[i]);
        if(glyph)
        {
            if(i) m_fontCacheManager.add_kerning(&x, &y);
            m_fontCacheManager.init_embedded_adaptors(glyph, start_x, start_y);

            if(glyph->data_type == agg::glyph_data_outline)
            {
                m_path.remove_all();
                //m_path.add_path(tr, 0, false);
        m_path.concat_path(tr,0); // JME
                drawPath();
            }

            if(glyph->data_type == agg::glyph_data_gray8)
            {
                render(m_fontCacheManager.gray8_adaptor(),
                       m_fontCacheManager.gray8_scanline());
            }
            start_x += glyph->advance_x;
            start_y += glyph->advance_y;
        }
    }
  #endif
}


//------------------------------------------------------------------------
void Agg2D::resetPath() { m_path.remove_all(); }

//------------------------------------------------------------------------
void Agg2D::moveTo(real x, real y)
{
    m_start_x = x;
    m_start_y = y;

    m_path.move_to(x, y);
}

//------------------------------------------------------------------------
void Agg2D::moveRel(real dx, real dy)
{
    if(m_path.vertices().total_vertices())
    {
        real x2;
        real y2;
        m_path.vertices().last_vertex(&x2, &y2);

        dx += x2;
        dy += y2;
    }

    moveTo(dx, dy);
}


//------------------------------------------------------------------------
void Agg2D::lineTo(real x, real y)
{
    m_path.line_to(x, y);
}


//------------------------------------------------------------------------
void Agg2D::lineRel(real dx, real dy)
{
    m_path.line_rel(dx, dy);
}


//------------------------------------------------------------------------
void Agg2D::horLineTo(real x)
{
    m_path.hline_to(x);
}


//------------------------------------------------------------------------
void Agg2D::horLineRel(real dx)
{
    m_path.hline_rel(dx);
}


//------------------------------------------------------------------------
void Agg2D::verLineTo(real y)
{
    m_path.vline_to(y);
}


//------------------------------------------------------------------------
void Agg2D::verLineRel(real dy)
{
    m_path.vline_rel(dy);
}


//------------------------------------------------------------------------
void Agg2D::arcTo(real rx, real ry,
                  real angle,
                  bool largeArcFlag,
                  bool sweepFlag,
                  real x, real y)
{
    m_path.arc_to(rx, ry, angle, largeArcFlag, sweepFlag, x, y);
}


//------------------------------------------------------------------------
void Agg2D::arcRel(real rx, real ry,
                   real angle,
                   bool largeArcFlag,
                   bool sweepFlag,
                   real dx, real dy)
{
    m_path.arc_rel(rx, ry, angle, largeArcFlag, sweepFlag, dx, dy);
}


//------------------------------------------------------------------------
void Agg2D::quadricCurveTo(real xCtrl, real yCtrl,
                           real xTo,   real yTo)
{
    m_path.curve3(xCtrl, yCtrl, xTo, yTo);
}


//------------------------------------------------------------------------
void Agg2D::quadricCurveRel(real dxCtrl, real dyCtrl,
                            real dxTo,   real dyTo)
{
    m_path.curve3_rel(dxCtrl, dyCtrl, dxTo, dyTo);
}


//------------------------------------------------------------------------
void Agg2D::quadricCurveTo(real xTo, real yTo)
{
    m_path.curve3(xTo, yTo);
}


//------------------------------------------------------------------------
void Agg2D::quadricCurveRel(real dxTo, real dyTo)
{
    m_path.curve3_rel(dxTo, dyTo);
}


//------------------------------------------------------------------------
void Agg2D::cubicCurveTo(real xCtrl1, real yCtrl1,
                         real xCtrl2, real yCtrl2,
                         real xTo,    real yTo)
{
    m_path.curve4(xCtrl1, yCtrl1, xCtrl2, yCtrl2, xTo, yTo);
}


//------------------------------------------------------------------------
void Agg2D::cubicCurveRel(real dxCtrl1, real dyCtrl1,
                          real dxCtrl2, real dyCtrl2,
                          real dxTo,    real dyTo)
{
    m_path.curve4_rel(dxCtrl1, dyCtrl1, dxCtrl2, dyCtrl2, dxTo, dyTo);
}


//------------------------------------------------------------------------
void Agg2D::cubicCurveTo(real xCtrl2, real yCtrl2,
                         real xTo,    real yTo)
{
    m_path.curve4(xCtrl2, yCtrl2, xTo, yTo);
}


//------------------------------------------------------------------------
void Agg2D::cubicCurveRel(real xCtrl2, real yCtrl2,
                          real xTo,    real yTo)
{
    m_path.curve4_rel(xCtrl2, yCtrl2, xTo, yTo);
}

//------------------------------------------------------------------------
void Agg2D::addEllipse(real cx, real cy, real rx, real ry, Direction dir)
{
    agg::bezier_arc arc(cx, cy, rx, ry, 0, (dir == CCW) ? 2*pi() : -2*pi());
    //m_path.add_path(arc, 0, false);
  m_path.concat_path(arc,0); // JME
    m_path.close_polygon();
}

//------------------------------------------------------------------------
void Agg2D::closePolygon()
{
    if(agg::is_vertex(m_path.vertices().last_command()))
    {
        m_path.vertices().add_vertex(m_start_x, m_start_y,
            agg::path_cmd_end_poly | agg::path_flags_close);
    }
}

void Agg2D::blur(unsigned rx, unsigned ry)
{
  PixFormat pixf(m_rbuf);
  agg::stack_blur_rgba32(pixf, rx, ry);
}

//------------------------------------------------------------------------
void Agg2D::imageFilter(ImageFilter f)
{
    m_imageFilter = f;
    switch(f)
    {
        case NoFilter:    break;
        case Bilinear:    m_imageFilterLut.calculate(agg::image_filter_bilinear(),    true); break;
        case Hanning:     m_imageFilterLut.calculate(agg::image_filter_hanning(),     true); break;
        case Hamming:     m_imageFilterLut.calculate(agg::image_filter_hamming(),     true); break;
        case Hermite:     m_imageFilterLut.calculate(agg::image_filter_hermite(),     true); break;
        case Quadric:     m_imageFilterLut.calculate(agg::image_filter_quadric(),     true); break;
        case Bicubic:     m_imageFilterLut.calculate(agg::image_filter_bicubic(),     true); break;
        case Catrom:      m_imageFilterLut.calculate(agg::image_filter_catrom(),      true); break;
        case Spline16:    m_imageFilterLut.calculate(agg::image_filter_spline16(),    true); break;
        case Spline36:    m_imageFilterLut.calculate(agg::image_filter_spline36(),    true); break;
        case Blackman144: m_imageFilterLut.calculate(agg::image_filter_blackman144(), true); break;
    }
}


//------------------------------------------------------------------------
Agg2D::ImageFilter Agg2D::imageFilter() const
{
    return m_imageFilter;
}


//------------------------------------------------------------------------
void Agg2D::imageResample(ImageResample f)
{
    m_imageResample = f;
}


//------------------------------------------------------------------------
Agg2D::ImageResample Agg2D::imageResample() const
{
    return m_imageResample;
}


//------------------------------------------------------------------------
void Agg2D::transformImage(const Image& img,    int imgX1,    int imgY1,    int imgX2,    int imgY2,
                                             real dstX1, real dstY1, real dstX2, real dstY2)
{
    resetPath();
    moveTo(dstX1, dstY1);
    lineTo(dstX2, dstY1);
    lineTo(dstX2, dstY2);
    lineTo(dstX1, dstY2);
    closePolygon();
    real parallelogram[6] = { dstX1, dstY1, dstX2, dstY1, dstX2, dstY2 };
    //real parallelogram[6] = { dstX1, dstY2, dstX2, dstY2, dstX2, dstY1 };
    renderImage(img, imgX1, imgY1, imgX2, imgY2, parallelogram);
}

//------------------------------------------------------------------------
void Agg2D::transformImage(const Image& img, real dstX1, real dstY1, real dstX2, real dstY2)
{
    resetPath();
    moveTo(dstX1, dstY1);
    lineTo(dstX2, dstY1);
    lineTo(dstX2, dstY2);
    lineTo(dstX1, dstY2);
    closePolygon();
    real parallelogram[6] = { dstX1, dstY1, dstX2, dstY1, dstX2, dstY2 };
    //real parallelogram[6] = { dstX1, dstY2, dstX2, dstY2, dstX2, dstY1 };
    renderImage(img, 0, 0, img.renBuf.width(), img.renBuf.height(), parallelogram);
}

//------------------------------------------------------------------------
void Agg2D::transformImage(const Image& img, int imgX1, int imgY1, int imgX2, int imgY2,
                           const real* parallelogram)
{
    resetPath();
    moveTo(parallelogram[0], parallelogram[1]);
    lineTo(parallelogram[2], parallelogram[3]);
    lineTo(parallelogram[4], parallelogram[5]);
    lineTo(parallelogram[0] + parallelogram[4] - parallelogram[2],
           parallelogram[1] + parallelogram[5] - parallelogram[3]);
    closePolygon();
    renderImage(img, imgX1, imgY1, imgX2, imgY2, parallelogram);
}


//------------------------------------------------------------------------
void Agg2D::transformImage(const Image& img, const real* parallelogram)
{
    resetPath();
    moveTo(parallelogram[0], parallelogram[1]);
    lineTo(parallelogram[2], parallelogram[3]);
    lineTo(parallelogram[4], parallelogram[5]);
    lineTo(parallelogram[0] + parallelogram[4] - parallelogram[2],
           parallelogram[1] + parallelogram[5] - parallelogram[3]);
    closePolygon();
    renderImage(img, 0, 0, img.renBuf.width(), img.renBuf.height(), parallelogram);
}

//------------------------------------------------------------------------
void Agg2D::transformImagePath(const Image& img,    int imgX1,    int imgY1,    int imgX2,    int imgY2,
                                                 real dstX1, real dstY1, real dstX2, real dstY2)
{
    real parallelogram[6] = { dstX1, dstY1, dstX2, dstY1, dstX2, dstY2 };
    renderImage(img, imgX1, imgY1, imgX2, imgY2, parallelogram);
}

//------------------------------------------------------------------------
void Agg2D::transformImagePath(const Image& img, real dstX1, real dstY1, real dstX2, real dstY2)
{
    real parallelogram[6] = { dstX1, dstY1, dstX2, dstY1, dstX2, dstY2 };
    renderImage(img, 0, 0, img.renBuf.width(), img.renBuf.height(), parallelogram);
}

//------------------------------------------------------------------------
void Agg2D::transformImagePath(const Image& img, int imgX1, int imgY1, int imgX2, int imgY2,
                               const real* parallelogram)
{
    renderImage(img, imgX1, imgY1, imgX2, imgY2, parallelogram);
}

//------------------------------------------------------------------------
void Agg2D::transformImagePath(const Image& img, const real* parallelogram)
{
    renderImage(img, 0, 0, img.renBuf.width(), img.renBuf.height(), parallelogram);
}



//------------------------------------------------------------------------
void Agg2D::drawPath(DrawPathFlag flag)
{
    m_rasterizer.reset();
    switch(flag)
    {
    case FillOnly:
        if (m_fillColor.a)
        {
            m_rasterizer.add_path(m_pathTransform);
            render(true);
        }
        break;

    case StrokeOnly:
        if (m_lineColor.a && m_lineWidth > 0.0f)
        {
            addStrokePath();
            render(false);
        }
        break;

    case FillAndStroke:
        if (m_fillColor.a)
        {
            m_rasterizer.add_path(m_pathTransform);
            render(true);
        }

        if (m_lineColor.a && m_lineWidth > 0.0f)
        {
            addStrokePath();
            render(false);
        }
        break;

    case FillWithLineColor:
        if (m_lineColor.a)
        {
            m_rasterizer.add_path(m_pathTransform);
            render(false);
        }
        break;
    }
}



//------------------------------------------------------------------------
class Agg2DRenderer
{
  typedef Agg2D::ColorType        color_type;
  typedef Agg2D::GradientArray    gradient_lut_type;
  typedef Agg2D::SpanInterpolator interpolator_type;
  typedef Agg2D::SpanAllocator    span_allocator_type;

  typedef agg::span_gradient<color_type,
                 Agg2D::SpanInterpolator,
                 agg::gradient_x,
                 gradient_opacity> LinearGradientSpan;

  typedef agg::span_gradient<color_type,
                Agg2D::SpanInterpolator,
                agg::gradient_radial,
                gradient_opacity> RadialGradientSpan;
  //-------------------------------------------------------------------------
  template <class GradientFunc, class BaseRenderer>
  static void render_gradient(Agg2D&                   gr,
                BaseRenderer&            renBase,
                const gradient_lut_type& lut,
                const GradientFunc&      func,
                interpolator_type&       interpolator,
                real                   D1,
                real                   D2,
                real                   opacity)
  {
    gradient_opacity lut_with_opacity(lut, opacity);

    typedef agg::span_gradient
    <
      color_type,
      interpolator_type,
      GradientFunc,
      gradient_opacity
    >
    gradient_span_type;

    gradient_span_type span(interpolator, func, lut_with_opacity, D1, D2);

    agg::renderer_scanline_aa
    <
      BaseRenderer,
      span_allocator_type,
      gradient_span_type
    >
    ren(renBase, gr.m_allocator, span);

    agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ren);
  }
  //-------------------------------------------------------------------------
  template <class GradientFunc, class BaseRenderer>
  static void render_gradient(Agg2D&                   gr,
                BaseRenderer&            renBase,
                Agg2D::spreadMethod_e    spreadMethod,
                const gradient_lut_type& lut,
                const GradientFunc&      func,
                interpolator_type&       intr,
                real                   D1,
                real                   D2,
                real                   opacity
                )
  {
    agg::gradient_reflect_adaptor<GradientFunc> reflect_func(func);
    agg::gradient_repeat_adaptor<GradientFunc>  repeat_func(func);

    switch (spreadMethod)
    {
    case Agg2D::spreadMethod_pad:
      render_gradient(gr, renBase, lut, func, intr, D1, D2, opacity);
      break;

    case Agg2D::spreadMethod_reflect:
      render_gradient(gr, renBase, lut, reflect_func, intr, D1, D2, opacity);
      break;

    case Agg2D::spreadMethod_repeat:
      render_gradient(gr, renBase, lut, repeat_func, intr, D1, D2, opacity);
      break;

    default:
      assert(false);
    }
  }

    //-------------------------------------------------------------------------
  template< typename Wrap, typename BaseRenderer >
  static void render_pattern( Agg2D& gr
                            , BaseRenderer& renBase
                            , Agg2D::Image& img_fill
                            , int offset_x
                            , int offset_y
                            , unsigned opacity )
  {
    //agg::wrap_mode_repeat::
    //agg::wrap_mode_repeat_pow2::

    typedef Agg2D::PixFormat                                pixfmt_a;
    typedef agg::image_accessor_wrap<pixfmt_a, Wrap, Wrap>  img_source_a;
    typedef agg::span_pattern_rgb<img_source_a>             span_gen_a;

    pixfmt_a        img_pxf( img_fill.renBuf );    // The pattern buffer
    img_source_a    img_src( img_pxf );
    
    span_gen_a      sg( img_src, gr.width(), gr.height() );


    if( opacity < 0   ) opacity = 0;
    if( opacity > 255 ) opacity = 255;

    if( offset_x < 0 ) offset_x = img_fill.width()  - offset_x;
    if( offset_y < 0 ) offset_y = img_fill.height() - offset_y;
    
    sg.alpha( opacity );
    sg.offset_x( (unsigned)offset_x );
    sg.offset_y( (unsigned)offset_y );

    agg::render_scanlines_aa( gr.m_rasterizer, gr.m_scanline, renBase, gr.m_allocator, sg );
  }

  static inline bool is_power_of_2( unsigned v )
  {
     return v && !(v & (v - 1));
  }

  //-------------------------------------------------------------------------
  template< typename BaseRenderer >
  static void render_pattern( Agg2D& gr
                            , BaseRenderer& renBase
                            , Agg2D::spreadMethod_e //spreadMethod
                            , Agg2D::Image& img_fill
                            , int offset_x
                            , int offset_y
                            , unsigned opacity )
  {
    if( is_power_of_2( img_fill.width() ) && 
        is_power_of_2( img_fill.height() ) )
    {
      typedef agg::wrap_mode_repeat_pow2    wrap_a;
      render_pattern<wrap_a>( gr, renBase, img_fill, offset_x, offset_y, opacity );
    }
    else
    {
      typedef agg::wrap_mode_repeat         wrap_a;
      render_pattern<wrap_a>( gr, renBase, img_fill, offset_x, offset_y, opacity );
    }
  }

public:
    //--------------------------------------------------------------------
    template<class BaseRenderer, class SolidRenderer>
    static void render(Agg2D&         gr,
             BaseRenderer&  renBase,
             SolidRenderer& renSolid,
             bool           fillColor)
    {
        if ((fillColor && gr.m_fillGradientFlag == Agg2D::Linear) ||
           (!fillColor && gr.m_lineGradientFlag == Agg2D::Linear))
        {
            if (fillColor)
            {
        render_gradient
        (
          gr,
          renBase,
          gr.m_fillGradientSpreadMethod,
          gr.m_fillGradient,
          gr.m_linearGradientFunction,
                                               gr.m_fillGradientInterpolator,
                                               gr.m_fillGradientD1,
          gr.m_fillGradientD2,
          gr.m_fillGradientOpacity
        );
            }
            else
            {
        render_gradient
        (
          gr,
          renBase,
          gr.m_lineGradientSpreadMethod,
          gr.m_lineGradient,
          gr.m_linearGradientFunction,
                                               gr.m_lineGradientInterpolator,
                                               gr.m_lineGradientD1,
          gr.m_lineGradientD2,
          gr.m_lineGradientOpacity
        );
            }
      return;
        }

            if ((fillColor && gr.m_fillGradientFlag == Agg2D::Radial) ||
               (!fillColor && gr.m_lineGradientFlag == Agg2D::Radial))
            {
              if (fillColor)
              {
                render_gradient
                (
                  gr,
                  renBase,
                  gr.m_fillGradientSpreadMethod,
                  gr.m_fillGradient,
                  gr.m_radialGradientFunction,
                  gr.m_fillGradientInterpolator,
                  gr.m_fillGradientD1,
                  gr.m_fillGradientD2,
                  gr.m_fillGradientOpacity
                );
              }
              else
              {
                render_gradient
                (
                  gr,
                  renBase,
                  gr.m_lineGradientSpreadMethod,
                  gr.m_lineGradient,
                  gr.m_radialGradientFunction,
                  gr.m_lineGradientInterpolator,
                  gr.m_fillGradientD1,
                  gr.m_fillGradientD2,
                  gr.m_lineGradientOpacity
                );
              }
              return;
            }

    if ((fillColor && gr.m_fillGradientFlag == Agg2D::RadialWithFocus) ||
      (!fillColor && gr.m_lineGradientFlag == Agg2D::RadialWithFocus))
    {
      if (fillColor)
      {
        agg::gradient_radial_focus gradient_func(100.0f, gr.m_fillGradientD1, gr.m_fillGradientD2);

        render_gradient
        (
          gr,
          renBase,
          gr.m_fillGradientSpreadMethod,
                                                   gr.m_fillGradient,
          gradient_func,
          gr.m_fillGradientInterpolator,
          0.,
          100.,
          gr.m_fillGradientOpacity
        );
      }
      else
      {
        agg::gradient_radial_focus gradient_func(100.0f, gr.m_lineGradientD1, gr.m_lineGradientD2);

        render_gradient
        (
          gr,
          renBase,
          gr.m_lineGradientSpreadMethod,
          gr.m_lineGradient,
          gradient_func,
          gr.m_lineGradientInterpolator,
          0.,
          100.,
          gr.m_lineGradientOpacity
        );
      }
      return;
            }

        if( ( fillColor && gr.m_fillGradientFlag == Agg2D::Pattern) ||
            (!fillColor && gr.m_lineGradientFlag == Agg2D::Pattern) )
        {
          // TODO!!!

          if (fillColor)
          {
            render_pattern
            (
              gr,
              renBase,
              gr.m_fillGradientSpreadMethod,
              gr.m_fillPattern.pattern,
              gr.m_fillPattern.offset_x,
              gr.m_fillPattern.offset_y,
              gr.m_fillPattern.opacity
           );
          }
          else
          {
            render_pattern
            (
              gr,
              renBase,
              gr.m_fillGradientSpreadMethod,
              gr.m_fillPattern.pattern,
              gr.m_fillPattern.offset_x,
              gr.m_fillPattern.offset_y,
              gr.m_fillPattern.opacity
           );
          }
          return;
        }

                renSolid.color(fillColor ? gr.m_fillColor : gr.m_lineColor);
                agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, renSolid);
            }


    //--------------------------------------------------------------------
    class SpanConvImageBlend
    {
    public:
        SpanConvImageBlend(Agg2D::BlendMode m, Agg2D::Color c) :
            m_mode(m), m_color(c)
        {}

        void convert(Agg2D::Color* span, int x, int y, unsigned len) const
        {
            unsigned l2;
            Agg2D::Color* s2;
            if(m_mode != Agg2D::BlendDst)
            {
                l2 = len;
                s2 = span;
                typedef agg::comp_op_adaptor_clip_to_dst_rgba_pre<Agg2D::Color, agg::order_rgba> OpType;
                do
                {
                    OpType::blend_pix(m_mode,
                                      (Agg2D::Color::value_type*)s2,
                                      m_color.r,
                                      m_color.g,
                                      m_color.b,
                                      Agg2D::Color::base_mask,
                                      agg::cover_full);
                    ++s2;
                }
                while(--l2);
            }
            if(m_color.a < Agg2D::Color::base_mask)
            {
                l2 = len;
                s2 = span;
                unsigned a = m_color.a;
                do
                {
                    s2->r = (s2->r * a) >> Agg2D::Color::base_shift;
                    s2->g = (s2->g * a) >> Agg2D::Color::base_shift;
                    s2->b = (s2->b * a) >> Agg2D::Color::base_shift;
                    s2->a = (s2->a * a) >> Agg2D::Color::base_shift;
                    ++s2;
                }
                while(--l2);
            }
        }

    private:
        Agg2D::BlendMode m_mode;
        Agg2D::Color     m_color;
    };




    //--------------------------------------------------------------------
    template<class BaseRenderer, class SolidRenderer, class Rasterizer, class Scanline>
    void static render(Agg2D& gr, BaseRenderer& renBase, SolidRenderer& renSolid, Rasterizer& ras, Scanline& sl)
    {
    // JME
    typedef agg::span_allocator<agg::rgba8> span_allocator_type;
        typedef agg::renderer_scanline_aa<BaseRenderer,span_allocator_type,LinearGradientSpan> RendererLinearGradient;
        typedef agg::renderer_scanline_aa<BaseRenderer,span_allocator_type,RadialGradientSpan> RendererRadialGradient;

        if(gr.m_fillGradientFlag == Agg2D::Linear)
        {
            gradient_opacity lut_with_opacity(gr.m_fillGradient, gr.m_fillGradientOpacity);
            LinearGradientSpan span(
                                           gr.m_fillGradientInterpolator,
                                           gr.m_linearGradientFunction,
                                           lut_with_opacity,
                                           gr.m_fillGradientD1,
                                           gr.m_fillGradientD2);
            RendererLinearGradient ren(renBase,gr.m_allocator,span);
            agg::render_scanlines(ras, sl, ren);
        }
        else
        {
            gradient_opacity lut_with_opacity(gr.m_fillGradient, gr.m_fillGradientOpacity);
            if(gr.m_fillGradientFlag == Agg2D::Radial)
            {
                RadialGradientSpan span(
                                               gr.m_fillGradientInterpolator,
                                               gr.m_radialGradientFunction,
                                               lut_with_opacity,
                                               gr.m_fillGradientD1,
                                               gr.m_fillGradientD2);
                RendererRadialGradient ren(renBase,gr.m_allocator,span);
                agg::render_scanlines(ras, sl, ren);
            }
            else
            {
                renSolid.color(gr.m_fillColor);
                agg::render_scanlines(ras, sl, renSolid);
            }
        }
    }



    //--------------------------------------------------------------------
    //! JME - this is where the bulk of the changes have taken place.
    template<class BaseRenderer, class Interpolator>
    static void renderImage(Agg2D& gr, const Agg2D::Image& img,
                            BaseRenderer& renBase, Interpolator& interpolator)
    {
    //! JME - have not quite figured which part of this is not const-correct
    // hence the cast.
        Agg2D::Image& imgc = const_cast<Agg2D::Image&>(img);
        Agg2D::PixFormat img_pixf(imgc.renBuf);
        typedef agg::image_accessor_clone<Agg2D::PixFormat> img_source_type;
        img_source_type source(img_pixf);

        SpanConvImageBlend blend(gr.m_imageBlendMode, gr.m_imageBlendColor);
        if(gr.m_imageFilter == Agg2D::NoFilter)
        {
          typedef agg::span_image_filter_rgba_nn<img_source_type,Interpolator> SpanGenType;
          typedef agg::span_converter<SpanGenType,SpanConvImageBlend> SpanConvType;
          typedef agg::renderer_scanline_aa<BaseRenderer,Agg2D::SpanAllocator,SpanGenType> RendererType;

          SpanGenType sg(source,interpolator);
          SpanConvType sc(sg, blend);
          RendererType ri(renBase,gr.m_allocator,sg);
          agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ri);
        }
        else
        {
            bool resample = (gr.m_imageResample == Agg2D::ResampleAlways);
            if(gr.m_imageResample == Agg2D::ResampleOnZoomOut)
            {
                real sx, sy;
                interpolator.transformer().scaling_abs(&sx, &sy);
                if (sx > 1.125 || sy > 1.125)
                {
                   resample = true;
                }
            }

            if(resample)
            {
                typedef agg::span_image_resample_rgba_affine<img_source_type> SpanGenType;
                typedef agg::span_converter<SpanGenType,SpanConvImageBlend> SpanConvType;
                typedef agg::renderer_scanline_aa<BaseRenderer,Agg2D::SpanAllocator,SpanGenType> RendererType;

                SpanGenType sg(source,interpolator,gr.m_imageFilterLut);
                SpanConvType sc(sg, blend);
                RendererType ri(renBase,gr.m_allocator,sg);
                agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ri);
            }
            else
            {
        // this is the AGG2D default
                if(gr.m_imageFilter == Agg2D::Bilinear)
                {

                    typedef agg::span_image_filter_rgba_bilinear<img_source_type,Interpolator> SpanGenType;
                    typedef agg::span_converter<SpanGenType,SpanConvImageBlend> SpanConvType;
                    typedef agg::renderer_scanline_aa<BaseRenderer,Agg2D::SpanAllocator,SpanGenType> RendererType;

                    SpanGenType sg(source,interpolator);
                    SpanConvType sc(sg, blend);
                    RendererType ri(renBase,gr.m_allocator,sg);
                    agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ri);
                }
                else
                {
                    if(gr.m_imageFilterLut.diameter() == 2)
                    {
                        typedef agg::span_image_filter_rgba_2x2<img_source_type,Interpolator> SpanGenType;
                        typedef agg::span_converter<SpanGenType,SpanConvImageBlend> SpanConvType;
                        typedef agg::renderer_scanline_aa<BaseRenderer,Agg2D::SpanAllocator,SpanGenType> RendererType;

                        SpanGenType sg(source,interpolator,gr.m_imageFilterLut);
                        SpanConvType sc(sg, blend);
                        RendererType ri(renBase,gr.m_allocator,sg);
                        agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ri);
                    }
                    else
                    {
                        typedef agg::span_image_filter_rgba<img_source_type,Interpolator> SpanGenType;
                        typedef agg::span_converter<SpanGenType,SpanConvImageBlend> SpanConvType;
                        typedef agg::renderer_scanline_aa<BaseRenderer,Agg2D::SpanAllocator,SpanGenType> RendererType;
                        SpanGenType sg(source,interpolator,gr.m_imageFilterLut);
                        SpanConvType sc(sg, blend);
                        RendererType ri(renBase,gr.m_allocator,sg);
                        agg::render_scanlines(gr.m_rasterizer, gr.m_scanline, ri);
                    }
                }
            }
        }
    }
};


//------------------------------------------------------------------------
void Agg2D::render(bool fillColor)
{
    if(m_blendMode == BlendAlpha)
    {
        Agg2DRenderer::render(*this, m_renBase, m_renSolid, fillColor);
    }
    else
    {
        Agg2DRenderer::render(*this, m_renBaseComp, m_renSolidComp, fillColor);
    }
}
#if !defined( UNDER_CE )

//------------------------------------------------------------------------
void Agg2D::render(FontRasterizer& ras, FontScanline& sl)
{
    if(m_blendMode == BlendAlpha)
    {
        Agg2DRenderer::render(*this, m_renBase, m_renSolid, ras, sl);
    }
    else
    {
        Agg2DRenderer::render(*this, m_renBaseComp, m_renSolidComp, ras, sl);
    }
}

#endif


//------------------------------------------------------------------------
void Agg2D::renderImage(const Image& img, int x1, int y1, int x2, int y2,
                        const real* parl)
{
    agg::trans_affine mtx((real)x1,
                          (real)y1,
                          (real)x2,
                          (real)y2,
                          parl);
    mtx *= m_transform;
    mtx.invert();

    m_rasterizer.reset();
    m_rasterizer.add_path(m_pathTransform);

    typedef agg::span_interpolator_linear<agg::trans_affine> Interpolator;
    Interpolator interpolator(mtx);

    if(m_blendMode == BlendAlpha)
    {
    // JME audit -
         Agg2DRenderer::renderImage(*this, img, m_renBasePre, interpolator);
    }
    else
    {
        Agg2DRenderer::renderImage(*this, img, m_renBaseCompPre, interpolator);
    }
}

//------------------------------------------------------------------------
struct Agg2DRasterizerGamma
{

    Agg2DRasterizerGamma(real alpha, real gamma) :
        m_alpha(alpha), m_gamma(gamma) {}

    real operator() (real x) const
    {
        return m_alpha(m_gamma(x));
    }
    agg::gamma_multiply m_alpha;
    agg::gamma_power    m_gamma;
};

//------------------------------------------------------------------------
void Agg2D::updateRasterizerGamma()
{
    m_rasterizer.gamma(Agg2DRasterizerGamma(m_masterAlpha, m_antiAliasGamma));
}

//------------------------------------------------------------------------
void Agg2D::blendImage(Image& img,
                       int imgX1, int imgY1, int imgX2, int imgY2,
                       real dstX, real dstY, unsigned alpha)
{
    worldToScreen(dstX, dstY);
    PixFormat pixF(img.renBuf);
    // JME
    //agg::rect r(imgX1, imgY1, imgX2, imgY2);
    Rect r(imgX1, imgY1, imgX2, imgY2);
    if(m_blendMode == BlendAlpha)
    {
        m_renBasePre.blend_from(pixF, &r, int(dstX)-imgX1, int(dstY)-imgY1, alpha);
    }
    else
    {
        m_renBaseCompPre.blend_from(pixF, &r, int(dstX)-imgX1, int(dstY)-imgY1, alpha);
    }
}


//------------------------------------------------------------------------
void Agg2D::blendImage(Image& img, real dstX, real dstY, unsigned alpha)
{
    worldToScreen(dstX, dstY);
    PixFormat pixF(img.renBuf);
    m_renBasePre.blend_from(pixF, 0, int(dstX), int(dstY), alpha);
    if(m_blendMode == BlendAlpha)
    {
        m_renBasePre.blend_from(pixF, 0, int(dstX), int(dstY), alpha);
    }
    else
    {
        m_renBaseCompPre.blend_from(pixF, 0, int(dstX), int(dstY), alpha);
    }
}


//------------------------------------------------------------------------
void Agg2D::copyImage(Image& img,
                      int imgX1, int imgY1, int imgX2, int imgY2,
                      real dstX, real dstY)
{
    worldToScreen(dstX, dstY);
    // JME
    //agg::rect r(imgX1, imgY1, imgX2, imgY2);
    Rect r(imgX1, imgY1, imgX2, imgY2);
    m_renBase.copy_from(img.renBuf, &r, int(dstX)-imgX1, int(dstY)-imgY1);
}

//------------------------------------------------------------------------
void Agg2D::copyImage(Image& img, real dstX, real dstY)
{
    worldToScreen(dstX, dstY);
    m_renBase.copy_from(img.renBuf, 0, int(dstX), int(dstY));
}

//------------------------------------------------------------------------
void Agg2D::Image::premultiply()
{
    PixFormat pixf(renBuf);
    pixf.premultiply();
}

//------------------------------------------------------------------------
void Agg2D::Image::demultiply()
{
    PixFormat pixf(renBuf);
    pixf.demultiply();
}


