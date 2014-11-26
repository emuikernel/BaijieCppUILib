#include "StdAfx.h"

ARadioButton::ARadioButton(AComponent* pOwner):AContainer(pOwner)
{
	m_pImage = new AImageControl(this);
	m_pLabel = new ALabel(this);

	m_bChecked = false;
	m_iGroup = 0;


	m_pImage->OnPaint = AEventHandler(this,(FEventHandler)&ARadioButton::_PaintImage);
	//m_pLabel->Font.SetOnChange(this,(TOnControlFontChange)&ARadioButton::OnFontChange);
	m_pLabel->SetTextAlign(taLeftMiddle);

	m_pLabel->OnClick = AEventHandler(this,(FEventHandler)&ARadioButton::OnImageClick);
	m_pImage->OnClick = AEventHandler(this,(FEventHandler)&ARadioButton::OnImageClick);


	//Font.SetOnChange(this,(TOnControlFontChange)&ARadioButton::OnThisFontChange);
}

ARadioButton::~ARadioButton()
{
	//
}

ALabel* ARadioButton::GetLabel()
{
	return m_pLabel;
}

AImageControl* ARadioButton::GetImageControl()
{
	return m_pImage;
}

void ARadioButton::OnFontChange()
{
	CalcPos();

	m_pUICore->Refresh(GetAbsPosition());
}
void ARadioButton::SetGroup(int v)
{
	m_iGroup = v;
}
int ARadioButton::GetGroup()
{
	return m_iGroup;
}
void ARadioButton::OnThisFontChange()
{
	//m_pLabel->Font.Assign((AControlFont *)&Font);
}
void ARadioButton::CalcPos()
{
	int cy = m_pLabel->Font.GetSize();
	m_pImage->SetLeft(0);
	m_pImage->SetTop(0);
	m_pImage->SetWidth( cy-2 );
	m_pImage->SetHeight( GetHeight() );
	m_pLabel->SetLeft( m_pImage->GetWidth() + 4 );
	m_pLabel->SetTop(0);
	m_pLabel->SetWidth( GetWidth() - m_pImage->GetWidth() - 4 );
	m_pLabel->SetHeight(GetHeight());
}
void ARadioButton::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		//
	}
	AContainer::DoPaintChildren(pEvent);
}

void ARadioButton::OnImageClick(AObject* pSender,AEvent* pEvent)
{
	this->DoClick(pEvent);
}
void ARadioButton::SetChecked(bool v)
{
	m_bChecked = v;
	if( v )
	{
		AContainer* pContainer = dynamic_cast<AContainer *>(GetParent());
		for(int i=0;i<pContainer->GetChildControls()->GetCount();i++)
		{
			AControl* pControl = (AControl *)(pContainer->GetChildControls()->operator [](i));
			ARadioButton* pr = dynamic_cast<ARadioButton *>(pControl);
			if( pr == NULL || pr == this || pr->GetGroup() != (int)m_iGroup ) continue;
			pr->SetChecked(false);
		}
	}
	m_pUICore->Refresh(GetAbsPosition());
}
bool ARadioButton::GetChecked()
{
	return m_bChecked;
}
void ARadioButton::DoClick(AEvent* pEvent)
{
	AContainer* pContainer = dynamic_cast<AContainer *>(GetParent());
	for(int i=0;i<pContainer->GetChildControls()->GetCount();i++)
	{
		AControl* pControl = (AControl *)(pContainer->GetChildControls()->operator [](i));
		ARadioButton* pr = dynamic_cast<ARadioButton *>(pControl);
		if( pr == NULL || pr == this || pr->GetGroup() != (int)m_iGroup ) continue;
		pr->SetChecked(false);
	}
	m_bChecked = true;

	OnClick.Call(this,pEvent);

	m_pUICore->Refresh(GetAbsPosition());
}
void ARadioButton::_PaintImage(AObject* pSender,HDC hDC)
{
	AApplication::Get()->GetCurSkin()->DrawRadioButtonImage(m_pUICore->GetCanvas(),m_pImage);
}


void ARadioButton::SetLeft(int v)
{
	AContainer::SetLeft(v);
}
void ARadioButton::SetTop(int v)
{
	AContainer::SetTop(v);
}

void ARadioButton::SetWidth(int v)
{
	AContainer::SetWidth(v);
	CalcPos();
}

void ARadioButton::SetHeight(int v)
{
	AContainer::SetHeight(v);
	CalcPos();
}

void ARadioButton::SetText(AString s)
{
	AContainer::SetText(s);
	m_pLabel->SetText(s);
}

void ARadioButton::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == _T("Checked") ) pValue->SetString( AStringVal(GetChecked() ? 1:0) );
	else if( sPath == _T("Group") ) pValue->SetInt( GetGroup() );
}

void ARadioButton::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("Checked") ) SetChecked( pValue->GetString()==_T("1") ? true : false );
	else if( sPath == _T("Group") ) SetGroup( pValue->GetInt() );
}
void ARadioButton::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{

}

void ARadioButton::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
	if( pNode->HasProp(_T("gr")) ) SetGroup( AIntVal(pNode->GetProp(_T("gr"))));
	if( pNode->HasProp(_T("chk")) ) SetChecked( AIntVal(pNode->GetProp(_T("chk"))) == 1 ? true : false );
}

