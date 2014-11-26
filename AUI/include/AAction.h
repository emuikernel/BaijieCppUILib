#ifndef AACTION_H
#define AACTION_H

class AUI_API AAction : public AComponent
{
	CLASS_INFO(Action,Component);
protected:
	friend class AActionList;
	AAction(AComponent* pOwner);
public:
	virtual ~AAction();

	virtual AString GetName();
	virtual void SetName(const AString& s);

	virtual AString GetText();
	virtual void SetText(const AString& s);

	AEventHandler		OnUpdate;
	AEventHandler		OnExecute;
protected:
	AString		m_sName;
	AString		m_sText;
};

#endif//AACTION_H