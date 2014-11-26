#include "StdAfx.h"

ALabel::ALabel(AComponent* pOwner):AControl(pOwner)
{
	m_TextAlign = taLeftMiddle;

	//Font.SetOnChange(this,(TOnControlFontChange)&ALabel::OnFontChange);

}

ALabel::~ALabel()
{

}

void ALabel::SetTextAlign(TTextAlign v)
{
	m_TextAlign = v;
}

TTextAlign ALabel::GetTextAlign()
{
	return m_TextAlign;
}

void ALabel::OnFontChange()
{

	m_pUICore->Refresh(GetAbsPosition());
}

void ALabel::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawLabel(cs,this);
}

void ALabel::SetPropFromNode(AMLBody* pNode)
{
	if( pNode == NULL ) return;
	base_class::SetPropFromNode(pNode);
	if( pNode->HasProp(_T("txal")) ) SetTextAlign( (TTextAlign)AIntVal(pNode->GetProp(_T("txal"))) );
}

void ALabel::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == _T("TextAlign") ) pValue->SetInt( (int)GetTextAlign() );
}

void ALabel::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("TextAlign") ) SetTextAlign( (TTextAlign)pValue->GetInt() );
}

void ALabel::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{
	base_class::ESCall(sPath,args,pRetValue);
}