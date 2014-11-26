#include "StdAfx.h"
#include "ACloseButton.h"

ACloseButton::ACloseButton(AComponent* pOwner):AControl(pOwner)
{
	//
}

ACloseButton::~ACloseButton()
{
	//
}

void ACloseButton::DoPaint(ACanvas* cs,AEvent *pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	
	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		AApplication::Get()->GetCurSkin()->DrawCloseButton(cs,this);
	}
	
}

void ACloseButton::DoMouseDown(AEvent* pEvent)
{
	GetUICore()->Refresh(GetAbsPosition());
	AControl::DoMouseDown(pEvent);
}

void ACloseButton::DoClick(AEvent* pEvent)
{
	OnClick.Call(this,pEvent);
	AControl::DoClick(pEvent);

	GetUICore()->SetFocusControl(NULL);
	GetUICore()->SetLastClick(NULL);
	GetUICore()->SetMouseDownControl(NULL);
	GetUICore()->SetMouseOverControl(NULL);
	::PostMessage(GetUICore()->GetHandle(),WM_CLOSE,0,0);
}
