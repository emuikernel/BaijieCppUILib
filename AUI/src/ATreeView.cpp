#include "StdAfx.h"

//===================== ATreeView ================
ATreeView::ATreeView(AComponent* pOwner):
		AContainer(pOwner),
		m_pHoveredItem(NULL),
		m_pSelectedItem(NULL)
{
	m_iIndent = 20;
	m_iPadding = 0;
	m_pImageList = NULL;

	SetAutoVScrollBar(true);
	SetAutoHScrollBar(true);

}

ATreeView::~ATreeView()
{

}

AImageList* ATreeView::GetImageList()
{
	return m_pImageList;
}

void ATreeView::SetImageList(AImageList* pImageList)
{
	m_pImageList = pImageList;
}


void ATreeView::SetIndent(int v)
{
	m_iIndent = v;
}
int ATreeView::GetIndent()
{
	return m_iIndent;
}

void ATreeView::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawTreeView(cs,this);


// 	int iCount = m_aChild.GetCount();
// 	for(int i=0;i<iCount;i++)
// 	{
// 		AControl* p = (AControl*)m_aChild[i];
// 		if( p && p->GetVisible() && m_pUICore->InDrawArea(p->GetAbsPosition()) ) 
// 		{
// 			AAutoClip cp(cs,p->GetAbsPosition());
// 			p->DoPaint(cs,pEvent);
// 		}
// 	}
	AContainer::DoPaintChildren(pEvent);
}

ATreeViewItem* ATreeView::AddItem(ATreeViewItem* pParentItem/* =NULL */,AString sText/* =_T("") */)
{
	ATreeViewItem* pItem = NULL;
	if( pParentItem == NULL )
	{
		pItem = new ATreeViewItem(this);
		m_aChild.Add(pItem);
		pItem->SetText( sText );
		pItem->m_pOwnerTree = this;
	}
	else 
		pItem = pParentItem->AddChild(sText);
	if( pItem )
	{
		pItem->m_pParentItem = pParentItem;
		DoLayout();
		m_pUICore->Refresh(GetAbsPosition());
	}
	return pItem;
}

ATreeViewItem* ATreeView::GetHoveredItem()
{
	return m_pHoveredItem;
}

void ATreeView::DoLayout()
{
	if( !m_pUICore->GetLayoutEnabled() ) return;

	ACanvas *cs= m_pUICore->GetCanvas();
	if( cs == NULL ) return;
	int iHeight = GetIndent();
	for(int i=0;i<m_aChild.GetCount();i++)
	{
		ATreeViewItem* pItem = dynamic_cast<ATreeViewItem *>((AControl*)m_aChild.GetItem(i));
		pItem->SetTop(iHeight);
		pItem->SetLeft(5);

		iHeight += _calcItem(pItem,cs)+5;
	}

	base_class::DoLayout();
}

int ATreeView::_calcItem(ATreeViewItem* pItem,ACanvas* pCanvas)
{
	if( pItem->GetVisible() == false )
	{
		for(int i=0;i<pItem->m_aChild.GetCount();i++)
		{
			ATreeViewItem* pi = (ATreeViewItem *)pItem->m_aChild.GetItem(i);
			pi->SetVisible(false);
			_calcItem(pi,pCanvas);
		}
		return 0;
	}
	ASize sz(0,0);
	bool b = false;
	if( pItem->m_pText->GetText() != _T(""))
	{
		AFont f(pItem->m_pText->Font);
		sz = pCanvas->Use(&f)->MeasureText(pItem->m_pText->GetText());
		sz.cx += 4;
		sz.cy += 2;
	}

	pItem->SetWidth( sz.cx + 32 );
	pItem->SetHeight( sz.cy );

	int iHeight = pItem->GetHeight() + 1;
	if( pItem->m_aChild.GetCount() > 0 )
	{
		if( (bool)pItem->GetExpanded() == true )
		{
			int iTop = pItem->GetTop() + iHeight;
			for(int i=0;i<pItem->m_aChild.GetCount();i++)
			{
				ATreeViewItem* pi = (ATreeViewItem *)pItem->m_aChild.GetItem(i);
				pi->SetVisible( true );
				pi->SetLeft( pItem->GetLeft() + GetIndent() );
				pi->SetTop( iTop );

				int h = _calcItem(pi,pCanvas)+1;
				iTop += h;
				iHeight += h;
			}

		}
		else
		{
			for(int i=0;i<pItem->m_aChild.GetCount();i++)
			{
				ATreeViewItem* pi = (ATreeViewItem *)pItem->m_aChild.GetItem(i);
				pi->SetVisible( false );
				_calcItem(pi,pCanvas);
			}
		}
	}

	return iHeight;
}

bool ATreeView::_walkItem(ATreeViewItem* pItem,FWalkTreeItemFunc f,void* pArg)
{
	if( !f(pItem,pArg) ) return false;
	for(int i=0;i<pItem->m_aChild.GetCount();i++)
	{
		ATreeViewItem* pi = (ATreeViewItem *)pItem->m_aChild.GetItem(i);
		if( !_walkItem(pi,f,pArg) ) return false;
	}
	return true;
}
void ATreeView::WalkItem(FWalkTreeItemFunc f,void* pArg)
{
	if( f )
	{
		for(int i=0;i<m_aChild.GetCount();i++)
		{
			ATreeViewItem* pi = dynamic_cast<ATreeViewItem *>((AControl*)m_aChild.GetItem(i));
			if( !_walkItem(pi,f,pArg) ) return;
		}
	}
}

