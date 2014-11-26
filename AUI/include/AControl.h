#ifndef ACONTROL_H
#define ACONTROL_H

class AUICore;

class AUI_API AControl: public AComponent
{
	friend class AContainer;
	CLASS_INFO(Control,Component);
public:
	AControl(AComponent* pOwner);
	virtual ~AControl();

	AFont			Font;

	AColor			BorderColor;
	AAnchors		Anchors;

	static AControl* CreateInstance(const AString& sClassName,AComponent* pParent);
	static AControl* CreateInstance(AMLBody* pBody,AComponent* pParent);

	virtual void SetParent(AControl* pParent);
	virtual AControl* GetParent();

	virtual void SetName(const AString& sName);
	virtual AString GetName();

	virtual void SetEnabled(bool b);
	virtual bool GetEnabled();

	virtual void SetVisible(bool b);
	virtual bool GetVisible();

	virtual void SetFloatType(TFloatType ft);
	virtual TFloatType GetFloatType();

	virtual void SetLeft(int v);
	virtual int GetLeft();

	virtual void SetTop(int v);
	virtual int GetTop();

	virtual void SetWidth(int v);
	virtual int GetWidth();

	virtual void SetHeight(int v);
	virtual int GetHeight();

	virtual void SetZOrder(int v);
	virtual int GetZOrder();

	virtual void SetAlign(eAlign v);
	virtual eAlign GetAlign();

	virtual void SetText(AString s);
	virtual AString GetText();

	virtual void SetHint(AString s);
	virtual AString GetHint();

	virtual void SetTag(void* p);
	virtual void* GetTag();

	virtual void SetBorderWidth(int v);
	virtual int GetBorderWidth();

	virtual void SetPadding(int v);
	virtual int GetPadding();

	virtual void SetCursorType(TCursorType ct);
	virtual TCursorType GetCursorType();

	virtual void SetDesignMode(bool v);
	virtual bool GetDesignMode();

	virtual void SetUseSkin(bool v);
	virtual bool GetUseSkin();

	virtual void SetColor(AColor cr);
	virtual AColor GetColor();

	virtual bool CanFocus();
	virtual void SetCanFocus(bool v);

	virtual bool IsChildOf(AControl* pParent);

	virtual void Refresh();

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void DoMouseDown(AEvent* pEvent);
	virtual AControl* _HitTest(int X,int Y);
	virtual void DoMouseUp(AEvent* pEvent);
	virtual void DoMouseMove(AEvent* pEvent);
	virtual void DoMouseWheel(AEvent* pEvent);
	virtual void DoClick(AEvent* pEvent);
	virtual void DoDblClick(AEvent* pEvent);
	virtual void DoEnter(AEvent* pEvent);
	virtual void DoLeave(AEvent* pEvent);
	virtual void DoFocus(AEvent* pEvent);
	virtual void DoLostFocus(AEvent* pEvent);
	virtual void DoKeyboard(AEvent* pEvent);
	
	virtual ARect GetAbsPosition();//控件绝对位置是相对于AFC窗口的位置，不是屏幕位置
	virtual ARect GetControlScrollPosition();//控件相对位置（带滚动条滚动后位置的修正）
	virtual ARect GetControlPosition();//控件相对位置，相对于它的父控件的位置
	virtual ARect GetScreenPosition();//控件相对于屏幕的绝对位置

	virtual void SetPropFromNode(AMLBody* pNode);

	virtual AUICore* GetUICore();
	virtual void Destroy();
	virtual bool IsDestroyed();

	virtual void DoLayout();
	virtual void StartDrag(AControl* pSource=NULL,ASize szDraw=ASize(0,0));

	virtual void WatchDelete(AControl* p);//内部使用

	//对ES的支持
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

	AEventHandler	OnMouseDown;
	AEventHandler	OnMouseUp;
	AEventHandler	OnMouseMove;
	AEventHandler	OnClick;
	AEventHandler	OnPaint;
	AEventHandler	OnEnter;
	AEventHandler	OnLeave;
	AEventHandler	OnKillFocus;
	AEventHandler	OnFocus;
	AEventHandler	OnMouseWheel;
	AEventHandler	OnDblClick;
	AEventHandler	OnKeyboard;//键盘事件
	AEventHandler	OnStartDrag;
	AEventHandler	OnDragOver;
	AEventHandler	OnDragDrop;
	AEventHandler	OnEndDrag;
	AEventHandler	OnDrawDrag;//绘制拖放图标
	AEventHandler	OnLayout;
	AEventHandler	OnDestroy;
protected:
	AControl*	m_pParent;
	AUICore*	m_pUICore;
	AString		m_sName;
	AString		m_sText;
	AString		m_sHint;
	bool		m_bEnabled;
	bool		m_bVisible;
	TFloatType	m_FloatType;
	int		m_iLeft;
	int		m_iTop;
	int		m_iWidth;
	int		m_iHeight;
	ARect		m_rAbsPos;//相对于窗口的绝对坐标
	int		m_iZOrder;
	void*		m_pTag;
	int		m_iBorderWidth;
	int		m_iPadding;
	TCursorType	m_CursorType;
	eAlign		m_Align;
	bool		m_bIsDesign;
	AColor		m_Color;
	bool		m_bUseSkin;//是否使用皮肤配置，否的话就使用控件自己的设置
	bool			m_bDestroyed;
	AObjectArray		m_aWatchDelete;
	bool		m_bCanFocus;
};


#endif//ACONTROL_H
