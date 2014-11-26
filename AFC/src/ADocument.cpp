#include "stdafx.h"

ADocument::ADocument(AModelBase* pModel):base_class(),m_pModel(pModel)
{
	if( m_pModel ) m_pModel->AddRef();
	m_bModified = false;
}

ADocument::~ADocument()
{
	if( m_pModel ) m_pModel->Release();
}

bool ADocument::GetModified()
{
	return m_bModified;
}

void ADocument::SetModified(bool v)
{
	m_bModified = v;
}