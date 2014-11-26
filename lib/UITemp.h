#ifndef A$FORMNAME_UPCASE$_H
#define A$FORMNAME_UPCASE$_H


class A$FORMNAME$UI : public AForm
{
	CLASS_INFO($FORMNAME$UI,Form);
public:
	A$FORMNAME$UI(AComponent* pOwner):base_class(pOwner)
	{
		$CONTROL_INIT$
	}
	virtual ~A$FORMNAME$UI()
	{
	}

	virtual void InitControls()
	{
	}
	virtual void ConnectEvent()
	{
	}
	virtual void DoCreate()
	{
	}
	virtual void CreateFromXML()
	{
		static ACHAR* sz$FORMNAME$ = _T("$FORMNAME_XML$");
		xobject<AMLBody> pBody(new AMLBody());
		pBody->FromXML(sz$FORMNAME$);
		this->FromXML(&pBody);
	}

	$CONTROL_MAPS$

	$EVENTS$
};

#endif//A$FORMNAME_UPCASE$_H