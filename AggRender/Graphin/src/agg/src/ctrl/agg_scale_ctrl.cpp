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

#include "ctrl/agg_scale_ctrl.h"

namespace agg
{

    //------------------------------------------------------------------------
    scale_ctrl_impl::scale_ctrl_impl(real x1, real y1, 
                                     real x2, real y2, bool flip_y) :
        ctrl(x1, y1, x2, y2, flip_y),
        m_border_thickness(1.0f),
        m_border_extra((FABS(x2 - x1) > FABS(y2 - y1)) ? (y2 - y1) / 2 : (x2 - x1) / 2),
        m_pdx(0.0f),
        m_pdy(0.0f),
        m_move_what(move_nothing),
        m_value1(0.3),
        m_value2(0.7),
        m_min_d(0.01)
    {
        calc_box();
    }


    //------------------------------------------------------------------------
    void scale_ctrl_impl::calc_box()
    {
        m_xs1 = m_x1 + m_border_thickness;
        m_ys1 = m_y1 + m_border_thickness;
        m_xs2 = m_x2 - m_border_thickness;
        m_ys2 = m_y2 - m_border_thickness;
    }


    //------------------------------------------------------------------------
    void scale_ctrl_impl::border_thickness(real t, real extra)
    { 
        m_border_thickness = t; 
        m_border_extra = extra;
        calc_box(); 
    }


    //------------------------------------------------------------------------
    void scale_ctrl_impl::resize(real x1, real y1, real x2, real y2)
    {
        m_x1 = x1;
        m_y1 = y1;
        m_x2 = x2;
        m_y2 = y2;
        calc_box(); 
        m_border_extra = (FABS(x2 - x1) > FABS(y2 - y1)) ? 
                            (y2 - y1) / 2 : 
                            (x2 - x1) / 2;
    }


    //------------------------------------------------------------------------
    void scale_ctrl_impl::value1(real value) 
    { 
        if(value < 0.0f) value = 0.0f;
        if(value > 1.0f) value = 1.0f;
        if(m_value2 - value < m_min_d) value = m_value2 - m_min_d;
        m_value1 = value; 
    }


    //------------------------------------------------------------------------
    void scale_ctrl_impl::value2(real value) 
    { 
        if(value < 0.0f) value = 0.0f;
        if(value > 1.0f) value = 1.0f;
        if(m_value1 + value < m_min_d) value = m_value1 + m_min_d;
        m_value2 = value; 
    }


    //------------------------------------------------------------------------
    void scale_ctrl_impl::move(real d)
    {
        m_value1 += d;
        m_value2 += d;
        if(m_value1 < 0.0f)
        {
            m_value2 -= m_value1;
            m_value1 = 0.0f;
        }
        if(m_value2 > 1.0f)
        {
            m_value1 -= m_value2 - 1.0f;
            m_value2 = 1.0f;
        }
    }


