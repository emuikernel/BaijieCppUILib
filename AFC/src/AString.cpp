
#include "stdafx.h"
#include "AString.h"

const ACHAR* g_szEmpty=_T("");



AString::AString()
{
	m_pData = NULL;
	m_pConvData = NULL;
}
AString::AString(const AString& s)
{
	m_pData = NULL;
	m_pConvData = NULL;
	if( s.m_pData )
	{
		m_pData = s.m_pData;
		m_pData->AddRef();
	}
}
AString::AString(ACHAR ch)
{
	m_pConvData = NULL;
	m_pData = new _StrData(2);
	ACHAR*& sz = m_pData->GetText();
	sz[0] = ch;
	sz[1] = 0;
}

AString::AString(const ACHAR* s1)
{
	m_pConvData = NULL;
	m_pData = NULL;
	if( s1 && s1[0] )
	{
		int iLen = _tcslen(s1);
		m_pData = new _StrData(iLen+1);
		_tcscpy(m_pData->GetText(),s1);
	}
}
AString::AString(const UACHAR* s1)
{
	m_pConvData = NULL;
	m_pData = NULL;
	this->ConvertFrom(s1);
}

AString::AString(const ACHAR* s1,const ACHAR* s2)
{
	m_pConvData = NULL;
	m_pData = NULL;
	int iLen1,iLen2;
	if( s1 == NULL ) iLen1 = 0;
	else iLen1 = _tcslen(s1);
	if( s2 == NULL ) iLen2 = 0;
	else iLen2 = _tcslen(s2);
	if( iLen1 + iLen2 > 0 )
	{
		m_pData = new _StrData(iLen1+iLen2+1);
		if( s1 != NULL ) _tcscpy(m_pData->GetText(),s1);
		if( s2 != NULL ) _tcscat(m_pData->GetText(),s2);
	}
}
AString::AString(const ACHAR* s1,int nCount)
{
	m_pData = NULL;
	m_pConvData = NULL;
	if( s1 != NULL && s1[0] && nCount > 0 )
	{
		int iLen = _tcslen(s1);
		if( nCount > iLen )
			nCount = iLen;

		m_pData = new _StrData(nCount+1);
		_tcsncpy(m_pData->GetText(),s1,nCount);
		m_pData->GetText()[nCount] = 0;
	}
}
AString::AString(const UACHAR* s1,int nCount)
{
	m_pData = NULL;
	m_pConvData = NULL;
	if( s1 != NULL && nCount > 0 )
	{
		//ACHAR* ptr = NULL;
#ifdef _UNICODE
		int iSize = ::MultiByteToWideChar(CP_ACP,0,s1,nCount,NULL,0)+1;
		m_pData = new _StrData(iSize);
		::MultiByteToWideChar(CP_ACP,0,s1,nCount,m_pData->GetText(),iSize);
		m_pData->GetText()[iSize-1] = 0;
#else
		BOOL b = FALSE;
		int iSize = ::WideCharToMultiByte(CP_ACP,0,s1,nCount,NULL,0,"?",&b)+1;
		m_pData = new _StrData(iSize);
		::WideCharToMultiByte(CP_ACP,0,s1,nCount,m_pData->GetText(),iSize,"?",&b);
		m_pData->GetText()[iSize-1] = 0;
#endif
	}
}
AString::AString(ACHAR ch,int nCount)
{
	m_pConvData = NULL;
	m_pData = new _StrData(nCount+1);
	ACHAR* sz = m_pData->GetText();
	for(int i=0;i<nCount;i++)
	{
		sz[i] = ch;
	}
	sz[nCount] = 0;
}
AString::~AString()
{
	if( m_pData ) m_pData->Release();
	if( m_pConvData ) delete[] m_pConvData;
}

int AString::Length() const
{
	if( m_pData ) return _tcslen(m_pData->GetText());
	else return 0;
}
const ACHAR* AString::Text() const
{
	if( m_pData ) return m_pData->GetText();
	else return g_szEmpty;
}
AString::operator const ACHAR* () const
{
	return Text();
}
bool AString::IsEmpty() const
{
	if( m_pData && m_pData->GetText() && m_pData->GetText()[0] ) return false;
	else return true;
}
ACHAR& AString::operator[](int i)
{
	return m_pData->GetText()[i];
}
const ACHAR& AString::operator[](int i) const
{
	return m_pData->GetText()[i];
}

