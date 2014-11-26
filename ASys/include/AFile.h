#ifndef AFILE_H
#define AFILE_H

enum afc_FileMode
{
	afcReadWrite=0,
	afcRead=1,
	afcWrite=2
};


class ASYS_API AFile : public AObject
{
	CLASS_INFO(File,Object);
public:
	AFile();
	virtual ~AFile();

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

	static bool IsExists(const AString& sFileName);
	static AString ExtractFileName(const AString& sPath);
	static AString ExtractFilePath(const AString& sPath);
	static AString File2String(const AString& sFileName,bool bUnicode=false);
	static void String2File(const AString& str,const AString& sFile,bool bUnicode=false);
protected:
	HANDLE	m_hFile;
};



#endif//AFILE_H