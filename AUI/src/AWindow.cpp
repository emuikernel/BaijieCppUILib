#include "stdafx.h"
#include "APaintMsgExtend.h"
#include "ACaptureMsgExtend.h"

/* 关于窗口的几个原则：
 * 1. AWinControlBase的AWindow窗口存在，则AWinControlBase对象必定存在（没释放，但可能已经名义上销毁）
 * 2. 销毁一个控件前，它的子控件必定先销毁
 * 3. 在一个消息处理结束前，控件不应真正的销毁（可以是名义上销毁）
 *
 */
AWindow::AWindow(AWinControlBase* pOwner):
	base_class(),
	m_pOwnerControl(pOwner),
	m_bPostQuit(false),
	m_hDC(NULL)
{
	AddMsgExtend(new APaintMsgExtend(this));
	AddMsgExtend(new ACaptureMsgExtend(this));
}

AWindow::~AWindow()
{
	for(int i = 0;i<m_aMsgExtend.GetCount();i++)
	{
		AMsgExtend* p = (AMsgExtend *)m_aMsgExtend[i];
		delete p;
	}
	m_aMsgExtend.Clear();
}

void AWindow::SetPostQuitFlag(bool b)
{
	DebugStr(AString().Format(_T("SetPostQuitFlag:%d win:%p handle:%d class:%s"),b?1:0, this,m_hWnd,m_pOwnerControl->GetClass()));
	m_bPostQuit = b;
}

void AWindow::AddMsgExtend(AMsgExtend* pExt)
{
	m_aMsgExtend.Add(pExt);
}

AWinControlBase* AWindow::GetOwner()
{
	return m_pOwnerControl;
}

LRESULT CALLBACK AWindow::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	AWinControlBase* pBase = m_pOwnerControl;
	if( pBase->IsDestroyed() ) return 0;

	AWindow* pThis = (AWindow*)(AWindowBase *) (::GetWindowLong (hWnd, GWL_USERDATA));
	//if( pThis->m_pOwnerControl != pBase || hWnd != m_hWnd )
		//DebugStr(L"&&&&%%%%% 请注意！！！！！====++++++++");
	
	LRESULT lRes=0;

	if( !BeforeMsg(uMsg,wParam,lParam,lRes) ) return lRes;


	if( DealMsg(uMsg,wParam,lParam,lRes) )
	{
		AUICore* pCore = pBase->GetUICore();
		switch( uMsg )
		{
		case WM_NCDESTROY:
			lRes = _ncdestroy(hWnd,wParam,lParam);
			break;
		case WM_CLOSE:
			lRes = _close(hWnd,wParam,lParam);
			break;
		case WM_PAINT:
			lRes = _paint(hWnd,wParam,lParam);
			break;
		case WM_ERASEBKGND:
			lRes = _erasebkground(hWnd,wParam,lParam);
			break;
		case WM_SIZE:
			lRes = _size(hWnd,wParam,lParam);
			break;
		case WM_MOVE:
			lRes = _move(hWnd,wParam,lParam);
			break;
		case WM_LBUTTONDOWN:
			lRes = _lbuttondown(hWnd,wParam,lParam);
			break;
		case WM_MOUSEMOVE:
			lRes = _mousemove(hWnd,wParam,lParam);
			break;
		case WM_LBUTTONUP:
			lRes = _lbuttonup(hWnd,wParam,lParam);
			break;
		case WM_RBUTTONUP:
			lRes = _rbuttonup(hWnd,wParam,lParam);
			break;
		case WM_MOUSEWHEEL:
			lRes = _mousewheel(hWnd,wParam,lParam);
			break;
		case WM_MOUSELEAVE:
			lRes = _mouseleave(hWnd,wParam,lParam);
			break;
		case WM_KILLFOCUS:
			lRes = _killfocus(hWnd,wParam,lParam);
			break;
		case WM_CHAR:
			lRes = _char(hWnd,wParam,lParam);
			break;
		case WM_KEYDOWN:
			lRes = _keydown(hWnd,wParam,lParam);
			break;
		case WM_KEYUP:
			lRes = _keyup(hWnd,wParam,lParam);
			break;
		case WM_SYSKEYDOWN:
			lRes = _keydown(hWnd,wParam,lParam);
			break;
		case WM_SYSKEYUP:
			lRes = _keyup(hWnd,wParam,lParam);
			break;
		case WM_ACTIVATE:
			if( pBase->GetUICore()->GetOldFocus() )
			{
				pBase->GetUICore()->SetFocusControl(pBase->GetUICore()->GetOldFocus());
				AEvent evt;
				pBase->GetUICore()->GetOldFocus()->DoFocus(&evt);
			}
			break;
		default:
			break;
		}

	}
	else
	{
		return lRes;

	}
	

	if( uMsg == WM_NCDESTROY ) 
		return lRes;

	

	if( !m_pOwnerControl->IsDestroyed() )
	{
		AfterMsg(uMsg,wParam,lParam,lRes);
		m_pOwnerControl->GetUICore()->m_iEnableLayout = 0;//如果忘了打开EnableLayout（或因异常跳过），这里是最后的机会
	}
	return lRes;
}

