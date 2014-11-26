#ifndef GBRUSH_H
#define GBRUSH_H

class GBrush : AObject
{
	friend class GCanvas;
public:
	
	virtual ~GBrush();

public:
	void Flush();

	__declspec(property(get=GetStyle,put=SetStyle)) UINT Style;
	__declspec(property(get=GetColor,put=SetColor)) AColor Color;
	__declspec(property(get=GetHandle)) HBRUSH Handle;

	virtual HBRUSH GetHandle();

	void SetStyle(UINT nStyle);
	UINT GetStyle();
	void SetColor(AColor cl);
	AColor GetColor();

protected:
	GBrush(HDC hDC);
	GBrush();//prepare for ACanvas
	void Do_Clean();
	HDC		m_hDC;
	HBRUSH		m_hBrush;
	HBRUSH		m_hOldBrush;

	AColor	m_Color;
	UINT		m_nStyle;

	BOOL		m_bDirty;
private:
	void GetInfor();
};

#endif//GBRUSH_H


