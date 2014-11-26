#include "StdAfx.h"
#include "AObject.h"
#include <map>

#ifdef _DEBUG
class AMemCheck
{
public:
	AMemCheck()
	{

	}
	~AMemCheck()
	{
		
	}
	void CheckMemoryLeak()
	{
		AString s;
		s += _T("\r\nnow checking memory leak.....");
		//check mem
		if( m_aObject.size() )
		{
			s += (_T("\r\n@@@@@@@@@@@@ 发现内存泄露！！@@@@@@@@\r\n"));
			//OutputDebugString(AString().Format(_T("\r\n发现内存泄露！！")));
			for(std::map<AObject*,int>::iterator it = m_aObject.begin();it!=m_aObject.end();it++)
			{
				//OutputDebugString(AString().Format(_T("\r\nObject=%p"),it->first));
				//OutputDebugString(AString().Format(_T(" class=%s"),it->first->GetClass()));
				AString str;
				str.Format(_T("\r\n\tObject:%p class:%s"),it->first,(it->first ? it->first->GetClass() : _T("")));
				s +=(str);
			}
			s += (_T("\r\n====== end ========\r\n"));
			
		}
		else
			s += (_T("\r\n --- 祝贺！！no memory leak!! --- \r\n"));
		if( m_aObject.size() )
			::MessageBox(NULL,s.Text(),_T("Hint_MemoryLeak"),0);

		OutputDebugString(s);
	}
	std::map<AObject*,int>	m_aObject;
};

AMemCheck g_MemCheck;

void AddObject(AObject* p)
{
	g_MemCheck.m_aObject[p] = (int)p;
}
void RemoveObject(AObject* p)
{
	//OutputDebugString(AString().Format(_T("\r\n一个对象要释放 object=%p class=%s"),p,p->GetClass()));
	std::map<AObject*,int>::iterator it = g_MemCheck.m_aObject.find(p);
	if( it != g_MemCheck.m_aObject.end() )
		g_MemCheck.m_aObject.erase(it);
}
void CheckMem()
{
	g_MemCheck.CheckMemoryLeak();
}

#endif

AObject::AObject():m_iRefCount(0)
{
#ifdef _DEBUG
	AddObject(this);
#endif
}

AObject::~AObject()
{
#ifdef _DEBUG
	
	RemoveObject(this);
#endif
}

void AObject::Release()
{
	if( AString(_T("PopupListBox")) == GetClass() )
	{
		OutputDebugString(_T("PopupListBox Release"));
	}
	LONG i = ::InterlockedDecrement(&m_iRefCount);
	if( i == 0 ) 
		delete this;
}

int AObject::AddRef()
{
	if( AString(_T("PopupListBox")) == GetClass() )
	{
		OutputDebugString(_T("PopupListBox AddRef"));
	}
	return ::InterlockedIncrement(&m_iRefCount);
}
