#ifndef AMENUBAR_H
#define AMENUBAR_H

class AUI_API AMenuBar: public AContainer
{
	CLASS_INFO(MenuBar,Container);
public:
	AMenuBar(AComponent* pOwner);
	virtual ~AMenuBar();

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void DoClick(AEvent* pEvent);
	virtual void DoMouseMove(AEvent* pEvent);

	virtual AMenu* AddMenu(const AString& sText);
	virtual void RemoveMenu(const AString& sText);
	virtual void RemoveAllMenu();
	virtual AMenu* GetMenu(const AString& sText);
	virtual void DoLayout();
	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void CreateChildren(AMLBody* pNode);
protected:
	AArray		m_aMenu;
	AMenu*		m_pPopupMenu;

	virtual void OnLabelPaint(AObject* p,AEvent* pEvent);
	virtual void OnLabelClick(AObject* p,AEvent* pEvent);
	virtual void OnLabelMouseMove(AObject* p,AEvent* pEvent);
	virtual void OnLabelMouseDown(AObject* p,AEvent* pEvent);

	void DoPopup(AMenu* p,int x,int y);
};

#endif//AMENUBAR_H