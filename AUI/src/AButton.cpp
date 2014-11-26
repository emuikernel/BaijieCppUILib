#include "StdAfx.h"

AButton::AButton(AComponent* pOwner):AControl(pOwner)
{
	m_pAction = NULL;
	m_pImage = NULL;
	m_pOverImage = NULL;
	m_pDownImage = NULL;
	m_pDisableImage = NULL;
	m_eImageAlign = alLeft;
	m_bTransparent = false;
}

AButton::~AButton()
{
	if( m_pImage )
	{
		m_pImage->Release();
		m_pImage = NULL;
	}
	if( m_pOverImage )
	{
		m_pOverImage->Release();
		m_pOverImage = NULL;
	}
	if( m_pDownImage )
	{
		m_pDownImage->Release();
		m_pDownImage = NULL;
	}
	if( m_pDisableImage )
	{
		m_pDisableImage->Release();
		m_pDisableImage = NULL;
	}
}

AAction* AButton::GetAction()
{
	return m_pAction;
}
void AButton::SetAction(AAction* pAction)
{
	m_pAction = pAction;
}
void AButton::DoPaint(ACanvas* cs,AEvent *pEvent)
{
	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		APaintEvent* e = dynamic_cast<APaintEvent*>(pEvent);
		if( e )
			AApplication::Get()->GetCurSkin()->DrawButton(cs,this);
	}

}

void AButton::SetImage(AImage* p)
{
	if( m_pImage != p )
	{
		if( m_pImage ) m_pImage->Release();
		m_pImage = p;
		if( m_pImage ) m_pImage->AddRef();
	}
}
AImage* AButton::GetImage()
{
	return m_pImage;
}

eAlign AButton::GetImageAlign()
{
	return m_eImageAlign;
}
void AButton::SetImageAlign(eAlign e)
{
	m_eImageAlign = e;
}

AString AButton::GetImagePath()
{
	return m_sImagePath;
}

void AButton::SetImagePath(const AString& s)
{
	if( m_sImagePath != s )
	{
		m_sImagePath = s;
		m_sImagePath.Replace(_T("/"),_T("\\"));
		if( m_pImage )
		{
			m_pImage->Release();
			m_pImage = NULL;
		}
		m_pImage = AImage::CreateImage();
		m_pImage->AddRef();
		m_pImage->LoadFromFile(s);
	}
}

void AButton::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
	if( pNode->HasProp(_T("imgurl")) )
		SetImagePath(pNode->GetProp(_T("imgurl")));
	if( pNode->HasProp(_T("overimgurl")) )
		SetOverImagePath(pNode->GetProp(_T("overimgurl")));
	if( pNode->HasProp(_T("downimgurl")) )
		SetDownImagePath(pNode->GetProp(_T("downimgurl")));
	if( pNode->HasProp(_T("disableimgurl")) )
		SetDisableImagePath(pNode->GetProp(_T("disableimgurl")));
	if( pNode->HasProp(_T("imgal")))
		SetImageAlign(AUTIL::StringToAlign(pNode->GetProp(_T("imgal"))));
	if( pNode->HasProp(_T("trs")))
		SetTransparent(pNode->GetProp(_T("trs")) == _T("1") ? true : false);
}

void AButton::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == _T("ImagePath") ) pValue->SetString( m_sImagePath );
	else if( sPath == _T("OverImagePath") ) pValue->SetString( m_sOverImage );
	else if( sPath == _T("DownImagePath") ) pValue->SetString( m_sDownImage );
	else if( sPath == _T("DisableImagePath") ) pValue->SetString( m_sDisableImage );
	else if( sPath == _T("ImageAlign") ) pValue->SetString( AUTIL::AlignToString(m_eImageAlign));
	else if( sPath == _T("Transparent")) pValue->SetString(m_bTransparent ? _T("1"):_T("0"));
}

void AButton::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("ImagePath") ) SetImagePath( pValue->GetString() );
	else if( sPath == _T("OverImagePath") ) SetOverImagePath( pValue->GetString() );
	else if( sPath == _T("DownImagePath") ) SetDownImagePath( pValue->GetString() );
	else if( sPath == _T("DisableImagePath") ) SetDisableImagePath( pValue->GetString() );
	else if( sPath == _T("ImageAlign") ) SetImageAlign(AUTIL::StringToAlign(pValue->GetString()));
	else if( sPath == _T("Transparent")) SetTransparent(pValue->GetString()==_T("1")?true:false);
}

bool AButton::GetTransparent()
{
	return m_bTransparent;
}
void AButton::SetTransparent(bool v)
{
	if( m_bTransparent != v )
	{
		m_bTransparent = v;
		Refresh();
	}
}
void AButton::DoClick(AEvent* pEvent)
{
	if( m_pAction && m_pAction->OnExecute.IsValid() )
		m_pAction->OnExecute.Call(this,pEvent);
	else
		base_class::DoClick(pEvent);
}

void AButton::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{

}

void AButton::SetOverImage(AImage* p)
{
	if( m_pOverImage != p )
	{
		if( m_pOverImage ) m_pOverImage->Release();
		m_pOverImage = p;
		if( m_pOverImage ) m_pOverImage->AddRef();
	}
}
AImage* AButton::GetOverImage()
{
	return m_pOverImage;
}
AString AButton::GetOverImagePath()
{
	return m_sOverImage;
}
void AButton::SetOverImagePath(const AString& s)
{
	if( m_sOverImage != s )
	{
		m_sOverImage = s;
		m_sOverImage.Replace(_T("/"),_T("\\"));
		if( m_pOverImage )
		{
			m_pOverImage->Release();
			m_pOverImage = NULL;
		}
		m_pOverImage = AImage::CreateImage();
		m_pOverImage->AddRef();
		m_pOverImage->LoadFromFile(s);
	}
}

void AButton::SetDownImage(AImage* p)
{
	if( m_pDownImage != p )
	{
		if( m_pDownImage ) m_pDownImage->Release();
		m_pDownImage = p;
		if( m_pDownImage ) m_pDownImage->AddRef();
	}
}
AImage* AButton::GetDownImage()
{
	return m_pDownImage;
}
AString AButton::GetDownImagePath()
{
	return m_sDownImage;
}
void AButton::SetDownImagePath(const AString& s)
{
	if( m_sDownImage != s )
	{
		m_sDownImage = s;
		m_sDownImage.Replace(_T("/"),_T("\\"));
		if( m_pDownImage )
		{
			m_pDownImage->Release();
			m_pDownImage = NULL;
		}
		m_pDownImage = AImage::CreateImage();
		m_pDownImage->AddRef();
		m_pDownImage->LoadFromFile(s);
	}
}

void AButton::SetDisableImage(AImage* p)
{
	if( m_pDisableImage != p )
	{
		if( m_pDisableImage ) m_pDisableImage->Release();
		m_pDisableImage = p;
		if( m_pDisableImage ) m_pDisableImage->AddRef();
	}
}
AImage* AButton::GetDisableImage()
{
	return m_pDisableImage;
}
AString AButton::GetDisableImagePath()
{
	return m_sDisableImage;
}
void AButton::SetDisableImagePath(const AString& s)
{
	if( m_sDisableImage != s )
	{
		m_sDisableImage = s;
		m_sDisableImage.Replace(_T("/"),_T("\\"));
		if( m_pDisableImage )
		{
			m_pDisableImage->Release();
			m_pDisableImage = NULL;
		}
		m_pDisableImage = AImage::CreateImage();
		m_pDisableImage->AddRef();
		m_pDisableImage->LoadFromFile(s);
	}
}