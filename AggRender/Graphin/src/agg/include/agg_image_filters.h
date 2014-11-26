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
// Image transformation filters,
// Filtering classes (image_filter_lut, image_filter),
// Basic filter shape classes
//----------------------------------------------------------------------------
#ifndef AGG_IMAGE_FILTERS_INCLUDED
#define AGG_IMAGE_FILTERS_INCLUDED

#include "agg_array.h"
#include "agg_math.h"

namespace agg
{

    // See Implementation agg_image_filters.cpp 

    enum image_filter_scale_e
    {
        image_filter_shift = 14,                      //----image_filter_shift
        image_filter_scale = 1 << image_filter_shift, //----image_filter_scale 
        image_filter_mask  = image_filter_scale - 1   //----image_filter_mask 
    };

    enum image_subpixel_scale_e
    {
        image_subpixel_shift = 8,                         //----image_subpixel_shift
        image_subpixel_scale = 1 << image_subpixel_shift, //----image_subpixel_scale 
        image_subpixel_mask  = image_subpixel_scale - 1   //----image_subpixel_mask 
    };


    //-----------------------------------------------------image_filter_lut
    class image_filter_lut
    {
    public:
        template<class FilterF> void calculate(const FilterF& filter,
                                               bool normalization=true)
        {
            real r = filter.radius();
            realloc_lut(r);
            unsigned i;
            unsigned pivot = diameter() << (image_subpixel_shift - 1);
            for(i = 0; i < pivot; i++)
            {
                real x = real(i) / real(image_subpixel_scale);
                real y = filter.calc_weight(x);
                m_weight_array[pivot + i] = 
                m_weight_array[pivot - i] = (int16)iround(y * image_filter_scale);
            }
            unsigned end = (diameter() << image_subpixel_shift) - 1;
            m_weight_array[0] = m_weight_array[end];
            if(normalization) 
            {
                normalize();
            }
        }

        image_filter_lut() : m_radius(0), m_diameter(0), m_start(0) {}

        template<class FilterF> image_filter_lut(const FilterF& filter, 
                                                 bool normalization=true)
        {
            calculate(filter, normalization);
        }

        real       radius()       const { return m_radius;   }
        unsigned     diameter()     const { return m_diameter; }
        int          start()        const { return m_start;    }
        const int16* weight_array() const { return &m_weight_array[0]; }
        void         normalize();

    private:
        void realloc_lut(real radius);
        image_filter_lut(const image_filter_lut&);
        const image_filter_lut& operator = (const image_filter_lut&);

        real           m_radius;
        unsigned         m_diameter;
        int              m_start;
        pod_array<int16> m_weight_array;
    };



    //--------------------------------------------------------image_filter
    template<class FilterF> class image_filter : public image_filter_lut
    {
    public:
        image_filter()
        {
            calculate(m_filter_function);
        }
    private:
        FilterF m_filter_function;
    };


    //-----------------------------------------------image_filter_bilinear
    struct image_filter_bilinear
    {
        static real radius() { return 1.0f; }
        static real calc_weight(real x)
        {
            return 1.0f - x;
        }
    };
    struct image_filter_hanning
    {
        static real radius() { return 1.0f; }
        static real calc_weight(real x)
        {
      return 0.5f + 0.5f * (real)cos(pi * x);        
    }
    };

  //-----------------------------------------------image_filter_hamming

#ifdef _WIN32_WCE
  struct hamming_fixed
  {
    enum {  ANGLE2PI = 1024 };
    float m_table[ANGLE2PI];

    hamming_fixed()
    {
      memset(m_table, 0, sizeof(m_table));
    }

    real get(real x)
    {
        int idx = ((int) (x * 1024.0f)) & ((1 << 10) - 1);
      if( (int) m_table[idx] == 0 )
      {
        m_table[idx] = 0.54f + 0.46f * (real) cos(pi * x);
      }
      return m_table[idx];
    }
  };

    //-----------------------------------------------image_filter_hanning
    struct image_filter_hamming
    {
        static real radius() { return 1.0f; }
        static real calc_weight(real x)
        {
      static hamming_fixed hf;
      return hf.get(x);
        }
    };

#else

    struct image_filter_hamming
    {
        static real radius() { return 1.0f; }
        static real calc_weight(real x)
        {
            return 0.54f + 0.46f * (real)cos(pi * x);
        }
    };
#endif

    //-----------------------------------------------image_filter_hermite
    struct image_filter_hermite
    {
        static real radius() { return 1.0f; }
        static real calc_weight(real x)
        {
            return (2.0f * x - 3.0f) * x * x + 1.0f;
        }
    };
   
    //------------------------------------------------image_filter_quadric
    struct image_filter_quadric
    {
        static real radius() { return 1.5f; }
        static real calc_weight(real x)
        {
            real t;
            if(x <  0.5f) return 0.75f - x * x;
            if(x <  1.5f) {t = x - 1.5f; return 0.5f * t * t;}
            return 0.0f;
        }
    };

