#ifndef  ABRUSH_H
#define ABRUSH_H

class AUI_API ABrush : public AObject
{
	CLASS_INFO(Brush,Object);
public:
	ABrush();
	virtual ~ABrush();

	virtual BrushType GetType()=0;
};

class AUI_API ASolidBrush : public ABrush
{
	CLASS_INFO(SolidBrush,Brush);
public:
	ASolidBrush();
	ASolidBrush(AColor cr);
	virtual ~ASolidBrush();
public:
	void SetColor(AColor cl);
	AColor GetColor();

	virtual BrushType GetType();
protected:
	AColor			m_Color;
	
};

class AUI_API ALinearGradientBrush : public ABrush
{
	CLASS_INFO(LinearGradientBrush,Brush);
public:
	ALinearGradientBrush();
	ALinearGradientBrush(AColor crStart,AColor crEnd,ARectF r,bool bVert=false);
	virtual ~ALinearGradientBrush();
public:
	virtual BrushType GetType();

	AColor GetStartColor();
	void SetStartColor(AColor crStart);

	AColor GetEndColor();
	void SetEndColor(AColor crEnd);

	ARectF GetRect();
	void SetRect(ARectF r);

	bool IsVert();
	void IsVert(bool bVert);
protected:
	AColor		m_crStart;
	AColor		m_crEnd;
	ARectF		m_Rect;
	bool		m_bVert;//ÊÇ·ñ´¹Ö±½¥±ä
};

class AUI_API ARadialBrush : public ABrush
{
	CLASS_INFO(RadialBrush,Brush);
public:
	ARadialBrush();
	virtual ~ARadialBrush();
	virtual BrushType GetType();

	AColor GetStartColor();
	void SetStartColor(AColor crStart);

	AColor GetEndColor();
	void SetEndColor(AColor crEnd);

	APoint GetCenter();
	void SetCenter(APoint pt);

	int GetRadius();
	void SetRadius(int n);
protected:
	AColor		m_crStart;
	AColor		m_crEnd;
	APoint		m_ptCenter;
	int		m_nRadius;
};

#endif//ABRUSH_H


