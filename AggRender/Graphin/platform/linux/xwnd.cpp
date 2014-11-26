#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>

#include "xapp.hpp"
#include "xwnd.hpp"

#include "window.h"

namespace xlib
{

  Atom    window::atomDelete = None;
  window* window::first = 0;

  const int event_mask = ExposureMask |
    ButtonPressMask |
    ButtonReleaseMask |
    EnterWindowMask |
    LeaveWindowMask |
    PointerMotionMask |
    FocusChangeMask |
    KeyPressMask |
    KeyReleaseMask |
    SubstructureNotifyMask |
    StructureNotifyMask |
    SubstructureRedirectMask;

/*
_NET_WM_WINDOW_TYPE_DESKTOP, ATOM
_NET_WM_WINDOW_TYPE_DOCK, ATOM
_NET_WM_WINDOW_TYPE_TOOLBAR, ATOM
_NET_WM_WINDOW_TYPE_MENU, ATOM
_NET_WM_WINDOW_TYPE_UTILITY, ATOM
_NET_WM_WINDOW_TYPE_SPLASH, ATOM
_NET_WM_WINDOW_TYPE_DIALOG, ATOM
_NET_WM_WINDOW_TYPE_NORMAL, ATOM
*/

  bool window::create()
	{
	  if(hwnd)
	    return true;

    int width = xlib::width(place);
    int height = xlib::height(place);

    hwnd = XCreateSimpleWindow ( env,
             RootWindow((Display*)env,0),
             left(place),
             top(place),
             width,
             height,
             0,
             WhitePixel((Display*)env,0),
             WhitePixel((Display*)env,0));


    if(atomDelete != None)
      atomDelete = XInternAtom ( env, "WM_DELETE_WINDOW", false );

    if ( !is_root() )
    {
      // keeps this window in front of its parent at all times
      XSetTransientForHint ( env,
           hwnd,
           hwnd_parent );
      // make sure the app doesn't get killed when this
      // window gets destroyed
      XSetWMProtocols ( env, hwnd, &atomDelete, 1 );
    }

    if ( hwnd == 0 )
      return false;

    gc = XCreateGC(env, hwnd, 0, 0);
    //rbuf_data = new unsigned char[ width * height * (env.bpp / 8)];
    //rbuf.attach(rbuf_data, width, height, width * (env.bpp / 8) );
    /*
    buffer = new image(width, height);

    ximg = XCreateImage(env, env.visual, env.depth, ZPixmap,
                         0,
                         (char*)buffer->pmap.buf(),
                         width,
                         height,
                         env.sys_bpp,
                         width * (env.sys_bpp / 8));
    ximg->byte_order = env.byte_order;
    */

    set_background ( back_color );
    created();

    return true;
  }

  void window::destroy()
	{
	  hide();
 	  if ( hwnd )
    {
	      XDestroyWindow ( env, hwnd );
	      hwnd = 0;
    }
	  destroyed();
	}

  void window::refresh ()
	{
	  XClearWindow ( env, hwnd );
	  XFlush ( env );
	  exposed();
	}

  void window::hide()
	{
	  XUnmapWindow ( env, hwnd );
	  XFlush ( env );
	}

  void window::show()
	{
    create();

	  XSelectInput ( env, hwnd, event_mask );
	  XMapWindow ( env, hwnd );
	  XFlush ( env );
	}

	void window::collapse()
	{
    XIconifyWindow(env, hwnd, DefaultScreen((Display*)env));
	}


/*_NET_WM_STATE_MODAL, ATOM
_NET_WM_STATE_STICKY, ATOM
_NET_WM_STATE_MAXIMIZED_VERT, ATOM
_NET_WM_STATE_MAXIMIZED_HORZ, ATOM
_NET_WM_STATE_SHADED, ATOM
_NET_WM_STATE_SKIP_TASKBAR, ATOM
_NET_WM_STATE_SKIP_PAGER, ATOM
_NET_WM_STATE_HIDDEN, ATOM
_NET_WM_STATE_FULLSCREEN, ATOM
_NET_WM_STATE_ABOVE, ATOM
_NET_WM_STATE_BELOW, ATOM
_NET_WM_STATE_DEMANDS_ATTENTION, ATOM*/

