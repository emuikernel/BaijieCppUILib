#include "StdAfx.h"

#define ITEM_HEIGHT	16

APopupListBox::APopupListBox(AComponent* pOwner):base_class(pOwner)
{
	m_pListBox = new AListBox(this);
	m_iBorderWidth = 0;
	m_iPadding = 0;

	EVENT_CONNECT(m_pListBox,OnSelChange,OnListBoxSelChange);
}

APopupListBox::~APopupListBox()
{

}

void APopupListBox::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	base_class::DoPaint(cs,pEvent);
}

int APopupListBox::GetItemCount()
{
	return m_pListBox->GetItemCount();
}
AString APopupListBox::GetItem(int index)
{
	return m_pListBox->GetItem(index);
}

void APopupListBox::AddItem(const AString& s)
{
	m_pListBox->AddString(s);
}

int APopupListBox::GetItemIndex()
{
	return m_pListBox->GetItemIndex();
}
void APopupListBox::SetItemIndex(int index)
{
	m_pListBox->SetItemIndex(index);
}
void APopupListBox::DoLayout()
{
	if( !m_pUICore->GetLayoutEnabled() ) return;

	if( m_pWindow == NULL )//|| GetHandle() == NULL ) 
		return;

	AComboBox* pParent = dynamic_cast<AComboBox *>(GetParent());
	if( pParent == NULL ) 
		return;

	ARect rParent = pParent->GetAbsPosition();
	int iWidth = rParent.GetWidth();
	rParent.left += pParent->GetUICore()->GetWin()->GetLeft();
	rParent.top += pParent->GetUICore()->GetWin()->GetTop()+pParent->GetHeight()-1;

	if( (int)pParent->GetDropDownWidth() != 0 ) iWidth = (int)pParent->GetDropDownWidth();
	int iHeight = 0;
	if( (int)pParent->GetDropDownHeight() != 0 ) iHeight = (int)pParent->GetDropDownHeight();
	else
	{
		iHeight = (int)m_pListBox->GetItemHeight() * (int)m_pListBox->GetChildControls()->GetCount()+2;
		if( iHeight < 1 ) iHeight = 10;
		if( iHeight > pParent->GetUICore()->GetWin()->GetHeight() - 50 )
			iHeight = pParent->GetUICore()->GetWin()->GetHeight() - 50;
	}
	iHeight += 5;
	int bp = m_iBorderWidth + m_iPadding;
	SetLeft(rParent.left);
	SetTop(rParent.top);
	SetWidth(iWidth + bp * 2);
	SetHeight(iHeight+ bp * 2);


	m_pListBox->SetLeft(bp);
	m_pListBox->SetTop(bp);
	m_pListBox->SetWidth(iWidth );
	m_pListBox->SetHeight(iHeight);
	m_pListBox->DoLayout();

	base_class::DoLayout();
}

void APopupListBox::OnListBoxSelChange(AObject* pSender,AEvent* pEvent)
{
	OnSelected.Call(this,pEvent);
}

AString APopupListBox::GetSelected()
{
	return m_pListBox->GetSelect();
}