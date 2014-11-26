#include "stdafx.h"

AForm::AForm(AComponent* pOwner):AWinControlBase(pOwner)
{
	m_iBorderWidth = 1;
	m_iPadding = 0;
	m_bVisible = false;
	
}

AForm::~AForm()
{
	AApplication* p = AApplication::Get();
	if( p->m_pMainForm == this )
	{
		p->m_pMainForm = NULL;
	}
}

void AForm::SetIcon(HICON hIcon)
{
	if( m_pWindow )
	{
		HICON h = (HICON)m_pWindow->SendMessage(WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
		if( h != NULL ) ::DeleteObject(h);
	}
}
AWindow* AForm::CreateWnd()
{
	return new AFormWindow(this);
}



void AForm::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		AApplication::Get()->GetCurSkin()->DrawForm(cs,this);
	}

	AWinControlBase::DoPaintChildren(pEvent);
}

void AForm::CenterToScreen()
{
	RECT rScreen;
	if( TRUE == SystemParametersInfo(SPI_GETWORKAREA,0,&rScreen,0) )
	{
		int dw = (rScreen.right-rScreen.left-GetWidth())/2;
		int dh = (rScreen.bottom-rScreen.top-GetHeight())/2;
		SetLeft(rScreen.left+dw);
		SetTop(rScreen.top+dh);
		::MoveWindow(GetUICore()->GetHandle(),GetLeft(),GetTop(),GetWidth(),GetHeight(),TRUE);
	}
}

void AForm::FromXML(AMLBody* pBody)
{
	SetPropFromNode(pBody);
	AMLBody* pChildren = pBody->FindBody(_T("children"));
	if( pChildren )
	{
		for(int i=0;i<pChildren->GetChildCount();i++)
		{
			AMLBody* pChildBody = pChildren->GetChild(i);
			CreateInstance(pChildBody,this);
		}
	}
}

void AForm::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);

	if( pNode->HasProp(_T("szb"))) m_bSizable = (AIntVal( pNode->GetProp(_T("szb"))) == 0 ? false : true);
	if( pNode->HasProp(_T("pos")))
	{
		AString s = pNode->GetProp(_T("pos"));//ø…ƒ‹ « d:default s:design c:CenterScreen
		if( s == _T("s") ) m_WindowPos = wpDesign;
		else if( s == _T("d") ) m_WindowPos = wpDefault;
		else if( s == _T("c")) m_WindowPos = wpCenterScreen;
	}
}

