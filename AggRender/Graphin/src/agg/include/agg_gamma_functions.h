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

#ifndef AGG_GAMMA_FUNCTIONS_INCLUDED
#define AGG_GAMMA_FUNCTIONS_INCLUDED

#include <math.h>
#include "agg_basics.h"

namespace agg
{
    //===============================================================gamma_none
    struct gamma_none
    {
        real operator()(real x) const { return x; }
    };


    //==============================================================gamma_power
    class gamma_power
    {
    public:
        gamma_power() : m_gamma(1.0f) {}
        gamma_power(real g) : m_gamma(g) {}

        void gamma(real g) { m_gamma = g; }
        real gamma() const { return m_gamma; }

        real operator() (real x) const
        {
            return (real)pow(x, m_gamma);
        }

    private:
        real m_gamma;
    };


    //==========================================================gamma_threshold
    class gamma_threshold
    {
    public:
        gamma_threshold() : m_threshold(0.5f) {}
        gamma_threshold(real t) : m_threshold(t) {}

        void threshold(real t) { m_threshold = t; }
        real threshold() const { return m_threshold; }

        real operator() (real x) const
        {
            return (x < m_threshold) ? 0.0f : 1.0f;
        }

    private:
        real m_threshold;
    };


    //============================================================gamma_linear
    class gamma_linear
    {
    public:
        gamma_linear() : m_start(0.0f), m_end(1.0f) {}
        gamma_linear(real s, real e) : m_start(s), m_end(e) {}

        void set(real s, real e) { m_start = s; m_end = e; }
        void start(real s) { m_start = s; }
        void end(real e) { m_end = e; }
        real start() const { return m_start; }
        real end() const { return m_end; }

        real operator() (real x) const
        {
            if(x < m_start) return 0.0f;
            if(x > m_end) return 1.0f;
            return (x - m_start) / (m_end - m_start);
        }

    private:
        real m_start;
        real m_end;
    };


    //==========================================================gamma_multiply
    class gamma_multiply
    {
    public:
        gamma_multiply() : m_mul(1.0f) {}
        gamma_multiply(real v) : m_mul(v) {}

        void value(real v) { m_mul = v; }
        real value() const { return m_mul; }

        real operator() (real x) const
        {
            real y = x * m_mul;
            if(y > 1.0f) y = 1.0f;
            return y;
        }

    private:
        real m_mul;
    };

}

#endif




