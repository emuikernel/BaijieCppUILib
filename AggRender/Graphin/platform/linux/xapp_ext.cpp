#include <iostream>
using namespace std;

#include "window.h"
#include "xapp.hpp"

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        application_init(void* hinstance, const char* command_line)
{
  static bool done = false;
  if( !done )
  {
    done = true;
    if(xlib::app::init())
      xlib::app::start();
  }
  return xlib::app::is_valid()? GRAPHIN_OK : GRAPHIN_FAILURE;
}

GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        application_do_event(int* retval)
{
  int rv;
  if(xlib::app::step(rv))
  {
    return GRAPHIN_OK;
  }
  else
  {
    if(retval)
      *retval = rv;
    return GRAPHIN_OK_FALSE;
  }
}

// request exit
GRAPHIN_API GRAPHIN_RESULT GRAPHIN_CALL
        application_request_exit(int retval)
{
  xlib::app::stop(retval);
  return GRAPHIN_OK;
}


