
#include "stdafx.h"
#include "AStringArray.h"


AStringArray::AStringArray(void):AObject()
{
	m_aData = NULL;
	m_iCount = 0;
	m_iTotalSize = 0;
}

AStringArray::~AStringArray(void)
{
	Clear();
	if( m_aData != NULL ) delete[] m_aData;
}

int AStringArray::GetCount()
{
	return m_iCount;
}
AString AStringArray::operator[](int index)
{
	if( index < 0 || index >= m_iCount ) throw AException(_T("[StringArray]Out of bound"));
	AString* x = m_aData[index];
	return *x;
}
AString AStringArray::GetItem(int index)
{
	AString* x = m_aData[index];
	return *x;
}
void AStringArray::Clear()
{
	for(int i=0;i<m_iCount;i++)
	{
		AString* x = m_aData[i];
		delete x;
	}
	m_iCount = 0;
}

void AStringArray::SetSize(int iSize)
{
	if( m_iTotalSize == iSize ) return;
	if( m_iTotalSize < iSize )
	{
		int count = iSize + (iSize/4 < 256 ? iSize/4 : 256);
		AString** x = new AString*[count];
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
			AString** x = new AString*[ iSize ];
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

void AStringArray::Add(const AString& v)
{
	int iSize = m_iCount + 1;
	SetSize(iSize);
	AString* x = new AString(v);
	m_aData[m_iCount] = x;
	m_iCount++;
}

int AStringArray::IndexOf(const AString& v)
{
	for(int i=0;i<m_iCount;i++)
	{
		AString* x = m_aData[i];
		if( *x == v ) return i;
	}
	return -1;
}

void AStringArray::SetItem(int index,const AString &s)
{
	if( index < 0 || index >= GetCount()) throw AException("[StringArray]Out of bound");
	(*m_aData[index]) = s;
}
void AStringArray::Delete(int index)
{
	if( index < 0 || index >= m_iCount ) return;
	AString* x = m_aData[index];
	if( index +  1 < m_iCount )
	{
		::memmove(&m_aData[index],&m_aData[index+1],(m_iCount-index-1) * sizeof(AObject*));
	}
	//for(int i=index+1;i<m_iCount;i++) m_aData[i-1] = m_aData[i];
	delete x;
	m_iCount--;
	SetSize(m_iCount);
}

void AStringArray::Insert(int index,const AString& s)
{
	AString* x = new AString(s);
	if( m_iCount == 0 )
	{
		SetSize(1);
		m_aData[0] = x;
		m_iCount = 1;
		return;
	}
	if( index == m_iCount )
	{
		SetSize(m_iCount+1);
		m_aData[m_iCount]=x;
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
	
	m_aData[index] = x;
	m_iCount++;
}
void AStringArray::Assign(const AStringArray& a)
{
	Clear();
	SetSize( a.m_iCount );
	for(int i=0;i<a.m_iCount;i++)
	{
		m_aData[i] = a.m_aData[i];
	}
}
AString AStringArray::ToString(const AString& split)
{
	AString str;
	for(int i=0;i<m_iCount;i++)
	{
		if( i > 0 ) str += split;
		str += *(m_aData[i]);
		
	}
	return str;
}
void AStringArray::FromString(const AString& sFrom,const AString& split)
{
	Clear();
	if( split == _T("") )
	{
		//Add( sFrom );
	}
	else
	{
		int iLen = split.Length();
		if( iLen < 1 ) 
		{
			Add(sFrom);
			return;
		}
		int iPos1=0,iPos2;
		while( true )
		{
			iPos2 = sFrom.Find(split.Text(),iPos1);
			if( iPos2 > -1 )
			{
				if( iPos2 == iPos1 ) 
					Add(_T(""));
				else
					Add( sFrom.Mid(iPos1,iPos2-iPos1) );
				iPos1 = iPos2 + iLen;
			}
			else
			{
				if( iPos1 < sFrom.Length() )
					Add( sFrom.Mid(iPos1,sFrom.Length()-iPos1) );
				break;
			}
		}
	}
}
void AStringArray::Sort(FTextCompareProc f)
{
	if( m_iCount <= 1 ) return;
	for(int i=0;i<m_iCount-1;i++)
	{
		for(int j=i+1;j<m_iCount;j++)
		{
			int v=0;
			if( f != NULL ) v = f(*(m_aData[i]),*(m_aData[j]));
			else
			{
				if( *(m_aData[i]) == *(m_aData[j]) ) v = 0;
				else if( *(m_aData[i]) > *(m_aData[j]) ) v = 1;
				else v = -1;
			}
			if( v > 0 )
			{
				AString* x = m_aData[i];
				m_aData[i] = m_aData[j];
				m_aData[j] = x;
			}
		}
	}
}

//====================== Enumerator ==================
AStringArrayEnumerator::AStringArrayEnumerator(AStringArray* pOwner):m_pOwner(pOwner)
{
	m_iIndex = -1;
	m_bEnd = false;
}

AStringArrayEnumerator::~AStringArrayEnumerator()
{

}


bool AStringArrayEnumerator::Next()
{
	if( !m_bEnd )
	{
		m_iIndex ++;
		if( m_iIndex >= m_pOwner->GetCount() ) m_bEnd = true;
	}
	return !m_bEnd;
}
AString AStringArrayEnumerator::Cur()
{
	return m_pOwner->GetItem(m_iIndex);
}
int AStringArrayEnumerator::Index()
{
	return m_iIndex;
}


