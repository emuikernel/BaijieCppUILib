
#include "stdafx.h"
#include "AArray.h"


AArray::AArray(void) : AObject()
{
	m_iCount = 0;
	m_aData = NULL;
	m_iTotalSize = 0;
	m_OnDelete = NULL;
}

AArray::~AArray(void)
{
	Clear();
	if( m_aData != NULL ) delete[] m_aData;
}

void AArray::Clear()
{
	if( m_OnDelete != NULL )
	{
		for(int i=0;i<m_iCount;i++)
		{
			m_OnDelete(i,m_aData[i]);
		}
	}
	m_iCount = 0;
}

void AArray::Add(void* v)
{
	int iSize = m_iCount + 1;
	SetSize(iSize);
	m_aData[m_iCount] = v;
	m_iCount++;
}
void* AArray::operator[](int index)
{
	if( index < 0 || index >= m_iCount ) 
		throw AException(_T("[Array]Out of bound"));
	return m_aData[index];
}

void* AArray::GetItem(int index)
{
	if( index < 0 || index >= m_iCount )
		throw AException(_T("[Array]Out of bound"));
	return m_aData[index];
}

void AArray::SetItem(int index,void* p)
{
	if( index < 0 || index >= m_iCount ) 
		throw AException(_T("[Array]Out of bound"));
	m_aData[index] = p;
}

int AArray::IndexOf(void* v)
{
	for(int i=0;i<m_iCount;i++)
	{
		if( m_aData[i] == v ) return i;
	}
	return -1;
}
void AArray::Delete(int index)
{
	if( index < 0 || index >= m_iCount ) return;
	void* x = m_aData[index];
	if( index +  1 < m_iCount )
	{
		::memmove(&m_aData[index],&m_aData[index+1],(m_iCount-index-1) * sizeof(AObject*));
	}
	//for(int i=index+1;i<m_iCount;i++) m_aData[i-1] = m_aData[i];
	m_iCount--;
	SetSize(m_iCount);
	if( m_OnDelete ) m_OnDelete(index,x);
}
void AArray::Insert(int index,void* v)
{
	if( m_iCount == 0 )
	{
		SetSize(1);
		m_aData[0] = v;
		m_iCount = 1;
		return;
	}
	if( index == m_iCount )
	{
		SetSize(m_iCount+1);
		m_aData[m_iCount]=v;
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
	m_iCount++;
}
void AArray::Assign(const AArray& a)
{
	Clear();
	SetSize( a.GetCount() );
	for(int i=0;i<a.GetCount();i++)
	{
		m_aData[i] = a.m_aData[i];
	}
}
void AArray::SetSize(int iSize)
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

		void** x = new void*[count];
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
			void** x = new void*[ iSize ];
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

void AArray::SetOnDeleteItem(FOnArrayDeleteItem f)
{
	m_OnDelete = f;
}

FOnArrayDeleteItem AArray::GetOnDeleteItem()
{
	return m_OnDelete;
}

void AArray::Sort(int (*sortfunc)(void* p1,void* p2))
{
	for(int i=0;i<m_iCount;i++)
	{
		if( i < m_iCount - 1)
		{
			int iRet = (*sortfunc)(m_aData[i],m_aData[i+1]);
			if( i > 0 )
			{
				void* p =m_aData[i];
				m_aData[i] = m_aData[i+1];
				m_aData[i+1] = p;
			}
		}
	}
}

int AArray::GetCount() const
{
	return m_iCount;
}

//====================== Enumerator ==================
AArrayEnumerator::AArrayEnumerator(AArray* pOwner):m_pOwner(pOwner)
{
	m_iIndex = -1;
	m_bEnd = false;
}

AArrayEnumerator::~AArrayEnumerator()
{

}


bool AArrayEnumerator::Next()
{
	if( !m_bEnd ) 
	{
		m_iIndex++;
		if( m_iIndex >= m_pOwner->GetCount() )
			m_bEnd = true;
	}

	return !m_bEnd;
}
void* AArrayEnumerator::Cur()
{
	return m_pOwner->GetItem(m_iIndex);
}
int AArrayEnumerator::Index()
{
	return m_iIndex;
}
