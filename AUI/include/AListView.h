#ifndef ALISTVIEW_H
#define ALISTVIEW_H

class AUI_API AListView: public AContainer
{
	CLASS_INFO(ListView,Container);
public:
	friend class AListHeader;
	AListView(AComponent* pOwner);
	virtual ~AListView();

	virtual void SetViewStyle(TListViewStyle vs);
	virtual TListViewStyle GetViewStyle();
	virtual void SetItemHeight(int v);
	virtual int GetItemHeight();
	virtual void SetMultiSelection(bool v);
	virtual bool GetMultiSelection();

	virtual void DoLayout();

	virtual AListViewItem* GetSelected();
	virtual void SetSelected(AListViewItem* pSel);

	virtual int GetSelectedIndex();
	virtual void SetSelectedIndex(int index);

	virtual AListHeader* GetHeader();
	virtual AListViewItem* AddItem();
	virtual AObjectArray* GetItems();
	virtual void RemoveAllItems();

	virtual void SetLargeImageList(AImageList* pImageList);
	virtual AImageList* GetLargeImageList();
	virtual void SetSmallImageList(AImageList* pImageList);
	virtual AImageList* GetSmallImageList();

	//根据字符串设置Head
	//格式：Head1:width1,Head2:width2,Head3:width3,etc.
	virtual void SetHeaders(const AString& s);

	virtual void OnListItemClick(AObject* p,AEvent* pEvent);

	virtual void CreateChildren(AMLBody* pNode);
	virtual void SetPropFromNode(AMLBody* pNode);

	//对ES的支持
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

	AListHeader*	m_pListHeader;
	AContainer*	m_pItemContainer;
	AImageList*	m_pLargeImageList;
	AImageList*	m_pSmallImageList;
	TListViewStyle	m_ViewStyle;
	int		m_iItemHeight;
	bool		m_bMultiSelection;

	virtual void ESSetValue(AUI_ES_VALUE_INTF* pValue);
	virtual void ESGetValue(AUI_ES_VALUE_INTF* pValue);
	virtual void ESAdd(AStringArray& sa);
};
#endif//ALABEL_H
