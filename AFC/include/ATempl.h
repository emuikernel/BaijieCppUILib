#ifndef ATEMPL_H
#define ATEMPL_H


template <typename T>
inline T afc_min(T a, T b ) {  return a < b? a : b;  }

template <typename T>
inline T afc_max(T a, T b ) {  return a > b? a : b;  }

template< typename T >
class afc_object
{
public:
	explicit afc_object(const T* p): m_pObj((T*)p)
	{
		this->AddRef();
	}
	explicit afc_object() : m_pObj(NULL)
	{
	}
	afc_object(const afc_object& rhs)
	{
		m_pObj = (T*)rhs.m_pObj;
		this->AddRef();
	}

	afc_object(AObject * r):m_pObj( (T *)r)
	{
		this->AddRef();
	}

	virtual ~afc_object()
	{
		this->Release();  // 计数递减
	}
	afc_object& operator=(const afc_object& rhs)
	{
		if(m_pObj != rhs.m_pObj)  // 避免自赋值
		{
			this->Release(); // 计数递减，不再共享原对象
			m_pObj = (T *)rhs.m_pObj;  // 共享新的对象
			this->AddRef(); // 计数递增，维护正确的引用计数值
		}
		return *this;
	}
	afc_object& operator=(const T* rhs)
	{
		if(m_pObj != rhs)  // 避免自赋值
		{
			this->Release();  // 计数递减，不再共享原对象
			m_pObj = (T *)rhs;
			this->AddRef();
		}
		return *this;
	}
	afc_object& operator=(AObject *rhs)
	{
		return this->operator=( (T *)rhs);
	}
	bool operator == (AObject *p)
	{
		return ((AObject *)m_pObj == p ? true : false);
	}
	bool operator ==(const afc_object& rhs)
	{
		return (m_pObj == rhs.m_pObj ? true : false );
	}
	bool operator !=(const afc_object& rhs)
	{
		return (m_pObj != rhs.m_pObj ? true : false );
	}
	bool operator ==(const T* p)
	{
		return (m_pObj == p ? true : false);
	}
	bool operator !=(const T* p)
	{
		return (m_pObj != p ? true : false);
	}
	bool operator != (AObject *p)
	{
		return ((AObject *)m_pObj != p ? true : false);
	}
	operator AObject *()
	{
		return (AObject *)m_pObj;
	}
	T* get() const { return m_pObj; }
	T* operator->() const
	{
		return m_pObj;
	}
	T* operator &() const
	{
		return m_pObj;
	}
	T& operator*() const
	{
		return *m_pObj;
	}
	void AddRef()
	{
		if( m_pObj ) m_pObj->AddRef();
	}
	void Release()
	{
		if( m_pObj )
		{
			m_pObj->Release();
			m_pObj = NULL;
		}
	}
private:
	T*		m_pObj;
};

//afc_object<T>向量
template< typename T >
class afc_vector
{
public:
	class iterator
	{
	public:
		iterator()
		{
			m_pOwner = NULL;
			m_iIndex = 0;
		}
		~iterator()
		{
		}
		iterator(const iterator& it)
		{
			m_pOwner = it.m_pOwner;
			m_iIndex = it.m_iIndex;
		}
		iterator& operator =(const iterator& it)
		{
			m_pOwner = it.m_pOwner;
			m_iIndex = it.m_iIndex;
			return *this;
		}
		T* operator *()
		{
			return (*m_pOwner)[m_iIndex];
		}
		T* operator ->()
		{
			return (*m_pOwner)[m_iIndex];
		}

