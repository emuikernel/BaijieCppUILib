// CustomRasterizerView.cpp : implementation of the CCustomRasterizerView class
//

#include "stdafx.h"
#include <algorithm>
#include "CustomRasterizer.h"

#include "CustomRasterizerDoc.h"
#include "CustomRasterizerView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "agg_trans_affine.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_color_rgba.h"
#include "agg_bounding_rect.h"
#include "ctrl/agg_slider_ctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCustomRasterizerView

IMPLEMENT_DYNCREATE(CCustomRasterizerView, CView)

BEGIN_MESSAGE_MAP(CCustomRasterizerView, CView)
	//{{AFX_MSG_MAP(CCustomRasterizerView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



//============================================================================
// Demonstration of the principle of creating custom rasterizer.
// Strictly speaking it's not a rasterizer because it can use any HDC, 
// including non-raster ones (windows metafile or something)
// This class doesn't use anything but WinGDI function PolyPolygon.
// 
// This is only a demonstration and a proof of concept!
//
// Do NOT expect great quality from it because it depends on the 
// abilities of the PolyPolygon only. So, forget about any anti-aliasing
// and subpixel accuracy.
//
// The proof of concept is that you can use any part of the library. This
// example doesn't use the nice AGG rasterization algorithm, in fact, 
// it doesn't use anything but the coordinate conversion pipeline.
// The example demonstrates how the library can be adapted to the use
// a write-only graphic device with very poor drawing abilities.
//
// The example also demonstrates the use of the AGG controls. The control 
// in this example changes the saturation of the image and it's also being 
// rendered with this very class. 
//
// At the end of this file you can find the code that makes the control "alive"
// i.e. just processing the mouse events.
// 
//----------------------------------------------------------------------------
class hdc_rasterizer
{
    // Since it's only a demonstratoion, the capacity of the point container
    // is restricted. There're no reallocs, but if you exceed this number
    // it won't crash. The maximal number of polygons doesn't mean that
    // you can draw no more than this number of polygons in total.
    // This value restricts only the number of polygons in a single
    // PolyPolygon.
    enum max_e
    {
        max_points   = 4094,
        max_polygons = 256
    };

public:
    ~hdc_rasterizer()
    {
        ::SelectObject(m_dc, m_old_pen);
        ::DeleteObject(m_null_pen);
        ::SelectObject(m_dc, m_old_brush);
        if(m_brush)
        {
            ::DeleteObject(m_brush);
        }
        delete [] m_counts;
        delete [] m_points;
    }


    hdc_rasterizer(HDC dc, agg::rgba8 c) :
        m_dc(dc),
        m_sat(255),
        m_brush(0),
        m_old_brush(0),
        m_null_pen(0),
        m_old_pen(0),
        m_points(new POINT[max_points]),
        m_counts(new int[max_polygons]),
        m_num_poly(0),
        m_num_points(0),
        m_reset_flag(false)
    {
        HBRUSH brush = ::CreateSolidBrush(0);
        m_old_brush = (HBRUSH)::SelectObject(m_dc, brush);
        ::SelectObject(m_dc, m_old_brush);
        ::DeleteObject(brush);
        m_null_pen = ::CreatePen(PS_NULL, 1, 0);
        m_old_pen = (HPEN)::SelectObject(m_dc, m_null_pen);
        ::SetPolyFillMode(m_dc, WINDING);
    }

    //------------------------------------------------------------------------
    void color(agg::rgba8 c);
    void saturation(unsigned sat) { m_sat = sat; }

    //------------------------------------------------------------------------
    void reset();
    void move_to(int x, int y);
    void line_to(int x, int y); 
    void render();

    //------------------------------------------------------------------------
    template<class VertexSource>
    void add_path(VertexSource& vs, unsigned path_id=0)
    {
        double x;
        double y;

        unsigned flag;
        vs.rewind(path_id);
        while(!agg::is_stop(flag = vs.vertex(&x, &y)))
        {
            if(agg::is_vertex(flag))
            {
                if(agg::is_move_to(flag)) 
                {
                    move_to(int(x), int(y));
                }
                else 
                {
                    line_to(int(x), int(y));
                }
            }
        }
    }

    //------------------------------------------------------------------------
    template<class VertexSource, class ColorStorage, class PathId>
    void render_all_paths(VertexSource& vs, 
                          const ColorStorage& colors, 
                          const PathId& path_id,
                          unsigned num_paths)
    {
        for(unsigned i = 0; i < num_paths; i++)
        {
            reset();
            add_path(vs, path_id[i]);
            color(colors[i]);
            render();
        }
    }

    //------------------------------------------------------------------------
    template<class Ctrl> void render_ctrl(Ctrl& c)
    {
        unsigned i;
        for(i = 0; i < c.num_paths(); i++)
        {
            reset();
            add_path(c, i);
            color(c.color(i));
            render();
        }
    }

private:
    HDC       m_dc;
    unsigned  m_sat;
    HBRUSH    m_brush;
    HBRUSH    m_old_brush;
    HPEN      m_null_pen;
    HPEN      m_old_pen;
    POINT*    m_points;
    int*      m_counts;
    int       m_num_poly;
    int       m_num_points;
    bool      m_reset_flag;
};




//----------------------------------------------------------------------------
// The real drawing color is calculated considering the additional
// parameter "saturation". The saturation is also not a part of AGG, it's
// used just to demonstrate that the renderer can have anything you want
// and to make the slider control have some effect :-)
void hdc_rasterizer::color(agg::rgba8 c)
{
    int gray8 = c.r * 77 + c.g * 150 + c.b * 29;
    int gray = gray8 >> 8;
    int r = (((int(c.r) - gray) * m_sat) + gray8) >> 8;
    int g = (((int(c.g) - gray) * m_sat) + gray8) >> 8;
    int b = (((int(c.b) - gray) * m_sat) + gray8) >> 8;

    HBRUSH brush = ::CreateSolidBrush(RGB(r, g, b));
    ::SelectObject(m_dc, brush);
    if(m_brush)
    {
        ::DeleteObject(m_brush);
    }
    m_brush = brush;
}


