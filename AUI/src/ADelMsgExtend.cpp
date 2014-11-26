#include "stdafx.h"
#include "ADelMsgExtend.h"
#include "AWindow.h"

ADelMsgExtend::ADelMsgExtend():AMsgExtend()
{
	//
}


ADelMsgExtend::~ADelMsgExtend()
{
	//
}

ADelMsgExtend::ADelMsgExtend(AWindow* pWin):AMsgExtend(),m_pWindow(pWin)
{
	//
}

//消息处理前缀，如果返回false，则不进行后续处理
bool ADelMsgExtend::BeforeMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)
{
	return true;
}

//消息处理后缀
void ADelMsgExtend::AfterMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)
{
	
}
//消息过滤处理
bool ADelMsgExtend::DealMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)
{
	//
	return true;
}


