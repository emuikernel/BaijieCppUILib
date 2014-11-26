#ifndef __graphinius_window_h__
#define __graphinius_window_h__

#include "graphin.h"

// SECTION: window functions

struct  window;
typedef window* HWINDOW;

typedef bool GRAPHIN_CALL window_function(HWINDOW hw, unsigned message, void* params);

enum WINDOW_MESSAGE
{
  WINDOW_ON_CREATE,
  WINDOW_ON_DESTROY,
  WINDOW_ON_ACTIVATE,
  WINDOW_ON_SIZE,
  WINDOW_ON_MOVE,
  WINDOW_ON_MOUSE,
  WINDOW_ON_KEY,
  WINDOW_ON_TIMER,
  WINDOW_ON_IDLE,
  WINDOW_ON_CLOSE,
  WINDOW_ON_PAINT,
};

struct WINDOW_ON_PAINT_PARAMS     { HGFX surface;  int  clip_x;  int  clip_y;  int  clip_w;  int  clip_h; int view_w; int view_h; };
struct WINDOW_ON_SIZE_PARAMS      { int width;  int height; };
struct WINDOW_ON_MOVE_PARAMS      { int x; int y; };

struct WINDOW_ON_ACTIVATE_PARAMS  { int manner; };
struct WINDOW_ON_TIMER_PARAMS     { unsigned long id; bool cancel; };

enum ALT_FLAGS
{
  CONTROL = 1,
  SHIFT = 2,
  ALT = 4,
};

enum KEY_EVENTS
{
  KEY_DOWN = 0,
  KEY_UP,
  KEY_CHAR
};

struct WINDOW_ON_KEY_PARAMS
{
  KEY_EVENTS   event;
  unsigned int code; // scan code (see keycodes.h) or unicode code point of character in
  unsigned int alts; // ALTS above
};

enum MOUSE_EVENTS
{
  MOUSE_ENTER = 0,
  MOUSE_LEAVE,
  MOUSE_MOVE,
  MOUSE_UP,
  MOUSE_DOWN,
  MOUSE_DCLICK,
  MOUSE_WHEEL,
  MOUSE_TICK, // mouse pressed ticks
  MOUSE_IDLE, // mouse stay idle for some time
};

enum MOUSE_BUTTONS
{
  MAIN_BUTTON = 0x01, //aka left button
  PROP_BUTTON = 0x02, //aka right button
  MIDDLE_BUTTON = 0x04,
};

struct WINDOW_ON_MOUSE_PARAMS
{
  MOUSE_EVENTS event;
  unsigned int buttons; // buttons or mouse wheel delta ticks
  unsigned int alts;    // ALTS above
  int x;
  int y;
  int screen_x;
  int screen_y;
};

enum WINDOW_TYPE
{
  WINDOW_TYPE_POPUP,  // no border, no caption
  WINDOW_TYPE_FRAME,  // caption and resizeable frame
  WINDOW_TYPE_TOOL,   // narrow caption and resizeable frame
  WINDOW_TYPE_DIALOG, // caption and not resizeable frame
};

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_create(window_function* pf, void* tag, WINDOW_TYPE type, HWINDOW* out_hwnd);

GRAPHIN_API bool GRAPHIN_CALL
        window_is_valid(HWINDOW hwnd);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_get_tag(HWINDOW hw, void** ptag);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_set_tag(HWINDOW hw, void* tag);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_destroy(HWINDOW hwnd);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_move(HWINDOW hwnd, int x, int y, int w, int h);

enum WINDOW_STATE
{
  WINDOW_HIDDEN,     // hidden
  WINDOW_SHOWN,      // shown
  WINDOW_COLLAPSED,  // minimized, iconified
  WINDOW_EXPANDED,   // maximized, but not full screen.
};

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_set_state(HWINDOW hwnd, WINDOW_STATE st);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_get_state(HWINDOW hwnd, WINDOW_STATE* pst);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_request_close(HWINDOW hwnd);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_request_idle(HWINDOW hwnd);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_set_timer(HWINDOW hwnd, unsigned ms, unsigned long id);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_set_caption(HWINDOW hwnd, const wchar_t* text);

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_get_caption(HWINDOW hwnd, wchar_t* buf, unsigned buf_size);

// invalidates rect - set dirty area
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_invalidate(HWINDOW hwnd, int x, int y, int w, int h);

// updates the window
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        window_update(HWINDOW hwnd);


// SECTION: GUI application functions

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        application_init(void* hinstance, const char* command_line);

// fetch and dispatch single event from event queue.
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        application_do_event(int* retval);

// request exit
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        application_request_exit(int retval);


#endif
