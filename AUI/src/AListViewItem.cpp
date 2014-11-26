#include "StdAfx.h"

AListViewItem::AListViewItem(AComponent* pOwner):ALabel(pOwner)
{
	m_bSelected = false;
	m_iImageIndex = -1;

}

AListViewItem::~AListViewItem()
{

}

AListView* AListViewItem::GetListView()
{
	return dynamic_cast<AListView*>(((AControl *)GetParent())->GetParent());
}

void AListViewItem::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawListViewItem(cs,this);

}

AString AListViewItem::operator [](const AString& sColumn)
{
	int index=-1;
	AListView* pListView = GetListView();
	if( pListView == NULL ) throw AException("null ListView!");
	AListHeader* pHeader = pListView->GetHeader();

	AString s1 = sColumn;
	s1 = s1.ToLower();
	for(int i=0;i<pHeader->GetListHeaderItemCount();i++)
	{
		AListHeaderItem* pHeaderItem = pHeader->GetListHeaderItem(i);
		AString s2 = pHeaderItem->GetText();
		s2 = s2.ToLower();
		if( s1 == s2 )
		{
			index = i;
			break;
		}
	}

	return this->operator [](index);
}

AString AListViewItem::operator [](int i)
{
	if( i < 0 || i >= SubItems.GetCount() ) throw AException("[ListViewItem]Out of bound");
	return SubItems.GetItem(i);
}

void AListViewItem::SetSelected(bool v)
{
	m_bSelected = v;
}
bool AListViewItem::GetSelected()
{
	return m_bSelected;
}

void AListViewItem::SetImageIndex(int v)
{
	m_iImageIndex = v;
}
int AListViewItem::GetImageIndex()
{
	return m_iImageIndex;
}

void AListViewItem::DoDblClick(AEvent* pEvent)
{
	AControl* p = GetParent();
	AControl* pp = p->GetParent();
	pp->DoDblClick(pEvent);
}