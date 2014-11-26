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
// classes rbox_ctrl_impl, rbox_ctrl
//
//----------------------------------------------------------------------------

#ifndef AGG_RBOX_CTRL_INCLUDED
#define AGG_RBOX_CTRL_INCLUDED

#include "agg_array.h"
#include "agg_ellipse.h"
#include "agg_conv_stroke.h"
#include "agg_gsv_text.h"
#include "agg_trans_affine.h"
#include "agg_color_rgba.h"
#include "agg_ctrl.h"



namespace agg
{

    //------------------------------------------------------------------------
    class rbox_ctrl_impl : public ctrl
    {
    public:
        rbox_ctrl_impl(real x1, real y1, real x2, real y2, bool flip_y=false);

        void border_width(real t, real extra=0.0f);
        void text_thickness(real t)  { m_text_thickness = t; }
        void text_size(real h, real w=0.0f);

        void add_item(const char* text);
        int  cur_item() const { return m_cur_item; }
        void cur_item(int i) { m_cur_item = i; }

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
        void calc_rbox();

        real          m_border_width;
        real          m_border_extra;
        real          m_text_thickness;
        real          m_text_height;
        real          m_text_width;
        pod_array<char> m_items[32];
        unsigned        m_num_items;
        int             m_cur_item;

        real   m_xs1;
        real   m_ys1;
        real   m_xs2;
        real   m_ys2;

        real   m_vx[32];
        real   m_vy[32];
        unsigned m_draw_item;
        real   m_dy;

        ellipse               m_ellipse;
        conv_stroke<ellipse>  m_ellipse_poly;
        gsv_text              m_text;
        conv_stroke<gsv_text> m_text_poly;

        unsigned m_idx;
        unsigned m_vertex;
    };



    //------------------------------------------------------------------------
    template<class ColorT> class rbox_ctrl : public rbox_ctrl_impl
    {
    public:
        rbox_ctrl(real x1, real y1, real x2, real y2, bool flip_y=false) :
            rbox_ctrl_impl(x1, y1, x2, y2, flip_y),
            m_background_color(rgba(1.0f, 1.0f, 0.9f)),
            m_border_color(rgba(0.0f, 0.0f, 0.0f)),
            m_text_color(rgba(0.0f, 0.0f, 0.0f)),
            m_inactive_color(rgba(0.0f, 0.0f, 0.0f)),
            m_active_color(rgba(0.4, 0.0f, 0.0f))
        {
            m_colors[0] = &m_background_color;
            m_colors[1] = &m_border_color;
            m_colors[2] = &m_text_color;
            m_colors[3] = &m_inactive_color;
            m_colors[4] = &m_active_color;
        }
          

        void background_color(const ColorT& c) { m_background_color = c; }
        void border_color(const ColorT& c) { m_border_color = c; }
        void text_color(const ColorT& c) { m_text_color = c; }
        void inactive_color(const ColorT& c) { m_inactive_color = c; }
        void active_color(const ColorT& c) { m_active_color = c; }

        const ColorT& color(unsigned i) const { return *m_colors[i]; } 

    private:
        rbox_ctrl(const rbox_ctrl<ColorT>&);
        const rbox_ctrl<ColorT>& operator = (const rbox_ctrl<ColorT>&);
       
        ColorT m_background_color;
        ColorT m_border_color;
        ColorT m_text_color;
        ColorT m_inactive_color;
        ColorT m_active_color;
        ColorT* m_colors[5];
    };



}



#endif


