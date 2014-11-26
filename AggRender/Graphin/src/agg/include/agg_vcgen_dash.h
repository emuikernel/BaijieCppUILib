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
// Line dash generator
//
//----------------------------------------------------------------------------
#ifndef AGG_VCGEN_DASH_INCLUDED
#define AGG_VCGEN_DASH_INCLUDED

#include "agg_basics.h"
#include "agg_vertex_sequence.h"

namespace agg
{

    //---------------------------------------------------------------vcgen_dash
    //
    // See Implementation agg_vcgen_dash.cpp
    //
    class vcgen_dash
    {
        enum max_dashes_e
        {
            max_dashes = 32
        };

        enum status_e
        {
            initial,
            ready,
            polyline,
            stop
        };

    public:
        typedef vertex_sequence<vertex_dist, 6> vertex_storage;

        vcgen_dash();

        void remove_all_dashes();
        void add_dash(real dash_len, real gap_len);
        void dash_start(real ds);

        void shorten(real s) { m_shorten = s; }
        real shorten() const { return m_shorten; }

        // Vertex Generator Interface
        void remove_all();
        void add_vertex(real x, real y, unsigned cmd);

        // Vertex Source Interface
        void     rewind(unsigned path_id);
        unsigned vertex(real* x, real* y);

    private:
        vcgen_dash(const vcgen_dash&);
        const vcgen_dash& operator = (const vcgen_dash&);

        void calc_dash_start(real ds);

        real             m_dashes[max_dashes];
        real             m_total_dash_len;
        unsigned           m_num_dashes;
        real             m_dash_start;
        real             m_shorten;
        real             m_curr_dash_start;
        unsigned           m_curr_dash;
        real             m_curr_rest;
        const vertex_dist* m_v1;
        const vertex_dist* m_v2;

        vertex_storage m_src_vertices;
        unsigned       m_closed;
        status_e       m_status;
        unsigned       m_src_vertex;
    };


}

#endif
