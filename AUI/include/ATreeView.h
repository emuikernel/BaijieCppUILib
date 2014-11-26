#ifndef ATREEVIEW_H
#define ATREEVIEW_H

typedef bool (*FWalkTreeItemFunc)(ATreeViewItem* pItem,void* pArg);//返回false时停止遍历


class AUI_API ATreeView: public AContainer
{
	CLASS_INFO(TreeView,Container);
	friend class ATreeViewEnumerator;
	friend class ATreeViewItem;
public:
	ATreeView(AComponent* pOwner);
	virtual ~ATreeView();

	virtual void SetIndent(int v);
	virtual int GetIndent();

	virtual ATreeViewItem* GetHoveredItem();
	virtual ATreeViewItem* AddItem(ATreeViewItem* pParentItem=NULL,AString sText=_T(""));

	virtual void DoLayout();
	virtual void WalkItem(FWalkTreeItemFunc f,void* pArg);

	virtual ATreeViewItem* GetSelected();
	virtual void SetSelected(ATreeViewItem* pItem);

	virtual void RemoveItem(ATreeViewItem* pItem);
	virtual void RemoveAllItem();

	virtual void SetImageList(AImageList* pImageList);
	virtual AImageList* GetImageList();

	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void CreateChildren(AMLBody* pNode);

	virtual void MakeVisible(ATreeViewItem* pItem);

	//对ES的支持
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

	AEventHandler		OnChange;//选中项改变
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

	int _calcItem(ATreeViewItem* pItem,ACanvas* pCanvas);
	bool _walkItem(ATreeViewItem* pItem,FWalkTreeItemFunc f,void* pArg);
	AArray		m_aChild;
	ATreeViewItem*	m_pHoveredItem;
	ATreeViewItem*	m_pSelectedItem;
	int		m_iIndent;
	AImageList*	m_pImageList;

	virtual void ESSetValue(AMLBody* pNode);
	virtual void ESGetValue(AMLBody* pNode);

};

class AUI_API ATreeViewEnumerator : public AObject
{
	CLASS_INFO(TreeViewEnumerator,Object);
public:
	ATreeViewEnumerator(ATreeView* pView);
	virtual ~ATreeViewEnumerator();

	bool Next();
	ATreeViewItem* Cur();
private:
	ATreeView*	m_pView;
	bool		m_bEnd;
	
	ATreeViewItem*	m_pCurItem;//当前节点
	int		m_iIndex;//当前节点在同级“子节点集合中”的下标
	AArray		m_aParentIndex;//父节点在它的同级“子节点集合”中的下标

	bool NextNode(ATreeViewItem* pNode);//根据当前节点定位到下一个遍历节点
};

#endif//ATREEVIEW_H
