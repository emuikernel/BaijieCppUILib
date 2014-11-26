
#include "stdafx.h"

AEvent::AEvent()
{
	m_wParam = 0;
	m_lParam = 0;
}

AEvent::~AEvent()
{
	//
}

void AEvent::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( sPath == _T("Name") ) pValue->SetString(_T("Event"));
	else if( sPath == _T("lparam") ) pValue->SetInt(m_lParam);
	else if( sPath == _T("wparam") ) pValue->SetInt(m_wParam);
}

void AEvent::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
}

APaintEvent::APaintEvent():AEvent()
{
}

APaintEvent::~APaintEvent()
{
	//
}

void APaintEvent::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( sPath == _T("Name") ) pValue->SetString(_T("PaintEvent"));
	else base_class::ESGet(sPath,pValue);
}

void APaintEvent::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
}

ASpinEvent::ASpinEvent():AEvent()
{
	m_bUp = true;
}

ASpinEvent::~ASpinEvent()
{
	//
}

void ASpinEvent::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( sPath == _T("Name") ) pValue->SetString(_T("SpinEvent"));
	else if( sPath == _T("Up") ) pValue->SetInt( m_bUp ? 1 : 0 );
	else if( sPath == _T("Down")) pValue->SetInt(m_bUp ? 0 : 1 );
	else base_class::ESGet(sPath,pValue);
}

void ASpinEvent::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
}

ACloseQueryEvent::ACloseQueryEvent():AEvent()
{
	m_bCanClose = true;
}

ACloseQueryEvent::~ACloseQueryEvent()
{
	//
}
void ACloseQueryEvent::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( sPath == _T("Name") ) pValue->SetString(_T("CloseQueryEvent"));
	else if( sPath == _T("CanClose")) pValue->SetInt(m_bCanClose ? 1 : 0);
	else base_class::ESGet(sPath,pValue);
}

void ACloseQueryEvent::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( sPath == _T("CanClose") ) m_bCanClose = (pValue->GetInt() == 1 ? true : false);
	else base_class::ESSet(sPath,pValue);
}


ACloseEvent::ACloseEvent():AEvent()
{
	m_CloseAction = caFree;
}

ACloseEvent::~ACloseEvent()
{
	//
}
void ACloseEvent::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( sPath == _T("Name") ) pValue->SetString(_T("CloseEvent"));
	else if( sPath == _T("CloseAction")) pValue->SetInt((int)m_CloseAction);
	else base_class::ESGet(sPath,pValue);
}

void ACloseEvent::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( sPath == _T("CloseAction") ) m_CloseAction = (TCloseAction)pValue->GetInt();
	else base_class::ESSet(sPath,pValue);
}

AKeyEvent::AKeyEvent():AEvent()
{
	m_chKey = 0;
	m_Type = ketPress;
}

AKeyEvent::~AKeyEvent()
{
	//
}


void AKeyEvent::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( sPath == _T("Name") ) pValue->SetString(_T("KeyEvent"));
	else if( sPath == _T("KeyEventType")) pValue->SetInt((int)m_Type);
	else if( sPath == _T("Char") ) pValue->SetString(AString(m_chKey));
	else if( sPath == _T("Ctrl") ) pValue->SetInt( m_KeyStates.Has(ksCtrl) ? 1 : 0 );
	else if( sPath == _T("Alt") ) pValue->SetInt( m_KeyStates.Has(ksAlt) ? 1 : 0 );
	else if( sPath == _T("Shift") ) pValue->SetInt( m_KeyStates.Has(ksShift) ? 1 : 0 );
	else base_class::ESGet(sPath,pValue);
}

void AKeyEvent::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	base_class::ESSet(sPath,pValue);
}
AMouseEvent::AMouseEvent():AEvent()
{
	m_X = 0;
	m_Y = 0;
}

AMouseEvent::~AMouseEvent()
{
	//
}
void AMouseEvent::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( sPath == _T("Name") ) pValue->SetString(_T("MouseEvent"));
	else if( sPath == _T("Delta")) pValue->SetInt((int)m_Delta);
	else if( sPath == _T("X") ) pValue->SetInt(m_X);
	else if( sPath == _T("Y") ) pValue->SetInt(m_Y);
	else if( sPath == _T("Ctrl") ) pValue->SetInt( m_KeyStates.Has(ksCtrl) ? 1 : 0 );
	else if( sPath == _T("Alt") ) pValue->SetInt( m_KeyStates.Has(ksAlt) ? 1 : 0 );
	else if( sPath == _T("Shift") ) pValue->SetInt( m_KeyStates.Has(ksShift) ? 1 : 0 );
	else base_class::ESGet(sPath,pValue);
}

