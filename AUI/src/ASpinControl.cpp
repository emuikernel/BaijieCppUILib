#include "StdAfx.h"

ASpinControl::ASpinControl(AComponent* pOwner):AContainer(pOwner)
{
	m_iBorderWidth=1;
	m_iPadding = 1;
	m_pDownButton = new AButton(this);
	m_pUpButton = new AButton(this);
	m_pUpButton->SetAlign( alTop );
	m_pDownButton->SetAlign( alBottom );
	EVENT_CONNECT(m_pUpButton,OnPaint,OnUpButtonPaint);
	EVENT_CONNECT(m_pDownButton,OnPaint,OnDownButtonPaint);
	EVENT_CONNECT(m_pUpButton,OnClick,OnButtonClick);
	EVENT_CONNECT(m_pDownButton,OnClick,OnButtonClick);

	m_bAutoSpin = true;
	m_iMin = 0;
	m_iMax = 0;
}

ASpinControl::~ASpinControl()
{

}

void ASpinControl::DoLayout()
{
	m_pUpButton->SetHeight( m_iHeight/2 );
	m_pDownButton->SetHeight( m_iHeight/2 );
	base_class::DoLayout();
}

void ASpinControl::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawSpinControl(cs,this);

	AContainer::DoPaintChildren(pEvent);
}

void ASpinControl::SetAutoSpin(bool v)
{
	m_bAutoSpin = v;
}
bool ASpinControl::GetAutoSpin()
{
	return m_bAutoSpin;
}

int ASpinControl::GetMinValue()
{
	return m_iMin;
}
void ASpinControl::SetMinValue(int v)
{
	m_iMin = v;
}

int ASpinControl::GetMaxValue()
{
	return m_iMax;
}
void ASpinControl::SetMaxValue(int v)
{
	m_iMax = v;
}

void ASpinControl::OnUpButtonPaint(AObject* pSender,AEvent* pEvent)
{
	ACanvas *cs = m_pUICore->GetCanvas();
	AApplication::Get()->GetCurSkin()->DrawSpinUpButton(cs,m_pUpButton);
}

void ASpinControl::OnDownButtonPaint(AObject* pSender,AEvent* pEvent)
{
	ACanvas *cs = m_pUICore->GetCanvas();
	AApplication::Get()->GetCurSkin()->DrawSpinDownButton(cs,m_pDownButton);
}

void ASpinControl::OnButtonClick(AObject* pSender,AEvent* pEvent)
{
	if( m_bAutoSpin )
	{
		AControl* p = _FindLeft();
		if( p )
		{
			AString s = p->GetText();
			int v = AIntVal(s);
			if( (AControl *)pSender == m_pUpButton ) 
			{
				v++;
				if( v > m_iMax ) v = m_iMin;
			}
			else 
			{
				v--;
				if( v < m_iMin ) v = m_iMax;
			}
			p->SetText( AStringVal(v) );
		}
	}
	ASpinEvent evt;
	evt.m_bUp = ((AControl *)pSender == m_pUpButton ? true : false);
	OnSpin.Call(this,&evt);
}

AControl* ASpinControl::_FindLeft()
{
	AControl* pLeft = NULL;
	AContainer* pc = dynamic_cast<AContainer*>(GetParent());
	ARect rThis = GetControlPosition();
	if( pc )
	{
		int i,iCount = pc->GetChildControls()->GetCount();
		int delta=10000;
		//找到离它最近的
		for(i=0;i<iCount;i++)
		{
			AControl* p = (AControl *)pc->GetChildControls()->GetItem(i);
			if( p == this ) continue;
			ARect r = p->GetControlPosition();
			int dw = abs(r.right-rThis.left);
			if( dw < delta ) 
			{
				delta = dw;
				pLeft = p;
			}
		}
	}
	return pLeft;
}


void ASpinControl::SetPropFromNode(AMLBody* pNode)
{
	if( pNode == NULL ) return;
	base_class::SetPropFromNode(pNode);
	if( pNode->HasProp(_T("autospin")) ) SetAutoSpin( AIntVal(pNode->GetProp(_T("autospin"))) );
	if( pNode->HasProp(_T("minv")) ) SetMinValue( AIntVal(pNode->GetProp(_T("minv"))) );
	if( pNode->HasProp(_T("maxv")) ) SetMaxValue( AIntVal(pNode->GetProp(_T("maxv"))) );
	PROP_SET(OnSpin);
}

void ASpinControl::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == _T("AutoSpin") ) pValue->SetInt( m_bAutoSpin ? 1 : 0 );
	else if( sPath == _T("MinValue") ) pValue->SetInt( m_iMin );
	else if( sPath == _T("MaxValue") ) pValue->SetInt( m_iMax );
}

void ASpinControl::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("AutoSpin") ) SetAutoSpin( pValue->GetInt() == 1 ? true : false );
	else if( sPath == _T("MinValue") ) SetMinValue( pValue->GetInt() );
	else if( sPath == _T("MaxValue") ) SetMaxValue( pValue->GetInt() );
}

void ASpinControl::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{
	base_class::ESCall(sPath,args,pRetValue);
}