    //------------------------------------------------------------------------
    void scale_ctrl_impl::rewind(unsigned idx)
    {
        m_idx = idx;

        switch(idx)
        {
        default:

        case 0:                 // Background
            m_vertex = 0;
            m_vx[0] = m_x1 - m_border_extra; 
            m_vy[0] = m_y1 - m_border_extra;
            m_vx[1] = m_x2 + m_border_extra; 
            m_vy[1] = m_y1 - m_border_extra;
            m_vx[2] = m_x2 + m_border_extra; 
            m_vy[2] = m_y2 + m_border_extra;
            m_vx[3] = m_x1 - m_border_extra; 
            m_vy[3] = m_y2 + m_border_extra;
            break;

        case 1:                 // Border
            m_vertex = 0;
            m_vx[0] = m_x1; 
            m_vy[0] = m_y1;
            m_vx[1] = m_x2; 
            m_vy[1] = m_y1;
            m_vx[2] = m_x2; 
            m_vy[2] = m_y2;
            m_vx[3] = m_x1; 
            m_vy[3] = m_y2;
            m_vx[4] = m_x1 + m_border_thickness; 
            m_vy[4] = m_y1 + m_border_thickness; 
            m_vx[5] = m_x1 + m_border_thickness; 
            m_vy[5] = m_y2 - m_border_thickness; 
            m_vx[6] = m_x2 - m_border_thickness; 
            m_vy[6] = m_y2 - m_border_thickness; 
            m_vx[7] = m_x2 - m_border_thickness; 
            m_vy[7] = m_y1 + m_border_thickness; 
            break;

        case 2:                 // pointer1
            if(FABS(m_x2 - m_x1) > FABS(m_y2 - m_y1))
            {
                m_ellipse.init(m_xs1 + (m_xs2 - m_xs1) * m_value1,
                               (m_ys1 + m_ys2) / 2.0,
                               m_y2 - m_y1,
                               m_y2 - m_y1, 
                               32);
            }
            else
            {
                m_ellipse.init((m_xs1 + m_xs2) / 2.0,
                               m_ys1 + (m_ys2 - m_ys1) * m_value1,
                               m_x2 - m_x1,
                               m_x2 - m_x1, 
                               32);
            }
            m_ellipse.rewind(0);
            break;

        case 3:                 // pointer2
            if(FABS(m_x2 - m_x1) > FABS(m_y2 - m_y1))
            {
                m_ellipse.init(m_xs1 + (m_xs2 - m_xs1) * m_value2,
                               (m_ys1 + m_ys2) / 2.0,
                               m_y2 - m_y1,
                               m_y2 - m_y1, 
                               32);
            }
            else
            {
                m_ellipse.init((m_xs1 + m_xs2) / 2.0,
                               m_ys1 + (m_ys2 - m_ys1) * m_value2,
                               m_x2 - m_x1,
                               m_x2 - m_x1, 
                               32);
            }
            m_ellipse.rewind(0);
            break;

        case 4:                 // slider
            m_vertex = 0;
            if(FABS(m_x2 - m_x1) > FABS(m_y2 - m_y1))
            {
                m_vx[0] = m_xs1 + (m_xs2 - m_xs1) * m_value1;
                m_vy[0] = m_y1 - m_border_extra / 2.0f;
                m_vx[1] = m_xs1 + (m_xs2 - m_xs1) * m_value2; 
                m_vy[1] = m_vy[0];
                m_vx[2] = m_vx[1]; 
                m_vy[2] = m_y2 + m_border_extra / 2.0f;
                m_vx[3] = m_vx[0]; 
                m_vy[3] = m_vy[2];
            }
            else
            {
                m_vx[0] = m_x1 - m_border_extra / 2.0f;
                m_vy[0] = m_ys1 + (m_ys2 - m_ys1) * m_value1;
                m_vx[1] = m_vx[0];
                m_vy[1] = m_ys1 + (m_ys2 - m_ys1) * m_value2; 
                m_vx[2] = m_x2 + m_border_extra / 2.0f;
                m_vy[2] = m_vy[1]; 
                m_vx[3] = m_vx[2];
                m_vy[3] = m_vy[0]; 
            }
            break;
        }
    }


    //------------------------------------------------------------------------
    unsigned scale_ctrl_impl::vertex(real* x, real* y)
    {
        unsigned cmd = path_cmd_line_to;
        switch(m_idx)
        {
        case 0:
        case 4:
            if(m_vertex == 0) cmd = path_cmd_move_to;
            if(m_vertex >= 4) cmd = path_cmd_stop;
            *x = m_vx[m_vertex];
            *y = m_vy[m_vertex];
            m_vertex++;
            break;

        case 1:
            if(m_vertex == 0 || m_vertex == 4) cmd = path_cmd_move_to;
            if(m_vertex >= 8) cmd = path_cmd_stop;
            *x = m_vx[m_vertex];
            *y = m_vy[m_vertex];
            m_vertex++;
            break;

        case 2:
        case 3:
            cmd = m_ellipse.vertex(x, y);
            break;

        default:
            cmd = path_cmd_stop;
            break;
        }

        if(!is_stop(cmd))
        {
            transform_xy(x, y);
        }

        return cmd;
    }



    //------------------------------------------------------------------------
    bool scale_ctrl_impl::in_rect(real x, real y) const
    {
        inverse_transform_xy(&x, &y);
        return x >= m_x1 && x <= m_x2 && y >= m_y1 && y <= m_y2;
    }


