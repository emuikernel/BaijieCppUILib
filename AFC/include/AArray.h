#ifndef AARRAY_H
#define AARRAY_H

typedef void (*FOnArrayDeleteItem)(int index,void* v);

/* AArray : 集合类，可以存储任何类型，但需要转成void*类型
 * 不负责存储物的有效生命周期管理，只负责存储
 */
class AFC_API AArray : public AObject
{
	friend class AArrayEnumerator;
	CLASS_INFO(Array,Object);
public:
	AArray(void);
	virtual ~AArray(void);

	virtual int GetCount() const;
	virtual void Clear();
	virtual void Add(void* v);
	virtual void* operator[](int index);
	virtual void* GetItem(int index);
	virtual void SetItem(int index,void* p);
	virtual int IndexOf(void* v);
	virtual void Delete(int index);
	virtual void Insert(int index,void* v);
	virtual void Assign(const AArray& a);
	virtual void Sort(int (*sortfunc)(void* p1,void* p2));

	virtual void SetOnDeleteItem(FOnArrayDeleteItem f);
	virtual FOnArrayDeleteItem GetOnDeleteItem();
protected:
	void**		m_aData;
	int		m_iCount;
	int		m_iTotalSize;
	FOnArrayDeleteItem m_OnDelete;

	virtual void SetSize(int iSize);
};

class AFC_API AArrayEnumerator
{
public:
	AArrayEnumerator(AArray* pOwner);
	~AArrayEnumerator();

	virtual bool Next();
	virtual void* Cur();
	virtual int Index();
protected:
	AArray*		m_pOwner;
	int		m_iIndex;
	bool		m_bEnd;
};

#endif//AARRAY_H
