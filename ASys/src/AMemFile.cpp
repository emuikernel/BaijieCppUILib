#include "stdafx.h"

#define STEP_LEN	1024
static BYTE g_dummyaddr;

AMemFile::AMemFile():base_class()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_pData = NULL;
	m_nStep = STEP_LEN;
	m_nLen = 0;
	m_nSize = 0;
	m_nPos = 0;
}

AMemFile::~AMemFile()
{
	Close();
}
HANDLE AMemFile::GetHandle()
{
	return m_hFile;
}

void AMemFile::SetStep(int nStep)
{
	if( nStep < STEP_LEN ) nStep = STEP_LEN;
	m_nStep = nStep;
}
bool AMemFile::Create(const ACHAR* sFileName,bool bCreateAlways /*= true*/)
{
	Close();
	m_pData = &g_dummyaddr;
	return true;
}
bool AMemFile::Open(const ACHAR* sFileName,afc_FileMode fm /*=fmReadWrite*/)
{
	Close();
	m_pData = &g_dummyaddr;
	return true;
}
int AMemFile::Seek(int iPos)
{
	if( IsOpened() )
	{
		if( iPos >= 0 && iPos < m_nLen ) m_nPos = iPos;
		return m_nPos;
	}
	else
		return 0;
}
int AMemFile::SeekToBegin()
{
	if( IsOpened() )
	{
		m_nPos = 0;
		return m_nPos;
	}
	else
		return 0;
}
int AMemFile::SeekToEnd()
{
	if( IsOpened() )
	{
		m_nPos = m_nLen;
		return m_nPos;
	}
	else
		return 0;
}
int AMemFile::Length()
{
	if( IsOpened() )
	{
		return m_nLen;
	}
	else
		return 0;
}
BYTE* AMemFile::GetData()
{
	return m_pData;
}

int AMemFile::GetPos()
{
	return m_nPos;
}
int AMemFile::Read(BYTE* pData,int iLen)
{
	if( IsOpened() )
	{
		if( m_nPos >= m_nLen ) return 0;
		if( m_nPos + iLen > m_nLen )
			iLen = m_nLen - m_nPos-1;
		memcpy(pData,m_pData+m_nPos,iLen);
		return iLen;
	}
	else
		return 0;
}
int AMemFile::Write(BYTE* pData,int iLen)
{
	if( IsOpened() )
	{
		int iSize = (iLen / m_nStep + 1) * m_nStep;
		if( m_nLen + iLen < m_nSize && m_pData != &g_dummyaddr )
		{
			BYTE* p = m_pData;
			p += m_nLen;
			memcpy(p,pData,iLen);
			m_nLen += iLen;
		}
		else
		{
			BYTE* p = new BYTE[m_nSize + iSize];
			if( p == NULL ) return 0;
			if( m_pData != &g_dummyaddr )
			{
				memcpy(p,m_pData,m_nLen);
				delete[] m_pData;
			}
			m_pData = p;
			p += m_nLen;
			memcpy(p,pData,iLen);
			m_nLen += iLen;
			m_nSize += iSize;
		}
		return iLen;
	}
	else
		return 0;
}
bool AMemFile::IsOpened()
{
	return (m_pData != NULL ) ? true : false;
}
void AMemFile::Close()
{
	if( IsOpened() )
	{
		delete[] m_pData;
		m_pData = NULL;
		m_nLen = 0;
		m_nSize = 0;
		m_nPos = 0;
	}
}

