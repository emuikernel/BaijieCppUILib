#ifndef ALISTVIEWITEM_H
#define ALISTVIEWITEM_H

class AListView;
class AUI_API AListViewItem: public ALabel
{
	CLASS_INFO(ListViewItem,Label);
public:
	AListViewItem(AComponent* pOwner);
	virtual ~AListViewItem();

	AStringArray	SubItems;


	AListView* GetListView();

	AString operator[](const AString& sColumn);
	AString operator[](int i);
	void SetSelected(bool v);
	bool GetSelected();
	void SetImageIndex(int v);
	int GetImageIndex();

	virtual void DoDblClick(AEvent* pEvent);
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	bool		m_bSelected;
	int		m_iImageIndex;

};
#endif//ALISTVIEWITEM_H