bool AWindow::Create(HWND hParent,TWindowPos wp)
{
	if( !base_class::Create(hParent,wp) ) return false;

	AString s;
	s.Format(_T("\r\n---- 创建了一个新窗口，句柄=0x%08X 所属类:%s"),m_hWnd,m_pOwnerControl->GetClass());
	DebugStr(s);

	//::MoveWindow(m_hWnd,m_pOwnerControl->GetLeft(),m_pOwnerControl->GetTop(),m_pOwnerControl->GetWidth(),m_pOwnerControl->GetHeight(),FALSE);
	return true;
}

DWORD AWindow::GetClassStyle(DWORD dwOldStyle)
{
	dwOldStyle |= CS_VREDRAW|CS_SAVEBITS|CS_PARENTDC|CS_OWNDC|CS_HREDRAW|CS_DBLCLKS|CS_CLASSDC|CS_BYTEALIGNWINDOW;
	return dwOldStyle;//缺省情况下不变，子类可重载
}

DWORD AWindow::GetExStyle(DWORD dwOldExStyle)
{
	return dwOldExStyle;//缺省情况下不变，子类可重载
}

AString AWindow::GetWindowName()
{
	return m_pOwnerControl->GetText();
}

AString AWindow::GetClassName()
{
	AString sName;
	sName = m_pOwnerControl->GetName();
	if( sName == _T("") ) sName.Format(_T("AFCWIN%p"),this);
	return sName;
}

//消息处理前缀，如果返回false，则不进行后续处理
bool AWindow::BeforeMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)
{
	for(int i = 0;i<m_aMsgExtend.GetCount();i++)
	{
		AMsgExtend* p = (AMsgExtend*)m_aMsgExtend[i];
		if( !p->BeforeMsg(uMsg,wParam,lParam,ret) ) return false;
	}
	return true;
}

//消息处理后缀
void AWindow::AfterMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)
{
	for(int i = 0;i<m_aMsgExtend.GetCount();i++)
	{
		AMsgExtend* p = (AMsgExtend*)m_aMsgExtend[i];
		p->AfterMsg(uMsg,wParam,lParam,ret);
	}
}
//消息过滤处理
bool AWindow::DealMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)
{
	for(int i = 0;i<m_aMsgExtend.GetCount();i++)
	{
		AMsgExtend* p = (AMsgExtend *)m_aMsgExtend[i];
		if( !p->DealMsg(uMsg,wParam,lParam,ret) ) return false;
	}
	return true;
}

void AWindow::Close()
{
	base_class::Close();
}

inline LRESULT AWindow::_ncdestroy(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	DebugStr(AString().Format(_T("一个窗口要销毁 WM_NCDESTROY Owner:%p OwnerClass:%s win:%p handle:%p"),m_pOwnerControl,m_pOwnerControl->GetClass(),this,m_hWnd));
	if( !m_pOwnerControl->IsDestroyed() )
	{
		m_pOwnerControl->m_bDestroyFromWin = true;
		m_pOwnerControl->Destroy();
		//m_pOwnerControl->_lastRelease();
	}
	else
		m_pOwnerControl->_lastRelease();

	if( m_hDC )
	{
		::ReleaseDC(m_hWnd,m_hDC);
		m_hDC = NULL;
	}
	if( m_bPostQuit ) 
	{
		DebugStr(AString().Format(_T("一个窗口要销毁 PostQuitMessage win:%p handle:%p"),this,m_hWnd));
		::PostQuitMessage(0);
		return 1;
	}

	return 0;
}

