
#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AFont::AFont():AObject()
{
	m_sName = _T("ËÎÌו");
	m_eStyle = FontStyleRegular;
	m_nSize = 12;
	m_Unit = UnitPoint;
	m_Color = AColor(255,0,0,0);
}

AFont::AFont(const AFont& f)
{
	m_sName = f.m_sName;
	m_nSize = f.m_nSize;
	m_eStyle = f.m_eStyle;
	m_Unit = f.m_Unit;
	m_Color = f.m_Color;
}
AFont::AFont(const AString& sName,int nSize,AColor cr,FontStyle nStyle/* = FontStyleRegular*/,Unit u/* =UnitPoint */)
{
	m_sName = sName;
	m_nSize = nSize;
	m_Color = cr;
	m_Unit = u;
	m_eStyle = nStyle;
}
AFont::~AFont()
{
}

AString AFont::GetName()
{
	return m_sName;
}

void	AFont::SetName(const AString& sName)
{
	m_sName		=	sName;
}

AColor AFont::GetColor()
{
	return m_Color;
}

void AFont::SetColor(AColor cr)
{
	m_Color		=	cr;
}

FontStyle AFont::GetStyle()
{
	return m_eStyle;
}
void AFont::SetStyle(FontStyle nStyle)
{
	m_eStyle = nStyle;
}

int AFont::GetSize()
{
	return m_nSize;
}
void AFont::SetSize(int nSize)
{
	m_nSize = nSize;
}

Unit AFont::GetUnit()
{
	return m_Unit;
}

void AFont::SetUnit(Unit u)
{
	m_Unit = u;
}


