// CustomRendererView.cpp : implementation of the CCustomRendererView class
//

#include "stdafx.h"
#include "CustomRenderer.h"

#include "CustomRendererDoc.h"
#include "CustomRendererView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_trans_affine.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_color_rgba.h"
#include "agg_bounding_rect.h"
#include "ctrl/agg_slider_ctrl.h"


/////////////////////////////////////////////////////////////////////////////
// CCustomRendererView

IMPLEMENT_DYNCREATE(CCustomRendererView, CView)

BEGIN_MESSAGE_MAP(CCustomRendererView, CView)
	//{{AFX_MSG_MAP(CCustomRendererView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()





//============================================================================
// Demonstration of the principle of creating custom scanline renderers.
// This class doesn't use anything but WinGDI functions:
// GetPixel(), SetPixel(), SetBkColor(), and ExtTextOut(). The last two ones
// are used to draw a solid horizontal line (I took this method from the MFC 
// function FillSolidRect()) 
// 
// This is only a demonstration!!!
//
// So, do NOT expect great performance from it. In fact the performance is 
// terribly low because I use GetPixel/SetPixel to draw proper anti-aliased 
// edges.
//
// The example also demonstrates the use of the AGG controls. The control 
// in this example changes the saturation of the image and it's also being 
// rendered with this very class. 
//
// At the end of this file you can find the code that makes the control "alive"
// i.e. just processing the mouse events.
// 
//----------------------------------------------------------------------------
class hdc_renderer
{
public:
    hdc_renderer(HDC dc, agg::rgba8 c) :
        m_dc(dc),
        m_rgb(c),
        m_clr(RGB(c.r, c.g, c.b)),
        m_sat(255)
    {
    }
    
    //--------------------------------------------------------------------
    void color(agg::rgba8 c);
    void saturation(unsigned sat) { m_sat = sat; }

    //--------------------------------------------------------------------
    void prepare(unsigned) {}

    //--------------------------------------------------------------------
    // The scanline rendering function itself.
    template<class Scanline> void render(const Scanline& sl)
    {
        unsigned num_spans = sl.num_spans();
        int y = sl.y();

        Scanline::const_iterator span = sl.begin();

        do
        {
            int len = span->len;
            int x;
            if(len > 0)
            {
                // pixel-by-pixel
                const Scanline::cover_type* covers = span->covers;
                x = span->x;
                do
                {
                    blend_pixel(x++, y, *covers++);
                }
                while(--len);
            }
            else
            {
                if(*span->covers == 255)
                {
                    // solid span. happens often
                    draw_solid_span(span->x, y, -len);
                }
                else
                {
                    // pixel-by-pixel again
                    x = span->x;
                    do
                    {
                        blend_pixel(x++, y, *span->covers);
                    }
                    while(++len);
                }
            }
            ++span;
        }
        while(--num_spans);
    }

private:
    void draw_solid_span(int x, int y, int len) const;
    void blend_pixel(int x, int y, unsigned alpha) const;

private:
    HDC           m_dc;
    agg::rgba8    m_rgb;
    COLORREF      m_clr;
    unsigned      m_sat;
};



//----------------------------------------------------------------------------
// The real drawing color is calculated considering the additional
// parameter "saturation". The saturation is also not a part of AGG, it's
// used just to demonstrate that the renderer can have anything you want
// and to make the slider control have some effect :-)
void hdc_renderer::color(agg::rgba8 c)
{
    int gray8 = c.r * 77 + c.g * 150 + c.b * 29;
    int gray = gray8 >> 8;
    int r = (((int(c.r) - gray) * m_sat) + gray8) >> 8;
    int g = (((int(c.g) - gray) * m_sat) + gray8) >> 8;
    int b = (((int(c.b) - gray) * m_sat) + gray8) >> 8;
    m_rgb = agg::rgba8(r, g, b);
    m_clr = RGB(r, g, b);
}


