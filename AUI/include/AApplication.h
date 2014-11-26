#ifndef AAPPLICATION_H
#define AAPPLICATION_H

enum { ApplicationCursorCount=15 };

typedef AObject* (*TControlCreator)(AComponent* pOwner);
typedef void (*TCallMCF)(const AString& sMCF,AObject* pSender,AEvent* pEvent);


class AUI_API AApplication : public AComponent
{
	CLASS_INFO(Application,Component);
public:
	friend class AForm;
	friend class AControl;

	AApplication(AComponent* pOwner);
	virtual ~AApplication();

	static AApplication* Get();
	static void Discard(AObject* p);

	void Init();
	void Clean();
	void Run();
	
	AForm* GetMainForm();
	void SetCurSkin(ASkin* pSkin);
	ASkin* GetCurSkin();

	HINSTANCE GetHandle();
	void SetHandle(HINSTANCE h);

	HCURSOR GetCursor(TCursorType ct);

	int MsgBox(const AString& sText,const AString& sCaption=_T("ÌבÊ¾"),UINT nTyppe=0);

	void RegisterControlCreator(const AString& sClassName,TControlCreator cc);
	void RegisterMCFCall(TCallMCF c);
	TCallMCF GetMCFCall();

	AString GetOSName(){ return m_sOSName; }
	AString GetOSVersionMark(){ return m_sOSVersionMark; }
	int GetOSVersion(){ return m_iOSVersion; }
protected:
	ASkin*		m_pCurSkin;
	HINSTANCE	m_hInstance;
	HCURSOR		m_aCursor[ApplicationCursorCount];
	AStringMap	m_aControlCreator;
	AForm*		m_pMainForm;
	AString		m_sOSName;
	AString		m_sOSVersionMark;
	int		m_iOSVersion;
	TCallMCF	m_MCFCall;

	void _InitOSVersion();
};

#endif//AAPPLICATION_H