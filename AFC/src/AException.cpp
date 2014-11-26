
#include "stdafx.h"
#include "AException.h"


AException::AException(void)
{
}

AException::~AException(void)
{
}

AException::AException(const AString& sDesc)
{
	m_sDesc = sDesc;
}

AException::AException(const AException& e)
{
	m_sDesc = e.m_sDesc;
}

AException& AException::operator =(const AException& e)
{
	m_sDesc = e.m_sDesc;
	return *this;
}

AString AException::GetDesc()
{
	return m_sDesc;
}

