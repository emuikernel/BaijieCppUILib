#ifndef INVISIBLECONTROL_H
#define INVISIBLECONTROL_H

class AUI_API AInvisibleControl : public AControl
{
	CLASS_INFO(InvisibleControl,Control);
public:
	AInvisibleControl(AComponent* pOwner);
	virtual ~AInvisibleControl();
};

#endif//INVISIBLECONTROL_H