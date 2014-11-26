#include "StdAfx.h"

/*
		=== 滚动条算法 ===
	虚拟大小(父容器所含内容大小)：VX
	可见大小（容器大小并扣掉滚动条所占面积）：TX （一般就等于滚动条的高度或宽度）
	不可见大小：IX = VX - TX
	滚动条总长度（不含按钮）：RL（轨道长度）
	滑块长度：D
	待滚动长度：N （D + N = RL） 是滑块上面和下面长度之和
	按钮大小：B  (D+N+4*B=TX）
	缩放比例：Rate = D/RL = TX / VX    （可得出：D = RL * TX / VX ）

	水平滚动条布局：ToTopButton Button1 1pix   N1    D       N2   1pix Button2 ToBottomButton
	                ----------- ------  ---- -----  ------  ----  ----- ------ --------------
*/
AScrollBar::AScrollBar(AComponent* pOwner):AControl(pOwner)
{
	m_Kind = sbVert;

	m_iPosition = 0;
	m_iRange = 100;
	m_bShowTop = false;
	m_bShowButton = true;

	m_MouseOnElem = SCROLLBAR_ELEM_NONE;
	m_ElemStatus = SCROLLBAR_STATUS_NORMAL;
	m_iMiniStep = 10;
	m_iBigStep = 100;
	m_iDownPosition = -1;
}

AVertScrollBar::AVertScrollBar(AComponent* pOwner) : AScrollBar(pOwner)
{
	m_Kind = sbVert;
	m_iWidth = AApplication::Get()->GetCurSkin()->m_ScrollBarWidth ;
}
AHorzScrollBar::AHorzScrollBar(AComponent* pOwner) : AScrollBar(pOwner)
{
	m_Kind = sbHorz;
	m_iHeight = AApplication::Get()->GetCurSkin()->m_ScrollBarWidth;
}

AScrollBar::~AScrollBar()
{
	//
}

int AScrollBar::GetVirtualSize()
{
	int iSize=m_iRange;

	if( IsContainerScrollBar() )
	{
		AContainer* pContainer = dynamic_cast<AContainer *>(GetParent());
		if( m_Kind == sbVert )
		{
			iSize = pContainer->m_ContentSize.cy;

		}
		else
		{
			iSize = pContainer->m_ContentSize.cx;
		}
	}
	return iSize;
}

int AScrollBar::GetTrueSize()
{
	if( m_Kind == sbVert ) return GetHeight();
	else return GetWidth();
}

double AScrollBar::GetRate()
{
	if( GetVirtualSize() == 0 ) return 1.0;
	return (double)GetRailLen() / (double)GetVirtualSize();
}

void AScrollBar::SetWidth(int v)
{
	//if( GetKind() != sbVert )
		base_class::SetWidth(v);
}

void AScrollBar::SetHeight(int v)
{
	//if( GetKind() != sbHorz )
		base_class::SetHeight(v);
}

void AScrollBar::SetRange(int v)
{
	if( v < 1 ) v = 1;
	m_iRange = v;
	if( m_iPosition > v ) m_iPosition = v;
}

int AScrollBar::GetRange()
{
	return m_iRange;
}
void AScrollBar::SetPosition(int v)
{
	if( v > m_iRange) v = m_iRange;
	if( v < 0 ) v = 0;
	int delta = v - m_iPosition;
	m_iPosition = v;

	AMouseEvent evt;
	evt.m_Delta = delta;
	OnScroll.Call(this,&evt);

	m_pUICore->Refresh(GetAbsPosition());
}

