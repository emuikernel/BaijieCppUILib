#ifndef ADATECONTROL_H
#define ADATECONTROL_H

class AUI_API ADateControl: public AContainer
{
	CLASS_INFO(DateControl,Container);
public:
	ADateControl(AComponent* pOwner);
	virtual ~ADateControl();

	virtual void SetDate(const ADateTime& dt);
	virtual ADateTime GetDate();

	virtual void DoClick(AEvent* pEvent);

	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

protected:
	ADateTime		m_Date;
	APopupDateBox*		m_pBox;

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void OnBoxSelect(AObject* pSender,AEvent* pEvent);
};
#endif//ADATECONTROL_H
