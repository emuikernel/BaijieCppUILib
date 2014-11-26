#include "StdAfx.h"
#include "ATitleBar.h"

ATitleBar::ATitleBar(AComponent* pOwner):AContainer(pOwner)
{
	m_pBackGroundImage = NULL;
}

ATitleBar::~ATitleBar()
{
	//
}
void ATitleBar::DoDblClick(AEvent* pEvent)
{
	if( ::IsZoomed(GetUICore()->GetWin()->GetHandle()) )
		::ShowWindow(GetUICore()->GetWin()->GetHandle(),SW_RESTORE);
	else
	{
		::ShowWindow(GetUICore()->GetWin()->GetHandle(),SW_SHOWMAXIMIZED);
		RECT rScreen;
		if( TRUE == SystemParametersInfo(SPI_GETWORKAREA,0,&rScreen,0) )
		{
			::MoveWindow(GetUICore()->GetWin()->GetHandle(),rScreen.left,rScreen.top,rScreen.right-rScreen.left,rScreen.bottom-rScreen.top,TRUE);
		}
	}
	AControl::DoDblClick(pEvent);
}

void ATitleBar::DoMouseDown(AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent *>(pEvent);
	if( evt == NULL ) return;

	if( evt->m_KeyStates.Has(ksLeftButton) )
	{//fix me later 加了PostMessage后，无法响应dblclick事件 ！！
		//先判断是不是doubleClick
		DWORD dw = ::GetTickCount(),dwLast = GetUICore()->GetLastTick();
		if( dw > dwLast ) dw -= dwLast;
		else dw = (DWORD)-1 - dwLast + dw;
		DebugStr(AString().Format(_T("dw_delta:%d dwLast:%d"),dw,dwLast));

		if( dw <= ::GetDoubleClickTime() )
		{
			this->DoDblClick(pEvent);
		}
		else
		{
			//if( FALSE == ::IsZoomed(GetUICore()->GetWin()->GetHandle()))
			//	::PostMessage(GetUICore()->GetWin()->GetHandle(),WM_NCLBUTTONDOWN,HTCAPTION,evt->m_lParam);
		}

		//GetUICore()->SetCaptureControl(this);
		//::SendMessage(GetUICore()->GetWin()->GetHandle(),WM_NCHITTEST,0,evt->m_lParam);
		
		//PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));
	}
	AContainer::DoMouseDown(pEvent);
}

void ATitleBar::DoMouseMove(AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent *>(pEvent);
	if( evt == NULL ) return;
	
	if( evt->m_KeyStates.Has(ksLeftButton) )
	{
		//::SendMessage(GetUICore()->GetWin()->GetHandle(),WM_NCHITTEST,0,evt->m_lParam);
		if( FALSE == ::IsZoomed(GetUICore()->GetWin()->GetHandle()))
			::PostMessage(GetUICore()->GetWin()->GetHandle(),WM_NCLBUTTONDOWN,HTCAPTION,evt->m_lParam);
	}
	AContainer::DoMouseMove(pEvent);
}

void ATitleBar::DoMouseUp(AEvent* pEvent)
{
	GetUICore()->Refresh(GetAbsPosition());
	AContainer::DoMouseUp(pEvent);
}
void ATitleBar::DoPaint(ACanvas* cs,AEvent *pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		AApplication::Get()->GetCurSkin()->DrawTitleBar(cs,this);
	}
	AContainer::DoPaint(cs,pEvent);
}

void ATitleBar::SetBackgroundImage(AImage* p)
{
	m_pBackGroundImage = p;
}

AImage* ATitleBar::GetBackgroundImage()
{
	return m_pBackGroundImage;
}
