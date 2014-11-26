#ifndef __graphinius_window_hpp__
#define __graphinius_window_hpp__

#include "window.h"

namespace graphin
{
  namespace app
  {
    bool init(void* hinstance, const char* command_line);
    bool do_event(int& retval);
    void request_exit(int retval);
    int  run();
  }

  class window // top level window frame
  {
    HWINDOW hwnd;
    static bool GRAPHIN_CALL proc(HWINDOW hw, unsigned message, void* params);

    window(): hwnd(0) {}
  public:
  
    typedef WINDOW_ON_MOUSE_PARAMS MOUSE_EVT;
    typedef WINDOW_ON_KEY_PARAMS   KEY_EVT;
    typedef WINDOW_ON_PAINT_PARAMS PAINT_EVT;
    typedef WINDOW_ON_SIZE_PARAMS  SIZE_EVT;
    typedef WINDOW_ON_MOVE_PARAMS  MOVE_EVT;
    typedef WINDOW_ON_TIMER_PARAMS TIMER_EVT;
    typedef WINDOW_STATE           STATE;
  
    window(WINDOW_TYPE type);
    ~window() { destroy(); }

    bool is_valid();
    
    void destroy();
    
    void move(int x, int y, int w, int h);
    STATE get_state();
    void set_state(STATE st);
    
    void request_close();
    void request_idle();
    
    void set_timer(unsigned ms, unsigned long timer_id);
    void set_caption(const wchar_t* text);
    void get_caption(wchar_t* buffer, unsigned buffer_length);
    
    void invalidate(int x, int y, int w, int h);
    void update();
    
    // overridables
    virtual void on_create() {}
    virtual void on_destroy() {}
    virtual void on_activate(bool active) {}
    virtual void on_size(SIZE_EVT& evt) {}
    virtual void on_move(MOVE_EVT& evt) {}
    virtual void on_mouse(MOUSE_EVT& evt) {}
    virtual void on_key(KEY_EVT& evt) {}
    virtual void on_timer(TIMER_EVT& evt) {}
    virtual void on_idle() {}
    virtual void on_close() { destroy(); }
    virtual void on_paint(PAINT_EVT& evt) {}
  };
  
  inline window::window(WINDOW_TYPE type)
  {
    window_create(&proc, this, type, &hwnd);
  }
   
  inline bool window::is_valid()
  {
    return GRAPHIN_OK == window_is_valid( hwnd );
  }

  inline void window::destroy()
  { 
    if(is_valid())
    {
      window_destroy(hwnd);
      hwnd = 0;
    }
  }

  inline void window::move(int x, int y, int w, int h)
  {
    window_move( hwnd,x, y, w, h );
  }

  inline void window::request_close()
  {
    window_request_close(hwnd);
  }

  inline void window::request_idle()
  {
    window_request_idle(hwnd);
  }

  inline void window::set_state(window::STATE st)
  {
    window_set_state(hwnd, st);
  }

  inline window::STATE window::get_state()
  {
    WINDOW_STATE st;
    window_get_state(hwnd, &st);
    return st;
  }

  inline void window::set_timer(unsigned ms, unsigned long id)
  {
    window_set_timer(hwnd,ms,id);
  }

  inline void window::set_caption(const wchar_t* text)
  {
    window_set_caption(hwnd, text);
  }

  inline void window::get_caption(wchar_t* buffer, unsigned buffer_length)
  {
    window_get_caption(hwnd, buffer, buffer_length);
  }

  inline void window::invalidate(int x, int y, int w, int h)
  {
    window_invalidate(hwnd, x, y, w, h);
  }

  inline void window::update()
  {
    window_update(hwnd);
  }

  inline bool app::init(void* hinstance, const char* command_line)
  {
    return GRAPHIN_OK == application_init(hinstance, command_line);
  }
  
  inline bool app::do_event(int& retval)
  {
    return GRAPHIN_OK == application_do_event(&retval);
  }

  inline void app::request_exit(int retval)
  {
    application_request_exit(retval);
  }
  
  inline int app::run()
  {
    int retval = 0;
    while( do_event(retval) );
    return retval;
  }

  inline bool GRAPHIN_CALL window::proc(HWINDOW hw, unsigned message, void* params)
  {
    void* pw; window_get_tag(hw,&pw);
    window* self = static_cast< window* >( pw );
  
    switch( message )
    {
      case WINDOW_ON_CREATE:
      {
        self->on_create();
        return true;
      }
      case WINDOW_ON_DESTROY:
      {
        self->on_destroy();
        self->hwnd = 0;
        return true;
      }
      case WINDOW_ON_ACTIVATE:
      {
        WINDOW_ON_ACTIVATE_PARAMS* p = (WINDOW_ON_ACTIVATE_PARAMS*)params;
        self->on_activate(p->manner != 0);
        return true;
      }
      case WINDOW_ON_SIZE:
      {
        WINDOW_ON_SIZE_PARAMS* p = (WINDOW_ON_SIZE_PARAMS*)params;
        self->on_size(*p);
        return true;
      }
      case WINDOW_ON_MOVE:
      {
        WINDOW_ON_MOVE_PARAMS* p = (WINDOW_ON_MOVE_PARAMS*)params;
        self->on_move(*p);
        return true;
      }
      case WINDOW_ON_TIMER:
      {
        WINDOW_ON_TIMER_PARAMS* p = (WINDOW_ON_TIMER_PARAMS*)params;
        self->on_timer(*p);
        return true;
      }
      case WINDOW_ON_IDLE:
      {
        self->on_idle();
        return true;
      }
      case WINDOW_ON_PAINT:
      {
        WINDOW_ON_PAINT_PARAMS *prm = (WINDOW_ON_PAINT_PARAMS *)params;
        self->on_paint(*prm);
        return true;
      }
      case WINDOW_ON_MOUSE:
      {
        WINDOW_ON_MOUSE_PARAMS *prm = (WINDOW_ON_MOUSE_PARAMS *)params;
        self->on_mouse(*prm);
        return true;
      }
      case WINDOW_ON_KEY:
      {
        WINDOW_ON_KEY_PARAMS *prm = (WINDOW_ON_KEY_PARAMS *)params;
        self->on_key(*prm);
        return true;
      }
      case WINDOW_ON_CLOSE:
      {
        self->on_close();
        return true;
      }
    }
    return false;
  }
 
}

#endif
