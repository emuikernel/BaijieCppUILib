#ifndef MINIMIZEBUTTON_H
#define MINIMIZEBUTTON_H

class AUI_API AMinimizeButton : public AControl
{
	CLASS_INFO(MinimizeButton,Control);
public:
	AMinimizeButton(AComponent* pOwner);
	virtual ~AMinimizeButton();

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void DoClick(AEvent* pEvent);
	virtual void DoMouseDown(AEvent* pEvent);
};
#endif//MINIMIZEBUTTON_H