inline LRESULT AWindow::_paint(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC = ::BeginPaint(hWnd,&ps);

	AWinControlBase* pBase = m_pOwnerControl;
	if( hDC != NULL )
	{
		AUICore* pCore = pBase->GetUICore();

		ARect rClip;
		::GetClipBox(hDC,&rClip);
		ARect rx(0,0,pBase->GetWidth(),pBase->GetHeight());
		
		DebugStr(AString().Format(_T("OnPaint wnd:%x class:%s width:%d height:%d"),
			hWnd,pBase->GetClass(),rx.GetWidth(),rx.GetHeight()));
		time_counter cc;
		ACanvas* cs = pCore->GetCanvas();
		//if( cs == NULL || cs->GetWidth() != rx.GetWidth() || cs->GetHeight() != rx.GetHeight() )
			pCore->m_pCanvas->Create(hDC,rx.GetWidth(),rx.GetHeight());
		//else
		{
			//ASolidBrush br(AColor(255,255,255,255));
			//cs->Use(&br)->FillRect(ARect(0,0,rx.GetWidth(),rx.GetHeight()));
		}
		//cc.Print(L"Create Canvas used.............");
		pCore->m_pCanvas->SetClipBox(rClip);

		APaintEvent evt;
		evt.m_wParam = wParam;
		evt.m_lParam = lParam;

		try
		{
			DebugStr(L"DoPaint.....");
			time_counter tc;
			pBase->DoPaint(pCore->m_pCanvas,&evt);
			DebugStr(L"After do paint....");
			tc.Print(
				AString().Format(L"*********** paint Area(W %d * H %d = %d) used ****** ",
					rClip.GetWidth(),rClip.GetHeight(),rClip.GetWidth()*rClip.GetHeight()));
			if( g_DragInfo.bDragging && g_DragInfo.pSource )
			{
				APoint ptMouse = pCore->GetMovePoint();
				ADragEvent de;
				de.m_pSource = g_DragInfo.pSource;
				de.m_szDrawSize = g_DragInfo.szDraw;
				de.m_X = ptMouse.x;
				de.m_Y = ptMouse.y;
				de.m_bAccept = g_DragInfo.bAccept;
				g_DragInfo.pSource->OnDrawDrag.Call( g_DragInfo.pSource,&de);
			}
			//if( m_sHint != _T("") )
			//	AApplication::Get()->GetCurSkin()->DrawHint(pCore->GetCanvas(),m_sHint,m_rHint);
		}
		catch(...)
		{
			//throw ??
		}
		//DebugStr(L"Canvas.Write...");
// 		DebugStr(AString().Format(_T("Invalid(%d,%d,%d,%d) Clip(%d,%d,%d,%d)"),
// 			pCore->m_rRefreshArea.left,
// 			pCore->m_rRefreshArea.top,
// 			pCore->m_rRefreshArea.GetWidth(),
// 			pCore->m_rRefreshArea.GetHeight(),
// 			rClip.left,
// 			rClip.top,
// 			rClip.GetWidth(),
// 			rClip.GetHeight()));
		pCore->m_pCanvas->Write(rClip,rClip);
		pCore->m_bNeedRefresh = false;
		pCore->m_rRefreshArea = ARect(0,0,0,0);
		cc.Print(L"----------AllPaint used.............");
		//DebugStr(L"Afte onapint!!!!");
		//DebugStr(AString().Format(_T("()()()endPaint wnd:%x class:%s width:%d height:%d"),
		//	hWnd,pBase->GetClass(),rx.GetWidth(),rx.GetHeight()));
	}
	::EndPaint(hWnd,&ps);
	return 1;
}