AString& AString::operator =(const AString& s)
{
	if( m_pData == s.m_pData ) return *this;

	if( m_pData ) m_pData->Release();
	m_pData = s.m_pData;
	if( m_pData ) m_pData->AddRef();
	return *this;
}
AString& AString::operator =(const ACHAR* s)
{
	if( m_pData )
	{
		m_pData->Release();
		m_pData = NULL;
	}

	if( s && s[0] )
	{
		int iLen = _tcslen(s);
		m_pData = new _StrData(iLen+1);
		_tcscpy( m_pData->GetText(),s);
	}
	return *this;
}
AString& AString::operator=(const UACHAR* s)
{
	if( s && s[0] )
		return this->ConvertFrom(s);
	else
	{
		if( m_pData )
		{
			m_pData->Release();
			m_pData = NULL;
		}
		return *this;
	}
}

inline bool _MyIsAlpha(ACHAR c)
{
	if( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ) return true;
	else return false;
}

AString& AString::operator+=(const AString& s)
{
	return operator += (s.Text());
}
AString& AString::operator+=(const ACHAR* s)
{
	if( s == NULL || s[0] == 0 ) return *this;

	int iMeLen = Length();
	if( iMeLen < 1 ) return this->operator=(s);

	int iLen = _tcslen(s);

	_StrData* ptr = new _StrData(iMeLen+iLen+1);
	if( iMeLen > 0 )
		_tcscpy(ptr->GetText(),this->Text());
	ptr->GetText()[iMeLen] = 0;
	_tcscat(ptr->GetText(),s);

	if( m_pData )
		m_pData->Release();
	m_pData = ptr;

	return *this;
}
AString& AString::operator +=(const UACHAR* s)
{
	AString sv;
	sv = s;
	return operator +=(sv);
}
AString& AString::operator+=(ACHAR c)
{
	return operator +=(AString(c));
}
AString& AString::operator +=(UACHAR c)
{
	UACHAR buf[2];
	buf[0] = c;
	buf[1] = 0;
	AString sv;
	sv = buf;
	return operator +=(sv);
}

AString& AString::ToLower()
{
	if( !IsEmpty() )
	{
		CopyOnWrite();

		int i=0;
		while( m_pData->GetText()[i] )
		{
			//if( islower(m_pData->szData[i]) == 0 && isalpha(m_pData->szData[i]) )
			//        m_pData->szData[i] = _tolower(m_pData->szData[i]);
			//i++;
			if( m_pData->GetText()[i] >= 'A' && m_pData->GetText()[i] <= 'Z' )//_MyIsAlpha(sz[i]) && islower(sz[i]) == 0  )
				m_pData->GetText()[i] = m_pData->GetText()[i]-'A'+'a';//_tolower(sz[i]);
			i++;
		}

	}
	return *this;
}
AString& AString::ToUpper()
{
	if( !IsEmpty() )
	{
		CopyOnWrite();
		int i=0;
		while( m_pData->GetText()[i] )
		{
			//if( islower(m_pData->szData[i]) != 0 && isalpha(m_pData->szData[i]) )
			//        m_pData->szData[i] = _toupper(m_pData->szData[i]);
			//i++;
			if( m_pData->GetText()[i] >= 'a' && m_pData->GetText()[i] <= 'z' )//_MyIsAlpha(sz[i]) && islower(sz[i]) != 0   )
				m_pData->GetText()[i] = m_pData->GetText()[i] - 'a' + 'A';//_toupper(sz[i]);
			i++;
		}

	}
	return *this;
}

