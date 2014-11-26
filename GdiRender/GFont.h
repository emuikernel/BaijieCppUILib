#ifndef GFONT_H
#define GFONT_H

class GCanvasFont  : public AObject
{
	friend class GCanvas;
public:
	virtual ~GCanvasFont();

public://attributes
	__declspec(property(get=GetColor,put=SetColor)) AColor Color;
	__declspec(property(get=GetOrientation,put=SetOrientation)) long Orientation;
	__declspec(property(get=GetEscapement,put=SetEscapement)) long Escapement;
	__declspec(property(get=GetWidth,put=SetWidth))long Width;
	__declspec(property(get=GetHeight,put=SetHeight)) long Height;
	__declspec(property(get=GetBold,put=SetBold)) int Bold;
	__declspec(property(get=IsStrikeOut,put=SetStrikeOut)) bool StrikeOut;
	__declspec(property(get=IsUnderline,put=SetUnderline)) bool Underline;
	__declspec(property(get=IsItalic,put=SetItalic)) bool Italic;
	__declspec(property(get=GetCharset,put=SetCharset)) BYTE Charset;
	__declspec(property(get=GetName,put=SetName)) AString Name;
	__declspec(property(get=GetHandle)) HFONT Handle;
public:
	void Flush();
	
	AColor GetColor();
	void	SetColor(AColor cr);
	void SetOrientation(long lOri);
	long GetOrientation();
	void SetEscapement(long lEsc);
	long GetEscapement();
	void SetWidth(long lWidth);
	long GetWidth();
	void SetHeight(long lHeight);
	long GetHeight();
	void SetBold(int iBold);
	int GetBold();
	void SetStrikeOut(bool b);
	bool IsStrikeOut();
	void SetUnderline(bool b);
	bool IsUnderline();
	void SetItalic(bool b);
	bool IsItalic();
	void SetCharset(BYTE byChar);
	BYTE GetCharset();
	void SetName(LPCTSTR lpszName);
	virtual HFONT GetHandle();
	AString GetName();

protected:
	GCanvasFont(HDC hDC);
	GCanvasFont();//prepare for ACanvas
	void Do_Clean();

	HDC	m_hDC;
	HFONT	m_hFont;
	HFONT	m_hOldFont;

	AString	m_sName;
	BYTE	m_Charset;
	bool	m_bItalic;
	bool	m_bUnderline;
	bool	m_bStrikeOut;
	int	m_iBold;
	long	m_lHeight;
	long	m_lWidth;
	long	m_lEscapement;
	long	m_lOrientation;
	AColor	m_Color;
	BOOL	m_bDirty;
	BOOL	m_bColorDirty;
private:
	void GetInfor();
};

#endif//GFONT_H