inline LRESULT AWindow::_lbuttondown(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	AFlagSet DownFlag;

	if( wParam & MK_CONTROL ) DownFlag << ksCtrl;
	if( wParam & MK_LBUTTON ) DownFlag << ksLeftButton;
	if( wParam & MK_RBUTTON ) DownFlag << ksRightButton;
	if( wParam & MK_MBUTTON ) DownFlag << ksMiddleButton;
	if( wParam & MK_SHIFT ) DownFlag << ksShift;
	if((GetKeyState(VK_MENU) & (1<<(sizeof(SHORT)*8-1))) != 0) DownFlag << ksAlt;

	int X= GET_X_LPARAM(lParam);
	int Y= GET_Y_LPARAM(lParam);

	AWinControlBase* pBase = m_pOwnerControl;
	

	AMouseEvent evt;
	evt.m_wParam = wParam;
	evt.m_lParam = lParam;
	evt.m_X = X;
	evt.m_Y = Y;
	evt.m_KeyStates = DownFlag;

	AUICore* pCore = pBase->GetUICore();
	//::SetCapture(hWnd);
	pCore->SetDownPoint(APoint(X,Y));

	AControl* wHit = pBase->_HitTest(X,Y);
	if( wHit )
		DebugStr(AString().Format(_T("\r\nMouseDown on (X=%d Y=%d) wHit=%p class=%s"),X,Y,wHit,wHit->GetClass()));

	pCore->SetPopupControl(NULL);

	AControl* pFocus = pCore->GetFocusControl();
	if( pFocus != wHit )
	{
		if( pFocus ) 
		{
			pFocus->DoLostFocus(&evt);
		}
		if( wHit ) 
		{
			wHit->DoFocus(&evt);
		}
		pCore->SetFocusControl(wHit);
	}
	pCore->SetMouseDownControl( wHit );

	if( wHit )
	{
		wHit->DoMouseDown(&evt);
		wHit->Refresh();
	}
	return 0;
}

inline LRESULT AWindow::_lbuttonup(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	AFlagSet state;
	if( wParam & MK_CONTROL ) state << ksCtrl;
	//if( wParam & MK_LBUTTON )
	state << ksLeftButton;//既然进到这里了，那肯定就包含了左键的
	if( wParam & MK_RBUTTON ) state << ksRightButton;
	if( wParam & MK_MBUTTON ) state << ksMiddleButton;
	if( wParam & MK_SHIFT ) state << ksShift;
	if((GetKeyState(VK_MENU) & (1<<(sizeof(SHORT)*8-1))) != 0) state << ksAlt;

	int X= GET_X_LPARAM(lParam);
	int Y= GET_Y_LPARAM(lParam);

	AWinControlBase* pBase = m_pOwnerControl;
	

	AMouseEvent evt;
	evt.m_wParam = wParam;
	evt.m_lParam = lParam;
	evt.m_X = X;
	evt.m_Y = Y;
	evt.m_KeyStates = state;

	afc_object<AControl> wHit;
	//::ReleaseCapture();
	AUICore* pCore = pBase->GetUICore();

	wHit = pBase->_HitTest(X,Y);
	if( &wHit != NULL )
		DebugStr(AString().Format(_T("\r\nMouseUp on (X=%d Y=%d) wHit=%p class=%s"),X,Y,wHit,wHit->GetClass()));

	if( &wHit != NULL )
	{
		if( g_DragInfo.bDragging )//优先处理拖放事件
		{
			ADragEvent de;
			de.m_pSource = g_DragInfo.pSource;
			de.m_szDrawSize = g_DragInfo.szDraw;
			de.m_X = X;
			de.m_Y = Y;
			int dx = g_DragInfo.szDraw.cx;
			int dy = g_DragInfo.szDraw.cy;
			g_DragInfo.bDragging = false;
			pCore->Refresh(ARect(X,Y,X+dx+2,Y+dy+2));
			if( g_DragInfo.bAccept )
				wHit->OnDragDrop.Call(wHit,&de);
			if( g_DragInfo.pSource ) 
			{
				g_DragInfo.pSource->OnEndDrag.Call(g_DragInfo.pSource,&de);
				g_DragInfo.pSource->Refresh();
			}
			
		}
		else
		{
			ARect r = wHit->GetAbsPosition();
			wHit->DoMouseUp(&evt);
			//wHit如果是CloseButton，将导致窗口被关闭
			//if( m_pOwnerControl == NULL ) return 0;//避免后面再处理（因为对象已经释放）

			AControl* pDownControl = pCore->GetMouseDownControl();

			if( &wHit == pDownControl )
			{
				{
					wHit->DoClick(&evt);
				}
				//wHit->DoClick(&evt);

				DWORD dw = ::GetTickCount(),dwLast = pCore->GetLastTick();
				if( dw > dwLast ) dw -= dwLast;
				else dw = (DWORD)-1 - dwLast + dw;
				//AString s;
				//s.Format(_T("\r\ndw=%d dbl=%d"),dw,::GetDoubleClickTime());
				//DebugStr(s);
				if( &wHit == pCore->GetLastClick() && dw <= ::GetDoubleClickTime() )
				{
					wHit->DoDblClick(&evt);
				}

				pCore->SetLastClick(&wHit);
				pCore->SetLastTick(::GetTickCount());
			}
			else
			{
				if( pDownControl ) pDownControl->Refresh();
			}
		}
		if( m_pOwnerControl == NULL ) return 0;
		pCore->SetMouseDownControl(NULL);
		wHit->Refresh();
	}

	pCore->SetMouseDownControl(NULL);
	//pCore->SetMouseOverControl(NULL);
	pCore->SetDownPoint(APoint(0,0));
	pCore->SetMovePoint(APoint(0,0));
	if( ::GetCapture() == GetHandle() )
		::ReleaseCapture();
	return 0;
}

