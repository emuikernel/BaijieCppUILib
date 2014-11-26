#include "StdAfx.h"

AListBoxItem::AListBoxItem(AComponent* pOwner):ALabel(pOwner)
{
	m_bSelected = false;
}

AListBoxItem::~AListBoxItem()
{

}

void AListBoxItem::SetSelected(bool v)
{
	m_bSelected = v;
}
bool AListBoxItem::GetSelected()
{
	return m_bSelected;
}
void AListBoxItem::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	AListBox* pListBox = dynamic_cast<AListBox*>( GetParent() );
	if( pListBox && pListBox->OnItemPaint.IsValid() )
		pListBox->OnItemPaint.Call(this,pEvent);
	else
	{
		if( OnPaint.IsValid() )
		{
			OnPaint.Call(this,pEvent);
		}
		else
			AApplication::Get()->GetCurSkin()->DrawListBoxItem(cs,this);
	}
}