    //------------------------------------------------image_filter_bicubic
    class image_filter_bicubic
    {
        static real pow3(real x)
        {
            return (x <= 0.0f) ? 0.0f : x * x * x;
        }

    public:
        static real radius() { return 2.0f; }
        static real calc_weight(real x)
        {
            return
                (1.0f/6.0f) * 
                (pow3(x + 2.0f) - 4.0f * pow3(x + 1.0f) + 6.0f * pow3(x) - 4.0f * pow3(x - 1.0f));
        }
    };

    //-------------------------------------------------image_filter_kaiser
    class image_filter_kaiser
    {
        real a;
        real i0a;
        real epsilon;

    public:
        image_filter_kaiser(real b = 6.33f) :
            a(b), epsilon(1e-12f)
        {
            i0a = 1.0f / bessel_i0(b);
        }

        static real radius() { return 1.0f; }
        real calc_weight(real x) const
        {
            return bessel_i0(a * SQRT(1.0f - x * x)) * i0a;
        }

    private:
        real bessel_i0(real x) const
        {
            int i;
            real sum, y, t;

            sum = 1.;
            y = x * x / 4.f;
            t = y;
        
            for(i = 2; t > epsilon; i++)
            {
                sum += t;
                t *= (real)y / (i * i);
            }
            return sum;
        }
    };

    //----------------------------------------------image_filter_catrom
    struct image_filter_catrom
    {
        static real radius() { return 2.0f; }
        static real calc_weight(real x)
        {
            if(x <  1.0f) return 0.5f * (2.0f + x * x * (-5.0f + x * 3.0f));
            if(x <  2.0f) return 0.5f * (4.0f + x * (-8.0f + x * (5.0f - x)));
            return 0.;
        }
    };

    //---------------------------------------------image_filter_mitchell
    class image_filter_mitchell
    {
        real p0, p2, p3;
        real q0, q1, q2, q3;

    public:
        image_filter_mitchell(real b = 1.0/3.0, real c = 1.0/3.0f) :
            p0((6.0f - 2.0f * b) / 6.0f),
            p2((-18.0f + 12.0f * b + 6.0f * c) / 6.0f),
            p3((12.0f - 9.0f * b - 6.0f * c) / 6.0f),
            q0((8.0f * b + 24.0f * c) / 6.0f),
            q1((-12.0f * b - 48.0f * c) / 6.0f),
            q2((6.0f * b + 30.0f * c) / 6.0f),
            q3((-b - 6.0f * c) / 6.0f)
        {}

        static real radius() { return 2.0f; }
        real calc_weight(real x) const
        {
            if(x < 1.0f) return p0 + x * x * (p2 + x * p3);
            if(x < 2.0f) return q0 + x * (q1 + x * (q2 + x * q3));
            return 0.0f;
        }
    };


    //----------------------------------------------image_filter_spline16
    struct image_filter_spline16
    {
        static real radius() { return 2.0f; }
        static real calc_weight(real x)
        {
            if(x < 1.0f)
            {
                return ((x - 9.0f/5.0f ) * x - 1.0f/5.0f ) * x + 1.0f;
            }
            return ((-1.0f/3.0f * (x-1.0f) + 4.0f/5.0f) * (x-1.0f) - 7.0f/15.0f ) * (x-1.0f);
        }
    };


    //---------------------------------------------image_filter_spline36
    struct image_filter_spline36
    {
        static real radius() { return 3.0f; }
        static real calc_weight(real x)
        {
           if(x < 1.0f)
           {
              return ((13.0f/11.0f * x - 453.0f/209.0f) * x - 3.0f/209.0f) * x + 1.0f;
           }
           if(x < 2.0f)
           {
              return ((-6.0f/11.0f * (x-1.0f) + 270.0f/209.0f) * (x-1.0f) - 156.0f/ 209.0f) * (x-1.0f);
           }
           return ((1.0f/11.0f * (x-2.0f) - 45.0f/209.0f) * (x-2.0f) +  26.0f/209.0f) * (x-2.0f);
        }
    };


    //----------------------------------------------image_filter_gaussian
    struct image_filter_gaussian
    {
        static real radius() { return 2.0f; }
        static real calc_weight(real x) 
        {
            return (real)exp(-2.0f * x * x) * SQRT(2.0f / pi);
        }
    };


    //------------------------------------------------image_filter_bessel
    struct image_filter_bessel
    {
        static real radius() { return 3.2383; } 
        static real calc_weight(real x)
        {
            return (x == 0.0f) ? pi / 4.0f : besj(pi * x, 1) / (2.0f * x);
        }
    };