inline LRESULT AWindow::_erasebkground(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	return 1;
}

inline LRESULT AWindow::_size(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	AMouseEvent evt;
	evt.m_wParam = wParam;
	evt.m_lParam = lParam;
	//这样取得的是客户区大小，不要用
// 	evt.m_X = LOWORD(lParam);
// 	evt.m_Y = HIWORD(lParam);
	ARect r;
	::GetWindowRect(hWnd,&r);
	evt.m_X = r.GetWidth();
	evt.m_Y = r.GetHeight();
	DebugStr(AString().Format(_T("hWnd=%d OnResize x=%d y=%d"),hWnd,evt.m_X,evt.m_Y));
	AWinControlBase* pBase = m_pOwnerControl;
	
	pBase->DoResize(&evt);
	pBase->Refresh();
	return 0;
}

inline LRESULT AWindow::_move(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	AMouseEvent evt;
	evt.m_wParam = wParam;
	evt.m_lParam = lParam;
	evt.m_X =  (int)(short) LOWORD(lParam);
	evt.m_Y =  (int)(short) HIWORD(lParam);
	DebugStr(AString().Format(_T("hWnd=%d OnMove x=%d y=%d"),hWnd,evt.m_X,evt.m_Y));
	AWinControlBase* pBase = m_pOwnerControl;
	
	pBase->DoMove(&evt);
	return 0;
}

