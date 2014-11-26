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
// Affine transformations
//
//----------------------------------------------------------------------------
#include "agg_trans_affine.h"



namespace agg
{

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::parl_to_parl(const real* src, 
                                                   const real* dst)
    {
        sx  = src[2] - src[0];
        shy = src[3] - src[1];
        shx = src[4] - src[0];
        sy  = src[5] - src[1];
        tx  = src[0];
        ty  = src[1];
        invert();
        multiply(trans_affine(dst[2] - dst[0], dst[3] - dst[1], 
                              dst[4] - dst[0], dst[5] - dst[1],
                              dst[0], dst[1]));
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::rect_to_parl(real x1, real y1, 
                                                   real x2, real y2, 
                                                   const real* parl)
    {
        real src[6];
        src[0] = x1; src[1] = y1;
        src[2] = x2; src[3] = y1;
        src[4] = x2; src[5] = y2;
        parl_to_parl(src, parl);
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::parl_to_rect(const real* parl, 
                                                   real x1, real y1, 
                                                   real x2, real y2)
    {
        real dst[6];
        dst[0] = x1; dst[1] = y1;
        dst[2] = x2; dst[3] = y1;
        dst[4] = x2; dst[5] = y2;
        parl_to_parl(parl, dst);
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::multiply(const trans_affine& m)
    {
        real t0 = sx  * m.sx + shy * m.shx;
        real t2 = shx * m.sx + sy  * m.shx;
        real t4 = tx  * m.sx + ty  * m.shx + m.tx;
        shy = sx  * m.shy + shy * m.sy;
        sy  = shx * m.shy + sy  * m.sy;
        ty  = tx  * m.shy + ty  * m.sy + m.ty;
        sx  = t0;
        shx = t2;
        tx  = t4;
        return *this;
    }


    //------------------------------------------------------------------------
    const trans_affine& trans_affine::invert()
    {
        real d  = determinant_reciprocal();

        real t0  =  sy  * d;
               sy  =  sx  * d;
               shy = -shy * d;
               shx = -shx * d;

        real t4 = -tx * t0  - ty * shx;
               ty = -tx * shy - ty * sy;

        sx = t0;
        tx = t4;
        return *this;
    }


   //------------------------------------------------------------------------
    const trans_affine& trans_affine::flip_x()
    {
        sx  = -sx;
        shy = -shy;
        tx  = -tx;
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::flip_y()
    {
        shx = -shx;
        sy  = -sy;
        ty  = -ty;
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::reset()
    {
        sx  = sy  = 1.0f; 
        shy = shx = tx = ty = 0.0f;
        return *this;
    }

    //------------------------------------------------------------------------
    bool trans_affine::is_identity(real epsilon) const
    {
        return is_equal_eps(sx,  real(1.0), epsilon) &&
               is_equal_eps(shy, real(0.0), epsilon) &&
               is_equal_eps(shx, real(0.0), epsilon) && 
               is_equal_eps(sy,  real(1.0), epsilon) &&
               is_equal_eps(tx,  real(0.0), epsilon) &&
               is_equal_eps(ty,  real(0.0), epsilon);
    }

    //------------------------------------------------------------------------
    bool trans_affine::is_valid(real epsilon) const
    {
        return FABS(sx) > epsilon && FABS(sy) > epsilon;
    }

    //------------------------------------------------------------------------
    bool trans_affine::is_equal(const trans_affine& m, real epsilon) const
    {
        return is_equal_eps(sx,  m.sx,  epsilon) &&
               is_equal_eps(shy, m.shy, epsilon) &&
               is_equal_eps(shx, m.shx, epsilon) && 
               is_equal_eps(sy,  m.sy,  epsilon) &&
               is_equal_eps(tx,  m.tx,  epsilon) &&
               is_equal_eps(ty,  m.ty,  epsilon);
    }

    //------------------------------------------------------------------------
    real trans_affine::rotation() const
    {
        real x1 = 0.0f;
        real y1 = 0.0f;
        real x2 = 1.0f;
        real y2 = 0.0f;
        transform(&x1, &y1);
        transform(&x2, &y2);
        return (real)atan2(y2-y1, x2-x1);
    }

    //------------------------------------------------------------------------
    void trans_affine::translation(real* dx, real* dy) const
    {
        *dx = tx;
        *dy = ty;
    }

    //------------------------------------------------------------------------
    void trans_affine::scaling(real* x, real* y) const
    {
        real x1 = 0.0f;
        real y1 = 0.0f;
        real x2 = 1.0f;
        real y2 = 1.0f;
        trans_affine t(*this);
        t *= trans_affine_rotation(-rotation());
        t.transform(&x1, &y1);
        t.transform(&x2, &y2);
        *x = x2 - x1;
        *y = y2 - y1;
    }


}


