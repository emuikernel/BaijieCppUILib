#include "stdafx.h"
#include "agg.h"
#include "ACanvasAgg.h"
#include "APathAgg.h"
#include "memcache.h"
#include <vector>
#include "AMatrixAgg.h"

static MemCache g_MemCache;

class triangle{
public:
	triangle(APointF pt1,APointF pt2,APointF pt3)
	{
		// 直接准备好三个点
		m_step = 0;
		m_pt1 = pt1;
		m_pt2 = pt2;
		m_pt3 = pt3;
	}
	void rewind(unsigned)
	{
		m_step = 0;
	}
	unsigned vertex(double* x, double* y)
	{
		switch(m_step++)
		{
		case 0:
			//第一步，move_to
			*x = m_pt1.x;
			*y = m_pt1.y;
			return agg::path_cmd_move_to;
		case 1:
			*x = m_pt2.x;
			*y = m_pt2.y;
			return agg::path_cmd_line_to;
		case 2:
			*x = m_pt3.x;
			*y = m_pt3.y;
			return agg::path_cmd_line_to;
// 		case 3:
// 			*x = m_pt1.x;
// 			*y = m_pt1.y;
// 			return agg::path_cmd_line_to;
		case 3:
			// 第四步，闭合多边形
			return agg::path_cmd_end_poly|agg::path_flags_close;
		default:
			// 第五步，结束
			return agg::path_cmd_stop;
		}
	}
private:
	APointF m_pt1;
	APointF m_pt2;
	APointF m_pt3;
	unsigned m_step;
};


BYTE g_EmptyBuf[4];//用于计算时的buffer

#ifdef _DEBUG
BOOL SaveBitmapToFile(HBITMAP   hBitmap, const AString& szfilename)
{
	HDC hDC; //   设备描述表    
	int iBits; //   当前显示分辨率下每个像素所占字节数 
	WORD wBitCount; //   位图中每个像素所占字节数   
	DWORD dwPaletteSize = 0   ; //   定义调色板大小，  位图中像素字节大小  ， 
	//   位图文件大小  ，  写入文件字节数

	DWORD dwBmBitsSize   ;  
	DWORD dwDIBSize, dwWritten   ;  
	BITMAP Bitmap;      
	BITMAPFILEHEADER bmfHdr; //位图属性结构 
	BITMAPINFOHEADER bi; //位图文件头结构  
	LPBITMAPINFOHEADER lpbi;   //位图信息头结构   
	HANDLE fh,   hDib,   hPal,hOldPal = NULL   ; //指向位图信息头结构,定义文件，分配内存句柄，调色板句柄

	//计算位图文件每个像素所占字节数 
	hDC = CreateDC(L"DISPLAY", NULL, NULL, NULL); 
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES)  ;  
	DeleteDC(hDC)   ;  
	if (iBits <= 1)
	{
		wBitCount = 1;
	}
	else if (iBits <= 4)
	{
		wBitCount = 4;
	}
	else if (iBits <= 8)
	{
		wBitCount = 8;
	}
	else if (iBits <= 24)
	{
		wBitCount = 24;
	}
	else if (iBits <= 32)
	{
		wBitCount = 32;
	}  

	//计算调色板大小 

	if (wBitCount <= 8)
	{
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD)   ;
	} 

	//设置位图信息头结构 

	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap)   ;  
	bi.biSize = sizeof(BITMAPINFOHEADER);  
	bi.biWidth = Bitmap.bmWidth;  
	bi.biHeight = Bitmap.bmHeight;   
	bi.biPlanes = 1;  
	bi.biBitCount = wBitCount;     
	bi.biCompression = BI_RGB;  
	bi.biSizeImage = 0;  
	bi.biXPelsPerMeter = 0;  
	bi.biYPelsPerMeter = 0;  
	bi.biClrUsed = 0;  
	bi.biClrImportant = 0;   
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight   ;
	//为位图内容分配内存 
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER))   ;   
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib)   ;
	*lpbi = bi;  
	//   处理调色板  
	hPal = GetStockObject(DEFAULT_PALETTE);  
	if (hPal)
	{
		hDC = AApplication::Get()->GetMainForm()->GetWindow()->GetDC(); 
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE)   ;  
		RealizePalette(hDC)   ;
	} 

	//   获取该调色板下新的像素值 
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);   

	//恢复调色板   
	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
		RealizePalette(hDC)   ;   
		::ReleaseDC(NULL, hDC)   ;
	} 
	//创建位图文件
	fh = CreateFile(szfilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL)   ; 
	if (fh == INVALID_HANDLE_VALUE)
	{
		return   false;
	}    

	//   设置位图文件头 

	bmfHdr.bfType = 0x4D42;  //   "BM" 
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;    
	bmfHdr.bfSize = dwDIBSize;  
	bmfHdr.bfReserved1 = 0;  
	bmfHdr.bfReserved2 = 0;  
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;  
	//   写入位图文件头 
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);  
	//   写入位图文件其余内容 
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);  
	//消除内存分配     
	GlobalUnlock(hDib);   
	GlobalFree(hDib);  
	CloseHandle(fh);  
	return   true;
}
#endif
class CanvasBuff
{
public:
	CanvasBuff()
	{
		m_pData = NULL;
		m_nWidth = 0;
		m_nHeight = 0;
		m_pFontCacheMgr = NULL;
		m_pFontType = NULL;

	}
	~CanvasBuff()
	{
		if( m_pFontCacheMgr ) delete m_pFontCacheMgr;
		if( m_pFontType ) 
		{
			delete m_pFontType;
		}

		if( m_pData ) delete[] m_pData;
		m_pData = NULL;
	}


	void InitFontCache(HDC hDC,fe_type** fe,font_man_type** pCache)
	{
		if( hDC == NULL ) 
			return;//
		if( m_pFontCacheMgr == NULL )
		{
			m_pFontType = new fe_type(hDC);
			m_pFontCacheMgr = new font_man_type(*m_pFontType);
		}
		*fe = m_pFontType;
		*pCache = m_pFontCacheMgr;
	}

public:
	int	m_nWidth;
	int	m_nHeight;
	BYTE*	m_pData;
	fe_type*	m_pFontType;
	font_man_type*		m_pFontCacheMgr;

	APen		m_Pen;
	ASolidBrush	m_Brush;
	AFont		m_Font;
	AMatrixAgg	m_Matrix;
};
CanvasBuff g_CanvasBuf;

