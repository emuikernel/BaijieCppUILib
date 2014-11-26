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
// Arc generator. Produces at most 4 consecutive cubic bezier curves, i.e., 
// 4, 7, 10, or 13 vertices.
//
//----------------------------------------------------------------------------


#include <math.h>
#include "agg_bezier_arc.h"


namespace agg
{

    // This epsilon is used to prevent us from adding degenerate curves 
    // (converging to a single point).
    // The value isn't very critical. Function arc_to_bezier() has a limit 
    // of the sweep_angle. If FABS(sweep_angle) exceeds pi/2 the curve 
    // becomes inaccurate. But slight exceeding is quite appropriate.
    //-------------------------------------------------bezier_arc_angle_epsilon
    const real bezier_arc_angle_epsilon = 0.01;

    //------------------------------------------------------------arc_to_bezier
    void arc_to_bezier(real cx, real cy, real rx, real ry, 
                       real start_angle, real sweep_angle,
                       real* curve)
    {
        real x0 = (real)cos(sweep_angle / 2.0f);
        real y0 = (real)sin(sweep_angle / 2.0f);
        real tx = (1.0f - x0) * 4.0f / 3.0f;
        real ty = y0 - tx * x0 / y0;
        real px[4];
        real py[4];
        px[0] =  x0;
        py[0] = -y0;
        px[1] =  x0 + tx;
        py[1] = -ty;
        px[2] =  x0 + tx;
        py[2] =  ty;
        px[3] =  x0;
        py[3] =  y0;

        real sn = (real)sin(start_angle + sweep_angle / 2.0f);
        real cs = (real)cos(start_angle + sweep_angle / 2.0f);

        unsigned i;
        for(i = 0; i < 4; i++)
        {
            curve[i * 2]     = cx + rx * (px[i] * cs - py[i] * sn);
            curve[i * 2 + 1] = cy + ry * (px[i] * sn + py[i] * cs);
        }
    }



    //------------------------------------------------------------------------
    void bezier_arc::init(real x,  real y, 
                          real rx, real ry, 
                          real start_angle, 
                          real sweep_angle)
    {
        start_angle = FMOD(start_angle, 2.0f * pi);
        if(sweep_angle >=  2.0f * pi) sweep_angle =  2.0f * pi;
        if(sweep_angle <= -2.0f * pi) sweep_angle = -2.0f * pi;

        if(FABS(sweep_angle) < 1e-10)
        {
            m_num_vertices = 4;
            m_cmd = path_cmd_line_to;
            m_vertices[0] = x + rx * (real)cos(start_angle);
            m_vertices[1] = y + ry * (real)sin(start_angle);
            m_vertices[2] = x + rx * (real)cos(start_angle + sweep_angle);
            m_vertices[3] = y + ry * (real)sin(start_angle + sweep_angle);
            return;
        }

        real total_sweep = 0.0f;
        real local_sweep = 0.0f;
        real prev_sweep;
        m_num_vertices = 2;
        m_cmd = path_cmd_curve4;
        bool done = false;
        do
        {
            if(sweep_angle < 0.0f)
            {
                prev_sweep  = total_sweep;
                local_sweep = -pi * 0.5f;
                total_sweep -= pi * 0.5f;
                if(total_sweep <= sweep_angle + bezier_arc_angle_epsilon)
                {
                    local_sweep = sweep_angle - prev_sweep;
                    done = true;
                }
            }
            else
            {
                prev_sweep  = total_sweep;
                local_sweep =  pi * 0.5f;
                total_sweep += pi * 0.5f;
                if(total_sweep >= sweep_angle - bezier_arc_angle_epsilon)
                {
                    local_sweep = sweep_angle - prev_sweep;
                    done = true;
                }
            }

            arc_to_bezier(x, y, rx, ry, 
                          start_angle, 
                          local_sweep, 
                          m_vertices + m_num_vertices - 2);

            m_num_vertices += 6;
            start_angle += local_sweep;
        }
        while(!done && m_num_vertices < 26);
    }




