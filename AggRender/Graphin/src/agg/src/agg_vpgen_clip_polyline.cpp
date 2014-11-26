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

#include "agg_vpgen_clip_polyline.h"
#include "agg_clip_liang_barsky.h"

namespace agg
{
    //----------------------------------------------------------------------------
    void vpgen_clip_polyline::reset()
    {
        m_vertex = 0;
        m_num_vertices = 0;
        m_move_to = false;
    }

    //----------------------------------------------------------------------------
    void vpgen_clip_polyline::move_to(real x, real y)
    {
        m_vertex = 0;
        m_num_vertices = 0;
        m_x1 = x;
        m_y1 = y;
        m_move_to = true;
    }

    //----------------------------------------------------------------------------
    void vpgen_clip_polyline::line_to(real x, real y)
    {
        real x2 = x;
        real y2 = y;
        unsigned flags = clip_line_segment(&m_x1, &m_y1, &x2, &y2, m_clip_box);

        m_vertex = 0;
        m_num_vertices = 0;
        if((flags & 4) == 0)
        {
            if((flags & 1) != 0 || m_move_to)
            {
                m_x[0] = m_x1;
                m_y[0] = m_y1;
                m_cmd[0] = path_cmd_move_to;
                m_num_vertices = 1;
            }
            m_x[m_num_vertices] = x2;
            m_y[m_num_vertices] = y2;
            m_cmd[m_num_vertices++] = path_cmd_line_to;
            m_move_to = (flags & 2) != 0;
        }
        m_x1 = x;
        m_y1 = y;
    }

    //----------------------------------------------------------------------------
    unsigned vpgen_clip_polyline::vertex(real* x, real* y)
    {
        if(m_vertex < m_num_vertices)
        {
            *x = m_x[m_vertex];
            *y = m_y[m_vertex];
            return m_cmd[m_vertex++];
        }
        return path_cmd_stop;
    }
}