int AScrollBar::GetPosition()
{
	return m_iPosition;
}
void AScrollBar::SetMiniStep(int v)
{
	m_iMiniStep = v;
}
int AScrollBar::GetMiniStep()
{
	return m_iMiniStep;
}
void AScrollBar::SetBigStep(int v)
{
	m_iBigStep = v;
}
int AScrollBar::GetBigStep()
{
	return m_iBigStep;
}
ARect AScrollBar::GetButton1Pos()
{
	ARect ret(0,0,0,0);
	if( !m_bShowButton ) return ret;
	ARect r = GetAbsPosition();
	int sw = AApplication::Get()->GetCurSkin()->m_ScrollBarWidth;
	if( m_Kind == sbVert )
	{
		if( m_bShowTop ) r.top += sw;
		ret = ARect(r.left,r.top,r.right,r.top+sw);
	}
	else
	{
		if( m_bShowTop ) r.left += sw;
		ret = ARect(r.left,r.top,r.left+sw,r.bottom);
	}
	return ret;
}
ARect AScrollBar::GetButton2Pos()
{
	ARect ret(0,0,0,0);
	if( !m_bShowButton ) return ret;
	ARect r = GetAbsPosition();
	int sw = AApplication::Get()->GetCurSkin()->m_ScrollBarWidth;
	if( m_Kind == sbVert )
	{
		if( m_bShowTop ) r.bottom -= sw;
		ret = ARect(r.left,r.bottom-sw,r.right,r.bottom);
	}
	else
	{
		if( m_bShowTop ) r.right -= sw;
		ret = ARect(r.right-sw,r.top,r.right,r.bottom);
	}
	return ret;
}

ARect AScrollBar::GetTopButton1Pos()
{
	ARect ret(0,0,0,0);
	if( !m_bShowTop ) return ret;
	ARect r = GetAbsPosition();
	int sw = AApplication::Get()->GetCurSkin()->m_ScrollBarWidth;
	if( m_Kind == sbVert )
	{
		ret = ARect(r.left,r.top,r.right,r.top+sw);
	}
	else
	{
		ret = ARect(r.left,r.top,r.left+sw,r.bottom);
	}
	return ret;
}

ARect AScrollBar::GetTopButton2Pos()
{
	ARect ret(0,0,0,0);
	if( !m_bShowTop ) return ret;
	ARect r = GetAbsPosition();
	int sw = AApplication::Get()->GetCurSkin()->m_ScrollBarWidth;
	if( m_Kind == sbVert )
	{
		ret = ARect(r.left,r.bottom-sw,r.right,r.bottom);
	}
	else
	{
		ret = ARect(r.right-sw,r.top,r.right,r.bottom);
	}
	return ret;
}

ARect AScrollBar::GetRailPos()
{
	ARect r = GetAbsPosition();
	int sw = AApplication::Get()->GetCurSkin()->m_ScrollBarWidth;
	if( m_Kind == sbVert )
	{
		if( m_bShowTop ) r.Deflate(0,sw,0,sw);
		if( m_bShowButton ) r.Deflate(0,sw,0,sw);
		r.Deflate(0,1,0,1);
	}
	else
	{
		if( m_bShowTop ) r.Deflate(sw,0,sw,0);
		if( m_bShowButton ) r.Deflate(sw,0,sw,0);
		r.Deflate(1,0,1,0);
	}
	return r;
}

int AScrollBar::GetRailLen()
{
	ARect r = GetRailPos();

	if( m_Kind == sbVert ) return r.GetHeight();
	else return r.GetWidth();

}

ARect AScrollBar::GetRailPos_1()
{
	ARect r = GetRailPos();
	int n = m_iPosition * GetRate();
	if( m_Kind == sbVert ) return ARect(r.left,r.top,r.right,r.top+n);
	else return ARect(r.left,r.top,r.left+n,r.bottom);
}



ARect AScrollBar::GetRailPos_2()
{
	ARect r = GetRailPos();
	int D = (int)(GetTrueSize() * GetRate());
	int n = m_iPosition * GetRate();
	if( m_Kind == sbVert ) return ARect(r.left,r.top+n+D,r.right,r.bottom);
	else return ARect(r.left+n+D,r.top,r.right,r.bottom);
}
ARect AScrollBar::GetSlideButtonPos()
{
	ARect r = GetRailPos();
	int n = m_iPosition * GetRate();
	int D = (int)(GetTrueSize() * GetRate());
	if( D < AApplication::Get()->GetCurSkin()->m_ScrollBarWidth )
		D = AApplication::Get()->GetCurSkin()->m_ScrollBarWidth;
	if( m_Kind == sbVert ) 
	{
		if( D > r.GetHeight() ) D = r.GetHeight();
		return ARect(r.left,r.top+n,r.right,r.top+n+D);
	}
	else
	{
		if( D > r.GetWidth() ) D = r.GetWidth();
		return ARect(r.left+n,r.top,r.left+n+D,r.bottom);
	}
}

