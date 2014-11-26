#ifndef AFONT_H
#define AFONT_H


class AUI_API AFont  : public AObject
{
	CLASS_INFO(Font,Object);
public:
	AFont();
	AFont(const AFont& f);
	AFont(const AString& sName,int nSize,AColor cr,FontStyle nStyle = FontStyleRegular,Unit u=UnitPoint);
	virtual ~AFont();
public:
	AColor GetColor();
	void	SetColor(AColor cr);

	FontStyle GetStyle();
	void SetStyle(FontStyle nStyle);

	int GetSize();
	void SetSize(int nSize);

	void SetName(const AString& sName);
	AString GetName();

	Unit GetUnit();
	void SetUnit(Unit u);

protected:
	FontStyle	m_eStyle;
	int		m_nSize;

	AString	m_sName;
	AColor	m_Color;

	Unit		m_Unit;
};

#endif//AFONT_H
