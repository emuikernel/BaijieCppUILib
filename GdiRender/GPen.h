#ifndef GPEN_H
#define GPEN_H



class GPen  : public AObject
{
	friend class GCanvas;
public:
	
	virtual ~GPen();


public://attributes
	__declspec(property(get=GetMode,put=SetMode)) int Mode;
	__declspec(property(get=GetWidth,put=SetWidth)) long Width;
	__declspec(property(get=GetColor,put=SetColor)) AColor Color;
	__declspec(property(get=GetStyle,put=SetStyle)) UINT Style;
	__declspec(property(get=GetHandle)) HPEN Handle;
public:
	void Flush();
	
	void SetMode(int iMode);
	int GetMode();
	void SetWidth(long lWidth);
	long GetWidth();
	void SetColor(AColor crColor);
	AColor GetColor();
	UINT GetStyle();
	void SetStyle(UINT nStyle);

	virtual HPEN GetHandle();

protected:
	GPen(HDC hDC);
	GPen(); //prepare for ACanvas
	void Do_Clean();

	HDC	m_hDC;
	HPEN	m_hOldPen;
	HPEN	m_hPen;

	UINT	m_nStyle;
	int	m_iMode;
	long	m_lWidth;
	AColor	m_Color;
	BOOL	m_bDirty;
	BOOL	m_bModeDirty;
private:
	void GetInfor();
};


#endif//GPEN_H
