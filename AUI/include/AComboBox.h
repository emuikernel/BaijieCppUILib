#ifndef ACOMBOBOX_H
#define ACOMBOBOX_H

class AUI_API AComboBox: public AContainer
{
	CLASS_INFO(ComboBox,Container);
public:
	AComboBox(AComponent* pOwner);
	virtual ~AComboBox();

	virtual void SetDropDownWidth(int v);
	virtual int GetDropDownWidth();
	virtual void SetDropDownHeight(int v);
	virtual int GetDropDownHeight();

	virtual void SetHeight(int v);
	virtual void SetWidth(int v);

	virtual bool GetCanEdit();
	virtual void SetCanEdit(bool v);

	virtual int GetItemIndex();
	virtual void SetItemIndex(int index);

	virtual int GetItemCount();
	virtual AString GetItem(int index);

	virtual void OnButtonPaint(AObject* p,AEvent* pEvent);
	virtual void OnEditPaint(AObject* p,AEvent* pEvent);
	virtual void OnButtonClick(AObject* p,AEvent* pEvent);
	virtual void DoClick(AEvent* pEvent);

	virtual void SetText(AString s);

	virtual void AddString(const AString& s);
	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void CreateChildren(AMLBody* pNode);

	AEventHandler		OnSelChange;
	AEventHandler		OnTextChange;
protected:
	AEdit*		m_pEdit;
	AButton*	m_pButton;
	APopupListBox*	m_pListBox;
	int		m_iDropDownWidth;
	int		m_iDropDownHeight;//0:代表默认 由系统自动计算高度
	bool		m_bCanEdit;
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void RecalLayout();
	void OnEditMouseDown(AObject* pSender,AEvent* pEvent);
	virtual void OnListKillFocus(AObject* pSender,AEvent* pEvent);
	virtual void OnListSelect(AObject* pSender,AEvent* pEvent);
	virtual void OnEditChange(AObject* p,AEvent* pEvent);

};
#endif//ACOMBOBOX_H
