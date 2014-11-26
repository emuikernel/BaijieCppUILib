#ifndef ALISTHEADER_H
#define ALISTHEADER_H

class AUI_API AListHeader: public AContainer
{
	CLASS_INFO(ListHeader,Container);
public:
	AListHeader(AComponent* pOwner);
	virtual ~AListHeader();

	virtual void DoLayout();

	virtual AListHeaderItem* GetListHeaderItem(int index);
	virtual int GetListHeaderItemCount();
	virtual AListHeaderItem* AddItem();

	virtual void CreateChildren(AMLBody* pNode);
	virtual void SetPropFromNode(AMLBody* pNode);

	void OnSplitMouseUp(AObject* pSender,AEvent* pEvent);
	void OnSplitPaint(AObject* pSender,AEvent* pEvent);
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

};
#endif//ALISTHEADER_H