bool AScrollBar::IsContainerScrollBar()
{
	bool bIsContainerScrollBar = false;
	AContainer* pContainer = dynamic_cast<AContainer*>(GetParent());
	if( pContainer != NULL )
	{
		if( m_Kind == sbVert )
		{
			if( pContainer->GetVScrollBar() == this ) bIsContainerScrollBar = true;
		}
		else
		{
			if( pContainer->GetHScrollBar() == this ) bIsContainerScrollBar = true;
		}
	}
	return bIsContainerScrollBar;
}
void AScrollBar::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	if( OnPaint.IsValid() )
		OnPaint.Call(this,pEvent);
	else
		AApplication::Get()->GetCurSkin()->DrawScrollBar(cs,this);
}

AControl* AScrollBar::_HitTest(int X,int Y)
{
	ARect r = GetAbsPosition();
	if( r.PtInRect(APoint(X,Y)) )
	{
		return this;
	}
	else return NULL;
}
void AScrollBar::DoMouseDown(AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL ) return;

	m_MouseOnElem = ButtonElemOnPos(evt->m_X,evt->m_Y);

	m_ptMouse = APoint(evt->m_X,evt->m_Y);
	m_ElemStatus = SCROLLBAR_STATUS_DOWN;
	m_iDownPosition = (int)m_iPosition;

	if( OnMouseDown.IsValid() )
		OnMouseDown.Call(this,pEvent);
	else
	{
	
		int delta = 0;
		if( m_MouseOnElem == SCROLLBAR_ELEM_BUTTON1 ) delta = -m_iMiniStep;
		else  if( m_MouseOnElem == SCROLLBAR_ELEM_BUTTON2 ) delta = m_iMiniStep;
		else  if( m_MouseOnElem == SCROLLBAR_ELEM_RAIL_1 ) delta = -m_iBigStep;
		else  if( m_MouseOnElem == SCROLLBAR_ELEM_RAIL_2 ) delta = m_iBigStep;

		if( m_MouseOnElem == SCROLLBAR_ELEM_TOP1 )
		{
			delta = -GetPosition();
		}
		else if( m_MouseOnElem == SCROLLBAR_ELEM_TOP2 )
		{
			delta = GetRange() - GetPosition();
		}

		if( delta != 0 )
		{
			_Change(delta);
		}
		
		Refresh();
	}
}
void AScrollBar::DoMouseUp(AEvent* pEvent)
{
	m_MouseOnElem = SCROLLBAR_ELEM_NONE;
	m_ElemStatus = SCROLLBAR_STATUS_NORMAL;
	m_ptMouse = APoint(0,0);
	m_iDownPosition = -1;
	Refresh();
}

void AScrollBar::DoMouseMove(AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL ) return;

	int iElem = ButtonElemOnPos(evt->m_X,evt->m_Y);
	if( evt->m_KeyStates.Has(ksLeftButton) ) iElem = m_MouseOnElem;
	if(iElem == SCROLLBAR_ELEM_NONE ) return;


	if( !(evt->m_KeyStates.Has(ksLeftButton) || evt->m_KeyStates.Has(ksRightButton) || evt->m_KeyStates.Has(ksMiddleButton)) )
		m_ElemStatus = SCROLLBAR_STATUS_OVER;
	if( evt->m_KeyStates.Has(ksLeftButton))
	{
		if( iElem == SCROLLBAR_ELEM_SLIDE )
		{
			int iSize= 0;
			if( m_Kind == sbVert ) iSize = evt->m_Y - m_ptMouse.y;
			else iSize = evt->m_X - m_ptMouse.x;

			if( GetRate() != 0.0 )
				iSize = (int)( (double)iSize / GetRate() );
			if( iSize != 0 )
			{
				m_iPosition = m_iDownPosition;
				_Change(iSize);
			}
		}
	}
	base_class::DoMouseMove(pEvent);
	Refresh();
}