aggcolor ToAggColor(AColor cr)
{
	aggcolor c;
	c.r = cr.blue;//cr.red; fix me later : 这里为什么会需要颠倒一下顺序？
	c.g = cr.green;
	c.b = cr.red;//cr.blue;
	c.a = cr.alpha;
	return c;
}
ACanvasAgg::ACanvasAgg():ACanvas()
{
	m_hDC = NULL;
	m_SmoothingMode = SmoothingModeDefault;
	m_nWidth = 0;
	m_nHeight = 0;
	m_pBuff = NULL;
	m_pPen = NULL;
	m_pBrush = NULL;
	m_pFont = NULL;
	m_pMatrix = NULL;
	m_pBufCache = NULL;

	m_pPixFormat = NULL;
	m_pBaseRender = NULL;
	m_pSolidRender = NULL;
	m_pRAS = NULL;
	m_pScanLine = NULL;
	m_pFontEngineType = NULL;
	m_pFontCacheMgr = NULL;
}

ACanvasAgg::~ACanvasAgg()
{
	Free();
}

void ACanvasAgg::Create(HDC hDC,int nWidth,int nHeight)
{
	Free();
	Reset();
	//hDC为NULL时，不允许调用Write，只能用来计算 if( hDC == NULL ) hDC = g_CanvasBuf.GetDC();
	if( hDC == NULL )
		return;

	m_hDC = hDC;
	if( nWidth < 1 ) nWidth = 1;
	if( nHeight < 1 ) nHeight = 1;
	
	m_TextHint = thDefault;

	//m_pBuff = g_CanvasBuf.GetBuf(m_nWidth,nHeight);
	if( hDC )
	{
		m_pBufCache = g_MemCache.GetBuf(nWidth*nHeight*4);
		m_pBuff = m_pBufCache->GetData();
	}
	else 
	{
		nWidth = 1;
		nHeight = 1;
		m_pBuff = g_EmptyBuf;
	}
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_RenderBuff.attach(m_pBuff,nWidth,nHeight,-m_nWidth*4);

	//m_iTranslateX = 0;
	//m_iTranslateY = 0;

// 	ARect r;
// 	::GetClipBox(hDC,&r);
// 	m_ClipBox = r;
	//OutputDebugString(AString().Format(_T("\r\n()()()()()CanvasAgg Create w=%d h=%d"),nWidth,nHeight));
	m_ClipBox = ARect(0,0,nWidth,nHeight);

	m_pPixFormat = new pixformat_type(m_RenderBuff);
	m_pBaseRender = new base_renderer_type(*m_pPixFormat);
	//m_pBaseRender->clear(ToAggColor(AColor(0,255,255,255)));

	m_pSolidRender = new solid_renderer_type(*m_pBaseRender);
	m_pRAS = new agg::rasterizer_scanline_aa<>();
	m_pScanLine = new agg::scanline_p8();
	//m_pFontEngineType = new fe_type(m_hDC);
	//m_pFontCacheMgr = new font_man_type(*m_pFontEngineType);
	g_CanvasBuf.InitFontCache(m_hDC,&m_pFontEngineType,&m_pFontCacheMgr); 
}

