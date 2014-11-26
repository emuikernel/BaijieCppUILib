#ifndef AMEMOACTION_H
#define AMEMOACTION_H

#define SEL_TYPE_NONE	0 //未选中
#define SEL_TYPE_ROW	1 //行选择
#define SEL_TYPE_COL	2 //列选择

class AUI_API AMemoAction : public AObject
{
	CLASS_INFO(MemoAction,Object);
public:
	//鼠标和键盘事件
	virtual void OnMouseDown(AObject* pSender,AEvent* pEvent)=0;
	virtual void OnMouseMove(AObject* pSender,AEvent* pEvent)=0;
	virtual void OnMouseUp(AObject* pSender,AEvent* pEvent)=0;
	virtual void OnKeyBoard(AObject* pSender,AEvent* pEvent)=0;
	virtual void OnDblClick(AObject* pSender,AEvent* pEvent)=0;

	//光标控制
	virtual ACaretSeat GetCaretSeat()=0;
	virtual void SetCaretSeat(ACaretSeat seat)=0;//设置光标位子
	virtual ARect GetCaretPos()=0;
	virtual ARect GetCaretPosBySeat(ACaretSeat seat)=0;//根据指定位子获得光标显示区域

	//文本选择
	virtual ACaretSeat GetSelStart()=0;
	virtual ACaretSeat GetSelEnd()=0;
	virtual int GetSelCount()=0;
	virtual AString GetSelText()=0;
	virtual void SetSel(ACaretSeat seatStart,ACaretSeat seatEnd)=0;//块选择
	virtual void SetColSel(ACaretSeat seatStart,ACaretSeat seatEnd)=0;//列块选择
	virtual int GetSelType()=0;

	//文本操作
	virtual void Copy()=0;//拷贝当前选中文本到剪切块
	virtual void Cut()=0;//剪切文本　
	virtual void Paste()=0;//粘贴纯文本到当前光标位子
	virtual void InputChar(ACHAR ch)=0;//普通字符的输入（插入到光标处）
	virtual void InputTab()=0;
	virtual void InputEnter()=0;
	virtual void InputEsc()=0;
	virtual void InputDel()=0;
	virtual void InputBackspace()=0;

};


#endif//AMEMOACTION_H

