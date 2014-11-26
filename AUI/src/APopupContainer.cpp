#include "StdAfx.h"

APopupContainer::APopupContainer(AComponent* pOwner):AWinControlBase(pOwner)
{

}

APopupContainer::~APopupContainer()
{

}

AWindow* APopupContainer::CreateWnd()
{
	return new APopupContainerWindow(this);
}

void APopupContainer::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawPopup(cs,this);
	AContainer::DoPaintChildren(pEvent);
}

void APopupContainer::DoLostFocus(AEvent* pEvent)
{
	Hide();
	OnKillFocus.Call(this,pEvent);
}

AUICore* APopupContainer::GetParentUICore()
{
	AControl* pParent = dynamic_cast<AControl *>(GetParent());
	if( pParent == NULL ) return NULL;
	return pParent->GetUICore();
}
//X,Y是相对于屏幕的坐标
void APopupContainer::Popup(int X,int Y)
{
	AControl* pParent = dynamic_cast<AControl *>(GetParent());
	if( pParent == NULL ) return;

	if( X == 0 && Y == 0 )
	{
		POINT pt;
		::GetCursorPos(&pt);
		X = pt.x;
		Y = pt.y;
	}
	SetLeft(X);
	SetTop(Y);

	//清空痕迹，避免再次显示时保留上次的位置
	GetUICore()->SetMouseOverControl(NULL);

	AContainer* pp = dynamic_cast<AContainer*>(GetParent());
	if( pp )
	{
		//OutputDebugString(_T("AMenu SetCaptureControl"));
		//pp->GetUICore()->SetCaptureControl(this);
		//pp->Refresh();
		//::UpdateWindow(pp->GetUICore()->GetHandle());
	}
	SetVisible(true);

	::SetWindowPos(GetHandle(),HWND_TOPMOST,GetLeft(),GetTop(),GetWidth(),GetHeight(),SWP_SHOWWINDOW);

	AUICore* pCore = GetParentUICore();
	if( pCore )
		pCore->SetPopupControl(this);
	//Refresh();
}

void APopupContainer::ClosePopup()
{
	Hide();
}