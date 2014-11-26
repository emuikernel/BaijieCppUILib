
#include "stdafx.h"
#include "GPen.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GPen::GPen(HDC hDC)
	:m_hDC(hDC),m_hOldPen(NULL),m_hPen(NULL)
{
	m_bDirty	=	FALSE;
	m_Color		=	RGB(0,0,0);
	m_iMode		=	R2_COPYPEN;
	m_lWidth	=	1;
	m_nStyle	=	PS_SOLID;
	m_bModeDirty	=	FALSE;
}
GPen::GPen()
{
	m_hDC	=	NULL;
	m_hOldPen	=	NULL;
	m_hPen		=	NULL;
	m_bDirty	=	FALSE;
	m_Color		=	RGB(0,0,0);
	m_iMode		=	R2_COPYPEN;
	m_lWidth	=	1;
	m_nStyle	=	PS_SOLID;
	m_bModeDirty	=	FALSE;
}
GPen::~GPen()
{
}



HPEN GPen::GetHandle()
{
	return m_hPen;
}

void GPen::Do_Clean()
{
	if( m_hDC != NULL && m_hOldPen != NULL )
	{
		::SelectObject( m_hDC,m_hOldPen );
		::DeleteObject(m_hPen);
	}
}

void GPen::SetStyle(UINT nStyle)
{
	GetInfor();
	m_nStyle = nStyle;
	m_bDirty	=	TRUE;
}

UINT GPen::GetStyle()
{
	GetInfor();
	return m_nStyle;
}

AColor GPen::GetColor()
{
	GetInfor();
	return m_Color;
}

void GPen::SetColor(AColor crColor)
{
	GetInfor();
	m_Color		=	crColor;
	m_bDirty	=	TRUE;
}

long GPen::GetWidth()
{
	GetInfor();
	return m_lWidth;
}

void GPen::SetWidth(long lWidth)
{
	GetInfor();
	m_lWidth = lWidth;
	m_bDirty	=	TRUE;
}

int GPen::GetMode()
{
	GetInfor();
	return m_iMode;
}

void GPen::SetMode(int iMode)
{
	GetInfor();
	m_iMode	=	iMode;
	m_bDirty	=	TRUE;
	m_bModeDirty	=	TRUE;
}

void GPen::GetInfor( )
{
	//why not init this values in constructor ?
	//because m_pDC->m_hDC is NULL when construct it
	if( m_hPen != NULL ) return;

	HPEN  hPen = (HPEN)::GetCurrentObject(m_hDC,OBJ_PEN);
	LOGPEN lp;
	::GetObject(hPen,sizeof(lp),&lp);
	m_nStyle	=	lp.lopnStyle;
	m_iMode		=	R2_COPYPEN;//fix me later,hard code ?
	if( lp.lopnWidth.x == 0 ) m_lWidth	=	1;
	else	m_lWidth	=	lp.lopnWidth.x;
	m_Color		=	lp.lopnColor;

	m_hPen = (HPEN)::CreatePen(m_nStyle,m_lWidth,m_Color);

	m_hOldPen = (HPEN)::SelectObject(m_hDC,m_hPen);
}

void GPen::Flush()
{
	if( m_bDirty == TRUE )
	{
		m_bDirty	=	FALSE;
		GetInfor();
		if( m_hPen != NULL )
		{
			if( m_hOldPen != NULL  )
			{
				::SelectObject(m_hDC,m_hOldPen);
				m_hOldPen = NULL;
			}
			::DeleteObject(m_hPen);
			m_hPen = NULL;

			LOGPEN lp;

			lp.lopnColor = m_Color;
			lp.lopnStyle = m_nStyle;
			lp.lopnWidth.x  = m_lWidth;
			if( (m_hPen =::CreatePenIndirect( &lp ) ) != FALSE )
			{
				m_hOldPen	=	(HPEN)::SelectObject(m_hDC,m_hPen);
				if( m_bModeDirty ) 
				{
					//fix me later ? m_pDC->SetROP2( m_iMode );
					m_bModeDirty = FALSE;
				}
			}			
		}
	}
}

