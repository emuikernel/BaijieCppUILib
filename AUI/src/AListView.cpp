#include "StdAfx.h"

#define LISTITEM_WIDTH		240

AListView::AListView(AComponent* pOwner):AContainer(pOwner)
{
	m_ViewStyle = lvsReport;
	m_iItemHeight = 21;
	m_bMultiSelection = false;
	m_pLargeImageList = NULL;
	m_pSmallImageList = NULL;
	m_iPadding=0;
	m_iBorderWidth = 1;

	m_pItemContainer = new AContainer(this);
	m_pListHeader = new AListHeader(this);

	this->SetAutoVScrollBar(true);
	this->SetAutoHScrollBar(true);

	m_pVertScrollBar->SetWidth(AApplication::Get()->GetCurSkin()->m_ScrollBarWidth);
	m_pHorzScrollBar->SetHeight(AApplication::Get()->GetCurSkin()->m_ScrollBarWidth);

	m_pListHeader->SetFloatType(ftFloatY);
}

AListView::~AListView()
{
	if( m_pLargeImageList )
	{
		m_pLargeImageList->Release();
		m_pLargeImageList = NULL;
	}
	if( m_pSmallImageList )
	{
		m_pSmallImageList->Release();
		m_pSmallImageList = NULL;
	}
}

void AListView::SetLargeImageList(AImageList* pImageList)
{
	if( m_pLargeImageList ) m_pLargeImageList->Release();
	m_pLargeImageList = pImageList;
	if( m_pLargeImageList ) m_pLargeImageList->AddRef();
}
AImageList* AListView::GetLargeImageList()
{
	return m_pLargeImageList;
}
void AListView::SetSmallImageList(AImageList* pImageList)
{
	if( m_pSmallImageList ) m_pSmallImageList->Release();
	m_pSmallImageList = pImageList;
	if( m_pSmallImageList ) m_pSmallImageList->AddRef();
}
AImageList* AListView::GetSmallImageList()
{
	return m_pSmallImageList;
}

void AListView::SetViewStyle(TListViewStyle vs)
{
	m_ViewStyle = vs;
}
TListViewStyle AListView::GetViewStyle()
{
	return m_ViewStyle;
}

void AListView::SetItemHeight(int v)
{
	m_iItemHeight = v;
}
int AListView::GetItemHeight()
{
	return m_iItemHeight;
}
void AListView::SetMultiSelection(bool v)
{
	m_bMultiSelection = v;
}
bool AListView::GetMultiSelection()
{
	return m_bMultiSelection;
}

void AListView::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawListView(cs,this);

	AContainer::DoPaintChildren(pEvent);
}