AString& AString::Replace(const ACHAR* sOld,const ACHAR* sNew)
{
	if( sOld == NULL || sNew == NULL || IsEmpty() ) return *this;
	if( _tcscmp(sOld,sNew) == 0 ) return *this;//避免死循环

	int iOldLen = _tcslen(sOld);
	int iLength = Length();
	if( iOldLen < 1 || iLength < 1 ) return *this;
	int iNewLen = _tcslen(sNew);

	//查出出现次数
	int iFoundCount = 0;
	ACHAR* sFind = m_pData->GetText();
	while( true )
	{
		ACHAR* sPos = _tcsstr(sFind,sOld);
		if( sPos == NULL ) break;
		iFoundCount ++;
		sFind = sPos+iOldLen;
		if( sFind - m_pData->GetText() >= iLength ) break;
	}
	if( iFoundCount <= 0 ) return *this;

	CopyOnWrite();
	//计算空间
	ACHAR* sResult = NULL;//最终结果
	int iAllocLen = iLength + (iNewLen - iOldLen) * iFoundCount;
	if( iAllocLen > iLength )
	{
		_StrData* px = new _StrData( iAllocLen+1 );
		_tcscpy(px->GetText(),m_pData->GetText());
		if( m_pData ) m_pData->Release();
		m_pData =px;
	}
	sResult = m_pData->GetText();

	//进行替换
	sFind = sResult;
	int iCurLen = iLength;

	while( true )
	{
		ACHAR* sPos = _tcsstr(sFind,sOld);
		if( sPos == NULL ) break;

		memmove(sPos+iNewLen,sPos+iOldLen,sizeof(ACHAR) * (iCurLen-int(sPos-sResult)-iOldLen + 1) );//一定要加1，因为iCurLen不包括\0

		memcpy(sPos,sNew,iNewLen*sizeof(ACHAR));

		sFind = sPos + iNewLen;

		iCurLen += iNewLen - iOldLen;

		sResult[iCurLen] = 0;//因为有移动，因此可能变为不是sz字符串，所以要复位
	}
	return *this;
}
AString& AString::Trim()
{
	TrimLeft();
	TrimRight();
	return *this;
}
AString& AString::TrimLeft()
{
	if( IsEmpty() || Length() < 1 ) return *this;

	if( operator[](0) == _T(' ') )
	{

		CopyOnWrite();

		ACHAR* str = m_pData->GetText();
		if(str!=NULL)
		{
			register int s=0;
			register int x=0;
			while(isspace(str[s])) s++;
			while(str[s]) str[x++]=str[s++];
			str[x]=0;
		}
	}

	return *this;
}
AString& AString::TrimRight()
{
	if( IsEmpty() || Length() < 1 ) return *this;

	if( operator[](Length()-1) == _T(' ') )
	{

		CopyOnWrite();

		ACHAR* str = m_pData->GetText();
		if(str!=NULL)
		{
			register int e=Length();
			while(0<e && isspace(str[e-1])) e--;
			str[e]=0;
		}
	}
	return *this;
}
AString& AString::Clear()
{
	if( m_pData )
	{
		m_pData->Release();
		m_pData = NULL;
	}
	return *this;
}

AString AString::Left(int n) const
{
	if( IsEmpty() ) return AString();
	int iLen = Length();
	if( n > iLen ) n = iLen;
	return AString(Text(),n);
}
AString AString::Right(int n) const
{
	if( IsEmpty() ) return AString();
	int iLen = Length();
	if( n > iLen ) n = iLen;
	return AString(Text()+iLen-n,n);
}
AString AString::Mid(int pos,int nCount) const
{
	if( IsEmpty() ) return AString();
	int iLen = Length();
	if( pos > iLen ) pos = iLen;
	if( pos + nCount > iLen ) pos = iLen - nCount;
	return AString(Text()+pos,nCount);
}

int AString::Find(const ACHAR* s,int pos) const
{
	if( IsEmpty() || s == NULL ) return -1;
	if( pos >= Length() ) return -1;
	ACHAR* sFind = _tcsstr(m_pData->GetText()+pos,s);
	if( sFind == NULL ) return -1;
	return sFind - m_pData->GetText();
}
int AString::ReverseFind(const ACHAR* s,int pos) const
{
	if( IsEmpty() ) return -1;
	if( s == NULL ) return -1;
	int iLen = _tcslen(s);
	if( iLen == 0 ) return Length();
	if( pos > Length() ) pos = Length();
	while( pos >= 0 )
	{
		if( (*this)[pos] == s[0] )
		{
			if( ATextCompare(s,m_pData->GetText()+pos,iLen) == 0 )
				break;
		}
		pos --;
	}
	return pos;
}