		void operator ++()
		{
			m_iIndex ++;
		}
		void operator ++(int)
		{
			m_iIndex ++;
		}
		void operator --()
		{
			m_iIndex --;
		}
		void operator --(int)
		{
			m_iIndex --;
		}
		bool operator ==(const iterator& it)
		{
			return (m_iIndex == it.m_iIndex ? true : false);
		}
		bool operator !=(const iterator& it)
		{
			return (m_iIndex != it.m_iIndex ? true : false);
		}
		void remove()
		{
			m_pOwner->erase(m_iIndex);
		}
		afc_vector<T>* m_pOwner;
		int m_iIndex;
	};
	afc_vector()
	{
		m_iSize = 0;
		__iSize = 0;
		m_pVector = NULL;
	}
	afc_vector(UINT iStep )
	{
		m_iSize = 0;
		__iSize = 0;
		m_pVector = NULL;
	}
	~afc_vector()
	{
		free();
	}
	void reverse(int iSize)
	{
		_size(iSize);
	}
	//真正地释放内存
	void free()
	{
		_size(0);
		if( m_pVector )
		{
			delete[] m_pVector;
			m_pVector = NULL;
			__iSize = 0;
		}
	}

	iterator begin()
	{
		iterator it;
		it.m_pOwner = this;
		it.m_iIndex = 0;
		return it;
	}
	iterator end()
	{
		iterator it;
		it.m_pOwner = this;
		it.m_iIndex = size();
		return it;
	}
	UINT size()
	{
		return m_iSize;
	}
	T* operator [](UINT iPos) const
	{
		return m_pVector[iPos];
	}
	void push(T* elem)
	{
		_size(m_iSize+1);
		m_pVector[m_iSize-1] = elem;
		if( elem ) elem->AddRef();
	}
	void push(const afc_object<T> &p)
	{        ;
	push( &p );
	}
	afc_object<T> pop()
	{
		if( m_iSize < 1 ) return afc_object<T>();
		T* p = m_pVector[m_iSize-1];
		afc_object<T> r(p);
		p->Release();
		m_iSize --;
		return r;
	}
	void insert(T* p,UINT iPos)
	{
		if( iPos < 0 || iPos >= m_iSize ) return;
		_size(m_iSize +1);
		for(UINT i=m_iSize -1;i>iPos;i--)
			m_pVector[i] = m_pVector[i-1];
		m_pVector[iPos] = p;
		if( p ) p->AddRef();
	}
	void insert(afc_object<T>& p,UINT iPos)
	{
		insert(&p,iPos);
	}
	void erase(UINT iPos)
	{
		if( iPos >= m_iSize ) return;
		if( m_pVector[iPos] )
			m_pVector[iPos]->Release();
		for(UINT i=iPos;i<m_iSize-1;i++)
		{
			m_pVector[i] = m_pVector[i+1];
		}
		m_pVector[m_iSize-1] = (T *)NULL;
		m_iSize --;
	}
	void erase(T* p)
	{
		for(UINT i=0;i<m_iSize;i++)
		{
			T* px=m_pVector[i];
			if( px == p )
			{
				erase((UINT)i);
				break;
			}
		}
	}
	void erase(const afc_object<T>& p)
	{
		erase(&p);
	}
	void erase(iterator it)
	{
		erase(it.m_iIndex);
	}
	void clear()
	{
		_size(0);
	}
private:
	void _size(UINT iNew)
	{

		if( iNew <= __iSize )
		{
			if( __iSize > 0 && m_iSize > iNew )
			{
				for(unsigned int i=iNew;i<m_iSize;i++)
				{
					if( m_pVector[i] )
					{
						m_pVector[i]->Release();
					}
				}
			}
		}
		else
		{
			int iCount = iNew + (__iSize < 5 ? 5 : __iSize) / 3;
			T** p = new T*[iCount];
			if( p == NULL )
				throw AException(_T("内存不足"));
			//
			if( m_pVector )
			{
				memcpy(p,m_pVector,m_iSize * sizeof(T *));
				delete[] m_pVector;
			}
			m_pVector = p;
			__iSize = iCount;
		}
		m_iSize = iNew;
	}
private:
	UINT m_iSize;
	UINT __iSize;//true size
	T** m_pVector;
};


//afc_object<T>队列
template< typename T >
class afc_queue
{
public:
	afc_queue()
	{
		m_pStart = NULL;
		m_pEnd = NULL;
		m_iSize = 0;
	}
	~afc_queue()
	{
		clear();
	}

