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
// classes slider_ctrl_impl, slider_ctrl
//
//----------------------------------------------------------------------------

#ifndef AGG_SLIDER_CTRL_INCLUDED
#define AGG_SLIDER_CTRL_INCLUDED

#include "agg_basics.h"
#include "agg_math.h"
#include "agg_ellipse.h"
#include "agg_trans_affine.h"
#include "agg_color_rgba.h"
#include "agg_gsv_text.h"
#include "agg_conv_stroke.h"
#include "agg_path_storage.h"
#include "agg_ctrl.h"


namespace agg
{

    //--------------------------------------------------------slider_ctrl_impl
    class slider_ctrl_impl : public ctrl
    {
    public:
        slider_ctrl_impl(real x1, real y1, real x2, real y2, bool flip_y=false);

        void border_width(real t, real extra=0.0f);

        void range(real min, real max) { m_min = min; m_max = max; }
        void num_steps(unsigned num) { m_num_steps = num; }
        void label(const char* fmt);
        void text_thickness(real t) { m_text_thickness = t; }

        bool descending() const { return m_descending; }
        void descending(bool v) { m_descending = v; }

        real value() const { return m_value * (m_max - m_min) + m_min; }
        void value(real value);

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
        void calc_box();
        bool normalize_value(bool preview_value_flag);

        real   m_border_width;
        real   m_border_extra;
        real   m_text_thickness;
        real   m_value;
        real   m_preview_value;
        real   m_min;
        real   m_max;
        unsigned m_num_steps;
        bool     m_descending;
        char     m_label[64];
        real   m_xs1;
        real   m_ys1;
        real   m_xs2;
        real   m_ys2;
        real   m_pdx;
        bool     m_mouse_move;
        real   m_vx[32];
        real   m_vy[32];

        ellipse  m_ellipse;

        unsigned m_idx;
        unsigned m_vertex;

        gsv_text              m_text;
        conv_stroke<gsv_text> m_text_poly;
        path_storage          m_storage;

    };



    //----------------------------------------------------------slider_ctrl
    template<class ColorT> class slider_ctrl : public slider_ctrl_impl
    {
    public:
        slider_ctrl(real x1, real y1, real x2, real y2, bool flip_y=false) :
            slider_ctrl_impl(x1, y1, x2, y2, flip_y),
            m_background_color(rgba(1.0f, 0.9, 0.8f)),
            m_triangle_color(rgba(0.7, 0.6, 0.6)),
            m_text_color(rgba(0.0f, 0.0f, 0.0f)),
            m_pointer_preview_color(rgba(0.6, 0.4, 0.4, 0.4)),
            m_pointer_color(rgba(0.8, 0.0f, 0.0f, 0.6))
        {
            m_colors[0] = &m_background_color;
            m_colors[1] = &m_triangle_color;
            m_colors[2] = &m_text_color;
            m_colors[3] = &m_pointer_preview_color;
            m_colors[4] = &m_pointer_color;
            m_colors[5] = &m_text_color;
        }
          

        void background_color(const ColorT& c) { m_background_color = c; }
        void pointer_color(const ColorT& c) { m_pointer_color = c; }

        const ColorT& color(unsigned i) const { return *m_colors[i]; } 

    private:
        slider_ctrl(const slider_ctrl<ColorT>&);
        const slider_ctrl<ColorT>& operator = (const slider_ctrl<ColorT>&);

        ColorT m_background_color;
        ColorT m_triangle_color;
        ColorT m_text_color;
        ColorT m_pointer_preview_color;
        ColorT m_pointer_color;
        ColorT* m_colors[6];
    };





}



#endif


