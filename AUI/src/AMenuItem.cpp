#include "StdAfx.h"
#include "AMenuItem.h"

AMenuItem::AMenuItem(AComponent* pOwner):AContainer(pOwner)
{
	m_iImageIndex = -1;
	m_pImageIndex = NULL;
	m_bChecked = false;
	m_TextAlign = taLeftMiddle;
	m_pPopupMenu = NULL;
	m_iHeight = 20;
	m_bInPopup = false;
	m_pAction = NULL;

	//Font.SetName(_T("ºÚÌå"));
	//Font.SetSize(14);
	
	//Font.SetOnChange(this,(TOnControlFontChange)&AMenuItem::OnFontChange);
}

AMenuItem::~AMenuItem()
{
	//
}

void AMenuItem::SetImageIndex(int iIndex)
{
	m_iImageIndex = iIndex;
}
int AMenuItem::GetImageIndex()
{
	return m_iImageIndex;
}

AImageList* AMenuItem::GetImageList()
{
	return m_pImageIndex;
}

void AMenuItem::SetImageList(AImageList* pImageList)
{
	m_pImageIndex = pImageList;
}

void AMenuItem::SetChecked(bool bChecked)
{
	m_bChecked = bChecked;
}

bool AMenuItem::GetChecked()
{
	return m_bChecked;
}

void AMenuItem::SetPopupMenu(AMenu* pMenu)
{
	m_pPopupMenu = pMenu;
	pMenu->SetParentMenuItem(this);
}

AMenu* AMenuItem::GetPopupMenu()
{
	return m_pPopupMenu;
}

bool AMenuItem::IsPopupState()
{
	return m_bInPopup;
}
void AMenuItem::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawMenuItem(cs,this);
}

void AMenuItem::SetTextAlign(TTextAlign v)
{
	m_TextAlign = v;
}

TTextAlign AMenuItem::GetTextAlign()
{
	return m_TextAlign;
}

void AMenuItem::OnFontChange()
{
	
	m_pUICore->Refresh(GetAbsPosition());
}

void AMenuItem::DoLeave(AEvent* pEvent)
{
	if( m_pPopupMenu != NULL )
	{
		m_bInPopup = false;
		m_pPopupMenu->HiddenPopups();
		m_pPopupMenu->SetVisible(false);
		Refresh();
	}
}

void AMenuItem::DoEnter(AEvent* pEvent)
{
	if( m_pPopupMenu != NULL )
	{
		ARect r = GetScreenPosition();
		int x = r.left;
		int y = r.top;
		//y += 2;
		x += r.GetWidth()+1;
		m_bInPopup = true;
		m_pPopupMenu->Popup(x,y);
		Refresh();
	}
}

void AMenuItem::DoClick(AEvent* pEvent)
{
	if( ( m_pAction && m_pAction->OnExecute.IsValid()) || OnClick.IsValid() )
	{
		AMenu* pMenu = dynamic_cast<AMenu*>(GetParent());
		while( pMenu )
		{
			pMenu->ClosePopup();

			AControl* pParent = dynamic_cast<AControl *>(pMenu->GetParent());
			if( pParent )
				pParent->GetUICore()->SetPopupControl(NULL);

			if( pMenu->GetParentMenuItem() != NULL )
				pMenu = dynamic_cast<AMenu*>( pMenu->GetParentMenuItem()->GetParent() );
			else
				pMenu = NULL;

			
		}
		if( m_pAction && m_pAction->OnExecute.IsValid() )
			m_pAction->OnExecute.Call(this,pEvent);
		else
			OnClick.Call(this,pEvent);
	}
}

void AMenuItem::DoMouseDown(AEvent* pEvent)
{
	AContainer::DoMouseDown(pEvent);
}

void AMenuItem::CreateChildren(AMLBody* pNode)
{
	for(int i=0;i<pNode->GetChildCount();i++)
	{
		AMLBody* pChildBody = pNode->GetChild(i);
		if( pChildBody->GetProp(_T("type")) != _T("Menu") ) continue;
		AMenu* pMenu = new AMenu(this);
		SetPopupMenu(pMenu);
		pMenu->SetPropFromNode(pChildBody);

		for(int j=0;j<pChildBody->GetChildCount();j++)
		{
			AMLBody* p = pChildBody->GetChild(j);
			if( p->GetName() == _T("children") )
				pMenu->CreateChildren(p);
		}
	}
}

void AMenuItem::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
}
AAction* AMenuItem::GetAction()
{
	return m_pAction;
}
void AMenuItem::SetAction(AAction* pAction)
{
	m_pAction = pAction;
}
