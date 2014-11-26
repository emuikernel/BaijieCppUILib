#ifndef AOBJECT_H
#define AOBJECT_H

#define CLASS_INFO(TYPE,PARENT) \
	public:\
	AObject* ToClass(LPCTSTR pstrName)\
	{\
		if( pstrName != NULL && _tcscmp(pstrName,_T(#TYPE)) == 0 ) return this;\
		else return A##PARENT::ToClass(pstrName);\
	}\
	LPCTSTR GetClass() const\
	{\
		return _T(#TYPE);\
	}\
	typedef A##TYPE self_class;\
	typedef A##PARENT base_class;
	

class AFC_API AObject
{
public:
	AObject();
	virtual ~AObject();

	virtual void Release();
	virtual int AddRef();

	virtual AObject* ToClass(LPCTSTR pstrName)
	{
		if( pstrName != NULL && _tcscmp(pstrName,_T("Object")) == 0 ) return this;
		else return NULL;
	}
	virtual LPCTSTR GetClass() const
	{
		return _T("Object");
	}
protected:
	LONG	m_iRefCount;
};


#endif