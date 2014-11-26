#ifndef ACANVASAGG_H
#define ACANVASAGG_H

#include "agg_font_win32_tt.h"
class AMatrixAgg;

typedef agg::pixfmt_rgba32 pixformat_type;
typedef agg::renderer_base<pixformat_type> base_renderer_type;
typedef agg::renderer_scanline_aa_solid<base_renderer_type> solid_renderer_type;
typedef agg::renderer_scanline_bin_solid<base_renderer_type> solid_bin_renderer_type;
typedef agg::conv_curve<agg::path_storage> path_conv_curve;
typedef agg::conv_stroke<path_conv_curve> path_conv_stroke;
typedef agg::rgba8 aggcolor;
typedef agg::span_interpolator_linear<> interpolator_type_linear;
typedef agg::span_allocator<agg::rgba8> span_alloc_type;
typedef agg::font_engine_win32_tt_int16 fe_type;
typedef fe_type::gray8_adaptor_type ras_type;
typedef ras_type::embedded_scanline sl_type;
typedef agg::font_cache_manager<fe_type> font_man_type;
typedef fe_type::path_adaptor_type vs_type;

class ACanvasAgg : public ACanvas
{
	CLASS_INFO(CanvasAgg,Canvas);
public:
	ACanvasAgg();
	virtual ~ACanvasAgg();

public://attributes
	virtual void Create(HDC hDC,int nWidth,int nHeight);
	virtual void Write(ARect rDest,ARect rSrc);//输出到目的（屏幕、图片等）
	virtual bool IsValid();
	virtual void Free();

	virtual HDC GetDC(){return m_hDC; }//得到当前的DC
	virtual int GetWidth(){return m_nWidth; }//得到画布宽度
	virtual int GetHeight(){ return m_nHeight; }//得到画布高度

	virtual ACanvas* Use(AFont* pFont);
	virtual ACanvas* Use(APen* pPen);
	virtual ACanvas* Use(ABrush* pBrush);
	virtual ACanvas* Use(AMatrix* pMatrix);

	virtual APath* CreatePath();
	virtual void DestroyPath(APath* p);
	virtual AMatrix* CreateMatrix();
	virtual void DestroyMatrix(AMatrix *p);

	virtual SmoothingMode GetSmoothingMode();
	virtual void SetSmoothingMode(SmoothingMode smoothingMode);

	virtual TextHint GetTextHint(){ return m_TextHint; }
	virtual void SetTextHint(TextHint th){ m_TextHint = th; }

	virtual void Translate(int dx,int dy);

	virtual void DrawPoint(APoint pt,AColor cr);
	virtual void DrawLine(int x1,int y1,int x2,int y2);
	virtual void DrawLine(APoint pt1,APoint pt2);

	virtual void DrawRect(int x1,int y1,int x2,int y2);
	virtual void DrawRect(ARect r);

	virtual void FillRect(int x1,int y1,int x2,int y2);
	virtual void FillRect(ARect r);

	virtual void DrawRoundRect(int x1,int y1,int x2,int y2,int round);
	virtual void DrawRoundRect(ARect r,int round);

	virtual void FillRoundRect(int x1,int y1,int x2,int y2,int round);
	virtual void FillRoundRect(ARect r,int round);

	virtual void DrawEllipse(int x1,int y1,int x2,int y2);
	virtual void DrawEllipse(ARect r);

	virtual void FillEllipse(int x1,int y1,int x2,int y2);
	virtual void FillEllipse(ARect r);

	virtual void DrawPolygon(APoint* pts,int count);

	virtual void FillPolygon(APoint* pts,int count);

	virtual void DrawTrigle(APoint pt1,APoint pt2,APoint pt3);
	virtual void FillTrigle(APoint pt1,APoint pt2,APoint pt3);

	virtual void DrawText(int x1,int y1,const AString& sText);
	virtual void DrawText(int x1,int y1,const AString& sText,int iRowSpace);
	virtual void DrawText(ARect r,const AString& sText,StringAlignment hAlign = StringAlignmentNear,StringAlignmentV vAlign=StringAlignmentMiddle);
	virtual ASize MeasureText(const AString& sText);
	virtual ASize MeasureText(const AString& sText,int iRowSpace);//多行文本的计算
	virtual int MeasurePosition(const AString& sText,int iPos);//根据位置找到所在字符串的下标

	virtual void DrawGraphic(AImage* pGraphic,int x,int y,int w=0,int h=0);

	virtual void DrawPath(APath* pPath);
	virtual void FillPath(APath* pPath);

	virtual void SetClipBox(ARect r);
	virtual ARect GetClipBox();
protected:
	APen*			m_pPen;
	ABrush*			m_pBrush;
	AFont*			m_pFont;
	AMatrix*		m_pMatrix;

	AMemory*		m_pBufCache;

	SmoothingMode		m_SmoothingMode;
	TextHint		m_TextHint;
	HDC			m_hDC;
	int			m_nWidth;
	int			m_nHeight;
	BYTE*			m_pBuff;
	agg::rendering_buffer	m_RenderBuff;//agg的RenderBuff
	ARect			m_ClipBox;
	int			m_iTranslateX;
	int			m_iTranslateY;

	pixformat_type*		m_pPixFormat;
	base_renderer_type*	m_pBaseRender;
	solid_renderer_type*	m_pSolidRender;
	agg::rasterizer_scanline_aa<>* m_pRAS;
	agg::scanline_p8*	m_pScanLine;
	fe_type*		m_pFontEngineType;
	font_man_type*		m_pFontCacheMgr;

	AFont* UseFont();
	APen* UsePen();
	ABrush* UseBrush();
	AMatrix* UseMatrix();

	void Reset();
	bool InClipBox(const ARect& r);
	void _clip(ARect r);
	ASize _MeasureText(const AString& sText,bool bReset);
	ASize _MeasureText(const AString& sText,int iRowSpace,bool bReset);
	void _DoFillPathTextureClampSz(const AMatrixAgg &mImgMtx, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride);
};

#endif//ACANVASAGG_H