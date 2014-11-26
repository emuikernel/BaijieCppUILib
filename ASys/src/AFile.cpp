#include "stdafx.h"
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

AFile::AFile():base_class()
{
	m_hFile = INVALID_HANDLE_VALUE;
}

AFile::~AFile()
{
	Close();
}
HANDLE AFile::GetHandle()
{
	return m_hFile;
}
bool AFile::Create(const ACHAR* sFileName,bool bCreateAlways /*= true*/)
{
	m_hFile = ::CreateFile(
		sFileName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ| FILE_SHARE_WRITE,
		NULL,
		bCreateAlways ? CREATE_ALWAYS : CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if( m_hFile != INVALID_HANDLE_VALUE )
		return true;
	else
		return false;
}
bool AFile::Open(const ACHAR* sFileName,afc_FileMode fm /*=fmReadWrite*/)
{
	m_hFile = ::CreateFile(
		sFileName,
		fm == afcRead  ? GENERIC_READ : ( fm == afcWrite ? GENERIC_WRITE : (GENERIC_READ |GENERIC_WRITE)),
		FILE_SHARE_READ| FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if( m_hFile != INVALID_HANDLE_VALUE )
		return true;
	else
		return false;
}
int AFile::GetPos()
{
	if( IsOpened() )
		return ::SetFilePointer(m_hFile,0,NULL,FILE_CURRENT);
	else
		return 0;
}
int AFile::Seek(int iPos)
{
	if( IsOpened() )
		return ::SetFilePointer(m_hFile,iPos,NULL,FILE_BEGIN);
	else
		return 0;
}
int AFile::SeekToBegin()
{
	if( IsOpened() )
		return ::SetFilePointer(m_hFile,0,NULL,FILE_BEGIN);
	else
		return 0;
}
int AFile::SeekToEnd()
{
	if( IsOpened() )
		return ::SetFilePointer(m_hFile,0,NULL,FILE_END);
	else
		return 0;
}
int AFile::Length()
{
	if( IsOpened() )
	{
		int iPos = GetPos();
		int iLen = SeekToEnd();
		Seek(iPos);
		return iLen;
	}
	else
		return 0;
}
int AFile::Read(BYTE* pData,int iLen)
{
	if( IsOpened() )
	{
		DWORD dwRead=0;
		if( ::ReadFile(m_hFile,pData,iLen,&dwRead,NULL) )
			return dwRead;
		else
			return 0;
	}
	else
		return 0;
}
int AFile::Write(BYTE* pData,int iLen)
{
	if( IsOpened() )
	{
		DWORD dw=0;
		if( ::WriteFile(m_hFile,pData,iLen,&dw,NULL) )
			return dw;
		else
			return 0;
	}
	else
		return 0;
}
bool AFile::IsOpened()
{
	return (m_hFile != INVALID_HANDLE_VALUE && m_hFile != NULL) ? true : false;
}
void AFile::Close()
{
	if( IsOpened() )
	{
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

bool AFile::IsExists(const AString& sFileName)
{
	if( (_taccess( sFileName.Text(), 0 )) != -1 ) return true;
	else return false;
}

AString AFile::ExtractFileName(const AString& sPath)
{
	AString s;
	s = sPath;
	int iPos = s.ReverseFind(_T("\\"));
	if( iPos > -1 ) s = s.Right(s.Length()-iPos-1);
	iPos = s.ReverseFind(_T("."));
	if( iPos > -1 ) s = s.Left(iPos);
	return s;
}

AString AFile::ExtractFilePath(const AString& sPath)
{
	AString s;
	s = sPath;
	int iPos = s.ReverseFind(_T("\\"));
	if( iPos > -1 ) s = s.Left(iPos);
	return s;
}

AString AFile::File2String(const AString& sFileName,bool bUnicode /*=false*/)
{
	AFile f;
	AString sRet;
	if( f.Open(sFileName,afcRead) )
	{
		AMemory ms;
		ms.SetSize( f.Length() );
		f.Read( (BYTE *)ms.GetData(),ms.GetSize() );
		if( bUnicode ) sRet = AString( (WCHAR *)ms.GetData(),ms.GetSize()/2);
		else sRet = AString( (char *)ms.GetData(),ms.GetSize());
	}
	return sRet;
}
void AFile::String2File(const AString& str,const AString& sFile,bool bUnicode /*=false*/)
{
	AFile f;
	if( f.Create(sFile) )
	{
		if( str.Length() )
		{
			if( bUnicode )
			{
				wchar_t* sz = (wchar_t *)str.Text();
				f.Write( (BYTE *)sz,str.Length() * 2 );
			}
			else
			{
				AString s;
				s = str;
				char* sz = (char *)s.ConvertTo();
				f.Write((BYTE *)sz,strlen(sz));
			}
		}
	}
}