//----------------------------------------------------------------------------
// Drawing a solid span, i.e. a solid horizontal line. That's ridiculous:
// WinGDI doesn't have a function of drawing a solid horizontal line
// (in general a rectangle) without the necessity of creating and destroying
// brushes. ExtTextOut() function is the workaround of it - in this case it 
// doesn't put any text, it just draws a rectangle of the "imaginary" text. 
// I took this "hack" from MFC. 
void hdc_renderer::draw_solid_span(int x, int y, int len) const
{
	::SetBkColor(m_dc, m_clr);
	RECT r;
    r.left   = x;
    r.top    = y;
    r.right  = x + len, 
    r.bottom = y + 1;
	::ExtTextOut(m_dc, 0, 0, ETO_OPAQUE, &r, NULL, 0, NULL);
}


//----------------------------------------------------------------------------
// Drawing an Alpha-Blended pixel.
void hdc_renderer::blend_pixel(int x, int y, unsigned alpha) const
{
    agg::rgba8 c = agg::bgr8_packed(::GetPixel(m_dc, x, y));
    int r = (((int(m_rgb.r) - c.r) * alpha) + (c.r << 8)) >> 8;
    int g = (((int(m_rgb.g) - c.g) * alpha) + (c.g << 8)) >> 8;
    int b = (((int(m_rgb.b) - c.b) * alpha) + (c.b << 8)) >> 8;
    ::SetPixel(m_dc, x, y, RGB(r, g, b));
}


//============================================================================








agg::rasterizer_scanline_aa<> g_rasterizer;
agg::scanline_p8              g_scanline;
agg::slider_ctrl<agg::rgba8>  g_sat_slider(5, 5, 150, 12, true);

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












// I'm not the one who should tell you that's this :-)
//============================================================================
/////////////////////////////////////////////////////////////////////////////
// CCustomRendererView construction/destruction

CCustomRendererView::CCustomRendererView()
{
    parse_lion();
}

CCustomRendererView::~CCustomRendererView()
{
}

BOOL CCustomRendererView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCustomRendererView drawing

void CCustomRendererView::OnDraw(CDC* pDC)
{
	CCustomRendererDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    RECT clrect;
    GetClientRect(&clrect);
    int width = clrect.right - clrect.left;
    int height = clrect.bottom - clrect.top;

    hdc_renderer r(*pDC, agg::rgba8());

    agg::trans_affine mtx;
    mtx *= agg::trans_affine_translation(-g_base_dx, -g_base_dy);
    mtx *= agg::trans_affine_scaling(g_scale, g_scale);
    mtx *= agg::trans_affine_rotation(g_angle);
    mtx *= agg::trans_affine_translation(width/2, height/2);

    agg::path_storage p2(g_path);
    agg::conv_transform<agg::path_storage> trans(p2, mtx);

    r.saturation(unsigned(g_sat_slider.value() * 255.0));
    agg::render_all_paths(g_rasterizer, g_scanline, r, trans, g_colors, g_path_idx, g_npaths);
    agg::render_ctrl(g_rasterizer, g_scanline, r, g_sat_slider);
}



/////////////////////////////////////////////////////////////////////////////
// CCustomRendererView diagnostics

#ifdef _DEBUG
void CCustomRendererView::AssertValid() const
{
	CView::AssertValid();
}

void CCustomRendererView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCustomRendererDoc* CCustomRendererView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCustomRendererDoc)));
	return (CCustomRendererDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCustomRendererView message handlers


void transform(double width, double height, double x, double y)
{
    x -= width / 2;
    y -= height / 2;
    g_angle = atan2(y, x);
    g_scale = sqrt(y * y + x * x) / 100.0;
}

void CCustomRendererView::OnLButtonDown(UINT nFlags, CPoint point) 
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
}

void CCustomRendererView::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if(g_sat_slider.on_mouse_button_up(point.x, point.y))
    {
        Invalidate();
    }
	CView::OnLButtonUp(nFlags, point);
}

void CCustomRendererView::OnMouseMove(UINT nFlags, CPoint point) 
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