void AMouseEvent::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	base_class::ESSet(sPath,pValue);
}

ATextEvent::ATextEvent():AEvent()
{
	action = taInsert;
}

ATextEvent::~ATextEvent()
{

}

void ATextEvent::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( sPath == _T("Name") ) pValue->SetString(_T("TextEvent"));
	else if( sPath == _T("Action")) pValue->SetInt((int)action);
	else if( sPath == _T("StartRow") ) pValue->SetInt(seatStart.iRow);
	else if( sPath == _T("StartCol") ) pValue->SetInt(seatStart.iCol);
	else if( sPath == _T("EndRow") ) pValue->SetInt(seatEnd.iRow);
	else if( sPath == _T("EndCol") ) pValue->SetInt(seatEnd.iCol);
	else base_class::ESGet(sPath,pValue);
}

void ATextEvent::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	base_class::ESSet(sPath,pValue);
}
ADragEvent::ADragEvent():AEvent()
{
	m_pSource = NULL;
	m_szDrawSize = ASize(0,0);
	m_X = 0;
	m_Y = 0;
	m_bCanceled = false;
	m_bAccept = false;
}

ADragEvent::~ADragEvent()
{

}
void ADragEvent::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( sPath == _T("Name") ) pValue->SetString(_T("DragEvent"));
	else if( sPath == _T("Source") ) pValue->SetString( m_pSource ? ((AControl *)m_pSource)->GetName() : _T(""));
	else if( sPath == _T("X") ) pValue->SetInt(m_X);
	else if( sPath == _T("Y") ) pValue->SetInt(m_Y);
	else if( sPath == _T("Canceled") ) pValue->SetInt( m_bCanceled ? 1 : 0 );
	else if( sPath == _T("Accept") ) pValue->SetInt(m_bAccept ? 1 : 0 );
	else base_class::ESGet(sPath,pValue);
}

void ADragEvent::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( sPath == _T("Canceled") ) m_bCanceled = (pValue->GetInt() == 1 ? true : false);
	else if( sPath == _T("Accept") ) m_bAccept = (pValue->GetInt() == 1 ? true : false);
	else base_class::ESSet(sPath,pValue);
}

AUpdateEvent::AUpdateEvent():base_class()
{
	Enabled = true;
	Checked = false;
}

AUpdateEvent::~AUpdateEvent()
{

}

void AUpdateEvent::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( sPath == _T("Name") ) pValue->SetString(_T("UpdateEvent"));
	else if( sPath == _T("Enabled")) pValue->SetInt( Enabled ? 1 : 0 );
	else if( sPath == _T("Checked") ) pValue->SetInt( Checked ? 1 : 0 );
	else base_class::ESGet(sPath,pValue);
}

void AUpdateEvent::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	base_class::ESSet(sPath,pValue);
}

AEventHandler::AEventHandler()
{
	m_pObj = NULL;
	m_pFunc = NULL;
}

AEventHandler::~AEventHandler()
{
	//
}

AEventHandler::AEventHandler(AObject* pObj,FEventHandler func)
{
	m_pObj = pObj;
	m_pFunc = func;
}
AEventHandler::AEventHandler(const AEventHandler& e)
{
	m_pObj = e.m_pObj;
	m_pFunc = e.m_pFunc;
}
AEventHandler& AEventHandler::operator = (const AEventHandler& e)
{
	m_pObj = e.m_pObj;
	m_pFunc = e.m_pFunc;
	return *this;
}

bool AEventHandler::IsValid()
{
	return ( ( (m_pObj != NULL && m_pFunc != NULL) || m_sMCF != _T("") ) ? true : false);
}

void AEventHandler::Call(AObject* pSender,AEvent* pEvent)
{
	if( IsValid() )
	{
		if( m_pObj != NULL && m_pFunc != NULL )
		{
			AControl* p = dynamic_cast<AControl*>( pSender );
			if( p && p->GetUICore()->GetDisableEvent() == true ) 
				return;
			(*(m_pObj).*(m_pFunc))(pSender,pEvent);
		}
		else
		{
			TCallMCF c = AApplication::Get()->GetMCFCall();
			if( c )
				c( m_sMCF,pSender,pEvent );
		}
		
	}
}

void AEventHandler::SetMCF(const AString& s)
{
	m_sMCF = s;
}

AString AEventHandler::GetMCF()
{
	return m_sMCF;
}