inline LRESULT AWindow::_mousemove(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	AFlagSet state;
	if( wParam & MK_CONTROL ) state << ksCtrl;
	if( wParam & MK_LBUTTON ) state << ksLeftButton;
	if( wParam & MK_RBUTTON ) state << ksRightButton;
	if( wParam & MK_MBUTTON ) state << ksMiddleButton;
	if( wParam & MK_SHIFT ) state << ksShift;
	if((GetKeyState(VK_MENU) & (1<<(sizeof(SHORT)*8-1))) != 0) state << ksAlt;

	int X= GET_X_LPARAM(lParam);
	int Y= GET_Y_LPARAM(lParam);

	AWinControlBase* pBase = m_pOwnerControl;
	
	AUICore* pCore = pBase->GetUICore();
	AMouseEvent evt;
	evt.m_wParam = wParam;
	evt.m_lParam = lParam;
	evt.m_X = X;
	evt.m_Y = Y;
	evt.m_KeyStates = state;

	
	int delta = 4;
	ARect rOwner(0,0,pBase->GetWidth(),pBase->GetHeight());
	if( !( X < rOwner.left+delta || Y < rOwner.top+delta || (X > rOwner.right-delta) || (Y > rOwner.bottom-delta) ) )//位于窗口拖动区域的事件由窗口接收，控件不处理
	{
		//
		pCore->SetMovePoint(APoint(X,Y));
		AControl* wHit = pBase->_HitTest(X,Y);

		if( (wParam & MK_LBUTTON ) && g_DragInfo.bDragging )//拖放事件优先处理
		{
			if( wHit )
			{
				ADragEvent DragEvent;
				DragEvent.m_pSource = g_DragInfo.pSource;
				DragEvent.m_szDrawSize = g_DragInfo.szDraw;
				DragEvent.m_X = X;
				DragEvent.m_Y = Y;
				int dx = g_DragInfo.szDraw.cx;
				int dy = g_DragInfo.szDraw.cy;
				pCore->Refresh(ARect(g_DragInfo.X,g_DragInfo.Y,g_DragInfo.X+dx,g_DragInfo.Y+dy));
				
				//pCore->Refresh(pBase->GetAbsPosition());
				wHit->OnDragOver.Call(wHit,&DragEvent);
				g_DragInfo.bAccept = DragEvent.m_bAccept;
				g_DragInfo.X = X;
				g_DragInfo.Y = Y;
				pCore->Refresh(ARect(X,Y,X+dx,Y+dy));
			}
		}
		else
		{
			bool bRefresh= (wHit != pCore->GetMouseOverControl() ? true : false);
			if( wParam & MK_LBUTTON ) 
			{
				bRefresh = true;
				AControl* p = pCore->GetMouseDownControl();
				if( p )
				{
					ARect r = p->GetAbsPosition();
					p->DoMouseMove(&evt);
					//p->Refresh();
				}
			}
			else
			{
				AControl* pOldOver = pCore->GetMouseOverControl();
				if( pOldOver != wHit )
				{
					if( pOldOver ) 
					{
						//m_sHint = _T("");

						pOldOver->DoLeave(&evt);
						pOldOver->Refresh();
						//pCore->Refresh(pOldOver->GetAbsPosition());
					}
				}
				pCore->SetMouseOverControl(wHit);

				if( wHit )
				{
					::SetCursor( AApplication::Get()->GetCursor(wHit->GetCursorType()) );
					ARect r = wHit->GetAbsPosition();
					if( pOldOver != wHit )
					{
						//m_sHint = wHit->GetHint();
						//AFont f(_T("新宋体"),12,AColor(200,0,0,0));
						//ASize sz = pCore->GetCanvas()->Use(&f)->MeasureText(m_sHint,9);
						//m_rHint = ARect(X,r.bottom+3,X+sz.cx +6,r.bottom+3+sz.cy +6);
						wHit->DoEnter(&evt);
						wHit->Refresh();
						//pCore->Refresh(wHit->GetAbsPosition());
					}

					wHit->DoMouseMove(&evt);
					if( state.Has(ksLeftButton)) wHit->Refresh();
				}
			}
		}
		

		//if( bRefresh ) 
		//pCore->Refresh(pBase->GetAbsPosition());//pCore->m_rRefreshArea);
	}
	return 0;
}

inline LRESULT AWindow::_rbuttonup(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	AFlagSet state;
	if( wParam & MK_CONTROL ) state << ksCtrl;
	//if( wParam & MK_LBUTTON )
	state << ksRightButton;//既然进到这里了，那肯定就包含了右键的
	//if( wParam & MK_LBUTTON ) state << ksLeftButton;
	if( wParam & MK_MBUTTON ) state << ksMiddleButton;
	if( wParam & MK_SHIFT ) state << ksShift;
	if((GetKeyState(VK_MENU) & (1<<(sizeof(SHORT)*8-1))) != 0) state << ksAlt;

	int X= GET_X_LPARAM(lParam);
	int Y= GET_Y_LPARAM(lParam);

	AMouseEvent evt;
	evt.m_wParam = wParam;
	evt.m_lParam = lParam;
	evt.m_X = X;
	evt.m_Y = Y;
	evt.m_KeyStates = state;

	AWinControlBase* pBase = m_pOwnerControl;
	

	AControl* wHit=NULL;
	//::ReleaseCapture();
	AUICore* pCore = pBase->GetUICore();

	wHit = pBase->_HitTest(X,Y);

	if( wHit )
	{
		ARect r = wHit->GetAbsPosition();
		wHit->DoMouseUp(&evt);
	}

	pCore->SetMouseDownControl(NULL);
	pCore->SetMouseOverControl(NULL);
	pCore->SetDownPoint(APoint(0,0));
	pCore->SetMovePoint(APoint(0,0));
	if( ::GetCapture() == GetHandle() )
		::ReleaseCapture();
	return 0;
}

