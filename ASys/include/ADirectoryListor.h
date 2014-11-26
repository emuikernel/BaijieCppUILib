#ifndef ADIRECTORYLISTOR_H
#define ADIRECTORYLISTOR_H

/*
 * 目录遍历器
 *	根据给定的目录名，列出下面的文件或目录
 *		注意：不包括子目录，不嵌套递归
 */

class ASYS_API ADirectoryEnumerator
{
public:
	ADirectoryEnumerator(const AString& sDirectory);
	~ADirectoryEnumerator();

	//列举参数
	void ListDirectory(bool v);
	void ListFile(bool v);
	void ListReadOnly(bool v);
	void ListHidden(bool v);
	void ListExtend(const AString& sExt);

	bool Next();
	AString Cur();
	AString CurWithPath();
	int Index();
	void End();
	bool IsDirectory();
	bool IsFile();
	bool IsHidden();
	bool IsReadOnly();
	ADateTime GetModifyTime();
	ADateTime GetCreateTime();
	INT64 GetSize();
	AString GetSizeString();
private:
	//=== 列举时的参数
	AString		m_sDir;
	bool		m_bDirectory;//是否列举目录		（缺省：是）
	bool		m_bFile;//是否列举文件			（缺省：是）
	bool		m_bReadOnly;//是否列举只读文件		（缺省：是）
	bool		m_bHidden;//是否列举隐藏文件或文件夹	（缺省：否）
	AString		m_sExt;

	//=== 列举过程中的结果
	AString			m_sFile;
	bool			m_bDir;
	bool			m_bFileHidden;
	bool			m_bFileReadOnly;
	INT64			m_nSize;
	ADateTime		m_tModify;
	ADateTime		m_tCreate;
	int			m_iIndex;
	bool			m_bFirst;

	bool			m_bEnd;

	HANDLE			m_hFind;
	WIN32_FIND_DATA		m_FindData;

	int _next();
};

#endif//ADIRECTORYLISTOR_H