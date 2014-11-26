
#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

APen::APen():AObject()
{
	m_nWidth = 1;
	m_nStyle = DashStyleSolid;
	m_Color = AColor(255,0,0,0);
}
APen::APen(REAL nWidth,AColor crColor,DashStyle nStyle):AObject()
{
	m_nWidth = nWidth;
	m_Color = crColor;
	m_nStyle = nStyle;
}

APen::~APen()
{
}

void APen::SetStyle(DashStyle nStyle)
{
	m_nStyle = nStyle;
}

DashStyle APen::GetStyle()
{
	return m_nStyle;
}

AColor APen::GetColor()
{
	return m_Color;
}

void APen::SetColor(AColor crColor)
{
	m_Color		=	crColor;
}

REAL APen::GetWidth()
{
	return m_nWidth;
}

void APen::SetWidth(REAL nWidth)
{
	m_nWidth = nWidth;
}



