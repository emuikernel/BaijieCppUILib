#include "StdAfx.h"

ASlide::ASlide(AComponent* pOwner):base_class(pOwner)
{
	m_bShowButton = false;
	m_bShowTop = false;
}

ASlide::~ASlide()
{

}

int ASlide::GetRadius()
{
	return 11;
}

void ASlide::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawSlide(cs,this);
}


int ASlide::GetVirtualSize()
{
	return m_iRange;
}

int ASlide::GetTrueSize()
{
	if( m_Kind == sbVert ) return GetHeight() ;
	else return GetWidth() ;
}

double ASlide::GetRate()
{
	if( GetVirtualSize() == 0 ) return 1.0;
	return (double)GetRailLen() / (double)GetVirtualSize();
}

ARect ASlide::GetButton1Pos()
{
	ARect ret(0,0,0,0);
	return ret;
}
ARect ASlide::GetButton2Pos()
{
	ARect ret(0,0,0,0);
	return ret;
}

ARect ASlide::GetTopButton1Pos()
{
	ARect ret(0,0,0,0);
	
	return ret;
}

ARect ASlide::GetTopButton2Pos()
{
	ARect ret(0,0,0,0);
	
	return ret;
}

ARect ASlide::GetRailPos()
{
	ARect r = GetAbsPosition();
	if( m_Kind == sbVert ) r.bottom -= GetRadius()*2;
	else r.right -= GetRadius()*2;
	return r;
}

int ASlide::GetRailLen()
{
	ARect r = GetRailPos();

	if( m_Kind == sbVert ) return r.GetHeight();
	else return r.GetWidth();

}

ARect ASlide::GetRailPos_1()
{
	ARect r = GetRailPos();
	int n = m_iPosition * GetRate();
	if( m_Kind == sbVert ) return ARect(r.left,r.top,r.right,r.top+n);
	else return ARect(r.left,r.top,r.left+n,r.bottom);
}



ARect ASlide::GetRailPos_2()
{
	ARect r = GetRailPos();
	int D = GetRadius()*2;
	int n = m_iPosition * GetRate();
	if( m_Kind == sbVert ) return ARect(r.left,r.top+n+D,r.right,r.bottom);
	else return ARect(r.left+n+D,r.top,r.right,r.bottom);
}
ARect ASlide::GetSlideButtonPos()
{
	ARect r = GetRailPos();
	int n = m_iPosition * GetRate();
	int D = GetRadius()*2;
	if( D < AApplication::Get()->GetCurSkin()->m_ScrollBarWidth )
		D = AApplication::Get()->GetCurSkin()->m_ScrollBarWidth;
	if( m_Kind == sbVert ) 
	{
		if( D > r.GetHeight() ) D = r.GetHeight();
		return ARect(r.left,r.top+n,r.right,r.top+n+D);
	}
	else
	{
		if( D > r.GetWidth() ) D = r.GetWidth();
		return ARect(r.left+n,r.top,r.left+n+D,r.bottom);
	}
}

////////////////////////////////

AVertSlide::AVertSlide(AComponent* pOwner):base_class(pOwner)
{
	m_Kind = sbVert;
}

AVertSlide::~AVertSlide()
{

}

AHorzSlide::AHorzSlide(AComponent* pOwner):base_class(pOwner)
{
	m_Kind = sbHorz;
}

AHorzSlide::~AHorzSlide()
{

}