void AListView::DoLayout()
{
	if( !m_pUICore->GetLayoutEnabled() ) return;
	switch(GetViewStyle())
	{
	case lvsReport:
		{
			m_pListHeader->SetVisible(true);
			m_pListHeader->SetLeft(0);
			m_pListHeader->SetTop(0);
			m_pListHeader->SetHeight(20);
			m_pListHeader->DoLayout();
			m_pItemContainer->SetLeft(10);
			m_pItemContainer->SetTop((int)m_pListHeader->GetHeight()+10);
			m_pItemContainer->SetWidth((int)m_pListHeader->GetWidth()-10);
			m_pItemContainer->SetHeight((int)GetItemHeight() * (int)m_pItemContainer->GetChildControls()->GetCount());
			int iItemCount = m_pItemContainer->GetChildControls()->GetCount();
			int iTop = 0;
			for(int i=0;i<iItemCount;i++)
			{
				AListViewItem* pItem = dynamic_cast<AListViewItem *>(m_pItemContainer->GetChildControls()->GetItem(i));
				if( pItem == NULL ) continue;
				pItem->SetLeft(0);
				pItem->SetTop(iTop);
				iTop += GetItemHeight();
				pItem->SetHeight(GetItemHeight());
				pItem->SetWidth(m_pItemContainer->GetWidth());
			}
		}
		break;
	case lvsList:
		{
			m_pListHeader->SetVisible(false);
			int iItemCount = m_pItemContainer->GetChildControls()->GetCount();
			int iCols = (int)GetWidth() / LISTITEM_WIDTH;
			if( iCols < 1 ) iCols = 1;
			m_pItemContainer->SetLeft(0);
			m_pItemContainer->SetTop(0);
			m_pItemContainer->SetWidth(iCols * LISTITEM_WIDTH);

			int iTop = 0;
			int iLeftIndex = 0;
			for(int i=0;i<iItemCount;i++)
			{
				AListViewItem* pItem = dynamic_cast<AListViewItem *>(m_pItemContainer->GetChildControls()->GetItem(i));
				if( pItem == NULL ) continue;
				pItem->SetLeft(iLeftIndex * LISTITEM_WIDTH);
				pItem->SetTop(iTop);
				iLeftIndex ++;
				if( iLeftIndex >= iCols )
				{
					iLeftIndex = 0;
					iTop += GetItemHeight();
				}
				pItem->SetHeight(GetItemHeight());
				pItem->SetWidth(LISTITEM_WIDTH);
			}

			m_pItemContainer->SetHeight((int)GetItemHeight() + iTop);
		}
		break;
	case lvsIcon:
		{
			m_pListHeader->SetVisible(false);
			int iItemCount = m_pItemContainer->GetChildControls()->GetCount();
			int iSpaceH = 10;//水平间距
			int iSpaceV = 20;//垂直间距
			ASize szIcon=ASize(32,32);
			if( GetLargeImageList() != NULL)
			{
				szIcon = GetLargeImageList()->Size;
			}
			int iCols = (int)this->GetWidth() / (szIcon.cx + iSpaceH);
			if( iCols < 1 ) iCols = 1;

			m_pItemContainer->SetLeft(10);
			m_pItemContainer->SetTop(10);
			int iTemp = iCols * (szIcon.cx + iSpaceH);
			m_pItemContainer->SetWidth(iTemp);

			int iTop = 0;
			int iLeftIndex = 0;
			for(int i=0;i<iItemCount;i++)
			{
				AListViewItem* pItem = dynamic_cast<AListViewItem *>(m_pItemContainer->GetChildControls()->GetItem(i));
				if( pItem == NULL ) continue;
				pItem->SetLeft(iLeftIndex * (szIcon.cx + iSpaceH));
				pItem->SetTop(iTop);
				iLeftIndex ++;
				if( iLeftIndex >= iCols )
				{
					iLeftIndex = 0;
					iTop += GetItemHeight()+szIcon.cy+iSpaceV;
				}
				pItem->SetHeight(GetItemHeight()+szIcon.cy);
				pItem->SetWidth(szIcon.cx + iSpaceH);
			}

			m_pItemContainer->SetHeight((int)GetItemHeight() +szIcon.cy+ iTop);
		}
		break;
	default:
		break;
	}

	base_class::DoLayout();
}

AListHeader* AListView::GetHeader()
{
	return m_pListHeader;
}

AObjectArray* AListView::GetItems()
{
	return m_pItemContainer->GetChildControls();
}
void AListView::RemoveAllItems()
{
	m_pItemContainer->RemoveAllChild();
}
AListViewItem* AListView::AddItem()
{
	AListViewItem* pItem = new AListViewItem(m_pItemContainer);
// 	pItem->SetLeft(0);
// 	pItem->SetTop((int)GetItemHeight() * ((int)m_pItemContainer->GetControls()->GetCount() -1));
// 	pItem->SetWidth(m_pItemContainer->GetWidth());
// 	pItem->SetHeight(GetItemHeight());
	pItem->OnClick = AEventHandler(this,(FEventHandler)&AListView::OnListItemClick);
	DoLayout();
	return pItem;
}

