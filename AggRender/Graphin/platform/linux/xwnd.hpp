#ifndef XWND_HPP_INCLUDED
#define XWND_HPP_INCLUDED

#include "window.h"

#include "xapp.hpp"
#include "agg2D.h"
#include "primitives.h"
#include "objects.h"

namespace xlib
{

  typedef agg::rect_i rect;
  inline  rect::value_type width(const rect& r) { return r.x2 - r.x1 + 1; }
  inline  rect::value_type height(const rect& r) { return r.y2 - r.y1 + 1; }
  inline  rect::value_type top(const rect& r) { return r.y1; }
  inline  rect::value_type left(const rect& r) { return r.x1; }


  class window
  {
    friend class app;
    public:
      window():env(app::inst()),
        back_color( 197, 194, 197 ), // grey
        border_color ( 255, 255, 255 ),
        ximg(0),
        place(100,100,600,400),
        next(0),prev(0)
      {
         remember();
      }

      virtual ~window()
      {
        forget();
      }

      static window* find(Window w)
      {
        for(window* t = first; t; t = t->next)
          if( t->hwnd ) return t;
        return 0;
      }

      virtual bool create();
      virtual void destroy();

      virtual void refresh();

	    virtual void hide(); // iconify
	    virtual void show();
	    virtual void collapse();
	    virtual void expand();

      virtual bool is_valid() const { return hwnd != 0; }
      virtual bool is_root() const { return true; }
      virtual rect get_place() const;

      virtual void set_background ( color& c );
      virtual void set_focus();

      WINDOW_STATE get_state() const { return state; }
      void         set_state(WINDOW_STATE st);

// events:
      virtual void created() {}
      virtual void destroyed() {}
      virtual void exposed();
      virtual void delegate_paint(agg::rendering_buffer& rbuf) = 0;
      virtual void position_changed() {}

      virtual void shown() {}
      virtual void hidden() {}

      virtual void on_left_button_down ( int x, int y ) {}
      virtual void on_right_button_down ( int x, int y ) {}

      virtual void on_left_button_up ( int x, int y ) {}
      virtual void on_right_button_up ( int x, int y ) {}

      virtual void on_mouse_enter ( int x, int y ) {}
      virtual void on_mouse_exit ( int x, int y ) {}
      virtual void on_mouse_move ( int x, int y ) {}

      virtual void on_got_focus() {}
      virtual void on_lost_focus() {}

      virtual void on_key_press ( character c ) {}
      virtual void on_key_release ( character c ) {}

    protected:
      //
      // Not copyable
      //
      window ( const window& );
      void operator = ( window& );

      void remember() // insert it into global list
      {
        next = first;
        first = this;
      }

      void forget()  // remove it from global list
      {
        if( next ) next->prev = prev;
        if( prev ) prev->next = next;
        if( first == this ) first = next;
      }

      static void destroy_all()
      {
        while( first )
          delete first;
      }

      bool dispatch_event ( XEvent& evt );
      void put_image(const agg::rendering_buffer* src, agg::pix_format_e format);

      app&   env;

      Window hwnd;
      Window hwnd_parent;
      color  back_color, border_color;
      WINDOW_STATE state;

      GC                    gc;
      //unsigned char*        rbuf_data;
      //agg::rendering_buffer rbuf;
      XImage*               ximg;
      handle<image>         buffer;

      rect  place; // initial place
      rect  dirty_area;


      window *next,*prev; // in list of all windows:
      static window* first;

      static Atom atomDelete;

  };
}


#endif // XWND_HPP_INCLUDED
