#ifndef APAGESHEET_H
#define APAGESHEET_H

class AUI_API APageSheet: public AContainer
{
	CLASS_INFO(PageSheet,Container);
public:
	friend class APageControl;

	APageSheet(AComponent* pOwner);
	virtual ~APageSheet();

	AContainer* GetTabButton(){ return m_pTabButton; }
	ALabel* GetButtonText(){ return m_pButtonText; }

	virtual void SetText(AString s);

	virtual void OnButtonClick(AObject* p,AEvent* pEvent);

protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

	AContainer*	m_pTabButton;
	ALabel*		m_pButtonText;

};
#endif//APAGESHEET_H
