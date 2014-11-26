#ifndef AUICORE_H
#define AUICORE_H

class AUI_API AUICore: public AObject
{
	CLASS_INFO(UICore,Object);
public:
	friend class AWindowBase;
	friend class AWindow;
	friend class AWinControlBase;
	friend class APaintMsgExtend;

	AUICore(AWinControlBase* pForm);
	virtual ~AUICore();

	AWinControlBase* GetWin(){ return m_pWinControl; }

	AControl* GetMouseDownControl();
	void SetMouseDownControl(AControl* pControl);

	AControl* GetMouseOverControl();
	void SetMouseOverControl(AControl* pControl);

	AControl* GetCaptureControl();
	void SetCaptureControl(AControl* pControl);

	AControl* GetPopupControl();
	void SetPopupControl(AControl* pControl);

	void Refresh(ARect r=ARect(0,0,0,0));
	bool InDrawArea(ARect r);
	void SetFocusControl(AControl* p);
	AControl* GetFocusControl();

	void SetOldFocus(AControl* p);//窗口失去焦点前的Focus控件
	AControl* GetOldFocus();

	APoint GetDownPoint();
	void SetDownPoint(APoint pt);

	APoint GetMovePoint();
	void SetMovePoint(APoint pt);

	int GetMousePos();
	void SetMousePos(int nPos);
	
	HWND GetHandle();

	ACanvas* GetCanvas();

	AControl* GetLastClick();
	void SetLastClick(AControl* pLastClick);

	DWORD GetLastTick();
	void SetLastTick(DWORD dw);

	bool GetLayoutEnabled();
	void SetLayoutEnabled(bool v);

	bool GetDisableEvent();
	void SetDisableEvent(bool v);

	bool IsOverMe(AControl* p);
	bool IsFocus(AControl* p);

	void AddWatcher(AControl* p);
	void RemoveWatcher(AControl* p);
	void WatchTo(AControl* p);
	void AddRefresh(AControl* p);
protected:
	AWinControlBase*	m_pWinControl;
	AControl*		m_pFocusControl;
	AControl*		m_pCaptureControl;
	AControl*		m_pPopupControl;//当前弹出控件，一个UICore只能有一个弹出控件
	APoint			m_ptDown;
	APoint			m_ptMove;
	AObjectArray		m_aWatcher;
	AObjectArray		m_aRefresh;

	bool		m_bDisableEvent;//禁止事件产生
	bool		m_bBeginUpdate;
	bool		m_bNeedRefresh;
	ARect		m_rRefreshArea;
	AControl*	m_pMouseDownControl;
	AControl*	m_pMouseOverControl;
	AControl*	m_pOldFocus;

	int		m_iEnableLayout;

	int		m_nMousePos;
	AControl*	m_pClickControl;//last click on
	DWORD		m_dwLastClick;//last click time


	ACanvas*	m_pCanvas;
};

class AUI_API AutoDisableEvent
{
public:
	AutoDisableEvent(AUICore* pCore):m_pCore(pCore)
	{
		m_pCore->SetDisableEvent(true);
	}
	~AutoDisableEvent()
	{
		m_pCore->SetDisableEvent(false);
	}
private:
	AUICore*	m_pCore;
};

class AUI_API AutoDisableLayout
{
public:
	AutoDisableLayout(AUICore* pCore):m_pCore(pCore)
	{
		m_pCore->SetLayoutEnabled(false);
	}
	~AutoDisableLayout()
	{
		m_pCore->SetLayoutEnabled(true);
	}
private:
	AUICore*	m_pCore;
};

#endif//AUICORE_H
