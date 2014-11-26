
#include "stdafx.h"
#include "GBrush.h"
#include "GFont.h"
#include "GPen.h"
#include "GCanvas.h"

GCanvas::GCanvas(HDC hDC):AObject()
{
	Init(hDC);
}


void GCanvas::Init(HDC hDC)
{
	m_hDC = hDC;
	if( hDC == NULL )
	{
		m_pPen = NULL;
		m_pBrush = NULL;
		m_pFont = NULL;
		throw AException(_T("Invalid canvas context"));
	}
	else
	{
		m_pFont = new GCanvasFont(m_hDC);
		m_pPen = new GPen(m_hDC);
		m_pBrush = new GBrush(m_hDC);
	}
}
GCanvas::~GCanvas()	
{		
	m_pFont->Do_Clean();
	m_pPen->Do_Clean();
	m_pBrush->Do_Clean();
	delete m_pFont;
	delete m_pPen;
	delete m_pBrush;
}

GCanvas* GCanvas::operator ->()
{
	m_pBrush->Flush();
	m_pPen->Flush();
	m_pFont->Flush();
	return this;
}

GCanvas::operator HDC()
{
	return m_hDC;
}
HDC GCanvas::GetHandle()
{
	return m_hDC;
}
void GCanvas::Flush()
{
	m_pBrush->Flush();
	m_pPen->Flush();
	m_pFont->Flush();
}
GPen*	GCanvas::GetPen()
{	
	return m_pPen;	
}
GBrush*	GCanvas::GetBrush()
{	
	return m_pBrush;
}
GCanvasFont*	GCanvas::GetFont()
{	
	return m_pFont;	
}

void GCanvas::Arc(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	Flush();
	::Arc(m_hDC,x1,y1,x2,y2,x3,y3,x4,y4);
}
void GCanvas::DrawLine(int x1,int y1,int x2,int y2)
{
	Flush();
	APoint pt;
	::MoveToEx(m_hDC,x1,y1,&pt);
	::LineTo(m_hDC,x2,y2);
}
void GCanvas::DrawRect(int x1,int y1,int x2,int y2)
{
	Flush();
	::Rectangle(m_hDC,x1,y1,x2,y2);
}
void GCanvas::DrawRect(const ARect& r)
{
	Flush();
	::Rectangle(m_hDC,r.left,r.top,r.right,r.bottom);
}
void GCanvas::FillRect(int x1,int y1,int x2,int y2)
{
	Flush();
	ARect r(x1,y1,x2,y2);
	::FillRect(m_hDC,&r,m_pBrush->Handle);
}
void GCanvas::FillRect(const ARect& r)
{
	Flush();
	::FillRect(m_hDC,&r,m_pBrush->Handle);
}
void GCanvas::DrawText(int x1,int y1,const AString& sText)
{
	Flush();
	::TextOut(m_hDC,x1,y1,sText.Text(),sText.Length());
}
void GCanvas::DrawText(const ARect& r,const AString& sText,TTextAlign ta)
{
	Flush();

	UINT uFormat=DT_SINGLELINE;
	switch(ta)
	{
	case taLeftTop: uFormat |= DT_LEFT | DT_TOP; break;
	case taCenterTop:uFormat |= DT_CENTER | DT_TOP;break;
	case taRightTop:uFormat |= DT_RIGHT | DT_TOP; break;
	case taLeftMiddle:uFormat |= DT_LEFT | DT_VCENTER;break;
	case taCenterMiddle : uFormat |= DT_CENTER|DT_VCENTER;break;
	case taRightMiddle: uFormat |= DT_RIGHT | DT_VCENTER;break;
	case taLeftBottom: uFormat |= DT_LEFT | DT_BOTTOM; break;
	case taCenterBottom: uFormat |= DT_CENTER | DT_BOTTOM; break;
	case taRightBottom: uFormat |= DT_RIGHT | DT_BOTTOM;break;
	default:break;
	}
	::DrawText(m_hDC,sText.Text(),sText.Length(),(LPRECT)&r,uFormat);
}

void GCanvas::DrawTriple(APoint pt1,APoint pt2,APoint pt3)
{
	Flush();
	POINT pts[4];
	pts[0].x = pt1.x; pts[0].y = pt1.y;
	pts[1].x = pt2.x; pts[1].y = pt2.y;
	pts[2].x = pt3.x; pts[2].y = pt3.y;
	pts[3].x = pt1.x; pts[3].y = pt1.y;
	::Polygon(m_hDC,pts,4);
}

void GCanvas::DrawGraphic(AImage* pGraphic,int x,int y,int w,int h)
{
// 	if( pGraphic == NULL ) return;
// 	ABitmap* pBitmap = dynamic_cast<ABitmap *>(pGraphic);
// 	if( pBitmap )
// 	{
// 		ARect r(x,y,0,0);
// 		if( w == 0 ) r.right = x + pBitmap->GetWidth();
// 		else r.right = x+ w;
// 
// 		if( h == 0 ) r.bottom = y + pBitmap->GetHeight();
// 		else r.bottom = y + h;
// 
// 		pBitmap->Draw(this,r);
// 		return;
// 	}
}

void GCanvas::RoundRect(const ARect& r,int dx,int dy)
{
	Flush();
	::RoundRect(m_hDC,r.left,r.top,r.right,r.bottom,dx,dy);
}

void GCanvas::Ellipse(const ARect& r)
{
	Flush();
	::Ellipse(m_hDC,r.left,r.top,r.right,r.bottom);
}

void GCanvas::Polygon(const APoint* pts,int iCount)
{
	Flush();
	::Polygon(m_hDC,pts,iCount);
}

void GCanvas::DrawPoint(APoint pt,AColor cr)
{
	Flush();
	::SetPixel(m_hDC,pt.x,pt.y,RGB(cr.red,cr.green,cr.blue));
}

void GCanvas::DrawRoundRect(ARect r,int dx,int dy)
{
	GetBrush()->SetStyle(BS_NULL);  
	Flush();
	::RoundRect(m_hDC,r.left,r.top,r.right,r.bottom,dx,dy);
}

void GCanvas::FillRoundRect(ARect r,int dx,int dy)
{
	GetPen()->SetWidth(0);
	Flush();
	::RoundRect(m_hDC,r.left,r.top,r.right,r.bottom,dx,dy);
}