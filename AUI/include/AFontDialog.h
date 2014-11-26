#ifndef AFONTDIALOG_H
#define AFONTDIALOG_H

class AUI_API AFontDialog: public AInvisibleControl
{
	CLASS_INFO(FontDialog,InvisibleControl);
public:
	AFontDialog(AComponent* pOwner);
	virtual ~AFontDialog();

	AFont	SelectedFont;
	virtual bool Execute();
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

};
#endif//AFONTDIALOG_H
