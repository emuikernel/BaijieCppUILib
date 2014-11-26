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

#ifndef AGG_GAMMA_SPLINE_INCLUDED
#define AGG_GAMMA_SPLINE_INCLUDED

#include "agg_basics.h"
#include "agg_bspline.h"

namespace agg
{
    
    //------------------------------------------------------------------------
    // Class-helper for calculation gamma-correction arrays. A gamma-correction
    // array is an array of 256 unsigned chars that determine the actual values 
    // of Anti-Aliasing for each pixel coverage value from 0 to 255. If all the 
    // values in the array are equal to its index, i.e. 0,1,2,3,... there's
    // no gamma-correction. Class agg::polyfill allows you to use custom 
    // gamma-correction arrays. You can calculate it using any approach, and
    // class gamma_spline allows you to calculate almost any reasonable shape 
    // of the gamma-curve with using only 4 values - kx1, ky1, kx2, ky2.
    // 
    //                                      kx2
    //        +----------------------------------+
    //        |                 |        |    .  | 
    //        |                 |        | .     | ky2
    //        |                 |       .  ------|
    //        |                 |    .           |
    //        |                 | .              |
    //        |----------------.|----------------|
    //        |             .   |                |
    //        |          .      |                |
    //        |-------.         |                |
    //    ky1 |    .   |        |                |
    //        | .      |        |                |
    //        +----------------------------------+
    //            kx1
    // 
    // Each value can be in range [0...2]. Value 1.0f means one quarter of the
    // bounding rectangle. Function values() calculates the curve by these
    // 4 values. After calling it one can get the gamma-array with call gamma(). 
    // Class also supports the vertex source interface, i.e rewind() and
    // vertex(). It's made for convinience and used in class gamma_ctrl. 
    // Before calling rewind/vertex one must set the bounding box
    // box() using pixel coordinates. 
    //------------------------------------------------------------------------

    class gamma_spline
    {
    public:
        gamma_spline();

        void values(real kx1, real ky1, real kx2, real ky2);
        const unsigned char* gamma() const { return m_gamma; }
        real y(real x) const;
        void values(real* kx1, real* ky1, real* kx2, real* ky2) const;
        void box(real x1, real y1, real x2, real y2);

        void     rewind(unsigned);
        unsigned vertex(real* x, real* y);

    private:
        unsigned char m_gamma[256];
        real        m_x[4];
        real        m_y[4];
        bspline       m_spline;
        real        m_x1;
        real        m_y1;
        real        m_x2;
        real        m_y2;
        real        m_cur_x;
    };




}

#endif
