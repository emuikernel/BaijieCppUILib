#ifndef MAXIMIZEBUTTON_H
#define MAXIMIZEBUTTON_H

class AUI_API AMaximizeButton : public AControl
{
	CLASS_INFO(MaximizeButton,Control);
public:
	AMaximizeButton(AComponent* pOwner);
	virtual ~AMaximizeButton();

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void DoClick(AEvent* pEvent);
	virtual void DoMouseDown(AEvent* pEvent);
	
};
#endif//MAXIMIZEBUTTON_H