void AListView::OnListItemClick(AObject* p,AEvent* pEvent)
{
	AListViewItem* pItem = dynamic_cast<AListViewItem *>(p);
	if( pItem == NULL ) return;

	if( (bool)m_bMultiSelection ) pItem->SetSelected(( (bool)pItem->GetSelected() ? false : true));
	else
	{
		int i,iCount=m_pItemContainer->GetChildControls()->GetCount();
		for(i=0;i<iCount;i++)
		{
			AControl* pControl = dynamic_cast<AControl*>(m_pItemContainer->GetChildControls()->operator [](i) );
			if( pControl == (AControl*)pItem ) pItem->SetSelected(true);
			else
			{
				AListViewItem* p = dynamic_cast<AListViewItem*>(pControl);
				if( p ) p->SetSelected(false);
			}
		}
	}
	OnClick.Call(this,pEvent);
	pItem->GetUICore()->Refresh(GetAbsPosition());
}

AListViewItem* AListView::GetSelected()
{
	for(int i=0;i<m_pItemContainer->GetChildControls()->GetCount();i++)
	{
		AListViewItem* p = (AListViewItem*)m_pItemContainer->GetChildControls()->GetItem(i);
		if( p->GetSelected() ) return p;
	}
	return NULL;
}

int AListView::GetSelectedIndex()
{
	for(int i=0;i<m_pItemContainer->GetChildControls()->GetCount();i++)
	{
		AListViewItem* p = (AListViewItem*)m_pItemContainer->GetChildControls()->GetItem(i);
		if( p->GetSelected() ) return i;
	}
	return -1;
}

void AListView::SetSelected(AListViewItem* pSel)
{
	for(int i=0;i<m_pItemContainer->GetChildControls()->GetCount();i++)
	{
		AListViewItem* p = (AListViewItem*)m_pItemContainer->GetChildControls()->GetItem(i);
		if( p == pSel ) p->SetSelected(true);
		else p->SetSelected(false);
	}
}
void AListView::SetSelectedIndex(int index)
{
	for(int i=0;i<m_pItemContainer->GetChildControls()->GetCount();i++)
	{
		AListViewItem* p = (AListViewItem*)m_pItemContainer->GetChildControls()->GetItem(i);
		if( i == index ) p->SetSelected(true);
		else p->SetSelected(false);
	}
}
void AListView::CreateChildren(AMLBody* pNode)
{
	int i,iCount = pNode->GetChildCount();
	for(i=0;i<iCount;i++)
	{
		AMLBody* pChildBody = pNode->GetChild(i);
		AString sType = pChildBody->GetProp(_T("type"));
		if( sType == _T("ListHeadItem") )
		{
			AListHeaderItem* pItem = GetHeader()->AddItem();
			pItem->SetPropFromNode(pChildBody);
		}
	}
	for(i=0;i<iCount;i++)
	{
		AMLBody* pChildBody = pNode->GetChild(i);
		AString sType = pChildBody->GetProp(_T("type"));
		if( sType == _T("ListViewItem") )
		{
			AListViewItem* pItem = this->AddItem();
			pItem->SetPropFromNode(pChildBody);
		}
	}
}

void AListView::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);

	if( pNode->HasProp(_T("vs")) )
	{
		AString s = pNode->GetProp(_T("vs"));
		if( s == _T("r") ) SetViewStyle(lvsReport);
		else if( s == _T("i")) SetViewStyle(lvsIcon);
		else if( s == _T("l")) SetViewStyle(lvsList);
	}
}

void AListView::SetHeaders(const AString& s)
{
	AStringArray sa;
	sa.FromString(s,_T(","));
	
	m_pListHeader->RemoveAllChild();
	int i,iCount = sa.GetCount();
	for(i=0;i<iCount;i++)
	{
		AString s;
		s = sa.GetItem(i);
		AStringArray saItem;
		saItem.FromString(s,_T(":"));

		AString sName;
		if( saItem.GetCount() > 0 ) sName = saItem.GetItem(0);
		int iWidth=0;
		if( saItem.GetCount() > 1 ) iWidth = AIntVal(saItem.GetItem(1));
		if( sName.Length() )
		{
			AListHeaderItem* pItem = m_pListHeader->AddItem();
			pItem->SetWidth(iWidth);
			pItem->SetText(sName);
		}
	}
}

