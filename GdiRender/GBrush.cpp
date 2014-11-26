
#include "stdafx.h"
#include "GBrush.h"




GBrush::GBrush(HDC hDC):m_hDC(hDC)
{
	m_hBrush	=	NULL;
	m_hOldBrush	=	NULL;
	m_bDirty	=	FALSE;
	m_Color		=	AColor(0,0,0,0);
	m_nStyle	=	BS_SOLID;
}
GBrush::GBrush()
{
	m_hBrush	=	NULL;
	m_hOldBrush	=	NULL;
	m_hDC		=	NULL;
	m_bDirty	=	FALSE;
	m_Color		=	AColor(0,0,0,0);
	m_nStyle	=	BS_SOLID;
}

GBrush::~GBrush()
{
}


HBRUSH GBrush::GetHandle()
{
	return m_hBrush;
}
void GBrush::Do_Clean()
{
	if( m_hDC != NULL && m_hOldBrush != NULL ) 
	{
		::SelectObject(m_hDC,m_hOldBrush);
		::DeleteObject(m_hBrush);
	}
}

AColor GBrush::GetColor()
{
	GetInfor();
	return m_Color;
}

void GBrush::SetColor(AColor cl)
{
	GetInfor();
	m_Color		=	cl;
	m_bDirty	=	TRUE;
}

UINT GBrush::GetStyle()
{
	GetInfor();
	return m_nStyle;
}

void GBrush::SetStyle(UINT nStyle)
{
	GetInfor();
	m_nStyle	=	nStyle;
	m_bDirty	=	TRUE; 
}

void GBrush::GetInfor()
{
	if( m_hBrush != NULL ) return ;
	LOGBRUSH	lb;
	HBRUSH hBrush = (HBRUSH)::GetCurrentObject(m_hDC,OBJ_BRUSH);
	::GetObject(hBrush,sizeof(lb),&lb);
	m_nStyle	=	lb.lbStyle;
	m_Color		=	lb.lbColor;
	m_hBrush = ::CreateBrushIndirect(&lb);
	m_hOldBrush = (HBRUSH)::SelectObject(m_hDC,m_hBrush);
}

void GBrush::Flush()
{
	if( m_bDirty == TRUE )
	{
		m_bDirty	=	FALSE;
		GetInfor();
		if( m_hBrush != NULL )
		{
			if( m_hOldBrush != NULL  )
			{
				::SelectObject(m_hDC,m_hOldBrush);
				m_hOldBrush = NULL;
			}
			::DeleteObject(m_hBrush);
			m_hBrush = NULL;
			LOGBRUSH	lb;
			lb.lbColor	=	m_Color;
			lb.lbStyle	=	m_nStyle;
			m_hBrush	=	::CreateBrushIndirect(&lb);
			m_hOldBrush	=	(HBRUSH)::SelectObject(m_hDC,m_hBrush);
		}
	}
}
