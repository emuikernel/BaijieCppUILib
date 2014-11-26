#ifndef ATIMECONTROL_H
#define ATIMECONTROL_H

class AUI_API ATimeControl : public APanel
{
	CLASS_INFO(TimeControl,Panel);
public:
	ATimeControl(AComponent* pOwner);
	virtual ~ATimeControl();

	virtual void DoLayout();

	virtual ADateTime GetTime();
	virtual void SetTime(const ADateTime& dt);

	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

private:
	AEdit*		m_pEditHour;
	AEdit*		m_pEditMinute;
	AEdit*		m_pEditSecond;
	ALabel*		m_pLabel1;
	ALabel*		m_pLabel2;
	ASpinControl*	m_pSpin;
	AEdit*		m_pFocusEdit;

	void OnSpin(AObject* pSender,AEvent* pEvent);
	void OnEditFocus(AObject* pSender,AEvent* pEvent);
	void OnEditKillFocus(AObject* pSender,AEvent* pEvent);
};

#endif//ATIMECONTROL_H