AString& AString::Format(const ACHAR *fmt,...)
{
	va_list args;
	va_start(args,fmt);
	VFormat(fmt,args);
	va_end(args);
	return *this;

}
AString& AString::VFormat(const ACHAR *fmt,va_list args)
{
	//register int len=0;
	if(fmt && *fmt)
	{
		_StrData* px = new _StrData(1024);
		int iLen = _vstprintf(px->GetText(),fmt,args)+1;
		px->GetText()[iLen] = 0;
		if( m_pData ) m_pData->Release();
		m_pData = px;

	}
	return *this;
}

const UACHAR* AString::ConvertTo()
{
	if( m_pConvData )
	{
		delete[] m_pConvData;
		m_pConvData = NULL;
	}
#ifndef _UNICODE
	int iSize = ::MultiByteToWideChar(CP_ACP,0,this->Text(),this->Length(),NULL,0)+1;
	m_pConvData = new UACHAR[iSize];
	if( m_pConvData )
	{
		::MultiByteToWideChar(CP_ACP,0,this->Text(),this->Length(),m_pConvData,iSize);
		m_pConvData[iSize-1] = 0;
	}
#else
	BOOL b = FALSE;
	int iSize = ::WideCharToMultiByte(CP_ACP,0,this->Text(),this->Length(),NULL,0,"?",&b)+1;
	m_pConvData = new UACHAR[iSize];
	if( m_pConvData )
	{
		::WideCharToMultiByte(CP_ACP,0,this->Text(),this->Length(),m_pConvData,iSize,"?",&b);
		m_pConvData[iSize-1] = 0;
	}
#endif
	return m_pConvData;
}

AString& AString::ConvertFrom(const UACHAR* str)
{
	if( str == NULL )
	{
		if( m_pData )
		{
			m_pData->Release();
			m_pData = NULL;
		}
		return *this;
	}
	_StrData* px = NULL;
#ifdef _UNICODE
	int iStrLen = strlen(str);
	int iSize = ::MultiByteToWideChar(CP_ACP,0,str,iStrLen,NULL,0)+1;
	px = new _StrData(iSize);
	if( px )
	{
		::MultiByteToWideChar(CP_ACP,0,str,iStrLen,px->GetText(),iSize);
		px->GetText()[iSize-1] = 0;
	}
#else
	int iStrLen = wcslen(str);
	BOOL b = FALSE;
	int iSize = ::WideCharToMultiByte(CP_ACP,0,str,iStrLen,NULL,0,"?",&b)+1;
	px = new _StrData(iSize);
	if( px )
	{
		::WideCharToMultiByte(CP_ACP,0,str,iStrLen,px->GetText(),iSize,"?",&b);
		px->GetText()[iSize-1] = 0;
	}
#endif
	if( m_pData ) m_pData->Release();
	m_pData = px;
	return *this;
}

AString::_StrData::_StrData(int iSize)
{
	m_iRefCount = 1;
	if( iSize > 0 )
	{
		m_szData = new ACHAR[iSize];
		m_szData[0] = 0;
	}
	else
		m_szData = (ACHAR *)g_szEmpty;
}

AString::_StrData::~_StrData()
{
	Clear();
}
void AString::_StrData::AddRef()
{
#ifdef MULTITHREAD_REF
	::InterlockedIncrement(&m_iRefCount);
#else
	m_iRefCount++;
#endif
}
void AString::_StrData::Release()
{
	LONG tmp;
#ifdef MULTITHREAD_REF
	tmp = ::InterlockedDecrement(&m_iRefCount);
#else
	m_iRefCount--;
	tmp = m_iRefCount;
#endif
	if( tmp == 0 ) delete this;
}
LONG AString::_StrData::GetRefCount()
{
	return m_iRefCount;
}
AString::_StrData* AString::_StrData::Clone()
{
	_StrData* ptr = new _StrData();
	if( m_szData != g_szEmpty )
	{
		int iLen = _tcslen( m_szData );
		ptr->m_szData = new ACHAR[ iLen + 1];
		_tcscpy( ptr->m_szData,m_szData );
	}
	return ptr;
}
ACHAR*& AString::_StrData::GetText()
{ 
	return m_szData; 
}

