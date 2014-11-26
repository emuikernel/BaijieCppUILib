#pragma once

class AExpandButton : public AButton
{
public:
	CLASS_INFO(ExpandButton,Button);
	AExpandButton(AComponent* pOwner);
	virtual ~AExpandButton();
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
};

class APropItemControl : public AContainer
{
public:
	CLASS_INFO(PropItemControl,Container);
	APropItemControl(AComponent* pOwner);
	virtual ~APropItemControl(void);

public:
	APropItem*	m_pPropItem;
	AContainer*	m_pLeft;
	AContainer*	m_pRight;
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
};
