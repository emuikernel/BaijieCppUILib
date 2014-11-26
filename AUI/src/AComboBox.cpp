#include "StdAfx.h"

AComboBox::AComboBox(AComponent* pOwner):
	base_class(pOwner)
{
	m_bCanEdit = false;
	m_pEdit = NULL;
	SetCanEdit(false);

	m_pButton = new AButton(this);
	m_pButton->OnPaint = AEventHandler(this,(FEventHandler)&AComboBox::OnButtonPaint);
	m_pButton->OnClick = AEventHandler(this,(FEventHandler)&AComboBox::OnButtonClick);

	m_pListBox = new APopupListBox(this);
	m_pListBox->SetName(_T("ListBox..."));
	//m_pListBox->OnKillFocus = AEventHandler(this,(FEventHandler)&AComboBox::OnListKillFocus);
	EVENT_CONNECT(m_pListBox,OnSelected,OnListSelect);

	m_iDropDownWidth = 0;
	m_iDropDownHeight = 0;
}

AComboBox::~AComboBox()
{

}

int AComboBox::GetItemCount()
{
	return m_pListBox->GetItemCount();
}
AString AComboBox::GetItem(int index)
{
	return m_pListBox->GetItem(index);
}

void AComboBox::SetDropDownWidth(int v)
{
	m_iDropDownWidth = v;
}
int AComboBox::GetDropDownWidth()
{
	return m_iDropDownWidth;
}
void AComboBox::SetDropDownHeight(int v)
{
	m_iDropDownHeight = v;
}
int AComboBox::GetDropDownHeight()
{
	return m_iDropDownHeight;
}

int AComboBox::GetItemIndex()
{
	return m_pListBox->GetItemIndex();
}

void AComboBox::SetItemIndex(int index)
{
	if( index == -1 )
	{
		SetText(_T(""));
		m_pListBox->SetItemIndex(index);
	}
	else
	{
		m_pListBox->SetItemIndex(index);
		AString s = m_pListBox->GetSelected();
		SetText(s);
	}
}

void AComboBox::SetText(AString s)
{
	base_class::SetText(s);
	if( m_pEdit ) m_pEdit->SetText(s);
}
bool AComboBox::GetCanEdit()
{
	return m_bCanEdit;
}
void AComboBox::SetCanEdit(bool v)
{
	if( v != m_bCanEdit )
	{
		m_bCanEdit = v;
		if( v )
		{
			m_pEdit = new AEdit(this);
			m_pEdit->OnPaint = AEventHandler(this,(FEventHandler)&AComboBox::OnEditPaint);
			m_pEdit->OnClick = AEventHandler(this,(FEventHandler)&AComboBox::OnButtonClick);
			m_pEdit->OnMouseDown = AEventHandler(this,(FEventHandler)&AComboBox::OnEditMouseDown);
			EVENT_CONNECT(m_pEdit,OnChange,OnEditChange);
		}
		else
		{
			if( m_pEdit ) 
			{
				m_pEdit->Destroy();
				m_pEdit = NULL;
			}
		}
		
	}
	
}

void AComboBox::OnEditChange(AObject* p,AEvent* pEvent)
{
	base_class::SetText( m_pEdit->GetText() );
	OnTextChange.Call(this,pEvent);
}

void AComboBox::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent *>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		AApplication::Get()->GetCurSkin()->DrawComboBox(cs,this);
	}

	if( evt )
	{
		AContainer::DoPaintChildren(pEvent);
	}
}

void AComboBox::SetWidth(int v)
{
	AContainer::SetWidth(v);
	RecalLayout();
}

void AComboBox::SetHeight(int v)
{
	AContainer::SetHeight(v);
	RecalLayout();
}

void	AComboBox::RecalLayout()
{
	if( m_pEdit )
	{
		m_pEdit->SetLeft(1);
		m_pEdit->SetTop(1);
		m_pEdit->SetWidth(GetWidth() - 16 - 2);
		m_pEdit->SetHeight(GetHeight() - 2);
	}
	

	m_pButton->SetLeft(GetWidth() - 1 - 16);
	m_pButton->SetTop(1);
	m_pButton->SetWidth(16);
	m_pButton->SetHeight(GetHeight() - 2);
}

void AComboBox::OnButtonPaint(AObject* p,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent *>(pEvent);
	if( evt == NULL ) return;
	AApplication::Get()->GetCurSkin()->DrawComboButton(m_pUICore->GetCanvas(),(AControl *)p);
}

void AComboBox::OnEditPaint(AObject* p,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent *>(pEvent);
	if( evt == NULL ) return;
	AApplication::Get()->GetCurSkin()->DrawComboEdit(m_pUICore->GetCanvas(),(AControl *)p);
}
void AComboBox::OnButtonClick(AObject* p,AEvent* pEvent)
{
	OnClick.Call(this,pEvent);
	ARect r = GetScreenPosition();

	GetUICore()->SetPopupControl(NULL);
	m_pListBox->Popup(r.left,r.top+r.GetHeight());
}

void AComboBox::OnListSelect(AObject* p,AEvent* pEvent)
{
	SetText(m_pListBox->GetSelected());
	m_pListBox->SetVisible(false);
	AEvent evt;
	//OnTextChange.Call(this,&evt);
	OnSelChange.Call(this,&evt);
	Refresh();
}

void AComboBox::OnListKillFocus(AObject* pSender,AEvent* pEvent)
{
	DebugStr(_T("关闭弹出的listbox 消除CaptureControl"));
	m_pListBox->SetVisible(false);
}
void AComboBox::AddString(const AString& s)
{
	m_pListBox->AddItem(s);
}

void AComboBox::OnEditMouseDown(AObject* pSender,AEvent* pEvent)
{
	if( OnMouseDown.IsValid())
	{
		AEdit* pEdit = dynamic_cast<AEdit*>(pSender);
		if( pEdit == NULL ) return;
		OnMouseDown.Call(this,pEvent);
	}
}

void AComboBox::DoClick(AEvent* pEvent)
{
	base_class::DoClick(pEvent);
	if( m_bCanEdit == false )
		OnButtonClick(this,pEvent);
}

void AComboBox::CreateChildren(AMLBody* pNode)
{
	base_class::CreateChildren(pNode);
	DoLayout();
}

void AComboBox::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);


	PROP_SET(OnSelChange);
	PROP_SET(OnTextChange);
}
