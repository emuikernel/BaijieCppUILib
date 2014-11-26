#ifndef ACOMPONENT_H
#define ACOMPONENT_H


class AFC_API AComponent : public AObject
{
	CLASS_INFO(Component,Object);
public:
	AComponent(AComponent* pOwner);
	virtual ~AComponent();

	virtual AObjectArray* GetChildren();
	virtual void Free();
	virtual AComponent* GetOwner();
protected:
	AComponent*		m_pOwner;
	AObjectArray		m_aChildComponent;
};

#endif//ACOMPONENT_H