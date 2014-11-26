#include "stdafx.h"
#include "AWindowBase.h"
#include "AWindow.h"

AWindowBase::AWindowBase():AObject(),m_OldProc(::DefWindowProc)
{
	m_hWnd = NULL;

}

AWindowBase::~AWindowBase()
{
	//
}

AString AWindowBase::GetSuperClassName()
{
	return _T("");
}

LRESULT CALLBACK AWindowBase::msgRouter (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//DebugStr(AString().Format(_T("&&&&&&&& 进入消息处理msgRouter uMsg=%x"),uMsg));
	AWindowBase *pThis = 0;        // pointer to the window that should receive the message

	if (uMsg == WM_NCCREATE) {
		// if this message gets sent then a new window has just been created,
		// so we'll asociate its handle with its AbstractWindow instance pointer
		::SetWindowLong (hWnd, GWL_USERDATA, long((LPCREATESTRUCT(lParam))->lpCreateParams));
	}

	// --- messages different from WN_NCCREATE / or WM_NCCREATE was just processed ---
	// we retrieve the instance of AbstractWindow that corresponds to the destination window's HWND
	pThis = (AWindowBase *) (::GetWindowLong (hWnd, GWL_USERDATA));
	//if( pThis && pThis->m_hWnd != hWnd )
	{
		//DebugStr(AString().Format(L"Ooooops !!!!!!!!!!!!!  pThis:%p this.Wnd=%p hWnd=%p~~~~~~~~~~~~~",pThis,pThis->m_hWnd,hWnd));
	}

	// we then route the message to the wndProc method defined in the derived AbstractWindow class
	LRESULT ret=0;
	AWindowBase* pDestroyThis = NULL;
	if (pThis)
	{
		ret = pThis->wndProc(hWnd, uMsg, wParam, lParam);
		

		if (uMsg == WM_NCDESTROY) 
		{
			::SetWindowLong (hWnd, GWL_USERDATA, 0);
			pDestroyThis = pThis;
			pThis = NULL;
		}
	}
	if( !ret )
	{
		//ret = ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		if( pThis && uMsg != WM_NCDESTROY )//注意：WM_NCDESTROY之后pThis已经是个无效指针！
			ret = CallWindowProc(pThis->m_OldProc, hWnd, uMsg, wParam, lParam);
		else
			ret = ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	if( pDestroyThis ) 
		pDestroyThis->Release();
	//DebugStr(_T(">>>>>>>> 离开消息处理msgRouter"));
	return ret;
}
void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	//LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and exit the process


	MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	//LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}
bool AWindowBase::RegisterSupperClass()
{
	WNDCLASSEX wc = { 0 };

	AString sSuper;
	sSuper = GetSuperClassName();

	wc.cbSize = sizeof(WNDCLASSEX);
	if( !::GetClassInfoEx(NULL, sSuper.Text(), &wc) )
	{
		if( !::GetClassInfoEx(AApplication::Get()->GetHandle(), sSuper.Text(), &wc) )
		{
			throw AException(_T("Unable to locate window class"));
		}
	}
	wc.lpfnWndProc = AWindowBase::msgRouter;
	//wc.hInstance = AApplication::Get()->GetHandle();

	ATOM ret = ::RegisterClassEx(&wc);
	//_ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != (ATOM)NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}
bool AWindowBase::RegisterClass()
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;//GetClassStyle(0);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = AWindowBase::msgRouter;
	wc.hInstance = AApplication::Get()->GetHandle();
	wc.hCursor = NULL;//::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	AString sClass;
	sClass = GetClassName();
	wc.lpszClassName = sClass.Text();
	ATOM ret = ::RegisterClassEx(&wc);
	return ret != (ATOM)NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}
bool AWindowBase::Create(HWND hParent,TWindowPos wp)
{
	AString sClass;
	sClass = GetSuperClassName();
	if( sClass != _T("") )
	{
		if( !RegisterSupperClass() ) return false;
	}
	else
	{
		sClass = GetClassName();
		if( sClass != _T("") )
		{
			if( !RegisterClass() ) 
			{
				ErrorExit(_T("AWindowBase::Create.RegisterClass"));
				return false;
			}
		}
		else
			return false;
	}
	ARect rPos(0,0,0,0);
	if( wp == wpDefault ) rPos = ARect(CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT);
	m_hWnd = ::CreateWindow(
			sClass.Text(),
			GetWindowName().Text(),
			0,
			rPos.left,rPos.top,rPos.right,rPos.bottom,
			hParent,
			NULL,
			AApplication::Get()->GetHandle(),
			(LPVOID)this
		);

	if (m_hWnd == NULL ) return false;
	else
	{
		DebugStr(AString().Format(L"AWindowBase::CreateWindow m_hWnd=%p",m_hWnd));
		DWORD dwStyle = ::GetWindowLong(m_hWnd,GWL_STYLE);
		::SetWindowLong(m_hWnd,GWL_STYLE, this->GetClassStyle(dwStyle));
		::SetWindowLong(m_hWnd,GWL_EXSTYLE, this->GetExStyle(0));
		
		return true;
	}
}

void AWindowBase::Subclass()
{
	m_OldProc = (WNDPROC)GetWindowLong ( m_hWnd , GWL_WNDPROC );
	::SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)AWindowBase::msgRouter);
}

LRESULT AWindowBase::SendMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return ::SendMessage(m_hWnd,uMsg,wParam,lParam);
}

void AWindowBase::Close()
{
	if( ::IsWindow(m_hWnd) )
		::PostMessage(m_hWnd,WM_CLOSE,0,0);
}
