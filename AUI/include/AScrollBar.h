#ifndef ASCROLLBAR_H
#define ASCROLLBAR_H

//按钮部件标志
#define SCROLLBAR_ELEM_NONE		0 //无
#define SCROLLBAR_ELEM_BUTTON1		1 //左（上）按钮
#define SCROLLBAR_ELEM_BUTTON2		2 //右（下）按钮
#define SCROLLBAR_ELEM_SLIDE		3 //滑块按钮
#define SCROLLBAR_ELEM_RAIL_1		4 //轨道的上（或左）部分
#define SCROLLBAR_ELEM_RAIL_2		5 //轨道的下（或右）部分
#define SCROLLBAR_ELEM_TOP1		1 //最左（上）按钮
#define SCROLLBAR_ELEM_TOP2		2 //最右（下）按钮

//按钮部件状态
#define SCROLLBAR_STATUS_NORMAL		0 //正常
#define SCROLLBAR_STATUS_DOWN		1 //MouseDown
#define SCROLLBAR_STATUS_OVER		2 //MouseOver
#define SCROLLBAR_STATUS_UP		3 //MouseUp

class AUI_API AScrollBar: public AControl
{
	CLASS_INFO(ScrollBar,Control);
public:
	friend class AContainer;
	AScrollBar(AComponent* pOwner);
	virtual ~AScrollBar();

	//-- 类型：垂直或水平
	virtual eScrollBarKind GetKind()=0;

	//-- 范围
	virtual void SetRange(int v);
	virtual int GetRange();

	//-- 当前位置
	virtual void SetPosition(int v);
	virtual int GetPosition();

	//-- 按钮的显隐
	virtual bool GetShowButton();
	virtual void SetShowButton(bool v);
	virtual bool GetShowTop();
	virtual void SetShowTop(bool v);

	//坐标计算辅助函数
	virtual ARect GetButton1Pos();
	virtual ARect GetButton2Pos();
	virtual ARect GetTopButton1Pos();//置顶
	virtual ARect GetTopButton2Pos();//置底
	virtual ARect GetRailPos_1();
	virtual ARect GetRailPos_2();
	virtual ARect GetRailPos();
	virtual ARect GetSlideButtonPos();
	virtual int GetElemStatus();
	virtual int GetVirtualSize();
	virtual int GetTrueSize();
	virtual double GetRate();
	virtual int GetRailLen();
	virtual int GetMouseOnElem();

	virtual void DoMouseDown(AEvent* pEvent);
	virtual void DoMouseUp(AEvent* pEvent);
	virtual void DoMouseMove(AEvent* pEvent);
	
	
	virtual void DoEnter(AEvent* pEvent);
	virtual void DoLeave(AEvent* pEvent);
	virtual void DoMouseWheel(AEvent* pEvent);
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

	virtual void SetMiniStep(int v);
	virtual int GetMiniStep();
	virtual void SetBigStep(int v);
	virtual int GetBigStep();

	virtual void SetWidth(int v);
	virtual void SetHeight(int v);

	//对ES的支持
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

	bool IsContainerScrollBar();

	virtual AControl* _HitTest(int X,int Y);

	AEventHandler	OnScroll;

protected:
	int		m_iPosition;
	int		m_iRange;

	int		m_MouseOnElem;
	int		m_ElemStatus;

	int		m_iMiniStep;
	int		m_iBigStep;
	APoint		m_ptMouse;
	int		m_iDownPosition;

	bool		m_bShowButton;
	bool		m_bShowTop;//置顶（底）

	eScrollBarKind	m_Kind;

	int ButtonElemOnPos(int X,int Y);
	virtual void _Change(int iSize);
	
};

class AUI_API AHorzScrollBar : public AScrollBar
{
	CLASS_INFO(HorzScrollBar,ScrollBar);
public:
	friend class AContainer;
	AHorzScrollBar(AComponent* pOwner);
	virtual ~AHorzScrollBar(){m_Kind = sbHorz;}
	virtual eScrollBarKind GetKind(){ return m_Kind; }
};

class AUI_API AVertScrollBar : public AScrollBar
{
	CLASS_INFO(VertScrollBar,ScrollBar);
public:
	friend class AContainer;
	AVertScrollBar(AComponent* pOwner);
	virtual ~AVertScrollBar(){m_Kind= sbVert;}

	virtual eScrollBarKind GetKind(){ return m_Kind; }
};

#endif//ASCROLLBAR_H
