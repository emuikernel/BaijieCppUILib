#ifndef AACTIONLIST_H
#define AACTIONLIST_H

class AUI_API AActionList : public AInvisibleControl
{
	CLASS_INFO(ActionList,InvisibleControl);
public:
	AActionList(AComponent* pOwner);
	virtual ~AActionList();

	virtual AAction* AddAction(const AString& sName);
	virtual void RemoveAction(AAction* pAction);
	virtual AAction* GetAction(const AString& sActionName);
	virtual AAction* operator [](int index);
	virtual AAction* GetAction(int index);
	virtual int GetActionCount();
protected:
	AObjectArray		m_aAction;
};

#endif//AACTIONLIST_H