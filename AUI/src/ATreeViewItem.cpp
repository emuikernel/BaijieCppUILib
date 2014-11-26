#include "StdAfx.h"

ATreeViewItem::ATreeViewItem(AComponent* pOwner):AContainer(pOwner),m_pOwnerTree(NULL)
{
	m_bExpanded = false;
	m_bSelected = false;

	m_pArrow = new AImageControl(this);
	m_pArrow->SetWidth(16);
	EVENT_CONNECT(m_pArrow,OnPaint,OnArrowPaint);
	EVENT_CONNECT(m_pArrow,OnClick,OnArrowClick);


	m_pIcon = new AImageControl(this);
	m_pIcon->SetWidth(16+6);//fix me later
	EVENT_CONNECT(m_pIcon,OnMouseDown,_OnMouseDown);
	EVENT_CONNECT(m_pIcon,OnMouseMove,_OnMouseMove);
	EVENT_CONNECT(m_pIcon,OnMouseUp,_OnMouseUp);
	EVENT_CONNECT(m_pIcon,OnClick,_OnClick);
	EVENT_CONNECT(m_pIcon,OnDblClick,_OnDblClick);

	m_pText = new ALabel(this);
	EVENT_CONNECT(m_pText,OnMouseDown,_OnMouseDown);
	EVENT_CONNECT(m_pText,OnMouseMove,_OnMouseMove);
	EVENT_CONNECT(m_pText,OnMouseUp,_OnMouseUp);
	EVENT_CONNECT(m_pText,OnClick,_OnClick);
	EVENT_CONNECT(m_pText,OnDblClick,_OnDblClick);

	m_pArrow->SetAlign(alLeft);
	m_pIcon->SetAlign(alLeft);
	m_pText->SetAlign(alClient);
	m_pParentItem = NULL;
	m_iImageIndex = -1;
}

ATreeViewItem::~ATreeViewItem()
{

}

int ATreeViewItem::GetImageIndex()
{
	return m_iImageIndex;
}

void ATreeViewItem::SetImageIndex(int v)
{
	m_iImageIndex = v;
	UpdateIcon();
}

ATreeViewItem* ATreeViewItem::GetParentItem()
{
	return m_pParentItem;
}
void ATreeViewItem::SetExpanded(bool v)
{
	if( v != m_bExpanded )
	{
		m_bExpanded = v;
		GetTree()->DoLayout();
	}
}
bool ATreeViewItem::GetExpanded()
{
	return m_bExpanded;
}

bool ATreeViewItem::GetSelected()
{
	return m_bSelected;
}
void ATreeViewItem::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawTreeViewItem(cs,this);
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
	//DebugStr(_T("drawing node:")+GetText());
	AContainer::DoPaintChildren(pEvent);
	//AApplication::Get()->GetCurSkin()->DrawTreeViewItemArrow(m_pUICore->GetCanvas(),(AControl *)m_pArrow);
}

ATreeViewItem* ATreeViewItem::AddChild(AString sText/* =_T("") */)
{
	ATreeViewItem* pItem = new ATreeViewItem(m_pOwnerTree);
	pItem->SetText(sText);
	pItem->m_pOwnerTree = m_pOwnerTree;
	pItem->m_pParentItem = this;
	m_aChild.Add((void *)pItem);
	return pItem;
}



static bool _selectTest(ATreeViewItem* pItem,void* pArg)
{
	pItem->SetSelected( ( (int)pArg == 1 ? true : false) );
	return true;
}

void ATreeViewItem::SetSelected(bool v)
{
	if( v != m_bSelected )
	{
		m_bSelected = v;
		//if( v ) m_pOwnerTree->SetSelected(this);
		m_pOwnerTree->Refresh();
	}

}

void ATreeViewItem::UpdateIcon()
{
	if( m_iImageIndex == -1 )
	{
		m_pIcon->SetGraphic(NULL);
	}
	else
	{
		AImageList* pImageList = GetTree()->GetImageList();
		if( pImageList )
		{
			AImage* pImage = pImageList->GetItem(m_iImageIndex);
			m_pIcon->SetGraphic(pImage);
		}
	}
}
ATreeView* ATreeViewItem::GetTree()
{
	return m_pOwnerTree;
}

