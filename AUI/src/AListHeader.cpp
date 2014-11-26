#include "StdAfx.h"

AListHeader::AListHeader(AComponent* pOwner):AContainer(pOwner)
{


}

AListHeader::~AListHeader()
{

}

int AListHeader::GetListHeaderItemCount()
{
	return GetChildControls()->GetCount()/2;
}
AListHeaderItem* AListHeader::GetListHeaderItem(int index)
{
	if( index < 0 || index > GetChildControls()->GetCount()/2 ) return NULL;
	return dynamic_cast<AListHeaderItem*>(GetChildControls()->operator [](index*2));
}

void AListHeader::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawListHeader(cs,this);
	AContainer::DoPaintChildren(pEvent);
}

void AListHeader::DoLayout()
{
	if( !m_pUICore->GetLayoutEnabled() ) return;
	int iCount = GetChildControls()->GetCount();
	int cx = 0;
	for(int i=0;i<iCount;i++)
	{
		AControl* pControl = dynamic_cast<AControl*>(GetChildControls()->GetItem(i));
		pControl->SetLeft(cx);
		pControl->SetTop(0);
		pControl->SetHeight(GetHeight());
		cx += (int)pControl->GetWidth();
	}
	SetWidth(cx);
	base_class::DoLayout();
}

AListHeaderItem* AListHeader::AddItem()
{
	AListHeaderItem* pItem = new AListHeaderItem(this);
	pItem->SetAlign(alLeft);
	ASpliter* sp = new ASpliter(this);
	sp->SetAlign(alLeft);
	sp->SetWidth(2);
	EVENT_CONNECT(sp,OnMouseUp,OnSplitMouseUp);
	EVENT_CONNECT(sp,OnPaint,OnSplitPaint);
	return pItem;
}

void AListHeader::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
}

void AListHeader::CreateChildren(AMLBody* pNode)
{
	base_class::CreateChildren(pNode);
}

void AListHeader::OnSplitMouseUp(AObject* pSender,AEvent* pEvent)
{
	AControl* p = (AControl*)GetChildControls()->GetItem(GetChildControls()->GetCount()-1);
	SetWidth(p->GetLeft()+p->GetWidth());
	AListView* pView = dynamic_cast<AListView*>(GetParent());
	if( pView )
	{
		pView->DoLayout();
	}
}

void AListHeader::OnSplitPaint(AObject* pSender,AEvent* pEvent)
{

}