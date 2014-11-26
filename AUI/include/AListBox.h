#ifndef ALISTBOX_H
#define ALISTBOX_H

class AUI_API AListBox: public AContainer
{
	CLASS_INFO(ListBox,Container);
public:
	AListBox(AComponent* pOwner);
	virtual ~AListBox();

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

	virtual void AddString(const AString& s);
	virtual int GetItemCount();
	virtual AString GetItem(int index);
	virtual int GetItemIndex();
	virtual void SetItemIndex(int index);

	virtual void SetMultiSelection(bool v);
	virtual bool GetMultiSelection();
	virtual void SetItemHeight(int v);
	virtual int GetItemHeight();
	virtual void OnItemClick(AObject* p,AEvent* pEvent);
	virtual AString GetSelect();
	virtual void SetSelected(const AString& s);

	virtual void DoLayout();

	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void CreateChildren(AMLBody* pNode);

	//对ES的支持
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

	AEventHandler		OnItemPaint;
	AEventHandler		OnSelChange;

protected:
	int		m_iItemHeight;
	bool		m_bMultiSelection;
};
#endif//ALISTBOX_H
