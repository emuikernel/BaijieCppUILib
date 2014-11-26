#include "stdafx.h"
#include "ACanvasGdi.h"
#include "APathGdi.h"
#include "AMatrixGdi.h"


ACanvasGdi::ACanvasGdi():ACanvas()
{
	m_hDC = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_pPen = NULL;
	m_pBrush = NULL;
	m_pFont = NULL;
	m_pCanvas = NULL;
	m_dcEmpty = NULL;
	m_hPaintDC = NULL;
	m_hMemBitmap = NULL;
	m_hRgn = NULL;
	m_hOldRgn = NULL;
}

ACanvasGdi::~ACanvasGdi()
{
	Free();
	if( m_dcEmpty )
	{
		::ReleaseDC(0,m_dcEmpty);
		m_dcEmpty = NULL;
	}
}

void ACanvasGdi::Create(HDC hDC,int nWidth,int nHeight)
{
	Free();
	//clear
	Reset();

	if( nWidth < 1 || hDC == NULL ) nWidth = 1;
	if( nHeight < 1 || hDC == NULL ) nHeight = 1;
	if( hDC == NULL )
	{
		if( m_dcEmpty == NULL )
		{
			m_dcEmpty = ::GetDC(0);
		}
		m_hDC = m_dcEmpty;
	}
	else
		m_hDC = hDC;
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_rClip = ARect(0,0,m_nWidth,m_nHeight);

	if( hDC != NULL )
	{
		m_hPaintDC = CreateCompatibleDC(hDC); 
		//::SetWindowOrgEx(hPaintDC,r.left,r.top,NULL);

		HBITMAP hMemBitmap = CreateCompatibleBitmap(hDC, m_nWidth,m_nHeight); // 创建兼容位图
		::SelectObject(m_hPaintDC, hMemBitmap); // 把位图选进DC中, 对DC的操作实际上就是对位图的操作
		m_pCanvas = new GCanvas(m_hPaintDC);
	}
	else
	{
		m_hPaintDC = NULL;
		m_pCanvas = new GCanvas(m_hDC);
	}
}

APen* ACanvasGdi::GetPen()
{
	if( m_pPen == NULL ) 
	{
		m_pCanvas->Pen->Color = AColor(255,0,0,0);
		m_pCanvas->Pen->Width = 1;
		m_pCanvas->Pen->Style = PS_SOLID;
	}
	else
	{
		m_pCanvas->Pen->Color = m_pPen->GetColor();
		m_pCanvas->Pen->Width = m_pPen->GetWidth();
		switch( m_pPen->GetStyle() )
		{
		
		case DashStyleDash: m_pCanvas->Pen->Style = PS_DASH; break;
		case DashStyleDot: m_pCanvas->Pen->Style = PS_DOT; break;
		case DashStyleDashDot: m_pCanvas->Pen->Style = PS_DASHDOT;break;
		case DashStyleDashDotDot: m_pCanvas->Pen->Style = PS_DASHDOTDOT; break;
		case DashStyleSolid: 
		default:
				m_pCanvas->Pen->Style = PS_SOLID; break;
		}
	}
	return m_pPen;
}
ABrush* ACanvasGdi::GetBrush()
{
	if( m_pBrush == NULL )
	{
		
		m_pCanvas->Brush->SetColor( AColor(255,255,255,255) );
		m_pCanvas->Brush->SetStyle(BS_SOLID);
	}
	else
	{
		if( m_pBrush->GetType() == BrushTypeSolidColor )
		{
			ASolidBrush* sb = (ASolidBrush *)m_pBrush;
			m_pCanvas->Brush->SetColor( sb->GetColor() );
			m_pCanvas->Brush->SetStyle(BS_SOLID);
		}
	}
	return m_pBrush;
}
AFont* ACanvasGdi::GetFont()
{
	if( m_pFont == NULL )
	{
		m_pCanvas->Font->Name = _T("宋体");
		//m_pCanvas->Font->Width = 10;
		m_pCanvas->Font->Height = 10;
		m_pCanvas->Font->Bold = FW_NORMAL;
		m_pCanvas->Font->Italic = false;
		m_pCanvas->Font->Underline = false;
		m_pCanvas->Font->StrikeOut = false;
	}
	else
	{
		m_pCanvas->Font->Name = m_pFont->GetName();
		//m_pCanvas->Font->Width = 10;
		m_pCanvas->Font->Height = m_pFont->GetSize();
		m_pCanvas->Font->Bold = m_pFont->GetStyle() & FontStyleBold ? FW_BOLD : FW_NORMAL;
		m_pCanvas->Font->Italic = m_pFont->GetStyle() & FontStyleItalic ? true : false;
		m_pCanvas->Font->Underline = m_pFont->GetStyle() & FontStyleUnderline ? true : false;
		m_pCanvas->Font->StrikeOut = m_pFont->GetStyle() & FontStyleStrikeout ? true : false;
	}
	return m_pFont;
}

