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

#ifndef AGG2D_INCLUDED
#define AGG2D_INCLUDED

// With this define uncommented you can use FreeType font engine

#if defined(UNDER_CE)
#define AGG2D_USE_FREETYPE 1
#endif

#pragma warning(disable: 4786)

// JME
#include "agg_basics.h"
#include "agg_trans_affine.h"
#include "agg_trans_viewport.h"
#include "agg_path_storage.h"
#include "agg_conv_stroke.h"
#include "agg_conv_transform.h"
#include "agg_conv_curve.h"
#include "agg_conv_dash.h"
#include "agg_rendering_buffer.h"
#include "agg_renderer_base.h"
#include "agg_renderer_scanline.h"
#include "agg_span_gradient.h"
#include "agg_span_image_filter_rgba.h"
//#include "agg_span_image_resample_rgba.h"
//+ JME
#include "agg_span_allocator.h"
#include "agg_span_converter.h"
#include "agg_span_interpolator_linear.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_gamma_functions.h"
#include "agg_scanline_u.h"
#include "agg_bezier_arc.h"
#include "agg_rounded_rect.h"
#include "agg_font_cache_manager.h"
#include "agg_math_stroke.h"
#include "agg_span_pattern_rgb.h"
#include "agg_renderer_scanline.h"

#ifdef AGG2D_USE_FREETYPE
#include "../font_freetype/agg_font_freetype.h"
#else
#include "../font_win32_tt/agg_font_win32_tt.h"
#endif

#include "agg_pixfmt_rgba.h"
//+ JME
#include "agg_image_accessors.h"

class Agg2D
{
    struct Dash
    {
        real dash_len;
        real gap_len;
        Dash() {}
        Dash( real d_, real g_ ) : dash_len(d_), gap_len(g_) {}
    };

    typedef agg::order_bgra ComponentOrder; // Platform dependent!

    typedef agg::rgba8                                               ColorType;
    typedef agg::blender_rgba<ColorType, ComponentOrder>             Blender;
    typedef agg::comp_op_adaptor_rgba<ColorType, ComponentOrder>     BlenderComp;
    typedef agg::blender_rgba_pre<ColorType, ComponentOrder>         BlenderPre;
    typedef agg::comp_op_adaptor_rgba_pre<ColorType, ComponentOrder> BlenderCompPre;

  // JME
    //typedef agg::pixel_formats_rgba<Blender, agg::pixel32_type>    PixFormat;
    typedef agg::pixfmt_bgra32                  PixFormat;
    // JME
    //typedef agg::pixfmt_custom_blend_rgba<BlenderComp,>             PixFormatComp;
    typedef agg::pixfmt_custom_blend_rgba<BlenderComp,agg::rendering_buffer>             PixFormatComp;
  // JME
    //typedef agg::pixel_formats_rgba<BlenderPre, agg::pixel32_type> PixFormatPre;
    typedef agg::pixfmt_bgra32_pre PixFormatPre;
    // JME
    //typedef agg::pixfmt_custom_blend_rgba<BlenderCompPre>          PixFormatCompPre;
    typedef agg::pixfmt_custom_blend_rgba<BlenderCompPre,agg::rendering_buffer>          PixFormatCompPre;

    typedef agg::renderer_base<PixFormat>        RendererBase;
    typedef agg::renderer_base<PixFormatComp>    RendererBaseComp;
    typedef agg::renderer_base<PixFormatPre>     RendererBasePre;
    typedef agg::renderer_base<PixFormatCompPre> RendererBaseCompPre;

    typedef agg::renderer_scanline_aa_solid<RendererBase>     RendererSolid;
    typedef agg::renderer_scanline_aa_solid<RendererBaseComp> RendererSolidComp;

    typedef agg::span_allocator<ColorType> SpanAllocator;
    typedef agg::pod_auto_array<ColorType, 256> GradientArray;

    typedef agg::span_interpolator_linear<>     SpanInterpolator;

    typedef agg::pod_auto_vector<Dash, 16>          DashArray;


#ifdef AGG2D_USE_FREETYPE
    typedef agg::font_engine_freetype_int32       FontEngine;
#else
    typedef agg::font_engine_win32_tt_int32       FontEngine;
#endif
    typedef agg::font_cache_manager<FontEngine>   FontCacheManager;
    typedef FontCacheManager::gray8_adaptor_type  FontRasterizer;
    typedef FontCacheManager::gray8_scanline_type FontScanline;