	void push(T* p)
	{
		_data* ptr = m_pEnd;
		m_pEnd = new _data;
		m_pEnd->pData = p;
		m_pEnd->pNext = NULL;
		if( p ) p->AddRef();
		if( ptr != NULL ) ptr->pNext = m_pEnd;
		m_iSize ++;
		if( m_pStart == NULL ) m_pStart = m_pEnd;
	}
	void push(const afc_object<T>& p)
	{
		push(&p);
	}
	afc_object<T> pop()
	{
		if( m_pStart == NULL ) return afc_object<T>();
		_data* ptr = m_pStart;
		m_pStart = m_pStart->pNext;
		if( m_pStart == NULL ) m_pEnd = NULL;
		T* pData = ptr->pData;
		delete ptr;
		m_iSize --;
		afc_object<T> r;
		r = pData;
		if( pData ) pData->Release();
		return r;
	}
	void clear()
	{
		while(size()) pop();
	}
	UINT size()
	{
		return m_iSize;
	}
private:
	class _data
	{
	public:
		_data* pNext;
		T* pData;

		_data()
		{
			pData = NULL;
			pNext = NULL;
		}
		~_data()
		{
		}
	};
	_data*   m_pStart;
	_data*   m_pEnd;
	UINT	m_iSize;
};

//afc_object<T>栈
template< typename T >
class afc_stack
{
public:
	afc_stack()
	{
		m_pStart = NULL;
		m_pEnd = NULL;
		m_iSize = 0;
	}
	~afc_stack()
	{
		clear();
	}

	void push(const afc_object<T>& p)
	{
		_data* ptr = m_pStart;
		m_pStart = new _data;
		m_pStart->pData = &p;
		p.AddRef();
		m_pStart->pNext = ptr;
		m_iSize ++;
	}
	afc_object<T> pop()
	{
		if( m_pStart == m_pEnd )
		{
			m_iSize = 0;
			return afc_object<T>();
		}
		_data* ptr = m_pStart;
		m_pStart = m_pStart->pNext;
		T* pData = ptr->pData;
		delete ptr;
		m_iSize --;
		afc_object<T> r;
		r = pData;
		if( pData ) pData->Release();

		return r;
	}
	void clear()
	{
		while( size() ) pop();
	}
	UINT size()
	{
		return m_iSize;
	}
private:
	class _data
	{
	public:
		_data* pNext;
		T* pData;

		_data()
		{
			pData = NULL;
			pNext = NULL;
		}
		~_data()
		{
		}
	};
	_data*   m_pStart;
	_data*   m_pEnd;
	UINT	m_iSize;
};


//afc_object<T>链表
template< typename T >
class afc_list
{
private:
	class _data
	{
	public:
		_data* pNext;
		_data* pPrev;
		T* pData;

		_data()
		{
			pData = NULL;
			pNext = NULL;
			pPrev = NULL;
		}
		~_data()
		{
		}
	};
public:
	class iterator
	{
	public:
		_data* pCur;
		iterator() : pCur(NULL)
		{
		}
		iterator(const iterator& it)
		{
			pCur = it.pCur;
		}
		iterator& operator =(const iterator& it)
		{
			pCur = it.pCur;
			return *this;
		}
		~iterator()
		{
		}
		T* operator *()
		{
			return pCur->pData;
		}
		T* operator ->()
		{
			return pCur->pData;
		}
		T* get()
		{
			return pCur->pData;
		}
		void operator ++()
		{
			if( pCur )
				pCur = pCur->pNext;
		}
		void operator ++(int)
		{
			if( pCur )
				pCur = pCur->pNext;
		}
		void operator --()
		{
			if( pCur )
				pCur = pCur->pPrev;
		}
		void operator --(int)
		{
			if( pCur )
				pCur = pCur->pPrev;
		}
		bool operator ==(const iterator& it)
		{
			return (pCur == it.pCur ? true : false);
		}
		bool operator !=(const iterator& it)
		{
			return (pCur != it.pCur ? true : false);
		}
	};
	//--------------------- afc_list enum 未完，待续~~~
	class afc_list_enum
	{
	public:
		_data* pCur;
		afc_list_enum():pCur(NULL),m_pOwner(NULL)
		{
		}
		~afc_list_enum()
		{
		}
		void Perpare()//准备枚举（如果上一次枚举没有End，则自动终止上一次再重启新的枚举）
		{
		}
		bool Next()//进行枚举
		{
			return false;
		}
		void* Cur()//得到当前枚举值
		{
			return NULL;
		}
		int Index()//得到当前枚举所对应的下标 ( +1 后即为当前的总数 ）
		{
		}
		void End()//停止枚举（非强制调用）
		{
		}
		afc_list<T>*	m_pOwner;
	};
	afc_list()
	{
		m_pStart = NULL;
		m_pEnd = NULL;
		m_iSize = 0;
	}
	~afc_list()
	{
		clear();
	}

