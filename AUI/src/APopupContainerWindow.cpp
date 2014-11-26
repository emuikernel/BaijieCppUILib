#include "stdafx.h"


APopupContainerWindow::APopupContainerWindow(AWinControlBase* pOwner):AWindow(pOwner)
{

}

APopupContainerWindow::~APopupContainerWindow()
{
	//
}

DWORD APopupContainerWindow::GetClassStyle(DWORD dwOldStyle)
{
	dwOldStyle &= ~(WS_CAPTION  | WS_BORDER | WS_DLGFRAME);
	dwOldStyle |= WS_POPUP;
	return dwOldStyle;
}

DWORD APopupContainerWindow::GetExStyle(DWORD dwOldExStyle)
{
	dwOldExStyle |= WS_EX_TOOLWINDOW;//WS_EX_TOPMOST ;
	return dwOldExStyle;
}

void APopupContainerWindow::OnLButtonDown(int X,int Y,AFlagSet& flag)
{
//
}
