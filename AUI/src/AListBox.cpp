#include "StdAfx.h"

AListBox::AListBox(AComponent* pOwner):AContainer(pOwner)
{
	SetAutoVScrollBar(true);
	SetAutoHScrollBar(true);

	m_iItemHeight = 16;
	m_bMultiSelection = false;

	m_iBorderWidth = 1;
	m_iPadding = 1;
}

AListBox::~AListBox()
{

}

void AListBox::SetMultiSelection(bool v)
{
	m_bMultiSelection = v;
}
bool AListBox::GetMultiSelection()
{
	return m_bMultiSelection;
}
void AListBox::SetItemHeight(int v)
{
	m_iItemHeight = v;
}
int AListBox::GetItemHeight()
{
	return m_iItemHeight;
}
int AListBox::GetItemCount()
{
	return GetChildControls()->GetCount();
}
AString AListBox::GetItem(int index)
{
	AString s;
	AListBoxItem* pItem = dynamic_cast<AListBoxItem*>(GetChildControls()->GetItem(index));
	if( pItem ) s = pItem->GetText();
	return s;
}
void AListBox::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		AApplication::Get()->GetCurSkin()->DrawListBox(cs,this);
	}

	AContainer::DoPaintChildren(pEvent);
}

void AListBox::AddString(const AString& s)
{
	AListBoxItem* p = new AListBoxItem(this);
	p->SetText(s);
	p->SetHeight(GetItemHeight());
	p->SetVisible(true);
	p->OnClick = AEventHandler(this,(FEventHandler)&AListBox::OnItemClick);
	DoLayout();

	m_pUICore->Refresh(GetAbsPosition());

}

void AListBox::DoLayout()
{
	if( !m_pUICore->GetLayoutEnabled() ) return;
	int i,iCount=GetChildControls()->GetCount();
	int iMax=0;

	ACanvas *cs= m_pUICore->GetCanvas();
	if( cs == NULL ) return;
	for(i=0;i<iCount;i++)
	{
		AControl* pControl = dynamic_cast<AControl*>(GetChildControls()->operator [](i) );
		pControl->SetLeft(3);
		pControl->SetTop( i * GetItemHeight());
		SIZE sz;

		AFont f(pControl->Font ) ;
		sz = cs->Use(&f)->MeasureText(pControl->GetText());
		sz.cx += 4;
		sz.cy += 2;
		if( iMax < sz.cx ) iMax = sz.cx;

	}
	for(i=0;i<iCount;i++)
	{
		AControl* pControl = dynamic_cast<AControl*>(GetChildControls()->operator [](i) );
		pControl->SetWidth(iMax);
	}

	base_class::DoLayout();
}


void AListBox::OnItemClick(AObject* p,AEvent* pEvent)
{
	AListBoxItem* pItem = dynamic_cast<AListBoxItem *>(p);
	if( pItem == NULL ) return;

	if( m_bMultiSelection ) 
	{
		if( pItem->GetSelected() == false ) OnSelChange.Call(this,pEvent);
		pItem->SetSelected (((bool)pItem->GetSelected() ? false : true ) );
	}
	else
	{
		int i,iCount=GetChildControls()->GetCount();
		for(i=0;i<iCount;i++)
		{
			AControl* pControl = dynamic_cast<AControl*>(GetChildControls()->operator [](i) );
			if( pControl == (AControl*)pItem ) 
			{
				//if( pItem->GetSelected() == false ) 
				//	OnSelChange.Call(this,pEvent);
				pItem->SetSelected(true);
			}
			else
			{
				AListBoxItem* p = dynamic_cast<AListBoxItem *>(pControl);
				if( p ) p->SetSelected(false);
			}
		}
		OnSelChange.Call(this,pEvent);
	}

	pItem->GetUICore()->Refresh(GetAbsPosition());
	OnClick.Call(this,pEvent);
}


AString AListBox::GetSelect()
{
	AString s;
	int i,iCount=GetChildControls()->GetCount();
	for(i=0;i<iCount;i++)
	{
		AControl* pControl = dynamic_cast<AControl*>(GetChildControls()->operator [](i) );
		AListBoxItem* pItem = dynamic_cast<AListBoxItem*>(pControl);
		if( pItem && pItem->GetSelected() )
		{
			s = pItem->GetText();
			break;
		}
	}
	return s;
}

int AListBox::GetItemIndex()
{
	int index=-1;
	int i,iCount=GetChildControls()->GetCount();
	for(i=0;i<iCount;i++)
	{
		AControl* pControl = dynamic_cast<AControl*>(GetChildControls()->operator [](i) );
		AListBoxItem* pItem = dynamic_cast<AListBoxItem*>(pControl);
		if( pItem && pItem->GetSelected() )
		{
			index = i;
			break;
		}
	}
	return index;
}
void AListBox::SetItemIndex(int index)
{
	int i,iCount=GetChildControls()->GetCount();
	for(i=0;i<iCount;i++)
	{
		AControl* pControl = dynamic_cast<AControl*>(GetChildControls()->operator [](i) );
		AListBoxItem* pItem = dynamic_cast<AListBoxItem*>(pControl);
		if( pItem ) 
		{
			pItem->SetSelected(i == index ? true : false);
		}
	}
}

void AListBox::CreateChildren(AMLBody* pNode)
{
	base_class::CreateChildren(pNode);
	DoLayout();
}

void AListBox::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);

	PROP_SET(OnItemPaint);
	PROP_SET(OnSelChange);

}

void AListBox::SetSelected(const AString& s)
{
	int i,iCount=GetChildControls()->GetCount();
	for(i=0;i<iCount;i++)
	{
		AControl* pControl = dynamic_cast<AControl*>(GetChildControls()->operator [](i) );
		AListBoxItem* pItem = dynamic_cast<AListBoxItem*>(pControl);
		if( pItem  ) 
		{
			pItem->SetSelected(pItem->GetText() == s ? true : false );
		}
	}
}
void AListBox::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == _T("ItemIndex") ) pValue->SetString( AStringVal(GetItemIndex()) );
	else if( sPath == _T("Selected")) pValue->SetString( GetSelect() );
	else if( sPath == _T("OnSelChange") ) pValue->SetString( OnSelChange.GetMCF() );
}

void AListBox::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("ItemIndex") ) SetItemIndex( AIntVal(pValue->GetString()));
	else if( sPath == _T("Selected") ) SetSelected(pValue->GetString());
	else if( sPath == _T("Add") ) AddString(pValue->GetString());
	else if( sPath == _T("OnSelChange") ) OnSelChange.SetMCF( pValue->GetString() ); 
}

void AListBox::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{

}