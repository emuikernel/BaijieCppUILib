
// structures that represent "handled" objects.

#if !defined(__objects_h__)
#define __objects_h__

#ifdef _WINDOWS
  #include "platform/win32/agg_win32_bmp.h"
#else
  #include "platform/X11/agg_pixel_map.h"
#endif

#include "agg2D.h"
#include "primitives.h"

struct image:
  public resource,
  public Agg2D::Image
{
    agg::pixel_map pmap;

    image(unsigned int width, unsigned int height, unsigned init = 256)
    {
      pmap.create(width, height, agg::org_color32, init);
      attach(pmap.buf(),width,height,pmap.stride());
    }
    ~image()
    {

    }
};

struct graphics:
  public resource,
  public Agg2D
{
  struct state: Agg2D::State
  {
    state* next;
    state( state* n = 0): next(n) {}
    ~state() { delete next; }
  };

  //handle<image> img;
  state*        saved_states;

  graphics(image* p): saved_states(0)
  {
    if(!p)
      return;
    Agg2D::attach(p->pmap.buf(), p->pmap.width(), p->pmap.height(), p->pmap.stride());
    //Agg2D::clearAll(0xff, 0xff, 0xff);
    Agg2D::antiAliasGamma(1.4);
    Agg2D::blendMode(Agg2D::BlendAlpha);
    Agg2D::imageFilter(Agg2D::Bilinear);

    /*Agg2D::viewport(0, 0, p->width(), p->height(),
                    0, p->height(), p->width(), 0,
                    Agg2D::Anisotropic);*/

    Agg2D::flipText(true);
    Agg2D::noFill();
    Agg2D::lineWidth(1);
  }

  ~graphics()
  {
    delete saved_states;
  }

  void save_state()
  {
    saved_states = new state(saved_states);
    saveStateTo(*saved_states);
  }
  bool restore_state()
  {
    if(!saved_states)
      return false;
    restoreStateFrom(*saved_states);
    state *t = saved_states;
    saved_states = saved_states->next;
    t->next = 0;
    delete t;
    return true;
  }
};

struct paint_graphics: graphics
{
  paint_graphics(image* p, int clip_x1, int clip_y1, int clip_x2, int clip_y2): graphics(p)
  {
    clipBox( clip_x1, clip_y1, clip_x2, clip_y2 );
    clearClipBox(255,255,255);
  }
  virtual long release() // we create paint_graphics on stack
  {
      assert(_ref_cntr > 0);
      return --_ref_cntr;
  }
};


#endif //!defined(__common_h__)
