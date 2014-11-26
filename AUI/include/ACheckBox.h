#ifndef ACHECKBOX_H
#define ACHECKBOX_H

class AUI_API ACheckBox: public AContainer
{
	CLASS_INFO(CheckBox,Container);
public:
	ACheckBox(AComponent* pOwner);
	virtual ~ACheckBox();

	virtual void SetLeft(int v);
	virtual void SetTop(int v);
	virtual void SetWidth(int v);
	virtual void SetHeight(int v);
	virtual void SetText(AString s);

	virtual void SetChecked(bool v);
	virtual bool GetChecked();

	ALabel* GetLabel();
	AImageControl* GetImageControl();

	virtual void SetPropFromNode(AMLBody* pNode);

	//对ES的支持
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

	void OnFontChange();
	void OnThisFontChange();
	virtual void OnImageClick(AObject* pSender,AEvent* pEvent);
	virtual void DoClick(AEvent* pEvent);
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	ALabel*		m_pLabel;
	AImageControl*	m_pImage;
	bool		m_bChecked;

	virtual void _PaintImage(AObject* pSender,AEvent* pEvent);
	virtual void CalcPos();

};
#endif//ACHECKBOX_H
