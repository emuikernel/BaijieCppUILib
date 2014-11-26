#include "window.h"
#include "primitives.h"
#include "xwnd.hpp"
#include "xapp.hpp"

const wchar_t* WINDOW_CLASS_NAME = L"Graphinius";

struct window : xlib::window
{
    window_function*  pf;
    void*             tag;

    window(): pf(0), tag(0)  {}

    bool notify( unsigned cmd, void *params ) {  return pf( this, cmd, params ); }

    virtual void created() { notify(WINDOW_ON_CREATE, 0); }
    virtual void destroyed() { notify(WINDOW_ON_DESTROY, 0); }
    virtual void position_changed()
      {
        WINDOW_ON_SIZE_PARAMS p;
        xlib::rect rc = get_place();
        p.width = xlib::width(rc);
        p.height = xlib::height(rc);
        notify(WINDOW_ON_SIZE,&p);
        exposed();
      }

    virtual void shown() {  }
    virtual void hidden() {  }

    virtual void on_left_button_down ( int x, int y ) {}
    virtual void on_right_button_down ( int x, int y ) {}

    virtual void on_left_button_up ( int x, int y ) {}
    virtual void on_right_button_up ( int x, int y ) {}

    virtual void on_mouse_enter ( int x, int y ) {}
    virtual void on_mouse_exit ( int x, int y ) {}
    virtual void on_mouse_move ( int x, int y ) {}

    virtual void on_got_focus() {}
    virtual void on_lost_focus() {}

    virtual void on_key_press ( xlib::character c ) {}
    virtual void on_key_release ( xlib::character c ) {}

    virtual void delegate_paint(agg::rendering_buffer& rbuf)
    {
      if(dirty_area.is_valid())
      {
        WINDOW_ON_PAINT_PARAMS p;
        paint_graphics ps(buffer, dirty_area.x1, dirty_area.y1, dirty_area.x2, dirty_area.y2 );
        p.clip_x = dirty_area.x1;
        p.clip_y = dirty_area.y1;
        p.clip_w = dirty_area.x2 - dirty_area.x1;
        p.clip_h = dirty_area.y2 - dirty_area.y1;
        p.view_w = buffer->width();
        p.view_h = buffer->height();
        p.surface = &ps;
        notify(WINDOW_ON_PAINT, &p);
        dirty_area = agg::rect_i(1,1,0,0);
      }
    }

};

  /*inline int get_buttons(WPARAM wp) {
    int btns = 0;
    if(wp & MK_LBUTTON)  btns |= MAIN_BUTTON;
    if(wp & MK_RBUTTON)  btns |= PROP_BUTTON;
    return btns;
  }

  inline int get_alts()
  {
    int alts = 0;
    if (GetAsyncKeyState(VK_SHIFT) < 0) alts |= SHIFT;
    if (GetAsyncKeyState(VK_CONTROL) < 0) alts |= CONTROL;
    if (GetAsyncKeyState(VK_MENU) < 0) alts |= ALT;
    return alts;
  }

  inline int get_alts(WPARAM wp)
  {
    int alts = 0;
    if (wp & MK_SHIFT) alts |= SHIFT;
    if (wp & MK_CONTROL) alts |= CONTROL;
    return alts;
  }

  inline int get_buttons_async()
  {
    return (GetAsyncKeyState(VK_LBUTTON) < 0)?MAIN_BUTTON:0;
  }

  static LRESULT CALLBACK wnd_proc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

  static LRESULT CALLBACK wnd_proc_primordial( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    if(msg == WM_NCCREATE)
    {
      CREATESTRUCT* lpcs = (CREATESTRUCT*)lParam;
      window* pw = (window*)lpcs->lpCreateParams;
      pw->hwnd = hwnd;
      SetWindowLongPtr(hwnd, GWLP_USERDATA, LONG_PTR(pw));
      SetWindowLongPtr(hwnd, GWLP_WNDPROC,  LONG_PTR(&wnd_proc));
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  */

  /*

*/


