#ifndef AEVENT_H
#define AEVENT_H

//事件基类
class AUI_API AEvent : public AObject
{
	CLASS_INFO(Event,Object);
public:
	//来自于Windows消息机制的数据
	WPARAM m_wParam;
	LPARAM m_lParam;

	AEvent();
	virtual ~AEvent();
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
};

//Paint事件
class AUI_API APaintEvent : public AEvent
{
	CLASS_INFO(PaintEvent,Event);
public:
	APaintEvent();
	virtual ~APaintEvent();
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
};

class AUI_API ASpinEvent : public AEvent
{
	CLASS_INFO(SpinEvent,Event);
public:
	ASpinEvent();
	virtual ~ASpinEvent();
	bool m_bUp;//true: up  false:down
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
};

class AUI_API ACloseQueryEvent : public AEvent
{
	CLASS_INFO(CloseQueryEvent,Event);
public:
	ACloseQueryEvent();
	virtual ~ACloseQueryEvent();

	bool		m_bCanClose;
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
};

class AUI_API ACloseEvent : public AEvent
{
	CLASS_INFO(CloseEvent,Event);
public:
	ACloseEvent();
	virtual ~ACloseEvent();

	TCloseAction		m_CloseAction;
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
};

class AUI_API AKeyEvent : public AEvent
{
	CLASS_INFO(KeyEvent,Event);
public:
	AKeyEvent();
	virtual ~AKeyEvent();

	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);

	WCHAR		m_chKey;
	AFlagSet	m_KeyStates;
	TKeyEventType	m_Type;
};

class AUI_API AMouseEvent : public AEvent
{
	CLASS_INFO(MouseEvent,Event);
public:
	AMouseEvent();
	virtual ~AMouseEvent();

	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);

	int		m_X;
	int		m_Y;
	int		m_Delta;//for mousewheel
	AFlagSet	m_KeyStates;
};

//文本修改事件（用于多行编辑器）
class AUI_API ATextEvent : public AEvent
{
	CLASS_INFO(TextEvent,Event);
public:
	ATextEvent();
	virtual ~ATextEvent();

	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);

	ACaretSeat	seatStart;//受影响的起始位子
	ACaretSeat	seatEnd;//受影响的结束位子
	TextAction	action;
};

class AUI_API ADragEvent : public AEvent
{
	CLASS_INFO(DragEvent,Event);
public:
	ADragEvent();
	virtual ~ADragEvent();

	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);

	AControl*	m_pSource;
	ASize		m_szDrawSize;
	int		m_X;
	int		m_Y;
	bool		m_bCanceled;
	bool		m_bAccept;
};

//更新事件（用于Action控件之OnUpdate事件）
class AUI_API AUpdateEvent : public AEvent
{
	CLASS_INFO(UpdateEvent,Event);
public:
	AUpdateEvent();
	virtual ~AUpdateEvent();

	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);

	bool		Enabled;
	bool		Checked;
};

typedef void (AObject::*FEventHandler)(AObject* pSender,AEvent* pEvent);

class AUI_API AEventHandler
{
public:
	AEventHandler();
	virtual ~AEventHandler();
	AEventHandler(AObject* pObj,FEventHandler func);
	AEventHandler(const AEventHandler& e);
	AEventHandler& operator = (const AEventHandler& e);

	bool IsValid();
	void Call(AObject* pSender,AEvent* pEvent);
	void SetMCF(const AString& s);
	AString GetMCF();
private:
	AObject*	m_pObj;
	FEventHandler	m_pFunc;
	AString		m_sMCF;
};

#define EVENT_CONNECT(the_control,the_event,the_handler) \
	the_control->the_event = AEventHandler(this,(FEventHandler)&self_class::the_handler)

#define EVENT_CONNECT2(the_control,the_event,handlerObject,handlerClass,handler) \
	the_control->the_event = AEventHandler(handlerObject,(FEventHandler)&handlerClass::handler)

#endif//AEVENT_H
