#ifndef APOPUPLISTBOX_H
#define APOPUPLISTBOX_H

class AUI_API APopupListBox: public APopupContainer
{
	CLASS_INFO(PopupListBox,PopupContainer);
public:
	APopupListBox(AComponent* pOwner);
	virtual ~APopupListBox();

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

	virtual void AddItem(const AString& s);
	virtual int GetItemCount();
	virtual AString GetItem(int index);
	virtual int GetItemIndex();
	virtual void SetItemIndex(int index);

	virtual AString GetSelected();

	AEventHandler		OnSelected;
protected:
	virtual void OnListBoxSelChange(AObject* pSender,AEvent* pEvent);
	AListBox*	m_pListBox;
	virtual void DoLayout();

};
#endif//APOPUPLISTBOX_H