    typedef agg::conv_curve<agg::path_storage>    ConvCurve;
    typedef agg::conv_stroke<ConvCurve>           ConvStroke;
    typedef agg::conv_dash<ConvCurve>               ConvDash;
    typedef agg::conv_stroke<agg::conv_dash<ConvCurve> > ConvDashStroke;
    typedef agg::conv_transform<ConvCurve>        PathTransform;
    typedef agg::conv_transform<ConvStroke>       StrokeTransform;
    typedef agg::conv_transform<ConvDashStroke>     DashStrokeTransform;
public:

    enum Gradient
    {
        Solid,
        Linear,
        Radial,
        RadialWithFocus,
        Pattern,
    };

    friend class Agg2DRenderer;

    typedef ColorType         Color;
    typedef GradientArray     gradient_lut_type;
    // JME
    //typedef agg::rect         Rect;
    typedef agg::rect_i       Rect;
    typedef agg::rect_d       RectD;
    typedef agg::trans_affine Affine;

    enum gradientUnits_e
    {
      gradientUnits_userSpaceOnUse,
      gradientUnits_objectBoundingBox
    };

    enum spreadMethod_e
    {
      spreadMethod_pad,
      spreadMethod_reflect,
      spreadMethod_repeat,
    };

    /*miter_join         = 0,
    miter_join_revert  = 1,
    round_join         = 2,
    bevel_join         = 3,
    miter_join_round   = 4*/

    enum LineJoin
    {
        MiterJoin = agg::miter_join,
        RoundJoin = agg::round_join,
        BevelJoin = agg::bevel_join
    };

    enum LineCap
    {
        LineCapButt   = agg::butt_cap,
        LineCapSquare = agg::square_cap,
        LineCapRound  = agg::round_cap
    };

    enum TextAlignment
    {
        AlignLeft,
        AlignRight,
        AlignCenter,
        AlignTop = AlignRight,
        AlignBottom = AlignLeft
    };


    enum DrawPathFlag
    {
        FillOnly,
        StrokeOnly,
        FillAndStroke,
        FillWithLineColor
    };

    enum ViewportOption
    {
        Anisotropic,
        XMinYMin,
        XMidYMin,
        XMaxYMin,
        XMinYMid,
        XMidYMid,
        XMaxYMid,
        XMinYMax,
        XMidYMax,
        XMaxYMax
    };

    enum WindowFitLogic
    {
        WindowFitLogic_meet,
        WindowFitLogic_slice
    };

    struct Transformations
    {
        real affineMatrix[6];
    };


    struct Image
    {
        agg::rendering_buffer renBuf;

        Image() {}
        Image(unsigned char* buf, unsigned width, unsigned height, int stride) :
            renBuf(buf, width, height, stride) {}
        void attach(unsigned char* buf, unsigned width, unsigned height, int stride)
        {
            renBuf.attach(buf, width, height, stride);
        }
        int width()  const { return renBuf.width(); }
        int height() const { return renBuf.height(); }
        void premultiply();
        void demultiply();
    };

    enum ImageFilter
    {
        NoFilter,
        Bilinear,
        Hanning,
        Hamming,
        Hermite,
        Quadric,
        Bicubic,
        Catrom,
        Spline16,
        Spline36,
        Blackman144
    };

    enum ImageResample
    {
        NoResample,
        ResampleAlways,
        ResampleOnZoomOut
    };

    enum FontCacheType
    {
        RasterFontCache,
        VectorFontCache
    };

    enum BlendMode
    {
        BlendAlpha      = agg::end_of_comp_op_e,
        BlendClear      = agg::comp_op_clear,
        BlendSrc        = agg::comp_op_src,
        BlendDst        = agg::comp_op_dst,
        BlendSrcOver    = agg::comp_op_src_over,
        BlendDstOver    = agg::comp_op_dst_over,
        BlendSrcIn      = agg::comp_op_src_in,
        BlendDstIn      = agg::comp_op_dst_in,
        BlendSrcOut     = agg::comp_op_src_out,
        BlendDstOut     = agg::comp_op_dst_out,
        BlendSrcAtop    = agg::comp_op_src_atop,
        BlendDstAtop    = agg::comp_op_dst_atop,
        BlendXor        = agg::comp_op_xor,
        BlendAdd        = agg::comp_op_plus,
        BlendSub        = agg::comp_op_minus,
        BlendMultiply   = agg::comp_op_multiply,
        BlendScreen     = agg::comp_op_screen,
        BlendOverlay    = agg::comp_op_overlay,
        BlendDarken     = agg::comp_op_darken,
        BlendLighten    = agg::comp_op_lighten,
        BlendColorDodge = agg::comp_op_color_dodge,
        BlendColorBurn  = agg::comp_op_color_burn,
        BlendHardLight  = agg::comp_op_hard_light,
        BlendSoftLight  = agg::comp_op_soft_light,
        BlendDifference = agg::comp_op_difference,
        BlendExclusion  = agg::comp_op_exclusion,
        BlendContrast   = agg::comp_op_contrast,
        BlendInvert     = agg::comp_op_invert,
        BlendInvertRGB  = agg::comp_op_invert_rgb,
    };

