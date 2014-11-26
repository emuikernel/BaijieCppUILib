#ifndef APANEL_H
#define APANEL_H

class AUI_API APanel: public AContainer
{
	CLASS_INFO(Panel,Container);
public:
	APanel(AComponent* pOwner);
	virtual ~APanel();

protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

};
#endif//APANEL_H
