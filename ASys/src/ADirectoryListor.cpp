#include "stdafx.h"

//====================== ADirectoryEnumerator ================
ADirectoryEnumerator::ADirectoryEnumerator(const AString& sDirectory)
{
	m_sDir = sDirectory;
	m_hFind = INVALID_HANDLE_VALUE;
	m_bDir = false;
	m_bFile = true;
	m_bHidden = false;
	m_bReadOnly = false;
	m_nSize = 0;
	m_iIndex = 0;
	m_sExt = _T("*");
	m_bFirst = true;
	m_bEnd = false;
}

ADirectoryEnumerator::~ADirectoryEnumerator()
{
	End();
}

void ADirectoryEnumerator::ListDirectory(bool v)
{
	m_bDirectory = v;
}
void ADirectoryEnumerator::ListFile(bool v)
{
	m_bFile = v;
}
void ADirectoryEnumerator::ListReadOnly(bool v)
{
	m_bReadOnly = v;
}
void ADirectoryEnumerator::ListHidden(bool v)
{
	m_bHidden = v;
}

void ADirectoryEnumerator::ListExtend(const AString& sExt)
{
	m_sExt = sExt;
}

bool ADirectoryEnumerator::Next()
{
	if( m_bEnd ) return false;
	//准备遍历
	if( m_bFirst )
	{
		AString s;
		s = m_sDir;
		if( s.Right(1) != _T("\\") ) s += _T("\\");
		s += _T("*.");
		s += m_sExt;
		m_hFind = ::FindFirstFile(s.Text(),&m_FindData);
		m_iIndex = -1;
		
	}
	

	if( m_hFind == INVALID_HANDLE_VALUE ) return false;
	
	for(;;)
	{
		int iNext = _next();
		if( iNext == 1 )
		{
			m_bEnd = true;
			End();
			return false;
		}
		if( iNext == 0 ) 
		{
			m_iIndex ++;
			break;
		}
	}
	return true;
}
//0:找到符合条件的下一个
//1:没有下一个了
//2:找到一个不符合条件的下一个
int ADirectoryEnumerator::_next()
{
	if( m_bFirst )
	{
		if( m_hFind == INVALID_HANDLE_VALUE ) return 1;
		m_bFirst = false;
	}
	else
	{
		if( FALSE == ::FindNextFile(m_hFind,&m_FindData) ) return 1;
	}
	AString sFile;
	sFile = m_FindData.cFileName;
	if( sFile == _T(".") || sFile == _T("..") ) return 2;

	bool bDir = ((m_FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY ? true : false);
	if( bDir )
	{
		if( m_bDirectory == false ) return 2;
	}
	else
	{
		if( m_bFile == false ) return 2;
	}
	bool bReadOnly = ((m_FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY ? true : false);
	if( m_bReadOnly == false && bReadOnly ) return 2;

	bool bHidden = ((m_FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN ? true : false);
	if( m_bHidden == false && bHidden ) return 2;

	//全部条件都满足
	m_sFile = sFile;
	m_bDir = bDir;
	m_bFileHidden = bHidden;
	m_bFileReadOnly = bReadOnly;
	m_nSize =  ( (INT64)m_FindData.nFileSizeHigh << 32) + m_FindData.nFileSizeLow;

	FILETIME ft = m_FindData.ftLastWriteTime;
	SYSTEMTIME st;
	::FileTimeToSystemTime(&ft,&st);
	m_tModify = ADateTime(st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

	ft = m_FindData.ftCreationTime;
	::FileTimeToSystemTime(&ft,&st);
	m_tCreate = ADateTime(st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	m_iIndex++;
	return 0;
}
AString ADirectoryEnumerator::Cur()
{
	return m_sFile;
}
AString ADirectoryEnumerator::CurWithPath()
{
	AString s; 
	s = m_sDir;
	if( s.Right(1) != _T("\\") ) s += _T("\\");
	s += m_sFile;
	return s;
}
int ADirectoryEnumerator::Index()
{
	return m_iIndex;
}
void ADirectoryEnumerator::End()
{
	if( m_hFind != INVALID_HANDLE_VALUE )
	{
		::FindClose(m_hFind);
		m_hFind = INVALID_HANDLE_VALUE;
	}
}
bool ADirectoryEnumerator::IsDirectory()
{
	return m_bDir;
}
bool ADirectoryEnumerator::IsFile()
{
	return !m_bDir;
}
bool ADirectoryEnumerator::IsHidden()
{
	return m_bHidden;
}
bool ADirectoryEnumerator::IsReadOnly()
{
	return m_bReadOnly;
}
ADateTime ADirectoryEnumerator::GetModifyTime()
{
	return m_tModify;
}
ADateTime ADirectoryEnumerator::GetCreateTime()
{
	return m_tCreate;
}
INT64 ADirectoryEnumerator::GetSize()
{
	return m_nSize;
}
AString ADirectoryEnumerator::GetSizeString()
{
	AString sSize;
	INT64 nSize = m_nSize;
	if( nSize > 1024 * 1024 * 1024 )
	{
		int n = nSize / (1024 * 1024 * 1024);
		sSize.Format(_T("%d.%02d G"),n,(int)( (nSize-n)*100.0/(double)(1024 * 1024 * 1024)));
	}
	else if( nSize > 1024 * 1024 )
	{
		int n = nSize / (1024 * 1024);
		sSize.Format(_T("%d.%02d M"),n,(nSize-n)*100/(1024*1024));
	}
	else if(nSize > 1024)
	{
		sSize.Format(_T("%d.%02d K"),nSize/1024,(nSize-nSize/1024)*100/1024);
	}
	else
		sSize.Format(_T("%d B"),nSize);
	return sSize;
}