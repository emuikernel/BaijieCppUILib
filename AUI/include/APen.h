#ifndef APEN_H
#define APEN_H


class AUI_API APen  : public AObject
{
	CLASS_INFO(Pen,Object);
public:
	APen();
	APen(REAL nWidth,AColor crColor,DashStyle nStyle);
	virtual ~APen();
public:
	void SetWidth(REAL nWidth);
	REAL GetWidth();

	void SetColor(AColor crColor);
	AColor GetColor();

	DashStyle GetStyle();
	void SetStyle(DashStyle nStyle);
protected:
	DashStyle	m_nStyle;
	REAL	m_nWidth;
	AColor	m_Color;

};


#endif//APEN_H
