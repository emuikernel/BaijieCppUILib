#include "stdafx.h"
#include "ACanvasSkia.h"
#include "APathSkia.h"
#include "AMatrixSkia.h"
#include "SkTypeface.h"
#include "SkGradientShader.h"

class _BitmapBuf
{
public:
	APen		m_Pen;
	ASolidBrush	m_Brush;
	AFont		m_Font;
	
};
static _BitmapBuf g_BitmapBuf;

SkColor toSkColor(AColor cr)
{
	return SkColorSetARGB(cr.alpha,cr.red,cr.green,cr.blue);
}


ACanvasSkia::ACanvasSkia():ACanvas()
{
	m_hDC = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_pPen = NULL;
	m_pBrush = NULL;
	m_pFont = NULL;
	m_pMatrix = NULL;
	m_SmoothingMode = SmoothingModeAntiAlias;
	m_pCanvas = NULL;
	m_pBitmap = NULL;
	m_pLine = NULL;
}

ACanvasSkia::~ACanvasSkia()
{
	for(std::map<AString,SkTypeface*>::iterator it=m_aFontType.begin();it!=m_aFontType.end();it++)
	{
		SkTypeface* p = it->second;
		p->unref();
	}
	m_aFontType.clear();
	Free();
}

void ACanvasSkia::Create(HDC hDC,int nWidth,int nHeight)
{
	Free();
	//clear
	Reset();

	if( nWidth < 1 || hDC == NULL ) nWidth = 1;
	if( nHeight < 1 || hDC == NULL ) nHeight = 1;

	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_pBitmap = new SkBitmap();
	m_pBitmap->setConfig(SkBitmap::kARGB_8888_Config,m_nWidth,m_nHeight);
	m_pLine = new BYTE[m_nWidth*4];
	m_pBitmap->allocPixels();
	m_pCanvas = new SkCanvas(*m_pBitmap);
	m_SmoothingMode = SmoothingModeAntiAlias;
	m_TextHint = thFull;
	
	m_hDC = hDC;

	m_rClip = ToSkRect(ARect(0,0,m_nWidth,m_nHeight));
}

APen* ACanvasSkia::GetPen()
{
	m_Paint.setAntiAlias(m_SmoothingMode == SmoothingModeAntiAlias ? true : false);
	if( m_pPen == NULL ) m_pPen = &(g_BitmapBuf.m_Pen);
	return m_pPen;
}
ABrush* ACanvasSkia::GetBrush()
{
	m_Paint.setAntiAlias(m_SmoothingMode == SmoothingModeAntiAlias ? true : false);
	if( m_pBrush == NULL ) m_pBrush = &(g_BitmapBuf.m_Brush);
	return m_pBrush;
}
AFont* ACanvasSkia::GetFont()
{
	uint32_t flags= m_Paint.getFlags();
	flags &= ~SkPaint::kSubpixelText_Flag;
	flags &= ~SkPaint::kLCDRenderText_Flag;
	flags &= ~SkPaint::kAutoHinting_Flag;
	flags &= ~SkPaint::kDevKernText_Flag;
	if( m_TextHint == thDefault )
	{
		m_Paint.setAntiAlias( true );
		//m_Paint.setHinting(SkPaint::kSlight_Hinting);
		flags |= 0
			|SkPaint::kSubpixelText_Flag 
			//|SkPaint::kLCDRenderText_Flag
			//|SkPaint::kAutoHinting_Flag
			//|SkPaint::kDevKernText_Flag
			;
		flags |= SkPaint::kAntiAlias_Flag;
		m_Paint.setFlags(flags);
	}
	else if( m_TextHint == thFull )
	{
		m_Paint.setAntiAlias( true );
		//m_Paint.setHinting(SkPaint::kSlight_Hinting);
		flags |= 0
			|SkPaint::kSubpixelText_Flag 
			|SkPaint::kLCDRenderText_Flag
			|SkPaint::kAutoHinting_Flag
			//|SkPaint::kDevKernText_Flag
			;
		flags |= SkPaint::kAntiAlias_Flag;
		m_Paint.setFlags(flags);
	}
	m_Paint.setFlags(flags);
	
	if( m_pFont == NULL ) m_pFont = &(g_BitmapBuf.m_Font);
	return m_pFont;
}

