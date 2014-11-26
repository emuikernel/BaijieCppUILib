#ifndef ALABEL_H
#define ALABEL_H

class AUI_API ALabel: public AControl
{
	CLASS_INFO(Label,Control);
public:
	ALabel(AComponent* pOwner);
	virtual ~ALabel();

	virtual void SetTextAlign(TTextAlign textAlign);
	virtual TTextAlign GetTextAlign();

	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

	void OnFontChange();
protected:
	TTextAlign	m_TextAlign;

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

};
#endif//ALABEL_H