/*

  static LRESULT CALLBACK wnd_proc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    window* pw = (window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if(!pw)
      return DefWindowProc(hwnd, msg, wParam, lParam);

    switch(msg)
    {
      case WM_CREATE:
        pw->notify(WINDOW_ON_CREATE, 0);
        break;
      case WM_DESTROY:
        pw->notify(WINDOW_ON_DESTROY, 0);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
        delete pw;
        break;
      case WM_SIZE:
        if(::IsWindow(hwnd))
        {
          RECT rc; GetClientRect(hwnd, &rc);
          WINDOW_ON_SIZE_PARAMS p;
          p.width = rc.right - rc.left;
          p.height = rc.bottom - rc.top;
          if(pw->notify(WINDOW_ON_SIZE, &p))
          {
            //todo: MoveWindow here as client changed requested dimensions
          }
        }
        break;
      case WM_MOVE:
        if(::IsWindow(hwnd))
        {
          RECT rc; GetClientRect(hwnd, &rc);
          ::MapWindowPoints(hwnd,HWND_DESKTOP,(LPPOINT)&rc, 1);
          WINDOW_ON_MOVE_PARAMS p;
          p.x = rc.left;
          p.y = rc.top;
          if(pw->notify(WINDOW_ON_SIZE, &p))
          {
            //todo: MoveWindow here as client changed requested position
          }
        } break;
      case WM_ACTIVATE:
        {
          WINDOW_ON_ACTIVATE_PARAMS p;
          p.manner = LOWORD(wParam) != WA_INACTIVE;
          pw->notify(WINDOW_ON_ACTIVATE, &p);
        } break;
      case WM_IDLE:
        pw->notify(WINDOW_ON_IDLE, 0);
        break;
      case WM_CLOSE:
        pw->notify(WINDOW_ON_CLOSE, 0);
        break;

      case WM_TIMER:
        if( wParam )
        {
          WINDOW_ON_TIMER_PARAMS p;
          p.id = wParam;
          if(!pw->notify(WINDOW_ON_TIMER, &p))
            ::KillTimer(hwnd, wParam);
        }
        else // mouse tick timer
        {
          WINDOW_ON_MOUSE_PARAMS p;
          p.event = MOUSE_TICK;
          p.buttons = MAIN_BUTTON;
          POINT pt;
          ::GetCursorPos(&pt);
          p.screen_x = pt.x;
          p.screen_y = pt.y;
          ::MapWindowPoints(HWND_DESKTOP,hwnd,&pt,1);
          p.x = pt.x;
          p.y = pt.y;
          p.alts = get_alts();
          if(pw->notify(WINDOW_ON_MOUSE, &p))
            return 0;
          break;
        }
        return 0;
      case WM_ERASEBKGND:
        return TRUE;

      case WM_PAINT:
        {
          PAINTSTRUCT ps;
          HDC hdc = BeginPaint(hwnd, &ps);
          RECT ri; ::GetClipBox(hdc,&ri);
          w_paint(pw, hwnd, hdc, ri);
          EndPaint(hwnd, &ps);
          return 0;
        }
      case WM_PRINTCLIENT:
        {
          HDC hdc = (HDC) wParam;
          RECT ri; ::GetClipBox(hdc,&ri);
          w_paint(pw, hwnd, hdc, ri);
          return 0;
        }
      case WM_LBUTTONDOWN:
      {
          WINDOW_ON_MOUSE_PARAMS p;
          p.event = MOUSE_DOWN;
          p.buttons = MAIN_BUTTON;
          p.x = GET_X_LPARAM(lParam);
          p.y = GET_Y_LPARAM(lParam);
          p.screen_x = p.x;
          p.screen_y = p.y;
          ::MapWindowPoints(hwnd,HWND_DESKTOP,(LPPOINT)&p.screen_x, 1);
          p.alts = get_alts(wParam);

          ::SetTimer(hwnd,0,::GetDoubleClickTime()/4,0); // start MOUSE_TICK timer
          ::SetCapture(hwnd);

          if(pw->notify(WINDOW_ON_MOUSE, &p))
            return 0;
          break;
      }
      case WM_RBUTTONDOWN:
      {
          WINDOW_ON_MOUSE_PARAMS p;
          p.event = MOUSE_DOWN;
          p.buttons = PROP_BUTTON;
          p.x = GET_X_LPARAM(lParam);
          p.y = GET_Y_LPARAM(lParam);
          p.screen_x = p.x;
          p.screen_y = p.y;
          ::MapWindowPoints(hwnd,HWND_DESKTOP,(LPPOINT)&p.screen_x, 1);
          p.alts = get_alts(wParam);
          if(pw->notify(WINDOW_ON_MOUSE, &p))
            return 0;
          break;
      }
      case WM_LBUTTONUP:
      {
          ::KillTimer(hwnd,0); // stop MOUSE_TICK timer
          if(::GetCapture() == hwnd)
            ::ReleaseCapture();

          WINDOW_ON_MOUSE_PARAMS p;
          p.event = MOUSE_UP;
          p.buttons = MAIN_BUTTON;
          p.x = GET_X_LPARAM(lParam);
          p.y = GET_Y_LPARAM(lParam);
          p.screen_x = p.x;
          p.screen_y = p.y;
          ::MapWindowPoints(hwnd,HWND_DESKTOP,(LPPOINT)&p.screen_x, 1);
          p.alts = get_alts(wParam);
          if(pw->notify(WINDOW_ON_MOUSE, &p))
            return 0;
          break;
      }
      case WM_RBUTTONUP:
      {
          WINDOW_ON_MOUSE_PARAMS p;
          p.event = MOUSE_UP;
          p.buttons = PROP_BUTTON;
          p.x = GET_X_LPARAM(lParam);
          p.y = GET_Y_LPARAM(lParam);
          p.screen_x = p.x;
          p.screen_y = p.y;
          ::MapWindowPoints(hwnd,HWND_DESKTOP,(LPPOINT)&p.screen_x, 1);
          p.alts = get_alts(wParam);
          if(pw->notify(WINDOW_ON_MOUSE, &p))
            return 0;
          break;
      }
      case WM_MOUSEMOVE:
      {
          WINDOW_ON_MOUSE_PARAMS p;
          p.event = MOUSE_MOVE;
          p.buttons = get_buttons(wParam);
          p.x = GET_X_LPARAM(lParam);
          p.y = GET_Y_LPARAM(lParam);
          p.screen_x = p.x;
          p.screen_y = p.y;
          ::MapWindowPoints(hwnd,HWND_DESKTOP,(LPPOINT)&p.screen_x, 1);
          p.alts = get_alts(wParam);
          if( !pw->mouse_inside )
          {
            pw->mouse_inside = true;
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(tme);
            tme.dwFlags = TME_HOVER | TME_LEAVE;
            tme.hwndTrack = hwnd;
            tme.dwHoverTime = HOVER_DEFAULT;
            _TrackMouseEvent(&tme);
            WINDOW_ON_MOUSE_PARAMS t = p;
            t.event = MOUSE_ENTER;
            pw->notify(WINDOW_ON_MOUSE, &t);
          }
          if(pw->notify(WINDOW_ON_MOUSE, &p))
            return 0;
          break;
      }
      case WM_MOUSELEAVE:
      {
          WINDOW_ON_MOUSE_PARAMS p;
          p.event = MOUSE_LEAVE;
          p.buttons = get_buttons(wParam);
          p.x = GET_X_LPARAM(lParam);
          p.y = GET_Y_LPARAM(lParam);
          p.screen_x = p.x;
          p.screen_y = p.y;
          ::MapWindowPoints(hwnd,HWND_DESKTOP,(LPPOINT)&p.screen_x, 1);
          p.alts = get_alts(wParam);
          pw->mouse_inside = false;

          TRACKMOUSEEVENT tme;
          tme.cbSize = sizeof(tme);
          tme.dwFlags = TME_CANCEL;
          tme.hwndTrack = hwnd;
          tme.dwHoverTime = HOVER_DEFAULT;
          _TrackMouseEvent(&tme);

          if(pw->notify(WINDOW_ON_MOUSE, &p))
            return 0;
          break;
      }

      case WM_MOUSEHOVER:
      {
          WINDOW_ON_MOUSE_PARAMS p;
          p.event = MOUSE_IDLE;
          p.buttons = get_buttons(wParam);
          p.x = GET_X_LPARAM(lParam);
          p.y = GET_Y_LPARAM(lParam);
          p.screen_x = p.x;
          p.screen_y = p.y;
          ::MapWindowPoints(hwnd,HWND_DESKTOP,(LPPOINT)&p.screen_x, 1);
          p.alts = get_alts(wParam);

          TRACKMOUSEEVENT tme;
          tme.cbSize = sizeof(tme);
          tme.dwFlags = TME_HOVER | TME_LEAVE;
          tme.hwndTrack = hwnd;
          tme.dwHoverTime = HOVER_DEFAULT;
          _TrackMouseEvent(&tme);

          if(pw->notify(WINDOW_ON_MOUSE, &p))
            return 0;
          break;
      }
      case WM_KEYDOWN:
      {
          WINDOW_ON_KEY_PARAMS p;
          p.event = KEY_DOWN;
          p.code = wParam;
          p.alts = get_alts();
          if(pw->notify(WINDOW_ON_KEY, &p))
            return 0;
          break;
      }
      case WM_KEYUP:
      {
          WINDOW_ON_KEY_PARAMS p;
          p.event = KEY_UP;
          p.code = wParam;
          p.alts = get_alts();
          if(pw->notify(WINDOW_ON_KEY, &p))
            return 0;
          break;
      }
      case WM_CHAR:
      {
          WINDOW_ON_KEY_PARAMS p;
          p.event = KEY_CHAR;
          p.code = wParam;
          p.alts = get_alts();
          if(pw->notify(WINDOW_ON_KEY, &p))
            return 0;
          break;
      }

    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
*/

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_create(window_function* pf, void* tag, WINDOW_TYPE type, HWINDOW* out_hwnd)
{
    if(!pf || !out_hwnd)
      return GRAPHIN_BAD_PARAM;

    //unsigned wflags = 0;
    //unsigned wflagsex = 0;
    switch( type )
    {
    case WINDOW_TYPE_POPUP:  // no border, no caption
      //wflags = WS_POPUP;
      break;
    case WINDOW_TYPE_FRAME:  // caption and resizeable frame
      //wflags = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX;
      //wflagsex = WS_EX_OVERLAPPEDWINDOW;
      break;
    case WINDOW_TYPE_TOOL:   // narrow caption and resizeable frame
      //wflags = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX;
      //wflagsex = WS_EX_TOOLWINDOW;
      break;
    case WINDOW_TYPE_DIALOG: // caption and not resizeable frame
      //wflags = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
      //wflagsex = WS_EX_DLGMODALFRAME;
      break;
    }
    window* pw = new window();
    if( !pw )
      return GRAPHIN_FAILURE;
    pw->pf = pf;
    //pw->buffer = new image(1,1);
    pw->tag = tag;
    pw->create();

    *out_hwnd = pw;
    return GRAPHIN_OK;
}

