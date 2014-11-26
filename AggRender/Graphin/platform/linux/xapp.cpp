#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>

#include <iostream>
using namespace std;

#include "xapp.hpp"
#include "xwnd.hpp"

namespace xlib
{

  bool app::alive = false;
  int  app::retval = 0;
  handle<app> app::pinst;

  app::~app()
  {
    window::destroy_all();
  }


  bool app::step(int& rv)
	{
	  if(!is_valid() || !alive)
	  {
	    rv = retval;
	    return false;
	  }
    XEvent evt;
    XNextEvent ( inst(), &evt );
    dispatch_event ( evt );
    return true;
	}

  void app::start()
	{
    retval = 0;
    alive = true;
	}

  void app::stop(int rv)
	{
    retval = rv;
    alive = false;
    window::destroy_all();
	}

	int app::run()
	{
	  start();
	  int rv;
	  while( alive )
	    if( !step(rv) )
	      break;
    return rv;
	}

  bool app::dispatch_event ( XEvent& evt )
	{
	  window* pw = window::find(evt.xany.window);
	  if( !pw )
	    return false;

    return pw->dispatch_event(evt);
	}

  display::display( const char* name)
  {
      hd      = XOpenDisplay ( name );
      screen  = XDefaultScreen(hd);
      depth   = XDefaultDepth(hd, screen);
      visual  = XDefaultVisual(hd, screen);

      unsigned long r_mask = visual->red_mask;
      unsigned long g_mask = visual->green_mask;
      unsigned long b_mask = visual->blue_mask;

      if(depth < 15 || r_mask == 0 || g_mask == 0 || b_mask == 0)
      {
          fprintf(stderr,
                  "There's no Visual compatible with minimal Graphin requirements:\n"
                  "At least 15-bit color depth and True- or DirectColor class.\n\n");
          XCloseDisplay(hd); hd = 0;
          return;
       }

       int t = 1;
       int hw_byte_order = LSBFirst;
       if(*(char*)&t == 0) hw_byte_order = MSBFirst;

       // Perceive SYS-format by mask
       switch(depth)
       {
           case 15:
                sys_bpp = 16;
                if(r_mask == 0x7C00 && g_mask == 0x3E0 && b_mask == 0x1F)
                {
                    sys_format = agg::pix_format_rgb555;
                    byte_order = hw_byte_order;
                }
                break;

            case 16:
                sys_bpp = 16;
                if(r_mask == 0xF800 && g_mask == 0x7E0 && b_mask == 0x1F)
                {
                    sys_format = agg::pix_format_rgb565;
                    byte_order = hw_byte_order;
                }
                break;

            case 24:
            case 32:
                sys_bpp = 32;
                if(g_mask == 0xFF00)
                {
                    if(r_mask == 0xFF && b_mask == 0xFF0000)
                    {
                        switch(format)
                        {
                            case agg::pix_format_rgba32:
                                sys_format = agg::pix_format_rgba32;
                                byte_order = LSBFirst;
                                break;

                            case agg::pix_format_abgr32:
                                sys_format = agg::pix_format_abgr32;
                                byte_order = MSBFirst;
                                break;

                            default:
                                byte_order = hw_byte_order;
                                sys_format = (hw_byte_order == LSBFirst) ? agg::pix_format_rgba32 : agg::pix_format_abgr32;
                                break;
                        }
                    }
                    if(r_mask == 0xFF0000 && b_mask == 0xFF)
                    {
                        switch(format)
                        {
                            case agg::pix_format_argb32:
                                sys_format = agg::pix_format_argb32;
                                byte_order = MSBFirst;
                                break;

                            case agg::pix_format_bgra32:
                                sys_format = agg::pix_format_bgra32;
                                byte_order = LSBFirst;
                                break;

                            default:
                                byte_order = hw_byte_order;
                                sys_format = (hw_byte_order == MSBFirst) ? agg::pix_format_argb32 : agg::pix_format_bgra32;
                                break;
                        }
                    }
                }
                break;
        }

        if(sys_format == agg::pix_format_undefined)
        {
            fprintf(stderr,
                   "RGB masks of Visual are not compatible with Graphin pixel formats:\n"
                   "R=%08lx, R=%08lx, B=%08lx\n", r_mask, g_mask, b_mask);
            XCloseDisplay(hd); hd = 0;
            return;
        }
    }
}
