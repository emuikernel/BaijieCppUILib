#ifndef MENUITEM_H
#define MENUITEM_H

class AMenu;
class AUI_API AMenuItem : public AContainer
{
	CLASS_INFO(MenuItem,Container);
public:
	friend class AMenu;
	AMenuItem(AComponent* pOwner);
	virtual ~AMenuItem();

	virtual void SetImageIndex(int iIndex);
	virtual int GetImageIndex();

	virtual AImageList* GetImageList();
	virtual void SetImageList(AImageList* pImageList);

	virtual void SetChecked(bool bChecked);
	virtual bool GetChecked();

	virtual void SetTextAlign(TTextAlign textAlign);
	virtual TTextAlign GetTextAlign();

	virtual void SetPopupMenu(AMenu* pMenu);
	virtual AMenu* GetPopupMenu();

	virtual bool IsPopupState();

	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void CreateChildren(AMLBody* pNode);

	void OnFontChange();
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void DoEnter(AEvent* pEvent);
	virtual void DoLeave(AEvent* pEvent);
	virtual void DoClick(AEvent* pEvent);
	virtual void DoMouseDown(AEvent* pEvent);

	virtual AAction* GetAction();
	virtual void SetAction(AAction* pAction);
protected:
	int		m_iImageIndex;
	AImageList*	m_pImageIndex;
	bool		m_bChecked;
	TTextAlign	m_TextAlign;
	AMenu*		m_pPopupMenu;
	bool		m_bInPopup;
	AAction*	m_pAction;
	
};

#endif//MENUITEM_H

