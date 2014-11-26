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

#ifndef AGG_ARC_INCLUDED
#define AGG_ARC_INCLUDED

#include <math.h>
#include "agg_basics.h"

namespace agg
{

    //=====================================================================arc
    //
    // See Implementation agg_arc.cpp 
    //
    class arc
    {
    public:
        arc() : m_scale(1.0f), m_initialized(false) {}
        arc(real x,  real y, 
            real rx, real ry, 
            real a1, real a2, 
            bool ccw=true);

        void init(real x,  real y, 
                  real rx, real ry, 
                  real a1, real a2, 
                  bool ccw=true);

        void approximation_scale(real s);
        real approximation_scale() const { return m_scale;  }

        void rewind(unsigned);
        unsigned vertex(real* x, real* y);

    private:
        void normalize(real a1, real a2, bool ccw);

        real   m_x;
        real   m_y;
        real   m_rx;
        real   m_ry;
        real   m_angle;
        real   m_start;
        real   m_end;
        real   m_scale;
        real   m_da;
        bool     m_ccw;
        bool     m_initialized;
        unsigned m_path_cmd;
    };


}


#endif
