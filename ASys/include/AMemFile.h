#ifndef AMEMFILE_H
#define AMEMFILE_H

class ASYS_API AMemFile : public AFile
{
	CLASS_INFO(MemFile,File);
public:
	AMemFile();
	virtual ~AMemFile();

	virtual HANDLE GetHandle();
	virtual bool Create(const ACHAR* sFileName,bool bCreateAlways = true);
	virtual bool Open(const ACHAR* sFileName,afc_FileMode fm=afcReadWrite);
	virtual int Seek(int iPos);
	virtual int SeekToBegin();
	virtual int SeekToEnd();
	virtual int Length();
	virtual int Read(BYTE* pData,int iLen);
	virtual int Write(BYTE* pData,int iLen);
	virtual bool IsOpened();
	virtual void Close();
	virtual int GetPos();

	virtual void SetStep(int nStep);
	virtual BYTE* GetData();
protected:
	HANDLE	m_hFile;
	BYTE*	m_pData;
	int	m_nStep;
	int	m_nLen;
	int	m_nSize;
	int	m_nPos;
};

#endif//AMEMFILE_H