#include "stdafx.h"

AMVCBase::AMVCBase():base_class()
{
	m_bModified = false;
}

AMVCBase::~AMVCBase()
{

}

bool AMVCBase::GetModified()
{
	return m_bModified;
}

void AMVCBase::SetModified(bool v)
{
	m_bModified = v;
}