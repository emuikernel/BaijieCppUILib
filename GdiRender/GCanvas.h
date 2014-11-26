
#ifndef GCANVAS_H
#define GCANVAS_H

class GCanvas : public AObject  
{
public:
	GCanvas(HDC hDC);
	virtual ~GCanvas();

public://attributes
	__declspec(property(get=GetPen)) GPen*	Pen;
	__declspec(property(get=GetBrush)) GBrush* Brush;
	__declspec(property(get=GetFont)) GCanvasFont* Font;
	__declspec(property(get=GetHandle)) HDC Handle;

	GPen*	GetPen();
	GBrush*	GetBrush();
	GCanvasFont*	GetFont();

	virtual GCanvas* operator ->();
	virtual operator HDC();
	virtual HDC GetHandle();
public:
	void Arc(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4);
	void DrawLine(int x1,int y1,int x2,int y2);
	void DrawRect(int x1,int y1,int x2,int y2);
	void DrawRect(const ARect& r);
	void FillRect(int x1,int y1,int x2,int y2);
	void FillRect(const ARect& r);
	void DrawText(int x1,int y1,const AString& sText);
	void DrawText(const ARect& r,const AString& sText,TTextAlign ta=taLeftMiddle);
	void DrawGraphic(AImage* pGraphic,int x,int y,int w=0,int h=0);
	void DrawTriple(APoint pt1,APoint pt2,APoint pt3);
	void RoundRect(const ARect& r,int dx,int dy);
	void Ellipse(const ARect& r);
	void Polygon(const APoint* pts,int iCount);
	void DrawPoint(APoint pt,AColor cr);
	void DrawRoundRect(ARect r,int dx,int dy);
	void FillRoundRect(ARect r,int dx,int dy);
private:	
	HDC		m_hDC;

	GBrush*		m_pBrush;
	GPen*		m_pPen;
	GCanvasFont*	m_pFont;
	void		Flush();
	void		Init(HDC hDC);
protected:
	 

};

#endif//GCANVAS_H

