#ifndef AMEMOACTIONIMPL_H
#define AMEMOACTIONIMPL_H

#include "AMemoTextStorage.h"
class AMemo;

class AUI_API AMemoActionImpl : public AMemoAction
{
	CLASS_INFO(MemoActionImpl,MemoAction);
public:
	AMemoActionImpl(AMemo* pMemo);
	virtual ~AMemoActionImpl();

	//鼠标和键盘事件
	virtual void OnMouseDown(AObject* pSender,AEvent* pEvent);
	virtual void OnMouseMove(AObject* pSender,AEvent* pEvent);
	virtual void OnMouseUp(AObject* pSender,AEvent* pEvent);
	virtual void OnKeyBoard(AObject* pSender,AEvent* pEvent);
	virtual void OnDblClick(AObject* pSender,AEvent* pEvent);

	//光标控制
	virtual ACaretSeat GetCaretSeat();
	virtual void SetCaretSeat(ACaretSeat seat);//设置光标位子
	virtual ARect GetCaretPos();
	virtual ARect GetCaretPosBySeat(ACaretSeat seat);//根据指定位子获得光标显示区域

	//文本选择
	virtual ACaretSeat GetSelStart();
	virtual ACaretSeat GetSelEnd();
	virtual int GetSelCount();
	virtual AString GetSelText();
	virtual void SetSel(ACaretSeat seatStart,ACaretSeat seatEnd);//块选择
	virtual void SetColSel(ACaretSeat seatStart,ACaretSeat seatEnd);//列块选择
	virtual int GetSelType();

	//文本操作
	virtual void Copy();//拷贝当前选中文本到剪切块
	virtual void Cut();//剪切文本　
	virtual void Paste();//粘贴纯文本到当前光标位子
	virtual void InputChar(ACHAR ch);//普通字符的输入（插入到光标处）
	virtual void InputTab();
	virtual void InputEnter();
	virtual void InputEsc();
	virtual void InputDel();
	virtual void InputBackspace();
protected:
	AMemo*		m_pMemo;

	int			m_iCaretStep;//控制光标闪烁
	ACaretSeat		m_seatCaret;//光标位子
	ARect			m_rCaret;
	APoint			m_ptDown;

	//选择
	ACaretSeat		m_seatSelStart;
	ACaretSeat		m_seatSelEnd;
	int			m_iSelType;

	virtual void _updateCaretPos();
	virtual int _getLineHeight(int iLine);
	virtual void UpdateEditorSize();
	virtual void _unselect();
	virtual void _delSelected();
	virtual ACaretSeat _toNext(ACaretSeat seat);
	virtual ACaretSeat _toPrev(ACaretSeat seat);
};


#endif//AMEMOACTIONIMPL_H

