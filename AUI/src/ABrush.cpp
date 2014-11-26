
#include "stdafx.h"

ABrush::ABrush():AObject()
{
	//
}
ABrush::~ABrush()
{
	//
}
//==================== ASolidBrush ==========================
ASolidBrush::ASolidBrush():ABrush()
{
	m_Color = AColor(255,255,255,255);
}

ASolidBrush::ASolidBrush(AColor cr):ABrush()
{
	m_Color = cr;
}

ASolidBrush::~ASolidBrush()
{
}


AColor ASolidBrush::GetColor()
{
	return m_Color;
}

void ASolidBrush::SetColor(AColor cl)
{
	m_Color		=	cl;
}

BrushType ASolidBrush::GetType()
{
	return BrushTypeSolidColor;
}

//============================= ALinearGradientBrush ===================
ALinearGradientBrush::ALinearGradientBrush():ABrush()
{
	m_crStart = AColor(255,255,255,255);
	m_crEnd = AColor(255,255,255,255);
	m_bVert = false;
}

ALinearGradientBrush::ALinearGradientBrush(AColor crStart,AColor crEnd,ARectF r,bool bVert):ABrush()
{
	m_crStart = crStart;
	m_crEnd = crEnd;
	m_Rect = r;
	m_bVert = bVert;
}
ALinearGradientBrush::~ALinearGradientBrush()
{
	//
}

BrushType ALinearGradientBrush::GetType()
{
	return BrushTypeLinearGradient;
}

AColor ALinearGradientBrush::GetStartColor()
{
	return m_crStart;
}

void ALinearGradientBrush::SetStartColor(AColor crStart)
{
	m_crStart = crStart;
}

AColor ALinearGradientBrush::GetEndColor()
{
	return m_crEnd;
}

void ALinearGradientBrush::SetEndColor(AColor crEnd)
{
	m_crEnd = crEnd;
}

ARectF ALinearGradientBrush::GetRect()
{
	return m_Rect;
}

void ALinearGradientBrush::SetRect(ARectF r)
{
	m_Rect = r;
}

bool ALinearGradientBrush::IsVert()
{
	return m_bVert;
}
void ALinearGradientBrush::IsVert(bool bVert)
{
	m_bVert = bVert;
}
//================= RadialBrush =====================
ARadialBrush::ARadialBrush():base_class()
{
	m_crStart = AColor(255,0,0,0);
	m_crEnd = AColor(255,255,255,255);
	m_ptCenter = APoint(0,0);
	m_nRadius = 3;
}

ARadialBrush::~ARadialBrush()
{

}

BrushType ARadialBrush::GetType()
{
	return BrushTypeRadialGradient;
}

AColor ARadialBrush::GetStartColor()
{
	return m_crStart;
}

void ARadialBrush::SetStartColor(AColor crStart)
{
	m_crStart = crStart;
}

AColor ARadialBrush::GetEndColor()
{
	return m_crEnd;
}

void ARadialBrush::SetEndColor(AColor crEnd)
{
	m_crEnd = crEnd;
}

APoint ARadialBrush::GetCenter()
{
	return m_ptCenter;
}
void ARadialBrush::SetCenter(APoint pt)
{
	m_ptCenter = pt;
}

int ARadialBrush::GetRadius()
{
	return m_nRadius;
}
void ARadialBrush::SetRadius(int n)
{
	m_nRadius = n;
}