    //------------------------------------------------------------------------
    bool scale_ctrl_impl::on_mouse_button_down(real x, real y)
    {
        inverse_transform_xy(&x, &y);

        real xp1;
        real xp2;
        real ys1;
        real ys2;
        real xp;
        real yp;

        if(FABS(m_x2 - m_x1) > FABS(m_y2 - m_y1))
        {
            xp1 = m_xs1 + (m_xs2 - m_xs1) * m_value1;
            xp2 = m_xs1 + (m_xs2 - m_xs1) * m_value2;
            ys1 = m_y1  - m_border_extra / 2.0f;
            ys2 = m_y2  + m_border_extra / 2.0f;
            yp = (m_ys1 + m_ys2) / 2.0f;

            if(x > xp1 && y > ys1 && x < xp2 && y < ys2)
            {
                m_pdx = xp1 - x;
                m_move_what = move_slider;
                return true;
            }

            //if(x < xp1 && calc_distance(x, y, xp1, yp) <= m_y2 - m_y1)
            if(calc_distance(x, y, xp1, yp) <= m_y2 - m_y1)
            {
                m_pdx = xp1 - x;
                m_move_what = move_value1;
                return true;
            }

            //if(x > xp2 && calc_distance(x, y, xp2, yp) <= m_y2 - m_y1)
            if(calc_distance(x, y, xp2, yp) <= m_y2 - m_y1)
            {
                m_pdx = xp2 - x;
                m_move_what = move_value2;
                return true;
            }
        }
        else
        {
            xp1 = m_x1  - m_border_extra / 2.0f;
            xp2 = m_x2  + m_border_extra / 2.0f;
            ys1 = m_ys1 + (m_ys2 - m_ys1) * m_value1;
            ys2 = m_ys1 + (m_ys2 - m_ys1) * m_value2;
            xp = (m_xs1 + m_xs2) / 2.0f;

            if(x > xp1 && y > ys1 && x < xp2 && y < ys2)
            {
                m_pdy = ys1 - y;
                m_move_what = move_slider;
                return true;
            }

            //if(y < ys1 && calc_distance(x, y, xp, ys1) <= m_x2 - m_x1)
            if(calc_distance(x, y, xp, ys1) <= m_x2 - m_x1)
            {
                m_pdy = ys1 - y;
                m_move_what = move_value1;
                return true;
            }

            //if(y > ys2 && calc_distance(x, y, xp, ys2) <= m_x2 - m_x1)
            if(calc_distance(x, y, xp, ys2) <= m_x2 - m_x1)
            {
                m_pdy = ys2 - y;
                m_move_what = move_value2;
                return true;
            }
        }

        return false;
    }


    //------------------------------------------------------------------------
    bool scale_ctrl_impl::on_mouse_move(real x, real y, bool button_flag)
    {
        inverse_transform_xy(&x, &y);
        if(!button_flag)
        {
            return on_mouse_button_up(x, y);
        }

        real xp = x + m_pdx;
        real yp = y + m_pdy;
        real dv;

        switch(m_move_what)
        {
        case move_value1:
            if(FABS(m_x2 - m_x1) > FABS(m_y2 - m_y1))
            {
                m_value1 = (xp - m_xs1) / (m_xs2 - m_xs1);
            }
            else
            {
                m_value1 = (yp - m_ys1) / (m_ys2 - m_ys1);
            }
            if(m_value1 < 0.0f) m_value1 = 0.0f;
            if(m_value1 > m_value2 - m_min_d) m_value1 = m_value2 - m_min_d;
            return true;

        case move_value2:
            if(FABS(m_x2 - m_x1) > FABS(m_y2 - m_y1))
            {
                m_value2 = (xp - m_xs1) / (m_xs2 - m_xs1);
            }
            else
            {
                m_value2 = (yp - m_ys1) / (m_ys2 - m_ys1);
            }
            if(m_value2 > 1.0f) m_value2 = 1.0f;
            if(m_value2 < m_value1 + m_min_d) m_value2 = m_value1 + m_min_d;
            return true;

        case move_slider:
            dv = m_value2 - m_value1;
            if(FABS(m_x2 - m_x1) > FABS(m_y2 - m_y1))
            {
                m_value1 = (xp - m_xs1) / (m_xs2 - m_xs1);
            }
            else
            {
                m_value1 = (yp - m_ys1) / (m_ys2 - m_ys1);
            }
            m_value2 = m_value1 + dv;
            if(m_value1 < 0.0f)
            {
                dv = m_value2 - m_value1;
                m_value1 = 0.0f;
                m_value2 = m_value1 + dv;
            }
            if(m_value2 > 1.0f)
            {
                dv = m_value2 - m_value1;
                m_value2 = 1.0f;
                m_value1 = m_value2 - dv;
            }
            return true;
        }

        return false;
    }


    //------------------------------------------------------------------------
    bool scale_ctrl_impl::on_mouse_button_up(double, double)
    {
        m_move_what = move_nothing;
        return false;
    }


    //------------------------------------------------------------------------
    bool scale_ctrl_impl::on_arrow_keys(bool left, bool right, bool down, bool up)
    {
/*
        if(right || up)
        {
            m_value += 0.005;
            if(m_value > 1.0f) m_value = 1.0f;
            return true;
        }

        if(left || down)
        {
            m_value -= 0.005;
            if(m_value < 0.0f) m_value = 0.0f;
            return true;
        }
*/
        return false;
    }

}


