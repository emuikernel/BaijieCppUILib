#ifndef ACONTAINER_H
#define ACONTAINER_H

class AScrollBar;
class AUI_API AContainer:public AControl
{
	CLASS_INFO(Container,Control);
public:
	friend class AWindow;
	friend class AScrollBar;
	AContainer(AComponent* pOwner);
	virtual ~AContainer();

	virtual void SetAutoHScrollBar(bool v);
	virtual bool GetAutoHScrollBar();

	virtual void SetAutoVScrollBar(bool v);
	virtual bool GetAutoVScrollBar();

	virtual void SetHScrollBar(AScrollBar* p);
	virtual void SetVScrollBar(AScrollBar* p);
	virtual AScrollBar* GetHScrollBar();
	virtual AScrollBar* GetVScrollBar();

	virtual AControl* GetMaximizedControl();
	virtual void SetMaximizedControl(AControl* p);

	virtual AControl* _HitTest(int X,int Y);
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void DoMouseWheel(AEvent* pEvent);

	virtual ASize GetScrollSize();
	
	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void CreateChildren(AMLBody* pNode);
	AControl* FindControl(const AString& sName);

	virtual void SetWidth(int v);
	virtual void SetHeight(int v);
	virtual void DoLayout();
	virtual ARect GetViewRect();
	virtual AObjectArray* GetChildControls();

	virtual void RemoveAllChild();
	virtual void Destroy();
	
protected:
	AObjectArray		m_aChildren;
	bool			m_bAutoHScrollBar;
	bool			m_bAutoVScrollBar;
	
	
	AControl*		m_pMaximizedControl;//最大化的子控件（保留，将来支持子控件最大化时使用）
	
	virtual void _Layout(bool bInitLayout);
	virtual void _updateChildrenAbsPos();
	virtual void _addWatch(AControl* p);
protected:
	bool		m_bHorzScrollBarShow;
	bool		m_bVertScrollBarShow;
	ASize		m_ContentSize;
	ARect		m_rView;
	AScrollBar*	m_pHorzScrollBar;
	AScrollBar*	m_pVertScrollBar;
	bool		m_bFirstLayout;
	ASize		m_szDelta;//去掉alTop和alLeft后的位置

	void IsScrollBarShow(bool& bHorz,bool& bVert);

protected:

	virtual void DoPaintChildren(AEvent* pEvent);
	
	virtual void DoLButtonDown(AEvent* pEvent);
	virtual void DoLButtonUp(AEvent* pEvent);
	virtual void DoMouseMove(AEvent* pEvent);
};

#endif//ACONTAINER_H
