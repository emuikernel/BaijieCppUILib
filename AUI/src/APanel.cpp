#include "StdAfx.h"

APanel::APanel(AComponent* pOwner):AContainer(pOwner)
{
	m_iBorderWidth = 0;
}

APanel::~APanel()
{

}


void APanel::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawPanel(cs,this);

	AContainer::DoPaintChildren(pEvent);
}
