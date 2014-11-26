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

#ifndef AGG_ROUNDED_RECT_INCLUDED
#define AGG_ROUNDED_RECT_INCLUDED

#include "agg_basics.h"
#include "agg_arc.h"

namespace agg
{
    //------------------------------------------------------------rounded_rect
    //
    // See Implemantation agg_rounded_rect.cpp
    //
    class rounded_rect
    {
    public:
        rounded_rect() {}
        rounded_rect(real x1, real y1, real x2, real y2, real r);

        void rect(real x1, real y1, real x2, real y2);
        void radius(real r);
        void radius(real rx, real ry);
        void radius(real rx_bottom, real ry_bottom, real rx_top, real ry_top);
        void radius(real rx1, real ry1, real rx2, real ry2, 
                    real rx3, real ry3, real rx4, real ry4);
        void normalize_radius();

        void approximation_scale(real s) { m_arc.approximation_scale(s); }
        real approximation_scale() const { return m_arc.approximation_scale(); }

        void rewind(unsigned);
        unsigned vertex(real* x, real* y);

    private:
        real m_x1;
        real m_y1;
        real m_x2;
        real m_y2;
        real m_rx1;
        real m_ry1;
        real m_rx2;
        real m_ry2;
        real m_rx3;
        real m_ry3;
        real m_rx4;
        real m_ry4;
        unsigned m_status;
        arc      m_arc;
    };

}

#endif


