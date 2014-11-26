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
// classes polygon_ctrl_impl, polygon_ctrl
//
//----------------------------------------------------------------------------

#ifndef POLYGON_CTRL_INCLUDED
#define POLYGON_CTRL_INCLUDED

#include "agg_array.h"
#include "agg_conv_stroke.h"
#include "agg_ellipse.h"
#include "agg_color_rgba.h"
#include "agg_ctrl.h"

namespace agg
{
    class simple_polygon_vertex_source
    {
    public:
        simple_polygon_vertex_source(const real* polygon, unsigned np, 
                                     bool roundoff = false,
                                     bool close = true) :
            m_polygon(polygon),
            m_num_points(np),
            m_vertex(0),
            m_roundoff(roundoff),
            m_close(close)
        {
        }

        void close(bool f) { m_close = f;    }
        bool close() const { return m_close; }

        void rewind(unsigned)
        {
            m_vertex = 0;
        }

        unsigned vertex(real* x, real* y)
        {
            if(m_vertex > m_num_points) return path_cmd_stop;
            if(m_vertex == m_num_points) 
            {
                ++m_vertex;
                return path_cmd_end_poly | (m_close ? path_flags_close : 0);
            }
            *x = m_polygon[m_vertex * 2];
            *y = m_polygon[m_vertex * 2 + 1];
            if(m_roundoff)
            {
                *x = floorf(*x) + 0.5f;
                *y = floorf(*y) + 0.5f;
            }
            ++m_vertex;
            return (m_vertex == 1) ? path_cmd_move_to : path_cmd_line_to;
        }

    private:
        const real* m_polygon;
        unsigned m_num_points;
        unsigned m_vertex;
        bool     m_roundoff;
        bool     m_close;
    };




    class polygon_ctrl_impl : public ctrl
    {
    public:
        polygon_ctrl_impl(unsigned np, real point_radius=5);

        unsigned num_points() const { return m_num_points; }
        real xn(unsigned n) const { return m_polygon[n * 2];     }
        real yn(unsigned n) const { return m_polygon[n * 2 + 1]; }
        double& xn(unsigned n) { return m_polygon[n * 2];     }
        double& yn(unsigned n) { return m_polygon[n * 2 + 1]; }
    
        const real* polygon() const { return &m_polygon[0]; }

        void   line_width(real w) { m_stroke.width(w); }
        real line_width() const   { return m_stroke.width(); }

        void   point_radius(real r) { m_point_radius = r; }
        real point_radius() const   { return m_point_radius; }

        void in_polygon_check(bool f) { m_in_polygon_check = f; }
        bool in_polygon_check() const { return m_in_polygon_check; }

        void close(bool f) { m_vs.close(f);       }
        bool close() const { return m_vs.close(); }

        // Vertex source interface
        unsigned num_paths() { return 1; }
        void     rewind(unsigned path_id);
        unsigned vertex(real* x, real* y);

        virtual bool in_rect(real x, real y) const;
        virtual bool on_mouse_button_down(real x, real y);
        virtual bool on_mouse_button_up(real x, real y);
        virtual bool on_mouse_move(real x, real y, bool button_flag);
        virtual bool on_arrow_keys(bool left, bool right, bool down, bool up);


    private:
        bool check_edge(unsigned i, real x, real y) const;
        bool point_in_polygon(real x, real y) const;

        pod_array<double> m_polygon;
        unsigned          m_num_points;
        int               m_node;
        int               m_edge;
        simple_polygon_vertex_source              m_vs;
        conv_stroke<simple_polygon_vertex_source> m_stroke;
        ellipse  m_ellipse;
        real   m_point_radius;
        unsigned m_status;
        real   m_dx;
        real   m_dy;
        bool     m_in_polygon_check;
    };



    //----------------------------------------------------------polygon_ctrl
    template<class ColorT> class polygon_ctrl : public polygon_ctrl_impl
    {
    public:
        polygon_ctrl(unsigned np, real point_radius=5) :
            polygon_ctrl_impl(np, point_radius),
            m_color(rgba(0.0f, 0.0f, 0.0f))
        {
        }
          
        void line_color(const ColorT& c) { m_color = c; }
        const ColorT& color(unsigned i) const { return m_color; } 

    private:
        polygon_ctrl(const polygon_ctrl<ColorT>&);
        const polygon_ctrl<ColorT>& operator = (const polygon_ctrl<ColorT>&);

        ColorT m_color;
    };




}

#endif


