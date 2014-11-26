
#ifndef AEDIT_H
#define AEDIT_H

/* Afc单行编辑框
 *支持功能：
 *	1. 字符输入、删除
 *	2. 焦点跳转（未实现）
 *	3. 光标左右移动
 *	4. 鼠标点击确定光标位置
 *	5. 鼠标选择（未实现）
 *	6. 键盘选择（未实现）
 *	7. 剪切板操作（未实现）注意：密码模式下不允许拷贝出去
 *	8. 插入/替换模式（未实现）
 *	9. 移动光标超过可视区域时自动滚动
 *	10.密码框支持
 *
 *
 */
class AUI_API AEdit : public AControl
{
	CLASS_INFO(Edit,Control);
public:
	AEdit(AComponent* pOwner);
	virtual ~AEdit(void);

	virtual bool GetReadOnly();
	virtual void SetReadOnly(bool b);

	virtual void DoFocus(AEvent* pEvent);
	virtual void DoLostFocus(AEvent* pEvent);

	virtual void DoMouseDown(AEvent* pEvent);
	virtual void DoMouseMove(AEvent* pEvent);

	virtual int GetCaretPos();
	virtual int GetScrollPos();
	virtual int GetSelStartPos();
	virtual int GetSelEndPos();

	virtual void SetCaretIndex(int iPos);//字符位置
	virtual int GetCaretIndex();

	virtual int GetSelStart();
	virtual void SetSelStart(int iIndex);

	virtual int GetSelCount();
	virtual void SetSelCount(int iCount);

	virtual ACHAR GetMaskChar();
	virtual void SetMaskChar(ACHAR ch);

	virtual bool CanFocus();

	virtual bool ShowCaret();

	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void CreateChildren(AMLBody* pNode);

	AEventHandler		OnKeyDown;
	AEventHandler		OnKeyPress;
	AEventHandler		OnKeyUp;
	AEventHandler		OnChange;


	void OnFontChange();

	virtual void DoKeyboard(AEvent* pEvent);
	virtual void DoDblClick(AEvent* pEvent);
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

	virtual void _updatePos();//更新光标坐标和滚动距离
	virtual void _updateSelPos();
	void _onCaretTimer(AObject* pAsso,int nTimerID);

	bool			m_bReadOnly;
	int			m_iCaretPos;
	int			m_iCaretIndex;
	int			m_iScrollPos;//滚动位置（距离）
	int			m_nCaretTimerID;
	int			m_iCaret;//是否显示光标（用于闪烁光标）
	ACHAR			m_chMaskChar;
	int			m_iSelStart;
	int			m_iSelCount;
	int			m_iSelStartPos;
	int			m_iSelEndPos;
};


#endif//AEDIT_H
