
#include "stdafx.h"
#include "AStringMap.h"



AStringMap::AStringMap(void):AObject()
{
	memset( m_Data,0,sizeof(myitem*)*KSTRING_MAP_SIZE );
}

AStringMap::~AStringMap(void)
{
	Clear();
}



/* A Simple Hash Function */  
static unsigned int simple_hash(const ACHAR *str)  
{  
	register unsigned int hash;  
#ifdef UNICODE
	ACHAR* p;
	for(hash=0,p=(ACHAR *)str;*p;p++)
	{
		char* x = (char *)p;
		hash = 31 * hash + *x;
		if( *(x+1) != 0 ) hash = 31 * hash + *(x+1);
	}
#else
	register unsigned char *p;  

	for(hash = 0, p = (unsigned char *)str; *p ; p++)  
		hash = 31 * hash + *p; 
#endif
	 
	unsigned int h = hash & 0x7FFFFFFF;
	return h;  
}  
void AStringMap::SetItem(const AString& sKey,void* pData)
{
	myitem* p = _getitem(sKey);
	if( p == NULL ) p = _newitem(sKey);
	p->pData = pData;
}
void* AStringMap::GetItem(const AString& sKey)
{
	myitem* p = _getitem(sKey);
	if( p ) return p->pData;
	
	throw AException(_T("[StringMap.GetItem]Out of bound"));
}

AStringMap::myitem* AStringMap::_getitem(const AString& sKey)
{
	unsigned int h = GetHashValue(sKey);
	int index = h % KSTRING_MAP_SIZE;
	myitem* next = m_Data[index];
	while( next ) 
	{
		if( next->nHashValue == h ) return next;
		next = next->pNext;
	}
	return NULL;
}

AStringMap::myitem* AStringMap::_newitem(const AString& sKey)
{
	unsigned int h = GetHashValue(sKey);
	int index = h % KSTRING_MAP_SIZE;
	myitem* p = new myitem();
	p->nHashValue = h;
	p->index = index;
	p->sKey = sKey;
	p->pNext = NULL;
	myitem* next = m_Data[index];
	if( next == NULL ) m_Data[index] = p;
	else
	{
		while(next->pNext) next = next->pNext;
		next->pNext = p;
	}
	return p;
}

int AStringMap::GetCount()
{
	int iCount = 0;
	AStringMapEnumerator em(this);
	while( em.Next() ) iCount ++;
	return iCount;
}
void AStringMap::RemoveItem(const AString& sKey)
{
	unsigned int h = GetHashValue(sKey);
	int index = h % KSTRING_MAP_SIZE;
	myitem* next = m_Data[index];
	if( next == NULL ) return;
	if( next->nHashValue == h )
	{
		m_Data[index] = next->pNext;
		delete next;
	}
	else
	{
		while(next->pNext)
		{
			if( next->pNext->nHashValue == h )
			{
				myitem* p = next->pNext;
				next->pNext = next->pNext->pNext;
				delete p;
				break;
			}
			next = next->pNext;
		}
	}
}

void AStringMap::Delete(myitem* p)
{
	myitem* pFirst = m_Data[p->index];
	myitem* next=NULL;
	if( pFirst == p )
	{
		next = pFirst->pNext;
		if( next != NULL )
		{
			m_Data[p->index] = next;
			delete p;
		}
		else
		{
			m_Data[p->index] = NULL;
			delete p;
		}
	}
	else
	{
		next = pFirst;
		while( next->pNext != p ) next = next->pNext;
		next->pNext = p->pNext;
		delete p;
	}
}
bool AStringMap::IsExist(const AString& sKey)
{
	return _getitem(sKey) == NULL ? false : true;
}
void AStringMap::Clear()
{
	for(int i=0;i<KSTRING_MAP_SIZE;i++)
	{
		myitem* p = m_Data[i];
		while( p )
		{
			m_Data[i] = p->pNext;
			delete p;
			p = m_Data[i];
		}
	}
}
AStringMap::myitem* AStringMap::Begin()
{
	for(int i=0;i<KSTRING_MAP_SIZE;i++)
	{
		myitem* p = m_Data[i];
		if( p && p->nHashValue != 0 ) return p;
	}
	return NULL;
}
bool AStringMap::IsEnd(AStringMap::myitem* p)
{
	return ( p == NULL ? true : false );
}
AStringMap::myitem* AStringMap::Next(AStringMap::myitem* p)
{
	if( p == NULL ) return NULL;
	if( p->pNext != NULL ) return p->pNext;
	if( p->index < KSTRING_MAP_SIZE - 1 )
	{
		for(int i=p->index + 1;i<KSTRING_MAP_SIZE;i++ )
		{
			myitem* p = m_Data[i];
			if( p && p->nHashValue != 0 ) return p;
		}
	}
	return NULL;
}

unsigned int AStringMap::GetHashValue(const AString& s)
{
	unsigned int v = simple_hash(s.Text());
	if( v == 0 ) v = 1;//ÌØÊâ´¦Àí
	return v;
}



//====================== Enumerator ==================
AStringMapEnumerator::AStringMapEnumerator(AStringMap* pOwner):m_pOwner(pOwner)
{
	m_pItem = NULL;
	m_bEnd = false;
}

AStringMapEnumerator::~AStringMapEnumerator()
{

}


bool AStringMapEnumerator::Next()
{
	if( !m_bEnd )
	{
		if( m_pItem == NULL ) m_pItem = m_pOwner->Begin();
		else m_pItem = m_pOwner->Next(m_pItem);
		m_bEnd = m_pOwner->IsEnd(m_pItem);
	}
	return !m_bEnd;
}
void* AStringMapEnumerator::Cur()
{
	if( m_pItem == NULL ) return NULL;//throw ?
	else return m_pItem->pData;
}

AString AStringMapEnumerator::Index()
{
	AString s;
	if( m_pItem) return m_pItem->sKey;
	return s;
}