void AString::_StrData::Clear()
{
	if( m_szData != g_szEmpty )
	{
		delete[] m_szData;
		m_szData = (ACHAR *)g_szEmpty;
	}
}

void AString::CopyOnWrite()
{
	if( m_pData && m_pData->GetRefCount() > 1 )
	{
		_StrData* ptr = m_pData;
		m_pData =m_pData->Clone();
		ptr->Release();
	}
}


 
int ATextCompare(const ACHAR *s1,const ACHAR *s2)
{
	if( s1 == NULL || s1[0] == 0 )
	{
		if( s2 == NULL || s2[0] == 0 ) return 0;
		else return -1;
	}
	else
	{
		if( s2 == NULL || s2[0] == 0 ) return 1;
        }
	register const ACHAR *p1=(const ACHAR *)s1;
	register const ACHAR *p2=(const ACHAR *)s2;
	register ACHAR c1,c2;
	do
	{
		c1=*p1++;
		c2=*p2++;
	}while(c1 && c2 && (c1==c2));

	return c1-c2;

}

int ATextCompare(const ACHAR *s1,const AString &s2)
{
	return ATextCompare(s1,s2.Text());
}

int ATextCompare(const AString &s1,const ACHAR *s2)
{
	return ATextCompare(s1.Text(),s2);
}

int ATextCompare(const AString &s1,const AString &s2)
{
	return ATextCompare(s1.Text(),s2.Text());
}

	/// Compare up to n

int ATextCompare(const ACHAR *s1,const ACHAR *s2,int n)
{
	if( s1 == NULL || s1[0] == 0 )
	{
		if( s2 == NULL || s2[0] == 0 ) return 0;
		else return -1;
	}
	else
	{
		if( s2 == NULL || s2[0] == 0 ) return 1;
	}
	register const ACHAR *p1=(const ACHAR *)s1;
	register const ACHAR *p2=(const ACHAR *)s2;
	register ACHAR c1,c2;
	if(0<n){
		do{
			c1=*p1++;
			c2=*p2++;
		}
		while(--n && c1 && (c1==c2));
		return c1-c2;
	}
	return 0;
}

int ATextCompare(const ACHAR *s1,const AString &s2,int n)
{
	return ATextCompare(s1,s2.Text(),n);
}

int ATextCompare(const AString &s1,const ACHAR *s2,int n)
{
	return ATextCompare(s1.Text(),s2,n);
}

int ATextCompare(const AString &s1,const AString &s2,int n)
{
	return ATextCompare(s1.Text(),s2.Text(),n);
}

	/// Compare case insensitive

int ATextCompareCase(const ACHAR *s1,const ACHAR *s2)
{
	if( s1 == NULL || s1[0] == 0 )
	{
		if( s2 == NULL || s2[0] == 0 ) return 0;
		else return -1;
	}
	else
	{
		if( s2 == NULL || s2[0] == 0 ) return 1;
	}
	register const ACHAR *p1=(const ACHAR *)s1;
	register const ACHAR *p2=(const ACHAR *)s2;
	register ACHAR c1,c2;
	do
	{
		c1=tolower(*p1++);
		c2=tolower(*p2++);
	}while(c1 && (c1==c2));

	return c1-c2;
}

int ATextCompareCase(const ACHAR *s1,const AString &s2)
{
	return ATextCompareCase(s1,s2.Text());
}

int ATextCompareCase(const AString &s1,const ACHAR *s2)
{
	return ATextCompareCase(s1.Text(),s2);
}

int ATextCompareCase(const AString &s1,const AString &s2)
{
	return ATextCompareCase(s1.Text(),s2.Text());
}

	/// Compare case insensitive up to n

