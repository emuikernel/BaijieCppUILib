
#ifndef ACANVAS_H
#define ACANVAS_H

class ACanvas;
typedef ACanvas* (*FOnCreateCanvas)();

class AUI_API ACanvas : public AObject
{
	CLASS_INFO(Canvas,Object);
public:
	ACanvas();
	virtual ~ACanvas();
public://attributes
	virtual bool IsValid()=0;
	virtual void Create(HDC hDC,int nWidth,int nHeight)=0;
	virtual void Write(ARect rDest,ARect rSrc)=0;//输出到目的（屏幕、图片等）

	static void RegCanvasFactory(FOnCreateCanvas f);
	static ACanvas* CreateCanvas();

	virtual HDC GetDC()=0;//得到当前的DC
	virtual int GetWidth()=0;//得到画布宽度
	virtual int GetHeight()=0;//得到画布高度

	virtual ACanvas* Use(AFont* pFont)=0;
	virtual ACanvas* Use(APen* pPen)=0;
	virtual ACanvas* Use(ABrush* pBrush)=0;
	virtual ACanvas* Use(AMatrix* pMatrix)=0;

	virtual APath* CreatePath()=0;
	virtual void DestroyPath(APath* p)=0;
	virtual AMatrix* CreateMatrix()=0;
	virtual void DestroyMatrix(AMatrix *p)=0;

	virtual SmoothingMode GetSmoothingMode()=0;
	virtual void SetSmoothingMode(SmoothingMode smoothingMode)=0;

	virtual TextHint GetTextHint()=0;
	virtual void SetTextHint(TextHint th)=0;

	virtual void Translate(int dx,int dy)=0;

	virtual void DrawPoint(APoint pt,AColor cr)=0;

	virtual void DrawLine(int x1,int y1,int x2,int y2)=0;
	virtual void DrawLine(APoint pt1,APoint pt2)=0;

	virtual void DrawRect(int x1,int y1,int x2,int y2)=0;
	virtual void DrawRect(ARect r)=0;

	virtual void FillRect(int x1,int y1,int x2,int y2)=0;
	virtual void FillRect(ARect r)=0;

	virtual void DrawRoundRect(int x1,int y1,int x2,int y2,int round)=0;
	virtual void DrawRoundRect(ARect r,int round)=0;

	virtual void FillRoundRect(int x1,int y1,int x2,int y2,int round)=0;
	virtual void FillRoundRect(ARect r,int round)=0;

	virtual void DrawEllipse(int x1,int y1,int x2,int y2)=0;
	virtual void DrawEllipse(ARect r)=0;

	virtual void FillEllipse(int x1,int y1,int x2,int y2)=0;
	virtual void FillEllipse(ARect r)=0;

	virtual void DrawPolygon(APoint* pts,int count)=0;
	virtual void FillPolygon(APoint* pts,int count)=0;


	virtual void DrawTrigle(APoint pt1,APoint pt2,APoint pt3)=0;
	virtual void FillTrigle(APoint pt1,APoint pt2,APoint pt3)=0;

	virtual void DrawText(int x1,int y1,const AString& sText)=0;
	virtual void DrawText(int x1,int y1,const AString& sText,int iRowSpace)=0;
	virtual void DrawText(ARect r,const AString& sText,StringAlignment hAlign = StringAlignmentNear,StringAlignmentV vAlign=StringAlignmentMiddle)=0;
	virtual ASize MeasureText(const AString& sText)=0;
	virtual ASize MeasureText(const AString& sText,int iRowSpace)=0;//多行文本的计算
	virtual int MeasurePosition(const AString& sText,int iPos)=0;

	virtual void DrawGraphic(AImage* pGraphic,int x,int y,int w=0,int h=0)=0;

	virtual void DrawPath(APath* pPath)=0;
	virtual void FillPath(APath* pPath)=0;

	virtual void SetClipBox(ARect r)=0;
	virtual ARect GetClipBox()=0;

};

class AutoPath
{
public:
	AutoPath(ACanvas* cs) : m_pCanvas(cs)
	{
		m_pPath = m_pCanvas->CreatePath();
	}
	~AutoPath()
	{
		m_pCanvas->DestroyPath(m_pPath);
	}
	APath* get(){ return m_pPath; }
	APath* operator ->(){ return m_pPath; }
	APath* operator &(){ return m_pPath; }
private:
	APath*		m_pPath;
	ACanvas*	m_pCanvas;
};

class AutoMatrix
{
public:
	AutoMatrix(ACanvas* cs) : m_pCanvas(cs)
	{
		m_pMatrix = m_pCanvas->CreateMatrix();
	}
	~AutoMatrix()
	{
		m_pCanvas->DestroyMatrix(m_pMatrix);
	}
	AMatrix* get(){ return m_pMatrix; }
	AMatrix* operator ->(){ return m_pMatrix; }
	AMatrix* operator &(){ return m_pMatrix; }
private:
	AMatrix*	m_pMatrix;
	ACanvas*	m_pCanvas;
};

#endif//ACANVAS_H

