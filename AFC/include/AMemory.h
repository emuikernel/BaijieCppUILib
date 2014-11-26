#ifndef AMEMORY_H
#define AMEMORY_H

class AFC_API AMemory : public AObject
{
	CLASS_INFO(Memory,Object);
public:
	AMemory();
	AMemory(const AMemory& ms);
	virtual ~AMemory();

	void	SetSize(UINT nLen);
	UINT	GetSize() const;
	BYTE*	GetData() const;
	void SetData(BYTE *p,int iLen);

	AMemory& Add(BYTE* pBuf,int iLen);
	AMemory& operator=(const AMemory& ms);
protected:
	BYTE*		m_pData;
	UINT		m_nLen;
};

#endif//AMEMORY_H