#ifndef ASTRINGMAP_H
#define ASTRINGMAP_H

#define KSTRING_MAP_SIZE	1021

class AFC_API AStringMap :public AObject
{
	friend class AStringMapEnumerator;
	CLASS_INFO(StringMap,Object);
public:
	AStringMap(void);
	virtual ~AStringMap(void);

	class AFC_API myitem
	{
	public:
		myitem()
		{
			pData = NULL;
			index = 0;
			nHashValue = 0;
			pNext = NULL;
		}
		~myitem(){}
		AString sKey;
		void* pData;

		int index;
		unsigned int nHashValue;
		
		myitem* pNext;
	};

	virtual void SetItem(const AString& sKey,void* pData);
	virtual void* GetItem(const AString& sKey);
	virtual void RemoveItem(const AString& sKey);
	virtual bool IsExist(const AString& sKey);
	virtual void Delete(myitem* p);
	virtual void Clear();
	virtual myitem* Begin();
	virtual bool IsEnd(myitem* p);
	virtual myitem* Next(myitem* p);
	virtual int GetCount();

protected:
	myitem* m_Data[KSTRING_MAP_SIZE];
	unsigned int GetHashValue(const AString& s);
	myitem* _getitem(const AString& sKey);
	myitem* _newitem(const AString& sKey);
};

class AFC_API AStringMapEnumerator
{
public:
	AStringMapEnumerator(AStringMap* pOwner);
	~AStringMapEnumerator();

	virtual bool Next();
	virtual void* Cur();
	virtual AString Index();
protected:
	AStringMap*		m_pOwner;
	AStringMap::myitem*	m_pItem;
	bool			m_bEnd;
};

#endif//ASTRINGMAP_H
