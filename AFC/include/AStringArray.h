
#ifndef ASTRINGARRAY_H
#define ASTRINGARRAY_H


typedef int (*FTextCompareProc)(const AString& s1,const AString& s2);

class AFC_API AStringArray :public AObject
{
	friend class AStringArrayEnumerator;
	CLASS_INFO(StringArray,Object);
public:
	AStringArray(void);
	virtual ~AStringArray(void);
	
	int GetCount();
	AString operator[](int index);
	AString GetItem(int index);
	void SetItem(int index,const AString &s);
	void Clear();
	void Add(const AString& v);
	int IndexOf(const AString& v);
	void Delete(int index);
	void Insert(int index,const AString& s);
	void Assign(const AStringArray& a);
	AString ToString(const AString& split=_T(""));
	void FromString(const AString& sFrom,const AString& split=_T(" "));
	void Sort(FTextCompareProc f=NULL);

protected:
	AString**	m_aData;
	int		m_iCount;
	int		m_iTotalSize;

	void SetSize(int iSize);
};

class AFC_API AStringArrayEnumerator
{
public:
	AStringArrayEnumerator(AStringArray* pOwner);
	~AStringArrayEnumerator();

	virtual bool Next();
	virtual AString Cur();
	virtual int Index();
protected:
	AStringArray*		m_pOwner;
	int			m_iIndex;
	bool			m_bEnd;
};
#endif//ASTRINGARRAY_H
