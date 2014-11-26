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
// Arc vertex generator
//
//----------------------------------------------------------------------------

#include <math.h>
#include "agg_arc.h"


namespace agg
{
    //------------------------------------------------------------------------
    arc::arc(real x,  real y, 
             real rx, real ry, 
             real a1, real a2, 
             bool ccw) :
        m_x(x), m_y(y), m_rx(rx), m_ry(ry), m_scale(1.0f)
    {
        normalize(a1, a2, ccw);
    }

    //------------------------------------------------------------------------
    void arc::init(real x,  real y, 
                   real rx, real ry, 
                   real a1, real a2, 
                   bool ccw)
    {
        m_x   = x;  m_y  = y;
        m_rx  = rx; m_ry = ry; 
        normalize(a1, a2, ccw);
    }
    
    //------------------------------------------------------------------------
    void arc::approximation_scale(real s)
    {
        m_scale = s;
        if(m_initialized)
        {
            normalize(m_start, m_end, m_ccw);
        }
    }

    //------------------------------------------------------------------------
    void arc::rewind(unsigned)
    {
        m_path_cmd = path_cmd_move_to; 
        m_angle = m_start;
    }

    //------------------------------------------------------------------------
    unsigned arc::vertex(real* x, real* y)
    {
        if(is_stop(m_path_cmd)) return path_cmd_stop;
        if((m_angle < m_end - m_da/4) != m_ccw)
        {
            *x = m_x + (real)cos(m_end) * m_rx;
            *y = m_y + (real)sin(m_end) * m_ry;
            m_path_cmd = path_cmd_stop;
            return path_cmd_line_to;
        }

        *x = m_x + (real)cos(m_angle) * m_rx;
        *y = m_y + (real)sin(m_angle) * m_ry;

        m_angle += m_da;

        unsigned pf = m_path_cmd;
        m_path_cmd = path_cmd_line_to;
        return pf;
    }

    //------------------------------------------------------------------------
    void arc::normalize(real a1, real a2, bool ccw)
    {
        real ra = (FABS(m_rx) + FABS(m_ry)) / 2;
        m_da = (real)acos(ra / (ra + 0.125 / m_scale)) * 2;
        if(ccw)
        {
            while(a2 < a1) a2 += pi * 2.0f;
        }
        else
        {
            while(a1 < a2) a1 += pi * 2.0f;
            m_da = -m_da;
        }
        m_ccw   = ccw;
        m_start = a1;
        m_end   = a2;
        m_initialized = true;
    }

}
