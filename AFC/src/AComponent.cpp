#include "stdafx.h"

AComponent::AComponent(AComponent* pOwner):
	AObject(),
	m_pOwner(pOwner)
{
	if( m_pOwner )
		m_pOwner->GetChildren()->Add(this);
}

AComponent::~AComponent()
{

}

 AObjectArray* AComponent::GetChildren()
 {
	 return &m_aChildComponent;
 }

void AComponent::Free()
{
	if( m_pOwner ) 
	{
		m_pOwner->GetChildren()->Delete(this);//解除父子关系
	}
	//组件自己只维护父子关系，如果有额外的引用，须由外部来Release base_class::Release();
}

AComponent* AComponent::GetOwner()
{
	return m_pOwner;
}