  static Atom WM_STATE = 0;
  static Atom WM_STATE_FULLSCREEN = 0;
  static Atom WM_STATE_MAXIMIZED_VERT = 0;
  static Atom WM_STATE_MAXIMIZED_HORZ = 0;
  static Atom WM_STATE_HIDDEN = 0;

  static void atomize(app& env)
  {
    if( !WM_STATE )
    {
      WM_STATE                  = XInternAtom(env, "_NET_WM_STATE", False);
      WM_STATE_FULLSCREEN       = XInternAtom(env, "_NET_WM_STATE_FULLSCREEN", False);
      WM_STATE_MAXIMIZED_VERT   = XInternAtom(env, "_NET_WM_STATE_MAXIMIZED_VERT", False);
      WM_STATE_MAXIMIZED_HORZ   = XInternAtom(env, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
      WM_STATE_HIDDEN           = XInternAtom(env, "_NET_WM_STATE_HIDDEN", False);
    }
  }

	void window::expand()
	{
	  if( state == WINDOW_HIDDEN )
	  {
	    XSelectInput ( env, hwnd, event_mask );
	    XMapWindow ( env, hwnd );
	  }

	  atomize(env);

    XEvent xev = {0};
    xev.type = ClientMessage;
    xev.xclient.window = hwnd;
    xev.xclient.message_type = WM_STATE;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 2;
    xev.xclient.data.l[1] = WM_STATE_MAXIMIZED_VERT;
    xev.xclient.data.l[2] = WM_STATE_MAXIMIZED_HORZ;

    XSendEvent(env, DefaultRootWindow((Display*)env), False, SubstructureNotifyMask, &xev);

    XFlush(env);
	}

	void window::exposed()
	{
      xlib::rect rc = get_place();
      int view_w = xlib::width(rc);
      int view_h = xlib::height(rc);
      if( !buffer ||
          view_w != int(buffer->width()) ||
          view_h != int(buffer->height()) )
      {
        buffer = new image(view_w,view_h);
        dirty_area.x1 = 0; rc;//agg::rect_i(rc.x1, rc.y1, rc.x2, rc.y2);
        dirty_area.y1 = 0;
        dirty_area.x2 = dirty_area.x1 + buffer->width();
        dirty_area.y2 = dirty_area.y1 + buffer->height();
        if(ximg)
        {
          ximg->data = 0;
          XDestroyImage(ximg);
        }
        ximg = XCreateImage(env, env.visual, env.depth, ZPixmap,
                       0,
                       (char*)buffer->pmap.buf(),
                       view_w,
                       view_h,
                       env.sys_bpp,
                       view_w * (env.sys_bpp / 8));
        ximg->byte_order = env.byte_order;

      }

      agg::rendering_buffer rbuf;
      rbuf.attach(buffer->pmap.buf(), buffer->pmap.width(), buffer->pmap.height(), buffer->pmap.stride());

      delegate_paint(rbuf);

      put_image( &rbuf,agg::pix_format_bgra32 );
	}


  //XIconifyWindow(display, win, DefaultScreen(display));
  /* de-iconify our window. the icon window will be automatically */
  /* unmapped by this operation.                                  */
  //XMapWindow(display, win);


/*  void window::set_state(WINDOW_STATE cmd)
  {
    //XGetWindowProperty

    XEvent xev = {0};
    xev.type = ClientMessage;
    xev.xclient.window = hwnd;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1;
    xev.xclient.data.l[1] = fullscreen;
    xev.xclient.data.l[2] = 0;

    switch(cmd)
    {
      case WINDOW_SHOW:
    }

    XSendEvent(dpy, DefaultRootWindow(env), False,
    SubstructureNotifyMask, &xev);
  }

  SHOW_CMD window::get_state(SHOW_CMD cmd)
  {

  }


function GDK_WINDOW_GET_MAXIMIZED(Window: PGdkWindowPrivate): gboolean;
00229 var
00230   xdisplay: PDisplay;
00231   xwindow: TWindow;
00232
00233   atomtype: x.TAtom;
00234   format: gint;
00235   nitems: gulong;
00236   bytes_after: gulong;
00237   state_array: pguint;
00238   _NET_WM_STATE,
00239   _NET_WM_STATE_MAXIMIZED_VERT,
00240   _NET_WM_STATE_MAXIMIZED_HORZ: x.TAtom;
00241   X: Integer;
00242   maximized_horz, maximized_vert: Boolean;
00243 begin
00244   Result := False;
00245   XWindow := GDK_WINDOW_XWINDOW (Window);
00246   XDisplay := GDK_WINDOW_XDISPLAY (Window);
00247
00248   _NET_WM_STATE := XInternAtom(xdisplay, '_NET_WM_STATE', false);
00249   _NET_WM_STATE_MAXIMIZED_VERT := XInternAtom(xdisplay, '_NET_WM_STATE_MAXIMIZED_VERT', false);
00250   _NET_WM_STATE_MAXIMIZED_HORZ := XInternAtom(xdisplay, '_NET_WM_STATE_MAXIMIZED_HORZ', false);
00251
00252   XGetWindowProperty (xdisplay, xwindow,
00253              _NET_WM_STATE,
00254              0, MaxInt, False, XA_ATOM, @atomtype, @format, @nitems,
00255              @bytes_after, gpointer(@state_array));
00256
00257   if (atomtype = XA_ATOM) and (format = 32) and  (nitems > 0) then
00258   begin
00259     maximized_horz := False;
00260     maximized_vert := False;
00261     for X := 0 to nitems-1 do begin
00262       if (state_array[X] = _NET_WM_STATE_MAXIMIZED_VERT) then maximized_vert := True;
00263       if (state_array[X] = _NET_WM_STATE_MAXIMIZED_HORZ) then maximized_horz := True;
00264       Result:=(maximized_horz and maximized_vert);
00265
00266       if Result then Break;
00267     end;
00268     XFree (state_array);
00269   end;
00270 end;


  static bool rq_state(Display *display, Window hwnd, Atom feature)
  {
     Atom _NET_SUPPORTING_WM_CHECK = XInternAtom(dpy, "_NET_SUPPORTING_WM_CHECK", True);
     Atom _NET_SUPPORTED = XInternAtom(dpy, "_NET_SUPPORTING_WM_CHECK", True);
      wxMAKE_ATOM(_NET_SUPPORTING_WM_CHECK, display);
      wxMAKE_ATOM(_NET_SUPPORTED, display);

      // FIXME: We may want to cache these checks. Note that we can't simply
      //        remember the results in global variable because the WM may go
      //        away and be replaced by another one! One possible approach
      //        would be invalidate the case every 15 seconds or so. Since this
      //        code is currently only used by wxTopLevelWindow::ShowFullScreen,
      //        it is not important that it is not optimized.
      //
      //        If the WM supports ICCCM (i.e. the root window has
      //        _NET_SUPPORTING_WM_CHECK property that points to a WM-owned
      //        window), we could watch for DestroyNotify event on the window
      //        and invalidate our cache when the windows goes away (= WM
      //        is replaced by another one). This is what GTK+ 2 does.
      //        Let's do it only if it is needed, it requires changes to
      //        the event loop.

      Atom type;
      Window *wins;
      Atom *atoms;
      int format;
      unsigned long after;
      unsigned long nwins, natoms;

      // Is the WM ICCCM supporting?
      XGetWindowProperty(display, rootWnd,
                         _NET_SUPPORTING_WM_CHECK, 0, UINT_MAX,
                         False, XA_WINDOW, &type, &format, &nwins,
                         &after, (unsigned char **)&wins);
      if ( type != XA_WINDOW || nwins <= 0 || wins[0] == None )
         return FALSE;
      XFree(wins);

      // Query for supported features:
      XGetWindowProperty(display, rootWnd,
                         _NET_SUPPORTED, 0, UINT_MAX,
                         False, XA_ATOM, &type, &format, &natoms,
                         &after, (unsigned char **)&atoms);
      if ( type != XA_ATOM || atoms == NULL )
          return FALSE;

      // Lookup the feature we want:
      for (unsigned i = 0; i < natoms; i++)
      {
          if ( atoms[i] == feature )
          {
              XFree(atoms);
              return TRUE;
          }
      }
      XFree(atoms);
      return FALSE;
  }
  */

  void window::set_background ( color& c )
  {
    // hold a ref to the alloc'ed color
    back_color.set ( c );

    XSetWindowBackgroundPixmap( env, hwnd, None );
    /*XSetWindowBackground ( env,
         hwnd,
         c.pixel() ); */
    refresh();
  }

  void window::set_focus()
	{
	  XSetInputFocus ( env,
			   hwnd,
			   RevertToParent,
			   CurrentTime );
	  refresh();
	}

  rect window::get_place() const
	{
	  Window root = 0;
	  int x = 0, y = 0;
	  unsigned int width = 0, height = 0, border_width = 0, depth = 0;
	  XGetGeometry ( env,
			 hwnd,
			 &root,
			 &x,
			 &y,
			 &width,
			 &height,
			 &border_width,
			 &depth );

	  return rect( x,y, x + width - 1, y + height -1 );
	}


  bool window::dispatch_event ( XEvent& evt )
	{
    switch ( evt.type )
		{
      case Expose:
        exposed();
        break;
		case ButtonPress:
	      if ( evt.xbutton.button & Button2 )
           on_right_button_down ( evt.xbutton.x, evt.xbutton.y );
		    else if ( evt.xbutton.button & Button1 )
           on_left_button_down ( evt.xbutton.x, evt.xbutton.y );
		    break;
		case ButtonRelease:
		    if ( evt.xbutton.button & Button2 )
            on_right_button_up ( evt.xbutton.x, evt.xbutton.y );
		    else if ( evt.xbutton.button & Button1 )
            on_left_button_up ( evt.xbutton.x, evt.xbutton.y );
		    break;
		case EnterNotify:
		    on_mouse_enter ( evt.xcrossing.x, evt.xcrossing.y );
		    break;
		case LeaveNotify:
		    on_mouse_exit ( evt.xcrossing.x, evt.xcrossing.y );
		    break;
		case MotionNotify:
		    on_mouse_move ( evt.xmotion.x, evt.xmotion.y );
		    break;
		case FocusIn:
		    on_got_focus();
		    break;
		case FocusOut:
		    on_lost_focus();
		    break;
		case KeyPress:
		case KeyRelease:
        {
          character cp;
          KeySym keysym;
          XComposeStatus status;
          int count = XLookupString ( &evt.xkey, cp.text, character::MAX_CHAR_TEXT, &keysym, &status );
          cp.text[count] = 0;
          if ( evt.type == KeyPress )
            on_key_press ( cp );
          else
            on_key_release ( cp );
        } break;

		case MapNotify:
		  state = WINDOW_SHOWN;
      shown();
		  break;

		case UnmapNotify:
		  state = WINDOW_HIDDEN;
	    hidden();
		  break;

    case ConfigureNotify:
      //XClearWindow ( env, hwnd );
	    //XFlush ( env );
      position_changed();
      break;

		case ClientMessage:
		  {
		    if ( atomDelete == (Atom)evt.xclient.data.l[0] )
          destroy();
		    break;
		  }
		}
    return true;
	}

  void window::put_image(const agg::rendering_buffer* src,agg::pix_format_e format)
  {
    if(ximg == 0) return;

    if(format == env.sys_format)
    {
      //ximg->data = (char*)src->buf();
      //ximg->width = src->width(),
      //ximg->height = src->height(),
      //ximg->bytes_per_line = (src->stride());

      Display* pd = env;
      Drawable dw = hwnd;
      GC pgc = gc;

      XPutImage(pd, dw, pgc, ximg, 0, 0, 0, 0, ximg->width, ximg->height);
      return;
    }

    int row_len = src->width() * env.sys_bpp / 8;
    unsigned char* buf_tmp = new unsigned char[row_len * src->height()];

    agg::rendering_buffer rbuf_tmp;
    rbuf_tmp.attach(buf_tmp, src->width(), src->height(), row_len);

    switch(env.sys_format)
    {
        default: break;
        case agg::pix_format_rgb555:
            switch(format)
            {
                default: break;
                case agg::pix_format_rgb555: color_conv(&rbuf_tmp, src, agg::color_conv_rgb555_to_rgb555()); break;
                case agg::pix_format_rgb565: color_conv(&rbuf_tmp, src, agg::color_conv_rgb565_to_rgb555()); break;
                case agg::pix_format_rgb24:  color_conv(&rbuf_tmp, src, agg::color_conv_rgb24_to_rgb555());  break;
                case agg::pix_format_bgr24:  color_conv(&rbuf_tmp, src, agg::color_conv_bgr24_to_rgb555());  break;
                case agg::pix_format_rgba32: color_conv(&rbuf_tmp, src, agg::color_conv_rgba32_to_rgb555()); break;
                case agg::pix_format_argb32: color_conv(&rbuf_tmp, src, agg::color_conv_argb32_to_rgb555()); break;
                case agg::pix_format_bgra32: color_conv(&rbuf_tmp, src, agg::color_conv_bgra32_to_rgb555()); break;
                case agg::pix_format_abgr32: color_conv(&rbuf_tmp, src, agg::color_conv_abgr32_to_rgb555()); break;
            }
            break;

        case agg::pix_format_rgb565:
            switch(format)
            {
                default: break;
                case agg::pix_format_rgb555: color_conv(&rbuf_tmp, src, agg::color_conv_rgb555_to_rgb565()); break;
                case agg::pix_format_rgb565: color_conv(&rbuf_tmp, src, agg::color_conv_rgb565_to_rgb565()); break;
                case agg::pix_format_rgb24:  color_conv(&rbuf_tmp, src, agg::color_conv_rgb24_to_rgb565());  break;
                case agg::pix_format_bgr24:  color_conv(&rbuf_tmp, src, agg::color_conv_bgr24_to_rgb565());  break;
                case agg::pix_format_rgba32: color_conv(&rbuf_tmp, src, agg::color_conv_rgba32_to_rgb565()); break;
                case agg::pix_format_argb32: color_conv(&rbuf_tmp, src, agg::color_conv_argb32_to_rgb565()); break;
                case agg::pix_format_bgra32: color_conv(&rbuf_tmp, src, agg::color_conv_bgra32_to_rgb565()); break;
                case agg::pix_format_abgr32: color_conv(&rbuf_tmp, src, agg::color_conv_abgr32_to_rgb565()); break;
            }
            break;

        case agg::pix_format_rgba32:
            switch(format)
            {
                default: break;
                case agg::pix_format_rgb555: color_conv(&rbuf_tmp, src, agg::color_conv_rgb555_to_rgba32()); break;
                case agg::pix_format_rgb565: color_conv(&rbuf_tmp, src, agg::color_conv_rgb565_to_rgba32()); break;
                case agg::pix_format_rgb24:  color_conv(&rbuf_tmp, src, agg::color_conv_rgb24_to_rgba32());  break;
                case agg::pix_format_bgr24:  color_conv(&rbuf_tmp, src, agg::color_conv_bgr24_to_rgba32());  break;
                case agg::pix_format_rgba32: color_conv(&rbuf_tmp, src, agg::color_conv_rgba32_to_rgba32()); break;
                case agg::pix_format_argb32: color_conv(&rbuf_tmp, src, agg::color_conv_argb32_to_rgba32()); break;
                case agg::pix_format_bgra32: color_conv(&rbuf_tmp, src, agg::color_conv_bgra32_to_rgba32()); break;
                case agg::pix_format_abgr32: color_conv(&rbuf_tmp, src, agg::color_conv_abgr32_to_rgba32()); break;
            }
            break;

        case agg::pix_format_abgr32:
            switch(format)
            {
                default: break;
                case agg::pix_format_rgb555: color_conv(&rbuf_tmp, src, agg::color_conv_rgb555_to_abgr32()); break;
                case agg::pix_format_rgb565: color_conv(&rbuf_tmp, src, agg::color_conv_rgb565_to_abgr32()); break;
                case agg::pix_format_rgb24:  color_conv(&rbuf_tmp, src, agg::color_conv_rgb24_to_abgr32());  break;
                case agg::pix_format_bgr24:  color_conv(&rbuf_tmp, src, agg::color_conv_bgr24_to_abgr32());  break;
                case agg::pix_format_abgr32: color_conv(&rbuf_tmp, src, agg::color_conv_abgr32_to_abgr32()); break;
                case agg::pix_format_rgba32: color_conv(&rbuf_tmp, src, agg::color_conv_rgba32_to_abgr32()); break;
                case agg::pix_format_argb32: color_conv(&rbuf_tmp, src, agg::color_conv_argb32_to_abgr32()); break;
                case agg::pix_format_bgra32: color_conv(&rbuf_tmp, src, agg::color_conv_bgra32_to_abgr32()); break;
            }
            break;

        case agg::pix_format_argb32:
            switch(format)
            {
                default: break;
                case agg::pix_format_rgb555: color_conv(&rbuf_tmp, src, agg::color_conv_rgb555_to_argb32()); break;
                case agg::pix_format_rgb565: color_conv(&rbuf_tmp, src, agg::color_conv_rgb565_to_argb32()); break;
                case agg::pix_format_rgb24:  color_conv(&rbuf_tmp, src, agg::color_conv_rgb24_to_argb32());  break;
                case agg::pix_format_bgr24:  color_conv(&rbuf_tmp, src, agg::color_conv_bgr24_to_argb32());  break;
                case agg::pix_format_rgba32: color_conv(&rbuf_tmp, src, agg::color_conv_rgba32_to_argb32()); break;
                case agg::pix_format_argb32: color_conv(&rbuf_tmp, src, agg::color_conv_argb32_to_argb32()); break;
                case agg::pix_format_abgr32: color_conv(&rbuf_tmp, src, agg::color_conv_abgr32_to_argb32()); break;
                case agg::pix_format_bgra32: color_conv(&rbuf_tmp, src, agg::color_conv_bgra32_to_argb32()); break;
            }
            break;

        case agg::pix_format_bgra32:
            switch(format)
            {
                default: break;
                case agg::pix_format_rgb555: color_conv(&rbuf_tmp, src, agg::color_conv_rgb555_to_bgra32()); break;
                case agg::pix_format_rgb565: color_conv(&rbuf_tmp, src, agg::color_conv_rgb565_to_bgra32()); break;
                case agg::pix_format_rgb24:  color_conv(&rbuf_tmp, src, agg::color_conv_rgb24_to_bgra32());  break;
                case agg::pix_format_bgr24:  color_conv(&rbuf_tmp, src, agg::color_conv_bgr24_to_bgra32());  break;
                case agg::pix_format_rgba32: color_conv(&rbuf_tmp, src, agg::color_conv_rgba32_to_bgra32()); break;
                case agg::pix_format_argb32: color_conv(&rbuf_tmp, src, agg::color_conv_argb32_to_bgra32()); break;
                case agg::pix_format_abgr32: color_conv(&rbuf_tmp, src, agg::color_conv_abgr32_to_bgra32()); break;
                case agg::pix_format_bgra32: color_conv(&rbuf_tmp, src, agg::color_conv_bgra32_to_bgra32()); break;
            }
            break;
    }

    ximg->data = (char*)buf_tmp;
    ximg->width = src->width(),
    ximg->height = src->height(),
    XPutImage(env, hwnd, gc, ximg, 0, 0, 0, 0, src->width(), src->height());

    delete [] buf_tmp;
  }

}
