#include "StdAfx.h"

AImageControl::AImageControl(AComponent* pOwner):AControl(pOwner)
{
	m_pGraphic = NULL;
	m_DrawStyle = idsNormal;
	TransparentColor = AColor(255,255,255,255);
	m_bTransparent = true;
}

AImageControl::~AImageControl()
{
	if( m_pGraphic ) m_pGraphic->Release();
}

void AImageControl::SetGraphic(AImage* p)
{
	if( m_pGraphic ) m_pGraphic->Release();
	m_pGraphic = p;
	if( p ) p->AddRef();
}

AImage* AImageControl::GetGraphic()
{
	return m_pGraphic;
}

void AImageControl::SetDrawStyle(TImageDrawStyle v)
{
	m_DrawStyle = v;
}
TImageDrawStyle AImageControl::GetDrawStyle()
{
	return m_DrawStyle;
}

void AImageControl::SetTransparent(bool v)
{
	m_bTransparent = v;
}
bool AImageControl::GetTransparent()
{
	return m_bTransparent;
}

void AImageControl::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawImage(cs,this);
}
AString AImageControl::GetImagePath()
{
	return m_sImagePath;
}

void AImageControl::SetImagePath(const AString& s)
{
	if( m_sImagePath != s )
	{
		m_sImagePath = s;
		m_sImagePath.Replace(_T("/"),_T("\\"));
		if( m_pGraphic )
		{
			m_pGraphic->Release();
			m_pGraphic = NULL;
		}
		m_pGraphic = AImage::CreateImage();
		m_pGraphic->AddRef();
		m_pGraphic->LoadFromFile(s);
	}
}

void AImageControl::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
	if( pNode->HasProp(_T("path")) )
		SetImagePath(pNode->GetProp(_T("path")));
	if( pNode->HasProp(_T("stretch")) )
		SetDrawStyle( AIntVal(pNode->GetProp(_T("stretch"))) == 1 ? idsStretch : idsNormal);
	if( pNode->HasProp(_T("autosize")))
	{
		if( AIntVal( pNode->GetProp(_T("autosize"))) )
		{
			if( m_pGraphic && m_pGraphic->IsValid() )
			{
				SetWidth(m_pGraphic->GetWidth());
				SetHeight(m_pGraphic->GetHeight());
			}
		}
	}
}

void AImageControl::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{

}

void AImageControl::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == _T("ImagePath") ) pValue->SetString( m_sImagePath );

}

void AImageControl::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("ImagePath") ) SetImagePath( pValue->GetString() );
}
