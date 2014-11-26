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
// classes scale_ctrl_impl, scale_ctrl
//
//----------------------------------------------------------------------------

#ifndef AGG_SCALE_CTRL_INCLUDED
#define AGG_SCALE_CTRL_INCLUDED

#include "agg_basics.h"
#include "agg_math.h"
#include "agg_ellipse.h"
#include "agg_trans_affine.h"
#include "agg_color_rgba.h"
#include "agg_ctrl.h"


namespace agg
{

    //------------------------------------------------------------------------
    class scale_ctrl_impl : public ctrl
    {
        enum move_e
        {
            move_nothing,
            move_value1,
            move_value2,
            move_slider
        };

    public:
        scale_ctrl_impl(real x1, real y1, real x2, real y2, bool flip_y=false);

        void border_thickness(real t, real extra=0.0f);
        void resize(real x1, real y1, real x2, real y2);
        
        real min_delta() const { return m_min_d; }
        void min_delta(real d) { m_min_d = d; }
        
        real value1() const { return m_value1; }
        void value1(real value);

        real value2() const { return m_value2; }
        void value2(real value);

        void move(real d);

        virtual bool in_rect(real x, real y) const;
        virtual bool on_mouse_button_down(real x, real y);
        virtual bool on_mouse_button_up(real x, real y);
        virtual bool on_mouse_move(real x, real y, bool button_flag);
        virtual bool on_arrow_keys(bool left, bool right, bool down, bool up);

        // Vertex soutce interface
        unsigned num_paths() { return 5; };
        void     rewind(unsigned path_id);
        unsigned vertex(real* x, real* y);

    private:
        void calc_box();

        real   m_border_thickness;
        real   m_border_extra;
        real   m_value1;
        real   m_value2;
        real   m_min_d;
        real   m_xs1;
        real   m_ys1;
        real   m_xs2;
        real   m_ys2;
        real   m_pdx;
        real   m_pdy;
        move_e   m_move_what;
        real   m_vx[32];
        real   m_vy[32];

        ellipse  m_ellipse;

        unsigned m_idx;
        unsigned m_vertex;

    };



    //------------------------------------------------------------------------
    template<class ColorT> class scale_ctrl : public scale_ctrl_impl
    {
    public:
        scale_ctrl(real x1, real y1, real x2, real y2, bool flip_y=false) :
            scale_ctrl_impl(x1, y1, x2, y2, flip_y),
            m_background_color(rgba(1.0f, 0.9, 0.8f)),
            m_border_color(rgba(0.0f, 0.0f, 0.0f)),
            m_pointers_color(rgba(0.8, 0.0f, 0.0f, 0.8f)),
            m_slider_color(rgba(0.2, 0.1, 0.0f, 0.6))
        {
            m_colors[0] = &m_background_color;
            m_colors[1] = &m_border_color;
            m_colors[2] = &m_pointers_color;
            m_colors[3] = &m_pointers_color;
            m_colors[4] = &m_slider_color;
        }
          

        void background_color(const ColorT& c) { m_background_color = c; }
        void border_color(const ColorT& c)     { m_border_color = c; }
        void pointers_color(const ColorT& c)   { m_pointers_color = c; }
        void slider_color(const ColorT& c)     { m_slider_color = c; }

        const ColorT& color(unsigned i) const { return *m_colors[i]; } 

    private:
        scale_ctrl(const scale_ctrl<ColorT>&);
        const scale_ctrl<ColorT>& operator = (const scale_ctrl<ColorT>&);

        ColorT m_background_color;
        ColorT m_border_color;
        ColorT m_pointers_color;
        ColorT m_slider_color;
        ColorT* m_colors[5];
    };





}



#endif


