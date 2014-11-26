#include "stdafx.h"
#include "APaintMsgExtend.h"
#include "AWindow.h"

APaintMsgExtend::APaintMsgExtend():AMsgExtend()
{
	//
}


APaintMsgExtend::~APaintMsgExtend()
{
	//
}

APaintMsgExtend::APaintMsgExtend(AWindow* pWin):AMsgExtend(),m_pWindow(pWin)
{
	//
}

//消息处理前缀，如果返回false，则不进行后续处理
bool APaintMsgExtend::BeforeMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)
{
	//真正刷新显示后才重置NeedRefresh m_pWindow->GetOwner()->GetUICore()->m_bNeedRefresh = false;
	//m_pWindow->GetOwner()->GetUICore()->m_rRefreshArea = ARect(0,0,0,0);
	return true;
}

//消息处理后缀
void APaintMsgExtend::AfterMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)
{
	AWinControlBase* pBase = m_pWindow->GetOwner();
	if( pBase == NULL ) return ;

	AUICore* pCore = pBase->GetUICore();
	if( pCore->m_bNeedRefresh )
	{
		HWND hWnd = m_pWindow->GetHandle();
		ARect r = pCore->m_rRefreshArea;
		::InvalidateRect(hWnd,&r,FALSE);
		::UpdateWindow(hWnd);
		//pCore->m_bNeedRefresh = false;
		//pCore->m_rRefreshArea = ARect(0,0,0,0);
	}
}
//消息过滤处理
bool APaintMsgExtend::DealMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)
{
	return true;
}