GRAPHIN_API bool GRAPHIN_CALL
        window_is_valid(HWINDOW hw)
{
  return hw && hw->is_valid();
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_destroy(HWINDOW hw)
{
  if( window_is_valid(hw) )
  {
    hw->destroy();
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_move(HWINDOW hw, int x, int y, int w, int h)
{
  if( window_is_valid(hw) )
  {
    //::MoveWindow(hw->hwnd, x, y, w, h, TRUE);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_set_state(HWINDOW hw, WINDOW_STATE cmd)
{
  if( window_is_valid(hw) )
  {
    switch(cmd)
    {
      case WINDOW_SHOWN:
        hw->show();
        break;
      case WINDOW_EXPANDED:
        hw->expand();
        break;
      case WINDOW_COLLAPSED:
        hw->collapse();
        break;
      case WINDOW_HIDDEN:
        hw->hide();
        break;
    }
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_get_state(HWINDOW hw, WINDOW_STATE* pst)
{
   if( window_is_valid(hw) )
   {
     if(pst)
       *pst = hw->get_state();
     return GRAPHIN_OK;
   }
   return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_request_close(HWINDOW hw)
{
  if( window_is_valid(hw) )
  {
    //::PostMessage(hw->hwnd, WM_CLOSE, 0, 0);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_request_idle(HWINDOW hw)
{
  if( window_is_valid(hw) )
  {
    //::PostMessage(hw->hwnd, WM_IDLE, 0, 0);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_set_timer(HWINDOW hw, unsigned ms, unsigned long id)
{
  if( window_is_valid(hw) )
  {
    //if(ms)
    //  ::SetTimer(hw->hwnd, id, ms, 0);
    //else
    //  ::KillTimer(hw->hwnd, id);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_set_caption(HWINDOW hw, const wchar_t* text)
{
  if( window_is_valid(hw) )
  {
    //::SetWindowTextW(hw->hwnd,text);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_get_caption(HWINDOW hw, wchar_t* buf, unsigned buf_size)
{
  if( window_is_valid(hw) )
  {
    //::GetWindowTextW(hw->hwnd,buf,buf_size);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_get_tag(HWINDOW hw, void** ptag)
{
  if( !ptag )
    return GRAPHIN_BAD_PARAM;
  if( window_is_valid(hw) )
  {
    *ptag = hw->tag;
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_set_tag(HWINDOW hw, void* tag)
{
  if( window_is_valid(hw) )
  {
    hw->tag = tag;
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

// invalidates rect - set dirty area
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_invalidate(HWINDOW hw, int x, int y, int w, int h)
{
  if( window_is_valid(hw) )
  {
    if(w > 0 && h > 0)
    {
      /*agg::rect_i r( x, y, x + w, y + h );
      hw->dirty_area = hw->dirty_area.is_valid() ? agg::unite_rectangles(hw->dirty_area, r ):r;
      RECT rw;
      rw.left   = hw->dirty_area.x1;
      rw.right  = hw->dirty_area.x2;
      rw.top    = hw->dirty_area.y1;
      rw.bottom = hw->dirty_area.y2;
      ::InvalidateRect(hw->hwnd,&rw,FALSE);*/
    }
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

// updates the window
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_update(HWINDOW hw)
{
  if( window_is_valid(hw) )
  {
    hw->exposed();
    //::UpdateWindow(hw->hwnd);
    return GRAPHIN_OK;
  }
  return GRAPHIN_FAILURE;
}

