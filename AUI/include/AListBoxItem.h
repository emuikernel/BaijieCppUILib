#ifndef ALISTBOXITEM_H
#define ALISTBOXITEM_H

class AUI_API AListBoxItem: public ALabel
{
	CLASS_INFO(ListBoxItem,Label);
public:
	AListBoxItem(AComponent* pOwner);
	virtual ~AListBoxItem();

	virtual void SetSelected(bool v);
	virtual bool GetSelected();
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	bool		m_bSelected;

};
#endif//ALISTBOXITEM_H
