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

#include <math.h>
#include "agg_trans_warp_magnifier.h"

namespace agg
{

    //------------------------------------------------------------------------
    void trans_warp_magnifier::transform(real* x, real* y) const
    {
        real dx = *x - m_xc;
        real dy = *y - m_yc;
        real r = SQRT(dx * dx + dy * dy);
        if(r < m_radius)
        {
            *x = m_xc + dx * m_magn;
            *y = m_yc + dy * m_magn;
            return;
        }

        real m = (r + m_radius * (m_magn - 1.0f)) / r;
        *x = m_xc + dx * m;
        *y = m_yc + dy * m;
    }

    //------------------------------------------------------------------------
    void trans_warp_magnifier::inverse_transform(real* x, real* y) const
    {
        // New version by Andrew Skalkin
        //-----------------
        real dx = *x - m_xc;
        real dy = *y - m_yc;
        real r = SQRT(dx * dx + dy * dy);

        if(r < m_radius * m_magn) 
        {
            *x = m_xc + dx / m_magn;
            *y = m_yc + dy / m_magn;
        }
        else
        {
            real rnew = r - m_radius * (m_magn - 1.0f);
            *x = m_xc + rnew * dx / r; 
            *y = m_yc + rnew * dy / r;
        }

        // Old version
        //-----------------
        //trans_warp_magnifier t(*this);
        //t.magnification(1.0f / m_magn);
        //t.radius(m_radius * m_magn);
        //t.transform(x, y);
    }


}