    enum Direction
    {
        CW, CCW
    };

    struct PatternFillDesc
    {
        spreadMethod_e      spreadMethod;
        Image               pattern; 
        int                 offset_x;
        int                 offset_y;
        unsigned            opacity;  // [0..255]
    };

    struct State
    {
      RectD                           m_clipBox;

      BlendMode                       m_blendMode;
      BlendMode                       m_imageBlendMode;
      Color                           m_imageBlendColor;

      real                            m_masterAlpha;
      real                            m_antiAliasGamma;

      Color                           m_fillColor;
      Color                           m_lineColor;
      GradientArray                   m_fillGradient;
      GradientArray                   m_lineGradient;

      LineCap                         m_lineCap;
      LineJoin                        m_lineJoin;

      Gradient                        m_fillGradientFlag;
      Gradient                        m_lineGradientFlag;
      agg::trans_affine               m_fillGradientMatrix;
      agg::trans_affine               m_lineGradientMatrix;
      real                            m_fillGradientD1;
      real                            m_lineGradientD1;
      real                            m_fillGradientD2;
      real                            m_lineGradientD2;

      real                            m_textAngle;
      TextAlignment                   m_textAlignX;
      TextAlignment                   m_textAlignY;
      bool                            m_textHints;
      real                            m_fontHeight;
      real                            m_fontAscent;
      real                            m_fontDescent;
      FontCacheType                   m_fontCacheType;

      real                            m_lineWidth;
      bool                            m_evenOddFlag;

      agg::trans_affine               m_transform;
      agg::trans_affine               m_affine;

      real                            m_dashStart;
      DashArray                       m_dashes;

      PatternFillDesc                 m_fillPattern;
    };

    class gradient
    {
    public:
        gradient();
        //---------------------------------------------------------------------
        void reset();

        void type(Gradient gt);
        void gradientUnits(gradientUnits_e);
        void spreadMethod(spreadMethod_e);

        void linear(real x1, real y1, real x2, real y2);
        void radial(real cx, real cy, real r);
        void focus(real fx, real fy);
        //---------------------------------------------------------------------
        void x1(real v) { m_x1 = v; }
        void y1(real v) { m_y1 = v; }
        void x2(real v) { m_x2 = v; }
        void y2(real v) { m_y2 = v; }
        void cx(real v) { m_cx = v; }
        void cy(real v) { m_cy = v; }
        void  r(real v) { m_r  = v; }
        void fx(real v) { m_fx = v; }
        void fy(real v) { m_fy = v; }
        //---------------------------------------------------------------------
        void add_stop(real offset, const Color& color, real opacity);
        //--
        //-- getters ----------------------------------------------------------
        Gradient type()const {return m_type;             }
        //---------------------------------------------------------------------
        gradientUnits_e gradientUnits()const {return m_gradientUnits; }
        //---------------------------------------------------------------------
        spreadMethod_e  spreadMethod()      const {return m_spreadMethod;     }
        //---------------------------------------------------------------------
        const agg::trans_affine& transform()const {return m_gradientTransform;}
        //---------------------------------------------------------------------
        const gradient_lut_type& lut()      const {return m_color_lut;        }
        //---------------------------------------------------------------------
        real x1()const { return m_x1; }
        real y1()const { return m_y1; }
        real x2()const { return m_x2; }
        real y2()const { return m_y2; }
        real cx()const { return m_cx; }
        real cy()const { return m_cy; }
        real  r()const { return m_r ; }
        real fx()const { return m_fx; }
        real fy()const { return m_fy; }
        //--
        //-- transformations --------------------------------------------------
        void matrix(real a0, real a1, real a2,
                    real a3, real a4, real a5);
        //---------------------------------------------------------------------
        void translate(real dx, real dy);
        //---------------------------------------------------------------------
        void rotate(real angle);
        //---------------------------------------------------------------------
        void rotate(real angle, real cx, real cy);
        //---------------------------------------------------------------------
        void scale(real s);
        //---------------------------------------------------------------------
        void scale(real sx, real sy);
        //---------------------------------------------------------------------
        void skew(real sx, real sy);
        //---------------------------------------------------------------------
        void skew_x(real s);
        //---------------------------------------------------------------------
        void skew_y(real s);
        //--
        //---------------------------------------------------------------------
        void build_lut();
        //---------------------------------------------------------------------
    private:
        //---------------------------------------------------------------------
        struct color_stop
        {
            real     offset;
            Color      color;