void AListView::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == _T("ViewStyle") ) pValue->SetString( AUTIL::ListViewStyleToString(m_ViewStyle) );
	else if( sPath == _T("Value") ) ESGetValue(pValue);
	else if( sPath == _T("ItemIndex") ) pValue->SetString( AStringVal(GetSelectedIndex()));
	else if( sPath == _T("Selected"))
	{
		AListViewItem* pItem = GetSelected();
		AStringArray sa;
		if( pItem )
		{
			sa.Add( pItem->GetText() );
			for(int i=0;i<pItem->SubItems.GetCount();i++)
				sa.Add( pItem->SubItems.GetItem(i));
		}
		pValue->SetArray(sa);
	}
}

void AListView::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("ViewStyle") ) SetViewStyle( AUTIL::StringToListViewStyle(pValue->GetString()));
	else if( sPath == _T("Headers") ) SetHeaders(pValue->GetString());
	else if( sPath == _T("Value") ) ESSetValue(pValue);
	else if( sPath == _T("Add") )
	{ 
		AStringArray sa;
		pValue->GetArray(sa);
		ESAdd(sa);
	}
	else if( sPath == _T("Del") )
	{ 
		int index = AIntVal(pValue->GetString());
		if( GetSelectedIndex() == index ) SetSelectedIndex(-1);
		m_pItemContainer->GetChildControls()->Delete(index);
		DoLayout();
	}
	else if( sPath == _T("Selected"))
	{
		AListViewItem* pItem = GetSelected();
		AStringArray sa;
		pValue->GetArray(sa);
		for(int i=0;i<sa.GetCount() && i < pItem->SubItems.GetCount()+1;i++)
		{
			if( i == 0 ) pItem->SetText(sa[i]);
			else pItem->SubItems.SetItem(i-1,sa[i]);
		}
		Refresh();
	}
	else if( sPath == _T("ItemIndex") ) SetSelectedIndex( AIntVal(pValue->GetString()));
	else if( sPath == _T("Clear") )
	{
		RemoveAllItems();
	}
}
void AListView::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{

}
void AListView::ESSetValue(AUI_ES_VALUE_INTF* pValue)
{
	RemoveAllItems();
	if( pValue == NULL ) return;
	if( pValue->GetType() == aetArray2 )
	{
		AObjectArray *oa = pValue->GetArray2();
		int iCount = oa->GetCount();
		for(int i=0;i<iCount;i++)
		{
			AStringArray* sa = dynamic_cast<AStringArray*>(oa->GetItem(i));
			if( sa )
			{
				int saCount = sa->GetCount();
				if( saCount > 0 )
				{
					AListViewItem* pItem = this->AddItem();
					for(int n=0;n<saCount;n++)
					{
						AString s;
						s = sa->GetItem(n);
						if( n == 0 ) pItem->SetText(s);
						else pItem->SubItems.Add(s);
					}
				}
				
			}
		}
	}
}
void AListView::ESGetValue(AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	int iCount = GetItems()->GetCount();
	pValue->SetType(aetArray);
	AObjectArray* oa = pValue->GetArray2();
	oa->Clear();
	for(int i=0;i<iCount;i++)
	{
		AListViewItem* pItem = dynamic_cast<AListViewItem*>( GetItems()->GetItem(i));
		if( pItem == NULL ) continue;

		AStringArray* sa = new AStringArray();
		oa->Add( sa );
		sa->Add( pItem->GetText() );

		for(int x=0;x<pItem->SubItems.GetCount();x++)
			sa->Add( pItem->SubItems.GetItem(x) );
	}

}

void AListView::ESAdd(AStringArray& sa)
{
	AListViewItem* pItem = AddItem();
	for(int i=0;i<sa.GetCount();i++)
	{
		AString s;
		s = sa[i];
		if( i == 0 ) pItem->SetText(s);
		else pItem->SubItems.Add(s);
	}
}

