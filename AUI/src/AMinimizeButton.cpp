#include "StdAfx.h"
#include "AMinimizeButton.h"

AMinimizeButton::AMinimizeButton(AComponent* pOwner):AControl(pOwner)
{
	//
}

AMinimizeButton::~AMinimizeButton()
{
	//
}

void AMinimizeButton::DoPaint(ACanvas* cs,AEvent *pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	
	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		AApplication::Get()->GetCurSkin()->DrawMinimizeButton(cs,this);
	}
	
}

void AMinimizeButton::DoClick(AEvent* pEvent)
{
	::ShowWindow(GetUICore()->GetWin()->GetHandle(),SW_MINIMIZE);
	OnClick.Call(this,pEvent);
	AControl::DoClick(pEvent);
}

void AMinimizeButton::DoMouseDown(AEvent* pEvent)
{
	GetUICore()->Refresh(GetAbsPosition());
	AControl::DoMouseDown(pEvent);
}