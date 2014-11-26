//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
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
//
// classes bezier_ctrl_impl, bezier_ctrl
//
//----------------------------------------------------------------------------

#ifndef AGG_BEZIER_CTRL_INCLUDED
#define AGG_BEZIER_CTRL_INCLUDED

#include "agg_math.h"
#include "agg_ellipse.h"
#include "agg_trans_affine.h"
#include "agg_color_rgba.h"
#include "agg_conv_stroke.h"
#include "agg_conv_curve.h"
#include "agg_polygon_ctrl.h"


namespace agg
{

    //--------------------------------------------------------bezier_ctrl_impl
    class bezier_ctrl_impl : public ctrl
    {
    public:
        bezier_ctrl_impl();

        void curve(real x1, real y1, 
                   real x2, real y2, 
                   real x3, real y3,
                   real x4, real y4);
        curve4& curve();

        real x1() const { return m_poly.xn(0); }
        real y1() const { return m_poly.yn(0); }
        real x2() const { return m_poly.xn(1); }
        real y2() const { return m_poly.yn(1); }
        real x3() const { return m_poly.xn(2); }
        real y3() const { return m_poly.yn(2); }
        real x4() const { return m_poly.xn(3); }
        real y4() const { return m_poly.yn(3); }

        void x1(real x) { m_poly.xn(0) = x; }
        void y1(real y) { m_poly.yn(0) = y; }
        void x2(real x) { m_poly.xn(1) = x; }
        void y2(real y) { m_poly.yn(1) = y; }
        void x3(real x) { m_poly.xn(2) = x; }
        void y3(real y) { m_poly.yn(2) = y; }
        void x4(real x) { m_poly.xn(3) = x; }
        void y4(real y) { m_poly.yn(3) = y; }

        void   line_width(real w) { m_stroke.width(w); }
        real line_width() const   { return m_stroke.width(); }

        void   point_radius(real r) { m_poly.point_radius(r); }
        real point_radius() const   { return m_poly.point_radius(); }

        virtual bool in_rect(real x, real y) const;
        virtual bool on_mouse_button_down(real x, real y);
        virtual bool on_mouse_button_up(real x, real y);
        virtual bool on_mouse_move(real x, real y, bool button_flag);
        virtual bool on_arrow_keys(bool left, bool right, bool down, bool up);

        // Vertex source interface
        unsigned num_paths() { return 7; };
        void     rewind(unsigned path_id);
        unsigned vertex(real* x, real* y);


    private:
        curve4              m_curve;
        ellipse             m_ellipse;
        conv_stroke<curve4> m_stroke;
        polygon_ctrl_impl   m_poly;
        unsigned            m_idx;
    };



    //----------------------------------------------------------bezier_ctrl
    template<class ColorT> class bezier_ctrl : public bezier_ctrl_impl
    {
    public:
        bezier_ctrl() :
            m_color(rgba(0.0f, 0.0f, 0.0f))
        {
        }
          
        void line_color(const ColorT& c) { m_color = c; }
        const ColorT& color(unsigned i) const { return m_color; } 

    private:
        bezier_ctrl(const bezier_ctrl<ColorT>&);
        const bezier_ctrl<ColorT>& operator = (const bezier_ctrl<ColorT>&);

        ColorT m_color;
    };





    //--------------------------------------------------------curve3_ctrl_impl
    class curve3_ctrl_impl : public ctrl
    {
    public:
        curve3_ctrl_impl();

        void curve(real x1, real y1, 
                   real x2, real y2, 
                   real x3, real y3);
        curve3& curve();

        real x1() const { return m_poly.xn(0); }
        real y1() const { return m_poly.yn(0); }
        real x2() const { return m_poly.xn(1); }
        real y2() const { return m_poly.yn(1); }
        real x3() const { return m_poly.xn(2); }
        real y3() const { return m_poly.yn(2); }

        void x1(real x) { m_poly.xn(0) = x; }
        void y1(real y) { m_poly.yn(0) = y; }
        void x2(real x) { m_poly.xn(1) = x; }
        void y2(real y) { m_poly.yn(1) = y; }
        void x3(real x) { m_poly.xn(2) = x; }
        void y3(real y) { m_poly.yn(2) = y; }

        void   line_width(real w) { m_stroke.width(w); }
        real line_width() const   { return m_stroke.width(); }

        void   point_radius(real r) { m_poly.point_radius(r); }
        real point_radius() const   { return m_poly.point_radius(); }

        virtual bool in_rect(real x, real y) const;
        virtual bool on_mouse_button_down(real x, real y);
        virtual bool on_mouse_button_up(real x, real y);
        virtual bool on_mouse_move(real x, real y, bool button_flag);
        virtual bool on_arrow_keys(bool left, bool right, bool down, bool up);

        // Vertex source interface
        unsigned num_paths() { return 6; };
        void     rewind(unsigned path_id);
        unsigned vertex(real* x, real* y);


    private:
        curve3              m_curve;
        ellipse             m_ellipse;
        conv_stroke<curve3> m_stroke;
        polygon_ctrl_impl   m_poly;
        unsigned            m_idx;
    };



    //----------------------------------------------------------curve3_ctrl
    template<class ColorT> class curve3_ctrl : public curve3_ctrl_impl
    {
    public:
        curve3_ctrl() :
            m_color(rgba(0.0f, 0.0f, 0.0f))
        {
        }
          
        void line_color(const ColorT& c) { m_color = c; }
        const ColorT& color(unsigned i) const { return m_color; } 

    private:
        curve3_ctrl(const curve3_ctrl<ColorT>&);
        const curve3_ctrl<ColorT>& operator = (const curve3_ctrl<ColorT>&);

        ColorT m_color;
    };




}



#endif


