#include "StdAfx.h"

ASidePanel::ASidePanel(AComponent* pOwner):APanel(pOwner)
{
	m_pTitleContainer = new AContainer(this);
	m_pArrowButton = new AButton(m_pTitleContainer);
	m_pClientContainer = new APanel(this);
	//m_pClientContainer->SetBorderWidth(0);

	m_TextAlign = taLeftMiddle;

	m_pTitleContainer->SetAlign(alTop);
	m_pTitleContainer->SetHeight(20);
	//m_pTitleContainer->SetBorderWidth(0);
	m_pTitleContainer->OnPaint = AEventHandler(this,(FEventHandler)&ASidePanel::OnTitlePaint);

	m_pClientContainer->SetAlign(alClient);

	m_pArrowButton->SetAlign(alRight);
	m_pArrowButton->SetWidth(16);
	m_pArrowButton->OnPaint = AEventHandler(this,(FEventHandler)&ASidePanel::OnArrowPaint);
}

ASidePanel::~ASidePanel()
{

}

void ASidePanel::SetColor(AColor cr)
{
	m_Color = cr;
	m_pClientContainer->SetColor(cr);
}
void ASidePanel::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APanel::DoPaint(cs,pEvent);
}

void ASidePanel::DoLayout()
{
	APanel::DoLayout();
}

void ASidePanel::OnTitlePaint(AObject* p,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	AApplication::Get()->GetCurSkin()->DrawSidePanelTitle(m_pUICore->GetCanvas(),dynamic_cast<AControl *>(p));
}

void ASidePanel::OnArrowPaint(AObject* p,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	AApplication::Get()->GetCurSkin()->DrawSidePanelArrow(m_pUICore->GetCanvas(),dynamic_cast<AControl *>(p));
}

void ASidePanel::SetTextAlign(TTextAlign al)
{
	m_TextAlign = al;
}
TTextAlign ASidePanel::GetTextAlign()
{
	return m_TextAlign;
}
APanel* ASidePanel::GetClientContainer()
{
	return m_pClientContainer;
}
