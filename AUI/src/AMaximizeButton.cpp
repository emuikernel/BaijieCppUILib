#include "StdAfx.h"
#include "AMaximizeButton.h"

AMaximizeButton::AMaximizeButton(AComponent* pOwner):AControl(pOwner)
{
	//
}

AMaximizeButton::~AMaximizeButton()
{
	//
}

void AMaximizeButton::DoPaint(ACanvas* cs,AEvent *pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	
	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		AApplication::Get()->GetCurSkin()->DrawMaximizeButton(cs,this);
	}
	
}

void AMaximizeButton::DoClick(AEvent* pEvent)
{
	if( ::IsZoomed(GetUICore()->GetWin()->GetHandle()) )
		::ShowWindow(GetUICore()->GetWin()->GetHandle(),SW_RESTORE);
	else
	{
		::ShowWindow(GetUICore()->GetWin()->GetHandle(),SW_SHOWMAXIMIZED);
		//RECT rScreen;
		//if( TRUE == SystemParametersInfo(SPI_GETWORKAREA,0,&rScreen,0) )
		//{
			//::MoveWindow(GetUICore()->GetWin()->GetHandle(),rScreen.left,rScreen.top,rScreen.right-rScreen.left,rScreen.bottom-rScreen.top,TRUE);
		//}

		/*
GetSystemMetrics 得到屏幕大小，减去状态栏大小
TrayWnd   :=   FindWindow('Shell_TrayWnd',nil);//得到任务栏句柄
	GetWindowRect(TrayWnd,   Rec);          //获取任务栏尺寸
	mainfm .Left :=0;
	mainfm .top:=0;
	mainfm .Width :=Screen .Width ;
mainfm .Height :=Screen .Height-(rec.Bottom- rec.Top) ;
*/
		//::ShowWindow(GetUICore()->GetWin()->GetHandle(),SW_SHOWMAXIMIZED);
		//注：在WM_GETMINMAXINFO消息中处理最大化问题
	}

	OnClick.Call(this,pEvent);
	AControl::DoClick(pEvent);
}

void AMaximizeButton::DoMouseDown(AEvent* pEvent)
{
	GetUICore()->Refresh(GetAbsPosition());
	AControl::DoMouseDown(pEvent);
}
