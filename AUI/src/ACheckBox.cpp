#include "StdAfx.h"

ACheckBox::ACheckBox(AComponent* pOwner):AContainer(pOwner)
{
	m_pImage = new AImageControl(this);
	m_pLabel = new ALabel(this);

	m_bChecked = false;

	m_pImage->OnPaint = AEventHandler(this,(FEventHandler)&ACheckBox::_PaintImage);
	//m_pLabel->Font.SetOnChange(this,(TOnControlFontChange)&ACheckBox::OnFontChange);
	m_pLabel->SetTextAlign(taLeftMiddle);

	m_pLabel->OnClick = AEventHandler(this,(FEventHandler)&ACheckBox::OnImageClick);
	m_pImage->OnClick = AEventHandler(this,(FEventHandler)&ACheckBox::OnImageClick);


	//Font.SetOnChange(this,(TOnControlFontChange)&ACheckBox::OnThisFontChange);

}

ACheckBox::~ACheckBox()
{
	//
}

ALabel* ACheckBox::GetLabel()
{
	return m_pLabel;
}

AImageControl* ACheckBox::GetImageControl()
{
	return m_pImage;
}

void ACheckBox::SetChecked(bool v)
{
	m_bChecked = v;
}
bool ACheckBox::GetChecked()
{
	return m_bChecked;
}
void ACheckBox::OnFontChange()
{
	CalcPos();
	m_pUICore->Refresh(GetAbsPosition());
}
void ACheckBox::OnThisFontChange()
{
	//m_pLabel->Font.Assign((AControlFont *)&Font);
}
void ACheckBox::CalcPos()
{
	int cy = m_pLabel->Font.GetSize();
	m_pImage->SetLeft(0);
	m_pImage->SetTop(0);
	m_pImage->SetWidth(cy-2);
	m_pImage->SetHeight(GetHeight());
	m_pLabel->SetLeft(m_pImage->GetWidth() + 4);
	m_pLabel->SetTop(0);
	m_pLabel->SetWidth(GetWidth() - m_pImage->GetWidth() - 4);
	m_pLabel->SetHeight(GetHeight());
}
void ACheckBox::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent *>(pEvent);
	if( evt == NULL ) return;

	OnPaint.Call(this,pEvent);

	AContainer::DoPaintChildren(pEvent);
}

void ACheckBox::OnImageClick(AObject* pSender,AEvent* pEvent)
{
	this->DoClick(pEvent);
}

void ACheckBox::DoClick(AEvent* pEvent)
{
	m_bChecked = !m_bChecked;
	OnClick.Call(this,pEvent);

	m_pUICore->Refresh(GetAbsPosition());
}
void ACheckBox::_PaintImage(AObject* pSender,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt )
		AApplication::Get()->GetCurSkin()->DrawCheckBoxImage(m_pUICore->GetCanvas(),m_pImage);
}


void ACheckBox::SetLeft(int v)
{
	AContainer::SetLeft(v);
}
void ACheckBox::SetTop(int v)
{
	AContainer::SetTop(v);
}

void ACheckBox::SetWidth(int v)
{
	AContainer::SetWidth(v);
	CalcPos();
}

void ACheckBox::SetHeight(int v)
{
	AContainer::SetHeight(v);
	CalcPos();
}

void ACheckBox::SetText(AString s)
{
	AContainer::SetText(s);
	m_pLabel->SetText(s);
}

void ACheckBox::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
	if( pNode == NULL ) return;
	if( pNode->HasProp(_T("ck")) ) SetChecked( pNode->GetProp(_T("ck")) == _T("1") ? true : false );//Checked
}

void ACheckBox::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == _T("Checked") ) pValue->SetString(GetChecked() ? _T("1"):_T("0"));
}

void ACheckBox::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("Checked") ) SetChecked( pValue->GetString() == _T("1") ? true : false );
}
void ACheckBox::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{

}