            color_stop() {}
            color_stop(real offset, const Color& color) : offset(offset), color(color) {}
        };
        //---------------------------------------------------------------------
        typedef agg::pod_bvector<color_stop> stop_array_type;
        //---------------------------------------------------------------------
        static bool offset_less(const color_stop& a, const color_stop& b)
        {
            return a.offset < b.offset;
        }
        //---------------------------------------------------------------------
        static bool offset_equal(const color_stop& a, const color_stop& b)
        {
            return a.offset == b.offset;
        }
        //---------------------------------------------------------------------
        Gradient  m_type;
        gradientUnits_e   m_gradientUnits;
        spreadMethod_e    m_spreadMethod;
        agg::trans_affine m_gradientTransform;
        real            m_x1, m_y1, m_x2, m_y2;
        real            m_cx, m_cy, m_r, m_fx, m_fy;
        stop_array_type   m_colors;
        gradient_lut_type m_color_lut;

        struct color_interpolator
        {
        //---------------------------------------------------------------------
            color_interpolator(const Color& c1,
                               const Color& c2,
                               unsigned len) :
                r(c1.r, c2.r, len),
                g(c1.g, c2.g, len),
                b(c1.b, c2.b, len),
                a(c1.a, c2.a, len)
            {}
        //---------------------------------------------------------------------
            void operator ++ ()
            {
                ++r; ++g; ++b; ++a;
            }
        //---------------------------------------------------------------------
            Color color() const
            {
                return Color(r.y(), g.y(), b.y(), a.y());
            }
        //---------------------------------------------------------------------
        private:
            agg::dda_line_interpolator<14> r, g, b, a;
        };
    };


    ~Agg2D();
    Agg2D();

    // Setup
    //-----------------------
    void  attach(unsigned char* buf, unsigned width, unsigned height, int stride);
    void  attach(Image& img);

    void  clipBox(real x1, real y1, real x2, real y2);
    RectD clipBox() const;

    void  clearAll(Color c);
    void  clearAll(unsigned r, unsigned g, unsigned b, unsigned a = 255);

    void  clearClipBox(Color c);
    void  clearClipBox(unsigned r, unsigned g, unsigned b, unsigned a = 255);

    unsigned width()  const { return m_rbuf.width();  }
    unsigned height() const { return m_rbuf.height(); }

    // Conversions
    //-----------------------
    void   worldToScreen(real& x, real& y) const;
    void   screenToWorld(real& x, real& y) const;
    real   worldToScreen(real scalar) const;
    real   screenToWorld(real scalar) const;
    void   alignPoint(real& x, real& y) const;
    bool   inBox(real worldX, real worldY) const;

