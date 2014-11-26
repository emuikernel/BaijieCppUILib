#ifndef ABUTTON_H
#define ABUTTON_H

class AUI_API AButton: public AControl
{
	CLASS_INFO(Button,Control);
public:
	AButton(AComponent* pOwner);
	virtual ~AButton();

	virtual AAction* GetAction();
	virtual void SetAction(AAction* pAction);

	virtual void SetImage(AImage* p);
	virtual AImage* GetImage();

	virtual eAlign GetImageAlign();
	virtual void SetImageAlign(eAlign e);

	virtual AString GetImagePath();
	virtual void SetImagePath(const AString& s);

	virtual bool GetTransparent();
	virtual void SetTransparent(bool v);

	virtual void SetOverImage(AImage* p);
	virtual AImage* GetOverImage();
	virtual AString GetOverImagePath();
	virtual void SetOverImagePath(const AString& s);

	virtual void SetDownImage(AImage* p);
	virtual AImage* GetDownImage();
	virtual AString GetDownImagePath();
	virtual void SetDownImagePath(const AString& s);

	virtual void SetDisableImage(AImage* p);
	virtual AImage* GetDisableImage();
	virtual AString GetDisableImagePath();
	virtual void SetDisableImagePath(const AString& s);

	virtual void SetPropFromNode(AMLBody* pNode);

	//对ES的支持
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

	virtual void DoClick(AEvent* pEvent);
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

	AAction*	m_pAction;
	AImage*		m_pImage;
	eAlign		m_eImageAlign;
	AString		m_sImagePath;
	bool		m_bTransparent;

	AImage*		m_pOverImage;
	AString		m_sOverImage;
	AImage*		m_pDownImage;
	AString		m_sDownImage;
	AImage*		m_pDisableImage;
	AString		m_sDisableImage;
};
#endif//ABUTTON_H