void ACanvasSkia::Write(ARect rDest,ARect rSrc)//输出到目的（屏幕、图片等）
{
	if( m_hDC != NULL && IsValid() )
	{
		BITMAPINFO hDib;
		memset(&hDib, 0, sizeof(BITMAPINFO));
		hDib.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		hDib.bmiHeader.biWidth = m_nWidth;
		hDib.bmiHeader.biHeight = m_nHeight;
		hDib.bmiHeader.biPlanes = 1;
		hDib.bmiHeader.biBitCount = 32;
		hDib.bmiHeader.biCompression = BI_RGB;
		hDib.bmiHeader.biSizeImage = m_nWidth * m_nHeight * 4;

		LPVOID pBits;
		HBITMAP hBitmap = CreateDIBSection(m_hDC, &hDib, DIB_RGB_COLORS, &pBits, NULL, 0);
		if( hBitmap )
		{
			BYTE* ptr = (BYTE *)pBits;
			for(int i=m_nHeight-1;i>=0;i--)
			{
				memcpy(ptr,GetLine(i),m_nWidth*4);
				ptr += m_nWidth * 4;
			}

			HDC hMemDC=CreateCompatibleDC( m_hDC );
			if( hMemDC )
			{
				HBITMAP hbmOld=(HBITMAP)::SelectObject(hMemDC, hBitmap);
				BLENDFUNCTION bln;
				bln.BlendOp=AC_SRC_OVER;
				bln.BlendFlags=0;
				bln.SourceConstantAlpha=255;
				bln.AlphaFormat=AC_SRC_ALPHA;
			
				AlphaBlend(
					m_hDC, 
					rDest.left,rDest.top,rDest.GetWidth(),rDest.GetHeight(),
					hMemDC, 
					rSrc.left,rSrc.top,rSrc.GetWidth(),rSrc.GetHeight(),
					bln);

				//BitBlt(m_hDC, 0, 0, m_nWidth, m_nHeight, hMemDC, 0, 0, SRCCOPY);
				// 				::SetDIBitsToDevice(
				// 					m_hDC,            // handle to device context
				// 					0,           // x-coordinate of upper-left corner of 
				// 					0,           // y-coordinate of upper-left corner of 
				// 					m_nWidth,       // source rectangle width
				// 					m_nHeight,      // source rectangle height
				// 					0,           // x-coordinate of lower-left corner of 
				// 					0,           // y-coordinate of lower-left corner of 
				// 					0,               // first scan line in array
				// 					m_nHeight,      // number of scan lines
				// 					m_pBuff,           // address of array with DIB bits
				// 					&hDib,           // address of structure with bitmap info.
				// 					DIB_RGB_COLORS   // RGB or palette indexes
				// 					);
				::SelectObject(hMemDC, hbmOld);
				//::GdiFlush();
				DeleteDC(hMemDC);
			}
			::DeleteObject(hBitmap);
		}
	}
}
bool ACanvasSkia::IsValid()
{
	return (m_nWidth > 0 && m_nHeight > 0 ? true : false);
}

void ACanvasSkia::Free()
{
	if( m_pCanvas )
	{
		delete m_pCanvas;
		m_pCanvas = NULL;
	}
	if( m_pBitmap )
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
	if( m_pLine )
	{
		delete[] m_pLine;
		m_pLine = NULL;
	}
	Reset();
	m_nWidth = 0;
	m_nHeight = 0;
}
void ACanvasSkia::Reset()
{
	m_pPen = NULL;
	m_pBrush = NULL;
	m_pFont = NULL;
	m_pMatrix = NULL;
	m_Paint.reset();
}

ACanvas* ACanvasSkia::Use(AFont* pFont)
{
	m_pFont = pFont;
	return this;
}
ACanvas* ACanvasSkia::Use(APen* pPen)
{
	m_pPen = pPen;
	return this;
}
ACanvas* ACanvasSkia::Use(ABrush* pBrush)
{
	m_pBrush = pBrush;
	return this;
}
ACanvas* ACanvasSkia::Use(AMatrix* pMatrix)
{
	m_pMatrix = pMatrix;
	return this;
}

