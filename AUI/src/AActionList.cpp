#include "stdafx.h"

AActionList::AActionList(AComponent* pOwner):base_class(pOwner)
{

}

AActionList::~AActionList()
{
	//
}

AAction* AActionList::AddAction(const AString& sName)
{
	AAction* p = new AAction(this);
	m_aAction.Add( p );
	p->SetName( sName );
	return p;
}
void AActionList::RemoveAction(AAction* pAction)
{
	int index = m_aAction.IndexOf( pAction );
	if( index != -1 ) m_aAction.Delete( index );
}
AAction* AActionList::GetAction(const AString& sActionName)
{
	int iCount = m_aAction.GetCount();
	for(int i=0;i<iCount;i++)
	{
		AAction* p = (AAction* )m_aAction[i];
		if( p->GetName() == sActionName ) return p;
	}
	return NULL;
}
AAction* AActionList::operator [](int index)
{
	return GetAction(index);
}

AAction* AActionList::GetAction(int index)
{
	return (AAction *)m_aAction[index];
}

int AActionList::GetActionCount()
{
	return m_aAction.GetCount();
}