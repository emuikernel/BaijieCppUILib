#include "stdafx.h"

AMenuBar::AMenuBar(AComponent* pOwner):AContainer(pOwner)
{
	m_pPopupMenu = NULL;
}

AMenuBar::~AMenuBar()
{
	//
}

void AMenuBar::DoPopup(AMenu* p,int x,int y)
{
	//
}
AMenu* AMenuBar::AddMenu(const AString& sText)
{
	AMenu* pMenu = new AMenu(this);
	pMenu->SetText(sText);
	ALabel* pLabel = new ALabel(this);
	pLabel->SetText(sText);
	pLabel->OnPaint =  AEventHandler(this,(FEventHandler)&AMenuBar::OnLabelPaint);
	pLabel->OnClick = AEventHandler(this,(FEventHandler)&AMenuBar::OnLabelClick);
	pLabel->OnMouseMove = AEventHandler(this,(FEventHandler)&AMenuBar::OnLabelMouseMove);
	pLabel->OnMouseDown = AEventHandler(this,(FEventHandler)&AMenuBar::OnLabelMouseDown);
	pLabel->Font.SetColor(AColor(200,13,16,68));
	//pLabel->Font.SetName(_T("ºÚÌå"));
	//pLabel->Font.SetWidth(14);
	//pLabel->Font.SetBold(FW_BOLD);

	DoLayout();
	return pMenu;
}

void AMenuBar::RemoveMenu(const AString& sText)
{
	int i,iCount = GetChildControls()->GetCount();
	for(i=0;i<iCount;i++)
	{
		AMenu* pMenu = dynamic_cast<AMenu*>( GetChildControls()->GetItem(i));
		if( pMenu && pMenu->GetText() == sText )
		{
			GetChildControls()->Delete(GetChildControls()->IndexOf(pMenu));
			break;
		}
	}
}

void AMenuBar::RemoveAllMenu()
{
	bool b=true;
	while(b)
	{
		b = false;
		int i,iCount = GetChildControls()->GetCount();
		for(i=0;i<iCount;i++)
		{
			AMenu* pMenu = dynamic_cast<AMenu*>( GetChildControls()->GetItem(i));
			if( pMenu )
			{
				GetChildControls()->Delete(pMenu);
				b = true;
				break;
			}
		}
	}
}

AMenu* AMenuBar::GetMenu(const AString& sText)
{
	int i,iCount = GetChildControls()->GetCount();
	for(i=0;i<iCount;i++)
	{
		AMenu* pMenu = dynamic_cast<AMenu*>( GetChildControls()->GetItem(i));
		if( pMenu && pMenu->GetText() == sText )
		{
			return pMenu;
		}
	}
	return NULL;
}

void AMenuBar::DoLayout()
{
	if( !m_pUICore->GetLayoutEnabled() ) return;

	int x_space = 10;
	int y_space = 8;
	int i,iCount = GetChildControls()->GetCount();
	int x = x_space;
	AFont f(Font);
	ACanvas* cs = GetUICore()->GetCanvas();
	if( cs == NULL ) return;
	ARect rBar = GetControlPosition();

	for(i=0;i<iCount;i++)
	{
		ALabel* pMenuLabel = dynamic_cast<ALabel*>( GetChildControls()->GetItem(i));
		if( pMenuLabel )
		{
			ASize sz = cs->Use(&f)->MeasureText(pMenuLabel->GetText());
			ARect r;
			r.left = x - x_space/2;
			r.top = (rBar.GetHeight()-f.GetSize())/2-y_space/2;
			r.right = x + sz.cx + x_space/2;
			r.bottom = r.top + f.GetSize()+y_space/2;
			x += sz.cx + x_space;
			pMenuLabel->SetLeft(r.left);
			pMenuLabel->SetTop(r.top);
			pMenuLabel->SetWidth(r.GetWidth());
			pMenuLabel->SetHeight(r.GetHeight());
		}
	}
	base_class::DoLayout();
}

void AMenuBar::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		AApplication::Get()->GetCurSkin()->DrawMenuBar(cs,this);
	}
	AContainer::DoPaint(cs,pEvent);
}
void AMenuBar::DoClick(AEvent* pEvent)
{
	//
}

void AMenuBar::DoMouseMove(AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent *>(pEvent);
	if( evt == NULL ) return;

	if( evt->m_KeyStates.Has(ksLeftButton) )
	{
		//::SendMessage(GetUICore()->GetWin()->GetHandle(),WM_NCHITTEST,0,evt->m_lParam);
	}
	AContainer::DoMouseMove(pEvent);
	GetUICore()->Refresh(GetAbsPosition());
}

void AMenuBar::OnLabelPaint(AObject* p,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent *>(pEvent);
	if( evt == NULL ) return;
	AApplication::Get()->GetCurSkin()->DrawMenuBarLabel(m_pUICore->GetCanvas(),(AControl *)p);
}

void AMenuBar::OnLabelClick(AObject* p,AEvent* pEvent)
{
	ALabel* pLabel = dynamic_cast<ALabel*>(p);
	if( pLabel == NULL ) return;

	ARect r = pLabel->GetScreenPosition();
	AMenu* pMenu = GetMenu(pLabel->GetText());
	if( pMenu == NULL ) return;
	

	GetUICore()->SetPopupControl(NULL);
	if( pMenu->GetMenuItemCount() )
		pMenu->Popup(r.left,r.top+r.GetHeight());
	
}

void AMenuBar::OnLabelMouseDown(AObject* p,AEvent* pEvent)
{
	
}

void AMenuBar::OnLabelMouseMove(AObject* p,AEvent* pEvent)
{
	ALabel* pLabel = dynamic_cast<ALabel*>(p);
	if( pLabel == NULL ) return;

	//OutputDebugString(AString().Format(_T("\r\n --- On Label Move:%s"),pLabel->GetText().Text()));
	ARect r = pLabel->GetScreenPosition();
	AMenu* pMenu = GetMenu(pLabel->GetText());
	if( pMenu == NULL ) return;
	

	AControl* pPopup = GetUICore()->GetPopupControl();
	//OutputDebugString(AString().Format(_T("\r\n OnMenuLabel Move  popup=%p menu=%p µ±Ç°Label:%s"),pPopup,pMenu,pLabel->GetText().Text()));
	if( pPopup != NULL && pPopup != pMenu )
	{
		GetUICore()->SetPopupControl(NULL);
		if( pMenu->GetMenuItemCount() )
			pMenu->Popup(r.left,r.top+r.GetHeight());
	}
}

void AMenuBar::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
}

void AMenuBar::CreateChildren(AMLBody* pNode)
{
	for(int i=0;i<pNode->GetChildCount();i++)
	{
		AMLBody* pChildBody = pNode->GetChild(i);
		if( pChildBody->GetProp(_T("type")) != _T("Menu") ) continue;
		AMenu* pMenu = AddMenu( pChildBody->GetProp(_T("tx")) );
		pMenu->SetPropFromNode(pChildBody);

		for(int j=0;j<pChildBody->GetChildCount();j++)
		{
			AMLBody* p = pChildBody->GetChild(j);
			if( p->GetName() == _T("children") )
				pMenu->CreateChildren(p);
		}
	}
}