	void push(const afc_object<T>& p)
	{
		_data* ptr = new _data;
		ptr->pData = &p;
		if( ptr->pData ) ptr->pData->AddRef();

		if( m_pStart == m_pEnd )
		{
			if( m_pStart == NULL )
			{
				m_pStart = ptr;
				m_pEnd = ptr;
			}
			else
			{
				m_pStart->pNext = ptr;
				m_pEnd = ptr;
				m_pEnd->pPrev = m_pStart;
			}
		}
		else
		{
			m_pEnd->pNext = ptr;
			ptr->pPrev = m_pEnd;
			m_pEnd = ptr;
		}
		m_iSize ++;
	}
	void pop_front(afc_object<T>& pOut)
	{
		if( m_pStart == m_pEnd )
		{
			if( m_pStart == NULL )
			{
				return;
			}
			else
			{
				_data* ptr = m_pStart;
				m_pStart = NULL;
				m_pEnd = NULL;
				pOut = ptr->pData;
				if( ptr->pData ) ptr->pData->Release();

				delete ptr;
				m_iSize --;
			}
		}
		else
		{
			_data* ptr = m_pStart;
			m_pStart = m_pStart->pNext;
			m_pStart->pPrev = NULL;
			pOut = ptr->pData;
			if( ptr->pData ) ptr->pData->Release();
			delete ptr;
			m_iSize --;
		}
	}
	void pop_back(afc_object<T>& pOut)
	{
		if( m_pStart == m_pEnd )
		{
			if( m_pStart == NULL )
			{
				return ;
			}
			else
			{
				_data* ptr = m_pEnd;
				m_pStart = NULL;
				m_pEnd = NULL;
				pOut = ptr->pData;
				if( ptr->pData ) ptr->pData->Release();
				delete ptr;
				m_iSize --;
			}
		}
		else
		{
			_data* ptr = m_pEnd;
			m_pEnd = m_pEnd->pPrev;
			m_pEnd->pNext = NULL;
			pOut = ptr->pData;
			if( ptr->pData ) ptr->pData->Release();
			delete ptr;
			m_iSize --;
		}
	}
	void clear()
	{
		while(size() )
		{
			afc_object<T> xx;
			pop_back(xx);
		}
	}
	UINT size()
	{
		return m_iSize;
	}
	iterator begin()
	{
		iterator it;
		it.pCur = m_pStart;
		return it;
	}
	iterator last()
	{
		iterator it;
		it.pCur = m_pEnd;
		return it;
	}
	iterator end()
	{
		iterator it;
		it.pCur = NULL;
		return it;
	}