    //--------------------------------------------------------------------
    void bezier_arc_svg::init(real x0, real y0, 
                              real rx, real ry, 
                              real angle,
                              bool large_arc_flag,
                              bool sweep_flag,
                              real x2, real y2)
    {
        m_radii_ok = true;

        if(rx < 0.0f) rx = -rx;
        if(ry < 0.0f) ry = -rx;

        // Calculate the middle point between 
        // the current and the final points
        //------------------------
        real dx2 = (x0 - x2) / 2.0f;
        real dy2 = (y0 - y2) / 2.0f;

        real cos_a = (real)cos(angle);
        real sin_a = (real)sin(angle);

        // Calculate (x1, y1)
        //------------------------
        real x1 =  cos_a * dx2 + sin_a * dy2;
        real y1 = -sin_a * dx2 + cos_a * dy2;

        // Ensure radii are large enough
        //------------------------
        real prx = rx * rx;
        real pry = ry * ry;
        real px1 = x1 * x1;
        real py1 = y1 * y1;

        // Check that radii are large enough
        //------------------------
        real radii_check = px1/prx + py1/pry;
        if(radii_check > 1.0f) 
        {
            rx = SQRT(radii_check) * rx;
            ry = SQRT(radii_check) * ry;
            prx = rx * rx;
            pry = ry * ry;
            if(radii_check > 10.0f) m_radii_ok = false;
        }

        // Calculate (cx1, cy1)
        //------------------------
        real sign = (large_arc_flag == sweep_flag) ? -1.0f : 1.0f;
        real sq   = (prx*pry - prx*py1 - pry*px1) / (prx*py1 + pry*px1);
        real coef = sign * SQRT((sq < 0) ? 0 : sq);
        real cx1  = coef *  ((rx * y1) / ry);
        real cy1  = coef * -((ry * x1) / rx);

        //
        // Calculate (cx, cy) from (cx1, cy1)
        //------------------------
        real sx2 = (x0 + x2) / 2.0f;
        real sy2 = (y0 + y2) / 2.0f;
        real cx = sx2 + (cos_a * cx1 - sin_a * cy1);
        real cy = sy2 + (sin_a * cx1 + cos_a * cy1);

        // Calculate the start_angle (angle1) and the sweep_angle (dangle)
        //------------------------
        real ux =  (x1 - cx1) / rx;
        real uy =  (y1 - cy1) / ry;
        real vx = (-x1 - cx1) / rx;
        real vy = (-y1 - cy1) / ry;
        real p, n;

        // Calculate the angle start
        //------------------------
        n = SQRT(ux*ux + uy*uy);
        p = ux; // (1 * ux) + (0 * uy)
        sign = (uy < 0) ? -1.0f : 1.0f;
        real v = p / n;
        if(v < -1.0f) v = -1.0f;
        if(v >  1.0f) v =  1.0f;
        real start_angle = sign * (real)acos(v);

        // Calculate the sweep angle
        //------------------------
        n = SQRT((ux*ux + uy*uy) * (vx*vx + vy*vy));
        p = ux * vx + uy * vy;
        sign = (ux * vy - uy * vx < 0) ? -1.0f : 1.0f;
        v = p / n;
        if(v < -1.0f) v = -1.0f;
        if(v >  1.0f) v =  1.0f;
        real sweep_angle = sign * (real)acos(v);
        if(!sweep_flag && sweep_angle > 0) 
        {
            sweep_angle -= pi * 2.0f;
        } 
        else 
        if (sweep_flag && sweep_angle < 0) 
        {
            sweep_angle += pi * 2.0f;
        }

        // We can now build and transform the resulting arc
        //------------------------
        m_arc.init(0.0f, 0.0f, rx, ry, start_angle, sweep_angle);
        trans_affine mtx = trans_affine_rotation(angle);
        mtx *= trans_affine_translation(cx, cy);
        
        for(unsigned i = 2; i < m_arc.num_vertices()-2; i += 2)
        {
            mtx.transform(m_arc.vertices() + i, m_arc.vertices() + i + 1);
        }

        // We must make sure that the starting and ending points
        // exactly coincide with the initial (x0,y0) and (x2,y2)
        m_arc.vertices()[0] = x0;
        m_arc.vertices()[1] = y0;
        if(m_arc.num_vertices() > 2)
        {
            m_arc.vertices()[m_arc.num_vertices() - 2] = x2;
            m_arc.vertices()[m_arc.num_vertices() - 1] = y2;
        }
    }


}