void ACanvasGdi::Write(ARect rDest,ARect rSrc)//输出到目的（屏幕、图片等）
{
	if( m_hDC != NULL && m_hPaintDC != NULL && m_hDC != m_dcEmpty )
	{
		DeleteObject(m_hMemBitmap); // 图已经画出来了.删除位图.
		BitBlt(m_hDC,0,0,m_nWidth,m_nHeight, m_hPaintDC, 0, 0, SRCCOPY);  // 把图帖到真实DC上, 这样就能显示出来了.
		DeleteDC(m_hPaintDC); // 删掉内存 DC
		m_hPaintDC = NULL;
		m_hMemBitmap = NULL;
	}
}
bool ACanvasGdi::IsValid()
{
	return (m_nWidth > 0 && m_nHeight > 0 ? true : false);
}

void ACanvasGdi::Free()
{
	if( m_pCanvas )
	{
		delete m_pCanvas;
		m_pCanvas = NULL;
	}
	Reset();
	m_nWidth = 0;
	m_nHeight = 0;
	m_hDC = NULL;
	if( m_hOldRgn )
	{
		::DeleteObject(m_hOldRgn);
		m_hOldRgn = NULL;
	}
	if(m_hRgn )
	{
		::DeleteObject(m_hRgn);
		m_hRgn = NULL;
	}
	if( m_hMemBitmap )
	{
		::DeleteObject(m_hMemBitmap);
		m_hMemBitmap = NULL;
	}
	if( m_hPaintDC )
	{
		::DeleteObject(m_hPaintDC);
		m_hPaintDC = NULL;
	}
}
void ACanvasGdi::Reset()
{
	m_pPen = NULL;
	m_pBrush = NULL;
	m_pFont = NULL;
	//m_pMatrix = NULL;
}

ACanvas* ACanvasGdi::Use(AFont* pFont)
{
	m_pFont = pFont;
	return this;
}
ACanvas* ACanvasGdi::Use(APen* pPen)
{
	m_pPen = pPen;
	return this;
}
ACanvas* ACanvasGdi::Use(ABrush* pBrush)
{
	m_pBrush = pBrush;
	return this;
}
ACanvas* ACanvasGdi::Use(AMatrix* pMatrix)
{
	//m_pMatrix = pMatrix;
	return this;
}

void ACanvasGdi::Translate(int dx,int dy)
{
	//待实现
}
APath* ACanvasGdi::CreatePath()
{
	return new APathGdi();
}
void ACanvasGdi::DestroyPath(APath* p)
{
	APathGdi* x = dynamic_cast<APathGdi*>(p);
	if( x ) delete x;
}
AMatrix* ACanvasGdi::CreateMatrix()
{
	return new AMatrixGdi();
}
void ACanvasGdi::DestroyMatrix(AMatrix *p)
{
	AMatrixGdi* x = dynamic_cast<AMatrixGdi*>(p);
	if( x ) delete x;
}

SmoothingMode ACanvasGdi::GetSmoothingMode()
{
	return SmoothingModeAntiAlias;
}
void ACanvasGdi::SetSmoothingMode(SmoothingMode smoothingMode)
{
	
}

void ACanvasGdi::DrawPoint(APoint pt,AColor cr)
{
	if( m_hPaintDC == NULL ) return;
	m_pCanvas->DrawPoint(pt,cr);
}
void ACanvasGdi::DrawLine(int x1,int y1,int x2,int y2)
{
	DrawLine(APoint(x1,y1),APoint(x2,y2));
}