inline LRESULT AWindow::_mousewheel(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	AFlagSet WheelFlag;

	if( wParam & MK_CONTROL ) WheelFlag << ksCtrl;
	if( wParam & MK_LBUTTON ) WheelFlag << ksLeftButton;
	if( wParam & MK_RBUTTON ) WheelFlag << ksRightButton;
	if( wParam & MK_MBUTTON ) WheelFlag << ksMiddleButton;
	if( wParam & MK_SHIFT ) WheelFlag << ksShift;
	if((GetKeyState(VK_MENU) & (1<<(sizeof(SHORT)*8-1))) != 0) WheelFlag << ksAlt;
	AWinControlBase* pBase = m_pOwnerControl;
	
	int zDelta = (int) wParam;
	//注意: MouseWheel这里得到的坐标是屏幕坐标，跟MouseDown等是不同的，它们是相对于窗口的坐标
	int X= GET_X_LPARAM(lParam) - pBase->GetLeft();
	int Y= GET_Y_LPARAM(lParam) - pBase->GetTop();

	AMouseEvent evt;
	evt.m_wParam = wParam;
	evt.m_lParam = lParam;
	evt.m_X = X;
	evt.m_Y = Y;
	evt.m_Delta = zDelta;
	evt.m_KeyStates = WheelFlag;

	
	AControl* wHit = pBase->_HitTest(X,Y);
	if( wHit )
	{
		AContainer* pp = NULL;
		while( pp == NULL )
		{
			pp = dynamic_cast<AContainer *>(wHit);
			if( pp )
			{
				bool bh,bv;
				pp->IsScrollBarShow(bh,bv);
				if( ( WheelFlag.Has(ksShift) && bh ) || ( !WheelFlag.Has(ksShift) && bv ) )
				{
					pp->DoMouseWheel(&evt);
					break;
				}
				else pp = NULL;
			}
			wHit = (AControl *)wHit->GetParent();
			if( wHit == NULL ) break;
		}
	}
	return 0;
}

inline LRESULT AWindow::_mouseleave(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	AWinControlBase* pBase = m_pOwnerControl;
	
	AUICore* pCore = pBase->GetUICore();
	AControl* pCaptureControl = pCore->GetCaptureControl();
	if( pCaptureControl ) pCore->SetCaptureControl(NULL);
	//pCore->SetTraceEventControl(NULL);

	AEvent evt;
	evt.m_wParam = wParam;
	evt.m_lParam = lParam;
	if( pCore->GetMouseOverControl() )
		pCore->GetMouseOverControl()->DoLeave(&evt);

	pCore->SetMouseDownControl(NULL);
	pCore->SetMouseOverControl(NULL);
	return 0;
}

inline LRESULT AWindow::_killfocus(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	AWinControlBase* pBase = m_pOwnerControl;
	
	AUICore* pCore = pBase->GetUICore();
	AEvent evt;
	evt.m_wParam = wParam;
	evt.m_lParam = lParam;
	DebugStr(AString().Format(_T("\r\nkill focus this:%p"),pBase));
	AControl* pFocus = pCore->GetFocusControl();
	pCore->SetOldFocus(pFocus);
	if( pFocus )
	{
		pFocus->DoLostFocus(&evt);
		pCore->SetFocusControl(NULL);
	}
	pBase->DoLostFocus(&evt);
	return 0;
}

inline LRESULT AWindow::_char(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	AWinControlBase* pBase = m_pOwnerControl;
	
	AUICore* pCore = pBase->GetUICore();
	AControl* pFocus = pCore->GetFocusControl();
	if( pFocus )
	{
		DebugStr(AString().Format(_T("WM_CHAR key=%02X char=%c"),(WCHAR)wParam,(WCHAR)wParam));
		AKeyEvent evt;
		evt.m_wParam = wParam;
		evt.m_lParam = lParam;
		evt.m_chKey = (WCHAR)wParam;
		evt.m_Type = ketPress;
		pFocus->DoKeyboard(&evt);
	}
	return 0;
}

