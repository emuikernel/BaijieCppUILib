#ifndef AWINCONTROLBASE_H
#define AWINCONTROLBASE_H

class ATooltip;

/*
 * AWinControlBase ： AFC中带窗口句柄的控件，顶级窗口控件，除此以外的任何控件都是“无句柄”的控件
 */
class AUI_API AWinControlBase: public AContainer
{
	CLASS_INFO(WinControlBase,Container);
public:
	friend class AWindow;
	friend class AApplication;

	AWinControlBase(AComponent* pOwner);
	virtual ~AWinControlBase();

	//=== 位置
	virtual ARect GetAbsPosition();//控件绝对位置是相对于AFC窗口的位置，不是屏幕位置
	virtual ARect GetControlScrollPosition();//控件相对位置（带滚动条滚动后位置的修正）
	virtual ARect GetControlPosition();//控件相对位置，相对于它的父控件的位置

	//=== 窗口句柄（Microsoft Windows中的名柄）
	virtual HWND GetHandle();
	virtual AWindow* GetWindow();//得到与之对应的句柄窗口

	//=== 创建
	virtual bool Create();

	//=== 显示与隐藏
	virtual void SetVisible(bool b);
	virtual void Show();
	virtual void ShowModal();
	virtual void Hide();

	//=== 重载以处理与句柄窗口的关系
	virtual void Destroy();
	virtual void WatchDelete(AControl* p);//内部使用

	virtual void DoMove(AEvent* pEvent);
	virtual void DoResize(AEvent* pEvent);

	//=== 位置
	virtual int GetLeft();
	virtual int GetTop();
	virtual int GetWidth();
	virtual int GetHeight();
	virtual TWindowPos GetPositionType();
	virtual void SetPositionType(TWindowPos wp);
	virtual void DoLayout();
	virtual bool GetSizable();
	virtual void SetSizable(bool v);


	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void CreateChildren(AMLBody* pNode);

	//对ES的支持
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

	//=== 事件
	AEventHandler		OnShow;
	AEventHandler		OnHide;
	AEventHandler		OnMove;
	AEventHandler		OnResize;
	AEventHandler		OnCreate;
	AEventHandler		OnCloseQuery;
	AEventHandler		OnClose;
protected:
	AWindow*	m_pWindow;
	bool		m_bModal;
	TWindowPos	m_WindowPos;
	bool		m_bSizable;
	bool		m_bDestroyFromWin;
	
	

	
	virtual void InitControls();
	virtual void ConnectEvent();
	virtual void DoCreate();
	virtual AWindow* CreateWnd();

	
	virtual void _lastRelease();

	static void MessageLoop();
	
};
#endif//AWINCONTROLBASE_H