void ACanvasGdi::DrawLine(APoint pt1,APoint pt2)
{
	if( m_hPaintDC == NULL ) return;
	_clip();
	GetPen();
	m_pCanvas->DrawLine(pt1.x,pt1.y,pt2.x,pt2.y);
	Reset();
}


void ACanvasGdi::DrawRect(int x1,int y1,int x2,int y2)
{
	DrawRect(ARect(x1,y1,x2,y2));
}

void ACanvasGdi::DrawRect(ARect r)
{
	if( m_hPaintDC == NULL ) return;
	_clip();
	
	GetPen();
	m_pCanvas->DrawRect(r);

	Reset();
}

void ACanvasGdi::FillRect(int x1,int y1,int x2,int y2)
{
	FillRect(ARect(x1,y1,x2,y2));
}

void ACanvasGdi::FillRect(ARect r)
{
	if( m_hPaintDC == NULL ) return;
	_clip();
	GetBrush();
	m_pCanvas->FillRect(r);
	Reset();
}

void ACanvasGdi::DrawRoundRect(int x1,int y1,int x2,int y2,int round)
{
	DrawRoundRect(ARect(x1,y1,x2,y2),round);
}

void ACanvasGdi::DrawRoundRect(ARect r,int round)
{
	if( m_hPaintDC == NULL ) return;
	_clip();
	GetPen();
	m_pCanvas->DrawRoundRect(r,round,round);
	Reset();
}

void ACanvasGdi::FillRoundRect(int x1,int y1,int x2,int y2,int round)
{
	FillRoundRect(ARect(x1,y1,x2,y2),round);
}

void ACanvasGdi::FillRoundRect(ARect r,int round)
{
	if( m_hPaintDC == NULL ) return;
	_clip();
	GetBrush();
	m_pCanvas->FillRoundRect(r,round,round);
	Reset();
}

void ACanvasGdi::DrawEllipse(int x1,int y1,int x2,int y2)
{
	DrawEllipse(ARect(x1,y1,x2,y2));
}

void ACanvasGdi::DrawEllipse(ARect r)
{
	if( m_hPaintDC == NULL ) return;
	_clip();
	GetPen();
	m_pCanvas->Brush->Style = BS_NULL;
	m_pCanvas->Ellipse(r);
	Reset();
}

void ACanvasGdi::FillEllipse(int x1,int y1,int x2,int y2)
{
	FillEllipse(ARect(x1,y1,x2,y2));
}

void ACanvasGdi::FillEllipse(ARect r)
{
	if( m_hPaintDC == NULL ) return;
	_clip();
	GetBrush();
	m_pCanvas->Pen->SetWidth(0);
	m_pCanvas->Ellipse(r);
	Reset();
}

void ACanvasGdi::DrawPolygon(APoint* pts,int count)
{
	if( m_hPaintDC == NULL ) return;
	if( count == 1 )
	{
		_clip();
		GetPen();
		m_pCanvas->DrawPoint(*pts,m_pCanvas->Pen->Color);
		Reset();
	}
	else if( count < 1 )
	{
		//do nothing
	}
	else
	{
		GetPen();
		m_pCanvas->Brush->Style = BS_NULL;
		m_pCanvas->Polygon(pts,count);
	}
}

void ACanvasGdi::FillPolygon(APoint* pts,int count)
{
	if( m_hPaintDC == NULL ) return;
	if( count == 1 )
	{
		_clip();
		GetBrush();
		m_pCanvas->DrawPoint(*pts,m_pCanvas->Brush->Color);
		Reset();
	}
	else if( count < 1 )
	{
		//do nothing
	}
	else
	{
		m_pCanvas->Pen->Width = 0;
		GetBrush();
		m_pCanvas->Polygon(pts,count);
	}
}



void ACanvasGdi::DrawTrigle(APoint pt1,APoint pt2,APoint pt3)
{
	if( m_hPaintDC == NULL ) return;
	GetPen();
	m_pCanvas->DrawTriple(pt1,pt2,pt3);
}
void ACanvasGdi::FillTrigle(APoint pt1,APoint pt2,APoint pt3)
{
	if( m_hPaintDC == NULL ) return;
	APoint pts[4];
	pts[0] = pt1;
	pts[1] = pt2;
	pts[2] = pt3;
	pts[3] = pt1;
	FillPolygon(pts,4);
}

