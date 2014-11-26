#include "stdafx.h"
#include "ANcMsgExtend.h"

AFormWindow::AFormWindow(AWinControlBase* pOwner):base_class(pOwner)
{
	AddMsgExtend(new ANcMsgExtend(this));
}

AFormWindow::~AFormWindow()
{
	//
}


DWORD AFormWindow::GetClassStyle(DWORD dwOldStyle)
{
	//dwOldStyle |= WS_SYSMENU ;//| WS_OVERLAPPEDWINDOW;// | WS_SIZEBOX;
	//dwOldStyle |= WS_SYSMENU | WS_OVERLAPPEDWINDOW;// | WS_SIZEBOX;
	dwOldStyle &= ~(WS_CAPTION  | WS_BORDER | WS_DLGFRAME );//| WS_SIZEBOX);
	//dwOldStyle &= ~(WS_CAPTION  | WS_BORDER | WS_DLGFRAME | WS_SIZEBOX);
	//dwOldStyle |= 0x00020000;//¥∞ø⁄“ı”∞
	//dwOldStyle |= WS_VISIBLE|WS_OVERLAPPEDWINDOW;
	return dwOldStyle;
}

DWORD AFormWindow::GetExStyle(DWORD dwOldExStyle)
{
	return dwOldExStyle;
}

