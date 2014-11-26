
#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AUICore::AUICore(AWinControlBase* pWin):
	AObject(),
	m_pWinControl(pWin),
	m_pMouseDownControl(NULL),
	m_pMouseOverControl(NULL),
	m_pFocusControl(NULL),
	m_pCaptureControl(NULL),
	m_pPopupControl(NULL),
	m_bNeedRefresh(false),
	m_pOldFocus(NULL)
{
	m_ptDown = APoint(0,0);
	m_ptMove = APoint(0,0);
	m_nMousePos = HTNOWHERE;
	m_pClickControl = NULL;
	m_dwLastClick = 0;
	m_iEnableLayout = 0;//0:允许　>0:禁止Layout
	m_bDisableEvent = false;

	m_pCanvas = ACanvas::CreateCanvas();
	
}
AUICore::~AUICore()
{
	m_pCanvas->Release();
	//if( m_pFocusControl ) m_pFocusControl->Release();
}


void AUICore::Refresh(ARect r)
{
	if( !m_bNeedRefresh )
	{
		m_bNeedRefresh = true;
	}
	
	if( r.GetWidth() > 0 && r.GetHeight() > 0 )
	{
		//ARect rx(r.left-1,r.top-1,r.right+1,r.bottom+1);
		m_rRefreshArea.Union(r);//rx);
	}
}


AControl* AUICore::GetMouseDownControl()
{
	return m_pMouseDownControl;
}

void AUICore::SetMouseDownControl(AControl* pControl)
{
	m_pMouseDownControl = pControl;
}

AControl* AUICore::GetMouseOverControl()
{
	return m_pMouseOverControl;
}

void AUICore::SetMouseOverControl(AControl* pControl)
{
	m_pMouseOverControl = pControl;
}


void AUICore::SetFocusControl(AControl* p)
{
	//if( m_pFocusControl ) m_pFocusControl->Release();
	if( p == NULL || ( p!=NULL && p->CanFocus()) )
		m_pFocusControl = p;
}

AControl* AUICore::GetFocusControl()
{
	return m_pFocusControl;
}

void AUICore::SetCaptureControl(AControl* p)
{
	m_pCaptureControl = p;
}

AControl* AUICore::GetCaptureControl()
{
	return m_pCaptureControl;
}

AControl* AUICore::GetPopupControl()
{
	return m_pPopupControl;
}
void AUICore::SetPopupControl(AControl* pControl)
{
	if( m_pPopupControl != pControl )
	{
		if( m_pPopupControl != NULL )
		{
			AMenu* pMenu = dynamic_cast<AMenu*>(m_pPopupControl);
			if( pMenu != NULL )
				pMenu->ClosePopup();
		}
		m_pPopupControl = pControl;
	}
}
bool AUICore::InDrawArea(ARect r)
{
	ARect rx;
	rx = m_pCanvas->GetClipBox();
	if( rx.GetWidth() == 0 || rx.GetHeight() == 0 ) return false;
	r.Intersect(rx);
	if( r.GetWidth() > 0 && r.GetHeight() > 0 ) return true;
	return false;
}
HWND AUICore::GetHandle()
{
	if( m_pWinControl ) return m_pWinControl->GetHandle();
	else return NULL;
}

ACanvas* AUICore::GetCanvas()
{
	if( !m_pCanvas->IsValid() ) 
	{
		AWindow* pWin = m_pWinControl->GetWindow();
		HDC hDC = NULL;
		if( pWin ) hDC = pWin->GetDC();
		m_pCanvas->Create(hDC,m_pWinControl->GetWidth(),m_pWinControl->GetHeight());
	}
	
	if( !m_pCanvas->IsValid() ) 
		return NULL;
	else 
		return m_pCanvas;
}

APoint AUICore::GetDownPoint()
{
	return m_ptDown;
}
void AUICore::SetDownPoint(APoint pt)
{
	m_ptDown = pt;
}

APoint AUICore::GetMovePoint()
{
	return m_ptMove;
}

void AUICore::SetMovePoint(APoint pt)
{
	m_ptMove = pt;
}

void AUICore::SetMousePos(int nPos)
{
	m_nMousePos = nPos;
}

int AUICore::GetMousePos()
{
	return m_nMousePos;
}

AControl* AUICore::GetLastClick()
{
	return m_pClickControl;
}
void AUICore::SetLastClick(AControl* pLastClick)
{
	m_pClickControl = pLastClick;
}

DWORD AUICore::GetLastTick()
{
	return m_dwLastClick;
}
void AUICore::SetLastTick(DWORD dw)
{
	m_dwLastClick = dw;
}

bool AUICore::GetLayoutEnabled()
{
	return m_iEnableLayout == 0 ? true : false;
}

void AUICore::SetLayoutEnabled(bool v)
{
	if( v ) m_iEnableLayout --;
	else m_iEnableLayout ++;
}

bool AUICore::IsOverMe(AControl* p)
{
	if( p == NULL ) return false;
	if(p == m_pMouseOverControl ) return true;
	if( m_pMouseOverControl && m_pMouseOverControl->IsChildOf(p) ) return true;
	return false;
}

bool AUICore::IsFocus(AControl* p)
{
	if( p == NULL ) return false;
	if(p == m_pFocusControl ) return true;
	if( m_pFocusControl && m_pFocusControl->IsChildOf(p) ) return true;
	return false;
}

bool AUICore::GetDisableEvent()
{
	return m_bDisableEvent;
}

void AUICore::SetDisableEvent(bool v)
{
	m_bDisableEvent = v;
}

void AUICore::AddRefresh(AControl* p)
{
	if( m_aRefresh.IndexOf(p) == -1 )
		m_aRefresh.Add( p );
}
void AUICore::AddWatcher(AControl* p)
{
	m_aWatcher.Add( p );//不需要增加引用计数
}

void AUICore::RemoveWatcher(AControl* p)
{
	int i = m_aWatcher.IndexOf(p);
	if( i != -1 ) m_aWatcher.Delete(i);
}

void AUICore::WatchTo(AControl* p)
{
	int i,iCount = m_aWatcher.GetCount();
	for(i=0;i<iCount;i++)
	{
		AControl* x = (AControl*)m_aWatcher[i];
		if( !x->IsDestroyed() ) x->WatchDelete(p);
	}
}

void AUICore::SetOldFocus(AControl* p)
{
	m_pOldFocus = p;
}

AControl* AUICore::GetOldFocus()
{
	return m_pOldFocus;
}