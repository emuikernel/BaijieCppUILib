#ifndef AMENU_H
#define AMENU_H

class AUI_API AMenu: public APopupContainer
{
	CLASS_INFO(Menu,PopupContainer);
public:
	AMenu(AComponent* pOwner);
	virtual ~AMenu();

	virtual void ClosePopup();//取消弹出
	virtual void HiddenPopups();//隐藏所有的子菜单项

	virtual AMenuItem* AddMenuItem();
	virtual void RemoveMenuItem(AMenuItem* pItem);
	virtual int GetMenuItemCount();
	virtual AMenuItem* GetMenuItem(int iIndex);

	virtual void SetParentMenuItem(AMenuItem* pItem);
	virtual AMenuItem* GetParentMenuItem();

	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void CreateChildren(AMLBody* pNode);

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void DoLostFocus(AEvent* pEvent);

protected:
	AMenuItem*	m_pParentMenuItem;
	virtual void DoLayout();
	virtual AUICore* GetParentUICore();
};
#endif//AMENU_H