	iterator erase(iterator& it)
	{
		iterator itRet;

		if( it.pCur->pData ) it.pCur->pData->Release();

		if( m_pStart == m_pEnd )
		{
			if( m_pStart == NULL ) ;//do nothing
			else
			{
				if( it.pCur == m_pStart )
				{
					_data* ptr = m_pStart;
					m_pStart = NULL;
					m_pEnd = NULL;
					m_iSize --;
					delete ptr;
				}
			}
		}
		else
		{
			if( it.pCur == m_pStart )
			{
				if( it.pCur->pNext ) it.pCur->pNext->pPrev = NULL;
				m_pStart = it.pCur->pNext;
				itRet.pCur = m_pStart;
				m_iSize --;
				delete it.pCur;
			}
			else if( it.pCur == m_pEnd )
			{
				if( it.pCur->pPrev ) it.pCur->pPrev->pNext = NULL;
				m_pEnd = it.pCur->pPrev;
				itRet.pCur = NULL;
				m_iSize --;
				delete it.pCur;
			}
			else
			{
				_data* pNext = NULL;
				_data* pPrev = NULL;
				pNext = it.pCur->pNext;
				pPrev = it.pCur->pPrev;
				if( pPrev ) pPrev->pNext = pNext;
				if( pNext ) pNext->pPrev = pPrev;
				m_iSize --;
				delete it.pCur;
				itRet.pCur = pNext;
			}
		}
		return itRet;
	}
	void erase(const afc_object<T>& p)
	{
		for(iterator it=begin();it!=end();it++)
		{
			if( p == *it )
			{
				erase(it);
				break;
			}
		}
	}
	void insert(const iterator& it,afc_object<T>& ptr)
	{
		if( m_pStart == m_pEnd && m_pStart == NULL )
		{
			push( ptr );
		}
		else
		{
			_data* p = new _data;
			p->pData = &ptr;
			ptr.AddRef();
			if( it.pCur == m_pStart )
			{
				p->pNext = m_pStart;
				m_pStart->pPrev = p;
				m_pStart = p;
			}
			else if( it.pCur == m_pEnd )
			{
				m_pEnd->pPrev->pNext = p;
				p->pPrev = m_pEnd->pPrev;
				p->pNext = m_pEnd;
				m_pEnd->pPrev = p;
			}
			else
			{
				p->pPrev = it.pCur->pPrev;
				p->pNext = it.pCur;
				it.pCur->pPrev->pNext = p;
				it.pCur->pPrev = p;
			}
			m_iSize ++;
		}
	}
	afc_list_enum* GetEnumerator()
	{
		m_enum.m_pOwner = this;
		return &m_enum;
	}
private:
	_data*   m_pStart;
	_data*   m_pEnd;
	UINT	m_iSize;
	afc_list_enum	m_enum;
};

//X智能指针
template< typename T >
class afc_ptr
{
private:
	class implement  // 实现类，引用计数
	{
	public:
		implement(T* pp):p(pp),refs(1)
		{
		}
		~implement()
		{
			if( p )
			{
				try{
					delete p;
				}catch(...){}
			}
		}

		T* p; // 实际指针
		LONG refs; // 引用计数
	};
	implement* _impl;
public:
	explicit afc_ptr(T* p): _impl(new implement(p))
	{
	}
	explicit afc_ptr() : _impl(new implement(NULL))
	{
	}
	~afc_ptr()
	{
		decrease();  // 计数递减
	}
	afc_ptr(const afc_ptr& rhs):_impl(rhs._impl)
	{
		increase();  // 计数递增
	}

	afc_ptr& operator=(const afc_ptr& rhs)
	{
		if(_impl != rhs._impl)  // 避免自赋值
		{
			decrease();  // 计数递减，不再共享原对象
			_impl=rhs._impl;  // 共享新的对象
			increase();  // 计数递增，维护正确的引用计数值
		}
		return *this;
	}
	afc_ptr& operator=( T* rhs)
	{
		if(_impl->p != rhs)  // 避免自赋值
		{
			decrease();  // 计数递减，不再共享原对象
			_impl=new implement(rhs);  // 共享新的对象
			//上面一行已经增加过了，increase();  // 计数递增，维护正确的引用计数值
		}
		return *this;
	}
	bool operator ==(const T* p)
	{
		return (_impl->p == p ? true : false);
	}
	bool operator !=(const T* p)
	{
		return (_impl->p != p ? true : false);
	}
	T* get(){ return _impl->p; }
	T* operator->() const
	{
		return _impl->p;
	}
	T* operator &() const
	{
		return _impl->p;
	}

	T& operator*() const
	{
		return *(_impl->p);
	}

private:
	void decrease()
	{
		::InterlockedDecrement(&(_impl->refs));
		if(_impl->refs==0)
		{  // 不再被共享，销毁对象
			delete _impl;
		}
	}

	void increase()
	{
		::InterlockedIncrement(&(_impl->refs));
	}
};

#endif//ATEMPL_H