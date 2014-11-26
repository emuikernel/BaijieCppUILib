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
// class gamma_spline
//
//----------------------------------------------------------------------------

#include "ctrl/agg_gamma_spline.h"

namespace agg
{

    //------------------------------------------------------------------------
    gamma_spline::gamma_spline() : 
        m_x1(0), m_y1(0), m_x2(10), m_y2(10), m_cur_x(0.0f)
    {
        values(1.0f, 1.0f, 1.0f, 1.0f);
    }


    //------------------------------------------------------------------------
    real gamma_spline::y(real x) const 
    { 
        if(x < 0.0f) x = 0.0f;
        if(x > 1.0f) x = 1.0f;
        real val = m_spline.get(x);
        if(val < 0.0f) val = 0.0f;
        if(val > 1.0f) val = 1.0f;
        return val;
    }



    //------------------------------------------------------------------------
    void gamma_spline::values(real kx1, real ky1, real kx2, real ky2)
    {
        if(kx1 < 0.001) kx1 = 0.001;
        if(kx1 > 1.999) kx1 = 1.999;
        if(ky1 < 0.001) ky1 = 0.001;
        if(ky1 > 1.999) ky1 = 1.999;
        if(kx2 < 0.001) kx2 = 0.001;
        if(kx2 > 1.999) kx2 = 1.999;
        if(ky2 < 0.001) ky2 = 0.001;
        if(ky2 > 1.999) ky2 = 1.999;

        m_x[0] = 0.0f;
        m_y[0] = 0.0f;
        m_x[1] = kx1 * 0.25;
        m_y[1] = ky1 * 0.25;
        m_x[2] = 1.0f - kx2 * 0.25;
        m_y[2] = 1.0f - ky2 * 0.25;
        m_x[3] = 1.0f;
        m_y[3] = 1.0f;

        m_spline.init(4, m_x, m_y);

        int i;
        for(i = 0; i < 256; i++)
        {
            m_gamma[i] = (unsigned char)(y(real(i) / 255.0f) * 255.0f);
        }
    }


    //------------------------------------------------------------------------
    void gamma_spline::values(real* kx1, real* ky1, real* kx2, real* ky2) const
    {
        *kx1 = m_x[1] * 4.0f;
        *ky1 = m_y[1] * 4.0f;
        *kx2 = (1.0f - m_x[2]) * 4.0f;
        *ky2 = (1.0f - m_y[2]) * 4.0f;
    }


    //------------------------------------------------------------------------
    void gamma_spline::box(real x1, real y1, real x2, real y2)
    {
        m_x1 = x1;
        m_y1 = y1;
        m_x2 = x2;
        m_y2 = y2;
    }


    //------------------------------------------------------------------------
    void gamma_spline::rewind(unsigned)
    {
        m_cur_x = 0.0f;
    }


    //------------------------------------------------------------------------
    unsigned gamma_spline::vertex(real* vx, real* vy)
    {
        if(m_cur_x == 0.0f) 
        {
            *vx = m_x1;
            *vy = m_y1;
            m_cur_x += 1.0f / (m_x2 - m_x1);
            return path_cmd_move_to;
        }

        if(m_cur_x > 1.0f) 
        {
            return path_cmd_stop;
        }
        
        *vx = m_x1 + m_cur_x * (m_x2 - m_x1);
        *vy = m_y1 + y(m_cur_x) * (m_y2 - m_y1);

        m_cur_x += 1.0f / (m_x2 - m_x1);
        return path_cmd_line_to;
    }
  


}