int ATextCompareCase(const ACHAR *s1,const ACHAR *s2,int n)
{
	if( s1 == NULL || s1[0] == 0 )
	{
		if( s2 == NULL || s2[0] == 0 ) return 0;
		else return -1;
	}
	else
	{
		if( s2 == NULL || s2[0] == 0 ) return 1;
	}

	register const ACHAR *p1=(const ACHAR *)s1;
	register const ACHAR *p2=(const ACHAR *)s2;
	register ACHAR c1,c2;
	if(0<n){
		do{
			c1=tolower(*p1++);
			c2=tolower(*p2++);
		}
		while(--n && c1 && (c1==c2));
		return c1-c2;
	}
	return 0;
}

int ATextCompareCase(const ACHAR *s1,const AString &s2,int n)
{
	return ATextCompareCase(s1,s2.Text(),n);
}

int ATextCompareCase(const AString &s1,const ACHAR *s2,int n)
{
	return ATextCompareCase(s1.Text(),s2,n);
}

int ATextCompareCase(const AString &s1,const AString &s2,int n)
{
	return ATextCompareCase(s1.Text(),s2.Text(),n);
}

	/// Comparison operators

bool operator==(const AString &s1,const AString &s2)
{
	return (ATextCompare(s1,s2) == 0 ? true : false);
}

bool operator==(const AString &s1,const ACHAR *s2)
{
	return (ATextCompare(s1,s2) == 0 ? true : false);
}

bool operator==(const ACHAR *s1,const AString &s2)
{
	return (ATextCompare(s1,s2) == 0 ? true : false);
}


bool operator!=(const AString &s1,const AString &s2)
{
	return (ATextCompare(s1,s2) != 0 ? true : false);
}

bool operator!=(const AString &s1,const ACHAR *s2)
{
	return (ATextCompare(s1,s2) != 0 ? true : false);
}

bool operator!=(const ACHAR *s1,const AString &s2)
{
	return (ATextCompare(s1,s2) != 0 ? true : false);
}


bool operator<(const AString &s1,const AString &s2)
{
	return ATextCompare(s1,s2) <  0 ? true : false;
}

bool operator<(const AString &s1,const ACHAR *s2)
{
	return ATextCompare(s1,s2) <  0 ? true : false;
}

bool operator<(const ACHAR *s1,const AString &s2)
{
	return ATextCompare(s1,s2) <  0 ? true : false;
}


bool operator<=(const AString &s1,const AString &s2)
{
	return ATextCompare(s1,s2) <= 0 ? true : false;
}

bool operator<=(const AString &s1,const ACHAR *s2)
{
	return ATextCompare(s1,s2) <= 0 ? true : false;
}

bool operator<=(const ACHAR *s1,const AString &s2)
{
	return ATextCompare(s1,s2) <= 0 ? true : false;
}


bool operator>(const AString &s1,const AString &s2)
{
	return ATextCompare(s1,s2) > 0 ? true : false;
}

bool operator>(const AString &s1,const ACHAR *s2)
{
	return ATextCompare(s1,s2) > 0 ? true : false;
}

bool operator>(const ACHAR *s1,const AString &s2)
{
	return ATextCompare(s1,s2) > 0 ? true : false;
}


bool operator>=(const AString &s1,const AString &s2)
{
	return ATextCompare(s1,s2) >= 0 ? true : false;
}

bool operator>=(const AString &s1,const ACHAR *s2)
{
	return ATextCompare(s1,s2) >= 0 ? true : false;
}

bool operator>=(const ACHAR *s1,const AString &s2)
{
	return ATextCompare(s1,s2) >= 0 ? true : false;
}

	/// Concatenate two strings

AString operator+(const AString& s1,const AString& s2)
{
	return AString(s1.Text(),s2.Text());
}

AString operator *(const AString& s1,const AString& s2)
{
	return s1;
}

AString operator /(const AString& s1,const AString& s2)
{
	return s1;
}
AString operator+(const AString& s1,const ACHAR* s2)
{
	return AString(s1.Text(),s2);
}

AString operator+(const ACHAR* s1,const AString& s2)
{
	return AString(s1,s2.Text());
}

	// Concatenate with single character

