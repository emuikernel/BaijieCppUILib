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

#ifndef AGG_WARP_MAGNIFIER_INCLUDED
#define AGG_WARP_MAGNIFIER_INCLUDED


namespace agg
{

    //----------------------------------------------------trans_warp_magnifier
    //
    // See Inmplementation agg_trans_warp_magnifier.cpp
    //
    class trans_warp_magnifier
    {
    public:
        trans_warp_magnifier() : m_xc(0.0f), m_yc(0.0f), m_magn(1.0f), m_radius(1.0f) {}
 
        void center(real x, real y) { m_xc = x; m_yc = y; }
        void magnification(real m)    { m_magn = m;         }
        void radius(real r)           { m_radius = r;       }

        real xc()            const { return m_xc; }
        real yc()            const { return m_yc; }
        real magnification() const { return m_magn;   }
        real radius()        const { return m_radius; }

        void transform(real* x, real* y) const;
        void inverse_transform(real* x, real* y) const;

    private:
        real m_xc;
        real m_yc;
        real m_magn;
        real m_radius;
    };


}


#endif