void AScrollBar::DoMouseWheel(AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent *>(pEvent);
	if( evt == NULL ) return;

	if( evt->m_Delta == 0 ) return;


	int delta = 7864320;//for 上下滚动
	if( evt->m_KeyStates.Has(ksShift) ) delta = 7864316;//for 左右滚动 fix me later 为什么会有差异？
	int iSize = -(evt->m_Delta / delta * m_iMiniStep);//7864320 ? fix me later
	if( GetRate() != 0.0 ) iSize = (int)( (double)iSize/GetRate() );

	_Change(iSize);

	Refresh();
}
int AScrollBar::ButtonElemOnPos(int X,int Y)
{
	APoint pt(X,Y);
	if( m_bShowButton && GetButton1Pos().PtInRect(pt) ) return SCROLLBAR_ELEM_BUTTON1;
	if( m_bShowButton && GetButton2Pos().PtInRect(pt) ) return SCROLLBAR_ELEM_BUTTON2;
	if( m_bShowTop && GetTopButton1Pos().PtInRect(pt) ) return SCROLLBAR_ELEM_TOP1;
	if( m_bShowTop && GetTopButton2Pos().PtInRect(pt) ) return SCROLLBAR_ELEM_TOP2;

	if( GetSlideButtonPos().PtInRect(pt) ) return SCROLLBAR_ELEM_SLIDE;

	if( GetRailPos_1().PtInRect(pt)) return SCROLLBAR_ELEM_RAIL_1;
	if( GetRailPos_2().PtInRect(pt)) return SCROLLBAR_ELEM_RAIL_2;
	return SCROLLBAR_ELEM_NONE;
}

int AScrollBar::GetMouseOnElem()
{
	return m_MouseOnElem;
}

int AScrollBar::GetElemStatus()
{
	return m_ElemStatus;
}
void AScrollBar::DoEnter(AEvent* pEvent)
{
	base_class::DoEnter(pEvent);
}

void AScrollBar::DoLeave(AEvent* pEvent)
{
	//m_ElemStatus = SCROLLBAR_STATUS_NORMAL;
	//m_MouseOnElem = SCROLLBAR_ELEM_NONE;

	base_class::DoLeave(pEvent);
}

void AScrollBar::_Change(int iSize)
{
	//iSize代表的是VirtualSize中的对应大小
	if( iSize == 0 ) return;


	int pos = (int)GetPosition();
	if( pos + iSize < 0 ) iSize = -pos;
	else if( pos + iSize > (int)GetRange() ) iSize = (int)GetRange() - pos;

	int iDeltaPosition=iSize;
	SetPosition(pos + iSize);

	AMouseEvent evt;
	evt.m_Delta = iDeltaPosition;
	OnScroll.Call(this,&evt);

	AContainer* pc = dynamic_cast<AContainer*>(GetParent());
	if( pc )
	{
		pc->DoLayout();
	}
}

bool AScrollBar::GetShowButton()
{
	return m_bShowButton;
}
void AScrollBar::SetShowButton(bool v)
{
	m_bShowButton = v;
}
bool AScrollBar::GetShowTop()
{
	return m_bShowTop;
}
void AScrollBar::SetShowTop(bool v)
{
	m_bShowTop = v;
}

void AScrollBar::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == _T("Range") ) pValue->SetString( AStringVal(GetRange()) );
	else if( sPath == _T("Position")) pValue->SetString( AStringVal( GetPosition()) );
	else if( sPath == _T("OnScroll") ) pValue->SetString( OnScroll.GetMCF() );
}

void AScrollBar::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("Range") ) SetRange( AIntVal(pValue->GetString()));
	else if( sPath == _T("Position") ) SetPosition( AIntVal(pValue->GetString()) );
	else if( sPath == _T("OnScroll") ) OnScroll.SetMCF( pValue->GetString() ); 
}
void AScrollBar::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{

}