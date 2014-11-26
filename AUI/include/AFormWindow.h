#ifndef AFORMWINDOW_H
#define AFORMWINDOW_H

class AUI_API AFormWindow : public AWindow
{
	CLASS_INFO(FormWindow,Window);
public:
	AFormWindow(AWinControlBase* pOwner);
	virtual ~AFormWindow();

	virtual DWORD GetClassStyle(DWORD dwOldStyle);
	virtual DWORD GetExStyle(DWORD dwOldExStyle);
protected:

};


#endif//AFORMWINDOW_H