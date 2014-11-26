#ifndef APOPUPCONTAINER_H
#define APOPUPCONTAINER_H

class AUI_API APopupContainer: public AWinControlBase
{
	CLASS_INFO(PopupContainer,WinControlBase);
public:
	APopupContainer(AComponent* pOwner);
	virtual ~APopupContainer();

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void DoLostFocus(AEvent* pEvent);

	virtual void Popup(int X=0,int Y=0);
	virtual void ClosePopup();//È¡Ïûµ¯³ö
protected:
	virtual AWindow* CreateWnd();
	virtual AUICore* GetParentUICore();

};
#endif//ALABEL_H