void ACanvasAgg::Write(ARect rDest,ARect rSrc)
{
	if( m_hDC != NULL )
	{
		//OutputDebugString(AString().Format(_T("\r\n()()()()()CanvasAgg Write [%d,%d,%d,%d] - [%d,%d,%d,%d]"),rSrc.left,rSrc.top,rSrc.GetWidth(),rSrc.GetHeight(),rDest.left,rDest.top,rDest.GetWidth(),rDest.GetHeight()));
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
			memcpy(pBits, m_pBuff, m_nWidth * m_nHeight * 4);
#ifdef _DEBUG
// 			AString sBmpFile;
// 			static int i=1;
// 			sBmpFile.Format(_T("d:\\agg%d_(W%d H%d).bmp"),i++,m_nWidth,m_nHeight);
// 			SaveBitmapToFile(hBitmap,sBmpFile);
#endif
			HDC hMemDC=CreateCompatibleDC( m_hDC );
			if( hMemDC )
			{
				HBITMAP hbmOld=(HBITMAP)::SelectObject(hMemDC, hBitmap);
				BLENDFUNCTION bln;
				bln.BlendOp=AC_SRC_OVER;
				bln.BlendFlags=0;
				bln.SourceConstantAlpha=255;
				bln.AlphaFormat=AC_SRC_ALPHA;
				ARect r;
			 	::GetClipBox(m_hDC,&r);
				//r = m_ClipBox;
			 	AlphaBlend(//in Msimg32.lib
					m_hDC,
					r.left,r.top,r.GetWidth(),r.GetHeight(),
					hMemDC,
					r.left,r.top,r.GetWidth(),r.GetHeight(),
					bln);
// 				AlphaBlend(
// 					m_hDC, 
// 					rDest.left,
// 					rDest.top,
// 					rDest.GetWidth(),
// 					rDest.GetHeight(),
// 					hMemDC, 
// 					rSrc.left, 
// 					rSrc.top, 
// 					rSrc.GetWidth(), 
// 					rSrc.GetHeight(),
// 					bln);
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

bool ACanvasAgg::IsValid()
{
	return m_pBuff == NULL ? false : true;
}

void ACanvasAgg::Translate(int dx,int dy)
{
	//m_iTranslateX = dx;
	//m_iTranslateY = dy;
}

void ACanvasAgg::Free()
{
	if( m_pScanLine )
	{
		delete m_pScanLine;
		m_pScanLine = NULL;
	}
	if( m_pRAS )
	{
		delete m_pRAS;
		m_pRAS = NULL;
	}
	if( m_pSolidRender )
	{
		delete m_pSolidRender;
		m_pSolidRender = NULL;
	}
	if( m_pBaseRender )
	{
		delete m_pBaseRender;
		m_pBaseRender = NULL;
	}
	if( m_pPixFormat )
	{
		delete m_pPixFormat;
		m_pPixFormat = NULL;
	}
	m_pBuff = NULL;
	m_hDC = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	if( m_pBufCache )
	{
		g_MemCache.BackBuf(m_pBufCache );
		m_pBufCache = NULL;
	}
}

void ACanvasAgg::Reset()
{
	m_pPen = NULL;
	m_pBrush = NULL;
	m_pFont = NULL;
	m_pMatrix = NULL;
}

APath* ACanvasAgg::CreatePath()
{
	APathAgg* p = new APathAgg();
	return p;
}

void ACanvasAgg::DestroyPath(APath* p)
{
	APathAgg* x = dynamic_cast<APathAgg*>(p);
	if( x ) delete x;
}

AMatrix* ACanvasAgg::CreateMatrix()
{
	AMatrixAgg* p = new AMatrixAgg();
	return p;
}

void ACanvasAgg::DestroyMatrix(AMatrix *p)
{
	AMatrixAgg* x = dynamic_cast<AMatrixAgg *>(p);
	if( x ) delete x;
}

SmoothingMode ACanvasAgg::GetSmoothingMode()
{
	return m_SmoothingMode;
}

void ACanvasAgg::SetSmoothingMode(SmoothingMode smoothingMode)
{
	m_SmoothingMode = smoothingMode;
}

ACanvas* ACanvasAgg::Use(AFont* pFont)
{
	m_pFont = pFont;
	return this; 
}
ACanvas* ACanvasAgg::Use(APen* pPen)
{
	m_pPen = pPen;
	return this; 
}
ACanvas* ACanvasAgg::Use(ABrush* pBrush)
{
	m_pBrush = pBrush;
	return this; 
}
ACanvas* ACanvasAgg::Use(AMatrix* pMatrix)
{
	m_pMatrix = pMatrix;
	//m_pMatrix->Translate(m_iTranslateX,m_iTranslateY);
	return this;
}

AFont* ACanvasAgg::UseFont()
{
	if( m_pFont == NULL ) Use(&(g_CanvasBuf.m_Font));
	return m_pFont;
}

APen* ACanvasAgg::UsePen()
{
	if( m_pPen == NULL ) Use(&(g_CanvasBuf.m_Pen));
	return m_pPen;
}
ABrush* ACanvasAgg::UseBrush()
{
	if( m_pBrush == NULL ) Use(&(g_CanvasBuf.m_Brush));
	return m_pBrush;
}
AMatrix* ACanvasAgg::UseMatrix()
{
	if( m_pMatrix == NULL ) Use(&(g_CanvasBuf.m_Matrix));
	return m_pMatrix;
}

void ACanvasAgg::DrawPoint(APoint pt,AColor cr)
{
	aggcolor c = ToAggColor(cr);
	m_pBaseRender->copy_pixel(pt.x,pt.y,c);
}
void ACanvasAgg::DrawLine(int x1,int y1,int x2,int y2)
{
	UsePen();
	_clip(m_ClipBox);
	//m_pBaseRender->clip_box(m_ClipBox.left,m_ClipBox.top,m_ClipBox.right-1,m_ClipBox.bottom-1);
	if( m_pPen->GetStyle() == DashStyleSolid )
	{
		aggcolor c = ToAggColor(m_pPen->GetColor());
		/*agg::cover_type ct = agg::cover_full;
		int nPenWidth = (int)m_pPen->GetWidth();
		if( x1 == x2 )
		{
			x1 -= nPenWidth/2;
			for(int i= 0;i<nPenWidth;i++)
			{
				m_pBaseRender->blend_vline((int)x1,(int)y1,(int)y2,c,ct);
				x1++;
			}
		}
		else if( y1 == y2 )
		{
			y1 -= nPenWidth/2;
			for(int i= 0;i<nPenWidth;i++)
			{
				m_pBaseRender->blend_hline((int)x1,(int)y1,(int)x2,c,ct);
				y1++;
			}
		}
		else*/
		{
			APathAgg path;
			path.AddLine(APointF(x1,y1),APointF(x2,y2));
			path_conv_curve ccPath(path.m_agg_ps);
			path_conv_stroke csPath(ccPath);

			m_pSolidRender->color(c);
			csPath.width(m_pPen->GetWidth());

			m_pRAS->reset();
			m_pRAS->add_path(csPath);
		

			agg::render_scanlines(*m_pRAS, *m_pScanLine, *m_pSolidRender);
		}
	}
	Reset();
}

void ACanvasAgg::DrawLine(APoint pt1,APoint pt2)
{
	DrawLine(pt1.x,pt1.y,pt2.x,pt2.y);
}


void ACanvasAgg::DrawRect(int x1,int y1,int x2,int y2)
{
	DrawRect(ARect(x1,y1,x2,y2));
}


void ACanvasAgg::DrawRect(ARect r)
{
	if( !InClipBox(r) ) return;

	APathAgg path;
	path.AddRect(r);
	DrawPath(&path);
	return;
	
// 	m_pPen = g_CanvasBuf.GetPen(m_pPen);
// 
// 	m_pBaseRender->clip_box(m_ClipBox.left,m_ClipBox.top,m_ClipBox.right-1,m_ClipBox.bottom-1);
// 	if( m_pPen->GetStyle() == DashStyleSolid )
// 	{
// 		int nPenWidth = (int)m_pPen->GetWidth();
// 
// 		aggcolor c = ToAggColor(m_pPen->GetColor());
// 		int x1 = (int)r.left ,y1=(int)r.top,x2=(int)r.right-1,y2=(int)r.bottom-1;
// 		x1 -= nPenWidth/2;
// 		y1 -= nPenWidth/2;
// 		x2 -= nPenWidth/2;
// 		y2 -= nPenWidth/2;
// 
// 		agg::cover_type ct = agg::cover_full;
// 		
// 		for(int i= 0;i<nPenWidth;i++)
// 		{
// 			m_pBaseRender->blend_hline(x1,y1,x2,c,ct);
// 			m_pBaseRender->blend_hline(x1,y2,x2,c,ct);
// 			m_pBaseRender->blend_vline(x2,y1+1,y2-1,c,ct);
// 			m_pBaseRender->blend_vline(x1,y1+1,y2-1,c,ct);
// 			x1 ++;
// 			y1 ++;
// 			x2 --;
// 			y2--;
// 		}
// 	}
// 	Reset();
}

void ACanvasAgg::FillRect(int x1,int y1,int x2,int y2)
{
	FillRect(ARect(x1,y1,x2,y2));
}


void ACanvasAgg::FillRect(ARect r)
{
	if( !InClipBox(r) ) return;

	APathAgg path;
	path.Start();
	path.AddRect(r);
	//path.End();
	FillPath(&path);
}

void ACanvasAgg::DrawRoundRect(int x1,int y1,int x2,int y2,int round)
{
	DrawRoundRect(ARect(x1,y1,x2,y2),round);
}


void ACanvasAgg::DrawRoundRect(ARect r,int round)
{
	agg::rounded_rect rr(r.left,r.top,r.right,r.bottom,round);
	APathAgg path;
	path.m_agg_ps.join_path(rr);
	DrawPath(&path);
}

void ACanvasAgg::FillRoundRect(int x1,int y1,int x2,int y2,int round)
{
	FillRoundRect(ARect(x1,y1,x2,y2),round);
}

void ACanvasAgg::FillRoundRect(ARect r,int round)
{
	agg::rounded_rect rr(r.left,r.top,r.right,r.bottom,round);
	APathAgg path;
	path.m_agg_ps.join_path(rr);
	FillPath(&path);
}

void ACanvasAgg::DrawEllipse(int x1,int y1,int x2,int y2)
{
	DrawEllipse(ARect(x1,y1,x2,y2));
}

void ACanvasAgg::DrawEllipse(ARect r)
{
	REAL x,y,rx,ry;
	x = r.left+r.GetWidth()/2;
	y = r.top+r.GetHeight()/2;
	rx = r.GetWidth()/2;
	ry = r.GetHeight()/2;
	agg::ellipse ell(x,y,rx,ry);
	APathAgg path;
	path.m_agg_ps.join_path(ell);
	DrawPath(&path);
}

void ACanvasAgg::FillEllipse(int x1,int y1,int x2,int y2)
{
	FillEllipse(ARect(x1,y1,x2,y2));
}

void ACanvasAgg::FillEllipse(ARect r)
{
	REAL x,y,rx,ry;
	x = r.left+r.GetWidth()/2;
	y = r.top+r.GetHeight()/2;
	rx = r.GetWidth()/2;
	ry = r.GetHeight()/2;
	agg::ellipse ell(x,y,rx,ry);
	APathAgg path;
	path.m_agg_ps.join_path(ell);
	FillPath(&path);
}

void ACanvasAgg::DrawPolygon(APoint* pts,int count)
{
	APathAgg path;
	for(int i=0;i<count-1;i++)
	{
		path.AddLine(pts[i],pts[i+1]);
	}
	DrawPath(&path);
}


void ACanvasAgg::FillPolygon(APoint* pts,int count)
{
	APathAgg path;
	for(int i=0;i<count-1;i++)
	{
		path.AddLine(pts[i],pts[i+1]);
	}
	FillPath(&path);
}



void ACanvasAgg::DrawTrigle(APoint pt1,APoint pt2,APoint pt3)
{
	APathAgg path;
	triangle t(pt1,pt2,pt3);
	path.m_agg_ps.join_path(t);

	DrawPath(&path);
}
void ACanvasAgg::FillTrigle(APoint pt1,APoint pt2,APoint pt3)
{
	APathAgg path;

	triangle t(pt1,pt2,pt3);
	path.m_agg_ps.join_path(t);

	FillPath(&path);
}

void ACanvasAgg::DrawText(int x1,int y1,const AString& sText)
{
	// 字体引擎
	UseFont();
	_clip(m_ClipBox);
	//m_pBaseRender->clip_box(m_ClipBox.left,m_ClipBox.top,m_ClipBox.right-1,m_ClipBox.bottom-1);

	int nFontSize = m_pFont->GetSize();
	m_pFontEngineType->height(nFontSize);
	m_pFontEngineType->flip_y(true);
	if( (m_pFont->GetStyle() & FontStyleItalic) == FontStyleItalic )
		m_pFontEngineType->italic(true);
	else
		m_pFontEngineType->italic(false);
	if( (m_pFont->GetStyle() & FontStyleBold) == FontStyleBold )
		m_pFontEngineType->weight(FW_BOLD);
	else
		m_pFontEngineType->weight(FW_NORMAL);
	//fe.weight(13);
	m_pFontEngineType->width(0);//(m_width.value() == m_height.value()) ? 0.0 : m_width.value() / 2.4);
	m_pFontEngineType->hinting(m_SmoothingMode == SmoothingModeAntiAlias ? true : false);
	
	// 注意后面的glyph_rendering ren_type参数
	AString sFontName;
	sFontName = m_pFont->GetName();
	m_pFontEngineType->create_font(sFontName.ConvertTo(),agg::glyph_ren_agg_gray8);//agg::glyph_ren_outline
	// 字体串
	const wchar_t *s = sText.Text();


	// 存放字体数据
	std::vector<agg::int8u> data;
	// Rasterizer和Scanline
	ras_type ras_font;
	sl_type sl_font;
	// 字符输出的位置
	REAL x=(REAL)x1,y=(REAL)( nFontSize + y1);

	aggcolor fontColor = ToAggColor(m_pFont->GetColor());
	m_pSolidRender->color(fontColor);
	for(;*s;s++)
	{
		//取字模
		const agg::glyph_cache* glyph = m_pFontCacheMgr->glyph(*s);
		if(glyph)
		{
			// 初始化gray8_adaptor实例
			m_pFontCacheMgr->init_embedded_adaptors(glyph, x, y);

// 			// 先用conv_curve
// 			typedef agg::conv_curve<
// 				font_man_type::path_adaptor_type
// 				> 
// 					cc_pa_type;
// 			cc_pa_type ccpath(m_pFontCacheMgr->path_adaptor());
// 
// 			// 画轮廓
// 			//typedef agg::conv_stroke<cc_pa_type> cs_cc_pa_type;
// 			//cs_cc_pa_type csccpath(ccpath);
// 			m_pRAS->reset();
// 			m_pRAS->add_path(ccpath);//csccpath);
// 			agg::render_scanlines_aa_solid(*m_pRAS, *m_pScanLine, *m_pBaseRender, fontColor);

			
			agg::render_scanlines(m_pFontCacheMgr->gray8_adaptor(), 
				m_pFontCacheMgr->gray8_scanline(), 
				*m_pSolidRender);

// 			agg::render_scanlines_aa_solid(
// 				fman.gray8_adaptor(),
// 				fman.gray8_scanline(),
// 				_base_render, fontColor);
		
			// 前进
			x += (REAL)glyph->advance_x;
			y += (REAL)glyph->advance_y;
		}
	}
	Reset();
}


void ACanvasAgg::DrawText(int x1,int y1,const AString& sText,int iRowSpace)
{
	// 字体引擎
	UseFont();
	_clip(m_ClipBox);
	//m_pBaseRender->clip_box(m_ClipBox.left,m_ClipBox.top,m_ClipBox.right-1,m_ClipBox.bottom-1);

	int nFontSize = m_pFont->GetSize();
	m_pFontEngineType->height(nFontSize);
	m_pFontEngineType->flip_y(true);
	if( (m_pFont->GetStyle() & FontStyleItalic) == FontStyleItalic )
		m_pFontEngineType->italic(true);
	else
		m_pFontEngineType->italic(false);
	if( (m_pFont->GetStyle() & FontStyleBold) == FontStyleBold )
		m_pFontEngineType->weight(FW_BOLD);
	else
		m_pFontEngineType->weight(FW_NORMAL);
	//fe.weight(13);
	m_pFontEngineType->width(0);//(m_width.value() == m_height.value()) ? 0.0 : m_width.value() / 2.4);
	m_pFontEngineType->hinting(m_SmoothingMode == SmoothingModeAntiAlias ? true : false);
	// 注意后面的glyph_rendering ren_type参数
	AString sFontName;
	sFontName = m_pFont->GetName();
	m_pFontEngineType->create_font(sFontName.ConvertTo(),agg::glyph_ren_agg_gray8);//agg::glyph_ren_outline
	// 字体串
	const wchar_t *s = sText.Text();


	// 存放字体数据
	std::vector<agg::int8u> data;
	// Rasterizer和Scanline
	ras_type ras_font;
	sl_type sl_font;
	// 字符输出的位置
	REAL x=(REAL)x1,y=(REAL)( nFontSize + y1);

	aggcolor fontColor = ToAggColor(m_pFont->GetColor());
	m_pSolidRender->color(fontColor);
	for(;*s;s++)
	{
		if( *s == '\n' ) 
		{
			y += nFontSize + iRowSpace;
			x = 0;
			continue;
		}
		//if( *s == '\n' ) continue;
		//取字模
		const agg::glyph_cache* glyph = m_pFontCacheMgr->glyph(*s);
		if(glyph)
		{
			// 初始化gray8_adaptor实例
			m_pFontCacheMgr->init_embedded_adaptors(glyph, x, y);

// 			 			// 先用conv_curve
// 			 			typedef agg::conv_curve<
// 			 				font_man_type::path_adaptor_type
// 			 				> 
// 			 					cc_pa_type;
// 			 			cc_pa_type ccpath(m_pFontCacheMgr->path_adaptor());
// 			 
// 			 			// 画轮廓
// 			 			//typedef agg::conv_stroke<cc_pa_type> cs_cc_pa_type;
// 			 			//cs_cc_pa_type csccpath(ccpath);
// 			 			m_pRAS->reset();
// 			 			m_pRAS->add_path(ccpath);//csccpath);
// 			 			agg::render_scanlines_aa_solid(*m_pRAS, *m_pScanLine, *m_pBaseRender, fontColor);

			
			agg::render_scanlines(m_pFontCacheMgr->gray8_adaptor(), 
				m_pFontCacheMgr->gray8_scanline(), 
				*m_pSolidRender);

// 			 			agg::render_scanlines_aa_solid(
// 			 				fman.gray8_adaptor(),
// 			 				fman.gray8_scanline(),
// 			 				_base_render, fontColor);

			// 前进
			x += (REAL)glyph->advance_x;
			y += (REAL)glyph->advance_y;
		}
	}
	Reset();
}

void ACanvasAgg::DrawText(ARect r,const AString& sText,StringAlignment hAlign /*= StringAlignmentNear*/,StringAlignmentV vAlign /*=StringAlignmentMiddle*/)
{

	// 字体引擎类型定义
	
	AAutoClip autoClip(this,r);

	// 字体引擎

	UseFont();
	_clip(m_ClipBox);
	//m_pBaseRender->clip_box(m_ClipBox.left,m_ClipBox.top,m_ClipBox.right,m_ClipBox.bottom);

	int nFontSize = m_pFont->GetSize();
	m_pFontEngineType->height(nFontSize);
	m_pFontEngineType->flip_y(true);
	if( (m_pFont->GetStyle() & FontStyleItalic) == FontStyleItalic )
		m_pFontEngineType->italic(true);
	else
		m_pFontEngineType->italic(false);
	if( (m_pFont->GetStyle() & FontStyleBold) == FontStyleBold )
		m_pFontEngineType->weight(FW_BOLD);
	else
		m_pFontEngineType->weight(FW_NORMAL);
	//fe.weight(13);
	m_pFontEngineType->width(0);//(m_width.value() == m_height.value()) ? 0.0 : m_width.value() / 2.4);
	m_pFontEngineType->hinting(m_SmoothingMode == SmoothingModeAntiAlias ? true : false);
	// 注意后面的glyph_rendering ren_type参数
	AString sFontName;
	sFontName = m_pFont->GetName();
	m_pFontEngineType->create_font(sFontName.ConvertTo(),agg::glyph_ren_agg_gray8);//agg::glyph_ren_outline
	// 字体串
	const wchar_t *s = sText.Text();

	// 字符输出的位置
	REAL x=(REAL)0,y=(REAL)(0);


	aggcolor fontColor = ToAggColor(m_pFont->GetColor());
	int maxWidth=0;
	int iLineCount=1;
	int iRowSpace=6;
	int maxHeight=nFontSize;

	//先计算宽度
	for(;*s;s++)
	{
		//取字模
		const agg::glyph_cache* glyph = m_pFontCacheMgr->glyph(*s);
		if(glyph)
		{
			// 初始化gray8_adaptor实例
			m_pFontCacheMgr->init_embedded_adaptors(glyph, x, y);

			// 前进
			x += (REAL)glyph->advance_x;
			y += (REAL)glyph->advance_y;
			if( *s == '\n' ) 
			{
				if( maxWidth < x ) maxWidth = x;
				x =0;
				//if( *(s+1) ) //
					y += nFontSize+iRowSpace;
			}
		}
		
	}
	y += (REAL)(nFontSize);
	maxWidth = (int)x;
	maxHeight = y;
	int deltaX = ((int)r.GetWidth()-maxWidth)/2;
	int deltaY = (r.GetHeight()-maxHeight)/2;
	switch(hAlign)
	{
	default:
	case StringAlignmentNear:
		x = (REAL)r.left;
		break;
	case StringAlignmentCenter:
		x = r.left+deltaX;
		break;
	case StringAlignmentFar:
		x = r.right - maxWidth;
		break;
	
	}
	switch(vAlign)
	{
	default:
	case StringAlignmentTop:
		y = r.top;
		break;
	case StringAlignmentMiddle:
		y = r.top + deltaY;
		break;
	case StringAlignmentBottom:
		y = r.bottom-maxHeight;
		break;
	}
	
	//y += (REAL)( nFontSize * 5.0 / 6.0 );//修正y
	//再显示
	s = sText.Text();
	
	y += nFontSize;
	int dx = x;
	for(;*s;s++)
	{
		//取字模
		const agg::glyph_cache* glyph = m_pFontCacheMgr->glyph(*s);
		if(glyph)
		{
			// 初始化gray8_adaptor实例
			m_pFontCacheMgr->init_embedded_adaptors(glyph, dx, y);

// 			// 先用conv_curve
// 			typedef agg::conv_curve<font_man_type::path_adaptor_type>  cc_pa_type;
// 			cc_pa_type ccpath(m_pFontCacheMgr->path_adaptor());
// 
// 			// 画轮廓
// 			//typedef agg::conv_stroke<cc_pa_type> cs_cc_pa_type;
// 			//cs_cc_pa_type csccpath(ccpath);
// 
// 			m_pRAS->reset();
// 			m_pRAS->add_path(ccpath);//csccpath);
//  			agg::render_scanlines_aa_solid(*m_pRAS, *m_pScanLine, *m_pBaseRender, fontColor);

			// 			agg::render_scanlines_aa_solid(
			// 				fman.gray8_adaptor(),
			// 				fman.gray8_scanline(),
			// 				_base_render, fontColor);
			m_pSolidRender->color(fontColor);
			agg::render_scanlines(m_pFontCacheMgr->gray8_adaptor(), 
				m_pFontCacheMgr->gray8_scanline(), 
				*m_pSolidRender);
// 			agg::render_scanlines_aa_solid(m_pFontCacheMgr->gray8_adaptor(),
// 					                              m_pFontCacheMgr->gray8_scanline(),
// 					                              *m_pBaseRender, fontColor);

			// 前进
			dx += (REAL)glyph->advance_x;
			y += (REAL)glyph->advance_y;
			if( *s == '\n' )
			{
				y += nFontSize+iRowSpace;
				dx=x;
			}
		}
	}
	Reset();
}

ASize ACanvasAgg::MeasureText(const AString& sText)
{
	return _MeasureText(sText,true);
}
ASize ACanvasAgg::MeasureText(const AString& sText,int iRowSpace)
{
	return _MeasureText(sText,iRowSpace,true);
}
ASize ACanvasAgg::_MeasureText(const AString& sText,bool bReset)
{
	// 字体引擎
	UseFont();

	int nFontSize = m_pFont->GetSize();
	m_pFontEngineType->height(nFontSize);
	m_pFontEngineType->flip_y(true);
	if( (m_pFont->GetStyle() & FontStyleItalic) == FontStyleItalic )
		m_pFontEngineType->italic(true);
	m_pFontEngineType->width(0);
	m_pFontEngineType->hinting(m_SmoothingMode == SmoothingModeAntiAlias ? true : false);
	// 注意后面的glyph_rendering ren_type参数
	AString sFontName;
	sFontName = m_pFont->GetName();
	m_pFontEngineType->create_font(sFontName.ConvertTo(),agg::glyph_ren_agg_gray8);
	// 字体串
	const wchar_t *s = sText.Text();
	// 存放字体数据
	//std::vector<agg::int8u> data;
	//Rasterizer和Scanline
	//ras_type ras_font;
	//sl_type sl_font;
	// 字符输出的位置
	REAL x=0,y=0;


	aggcolor fontColor = ToAggColor(m_pFont->GetColor());
	for(;*s && *s != '\n';s++)
	{
		//取字模
		const agg::glyph_cache* glyph = m_pFontCacheMgr->glyph(*s);
		if(glyph)
		{
			// 初始化gray8_adaptor实例
			m_pFontCacheMgr->init_embedded_adaptors(glyph, x, y);

			// 前进
			x += (REAL)glyph->advance_x;
			y += (REAL)glyph->advance_y;
		}
	}
	ASize szRet((int)x,(int)nFontSize);
	if( bReset ) 
		Reset(); 
	return szRet;
}

ASize ACanvasAgg::_MeasureText(const AString& sText,int iRowSpace,bool bReset)
{
	// 字体引擎
	UseFont();

	int nFontSize = m_pFont->GetSize();
	m_pFontEngineType->height(nFontSize);
	m_pFontEngineType->flip_y(true);
	if( (m_pFont->GetStyle() & FontStyleItalic) == FontStyleItalic )
		m_pFontEngineType->italic(true);
	m_pFontEngineType->hinting(m_SmoothingMode == SmoothingModeAntiAlias ? true : false);
	// 注意后面的glyph_rendering ren_type参数
	AString sFontName;
	sFontName = m_pFont->GetName();
	m_pFontEngineType->create_font(sFontName.ConvertTo(),agg::glyph_ren_agg_gray8);
	// 字体串
	const wchar_t *s = sText.Text();
	// 存放字体数据
	//std::vector<agg::int8u> data;
	// Rasterizer和Scanline
	//ras_type ras_font;
	//sl_type sl_font;
	// 字符输出的位置
	REAL x=0,y=0;

	REAL maxX=0;
	aggcolor fontColor = ToAggColor(m_pFont->GetColor());
	for(;*s ;s++)
	{
		if( *s == '\n' ) 
		{
			y += iRowSpace+nFontSize;
			maxX = max(maxX,x);
			x = 0;
			continue;
		}

		//取字模
		const agg::glyph_cache* glyph = m_pFontCacheMgr->glyph(*s);
		if(glyph)
		{
			// 初始化gray8_adaptor实例
			m_pFontCacheMgr->init_embedded_adaptors(glyph, x, y);

			// 前进
			x += (REAL)glyph->advance_x;
			y += (REAL)glyph->advance_y;
		}
		
	}
	ASize szRet((int)x,(int)y);
	if( bReset )
		Reset(); 
	return szRet;
}
int ACanvasAgg::MeasurePosition(const AString& sText,int iPos)
{
	// 字体引擎

	UseFont();

	int nFontSize = m_pFont->GetSize();
	m_pFontEngineType->height(nFontSize);
	m_pFontEngineType->flip_y(true);
	if( (m_pFont->GetStyle() & FontStyleItalic) == FontStyleItalic )
		m_pFontEngineType->italic(true);
	m_pFontEngineType->hinting(m_SmoothingMode == SmoothingModeAntiAlias ? true : false);
	// 注意后面的glyph_rendering ren_type参数
	AString sFontName;
	sFontName = m_pFont->GetName();
	m_pFontEngineType->create_font(sFontName.ConvertTo(),agg::glyph_ren_agg_gray8);
	// 字体串
	const wchar_t *s = sText.Text();
	// 存放字体数据
	//std::vector<agg::int8u> data;
	// Rasterizer和Scanline
	//ras_type ras_font;
	//sl_type sl_font;
	// 字符输出的位置
	REAL x=0,y=0;


	aggcolor fontColor = ToAggColor(m_pFont->GetColor());
	int index=0;
	for(;*s && *s != '\n';s++)
	{
		//取字模
		const agg::glyph_cache* glyph = m_pFontCacheMgr->glyph(*s);
		if(glyph)
		{
			// 初始化gray8_adaptor实例
			m_pFontCacheMgr->init_embedded_adaptors(glyph, x, y);

			// 前进
			x += (REAL)glyph->advance_x;
			y += (REAL)glyph->advance_y;
			if( x >= iPos ) break;
		}
		index++;
	}
	return index;
}

void ACanvasAgg::_DoFillPathTextureClampSz(const AMatrixAgg &mImgMtx, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride)
{
	span_alloc_type span_allocator;                  // Span Allocator

	//m_pMatrix = g_CanvasBuf.GetMatrix(m_pMatrix,m_iTranslateX,m_iTranslateY);
	const AMatrixAgg* pMatrix = (AMatrixAgg *)m_pMatrix;

	typedef agg::pixfmt_rgba32     pixfmt;
	typedef agg::image_accessor_clip<pixfmt> img_source_type;
	typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
	typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
	agg::trans_affine mtx_Work(mImgMtx.m_agg_mtx);
	mtx_Work.multiply(pMatrix->m_agg_mtx);
	mtx_Work.invert();
	interpolator_type_linear interpolator(mtx_Work);
	{ 
		agg::rendering_buffer PatRendBuff((BYTE *)pImgBuff, dwImgWidth, dwImgHeight, nImgStride);
		pixfmt          img_pixf(PatRendBuff);
		img_source_type img_src(img_pixf, agg::rgba(0, 0.4, 0, 0.5));
		span_gen_type sg(img_src, interpolator);
		renderer_type ri(*m_pBaseRender, span_allocator, sg);
		agg::render_scanlines(*m_pRAS, *m_pScanLine, ri);
	}

// 	m_pMatrix = g_CanvasBuf.GetMatrix(m_pMatrix);
// 	AMatrixAgg* pMatrix = (AMatrixAgg *)m_pMatrix;
// 
// 	typedef agg::pixfmt_rgba32     pixfmt;
// 	typedef agg::image_accessor_clip<pixfmt> img_source_type;
}

void ACanvasAgg::DrawGraphic(AImage* pGraphic,int x,int y,int w/*=0*/,int h /*=0*/)
{
	if( pGraphic == NULL || pGraphic->IsValid() == false ) return;


	if( w == 0 ) w = pGraphic->GetWidth();
	if( h == 0 ) h = pGraphic->GetHeight();

	UseMatrix();
	AMatrixAgg* pMatrix = (AMatrixAgg *)m_pMatrix;
	//pMatrix->Translate(m_iTranslateX,m_iTranslateY);
	_clip(m_ClipBox);
	//m_pBaseRender->clip_box(m_ClipBox.left,m_ClipBox.top,m_ClipBox.right,m_ClipBox.bottom);

	m_pRAS->reset(); 

	agg::path_storage p2;
	p2.move_to(x, y);p2.line_to(x+w, y);p2.line_to(x+w, y+h);p2.line_to(x, y+h);p2.close_polygon();

	agg::conv_transform<agg::path_storage> trans(p2, pMatrix->m_agg_mtx);  // Global Affine transformer

	m_pRAS->add_path(trans);

	void *pImgBuff=pGraphic->GetPixels();
	DWORD dwImgWidth=pGraphic->GetWidth();
	DWORD dwImgHeight=pGraphic->GetHeight();

	AMatrixAgg ImgMtx;
	//ImgMtx.Translate(m_iTranslateX,m_iTranslateY);
	ImgMtx.Translate(x, y);
	ImgMtx.Scale(w/float(dwImgWidth), h/float(dwImgHeight));
	_DoFillPathTextureClampSz(ImgMtx, pImgBuff, dwImgWidth, dwImgHeight, pGraphic->GetStride());

}

void ACanvasAgg::DrawPath(APath* pPath)
{
	UsePen();
	UseBrush();
	UseMatrix();

	AMatrixAgg* pMatrix = (AMatrixAgg*)m_pMatrix;
	//pMatrix->Translate(m_iTranslateX,m_iTranslateY);
	_clip(m_ClipBox);
	//m_pBaseRender->clip_box(m_ClipBox.left,m_ClipBox.top,m_ClipBox.right,m_ClipBox.bottom);


	agg::line_join_e LineJoin=agg::miter_join;
// 	switch(pen->GetLineJoin())
// 	{
// 	case LineJoinMiter       : LineJoin=agg::miter_join; break;
// 	case LineJoinBevel       : LineJoin=agg::bevel_join; break;
// 	default:
// 	case LineJoinRound       : LineJoin=agg::round_join; break;
// 	case LineJoinMiterClipped: LineJoin=agg::miter_join_revert; break;
// 	}
	REAL fWidth=m_pPen->GetWidth();
	double dblMiterLimit=0.50;
	//what is miter_limit_theta ?

	agg::path_storage path_copy(((APathAgg *)pPath)->m_agg_ps);
	typedef agg::conv_curve<agg::path_storage> conv_crv_type;
	//typedef agg::path_storage conv_crv_type;
	conv_crv_type c_c_path(path_copy);


	if( m_pPen->GetStyle() == DashStyleSolid )
	{
		typedef agg::conv_stroke<conv_crv_type> Path_Conv_StrokeN;
		Path_Conv_StrokeN pgN(c_c_path);
		//typedef agg::conv_stroke<agg::path_storage> Path_Conv_StrokeN;
		//Path_Conv_StrokeN pgN(path->m_agg_ps);
		pgN.line_join(LineJoin);
		pgN.miter_limit(dblMiterLimit);
		pgN.width(fWidth);
		typedef agg::conv_transform<Path_Conv_StrokeN> transStroke;
		transStroke trans(pgN, pMatrix->m_agg_mtx);
		m_pRAS->reset();
		m_pRAS->add_path(trans);
	}
	else
	{
		typedef agg::conv_dash<conv_crv_type> Path_Conv_Dash;
		Path_Conv_Dash poly2_dash(c_c_path);
		//typedef agg::conv_dash<agg::path_storage> Path_Conv_Dash;
		//Path_Conv_Dash poly2_dash(path->m_agg_ps);
		typedef agg::conv_stroke<Path_Conv_Dash> Path_Conv_StrokeD;
		Path_Conv_StrokeD pgD(poly2_dash);

		switch( m_pPen->GetStyle() )
		{
		case DashStyleDash:
			poly2_dash.add_dash(3.00*fWidth, fWidth);
			break;
		case DashStyleDot:
			poly2_dash.add_dash(fWidth, fWidth);
			break;
		case DashStyleDashDot:
			poly2_dash.add_dash(3.00*fWidth, fWidth);
			poly2_dash.add_dash(fWidth, fWidth);
			break;
		case DashStyleDashDotDot:
			poly2_dash.add_dash(3.00*fWidth, fWidth);
			poly2_dash.add_dash(fWidth, fWidth);
			poly2_dash.add_dash(fWidth, fWidth);
			break;
		default:
		case DashStyleCustom:break;
		}
		pgD.line_join(LineJoin);
		pgD.miter_limit(dblMiterLimit);
		pgD.width(fWidth);

		agg::conv_transform<Path_Conv_StrokeD> trans(pgD, pMatrix->m_agg_mtx);
		m_pRAS->reset();
		m_pRAS->add_path(trans);
	}

	m_pSolidRender->color(ToAggColor(m_pPen->GetColor()));

	agg::render_scanlines(*m_pRAS,*m_pScanLine,*m_pSolidRender);

	Reset();
}
void ACanvasAgg::FillPath(APath* pPath)
{
	APathAgg* pp = dynamic_cast<APathAgg *>(pPath);
	if( pp == NULL ) return;

	UseBrush();
	UseMatrix();

	AMatrixAgg* pMatrix = (AMatrixAgg*)m_pMatrix;
	//pMatrix->Translate(m_iTranslateX,m_iTranslateY);
	_clip(m_ClipBox);
	//m_pBaseRender->clip_box(m_ClipBox.left,m_ClipBox.top,m_ClipBox.right,m_ClipBox.bottom);

	agg::path_storage p2(pp->m_agg_ps);
	typedef agg::conv_transform<agg::path_storage> trans_type;
	trans_type trans(p2, pMatrix->m_agg_mtx);

	typedef agg::conv_curve<trans_type> conv_crv_type;
	conv_crv_type c_c_path(trans);


	BrushType _brType = m_pBrush->GetType();
	if( _brType == BrushTypeSolidColor )
	{
		ASolidBrush* pBrush = dynamic_cast<ASolidBrush*>(m_pBrush);
	
		m_pSolidRender->color(ToAggColor(pBrush->GetColor()));

		m_pRAS->reset();
		m_pRAS->add_path(pp->m_agg_ps);
		

		agg::render_scanlines(*m_pRAS,*m_pScanLine,*m_pSolidRender);
	}
	else if( _brType == BrushTypeLinearGradient )
	{
		ALinearGradientBrush* pBrush = dynamic_cast<ALinearGradientBrush*>(m_pBrush);

		ARectF rBrush;
		rBrush = pBrush->GetRect();
		
		
		agg::trans_affine mtx_Work(pMatrix->m_agg_mtx);
		
		interpolator_type_linear span_interpolator(mtx_Work); // Span interpolator


		m_pRAS->reset();
		m_pRAS->add_path(pp->m_agg_ps);
		m_pRAS->close_polygon();



		agg::scanline_p8 sl;
		span_alloc_type span_allocator;                  // Span Allocator
		
		if( pBrush->IsVert() )
		{
			typedef agg::gradient_repeat_adaptor<agg::gradient_y> gr_adaptor;
			agg::gradient_y gradient_y;
			gr_adaptor gr_y(gradient_y);
			gr_adaptor* gr_ptr = &gr_y;

			typedef agg::span_gradient<
				agg::rgba8, 
				interpolator_type_linear,        
				gr_adaptor, 
				agg::gradient_linear_color<agg::rgba8> 
			>	
					gradient_span_gen;
			typedef agg::span_allocator<gradient_span_gen::color_type> gradient_span_alloc;
			gradient_span_alloc    span_alloc;
			agg::gradient_linear_color<agg::rgba8> colors(ToAggColor(pBrush->GetStartColor()),ToAggColor(pBrush->GetEndColor()));
			gradient_span_gen span_gen(span_interpolator, *gr_ptr, colors, rBrush.top,rBrush.bottom);
			typedef agg::renderer_scanline_aa<base_renderer_type, gradient_span_alloc, gradient_span_gen> renderer_gradient_type;
			// The gradient renderer
			pMatrix->Translate(rBrush.left,rBrush.top);
			mtx_Work.invert();
			renderer_gradient_type ren_gradient(*m_pBaseRender, span_alloc, span_gen);
			agg::render_scanlines(*m_pRAS, *m_pScanLine, ren_gradient);
		}
		else
		{
			typedef agg::gradient_repeat_adaptor<agg::gradient_x> gr_adaptor;
			agg::gradient_x gradient_x;
			gr_adaptor gr_x(gradient_x);
			gr_adaptor* gr_ptr = &gr_x;

			typedef agg::span_gradient<
				agg::rgba8, interpolator_type_linear,        
				gr_adaptor, 
				agg::gradient_linear_color<agg::rgba8> 
			>	gradient_span_gen;
			typedef agg::span_allocator<gradient_span_gen::color_type> gradient_span_alloc;
			gradient_span_alloc    span_alloc;
			agg::gradient_linear_color<agg::rgba8> colors(ToAggColor(pBrush->GetStartColor()),ToAggColor(pBrush->GetEndColor()));
			gradient_span_gen span_gen(span_interpolator, *gr_ptr, colors, rBrush.left,rBrush.right);
			typedef agg::renderer_scanline_aa<base_renderer_type, gradient_span_alloc, gradient_span_gen> renderer_gradient_type;
			// The gradient renderer
			pMatrix->Translate(rBrush.left,rBrush.top);
			mtx_Work.invert();
			renderer_gradient_type ren_gradient(*m_pBaseRender, span_alloc, span_gen);
			agg::render_scanlines(*m_pRAS,  *m_pScanLine, ren_gradient);
		}
		
		
	}
	else
	{
		throw AException(_T("不支持的填充刷子"));
	}
	Reset();
}

void ACanvasAgg::SetClipBox(ARect r)
{
	m_ClipBox = r;
}
ARect ACanvasAgg::GetClipBox()
{
	return m_ClipBox;
}

bool ACanvasAgg::InClipBox(const ARect& r)
{
	ARectF rx;
	rx = m_ClipBox;
	ARectF r2=r;
	rx.Intersect(r2);
	return rx.GetWidth() > 0 && rx.GetHeight() > 0 ? true : false;
}

void ACanvasAgg::_clip(ARect r)
{
	m_pBaseRender->clip_box(r.left,r.top,r.right,r.bottom);
}