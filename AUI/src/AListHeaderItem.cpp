#include "StdAfx.h"

AListHeaderItem::AListHeaderItem(AComponent* pOwner):ALabel(pOwner)
{
	m_ContentTextAlign = taLeftMiddle;

}

AListHeaderItem::~AListHeaderItem()
{

}

void AListHeaderItem::SetContentTextAlign(TTextAlign al)
{
	m_ContentTextAlign = al;
}
TTextAlign AListHeaderItem::GetContentTextAlign()
{
	return m_ContentTextAlign;
}

void AListHeaderItem::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
}

void AListHeaderItem::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawListHeaderItem(cs,this);
}