inline LRESULT AWindow::_keydown(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	AWinControlBase* pBase = m_pOwnerControl;
	
	AUICore* pCore = pBase->GetUICore();
	AControl* pFocus = pCore->GetFocusControl();
	if( g_DragInfo.bDragging && (WCHAR)wParam == VK_ESCAPE )
	{
		g_DragInfo.bCanceled = true;
		ADragEvent de;
		de.m_bCanceled = true;
		de.m_pSource = g_DragInfo.pSource;
		de.m_szDrawSize = g_DragInfo.szDraw;
		POINT pt;
		::GetCursorPos(&pt);
		ARect ra=pCore->GetWin()->GetAbsPosition();
		pt.x -= ra.left;pt.y-=ra.top;
		de.m_X = pt.x;
		de.m_Y = pt.y;
		if( g_DragInfo.pSource ) g_DragInfo.pSource->OnEndDrag.Call(g_DragInfo.pSource,&de);
		g_DragInfo.bDragging = false;
	}
	if( pFocus )
	{
		DebugStr(AString().Format(_T("WM_KEYDOWN key=%02X char=%c"),(WCHAR)wParam,(WCHAR)wParam));
		AKeyEvent evt;
		evt.m_wParam = wParam;
		evt.m_lParam = lParam;
		evt.m_chKey = (WCHAR)wParam;
		evt.m_Type = ketDown;
		if((GetKeyState(VK_MENU) & (1<<(sizeof(SHORT)*8-1))) != 0) evt.m_KeyStates << ksAlt;
		if((GetKeyState(VK_SHIFT) & (1<<(sizeof(SHORT)*8-1))) != 0) evt.m_KeyStates << ksShift;
		if((GetKeyState(VK_CONTROL) & (1<<(sizeof(SHORT)*8-1))) != 0) evt.m_KeyStates << ksCtrl;
		pFocus->DoKeyboard(&evt);
	}
	return 0;
}

inline LRESULT AWindow::_keyup(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	AWinControlBase* pBase = m_pOwnerControl;
	
	AUICore* pCore = pBase->GetUICore();
	AControl* pFocus = pCore->GetFocusControl();
	if( pFocus )
	{
		DebugStr(AString().Format(_T("WM_KEYUP key=%02X char=%c"),(WCHAR)wParam,(WCHAR)wParam));
		AKeyEvent evt;
		evt.m_wParam = wParam;
		evt.m_lParam = lParam;
		evt.m_chKey = (WCHAR)wParam;
		evt.m_Type = ketUp;
		if((GetKeyState(VK_MENU) & (1<<(sizeof(SHORT)*8-1))) != 0) evt.m_KeyStates << ksAlt;
		if((GetKeyState(VK_SHIFT) & (1<<(sizeof(SHORT)*8-1))) != 0) evt.m_KeyStates << ksShift;
		if((GetKeyState(VK_CONTROL) & (1<<(sizeof(SHORT)*8-1))) != 0) evt.m_KeyStates << ksCtrl;
		pFocus->DoKeyboard(&evt);
	}
	return 0;
}

inline LRESULT AWindow::_close(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	DebugStr(AString().Format(_T("一个窗口要关闭 WM_CLOSE Owner:%p OwnerClass:%s win:%p handle:%p"),m_pOwnerControl,m_pOwnerControl->GetClass(),this,m_hWnd));

	ACloseQueryEvent closeQueryEvent;
	closeQueryEvent.m_wParam = wParam;
	closeQueryEvent.m_lParam = lParam;
	AWinControlBase* pBase = m_pOwnerControl;
	
	pBase->OnCloseQuery.Call(pBase,&closeQueryEvent);
	if( closeQueryEvent.m_bCanClose == false ) return 1;

	ACloseEvent closeEvent;
	closeEvent.m_wParam = wParam;
	closeEvent.m_lParam = lParam;
	pBase->OnClose.Call(pBase,&closeEvent);
	if( closeEvent.m_CloseAction == caNone ) return 1;
	else if( closeEvent.m_CloseAction == caMinimize ) ::ShowWindow(pBase->GetHandle(),SW_MINIMIZE);
	else if( closeEvent.m_CloseAction == caHide ) pBase->Hide();
	else if( closeEvent.m_CloseAction == caFree ) return 0;

	return 0;
}

HDC AWindow::GetDC()
{
	if( m_hDC == NULL )
	{
		if( ::IsWindow(m_hWnd) )
			m_hDC = ::GetDC(m_hWnd);
	}
	return m_hDC;
}