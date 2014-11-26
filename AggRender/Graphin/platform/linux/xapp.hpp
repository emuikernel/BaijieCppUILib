#ifndef XAPP_HPP_INCLUDED
#define XAPP_HPP_INCLUDED

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "agg2D.h"
#include "agg_basics.h"
#include "primitives.h"
#include "util/agg_color_conv_rgb8.h"
#include "platform/agg_platform_support.h"

namespace xlib
{
  class display
  {
    friend class window;
    public:
    display ( const char* name = "" );
    /*
    {
      hd = XOpenDisplay ( name );
    }*/
    ~display()
    {
      if ( !hd ) return;
      XCloseDisplay ( hd );
	    hd = 0;
    }

    operator Display* () const { return hd; }

    bool is_valid() const { return hd != 0; }

    private:
      Display*          hd;
      int               screen;
      int               depth;
      Visual*           visual;
      agg::pix_format_e format;
      agg::pix_format_e sys_format;
      int               byte_order;
      unsigned          bpp;
      unsigned          sys_bpp;
  };

  class app: public display, public resource
  {
  public:
    static bool init( const char* disp_name = "" )
    {
       if(pinst) return false;
       pinst = new app(disp_name);
       return is_valid();
    }
    static bool is_valid() { return pinst && pinst->display::is_valid(); }
    static app& inst()
    {
       assert( pinst );
       if(!pinst) init();
       return *pinst;
    }

    static void start();
    static bool step(int& rv);
    static void stop(int rv);
	  static int  run();

    //operator Display* () const { return display::operator Display*(); }

  private:
    app( const char* command_line, const char* disp_name = "" ):display(disp_name) {}
    ~app();
    static bool dispatch_event ( XEvent& report );
    static handle<app> pinst;
    static int  retval;
    static bool alive;
  };

  class color
  {
    public:

      color ( short red, short green, short blue ): disp( app::inst() )
	    {
	      map = DefaultColormap ( (Display*)disp, 0 );
	      set_color ( red, green, blue );
	    }

      void set ( color& c )
	    {
	      free_color();
	      set_color ( c.red(), c.green(), c.blue() );
	    }

      ~color()
	    {
	      free_color();
	    }

      long pixel() { return sysc.pixel; }
      unsigned short red()   { return sysc.red * 255 / 65535; }
      unsigned short green() { return sysc.green * 255 / 65535; }
      unsigned short blue()  { return sysc.blue * 255 / 65535; }

    private:

      void free_color()
      {
        unsigned long pixels[2];
        pixels[0] = pixel();

        XFreeColors ( disp, map,   pixels, 1, 0 );
      }

      void set_color ( short red, short green, short blue )
      {
        sysc.red = red * 65535 / 255;
        sysc.green = green * 65535 / 255;
        sysc.blue = blue * 65535 / 255;
        sysc.flags = DoRed | DoGreen | DoBlue;

        if ( ! XAllocColor ( disp, map, &sysc ) )
        {
            //throw create_color_exception ( "Could not create the color." );
            assert(false);
        }
	    }
      XColor    sysc;
      display&  disp;
      Colormap  map;
    };

  class character
  {
      enum _ { MAX_CHAR_TEXT = 10 };
      friend class app;
      friend class window;
    public:
      character ( ) : key(0), state (0)	{ memset(text, 0 ,MAX_CHAR_TEXT + 1); }
      ~character (){}

      bool is_delete_key() const { return key == XK_Delete; }
      bool is_backspace_key() const { return key == XK_BackSpace; }
      bool is_left_arrow_key() const { return key == XK_Left || key == XK_KP_Left; }
      bool is_right_arrow_key() const { return key == XK_Right || key == XK_KP_Right; }

      bool is_printable() const
      {
        return ( ( ( key >= XK_KP_Space ) && ( key <= XK_KP_9 ) )
              || ( ( key >= XK_space ) && ( key <= XK_asciitilde ) ) );
      }

      bool is_shift_key_pressed() const
	    {
	      return (state & ShiftMask);
	    }

      const char* get_text() const
	    {
	      if ( !is_printable() ) return "";
	      return text;
	    }
    protected:
      long key;
      char text[MAX_CHAR_TEXT + 1];
      long state;
  };



}


#endif // XAPP_HPP_INCLUDED
