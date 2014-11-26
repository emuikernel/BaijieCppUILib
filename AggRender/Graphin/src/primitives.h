#ifndef __stakan_h__
#define __stakan_h__

#include "assert.h"

  template <class T>
  class handle
  {
  public:
    handle ()               { _ptr = 0; }
    handle ( T* p )         { _ptr = 0; _set ( p ); }
    handle ( const handle<T>& p )     { _ptr = 0; _set ( p._ptr ); }

    ~handle ()              {  _set (0);  }

    handle<T>& operator= ( T *p )               { _set ( p ); return *this;  }
    handle<T>& operator=( const handle<T>& p )  { _set ( p._ptr );  return *this; }

    T* operator-> () const  { return _ptr;  }
    T* ptr () const         { return _ptr; }

    operator T* () const    { return _ptr; }

    bool is_null () const   { return _ptr == 0; }

    bool operator == ( T *p ) const  { return _ptr == p;  }

    T* detach ()            { T* t = _ptr; _ptr = 0; return t; }

    private:
    T* _ptr;

    void  _set ( T *p )
    {
      if ( _ptr == p ) return;
      if ( _ptr ) _ptr->release();
      _ptr = p;
      if ( _ptr ) _ptr->add_ref();
    }
  };

  class resource
  {
  protected:
    int _ref_cntr;
  public:
    resource ()             {  _ref_cntr = 0; }
    virtual ~resource ()    {  assert ( _ref_cntr == 0 ); }
    virtual long release()
    {
        assert(_ref_cntr > 0);
        long t = --_ref_cntr;
        if(t == 0)
          delete this;
        return t;
    }
    virtual void add_ref() { ++_ref_cntr; }
  };
  template <class T>
  inline T
    limit ( T v, T minv, T maxv )
  {
    if (minv >= maxv)
      return minv;
    if (v > maxv) return maxv;
    if (v < minv) return minv;
    return v;
  }

#ifndef min

  template <typename T>
    inline T min(T a, T b ) {  return a < b? a : b;  }

  template <typename T>
    inline T max(T a, T b ) {  return a > b? a : b;  }

#endif

#endif