void ACanvasSkia::Translate(int dx,int dy)
{
	//待实现
}
APath* ACanvasSkia::CreatePath()
{
	return new APathSkia;
}
void ACanvasSkia::DestroyPath(APath* p)
{
	APathSkia* x = dynamic_cast<APathSkia*>(p);
	if( x ) delete x;
}
AMatrix* ACanvasSkia::CreateMatrix()
{
	return new AMatrixSkia;
}
void ACanvasSkia::DestroyMatrix(AMatrix *p)
{
	AMatrixSkia* x = dynamic_cast<AMatrixSkia*>(p);
	if( x ) delete x;
}

SmoothingMode ACanvasSkia::GetSmoothingMode()
{
	return m_SmoothingMode;
}
void ACanvasSkia::SetSmoothingMode(SmoothingMode smoothingMode)
{
	m_SmoothingMode = smoothingMode;
}

void ACanvasSkia::DrawPoint(APoint pt,AColor cr)
{
// 	m_pBitmap->lockPixels();
	BYTE* pLine = GetLine(pt.y);
	
 	pLine += pt.x * 4;
//	*(DWORD *)pLine = (cr.blue << 24) & 0xFF000000 + (cr.green << 16) & 0xFF0000 + (cr.red << 8) & 0xFF00 + 0xFF;
	*pLine ++ = cr.blue;
	*pLine ++ = cr.green;
	*pLine ++ = cr.red;
	*pLine ++ = cr.alpha;
// 	m_pBitmap->unlockPixels();
	//m_pBitmap->lockPixels();
	//m_pBitmap->Copy
	//_clip();
// 	m_Paint.setColor(toSkColor(cr));
// 	m_Paint.setStyle(SkPaint::kStroke_Style);
// 	m_pCanvas->drawPoint(SkIntToScalar(pt.x),SkIntToScalar(pt.y),m_Paint);
	//Reset();
}
void ACanvasSkia::DrawLine(int x1,int y1,int x2,int y2)
{
	DrawLine(APoint(x1,y1),APoint(x2,y2));
}

void ACanvasSkia::DrawLine(APoint pt1,APoint pt2)
{
	_clip();
	_SetPaint_Draw();
	SkScalar x0,y0,x1,y1;
	x0 = SkIntToScalar(pt1.x);
	y0 = SkIntToScalar(pt1.y);
	x1 = SkIntToScalar(pt2.x);
	y1 = SkIntToScalar(pt2.y);
	m_pCanvas->drawLine(x0,y0,x1,y1,m_Paint);
	Reset();
}


void ACanvasSkia::DrawRect(int x1,int y1,int x2,int y2)
{
	DrawRect(ARect(x1,y1,x2,y2));
}

void ACanvasSkia::DrawRect(ARect r)
{
	_clip();
	_SetPaint_Draw();

	SkRect rx;
	rx = ToSkRect(r);
	
	m_pCanvas->drawRect(rx,m_Paint);
	Reset();
}

void ACanvasSkia::FillRect(int x1,int y1,int x2,int y2)
{
	FillRect(ARect(x1,y1,x2,y2));
}

void ACanvasSkia::FillRect(ARect r)
{
	_clip();
	_SetPaint_Fill();
	m_pCanvas->drawRect( ToSkRect(r),m_Paint );
	Reset();
}

void ACanvasSkia::DrawRoundRect(int x1,int y1,int x2,int y2,int round)
{
	DrawRoundRect(ARect(x1,y1,x2,y2),round);
}

void ACanvasSkia::DrawRoundRect(ARect r,int round)
{
	_clip();
	_SetPaint_Draw();
	m_pCanvas->drawRoundRect(ToSkRect(r),round,round,m_Paint);
	Reset();
}

void ACanvasSkia::FillRoundRect(int x1,int y1,int x2,int y2,int round)
{
	FillRoundRect(ARect(x1,y1,x2,y2),round);
}

void ACanvasSkia::FillRoundRect(ARect r,int round)
{
	_clip();
	_SetPaint_Fill();
	m_pCanvas->drawRoundRect(ToSkRect(r),round,round,m_Paint);
	Reset();
}

void ACanvasSkia::DrawEllipse(int x1,int y1,int x2,int y2)
{
	DrawEllipse(ARect(x1,y1,x2,y2));
}