    // General Attributes
    //-----------------------
    void blendMode(BlendMode m);
    BlendMode blendMode() const;
    //-------------------------------------------------------------------------
    void imageBlendMode(BlendMode m);
    BlendMode imageBlendMode() const;
    //-------------------------------------------------------------------------
    void imageBlendColor(Color c);
    void imageBlendColor(unsigned r, unsigned g, unsigned b, unsigned a = 255);
    Color imageBlendColor() const;
    //-------------------------------------------------------------------------
    void masterAlpha(real a);
    real masterAlpha() const;
    //-------------------------------------------------------------------------
    void antiAliasGamma(real g);
    real antiAliasGamma() const;
    //-------------------------------------------------------------------------
    void fillColor(Color c);
    void fillColor(unsigned r, unsigned g, unsigned b, unsigned a = 255);
    void noFill();
    //-------------------------------------------------------------------------
    void fillPattern(const Image& img, unsigned opacity = 255, int offset_x = 0, int offset_y = 0);
    //-------------------------------------------------------------------------
    void lineColor(Color c);
    void lineColor(unsigned r, unsigned g, unsigned b, unsigned a = 255);
    void noLine();
    //-------------------------------------------------------------------------
    Color fillColor() const;
    Color lineColor() const;
    //-------------------------------------------------------------------------
  void fillLinearGradient( const gradient_lut_type&
                         , real x1, real y1, real x2, real y2
                         , const agg::trans_affine& gradientTransform
                         , spreadMethod_e
                         , gradientUnits_e units
                         , real opacity );
    //-------------------------------------------------------------------------
  void lineLinearGradient( const gradient_lut_type&
                         , real x1, real y1, real x2, real y2
                         , const agg::trans_affine& gradientTransform
                         , spreadMethod_e
                         , gradientUnits_e units
                         , real opacity );
    //-------------------------------------------------------------------------
  void fillRadialGradient( const gradient_lut_type&
                         , real cx, real cy, real r
                         , real fx, real fy
                         , const agg::trans_affine& gradientTransform
                         , spreadMethod_e
                         , gradientUnits_e units
                         , real opacity );
    //-------------------------------------------------------------------------
  void lineRadialGradient( const gradient_lut_type&
                         , real cx, real cy, real r
                         , real fx, real fy
                         , const agg::trans_affine& gradientTransform
                         , spreadMethod_e
                         , gradientUnits_e units
                         , real opacity);
    //-------------------------------------------------------------------------
    void fillLinearGradient(real x1, real y1, real x2, real y2, Color c1, Color c2, real profile=1.0f);
    void lineLinearGradient(real x1, real y1, real x2, real y2, Color c1, Color c2, real profile=1.0f);
    //-------------------------------------------------------------------------
    void fillRadialGradient(real x, real y, real r, Color c1, Color c2, real profile=1.0f);
    void lineRadialGradient(real x, real y, real r, Color c1, Color c2, real profile=1.0f);
    //-------------------------------------------------------------------------
    void fillRadialGradient(real x, real y, real r, Color c1, Color c2, Color c3);
    void lineRadialGradient(real x, real y, real r, Color c1, Color c2, Color c3);
    //-------------------------------------------------------------------------
    void fillRadialGradient(real x, real y, real r);
    void lineRadialGradient(real x, real y, real r);
    //-------------------------------------------------------------------------
    void lineWidth(real w);
    real lineWidth(real w) const;

    void lineCap(LineCap cap);
    LineCap lineCap() const;

    void lineJoin(LineJoin join);
    LineJoin lineJoin() const;

    void fillEvenOdd(bool evenOddFlag);
    bool fillEvenOdd() const;

    // Transformations
    //-----------------------
    Transformations transformations() const;
    void transformations(const Transformations& tr);

    const Affine& affine() const;
    void          affine(const Affine&);

    void resetTransformations();
    void matrix(const Affine& tr);
    void matrix(const Transformations& tr);
    void rotate(real angle);
    void rotate(real angle, real cx, real cy);
    void scale(real s);
    void scale(real sx, real sy);
    void skew(real sx, real sy);
    void translate(real x, real y);
    void parallelogram(real x1, real y1, real x2, real y2, const real* para);
    void viewport(real worldX1,  real worldY1,  real worldX2,  real worldY2,
                  real screenX1, real screenY1, real screenX2, real screenY2,
                  ViewportOption opt=XMidYMid, WindowFitLogic fl = WindowFitLogic_meet);

    // Basic Shapes
    //-----------------------
    void line(real x1, real y1, real x2, real y2);
    bool arrow_bone(real x_from, real y_from, real x_to, real y_to, real sz, real angle_ext);
    bool arrow(real x_from, real y_from, real x_to, real y_to, real sz, real angle_ext, real offset_int);
    void triangle(real x1, real y1, real x2, real y2, real x3, real y3);
    void rectangle(real x1, real y1, real x2, real y2);
    void roundedRect(real x1, real y1, real x2, real y2, real r);
    void roundedRect(real x1, real y1, real x2, real y2, real rx, real ry);
    void roundedRect(real x1, real y1, real x2, real y2,
                     real rxBottom, real ryBottom,
                     real rxTop,    real ryTop);
    void roundedRect(real x1, real y1, real x2, real y2,
                     real rx1, real ry1, real rx2, real ry2,
                     real rx3, real ry3, real rx4, real ry4);

    void ellipse(real cx, real cy, real rx, real ry);
    void arc(real cx, real cy, real rx, real ry, real start, real sweep);
    void star(real cx, real cy, real r1, real r2, real startAngle, int numRays);
    void curve(real x1, real y1, real x2, real y2, real x3, real y3);
    void curve(real x1, real y1, real x2, real y2, real x3, real y3, real x4, real y4);
    void polygon(real* xy, int numPoints);
    void polyline(real* xy, int numPoints);


