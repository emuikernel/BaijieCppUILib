#include "StdAfx.h"

#define ITEM_HEIGHT	16

AMenu::AMenu(AComponent* pOwner):base_class(pOwner)
{
	m_iWidth = 120;
	m_pParentMenuItem = NULL;
	m_iBorderWidth = 1;
	m_iPadding = 1;
}

AMenu::~AMenu()
{

}

void AMenu::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawMenu(cs,this);
	AContainer::DoPaintChildren(pEvent);

	int iCount = GetMenuItemCount();
	int i;
	for(i=0;i<iCount;i++)
	{
		AMenuItem* pMenuItem = (AMenuItem *)GetMenuItem(i);
		if( pMenuItem == NULL ) continue;
		pMenuItem->DoPaint(cs,pEvent);
	}
}

AMenuItem* AMenu::AddMenuItem()
{
	AMenuItem* pItem = new AMenuItem(this);
	pItem->SetAlign(alTop);
	return pItem;
}

void AMenu::RemoveMenuItem(AMenuItem* pItem)
{
	GetChildControls()->Delete(pItem);
}

void AMenu::DoLayout()
{
	if( !m_pUICore->GetLayoutEnabled() ) return;

	if( m_pWindow == NULL || GetHandle() == NULL ) return;

	int iMaxWidth = 0;
	ACanvas* cs = GetUICore()->GetCanvas();
	if( cs == NULL ) return;
	int iCount = GetMenuItemCount();
	int iHeight=0;
	int i;
	for(i=0;i<iCount;i++)
	{
		AMenuItem* pMenuItem = (AMenuItem *)GetMenuItem(i);
		if( pMenuItem == NULL ) continue;

		iHeight += pMenuItem->GetHeight();
		AFont f(pMenuItem->Font);
		ASize sz = cs->Use(&f)->MeasureText(pMenuItem->GetText());
		iMaxWidth = afc_max((int)sz.cx,iMaxWidth);
	}
	iMaxWidth += 110 + 28;

	SetWidth(iMaxWidth+(m_iBorderWidth+m_iPadding)*2);
	SetHeight(iHeight+(m_iBorderWidth+m_iPadding)*2);

	base_class::DoLayout();
}


void AMenu::DoLostFocus(AEvent* pEvent)
{
	bool bFound = false;
	for(int i=0;i<GetMenuItemCount();i++)
	{
		AMenuItem* pItem = (AMenuItem *)GetMenuItem(i);
		if( pItem == NULL ) continue;
		if( pItem->IsPopupState() )
		{
			bFound = true;
			break;
		}
	}
	if( !bFound )
	{
		ClosePopup();
		//递归判断是否需要释放上级菜单
		AMenuItem* pItem = m_pParentMenuItem;
		while( pItem )
		{
			AMenu* ppm = dynamic_cast<AMenu*>( pItem->GetParent() );
			if( ppm )
			{
				POINT pt;
				::GetCursorPos(&pt);
				ARect r(ppm->GetLeft(),ppm->GetTop(),ppm->GetLeft()+ppm->GetWidth(),ppm->GetTop()+ppm->GetHeight());
				if( !r.PtInRect(pt) ) 
				{
					ppm->ClosePopup();// DoLostFocus(pEvent);
				}
				pItem = ppm->GetParentMenuItem();
			}
			else
				break;
		}
	}
	OnKillFocus.Call(this,pEvent);
}

int AMenu::GetMenuItemCount()
{
	return GetChildControls()->GetCount();
}



AMenuItem* AMenu::GetMenuItem(int iIndex)
{
	return dynamic_cast<AMenuItem *>( GetChildControls()->GetItem(iIndex) );
}

void AMenu::SetParentMenuItem(AMenuItem* pItem)
{
	m_pParentMenuItem = pItem;
}

AMenuItem* AMenu::GetParentMenuItem()
{
	return m_pParentMenuItem;
}

AUICore* AMenu::GetParentUICore()
{
	if( m_pParentMenuItem != NULL ) return m_pParentMenuItem->GetUICore();
	return base_class::GetParentUICore();
}

void AMenu::HiddenPopups()
{
	for(int i=0;i<GetMenuItemCount();i++)
	{
		AMenuItem* pItem = (AMenuItem *)GetMenuItem(i);
		if( pItem == NULL ) continue;

		AMenu* pPop = pItem->GetPopupMenu();
		if( pPop != NULL ) 
		{
			pPop->HiddenPopups();
			pPop->SetVisible(false);
		}
	}
}

void AMenu::ClosePopup()
{
	int iCount = GetMenuItemCount();
	int i;
	for(i=0;i<iCount;i++)
	{
		AMenuItem* pItem = GetMenuItem(i);
		if( pItem )
		{
			AMenu* p = pItem->GetPopupMenu();
			if( p ) p->ClosePopup();
		}
	}

	base_class::ClosePopup();
}

void AMenu::CreateChildren(AMLBody* pNode)
{
	for(int i=0;i<pNode->GetChildCount();i++)
	{
		AMLBody* pChildBody = pNode->GetChild(i);
		if( pChildBody->GetProp(_T("type")) != _T("MenuItem") ) continue;
		AMenuItem* pMenuItem = this->AddMenuItem();
		pMenuItem->SetPropFromNode(pChildBody);

		for(int j=0;j<pChildBody->GetChildCount();j++)
		{
			AMLBody* p = pChildBody->GetChild(j);
			if( p->GetName() == _T("children") )
				pMenuItem->CreateChildren(p);
		}
	}

}

void AMenu::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
}