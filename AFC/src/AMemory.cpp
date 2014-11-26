#include "stdafx.h"

AMemory::AMemory():base_class()
{
	m_pData		=	NULL;
	m_nLen		=	0;
}

AMemory::AMemory(const AMemory& ms)
{
	m_pData		=	NULL;
	m_nLen		=	0;
	if( ms.GetSize() > 0 )
	{
		SetSize( ms.GetSize() );
		if( GetSize() > 0 ) memcpy(GetData(),ms.GetData(),ms.GetSize());
	}
}

AMemory::~AMemory()
{
	SetSize(0);
}

void	AMemory::SetSize(UINT nLen)
{
	if( m_pData != NULL )
	{
		delete[] m_pData;
		m_pData	=	NULL;
		m_nLen	=	0;
	}
	if( nLen < 1 )
		return;
	m_pData = new BYTE[nLen];

	if( m_pData == NULL )
		throw AException(_T("ÄÚ´æ²»×ã"));
	m_nLen	=	nLen;

}

UINT	AMemory::GetSize() const
{
	return m_nLen;
}

BYTE*	AMemory::GetData() const
{
	return m_pData;
}

void AMemory::SetData(BYTE *p,int iLen)
{
	SetSize(0);
	m_pData	=	p;
	m_nLen	=	iLen;
}

AMemory& AMemory::Add(BYTE* pBuf,int iLen)
{
	AMemory ms;
	ms.SetSize( GetSize() );
	memcpy( ms.GetData(),GetData(),GetSize() );
	UINT iSize = GetSize();
	SetSize( iSize + iLen);
	memcpy( GetData(),ms.GetData(),ms.GetSize() );
	BYTE* ptr = GetData();
	ptr += ms.GetSize();
	memcpy( ptr,pBuf,iLen );
	return *this;
}

AMemory& AMemory::operator=(const AMemory& ms)
{
	SetSize(0);
	if( ms.GetSize() > 0 )
	{
		SetSize( ms.GetSize() );
		if( GetSize() > 0 ) memcpy(GetData(),ms.GetData(),ms.GetSize());
	}
	return *this;
}