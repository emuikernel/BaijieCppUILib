#ifndef ATREEVIEWITEM_H
#define ATREEVIEWITEM_H

class ATreeView;
class AUI_API ATreeViewItem: public AContainer
{
	CLASS_INFO(TreeViewItem,Container);
protected:
	friend class ATreeView;
	ATreeViewItem(AComponent* pOwner);
public:
	virtual ~ATreeViewItem();


	ATreeViewItem* AddChild(AString sText=_T(""));
	ATreeView* GetTree();
	ATreeViewItem* GetParentItem();
	int GetChildItemCount();
	ATreeViewItem* GetChildItem(int index);

	virtual void SetExpanded(bool v);
	virtual bool GetExpanded();
	virtual void SetSelected(bool v);
	virtual bool GetSelected();

	virtual int GetImageIndex();
	virtual void SetImageIndex(int v);

	virtual void SetText(AString s);
	virtual void MakeVisible();

	virtual void OnArrowPaint(AObject* p,AEvent* pEvent);
	virtual void OnArrowClick(AObject* p,AEvent* pEvent);

	ALabel* GetTextControl(){ return m_pText; }
	AImageControl* GetIconControl(){ return m_pIcon; }
	AImageControl* GetArrowControl(){ return m_pArrow; }

	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void CreateChildren(AMLBody* pNode);

	AEventHandler		OnExpand;
	AEventHandler		OnCollapse;
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	ATreeView*	m_pOwnerTree;
	ATreeViewItem*	m_pParentItem;
	AArray		m_aChild;
	AImageControl*	m_pArrow;
	AImageControl*	m_pIcon;
	ALabel*		m_pText;
	bool		m_bExpanded;
	bool		m_bSelected;
	int		m_iImageIndex;

	void UpdateIcon();
	void _OnMouseDown(AObject* pSender,AEvent* pEvent);
	void _OnMouseMove(AObject* pSender,AEvent* pEvent);
	void _OnMouseUp(AObject* pSender,AEvent* pEvent);
	void _OnClick(AObject* pSender,AEvent* pEvent);
	void _OnDblClick(AObject* pSender,AEvent* pEvent);
};
#endif//ATREEVIEWITEM_H