void ACanvasGdi::DrawText(int x1,int y1,const AString& sText)
{
	if( m_hPaintDC == NULL ) return;
	GetFont();
	m_pCanvas->DrawText(x1,y1,sText);
	Reset();
}

void ACanvasGdi::DrawText(int x1,int y1,const AString& sText,int iRowSpace)
{
	
	Reset();
}

void ACanvasGdi::DrawText(ARect r,const AString& sText,StringAlignment hAlign /*= StringAlignmentNear*/,StringAlignmentV vAlign /*=StringAlignmentMiddle*/)
{
	if( m_hPaintDC == NULL ) return;
	AFont* pFont = GetFont();
	_clip();
	TTextAlign ta;
	switch(hAlign)
	{
	case StringAlignmentNear :
		switch(vAlign)
		{
		case StringAlignmentTop : ta = taLeftTop;break;
		case StringAlignmentMiddle : ta = taLeftMiddle;break;
		case StringAlignmentBottom  : ta = taLeftBottom;break;
		}
		break;
	case StringAlignmentCenter:
		switch(vAlign)
		{
		case StringAlignmentTop : ta = taCenterTop;break;
		case StringAlignmentMiddle : ta = taCenterMiddle;break;
		case StringAlignmentBottom  : ta = taCenterBottom;break;
		}
		break;
	case StringAlignmentFar :
		switch(vAlign)
		{
		case StringAlignmentTop : ta = taRightTop;break;
		case StringAlignmentMiddle : ta = taRightMiddle;break;
		case StringAlignmentBottom  : ta = taRightBottom;break;
		}
		break;
	}
	m_pCanvas->DrawText(r,sText,ta);
}

ASize ACanvasGdi::MeasureText(const AString& sText)
{
	GetFont();
	tagSIZE sz;
	::GetTextExtentPoint32(m_hDC,sText.Text(),sText.Length(),&sz);
	return ASize(sz.cx,sz.cy);
}
ASize ACanvasGdi::MeasureText(const AString& sText,int iRowSpace)//多行文本的计算
{
	return MeasureText(sText);//fix me later
}
int ACanvasGdi::MeasurePosition(const AString& sText,int iPos)//根据位置找到所在字符串的下标
{
	GetFont();
	int iLen = sText.Length();
	ACHAR* s = (ACHAR*)sText.Text();
	int x=0;
	int i=0;
	for(i=0;i<iLen;i++)
	{
		tagSIZE sz;
		::GetTextExtentPoint32(m_hDC,sText.Text(),i,&sz);
		int y = sz.cx;
		if( iPos >= x && iPos <= y ) 
		{
			int delta = (y-x)/2;
			if( iPos < x+delta ) return i;
			else return i+1;
		}
		x = y;
	}
	return i;
}

void ACanvasGdi::DrawGraphic(AImage* pGraphic,int x,int y,int w,int h)
{
	_clip();
	
	Reset();
}


void ACanvasGdi::DrawPath(APath* pPath)
{
	_clip();
	
	Reset();
}
void ACanvasGdi::FillPath(APath* pPath)
{
	_clip();
	
	Reset();
}

void ACanvasGdi::SetClipBox(ARect r)
{
	if( m_hOldRgn )
		::SelectClipRgn(m_hDC,m_hOldRgn);
	m_rClip = r;
	RECT rcClip = { 0 };
	::GetClipBox(m_hDC, &rcClip);
	m_hOldRgn = ::CreateRectRgnIndirect(&rcClip);
	ARect rx=r;
	ARect rc = rcClip;
	rx.Intersect(rc);//取交集
	m_hRgn = ::CreateRectRgnIndirect(&rx);
	::SelectClipRgn(m_hDC,m_hRgn);
}
ARect ACanvasGdi::GetClipBox()
{
	return m_rClip;
}

void ACanvasGdi::_clip()
{
	//SkRect r=m_rClip;
	//r.fRight++;
	//r.fBottom++;
	//m_pCanvas->clipRect(r,SkRegion::kReplace_Op);
}



