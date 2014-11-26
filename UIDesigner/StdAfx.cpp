// stdafx.cpp : source file that includes just the standard includes
//	App.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


AString NewGUID()
{
	GUID   guid; 
	AString   szGUID; 
	if(S_OK   ==   ::CoCreateGuid(&guid)) 
	{ 
		szGUID.Format( _T("{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X} ") 
			,   guid.Data1 
			,   guid.Data2 
			,   guid.Data3 
			,   guid.Data4[0],   guid.Data4[1] 
		,   guid.Data4[2],   guid.Data4[3],   guid.Data4[4],   guid.Data4[5] 
		,   guid.Data4[6],   guid.Data4[7] 
		); 
	} 
	return szGUID;
}

AColor Int2Color(int c)//abgr¸ñÊ½
{
	return AColor( (c >> 24) & 0xff,c & 0xff, (c >> 8 ) & 0xff, (c >> 16) & 0xff);
}
int Color2Int(AColor cr)
{
	unsigned int x=0;
	x += cr.red;
	x += (cr.green << 8) & 0xff00;
	x +=  (cr.blue << 16) & 0xff0000;
	x += (cr.alpha << 24) & 0xff000000;
	return x;
	//int c =(cr.alpha << 24) & 0xff000000 + (cr.blue << 16) & 0xff0000 + (cr.green << 8) & 0xff00 + cr.red;
	//return (cr.alpha << 24) & 0xff000000 + (cr.blue << 16) & 0xff0000 + (cr.green << 8) & 0xff00 + cr.red;
}

afc_object<AMLBody> LoadNodeFromFile(const EString& sFile)
{
	AFile f;
	if( f.Open(sFile,afcRead) )
	{
		xmemory ms;
		ms.SetSize(f.Length() );
		f.Read(ms.GetData(),ms.GetSize());
		EString s((char *)ms.GetData(),ms.GetSize());
		IConfig fg;
		fg.LoadText(s);
		CONFIG_BODY p;
		p = fg.GetRootBody();
		return p;
	}
	return afc_object<AMLBody>();
}

EString GetExt(const EString& sFileName)
{
	EString str;
	int iPos = sFileName.ReverseFind(L".");
	if( iPos > -1 ) str = sFileName.Right(sFileName.Length()-iPos-1);
	return str;
}

EString GetCurPath()
{
	ACHAR szBuf[1024];
	::GetModuleFileName(NULL,szBuf,1024);
	EString str;
	str = szBuf;
	int iPos = str.ReverseFind(_ES("\\"));
	str = str.Left(iPos+1);
	return str;
}

EString GetRelPath(const EString& sProjectPath,const EString& sFile)
{
	EString s1=sProjectPath;
	EString s2=sFile;
	if( s1.Right(1) != L"\\" ) s1 += L"\\";
	s1.ToUpper();
	s2.ToUpper();
	int iPos=s2.Find(s1);
	EString str;
	if( iPos < 0 )
	{
		str = sFile;
	}
	else
	{
		str = sFile.Right(sFile.Length()-iPos-s1.Length());
	}
	return str;
}

EString GetNamespacePath(const EString& sProjectPath,const EString& sFile)
{
	EString str = GetRelPath( sProjectPath,sFile);
	int iPos = str.ReverseFind(L".");
	if( iPos > -1 ) str = str.Left(iPos);
	str.Replace(L"\\",L".");
	return str;
}
EString AddPath(const EString& sPath,const EString& s)
{
	EString str;
	str = sPath;
	if( str.Right(1) != L"\\" ) str += L"\\";
	str += s;
	return str;
}