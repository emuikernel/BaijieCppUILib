
#include "stdafx.h"
#include "AObjectArray.h"


AObjectArray::AObjectArray(void) : AObject()
{
	m_iCount = 0;
	m_aData = NULL;
	m_iTotalSize = 0;
	m_OnDelete = NULL;
}

AObjectArray::~AObjectArray(void)
{
	Clear();
	if( m_aData != NULL ) delete[] m_aData;
}


void AObjectArray::Clear()
{
	for(int i=0;i<m_iCount;i++)
	{
		AObject* p = m_aData[i];
		if( m_OnDelete != NULL ) m_OnDelete(i,p);
		if( p ) p->Release();
	}

	m_iCount = 0;
}

void AObjectArray::Add(AObject* v)
{
	int iSize = m_iCount + 1;
	SetSize(iSize);
	m_aData[m_iCount] = v;
	if( v ) v->AddRef();
	m_iCount++;
}
AObject* AObjectArray::operator[](int index)
{
	if( index < 0 || index >= m_iCount ) 
		throw AException(_T("[ObjectArray]Out of bound"));
	return m_aData[index];
}

AObject* AObjectArray::GetItem(int index)
{
	if( index < 0 || index >= m_iCount ) 
		throw AException(_T("[ObjectArray]Out of bound"));
	return m_aData[index];
}

void AObjectArray::SetItem(int index,AObject* pObject)
{
	if( index < 0 || index >= m_iCount ) 
		throw AException(_T("[ObjectArray]Out of bound"));
	AObject* p = m_aData[index];
	if( p ) p->Release();
	m_aData[index] = pObject;
	if( pObject ) pObject->AddRef();
}

int AObjectArray::IndexOf(AObject* v)
{
	for(int i=0;i<m_iCount;i++)
	{
		if( m_aData[i] == v ) return i;
	}
	return -1;
}
void AObjectArray::Delete(AObject* v)
{
	for(int i=0;i<m_iCount;i++)
	{
		AObject* p = m_aData[i];
		if( p == v )
		{
			if( m_OnDelete ) m_OnDelete(i,v);
			if( p ) 
				p->Release();
			if( i +  1 < m_iCount )
			{
				::memmove(&m_aData[i],&m_aData[i+1],(m_iCount-i-1) * sizeof(AObject*));
			}
// 			for(int j=i+1;j<m_iCount;j++)
// 			{
// 				m_aData[j-1] = m_aData[j];
// 			}
			m_iCount--;
			SetSize(m_iCount);
			break;
		}
	}
}
void AObjectArray::Delete(int index)
{
	if( index < 0 || index >= m_iCount ) return;
	AObject* x = m_aData[index];
	if( m_OnDelete ) m_OnDelete(index,x);

	if( index +  1 < m_iCount )
	{
		::memmove(&m_aData[index],&m_aData[index+1],(m_iCount-index-1) * sizeof(AObject*));
	}
	//for(int i=index+1;i<m_iCount;i++) m_aData[i-1] = m_aData[i];
	m_iCount--;
	SetSize(m_iCount);
	
	if( x ) 
		x->Release();
}
void AObjectArray::Insert(int index,AObject* v)
{
	if( m_iCount == 0 )
	{
		SetSize(1);
		m_aData[0] = v;
		if( v ) v->AddRef();
		m_iCount = 1;
		return;
	}
	if( index == m_iCount )
	{
		SetSize(m_iCount+1);
		m_aData[m_iCount]=v;
		if( v ) v->AddRef();
		m_iCount++;
		return;
	}
	if( index < 0 ) index=0;
	if( index >= m_iCount ) index = m_iCount-1;
	SetSize(m_iCount+1);

	for(int i=m_iCount-1;i>=index;i--)
	{
		m_aData[i+1] = m_aData[i];
	}

	m_aData[index] = v;
	if( v ) v->AddRef();
	m_iCount++;
}
void AObjectArray::Assign(const AObjectArray& a)
{
	Clear();
	SetSize( a.GetCount() );
	for(int i=0;i<a.GetCount();i++)
	{
		m_aData[i] = a.m_aData[i];
		if( m_aData[i] ) m_aData[i]->AddRef();
	}
}
void AObjectArray::SetSize(int iSize)
{
	if( m_iTotalSize == iSize ) return;
	if( m_iTotalSize < iSize )
	{
		int count=iSize;
		if( count < 10 ) count = 10;
		else if( count < 64 ) count = 64;
		else if( count < 256 ) count = 256;
		else if( count < 512 ) count = 512;
		else count += m_iTotalSize / 2 + m_iTotalSize % 512;

		AObject** x = new AObject*[count];
		for(int i=0;i<count;i++)
		{
			if( i < m_iCount ) x[i] = m_aData[i];
			else x[i] = NULL;
		}
		if( m_aData != NULL ) delete[] m_aData;
		m_aData = x;
		m_iTotalSize = count;
	}
	else
	{
		if( iSize > 0 && iSize < m_iTotalSize / 2 && iSize >= m_iCount )
		{
			AObject** x = new AObject*[ iSize ];
			for(int i=0;i<iSize;i++)
			{
				if( i < m_iCount ) x[i] = m_aData[i];
				else x[i] = NULL;
			}
			if( m_aData != NULL ) delete[] m_aData;
			m_aData = x;
			m_iTotalSize = iSize;
		}
	}
}

void AObjectArray::SetOnDeleteItem(FOnObjectArrayDeleteItem f)
{
	m_OnDelete = f;
}

FOnObjectArrayDeleteItem AObjectArray::GetOnDeleteItem()
{
	return m_OnDelete;
}

void AObjectArray::Sort(int (*sortfunc)(AObject* p1,AObject* p2))
{
	for(int i=0;i<m_iCount;i++)
	{
		if( i < m_iCount - 1)
		{
			int iRet = (*sortfunc)(m_aData[i],m_aData[i+1]);
			if( i > 0 )
			{
				AObject* p =m_aData[i];
				m_aData[i] = m_aData[i+1];
				m_aData[i+1] = p;
			}
		}
	}
}

int AObjectArray::GetCount() const
{
	return m_iCount;
}

//====================== Enumerator ==================
AObjectArrayEnumerator::AObjectArrayEnumerator(AObjectArray* pOwner):m_pOwner(pOwner)
{
	m_iIndex = -1;
	m_bEnd = false;
}

AObjectArrayEnumerator::~AObjectArrayEnumerator()
{

}


bool AObjectArrayEnumerator::Next()
{
	if( !m_bEnd )
	{
		m_iIndex ++;
		if( m_iIndex >= m_pOwner->GetCount() ) m_bEnd = true;
	}
	return !m_bEnd;
}
AObject* AObjectArrayEnumerator::Cur()
{
	return m_pOwner->GetItem(m_iIndex);
}
int AObjectArrayEnumerator::Index()
{
	return m_iIndex;
}