    // Text
    //-----------------------
    void   flipText(bool flip);
    void   font(const char* fileName, real height,
                bool bold = false,
                bool italic = false,
                FontCacheType ch = RasterFontCache,
                real angle = 0.0f);
    real fontHeight() const;
    real fontAscent() const;

    void   textAlignment(TextAlignment alignX, TextAlignment alignY);
    bool   textHints() const;
    void   textHints(bool hints);
    real textWidth(const char* str, unsigned int len);
    real textWidth(const wchar_t* str, unsigned int len);
    void   text(real x, real y, const char* str, unsigned int len, bool roundOff=false, real dx=0.0f, real dy=0.0f);
    void   text(real x, real y, const wchar_t* str, unsigned int len, bool roundOff=false, real dx=0.0f, real dy=0.0f);

    real textWidth(const char* str);
    void   text(real x, real y, const char* str, bool roundOff=false, real dx=0.0f, real dy=0.0f);


    // Path commands
    //-----------------------
    void resetPath();

    void moveTo(real x, real y);
    void moveRel(real dx, real dy);

    void lineTo(real x, real y);
    void lineRel(real dx, real dy);

    void horLineTo(real x);
    void horLineRel(real dx);

    void verLineTo(real y);
    void verLineRel(real dy);

    void arcTo(real rx, real ry,
               real angle,
               bool largeArcFlag,
               bool sweepFlag,
               real x, real y);

    void arcRel(real rx, real ry,
                real angle,
                bool largeArcFlag,
                bool sweepFlag,
                real dx, real dy);

    void quadricCurveTo(real xCtrl, real yCtrl,
                         real xTo,   real yTo);
    void quadricCurveRel(real dxCtrl, real dyCtrl,
                         real dxTo,   real dyTo);
    void quadricCurveTo(real xTo, real yTo);
    void quadricCurveRel(real dxTo, real dyTo);

    void cubicCurveTo(real xCtrl1, real yCtrl1,
                      real xCtrl2, real yCtrl2,
                      real xTo,    real yTo);

    void cubicCurveRel(real dxCtrl1, real dyCtrl1,
                       real dxCtrl2, real dyCtrl2,
                       real dxTo,    real dyTo);

    void cubicCurveTo(real xCtrl2, real yCtrl2,
                      real xTo,    real yTo);

    void cubicCurveRel(real xCtrl2, real yCtrl2,
                       real xTo,    real yTo);

    void addEllipse(real cx, real cy, real rx, real ry, Direction dir);
    void closePolygon();

    template <class VertexSource>
    void addPath(VertexSource& vs, unsigned path_id = 0)
    {
        m_path.concat_path(vs, path_id);
    }

    void drawPath(DrawPathFlag flag = FillAndStroke);
    void drawPathNoTransform(DrawPathFlag flag = FillAndStroke);

    // Effects
    //-----------------------
    void blur(unsigned rx, unsigned ry);

    // Image Transformations
    //-----------------------
    void imageFilter(ImageFilter f);
    ImageFilter imageFilter() const;

    void imageResample(ImageResample f);
    ImageResample imageResample() const;

    void transformImage(const Image& img,
                           int imgX1,    int imgY1,    int imgX2,    int imgY2,
                        real dstX1, real dstY1, real dstX2, real dstY2);

    void transformImage(const Image& img,
                        real dstX1, real dstY1, real dstX2, real dstY2);

    void transformImage(const Image& img,
                        int imgX1, int imgY1, int imgX2, int imgY2,
                        const real* parallelogram);

    void transformImage(const Image& img, const real* parallelogram);


    void transformImagePath(const Image& img,
                            int imgX1,    int imgY1,    int imgX2,    int imgY2,
                            real dstX1, real dstY1, real dstX2, real dstY2);

    void transformImagePath(const Image& img,
                            real dstX1, real dstY1, real dstX2, real dstY2);

    void transformImagePath(const Image& img,
                            int imgX1, int imgY1, int imgX2, int imgY2,
                            const real* parallelogram);

    void transformImagePath(const Image& img, const real* parallelogram);


    // Image Blending (no transformations available)
    void blendImage(Image& img,
                    int imgX1, int imgY1, int imgX2, int imgY2,
                    real dstX, real dstY, unsigned alpha=255);
    void blendImage(Image& img, real dstX, real dstY, unsigned alpha=255);


    // Copy image directly, together with alpha-channel
    void copyImage(Image& img,
                   int imgX1, int imgY1, int imgX2, int imgY2,
                   real dstX, real dstY);
    void copyImage(Image& img, real dstX, real dstY);


