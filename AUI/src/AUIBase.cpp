
#include "StdAfx.h"

ARect::ARect()
{
	left = top = right = bottom = 0;
}

ARect::ARect(const RECT& src)
{
	left = src.left;
	top = src.top;
	right = src.right;
	bottom = src.bottom;
}

ARect::ARect(int iLeft, int iTop, int iRight, int iBottom)
{
	left = iLeft;
	top = iTop;
	right = iRight;
	bottom = iBottom;
}

int ARect::GetWidth() const
{
	return right - left;
}

int ARect::GetHeight() const
{
	return bottom - top;
}

ARect& ARect::Empty()
{
	left = top = right = bottom = 0;
	return *this;
}

ARect& ARect::Join(const RECT& rc)
{
	if( rc.left < left ) left = rc.left;
	if( rc.top < top ) top = rc.top;
	if( rc.right > right ) right = rc.right;
	if( rc.bottom > bottom ) bottom = rc.bottom;
	return *this;
}

ARect& ARect::ResetOffset()
{
	::OffsetRect(this, -left, -top);
	return *this;
}

ARect& ARect::Normalize()
{
	if( left > right ) { int iTemp = left; left = right; right = iTemp; }
	if( top > bottom ) { int iTemp = top; top = bottom; bottom = iTemp; }
	return *this;
}

ARect& ARect::Offset(int cx, int cy)
{
	::OffsetRect(this, cx, cy);
	return *this;
}

ARect& ARect::Inflate(int cx, int cy)
{
	::InflateRect(this, cx, cy);
	return *this;
}

ARect& ARect::Deflate(int cx, int cy)
{
	//::InflateRect(this, -cx, -cy);
	left += cx;
	top += cy;
	right -= cx;
	bottom -= cy;
	return *this;
}

ARect& ARect::Deflate(int l,int t,int r,int b)
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
	return *this;
}
ARect& ARect::Union(ARect& rc)
{
	::UnionRect(this, this, &rc);
	return *this;
}

ARect& ARect::Intersect(ARect& rc)
{
	::IntersectRect(this,this,&rc);
	return *this;
}