    //-------------------------------------------------image_filter_sinc
    class image_filter_sinc
    {
    public:
        image_filter_sinc(real r) : m_radius(r < 2.0f ? 2.0f : r) {}
        real radius() const { return m_radius; }
        real calc_weight(real x) const
        {
            if(x == 0.0f) return 1.0f;
            x *= pi;
            return (real)sin(x) / x;
        }
    private:
        real m_radius;
    };


    //-----------------------------------------------image_filter_lanczos
    class image_filter_lanczos
    {
    public:
        image_filter_lanczos(real r) : m_radius(r < 2.0f ? 2.0f : r) {}
        real radius() const { return m_radius; }
        real calc_weight(real x) const
        {
           if(x == 0.0f) return 1.0f;
           if(x > m_radius) return 0.0f;
           x *= pi;
           real xr = x / m_radius;
           return ((real)sin(x) / x) * ((real)sin(xr) / xr);
        }
    private:
        real m_radius;
    };


    //----------------------------------------------image_filter_blackman
    class image_filter_blackman
    {
    public:
        image_filter_blackman(real r) : m_radius(r < 2.0f ? 2.0f : r) {}
        real radius() const { return m_radius; }
        real calc_weight(real x) const
        {
           if(x == 0.0f) return 1.0f;
           if(x > m_radius) return 0.0f;
           x *= pi;
           real xr = x / m_radius;
           return ((real)sin(x) / x) * (0.42f + 0.5f*(real)cos(xr) + 0.08f*(real)cos(2*xr));
        }
    private:
        real m_radius;
    };

    //------------------------------------------------image_filter_sinc36
    class image_filter_sinc36 : public image_filter_sinc
    { public: image_filter_sinc36() : image_filter_sinc(3.0f){} };

    //------------------------------------------------image_filter_sinc64
    class image_filter_sinc64 : public image_filter_sinc
    { public: image_filter_sinc64() : image_filter_sinc(4.0f){} };

    //-----------------------------------------------image_filter_sinc100
    class image_filter_sinc100 : public image_filter_sinc
    { public: image_filter_sinc100() : image_filter_sinc(5.0f){} };

    //-----------------------------------------------image_filter_sinc144
    class image_filter_sinc144 : public image_filter_sinc
    { public: image_filter_sinc144() : image_filter_sinc(6.0f){} };

    //-----------------------------------------------image_filter_sinc196
    class image_filter_sinc196 : public image_filter_sinc
    { public: image_filter_sinc196() : image_filter_sinc(7.0f){} };

    //-----------------------------------------------image_filter_sinc256
    class image_filter_sinc256 : public image_filter_sinc
    { public: image_filter_sinc256() : image_filter_sinc(8.0f){} };

    //---------------------------------------------image_filter_lanczos36
    class image_filter_lanczos36 : public image_filter_lanczos
    { public: image_filter_lanczos36() : image_filter_lanczos(3.0f){} };

    //---------------------------------------------image_filter_lanczos64
    class image_filter_lanczos64 : public image_filter_lanczos
    { public: image_filter_lanczos64() : image_filter_lanczos(4.0f){} };

    //--------------------------------------------image_filter_lanczos100
    class image_filter_lanczos100 : public image_filter_lanczos
    { public: image_filter_lanczos100() : image_filter_lanczos(5.0f){} };

    //--------------------------------------------image_filter_lanczos144
    class image_filter_lanczos144 : public image_filter_lanczos
    { public: image_filter_lanczos144() : image_filter_lanczos(6.0f){} };

    //--------------------------------------------image_filter_lanczos196
    class image_filter_lanczos196 : public image_filter_lanczos
    { public: image_filter_lanczos196() : image_filter_lanczos(7.0f){} };

    //--------------------------------------------image_filter_lanczos256
    class image_filter_lanczos256 : public image_filter_lanczos
    { public: image_filter_lanczos256() : image_filter_lanczos(8.0f){} };

    //--------------------------------------------image_filter_blackman36
    class image_filter_blackman36 : public image_filter_blackman
    { public: image_filter_blackman36() : image_filter_blackman(3.0f){} };

    //--------------------------------------------image_filter_blackman64
    class image_filter_blackman64 : public image_filter_blackman
    { public: image_filter_blackman64() : image_filter_blackman(4.0f){} };

    //-------------------------------------------image_filter_blackman100
    class image_filter_blackman100 : public image_filter_blackman
    { public: image_filter_blackman100() : image_filter_blackman(5.0f){} };

    //-------------------------------------------image_filter_blackman144
    class image_filter_blackman144 : public image_filter_blackman
    { public: image_filter_blackman144() : image_filter_blackman(6.0f){} };

    //-------------------------------------------image_filter_blackman196
    class image_filter_blackman196 : public image_filter_blackman
    { public: image_filter_blackman196() : image_filter_blackman(7.0f){} };

    //-------------------------------------------image_filter_blackman256
    class image_filter_blackman256 : public image_filter_blackman
    { public: image_filter_blackman256() : image_filter_blackman(8.0f){} };


}

#endif
