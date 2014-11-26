#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

class AUI_API ACloseButton : public AControl
{
	CLASS_INFO(CloseButton,Control);
public:
	ACloseButton(AComponent* pOwner);
	virtual ~ACloseButton();

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void DoMouseDown(AEvent* pEvent);
	virtual void DoClick(AEvent* pEvent);
};
#endif//CLOSEBUTTON_H

