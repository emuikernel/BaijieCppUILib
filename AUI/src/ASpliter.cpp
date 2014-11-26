#include "StdAfx.h"

ASpliter::ASpliter(AComponent* pOwner):
	AContainer(pOwner),
	m_iOldPos(-1),
	m_iOldLength(-1)
{
	m_Align = alLeft;
	m_CursorType = ctSizeWE;
}

ASpliter::~ASpliter()
{

}
void ASpliter::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawSpliter(cs,this);
	AContainer::DoPaint(cs,pEvent);
}

void ASpliter::SetAlign(eAlign a)
{
	if( a == alLeft || a == alRight ) SetCursorType(ctSizeWE);
	else if( a == alTop || a == alBottom ) SetCursorType(ctSizeNS);
	AContainer::SetAlign(a);
}

void ASpliter::DoMouseMove(AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL ) return;

	AControl::DoMouseMove(pEvent);
	if( evt->m_KeyStates.Has(ksLeftButton) && m_iOldLength != -1 )
	{
		AControl* pBound = FindBoundControl();
		if( pBound == NULL || m_iOldLength == -1 ) return;

		int delta=0;
		ARect r = GetAbsPosition();
		int iMin=-1;
		AContainer* pParent = dynamic_cast<AContainer*>(GetParent());
		switch(GetAlign())
		{
		case alLeft:
			delta = evt->m_X - m_iOldPos;
			
			iMin = pParent->GetWidth();
			for(int i=0;i<pParent->GetChildControls()->GetCount();i++)
			{
				AControl* p = dynamic_cast<AControl*>(pParent->GetChildControls()->GetItem(i));
				if( p->GetAlign() != alRight ) continue;
				iMin = min(iMin,p->GetLeft());
			}
			if( pBound->GetLeft()+m_iOldLength+delta < iMin )
				pBound->SetWidth(m_iOldLength + delta);
			break;
		case alRight:
			delta = evt->m_X - m_iOldPos;
			
		
			iMin = 0;
			for(int i=0;i<pParent->GetChildControls()->GetCount();i++)
			{
				AControl* p = dynamic_cast<AControl*>(pParent->GetChildControls()->GetItem(i));
				if( p->GetAlign() != alLeft ) continue;
				iMin = max(iMin,p->GetLeft()+p->GetWidth());
			}
			if( pBound->GetLeft() +delta > iMin)
				pBound->SetWidth(m_iOldLength - delta);
			break;
		case alTop:
			delta = evt->m_Y - m_iOldPos;
			
	
			iMin = pParent->GetHeight();
			for(int i=0;i<pParent->GetChildControls()->GetCount();i++)
			{
				AControl* p = dynamic_cast<AControl*>(pParent->GetChildControls()->GetItem(i));
				if( p->GetAlign() != alBottom ) continue;
				iMin = min(iMin,p->GetTop());
			}
			if( pBound->GetTop()+m_iOldLength+delta < iMin)
				pBound->SetHeight( m_iOldLength + delta );
			break;
		case alBottom:
			delta = evt->m_Y - m_iOldPos;
			
			iMin = 0;
			for(int i=0;i<pParent->GetChildControls()->GetCount();i++)
			{
				AControl* p = dynamic_cast<AControl*>(pParent->GetChildControls()->GetItem(i));
				if( p->GetAlign() != alTop ) continue;
				iMin = max(iMin,p->GetTop()+p->GetHeight());
			}
			if( pBound->GetTop()+delta > iMin)
				pBound->SetHeight( m_iOldLength - delta );
			break;
		default:
			break;
		}
		AContainer* pc = dynamic_cast<AContainer*>(GetParent());
		pc->DoLayout();
		m_pUICore->Refresh(GetAbsPosition());
	}
}

void ASpliter::DoMouseDown(AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL ) return;

	AContainer::DoMouseDown(pEvent);
	if( evt->m_KeyStates.Has(ksLeftButton) )
	{
		AControl* pBound = FindBoundControl();
		if( pBound == NULL ) return;

		//m_pUICore->SetCaptureControl((AControl *)this);

		ARect r = GetAbsPosition();
		switch( GetAlign() )
		{
		case alLeft:
		case alRight:
			m_iOldPos = evt->m_X;
			if( pBound != NULL ) m_iOldLength = pBound->GetWidth();
			break;
		case alTop:
		case alBottom:
			m_iOldPos = evt->m_Y;
			if( pBound != NULL ) m_iOldLength = pBound->GetHeight();
			break;
		default:
			break;
		}
	}
}

AControl* ASpliter::FindBoundControl()
{
	AContainer* pc = dynamic_cast<AContainer*>(GetParent());
	if( pc == NULL ) return NULL;
	AControl* pPrev = NULL;
	for(int i=0;i<pc->GetChildControls()->GetCount();i++)
	{
		AControl* p = dynamic_cast<AControl *>(pc->GetChildControls()->GetItem(i));
		if( p->GetAlign() == this->GetAlign() )
		{
			if( p != (AControl *)this )
			{
				pPrev = p;
			}
			else
				break;
		}
	}
	return pPrev;
}

void ASpliter::DoMouseUp(AEvent* pEvent)
{
	AContainer::DoMouseUp(pEvent);
	m_iOldPos = -1;
	m_iOldLength = -1;
}
