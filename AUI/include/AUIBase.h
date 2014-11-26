#ifndef AUIBASE_H
#define AUIBASE_H

class AUI_API APoint : public tagPOINT
{
public:
	APoint();
	APoint(const POINT& src);
	APoint(int x, int y);
	APoint(LPARAM lParam);
};

class AUI_API ASize : public tagSIZE
{
public:
	ASize();
	ASize(const SIZE& src);
	ASize(const RECT rc);
	ASize(int cx, int cy);
};

class AUI_API ARect : public tagRECT
{
public:
	ARect();
	ARect(const RECT& src);
	ARect(int iLeft, int iTop, int iRight, int iBottom);

	int GetWidth() const;
	int GetHeight() const;
	ARect& Empty();
	ARect& Join(const RECT& rc);
	ARect& ResetOffset();
	ARect& Normalize();
	ARect& Offset(int cx, int cy);
	ARect& Inflate(int cx, int cy);
	ARect& Deflate(int cx, int cy);
	ARect& Deflate(int l,int t,int r,int b);
	ARect& Union(ARect& rc);
	ARect& Intersect(ARect& rc);
	bool PtInRect(APoint pt);
};

class AUI_API AColor
{
public:
	AColor():alpha(255),red(0),green(0),blue(0){}
	AColor(BYTE a,BYTE r,BYTE g,BYTE b):alpha(a),red(r),green(g),blue(b){}
	AColor(const AColor& b)
	{
		alpha = b.alpha;
		red = b.red;
		green = b.green;
		blue = b.blue;
	}
	AColor(COLORREF cr)
	{
		alpha = 0;
		red = GetRValue(cr);
		green = GetGValue(cr);
		blue = GetBValue(cr);
	}
	~AColor(){}
	operator COLORREF()
	{
		return RGB(red,green,blue);
	}

	BYTE alpha;
	BYTE red;
	BYTE green;
	BYTE blue;
};

class AUI_API AColorHSL
{
public:
	AColorHSL()
	{
		h = 180;
		s = 50;
		l = 50;
	}
	~AColorHSL(){}

	void FromRGB(const AColor& cr);
	AColor ToRGB();

	int h;
	int s;
	int l;
};


class AUI_API ASizeF
{
public:
	ASizeF();
	ASizeF(const ASizeF& src);
	ASizeF(const ASize& src);
	ASizeF(const RECT rc);
	ASizeF(REAL cx, REAL cy);
	operator ASize()
	{
		return ASize((int)cx,(int)cy);
	}
public:
	REAL	cx;
	REAL	cy;
};

class AUI_API APointF
{
public:
	APointF();
	APointF(const APoint& src);
	APointF(const APointF& src);
	APointF(REAL x, REAL y);
	APointF(LPARAM lParam);
	operator APoint()
	{
		return APoint((int)x,(int)y);
	}
public:
	REAL	x;
	REAL	y;
};

class AUI_API ARectF
{
public:
	ARectF();
	ARectF(const ARectF& src);
	ARectF(const ARect& src);
	ARectF(REAL left,REAL top,REAL right,REAL bottom);

	void Offset(REAL dx,REAL dy);
	void Inflate(REAL dx,REAL dy);
	bool Contains(const APointF& pt);
	bool Contains(const ARectF& r);
	REAL GetWidth() const;
	REAL GetHeight() const;
	void Normalize();
	ARectF& Intersect(ARectF& rc);
	ARectF& Union(ARectF& rc);
	operator ARect()
	{
		ARect r((int)left,(int)top,(int)right,(int)bottom);
		return r;
	}
public:
	REAL	left;
	REAL	top;
	REAL	right;
	REAL	bottom;
};

class AUI_API AWaitCursor
{
public:
	AWaitCursor();
	~AWaitCursor();

protected:
	HCURSOR m_hOrigCursor;
};

class AUI_API AAnchors
{
public:
	AAnchors();
	~AAnchors();

	bool		m_bLeft;
	bool		m_bTop;
	bool		m_bRight;
	bool		m_bBottom;
	int		m_iLeftDelta;
	int		m_iRightDelta;
	int		m_iTopDelta;
	int		m_iBottomDelta;
};

class AUI_API AFlagSet
{
public:
	AFlagSet();
	~AFlagSet();

	AFlagSet& operator <<(int iFlag);
	AFlagSet& operator >>(int iFlag);
	bool Has(int iFlag) const;
	void Clear();
private:
	int		m_aFlag[64];
	int		m_iCount;
};

class ACanvas;
class AUI_API AAutoClip
{
public:
	AAutoClip(ACanvas* cs,const ARect& r);
	~AAutoClip();
private:
	ACanvas* m_pCanvas;
	ARectF	m_rOld;
};

class AUI_API ACaretSeat
{
public:
	ACaretSeat()
	{
		iRow = 0;
		iCol = 0;
	}
	ACaretSeat(int r,int c)
	{
		iRow = r;
		iCol = c;
	}
	int	iRow;
	int	iCol;
};

#endif//AUIBASE_H