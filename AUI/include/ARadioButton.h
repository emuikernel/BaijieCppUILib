#ifndef ARADIOBUTTON_H
#define ARADIOBUTTON_H

class AUI_API ARadioButton: public AContainer
{
	CLASS_INFO(RadioButton,Container);
public:
	ARadioButton(AComponent* pOwner);
	virtual ~ARadioButton();

	virtual void SetLeft(int v);
	virtual void SetTop(int v);
	virtual void SetWidth(int v);
	virtual void SetHeight(int v);
	virtual void SetText(AString s);
	virtual void SetChecked(bool v);
	virtual bool GetChecked();
	virtual void SetGroup(int v);
	virtual int GetGroup();

	ALabel* GetLabel();
	AImageControl* GetImageControl();

	void OnFontChange();
	void OnThisFontChange();
	virtual void OnImageClick(AObject* pSender,AEvent* pEvent);
	virtual void DoClick(AEvent* pEvent);

	virtual void SetPropFromNode(AMLBody* pNode);

	//对ES的支持
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

	ALabel*		m_pLabel;
	AImageControl*	m_pImage;

	bool		m_bChecked;
	int		m_iGroup;

	virtual void _PaintImage(AObject* pSender,HDC hDC);
	virtual void CalcPos();
};
#endif//ARADIOBUTTON_H