AString operator+(const AString& s,ACHAR c)
{
	ACHAR s2[2];
	s2[0] = c;
	s2[1] = 0;
	return AString(s.Text(),s2);
}

AString operator+(ACHAR c,const AString& s)
{
	ACHAR s2[2];
	s2[0] = c;
	s2[1] = 0;
	return AString(s2,s.Text());
}

	/// Format a string a-la printf

AString AStringFormat(const ACHAR *fmt,...)
{
	AString result;
	va_list args;
	va_start(args,fmt);
	result.VFormat(fmt,args);
	va_end(args);
	return result;
}

AString AStringVFormat(const ACHAR *fmt,va_list args)
{
	AString result;
	result.VFormat(fmt,args);
	return result;
}

	/// Convert number to a string
AString AStringVal(__int64 num,int base)
{
	ACHAR buf[256];
	AString str;
        #ifdef _UNICODE
	str = _i64tow(num,buf,base);
        #else
	str = _i64toa(num,buf,base);
        #endif
	return str;
}
AString AStringVal(unsigned __int64 num,int base)
{
	ACHAR buf[256];
	AString str;
	#ifdef _UNICODE
	str = _ui64tow(num,buf,base);
        #else
	str = _ui64toa(num,buf,base);
        #endif
	return str;
}

AString AStringVal(int num,int base)
{
	const ACHAR g_szHEX[17]=
#ifdef _UNICODE
			L"0123456789ABCDEF"
#else
			"0123456789ABCDEF"
#endif
			;
	ACHAR buf[34];
	register ACHAR *p=buf+33;
	register UINT nn=(UINT)num;
	if( base < 2 ) base = 2;
	if( base > 16 ) base = 16;
	if(num<0){nn=(UINT)(~num)+1;}
	*p='\0';
	do{
		*--p=g_szHEX[nn%base];
		nn/=base;
	}
	while(nn);
	if(num<0) *--p='-';
	return AString(p,buf+33-p);
}

AString AStringVal(UINT num,int base)
{
	const ACHAR g_szHEX[17]=
#ifdef _UNICODE
			L"0123456789ABCDEF"
#else
			"0123456789ABCDEF"
#endif
	;
	ACHAR buf[34];
	register ACHAR *p=buf+33;
	register UINT nn=num;
	if( base < 2 ) base = 2;
	if( base > 16 ) base = 16;
	*p='\0';
	do{
		*--p=g_szHEX[nn%base];
		nn/=base;
	}
	while(nn);
	return AString(p,buf+33-p);
}

AString AStringVal(float num,int prec)
{
	return AStringFormat(
#ifdef _UNICODE
			L"%.*G"
#else
			"%.*G"
#endif
			,prec,num);
}

AString AStringVal(double num,int prec)
{
	return AStringFormat(
#ifdef _UNICODE
			L"%.*G"
#else
			"%.*G"
#endif
			,prec,num);
}

	/// Convert string to a number

int AIntVal(const AString& s,int base)
{
	return _tcstol(s,NULL,base);
}

__int64 AInt64Val(const AString& s,int base)
{
        #ifdef _UNICODE
        return _wtoi64(s);//,NULL,base);
        #else
	return _atoi64(s);//,NULL,base);
        #endif
}
unsigned __int64 AUInt64Val(const AString& s,int base)
{
	#ifdef _UNICODE
        return (unsigned __int64)_wtoi64(s);//,NULL,base);
        #else
	return (unsigned __int64)_atoi64(s);//,NULL,base);
        #endif
}

UINT AUINTVal(const AString& s,int base)
{
	return _tcstoul(s,NULL,base);
}

float AFloatVal(const AString& s)
{
	return (float)_tcstod(s,NULL);
}

double ADoubleVal(const AString& s)
{
	return _tcstod(s,NULL);
}

AString& AString::SureEndWith(ACHAR ch)
{
	if( Right(1) != ch ) this->operator+= (ch);
	return *this;
}
AString& AString::SureEndWith(const AString& s)
{
	if( ReverseFind(s) != Length()- s.Length() ) this->operator+= (s);
	return *this;
}