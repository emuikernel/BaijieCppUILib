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
// Rounded rectangle vertex generator
//
//----------------------------------------------------------------------------

#include <math.h>
#include "agg_rounded_rect.h"


namespace agg
{
    //------------------------------------------------------------------------
    rounded_rect::rounded_rect(real x1, real y1, real x2, real y2, real r) :
        m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2),
        m_rx1(r), m_ry1(r), m_rx2(r), m_ry2(r), 
        m_rx3(r), m_ry3(r), m_rx4(r), m_ry4(r)
    {
        if(x1 > x2) { m_x1 = x2; m_x2 = x1; }
        if(y1 > y2) { m_y1 = y2; m_y2 = y1; }
    }

    //--------------------------------------------------------------------
    void rounded_rect::rect(real x1, real y1, real x2, real y2)
    {
        m_x1 = x1;
        m_y1 = y1;
        m_x2 = x2;
        m_y2 = y2;
        if(x1 > x2) { m_x1 = x2; m_x2 = x1; }
        if(y1 > y2) { m_y1 = y2; m_y2 = y1; }
    }

    //--------------------------------------------------------------------
    void rounded_rect::radius(real r)
    {
        m_rx1 = m_ry1 = m_rx2 = m_ry2 = m_rx3 = m_ry3 = m_rx4 = m_ry4 = r; 
    }

    //--------------------------------------------------------------------
    void rounded_rect::radius(real rx, real ry)
    {
        m_rx1 = m_rx2 = m_rx3 = m_rx4 = rx; 
        m_ry1 = m_ry2 = m_ry3 = m_ry4 = ry; 
    }

    //--------------------------------------------------------------------
    void rounded_rect::radius(real rx_bottom, real ry_bottom, 
                              real rx_top,    real ry_top)
    {
        m_rx1 = m_rx2 = rx_bottom; 
        m_rx3 = m_rx4 = rx_top; 
        m_ry1 = m_ry2 = ry_bottom; 
        m_ry3 = m_ry4 = ry_top; 
    }

    inline real max( real r1, real r2 ) { return r1>r2?r1:r2; }
    //--------------------------------------------------------------------
    void rounded_rect::radius(real rx1, real ry1, real rx2, real ry2, 
                              real rx3, real ry3, real rx4, real ry4)
    {
        m_rx1 = max(0,rx1); m_ry1 = max(0,ry1); m_rx2 = max(0,rx2); m_ry2 = max(0,ry2); 
        m_rx3 = max(0,rx3); m_ry3 = max(0,ry3); m_rx4 = max(0,rx4); m_ry4 = max(0,ry4);
    }

    //--------------------------------------------------------------------
    void rounded_rect::normalize_radius()
    {
        real dx = FABS(m_x2 - m_x1);
        real dy = FABS(m_y2 - m_y1);

        real k = 1.0f;
        real t = 0;
        if(m_rx1 + m_rx2) { t = dx / (m_rx1 + m_rx2); if(t < k) k = t; }
        if(m_rx3 + m_rx4) { t = dx / (m_rx3 + m_rx4); if(t < k) k = t; }
        if(m_ry1 + m_ry4) { t = dy / (m_ry1 + m_ry4); if(t < k) k = t; }
        if(m_ry2 + m_ry3) { t = dy / (m_ry2 + m_ry3); if(t < k) k = t; }

        if(k < 1.0f)
        {
            m_rx1 *= k; m_ry1 *= k; m_rx2 *= k; m_ry2 *= k;
            m_rx3 *= k; m_ry3 *= k; m_rx4 *= k; m_ry4 *= k;
        }
    }

    //--------------------------------------------------------------------
    void rounded_rect::rewind(unsigned)
    {
        m_status = 0;
    }

    //--------------------------------------------------------------------
    unsigned rounded_rect::vertex(real* x, real* y)
    {
        unsigned cmd = path_cmd_stop;
        switch(m_status)
        {
        case 0:
            m_arc.init(m_x1 + m_rx1, m_y1 + m_ry1, m_rx1, m_ry1,
                       pi, pi+pi*0.5f);
            m_arc.rewind(0);
            m_status++;

        case 1:
            cmd = m_arc.vertex(x, y);
            if(is_stop(cmd)) m_status++;
            else return cmd;

        case 2:
            m_arc.init(m_x2 - m_rx2, m_y1 + m_ry2, m_rx2, m_ry2,
                       pi+pi*0.5f, 0.0f);
            m_arc.rewind(0);
            m_status++;

        case 3:
            cmd = m_arc.vertex(x, y);
            if(is_stop(cmd)) m_status++;
            else return path_cmd_line_to;

        case 4:
            m_arc.init(m_x2 - m_rx3, m_y2 - m_ry3, m_rx3, m_ry3,
                       0.0f, pi*0.5f);
            m_arc.rewind(0);
            m_status++;

        case 5:
            cmd = m_arc.vertex(x, y);
            if(is_stop(cmd)) m_status++;
            else return path_cmd_line_to;

        case 6:
            m_arc.init(m_x1 + m_rx4, m_y2 - m_ry4, m_rx4, m_ry4,
                       pi*0.5f, pi);
            m_arc.rewind(0);
            m_status++;

        case 7:
            cmd = m_arc.vertex(x, y);
            if(is_stop(cmd)) m_status++;
            else return path_cmd_line_to;

        case 8:
            cmd = path_cmd_end_poly | path_flags_close | path_flags_ccw;
            m_status++;
            break;
        }
        return cmd;
    }


}


