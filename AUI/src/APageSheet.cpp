#include "StdAfx.h"

APageSheet::APageSheet(AComponent* pOwner):AContainer(pOwner)
{
	m_pTabButton = new AContainer(pOwner);
	m_pTabButton->OnClick = AEventHandler(this,(FEventHandler)&APageSheet::OnButtonClick);

	m_pButtonText = new ALabel(m_pTabButton);
	m_pButtonText->SetTextAlign(taCenterMiddle);
	m_pButtonText->OnClick = AEventHandler(this,(FEventHandler)&APageSheet::OnButtonClick);

	m_pButtonText->SetAlign(alClient);
}

APageSheet::~APageSheet()
{

}

void APageSheet::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		//
	}
	AContainer::DoPaintChildren(pEvent);
}

void APageSheet::SetText(AString s)
{
	AContainer::SetText(s);
	m_pButtonText->SetText(s);
	APageControl* pc = dynamic_cast<APageControl*>(GetParent());
	AControl* px = (AControl*)pc->GetParent();//为什么一定要PageControl的父控件Layout才会显示？ fix me later
	px->DoLayout();
}


void APageSheet::OnButtonClick(AObject* p,AEvent* pEvent)
{
	APageControl* pc = dynamic_cast<APageControl*>(GetParent());
	pc->SetActiveSheet(this);
	AControl* px = (AControl*)pc->GetParent();//为什么一定要PageControl的父控件Layout才会显示？ fix me later
	px->DoLayout();
	//GetUICore()->GetWin()->DoLayout();
}
