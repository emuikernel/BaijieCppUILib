#include "StdAfx.h"

ADateControl::ADateControl(AComponent* pOwner):
		base_class(pOwner)
{
	m_Date = ADateTime::GetCurrentTime();
	m_pBox = new APopupDateBox(this);
	m_pBox->SetWidth(348);
	m_pBox->SetHeight(213);
	m_pBox->FillCell(m_Date.GetYear(),m_Date.GetMonth());
	//m_pListBox->OnKillFocus = AEventHandler(this,(FEventHandler)&AComboBox::OnListKillFocus);
	EVENT_CONNECT(m_pBox,OnSelected,OnBoxSelect);
}

ADateControl::~ADateControl()
{
}

void ADateControl::SetDate(const ADateTime& dt)
{
	m_Date = dt;
}
ADateTime ADateControl::GetDate()
{
	return m_Date;
}

void ADateControl::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent *>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		AApplication::Get()->GetCurSkin()->DrawDateControl(cs,this);
	}

	if( evt )
	{
		base_class::DoPaintChildren(pEvent);
	}
}

void ADateControl::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
	if( pNode->HasProp(_T("Date")) )
	{
		AStringArray sa;
		sa.FromString(pNode->GetProp(_T("Date")),_T("-"));
		if( sa.GetCount() == 3 )
		{
			ADateTime dt(AIntVal(sa[0]),AIntVal(sa[1]),AIntVal(sa[2]),0,0,0);
			SetDate(dt);
		}
	}
}

void ADateControl::DoClick(AEvent* pEvent)
{
	base_class::DoClick(pEvent);
	
	ARect r = GetScreenPosition();

	GetUICore()->SetPopupControl(NULL);
	m_pBox->Popup(r.left,r.top+r.GetHeight());
	m_pBox->FillCell(m_Date.GetYear(),m_Date.GetMonth());
}

void ADateControl::OnBoxSelect(AObject* pSender,AEvent* pEvent)
{
	m_Date = ADateTime(m_pBox->GetCurYear(),m_pBox->GetCurMonth(),m_pBox->GetCurDay(),0,0,0);
	Refresh();
}


void ADateControl::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == _T("Date") ) pValue->SetDouble( m_Date.operator DATE() );
}

void ADateControl::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("Date") ) m_Date = (DATE)(double)pValue->GetDouble();
}

void ADateControl::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{
	base_class::ESCall(sPath,args,pRetValue);
}