    // Line dashes
    //-----------------------
    
    void dashAdd( real dash_len, real gap_len );
    void dashStart( real dash_start );
    real dashStart()          const { return m_dashStart; }
    void dashClear();

    // State
    //-----------------------

    void saveStateTo(State& st);
    void restoreStateFrom(const State& st);


    // Auxiliary
    //-----------------------
    static real pi() { return agg::pi; }
    static real deg2Rad(real v) { return v * agg::pi / 180.0f; }
    static real rad2Deg(real v) { return v * 180.0f / agg::pi; }

private:
    void render(bool fillColor);

#if !defined( UNDER_CE )
    void render(FontRasterizer& ras, FontScanline& sl);
#endif

    void addLine(real x1, real y1, real x2, real y2);
    void updateRasterizerGamma();
    void renderImage(const Image& img, int x1, int y1, int x2, int y2, const real* parl);

    void updateTransformations();

    void addStrokePath();

    agg::rendering_buffer           m_rbuf;
    PixFormat                       m_pixFormat;
    PixFormatComp                   m_pixFormatComp;
    PixFormatPre                    m_pixFormatPre;
    PixFormatCompPre                m_pixFormatCompPre;
    RendererBase                    m_renBase;
    RendererBaseComp                m_renBaseComp;
    RendererBasePre                 m_renBasePre;
    RendererBaseCompPre             m_renBaseCompPre;
    RendererSolid                   m_renSolid;
    RendererSolidComp               m_renSolidComp;

    SpanAllocator                   m_allocator;
      RectD                           m_clipBox;

      BlendMode                       m_blendMode;
      BlendMode                       m_imageBlendMode;
      Color                           m_imageBlendColor;

    agg::scanline_u8                m_scanline;
    agg::rasterizer_scanline_aa<>   m_rasterizer;

      real                          m_masterAlpha;
      real                          m_antiAliasGamma;

      Color                           m_fillColor;
      Color                           m_lineColor;
      GradientArray                   m_fillGradient;
      GradientArray                   m_lineGradient;

      LineCap                         m_lineCap;
      LineJoin                        m_lineJoin;

      Gradient                        m_fillGradientFlag;
      Gradient                        m_lineGradientFlag;
      agg::trans_affine               m_fillGradientMatrix;
      agg::trans_affine               m_lineGradientMatrix;
      real                          m_fillGradientD1;
      real                          m_lineGradientD1;
      real                          m_fillGradientD2;
      real                          m_lineGradientD2;
      spreadMethod_e                  m_fillGradientSpreadMethod;
      spreadMethod_e                  m_lineGradientSpreadMethod;
      real                          m_fillGradientOpacity;
      real                          m_lineGradientOpacity;

      real                          m_textAngle;
      TextAlignment                   m_textAlignX;
      TextAlignment                   m_textAlignY;
      bool                            m_textHints;
      real                          m_fontHeight;
      real                          m_fontAscent;
      real                          m_fontDescent;
      FontCacheType                   m_fontCacheType;

    ImageFilter                     m_imageFilter;
    ImageResample                   m_imageResample;
    agg::image_filter_lut           m_imageFilterLut;

    SpanInterpolator                m_fillGradientInterpolator;
    SpanInterpolator                m_lineGradientInterpolator;

    agg::gradient_x                 m_linearGradientFunction;
    agg::gradient_radial            m_radialGradientFunction;

    real                          m_lineWidth;
    bool                            m_evenOddFlag;

    real                          m_start_x;
    real                          m_start_y;

    agg::path_storage               m_path;
    agg::trans_affine               m_transform;

    agg::trans_affine               m_viewport;
    agg::trans_affine               m_affine;

    ConvCurve                       m_convCurve;
    ConvStroke                      m_convStroke;
    ConvDash                        m_convDash;
    ConvDashStroke                  m_convDashStroke;

    PathTransform                   m_pathTransform;
    StrokeTransform                 m_strokeTransform;
    DashStrokeTransform             m_dashStrokeTransform;

    real                            m_dashStart;
    DashArray                       m_dashes;

    PatternFillDesc                 m_fillPattern;

#ifndef AGG2D_USE_FREETYPE
    HDC                             m_fontDC;
#endif
    FontEngine                      m_fontEngine;
    FontCacheManager                m_fontCacheManager;

};

inline bool operator == (const Agg2D::Color& c1, const Agg2D::Color& c2)
{
   return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}