void ACanvasSkia::DrawEllipse(ARect r)
{
	_clip();
	_SetPaint_Draw();
	SkPath path;
	path.addArc(ToSkRect(r),0,360);
	m_pCanvas->drawPath(path,m_Paint);
	Reset();
}

void ACanvasSkia::FillEllipse(int x1,int y1,int x2,int y2)
{
	FillEllipse(ARect(x1,y1,x2,y2));
}

void ACanvasSkia::FillEllipse(ARect r)
{
	_clip();
	_SetPaint_Fill();
	SkPath path;
	path.addArc(ToSkRect(r),0,360);
	m_pCanvas->drawPath(path,m_Paint);
	
	Reset();
}

void ACanvasSkia::DrawPolygon(APoint* pts,int count)
{
	if( count == 1 )
	{
		_clip();
		_SetPaint_Draw();
		m_pCanvas->drawPoint(pts[0].x,pts[0].y,m_Paint);
		Reset();
	}
	else if( count < 1 )
	{
		//do nothing
	}
	else
	{
		_SetPaint_Draw();
		SkPath path;
		path.moveTo(SkIntToScalar(pts[0].x),SkIntToScalar(pts[0].y));
		for(int i=0;i<count;i++)
		{
			path.lineTo(SkIntToScalar(pts[i].x),SkIntToScalar(pts[i].y));
		}
		path.close();
		m_pCanvas->drawPath(path,m_Paint);
	}
}

void ACanvasSkia::FillPolygon(APoint* pts,int count)
{
	if( count == 1 )
	{
		_clip();
		_SetPaint_Fill();
		m_pCanvas->drawPoint(SkIntToScalar(pts[0].x),SkIntToScalar(pts[0].y),m_Paint);
		Reset();
	}
	else if( count < 1 )
	{
		//do nothing
	}
	else
	{
		_SetPaint_Fill();
		SkPath path;
		path.moveTo(SkIntToScalar(pts[0].x),SkIntToScalar(pts[0].y));
		for(int i=1;i<count;i++)
		{
			//path.moveTo(SkIntToScalar(pts[i].x),SkIntToScalar(pts[i].y));
			path.lineTo(SkIntToScalar(pts[i].x),SkIntToScalar(pts[i].y));
		}
		path.close();
		m_pCanvas->drawPath(path,m_Paint);
	}
}



void ACanvasSkia::DrawTrigle(APoint pt1,APoint pt2,APoint pt3)
{
	APoint pts[3];
	pts[0] = pt1;
	pts[1] = pt2;
	pts[2] = pt3;
	//pts[3] = pt1;
	DrawPolygon(pts,3);
}
void ACanvasSkia::FillTrigle(APoint pt1,APoint pt2,APoint pt3)
{
	APoint pts[4];
	pts[0] = pt1;
	pts[1] = pt2;
	pts[2] = pt3;
	pts[3] = pt1;
	FillPolygon(pts,4);
}

void ACanvasSkia::DrawText(int x1,int y1,const AString& sText)
{
	//_clip();
	_SetPaint_Font();

	int nFontSize = GetFont()->GetSize();
	y1 += nFontSize;
	//y1 -= 2;//微调 fix me later
	//AString str;
	//str = sText;
// 	AMemory ms;
// 	ms.SetSize( ::WideCharToMultiByte(CP_UTF8,0,sText.Text(),sText.Length(),NULL,0,NULL,NULL) +1 );
// 	char* buf = (char *)ms.GetData();
// 	::WideCharToMultiByte(CP_UTF8,0,sText.Text(),sText.Length(),buf,ms.GetSize()-1,NULL,NULL);
// 	buf[ms.GetSize()-1] = 0;
	const char* sz = (char *)sText.Text();// buf;//str.ConvertTo();
	int iLen = sText.Length()*sizeof(ACHAR);// sz ? strlen(sz) : 0 );

	m_Paint.setTextSize(SkIntToScalar(nFontSize));
	//m_Paint.setSubpixelText(false);
	//m_Paint.setLinearText(true);
	//m_Paint.setHinting(SkPaint::kFull_Hinting);
	m_pCanvas->drawText(sz,iLen,SkIntToScalar(x1),SkIntToScalar(y1),m_Paint);
	
// 	SkPoint pts[1000];
// 	SkScalar xpos = x1;
// 	//SkASSERT(length <= SK_ARRAY_COUNT(pts));
// 	for (size_t i = 0; i < iLen; i++) {
// 		pts[i].set(xpos, y1), xpos += nFontSize;
// 	}
// 	m_pCanvas->drawPosText(sz, iLen, pts, m_Paint);

	Reset();
}

