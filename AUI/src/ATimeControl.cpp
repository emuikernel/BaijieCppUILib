#include "StdAfx.h"

ATimeControl::ATimeControl(AComponent* pOwner):base_class(pOwner),m_pFocusEdit(NULL)
{
	m_iBorderWidth = 1;
	m_pEditHour = new AEdit(this);
	m_pEditMinute =new AEdit(this);
	m_pEditSecond = new AEdit(this);
	m_pLabel1 = new ALabel(this);
	m_pLabel2 = new ALabel(this);
	m_pSpin = new ASpinControl(this);
	m_pSpin->SetAutoSpin(false);
	m_pSpin->SetWidth(11);
	m_pSpin->SetAlign( alRight );
	EVENT_CONNECT(m_pSpin,OnSpin,OnSpin);

	m_pEditHour->SetBorderWidth(0);
	m_pEditMinute->SetBorderWidth(0);
	m_pEditSecond->SetBorderWidth(0);
	m_pLabel1->SetText(L":");
	m_pLabel2->SetText(L":");

	ADateTime now = ADateTime::GetCurrentTime();
	m_pEditHour->SetText( AString().Format(L"%02d",now.GetHour()));
	m_pEditMinute->SetText( AString().Format(L"%02d",now.GetMinute()));
	m_pEditSecond->SetText( AString().Format(L"%02d",now.GetSecond()));

	EVENT_CONNECT(m_pEditHour,OnFocus,OnEditFocus);
	EVENT_CONNECT(m_pEditMinute,OnFocus,OnEditFocus);
	EVENT_CONNECT(m_pEditSecond,OnFocus,OnEditFocus);

	EVENT_CONNECT(m_pEditHour,OnKillFocus,OnEditKillFocus);
	EVENT_CONNECT(m_pEditMinute,OnKillFocus,OnEditKillFocus);
	EVENT_CONNECT(m_pEditSecond,OnKillFocus,OnEditKillFocus);
}

ATimeControl::~ATimeControl()
{

}

void ATimeControl::DoLayout()
{
	ARect r = GetAbsPosition();
	int nSpinWidth = m_pSpin->GetWidth();
	int h=r.GetHeight()-m_iBorderWidth*2-m_iPadding*2;
	int w=r.GetWidth()-nSpinWidth-m_iBorderWidth*2-m_iPadding*2;
	double dw = (double)(w) / (double)4.0;
	m_pEditHour->SetLeft(0);
	m_pEditHour->SetTop(0);
	m_pEditHour->SetWidth(dw);
	m_pEditHour->SetHeight(h);
	m_pLabel1->SetLeft(dw);
	m_pLabel1->SetTop(0);
	m_pLabel1->SetWidth(dw/2);
	m_pLabel1->SetHeight(h);
	m_pEditMinute->SetLeft(dw+dw/2);
	m_pEditMinute->SetTop(0);
	m_pEditMinute->SetWidth(dw);
	m_pEditMinute->SetHeight(h);
	m_pLabel2->SetLeft(dw*2+dw/2);
	m_pLabel2->SetTop(0);
	m_pLabel2->SetWidth(dw/2);
	m_pLabel2->SetHeight(h);
	m_pEditSecond->SetLeft(dw*3);
	m_pEditSecond->SetTop(0);
	m_pEditSecond->SetWidth(dw);
	m_pEditSecond->SetHeight(h);

	base_class::DoLayout();
}

void ATimeControl::OnSpin(AObject* pSender,AEvent* pEvent)
{
	AEdit* p = m_pFocusEdit;
	if( p == NULL ) p = m_pEditSecond;
	ASpinEvent* evt = dynamic_cast<ASpinEvent*>(pEvent);
	if( evt == NULL ) return;
	int v = AIntVal(p->GetText());
	if( evt->m_bUp ) v++;
	else v--;
	int minValue=0;
	int maxValue=59;
	if( p == m_pEditHour )
	{
		maxValue = 23;
	}
	if( v < minValue ) v = maxValue;
	if( v > maxValue ) v = minValue;
	p->SetText(AString().Format(L"%02d",v));
}

void ATimeControl::OnEditFocus(AObject* pSender,AEvent* pEvent)
{
	m_pFocusEdit = (AEdit*)pSender;
}

ADateTime ATimeControl::GetTime()
{
	int h,m,s;
	h = AIntVal(m_pEditHour->GetText());
	m = AIntVal(m_pEditMinute->GetText());
	s = AIntVal(m_pEditSecond->GetText());
	if( h < 0 ) h = 0;
	if( h > 23 ) h = 23;
	if( m < 0 ) m = 0;
	if( m > 59 ) m = 59;
	if( s < 0 ) s = 0;
	if( s > 59 ) s = 59;
	return ADateTime(1900,1,1,h,m,s);
}
void ATimeControl::SetTime(const ADateTime& dt)
{
	m_pEditHour->SetText( AString().Format(L"%02d",dt.GetHour()));
	m_pEditMinute->SetText( AString().Format(L"%02d",dt.GetMinute()));
	m_pEditSecond->SetText( AString().Format(L"%02d",dt.GetSecond()));
}

void ATimeControl::OnEditKillFocus(AObject* pSender,AEvent* pEvent)
{
	AEdit* p = (AEdit*)pSender;
	int minValue=0;
	int maxValue=59;
	if( p == m_pEditHour )
	{
		maxValue = 23;
	}
	int v = AIntVal(p->GetText());
	if( v < minValue )
	{
		v = minValue;
		p->SetText(AString().Format(L"%02d",v));
	}
	if( v > maxValue )
	{
		v = maxValue;
		p->SetText(AString().Format(L"%02d",v));
	}
}

void ATimeControl::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
	if( pNode->HasProp(L"time") ) 
	{
		AString s = pNode->GetProp(L"time");
		AStringArray sa;
		sa.FromString(s);
		if( sa.GetCount() == 3 )
		{
			m_pEditHour->SetText(sa[0]);
			m_pEditMinute->SetText( sa[1] );
			m_pEditSecond->SetText( sa[2] );
		}
	}
}
void ATimeControl::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == L"time" ) pValue->SetDouble( (double)GetTime().operator DATE() );
}
void ATimeControl::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == L"time" ) 
	{
		ADateTime dt;
		dt = (DATE)pValue->GetDouble();
		SetTime( dt );
	}
}
void ATimeControl::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{

}
