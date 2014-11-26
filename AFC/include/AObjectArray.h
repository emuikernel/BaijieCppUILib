#ifndef AOBJECTARRAY_H
#define AOBJECTARRAY_H

typedef void (*FOnObjectArrayDeleteItem)(int index,AObject* v);

/* AObjectArray：对象集合，专门存储对象。只表示存储关系，不表示拥有关系
 * 对象集合会维护对象的引用计数，确保在集合内的元素是有效的，不会被释放。要真正释放，必须先从集合中移除
 */
class AFC_API AObjectArray : public AObject
{
	friend class AObjectArrayEnumerator;
	CLASS_INFO(ObjectArray,Object);
public:
	AObjectArray(void);
	virtual ~AObjectArray(void);

	virtual int GetCount() const;
	virtual void Clear();
	virtual void Add(AObject* v);
	virtual AObject* operator[](int index);
	virtual AObject* GetItem(int index);
	virtual void SetItem(int index,AObject* pObject);
	virtual int IndexOf(AObject* v);
	virtual void Delete(int index);
	virtual void Delete(AObject* v);
	virtual void Insert(int index,AObject* v);
	virtual void Assign(const AObjectArray& a);
	virtual void Sort(int (*sortfunc)(AObject* p1,AObject* p2));

	virtual void SetOnDeleteItem(FOnObjectArrayDeleteItem f);
	virtual FOnObjectArrayDeleteItem GetOnDeleteItem();
protected:
	AObject**		m_aData;
	int		m_iCount;
	int		m_iTotalSize;
	FOnObjectArrayDeleteItem m_OnDelete;

	virtual void SetSize(int iSize);
};

class AFC_API AObjectArrayEnumerator
{
public:
	AObjectArrayEnumerator(AObjectArray* pOwner);
	~AObjectArrayEnumerator();

	virtual bool Next();
	virtual AObject* Cur();
	virtual int Index();
protected:
	bool		m_bEnd;
	AObjectArray*		m_pOwner;
	int		m_iIndex;
};

#endif//AOBJECTARRAY_H
