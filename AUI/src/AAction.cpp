#include "stdafx.h"

AAction::AAction(AComponent* pOwner):base_class(pOwner)
{

}

AAction::~AAction()
{

}

AString AAction::GetName()
{
	return m_sName;
}

void AAction::SetName(const AString& s)
{
	m_sName = s;
}

AString AAction::GetText()
{
	return m_sText;
}

void AAction::SetText(const AString& s)
{
	m_sText = s;
}
