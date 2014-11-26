#include "stdafx.h"
#include <map>

struct _ATimerInfo
{
	AObject* pObject;
	FOnTimerFunc func;
	AObject* pAsso;
};
class ATimerInternal : public AComponent
{
public:
	ATimerInternal(AComponent* pOwner):AComponent(pOwner)
	{
		//
	}
	virtual ~ATimerInternal()
	{
		//
	}
	static ATimerInternal* Get();

	std::map<UINT,_ATimerInfo>	m_aTimer;
};
ATimerInternal* g_pTimerInternal = NULL;

ATimerInternal* ATimerInternal::Get()
{
	if( g_pTimerInternal == NULL ) g_pTimerInternal = new ATimerInternal(AApplication::Get());
	return g_pTimerInternal;
}

ATimer::ATimer(AComponent* pOwner):AInvisibleControl(pOwner)
{
	//
}

ATimer::~ATimer()
{
	//
}

VOID CALLBACK _myTimerProc(
	_In_  HWND hwnd,
	_In_  UINT uMsg,
	_In_  UINT_PTR idEvent,
	_In_  DWORD dwTime
	)
{
	std::map<UINT,_ATimerInfo>::iterator it = ATimerInternal::Get()->m_aTimer.find(idEvent);
	if( it != ATimerInternal::Get()->m_aTimer.end() )
	{
		_ATimerInfo ti;
		ti = it->second;
		(*(ti.pObject).*(ti.func))(ti.pAsso,it->first);
	}
}

int ATimer::StartTimer(UINT nInterval,AObject* pEvtObject,FOnTimerFunc evtFunc,AObject* pAsso)
{
	int nRet = ::SetTimer(0,0,nInterval,_myTimerProc);
	_ATimerInfo ti;
	ti.pObject = pEvtObject;
	ti.func = evtFunc;
	ti.pAsso = pAsso;
	ATimerInternal::Get()->m_aTimer[nRet] = ti;
	return nRet;
}
void ATimer::EndTimer(int nTimerID)
{
	std::map<UINT,_ATimerInfo>::iterator it = ATimerInternal::Get()->m_aTimer.find(nTimerID);
	if( it != ATimerInternal::Get()->m_aTimer.end() )
	{
		 ATimerInternal::Get()->m_aTimer.erase(it);
		 ::KillTimer(NULL,nTimerID);
	}
}