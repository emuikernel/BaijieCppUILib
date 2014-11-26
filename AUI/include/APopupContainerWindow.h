#ifndef APOPUPCONTAINERWINDOW_H
#define APOPUPCONTAINERWINDOW_H

class AUI_API APopupContainerWindow : public AWindow
{
	CLASS_INFO(PopupContainerWindow,Window);
public:
	APopupContainerWindow(AWinControlBase* pOwner);
	virtual ~APopupContainerWindow();

	virtual DWORD GetClassStyle(DWORD dwOldStyle);
	virtual DWORD GetExStyle(DWORD dwOldExStyle);

	virtual void OnLButtonDown(int X,int Y,AFlagSet& flag);

protected:

};


#endif//AWINDOWFORM_H