inline bool operator != (const Agg2D::Color& c1, const Agg2D::Color& c2)
{
   return !(c1 == c2);
}


  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::type(Gradient type)
  {
      m_type = type;
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::gradientUnits(gradientUnits_e u)
  {
      m_gradientUnits = u;
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::spreadMethod(spreadMethod_e m)
  {
      m_spreadMethod = m;
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::linear(real x1, real y1, real x2, real y2)
  {
      m_type = Linear;
      m_x1 = x1;
      m_y1 = y1;
      m_x2 = x2;
      m_y2 = y2;
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::radial(real cx, real cy, real r)
  {
      m_type = Radial;
      m_cx = m_fx = cx;
      m_cy = m_fy = cy;
      m_r = r;
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::focus(real fx, real fy)
  {
      m_fx = fx;
      m_fy = fy;
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::add_stop(real            offset,
                                 const Color& color,
                                 real            opacity)
  {
      if(offset < 0.0) offset = 0.0;
      if(offset > 1.0) offset = 1.0;

      Color c = color;

      c.a = (Color::value_type)(c.a * opacity + 0.5);

      m_colors.add(color_stop(offset, c));
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::matrix(real a0, real a1, real a2,
                 real a3, real a4, real a5)
  {
      m_gradientTransform.premultiply(agg::trans_affine(a0, a1, a2, a3, a4, a5));
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::translate(real dx, real dy)
  {
      m_gradientTransform.premultiply(agg::trans_affine_translation(dx, dy));
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::rotate(real angle)
  {
      m_gradientTransform.premultiply(agg::trans_affine_rotation(angle));
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::rotate(real angle, real cx, real cy)
  {
      agg::trans_affine_translation m(-cx, -cy);
      m *= agg::trans_affine_rotation(angle);
      m *= agg::trans_affine_translation(cx, cy);
      m_gradientTransform.premultiply(m);
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::scale(real s)
  {
      m_gradientTransform.premultiply(agg::trans_affine_scaling(s));
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::scale(real sx, real sy)
  {
      m_gradientTransform.premultiply(agg::trans_affine_scaling(sx, sy));
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::skew(real sx, real sy)
  {
      m_gradientTransform.premultiply(agg::trans_affine_skewing(sx, sy));
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::skew_x(real s)
  {
      m_gradientTransform.premultiply(agg::trans_affine_skewing(s, 0.0));
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::skew_y(real s)
  {
      m_gradientTransform.premultiply(agg::trans_affine_skewing(0.0, s));
  }
  //-------------------------------------------------------------------------

  inline Agg2D::gradient::gradient()
      :
      m_type(Linear),
      m_gradientUnits(gradientUnits_userSpaceOnUse),
      m_spreadMethod(spreadMethod_pad),
      m_gradientTransform(),
      m_x1(0), m_y1(0), m_x2(1), m_y2(0),
      m_cx(0.5), m_cy(0.5), m_r(0.5), m_fx(0.5), m_fy(0.5)
  {
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::reset()
  {
        m_type = Linear;
        m_gradientUnits = gradientUnits_userSpaceOnUse;
        m_spreadMethod  = spreadMethod_pad;
        m_gradientTransform.reset();
        m_x1 = 0;
        m_y1 = 0;
        m_x2 = 1;
        m_y2 = 0;
        m_cx = 0.5;
        m_cy = 0.5;
        m_r  = 0.5;
        m_fx = 0.5;
        m_fy = 0.5;
        m_colors.clear();
  }
  //-------------------------------------------------------------------------
  inline void Agg2D::gradient::build_lut()
  {
      typedef color_interpolator interpolator_type;

      const unsigned color_lut_size = m_color_lut.size();

      agg::quick_sort(m_colors, offset_less);
      m_colors.cut_at(agg::remove_duplicates(m_colors, offset_equal));

      if(m_colors.size() >= 2)
      {
          unsigned i;
          unsigned start = agg::uround(m_colors[0].offset * color_lut_size);
          unsigned end;
          Color c = m_colors[0].color;
          for(i = 0; i < start; i++)
          {
              m_color_lut[i] = c;
          }
          for(i = 1; i < m_colors.size(); i++)
          {
              end  = agg::uround(m_colors[i].offset * color_lut_size);
              interpolator_type ci(m_colors[i-1].color,
                                   m_colors[i  ].color,
                                   end - start + 1);
              while(start < end)
              {
                  m_color_lut[start] = ci.color();
                  ++ci;
                  ++start;
              }
          }
          c = m_colors.last().color;
          for(; end < color_lut_size; end++)
          {
              m_color_lut[end] = c;
          }
      }
  }


#endif