bool ARect::PtInRect(APoint pt)
{
	if( pt.x >= left && pt.x <= right && pt.y >= top && pt.y <= bottom ) return true;
	else return false;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

ASize::ASize()
{
	cx = 0;
	cy = 0;
}

ASize::ASize(const SIZE& src)
{
	cx = src.cx;
	cy = src.cy;
}

ASize::ASize(const RECT rc)
{
	cx = rc.right - rc.left;
	cy = rc.bottom - rc.top;
}

ASize::ASize(int _cx, int _cy)
{
	cx = _cx;
	cy = _cy;
}

ASizeF::ASizeF()
{
	cx = 0;
	cy = 0;
}

ASizeF::ASizeF(const ASizeF& src)
{
	cx = src.cx;
	cy = src.cy;
}

ASizeF::ASizeF(const ASize& src)
{
	cx = (REAL)src.cx;
	cy = (REAL)src.cy;
}

ASizeF::ASizeF(const RECT rc)
{
	cx = (REAL)rc.right - (REAL)rc.left;
	cy = (REAL)rc.bottom - (REAL)rc.top;
}

ASizeF::ASizeF(REAL _cx,REAL _cy)
{
	cx = _cx;
	cy = _cy;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

APoint::APoint()
{
	x = 0;
	y = 0;
}

APoint::APoint(const POINT& src)
{
	x = src.x;
	y = src.y;
}

APoint::APoint(int _x, int _y)
{
	x = _x;
	y = _y;
}

APoint::APoint(LPARAM lParam)
{
	x = GET_X_LPARAM(lParam);
	y = GET_Y_LPARAM(lParam);
}


APointF::APointF()
{
	x = 0;
	y = 0;
}

APointF::APointF(const APoint& src)
{
	x = (REAL)src.x;
	y = (REAL)src.y;
}

APointF::APointF(const APointF& src)
{
	x = src.x;
	y = src.y;
}

APointF::APointF(REAL _x, REAL _y)
{
	x = _x;
	y = _y;
}

APointF::APointF(LPARAM lParam)
{
	x = (REAL)GET_X_LPARAM(lParam);
	y = (REAL)GET_Y_LPARAM(lParam);
}

ARectF::ARectF()
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}

ARectF::ARectF(const ARectF& src)
{
	left = src.left;
	top = src.top;
	right = src.right;
	bottom = src.bottom;
}

ARectF::ARectF(const ARect& src)
{
	left = (REAL)src.left;
	top = (REAL)src.top;
	right = (REAL)src.right;
	bottom = (REAL)src.bottom;
}

ARectF::ARectF(REAL _left,REAL _top,REAL _right,REAL _bottom)
{
	left = _left;
	top = _top;
	right = _right;
	bottom = _bottom;
}

void ARectF::Offset(REAL dx,REAL dy)
{
	left += dx;
	top += dy;
	right += dx;
	bottom += dy;
}

void ARectF::Inflate(REAL dx,REAL dy)
{
	left -= dx;
	top -= dy;
	right += dx;
	bottom += dy;
}

bool ARectF::Contains(const APointF& pt)
{
	if( pt.x >= left && pt.x < right && pt.y >= top && pt.y < bottom ) return true;
	else return false;
}

bool ARectF::Contains(const ARectF& r)
{
	if( r.left >= left && r.right <= right && r.top >= top && r.bottom <= bottom ) return true;
	else return false;
}

void ARectF::Normalize()
{
	REAL dx = right - left;
	if( dx < 0 ){ left += dx; right = left - dx; }
	REAL dy = bottom - top;
	if( dy < 0 ){ top += dy; bottom = top - dy; }
}

REAL ARectF::GetWidth() const
{
	return right - left;
}

REAL ARectF::GetHeight() const
{
	return bottom - top;
}

ARectF& ARectF::Intersect(ARectF& rc)
{
	left = max(left,rc.left);
	right = min(right,rc.right);
	top = max(top,rc.top);
	bottom = min(bottom,rc.bottom);
	if( right < left || bottom < top )
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	return *this;
}

ARectF& ARectF::Union(ARectF& rc)
{
	left = min(left,rc.left);
	right = max(right,rc.right);
	top = min(top,rc.top);
	bottom = max(bottom,rc.bottom);
	return *this;
}

//=================================== AAnchors =====================================
AAnchors::AAnchors()
{
	m_bLeft = true;
	m_bTop = true;
	m_bRight = false;
	m_bBottom = false;
	m_iLeftDelta = 0;
	m_iTopDelta = 0;
	m_iRightDelta = 0;
	m_iBottomDelta = 0;
}
AAnchors::~AAnchors()
{
	//
}


AFlagSet::AFlagSet()
{
	m_iCount = 0;
}
AFlagSet::~AFlagSet()
{

}

AFlagSet& AFlagSet::operator <<(int iFlag)
{
	if( Has(iFlag) ) return *this;

	if( m_iCount >= 64 ) throw AException(_T("[flagset]Out of bound"));
	m_aFlag[m_iCount] = iFlag;
	m_iCount++;
	return *this;
}
AFlagSet& AFlagSet::operator >>(int iFlag)
{
	for(int i=0;i<m_iCount;i++)
	{
		if( m_aFlag[i] == iFlag )
		{
			for(int k=i+1;k<m_iCount;k++) m_aFlag[k-1] = m_aFlag[k];
			m_iCount--;
			return *this;
		}
	}
	return *this;
}
bool AFlagSet::Has(int iFlag) const
{
	for(int i=0;i<m_iCount;i++)
	{
		if( m_aFlag[i] == iFlag ) return true;
	}
	return false;
}
void AFlagSet::Clear()
{
	m_iCount = 0;
}

AAutoClip::AAutoClip(ACanvas* cs,const ARect& r)
{
	ARectF r2  = cs->GetClipBox();
	m_rOld = r2;
	ARectF r1(r);
	r2.Intersect(r1);
	m_pCanvas = cs;
	cs->SetClipBox(r2);
}

AAutoClip::~AAutoClip()
{
	m_pCanvas->SetClipBox(m_rOld);
}


