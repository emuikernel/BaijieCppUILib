#ifndef ASPINCONTROL_H
#define ASPINCONTROL_H

class AUI_API ASpinControl: public AContainer
{
	CLASS_INFO(SpinControl,Container);
public:
	ASpinControl(AComponent* pOwner);
	virtual ~ASpinControl();

	virtual void SetAutoSpin(bool v);
	virtual bool GetAutoSpin();

	virtual int GetMinValue();
	virtual void SetMinValue(int v);

	virtual int GetMaxValue();
	virtual void SetMaxValue(int v);

	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

	AEventHandler		OnSpin;
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void OnUpButtonPaint(AObject* pSender,AEvent* pEvent);
	virtual void OnDownButtonPaint(AObject* pSender,AEvent* pEvent);
	virtual void OnButtonClick(AObject* pSender,AEvent* pEvent);

	virtual void DoLayout();
	AControl* _FindLeft();
private:
	AButton*		m_pUpButton;
	AButton*		m_pDownButton;

	bool			m_bAutoSpin;
	int			m_iMin;
	int			m_iMax;

};
#endif//ASPINCONTROL_H