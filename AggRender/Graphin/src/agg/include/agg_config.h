#ifndef AGG_CONFIG_INCLUDED
#define AGG_CONFIG_INCLUDED

// This file can be used to redefine certain data types.

#ifdef UNDER_CE
  typedef float real;
  #define REAL(n) ((float)n);

  #define SQRT(r) sqrtf(r)
  #define CEIL(r) ceilf(r)
  #define FABS(r) fabsf(r)
  #define FLOOR(r) floorf(r)
  #define FMOD(r1,r2) fmodf(r1,r2)

#else
  typedef double real;
  #define REAL(n) n

  #define SQRT(r) sqrt(r)
  #define CEIL(r) ceil(r)
  #define FABS(r) fabs(r)
  #define FLOOR(r) floor(r)
  #define FMOD(r1,r2) fmod(r1,r2)

#endif

//---------------------------------------
// 1. Default basic types such as:
// 
// AGG_INT8
// AGG_INT8U
// AGG_INT16
// AGG_INT16U
// AGG_INT32
// AGG_INT32U
// AGG_INT64
// AGG_INT64U
//
// Just replace this file with new defines if necessary. 
// For example, if your compiler doesn't have a 64 bit integer type
// you can still use AGG if you define the follows:
//
// #define AGG_INT64  int
// #define AGG_INT64U unsigned
//
// It will result in overflow in 16 bit-per-component image/pattern resampling
// but it won't result any crash and the rest of the library will remain 
// fully functional.


//---------------------------------------
// 2. Default rendering_buffer type. Can be:
//
// Provides faster access for massive pixel operations, 
// such as blur, image filtering:
// #define AGG_RENDERING_BUFFER row_ptr_cache<int8u>
// 
// Provides cheaper creation and destruction (no mem allocs):
// #define AGG_RENDERING_BUFFER row_accessor<int8u>
//
// You can still use both of them simultaneouslyin your applications 
// This #define is used only for default rendering_buffer type,
// in short hand typedefs like pixfmt_rgba32.

#endif