void ATreeView::SetSelected(ATreeViewItem* pItem)
{
	if( m_pSelectedItem != pItem )
	{
		if( m_pSelectedItem ) m_pSelectedItem->SetSelected(false);
		if( pItem ) pItem->SetSelected(true);
		m_pSelectedItem = pItem;
		AEvent evt;
		OnChange.Call(this,&evt);
		Refresh();
	}
}

ATreeViewItem* ATreeView::GetSelected()
{
	return m_pSelectedItem;
}
void ATreeView::CreateChildren(AMLBody* pNode)
{
	for(int i=0;i<pNode->GetChildCount();i++)
	{
		AMLBody* pChildBody = pNode->GetChild(i);
		AString sType = pChildBody->GetProp(_T("type"));
		if( sType != _T("TreeViewItem") ) continue;
		ATreeViewItem* pItem = this->AddItem(NULL,pChildBody->GetProp(_T("tx")));
		pItem->SetPropFromNode(pChildBody);

		for(int j=0;j<pChildBody->GetChildCount();j++)
		{
			AMLBody* p = pChildBody->GetChild(j);
			if( p->GetName() == _T("children") )
				pItem->CreateChildren(p);
		}
	}
}

void ATreeView::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);

	PROP_SET(OnChange);
}


void ATreeView::MakeVisible(ATreeViewItem* pItem)
{

}

void ATreeView::RemoveAllItem()
{
	RemoveAllChild();
	m_aChild.Clear();
	m_pSelectedItem = NULL;
	DoLayout();
}

void ATreeView::RemoveItem(ATreeViewItem* pItem)
{
	if( pItem == NULL ) return;
	if( pItem == m_pSelectedItem ) m_pSelectedItem = NULL;
	ATreeViewItem* pParentItem = pItem->GetParentItem();
	if( pParentItem )
		pParentItem->m_aChild.Delete( pParentItem->m_aChild.IndexOf(pItem));
	else
		m_aChild.Delete(m_aChild.IndexOf(pItem));
	pItem->Destroy();
	DoLayout();
}

void ATreeView::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == _T("ItemIndex") ) ;
	else if( sPath == _T("OnChange") ) pValue->SetString( OnChange.GetMCF() );
}

void ATreeView::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("ItemIndex") ) ;
	else if( sPath == _T("OnChange") ) OnChange.SetMCF( pValue->GetString() ); 
}

void ATreeView::ESSetValue(AMLBody* pNode)
{

}

void ATreeView::ESGetValue(AMLBody* pNode)
{

}

void ATreeView::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{

}

//============================= ATreeViewEnumerator ============================
ATreeViewEnumerator::ATreeViewEnumerator(ATreeView* pView):base_class()
{
	m_pView = pView;
	m_bEnd = false;
	m_pCurItem = NULL;
	m_iIndex = -1;
}

ATreeViewEnumerator::~ATreeViewEnumerator()
{
	//
}

bool ATreeViewEnumerator::Next()
{
	if( !m_bEnd )
	{
		if( !NextNode(m_pCurItem) )
			m_bEnd = true;
	}
	return !m_bEnd;
}
ATreeViewItem* ATreeViewEnumerator::Cur()
{
	return m_pCurItem;
}

//采用的是深度优先遍历方法
bool ATreeViewEnumerator::NextNode(ATreeViewItem* pNode)
{
	if( pNode == NULL )
	{
		if( m_pView->m_aChild.GetCount() > 0 )
		{
			m_aParentIndex.Add( (void *)-1 );
			ATreeViewItem* p = dynamic_cast<ATreeViewItem*>((AControl *)m_pView->m_aChild.GetItem(0));
			while( p )
			{
				m_iIndex = 0;
				m_pCurItem = p;
				if( p->GetChildItemCount() > 0 )
				{
					p = p->GetChildItem(0);
					m_aParentIndex.Add( (void *)0 );
				}
				else
					p = NULL;
			}
			return true;
		}
		else
			return false;
	}
	else
	{
		//算法：
		// 1.向右走
		// 2.如果没的走了，则定位到父级
		// 3.如果有，则一直往它的子节点走，走到最深处
		ATreeViewItem* pRightNode=NULL;//右边节点
		ATreeViewItem* pParentItem = m_pCurItem->GetParentItem();
		if( pParentItem == NULL )
		{
			if( m_iIndex + 1 >= m_pView->m_aChild.GetCount() )
				return false;//向右没的走了，则End
			pRightNode = dynamic_cast<ATreeViewItem*>((AControl *)m_pView->m_aChild.GetItem(m_iIndex+1));
		}
		else
		{
			if( m_iIndex + 1 >= pParentItem->GetChildItemCount() )
			{
				//右边没有了
				pRightNode = NULL;
			}
			else
				pRightNode = pParentItem->GetChildItem(m_iIndex+1);
		}

		if( pRightNode == NULL )
		{
			//向右没的走了，则走到父级
			m_iIndex = (int)m_aParentIndex.GetItem( m_aParentIndex.GetCount()-1 );
			m_aParentIndex.Delete( m_aParentIndex.GetCount()-1 );
			m_pCurItem = m_pCurItem->GetParentItem();
		}
		else
		{
			//暂时定位到右边这个
			m_pCurItem = pRightNode;
			m_iIndex ++;
			//再定位到最深处（深度优先算法）
			while( m_pCurItem->GetChildItemCount() > 0 )
			{
				m_aParentIndex.Add( (void *)m_iIndex );
				//暂定到它的第一个子节点
				m_iIndex = 0;
				m_pCurItem = m_pCurItem->GetChildItem(0);
			}
		}
	}
	return true;
}
