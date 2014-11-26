#ifndef AIMAGELIST_H
#define AIMAGELIST_H

class AUI_API AImageList: public AInvisibleControl
{
	CLASS_INFO(ImageList,InvisibleControl);
public:
	AImageList(AComponent* pOwner);
	virtual ~AImageList();

	ASize	Size;

	void Add(AImage* pGraphic,const AString& sKey=_T(""));
	void Clear();
	void Remove(AImage* pGraphic);
	AImage* GetItem(int index);
	int GetCount();
	int GetIndex(const AString& sKey);
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	AObjectArray		m_aImage;
	AStringMap	m_aIndex;
};
#endif//AIMAGELIST_H