void ACanvasSkia::DrawText(int x1,int y1,const AString& sText,int iRowSpace)
{
	
	Reset();
}

void ACanvasSkia::DrawText(ARect r,const AString& sText,StringAlignment hAlign /*= StringAlignmentNear*/,StringAlignmentV vAlign /*=StringAlignmentMiddle*/)
{
	AFont* pFont = GetFont();
	_clip();

	AAutoClip clip(this,r);

	int maxWidth = MeasureText(sText).cx;
	int nFontSize = pFont->GetSize();
	int x=r.left;
	int delta = ((int)r.GetWidth()-maxWidth)/2;
	switch(hAlign)
	{
	default:
	case StringAlignmentNear:
		x = (REAL)r.left;
		break;
	case StringAlignmentCenter:
		x = r.left+delta;
		break;
	case StringAlignmentFar:
		x = r.right - maxWidth;
		break;

	}
	int y = r.top;
	switch(vAlign)
	{
	default:
	case StringAlignmentTop:
		y = r.top;
		break;
	case StringAlignmentMiddle:
		y = r.top + (r.GetHeight()-nFontSize)/2 - ( (r.GetHeight()-nFontSize) % 2);
		break;
	case StringAlignmentBottom:
		y = r.bottom-nFontSize;
		break;
	}
	//y -= 2;//微调 fix me later
	//y -= nFontSize/2;
	DrawText(x,y,sText);
}
ASize ACanvasSkia::MeasureText(const AString& sText)
{
	_SetPaint_Font();
	int x = m_Paint.measureText(sText.Text(),sText.Length()*sizeof(ACHAR));
	return ASize(x,GetFont()->GetSize());
}
ASize ACanvasSkia::MeasureText(const AString& sText,int iRowSpace)//多行文本的计算
{
	return MeasureText(sText);//fix me later
}
int ACanvasSkia::MeasurePosition(const AString& sText,int iPos)//根据位置找到所在字符串的下标
{
	_SetPaint_Font();
	int iLen = sText.Length();
	ACHAR* s = (ACHAR*)sText.Text();
	int x=0;
	int i=0;
	for(i=0;i<iLen;i++)
	{
		int y = m_Paint.measureText(s,(i+1)*sizeof(ACHAR));
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

void ACanvasSkia::DrawGraphic(AImage* pGraphic,int x,int y,int w,int h)
{
	_clip();
	SkRect rDst;
	SkIRect rSrc;
	if( w == 0 ) w = pGraphic->GetWidth();
	if( h == 0 ) h = pGraphic->GetHeight();
	rDst.set(x,y,x+w+1,y+h+1);
	rSrc.set(0,0,pGraphic->GetWidth(),pGraphic->GetHeight());

	SkBitmap skb;
	skb.setConfig(SkBitmap::kARGB_8888_Config,pGraphic->GetWidth(),pGraphic->GetHeight());
	skb.setPixels(pGraphic->GetPixels());
// 	SkPaint paint;
// 	paint.setFlags(0);
// 	paint.setAntiAlias(false);
	m_pCanvas->drawBitmapRect(skb,&rSrc,rDst);
	Reset();
}


void ACanvasSkia::DrawPath(APath* pPath)
{
	_clip();
	_SetPaint_Draw();

	APathSkia* pp = dynamic_cast<APathSkia*>(pPath);
	if( pp )
	{
		m_pCanvas->drawPath(pp->m_Path,m_Paint);
	}
	Reset();
}
void ACanvasSkia::FillPath(APath* pPath)
{
	_clip();
	_SetPaint_Fill();

	APathSkia* pp = dynamic_cast<APathSkia*>(pPath);
	if( pp )
	{
		m_pCanvas->drawPath(pp->m_Path,m_Paint);
	}
	Reset();
}

void ACanvasSkia::SetClipBox(ARect r)
{
	m_rClip = ToSkRect(r);
}
ARect ACanvasSkia::GetClipBox()
{
	return ARect(m_rClip.fLeft,m_rClip.fTop,m_rClip.fRight,m_rClip.fBottom);
}

void ACanvasSkia::_clip()
{
	SkRect r=m_rClip;
	//r.fRight++;
	//r.fBottom++;
	m_pCanvas->clipRect(r,SkRegion::kReplace_Op);
}

SkRect ACanvasSkia::ToSkRect(ARect r)
{
	SkRect rx;
	rx.set(SkIntToScalar(r.left),SkIntToScalar(r.top),SkIntToScalar(r.right),SkIntToScalar(r.bottom));
	return rx;
}

void ACanvasSkia::_SetPaint_Draw()
{
	APen* pen = GetPen();
	m_Paint.setColor(toSkColor(pen->GetColor()));
	m_Paint.setStrokeWidth(SkScalar(pen->GetWidth()));
	m_Paint.setStyle(SkPaint::kStroke_Style);
}

void ACanvasSkia::_SetPaint_Fill()
{
	ABrush* br = GetBrush();
	m_Paint.setStyle(SkPaint::kFill_Style);

	if( br->GetType() == BrushTypeSolidColor )
	{
		ASolidBrush* sb = (ASolidBrush *)br;
		m_Paint.setColor(toSkColor(sb->GetColor()));
	}
	else if( br->GetType() == BrushTypeLinearGradient )
	{
		SkPoint pts[2];
		ALinearGradientBrush* lgb = (ALinearGradientBrush*)br;
		SkRect rx = ToSkRect( lgb->GetRect() );
		pts[0].set( rx.fLeft,rx.fTop );
		if( lgb->IsVert() ) pts[1].set( rx.fLeft,rx.fBottom );
		else pts[1].set( rx.fRight,rx.fTop );
		SkColor colors[2];
		colors[0] = toSkColor( lgb->GetStartColor() );
		colors[1] = toSkColor( lgb->GetEndColor() );
		SkScalar pos[2];
		pos[0] = SkScalar(0.0);
		pos[1] = SkScalar(1.0);

		SkShader* pShader = SkGradientShader::CreateLinear(pts,colors,pos,2,SkShader::kClamp_TileMode);
		m_Paint.setShader(pShader)->unref();
	}
	else if( br->GetType() == BrushTypeRadialGradient )
	{
		ARadialBrush* rb = (ARadialBrush*)br;
		SkPoint ptCenter;
		APoint ptCenter0 = rb->GetCenter();
		ptCenter.set( SkIntToScalar(ptCenter0.x),SkIntToScalar(ptCenter0.y) );
		SkScalar nRadius = SkIntToScalar( rb->GetRadius() );

		SkColor colors[2];
		colors[0] = toSkColor( rb->GetStartColor() );
		colors[1] = toSkColor( rb->GetEndColor() );
		SkScalar pos[2];
		pos[0] = SkScalar(0.0);
		pos[1] = SkScalar(1.0);

		SkShader* pShader = SkGradientShader::CreateRadial(ptCenter,nRadius,colors,pos,2,SkShader::kClamp_TileMode);
		m_Paint.setShader(pShader)->unref();
	}
}

void ACanvasSkia::_SetPaint_Font()
{
	AFont* pFont = GetFont();

	AString sFontName;
	sFontName = pFont->GetName();

	SkTypeface* tf = NULL;
	std::map<AString,SkTypeface*>::iterator it=m_aFontType.find(sFontName);
	if( it != m_aFontType.end() ) tf = it->second;
	//SkTypeface* tf = SkTypeface::CreateFromName(sFontName.ConvertTo(),SkTypeface::kNormal);
	if( tf == NULL )
	{
		tf = SkTypeface::CreateFromName(sFontName.ConvertTo(),SkTypeface::kNormal);
		m_aFontType[sFontName] = tf;
		//tf->ref();
	}
	if( tf ) m_Paint.setTypeface(tf);//->unref();

	m_Paint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
	if( (pFont->GetStyle() & FontStyleBold) == FontStyleBold ) m_Paint.setFakeBoldText(true);
	if( (pFont->GetStyle() & FontStyleItalic) == FontStyleItalic ) m_Paint.setTextSkewX(0.25);
	m_Paint.setColor( toSkColor( pFont->GetColor() ) );
	m_Paint.setTextSize(SkIntToScalar(pFont->GetSize()));
}
