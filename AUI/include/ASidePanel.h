#ifndef ASIDEPANEL_H
#define ASIDEPANEL_H

class AUI_API ASidePanel: public APanel
{
	CLASS_INFO(SidePanel,Panel);
public:
	ASidePanel(AComponent* pOwner);
	virtual ~ASidePanel();


	virtual void SetTextAlign(TTextAlign al);
	virtual TTextAlign GetTextAlign();

	virtual void DoLayout();

	virtual void SetColor(AColor cr);

	virtual void OnTitlePaint(AObject* p,AEvent* pEvent);
	virtual void OnArrowPaint(AObject* p,AEvent* pEvent);
	virtual APanel* GetClientContainer();

protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

	AContainer*	m_pTitleContainer;
	AButton*	m_pArrowButton;
	APanel*		m_pClientContainer;
	TTextAlign	m_TextAlign;

};
#endif//ASIDEPANEL_H
