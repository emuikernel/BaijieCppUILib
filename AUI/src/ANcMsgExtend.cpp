#include "stdafx.h"
#include "ANcMsgExtend.h"
#include "AWindow.h"

ANcMsgExtend::ANcMsgExtend():AMsgExtend()
{
	//
}


ANcMsgExtend::~ANcMsgExtend()
{
	//
}

ANcMsgExtend::ANcMsgExtend(AWindow* pWin):AMsgExtend(),m_pWindow(pWin)
{
	//
}

//消息处理前缀，如果返回false，则不进行后续处理
bool ANcMsgExtend::BeforeMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)
{
	return true;
}

//消息处理后缀
void ANcMsgExtend::AfterMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)
{

}
//消息过滤处理
bool ANcMsgExtend::DealMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)
{
	AWinControlBase* pBase = m_pWindow->GetOwner();
	AUICore* pCore = pBase->GetUICore();

	if( uMsg == WM_NCHITTEST && pBase->GetSizable() )
	{
		// 分解当前鼠标的坐标
		POINTS pt = MAKEPOINTS(lParam);
		int nPosX = pt.x;// LOWORD(lParam); 
		int nPosY = pt.y;// HIWORD(lParam);
		RECT rc;
		::GetWindowRect(m_pWindow->GetHandle(),&rc);
		int Left = rc.left,Top = rc.top;
		int Width = rc.right - rc.left,Height = rc.bottom - rc.top;
		int delta = 4;
		if(nPosX >= Left + Width - delta && nPosY >= Top + Height - delta)
		{
			// 鼠标位置在窗体的右下角附近
			pBase->SetCursorType(ctSizeNWSE);
			pBase->GetUICore()->SetMousePos(ret=HTBOTTOMRIGHT);
		}    
		else if( nPosX <= Left +delta && nPosY <= Top+delta)
		{
			pBase->SetCursorType(ctSizeNWSE);
			pBase->GetUICore()->SetMousePos(ret=HTTOPLEFT);
		}
		else if( nPosX >= Left+Width-delta && nPosY <= Top+delta )
		{
			pBase->SetCursorType(ctSizeNESW);
			pBase->GetUICore()->SetMousePos(ret=HTTOPRIGHT);
		}
		else if( nPosX <= Left+delta && nPosY >= Top+Height-delta )
		{
			pBase->SetCursorType(ctSizeNESW);
			pBase->GetUICore()->SetMousePos(ret=HTBOTTOMLEFT);
		}
		else if( nPosX >= Left+delta && nPosX <= Left+Width -delta && nPosY <= Top+delta )
		{
			pBase->SetCursorType(ctSizeNS);
			pBase->GetUICore()->SetMousePos(ret=HTTOP);
		}
		else if( nPosX <= Left + delta )
		{
			pBase->SetCursorType(ctSizeWE);
			pBase->GetUICore()->SetMousePos(ret=HTLEFT);
		}
		else if( nPosX >= Left+Width-delta )
		{
			pBase->SetCursorType(ctSizeWE);
			pBase->GetUICore()->SetMousePos(ret=HTRIGHT);
		}
		else if( nPosY >= Top+Height-delta )
		{
			pBase->SetCursorType(ctSizeNS);
			pBase->GetUICore()->SetMousePos(ret=HTBOTTOM);
		}
		else if( nPosX >= Left+delta && nPosX <= Left+Width-delta && nPosY >= Top+delta && nPosY <= Top+Height-delta )
		{
			pBase->SetCursorType(ctArrow);
			pBase->GetUICore()->SetMousePos(ret=HTCLIENT);
		}
		else
		{
			pBase->SetCursorType(ctArrow);
			pBase->GetUICore()->SetMousePos(ret=HTNOWHERE);
		}
		if( pBase->GetUICore()->GetMousePos() != HTNOWHERE && pBase->GetUICore()->GetMousePos() != HTCLIENT )
		{
			::SetCursor( AApplication::Get()->GetCursor(pBase->GetCursorType()) );
		}

		if( ::GetCapture() == pCore->GetHandle() )
			pCore->Refresh(pBase->GetAbsPosition());
	}
	else if( uMsg == WM_GETMINMAXINFO )
	{
		MONITORINFO oMonitor;
		memset(&oMonitor,0,sizeof(MONITORINFO));//= {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(m_pWindow->GetHandle(), MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		ARect rcWork = oMonitor.rcWork;
		rcWork.Offset(-rcWork.left, -rcWork.top);
		
		LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
		lpMMI->ptMaxPosition.x	= rcWork.left;
		lpMMI->ptMaxPosition.y	= rcWork.top;
		lpMMI->ptMaxSize.x		= rcWork.right;
		lpMMI->ptMaxSize.y		= rcWork.bottom;
	}
	else if( uMsg == WM_LBUTTONDOWN )
	{
		int nPosX = LOWORD(lParam); 
		int nPosY = HIWORD(lParam);
		if( wParam & MK_LBUTTON )
		{
			HWND h = m_pWindow->GetHandle();
			int nPos = pCore->GetMousePos();
			if( h != ::GetCapture() && (nPos == HTTOPLEFT || nPos == HTTOP || nPos == HTTOPRIGHT || nPos == HTRIGHT ||
				nPos == HTBOTTOMRIGHT || nPos == HTBOTTOM || nPos == HTBOTTOMLEFT || nPos == HTLEFT ) )
			{
				::SetCapture( m_pWindow->GetHandle() );
				m_ptDown = APoint(nPosX,nPosY);
			}
		}
	}
	else if( uMsg == WM_MOUSEMOVE )
	{
		int nPosX = LOWORD(lParam); 
		int nPosY = HIWORD(lParam);
		RECT rc;
		::GetWindowRect(m_pWindow->GetHandle(),&rc);
		if( wParam & MK_LBUTTON )
		{
			HWND h = m_pWindow->GetHandle();
			if( h == ::GetCapture() )
			{
				int dx = nPosX-m_ptDown.x,dy = nPosY-m_ptDown.y;
		
				switch( pCore->GetMousePos() )
				{
				case HTTOPLEFT: rc.left += dx;rc.top += dy;break;
				case HTTOP:rc.top+= dy;break;
				case HTTOPRIGHT:rc.right+=dx;rc.top+=dy;break;
				case HTRIGHT:rc.right+=dx;break;
				case HTBOTTOMRIGHT:rc.right+=dx;rc.bottom+=dy;break;
				case HTBOTTOM:rc.bottom+=dy;break;
				case HTBOTTOMLEFT:rc.bottom+=dy;rc.left+=dx;break;
				case HTLEFT:rc.left+=dx;break;
	
				}
				if( dx || dy )
					::MoveWindow(h,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,TRUE);
				//::UpdateWindow(h);
				m_ptDown = APoint(nPosX,nPosY);
			}
		}
	}
	else if( uMsg == WM_LBUTTONUP )
	{
		HWND h = m_pWindow->GetHandle();
		if( h == ::GetCapture() ) ::ReleaseCapture();
	}
	return true;
}


