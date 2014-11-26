#ifndef ACOLORDIALOG_H
#define ACOLORDIALOG_H

class AUI_API AColorDialog: public AInvisibleControl
{
	CLASS_INFO(ColorDialog,InvisibleControl);
public:
	AColorDialog(AComponent* pOwner);
	virtual ~AColorDialog();

	virtual AColor GetColor();
	virtual void SetColor(AColor cr);
	virtual bool Execute();
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	AColor	m_Color;

};
#endif//ACOLORDIALOG_H