//------------------------------------------------------------------------
void hdc_rasterizer::reset()
{
    m_num_poly = 0;
    m_num_points = 0;
    m_reset_flag = false;
}

//------------------------------------------------------------------------
void hdc_rasterizer::move_to(int x, int y) 
{ 
    if(m_reset_flag) reset();
    if(m_num_points < max_points && m_num_poly < max_polygons)
    {
        m_points[m_num_points].x = x;
        m_points[m_num_points].y = y;
        m_counts[m_num_poly] = 1;
        m_num_points++;
        m_num_poly++;
    }
}

//------------------------------------------------------------------------
void hdc_rasterizer::line_to(int x, int y) 
{ 
    if(m_num_points < max_points && m_num_poly)
    {
        m_points[m_num_points].x = x;
        m_points[m_num_points].y = y;
        m_counts[m_num_poly-1]++;
        m_num_points++;
    }
}

//------------------------------------------------------------------------
void hdc_rasterizer::render()
{
    if(m_num_poly)
    {
        ::PolyPolygon(m_dc, m_points, m_counts, m_num_poly);
    }
    m_reset_flag = true;
}




agg::slider_ctrl<agg::rgba8> g_sat_slider(5, 5, 150, 12, true);
agg::path_storage g_path;
agg::rgba8        g_colors[100];
unsigned          g_path_idx[100];
unsigned          g_npaths = 0;
double            g_x1 = 0;
double            g_y1 = 0;
double            g_x2 = 0;
double            g_y2 = 0;
double            g_base_dx = 0;
double            g_base_dy = 0;
double            g_angle = 0;
double            g_scale = 1.0;




unsigned parse_lion(agg::path_storage& ps, agg::rgba8* colors, unsigned* path_idx);
void parse_lion()
{
    g_npaths = parse_lion(g_path, g_colors, g_path_idx);
    agg::bounding_rect(g_path, g_path_idx, 0, g_npaths, &g_x1, &g_y1, &g_x2, &g_y2);
    g_base_dx = (g_x2 - g_x1) / 2.0;
    g_base_dy = (g_y2 - g_y1) / 2.0;
}




/////////////////////////////////////////////////////////////////////////////
// CCustomRasterizerView construction/destruction

CCustomRasterizerView::CCustomRasterizerView()
{
    parse_lion();
}

CCustomRasterizerView::~CCustomRasterizerView()
{
}

BOOL CCustomRasterizerView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCustomRasterizerView drawing

void CCustomRasterizerView::OnDraw(CDC* pDC)
{
    RECT clrect;
    GetClientRect(&clrect);
    int width = clrect.right - clrect.left;
    int height = clrect.bottom - clrect.top;

    hdc_rasterizer r(*pDC, agg::rgba8());

    agg::trans_affine mtx;
    mtx *= agg::trans_affine_translation(-g_base_dx, -g_base_dy);
    mtx *= agg::trans_affine_scaling(g_scale, g_scale);
    mtx *= agg::trans_affine_rotation(g_angle);
    mtx *= agg::trans_affine_translation(width/2, height/2);

    agg::path_storage p2(g_path);
    agg::conv_transform<agg::path_storage> trans(p2, mtx);

    r.saturation(unsigned(g_sat_slider.value() * 255.0));
    r.render_all_paths(trans, g_colors, g_path_idx, g_npaths);

    r.render_ctrl(g_sat_slider);
}

/////////////////////////////////////////////////////////////////////////////
// CCustomRasterizerView diagnostics

#ifdef _DEBUG
void CCustomRasterizerView::AssertValid() const
{
	CView::AssertValid();
}

void CCustomRasterizerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCustomRasterizerDoc* CCustomRasterizerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCustomRasterizerDoc)));
	return (CCustomRasterizerDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CCustomRasterizerView message handlers

void transform(double width, double height, double x, double y)
{
    x -= width / 2;
    y -= height / 2;
    g_angle = atan2(y, x);
    g_scale = sqrt(y * y + x * x) / 100.0;
}


void CCustomRasterizerView::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if(g_sat_slider.in_rect(point.x, point.y))
    {
        if(g_sat_slider.on_mouse_button_down(point.x, point.y))
        {
            Invalidate();
        }
    }
    else
    {
        RECT clrect;
        GetClientRect(&clrect);
        int width = clrect.right - clrect.left;
        int height = clrect.bottom - clrect.top;
        transform(width, height, point.x, point.y);
        Invalidate();
	    CView::OnLButtonDown(nFlags, point);
    }
	CView::OnLButtonDown(nFlags, point);
}

void CCustomRasterizerView::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if(g_sat_slider.on_mouse_button_up(point.x, point.y))
    {
        Invalidate();
    }
	CView::OnLButtonUp(nFlags, point);
}

void CCustomRasterizerView::OnMouseMove(UINT nFlags, CPoint point) 
{
    if(nFlags & MK_LBUTTON)
    {
        if(g_sat_slider.on_mouse_move(point.x, point.y, true))
        {
            Invalidate();
        }
        else
        {
            if(!g_sat_slider.in_rect(point.x, point.y))
            {
                RECT clrect;
                GetClientRect(&clrect);
                int width = clrect.right - clrect.left;
                int height = clrect.bottom - clrect.top;
                transform(width, height, point.x, point.y);
                Invalidate();
	            CView::OnMouseMove(nFlags, point);
            }
        }
    }
	CView::OnMouseMove(nFlags, point);
}
