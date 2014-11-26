#ifndef ALISTHEADERITEM_H
#define ALISTHEADERITEM_H

class AUI_API AListHeaderItem: public ALabel
{
	CLASS_INFO(ListHeaderItem,Label);
public:
	AListHeaderItem(AComponent* pOwner);
	virtual ~AListHeaderItem();

	virtual void SetContentTextAlign(TTextAlign al);
	virtual TTextAlign GetContentTextAlign();

	virtual void SetPropFromNode(AMLBody* pNode);
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);


	TTextAlign	m_ContentTextAlign;
};
#endif//ALISTHEADERITEM_H