int ATreeViewItem::GetChildItemCount()
{
	return (int)m_aChild.GetCount();
}

ATreeViewItem* ATreeViewItem::GetChildItem(int index)
{
	return (ATreeViewItem *)m_aChild[index];
}
void ATreeViewItem::OnArrowPaint(AObject* p,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	AApplication::Get()->GetCurSkin()->DrawTreeViewItemArrow(m_pUICore->GetCanvas(),(AControl *)p);
}

void ATreeViewItem::OnArrowClick(AObject* p,AEvent* pEvent)
{
	ATreeViewItem* pItem = dynamic_cast<ATreeViewItem*>(((AControl *)p)->GetParent());
	if( (bool)pItem->GetExpanded() ) pItem->SetExpanded( false );
	else pItem->SetExpanded( true );

	pItem->m_pOwnerTree->DoLayout();
	if( pItem->GetExpanded() ) OnExpand.Call(this,pEvent);
	else OnCollapse.Call(this,pEvent);

	m_pUICore->Refresh(GetAbsPosition());
}


void ATreeViewItem::SetText(AString s)
{
	m_pText->SetText(s);
	AContainer::SetText(s);
}

void ATreeViewItem::_OnMouseDown(AObject* pSender,AEvent* pEvent)
{
	if( pSender == (AObject*)m_pText || pSender == (AObject*)m_pIcon )
	{
		//ATreeViewItem* pItem = this;
		//pItem->SetSelected( true );//! (bool)pItem->GetSelected());
		GetTree()->SetSelected(this);
	}
	DoMouseDown(pEvent);
	GetTree()->DoMouseDown(pEvent);
	GetTree()->Refresh();
}
void ATreeViewItem::_OnMouseMove(AObject* pSender,AEvent* pEvent)
{
	DoMouseMove(pEvent);
	GetTree()->DoMouseMove(pEvent);
	GetTree()->Refresh();
}
void ATreeViewItem::_OnMouseUp(AObject* pSender,AEvent* pEvent)
{
	DoMouseUp(pEvent);
	GetTree()->DoMouseUp(pEvent);
	GetTree()->Refresh();
}
void ATreeViewItem::_OnClick(AObject* pSender,AEvent* pEvent)
{
	if( pSender == (AObject*)m_pText || pSender == (AObject*)m_pIcon )
	{
		this->SetExpanded(true);
	}
	GetTree()->SetSelected(this);
	OnClick.Call(this,pEvent);
	GetTree()->OnClick.Call(GetTree(),pEvent);
	GetTree()->Refresh();
}
void ATreeViewItem::_OnDblClick(AObject* pSender,AEvent* pEvent)
{
	if( pSender == (AObject*)m_pText || pSender == (AObject*)m_pIcon )
	{
		ATreeViewItem* pItem = this;
		pItem->SetSelected( true );//! (bool)pItem->GetSelected());
		//GetTree()->SetSelected( pItem );
	}
	OnDblClick.Call(this,pEvent);
	GetTree()->OnDblClick.Call(GetTree(),pEvent);
	GetTree()->Refresh();
}

void ATreeViewItem::CreateChildren(AMLBody* pNode)
{
	for(int i=0;i<pNode->GetChildCount();i++)
	{
		AMLBody* pChildBody = pNode->GetChild(i);
		if( pChildBody->GetProp(_T("type")) != _T("TreeViewItem") ) continue;
		ATreeViewItem* pItem = m_pOwnerTree->AddItem(this,pChildBody->GetProp(_T("tx")));
		pItem->SetPropFromNode(pChildBody);

		for(int j=0;j<pChildBody->GetChildCount();j++)
		{
			AMLBody* p = pChildBody->GetChild(j);
			if( p->GetName() == _T("children") )
				pItem->CreateChildren(p);
		}
	}
}

void ATreeViewItem::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
}

void ATreeViewItem::MakeVisible()
{

}