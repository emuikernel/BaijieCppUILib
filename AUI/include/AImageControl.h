#ifndef AIMAGECONTROL_H
#define AIMAGECONTROL_H

class AUI_API AImageControl: public AControl
{
	CLASS_INFO(ImageControl,Control);
public:
	AImageControl(AComponent* pOwner);
	virtual ~AImageControl();

	AColor			TransparentColor;

	virtual void SetGraphic(AImage* p);
	virtual AImage* GetGraphic();

	virtual AString GetImagePath();
	virtual void SetImagePath(const AString& s);

	virtual void SetDrawStyle(TImageDrawStyle v);
	virtual TImageDrawStyle GetDrawStyle();

	virtual void SetTransparent(bool v);
	virtual bool GetTransparent();

	virtual void SetPropFromNode(AMLBody* pNode);

	//对ES的支持
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

protected:
	AImage*	m_pGraphic;
	TImageDrawStyle	m_DrawStyle;
	bool		m_bTransparent;
	AString		m_sImagePath;

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
};
#endif//AIMAGECONTROL_H
