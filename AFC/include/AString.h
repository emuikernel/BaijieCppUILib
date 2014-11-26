
#ifndef __ASTRING_H__
#define __ASTRING_H__

#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif
#endif

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif

#ifdef _UNICODE
#define ACHAR	wchar_t
#define UACHAR	char
#else
#define ACHAR	char
#define UACHAR	wchar_t
#endif

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <TCHAR.h>
#include <windows.h>



class AFC_API AString
{
public:
	friend  AFC_API int ATextCompare(const ACHAR *s1,const ACHAR *s2);
	friend  AFC_API int ATextCompare(const ACHAR *s1,const AString &s2);
	friend  AFC_API int ATextCompare(const AString &s1,const ACHAR *s2);
	friend  AFC_API int ATextCompare(const AString &s1,const AString &s2);

	/// Compare up to n
	friend  AFC_API int ATextCompare(const ACHAR *s1,const ACHAR *s2,int n);
	friend  AFC_API int ATextCompare(const ACHAR *s1,const AString &s2,int n);
	friend  AFC_API int ATextCompare(const AString &s1,const ACHAR *s2,int n);
	friend  AFC_API int ATextCompare(const AString &s1,const AString &s2,int n);

	/// Compare case insensitive
	friend  AFC_API int ATextCompareCase(const ACHAR *s1,const ACHAR *s2);
	friend  AFC_API int ATextCompareCase(const ACHAR *s1,const AString &s2);
	friend  AFC_API int ATextCompareCase(const AString &s1,const ACHAR *s2);
	friend  AFC_API int ATextCompareCase(const AString &s1,const AString &s2);

	/// Compare case insensitive up to n
	friend  AFC_API int ATextCompareCase(const ACHAR *s1,const ACHAR *s2,int n);
	friend  AFC_API int ATextCompareCase(const ACHAR *s1,const AString &s2,int n);
	friend  AFC_API int ATextCompareCase(const AString &s1,const ACHAR *s2,int n);
	friend  AFC_API int ATextCompareCase(const AString &s1,const AString &s2,int n);

	/// Comparison operators
	friend AFC_API  bool operator==(const AString &s1,const AString &s2);
	friend AFC_API  bool operator==(const AString &s1,const ACHAR *s2);
	friend AFC_API  bool operator==(const ACHAR *s1,const AString &s2);

	friend AFC_API  bool operator!=(const AString &s1,const AString &s2);
	friend AFC_API  bool operator!=(const AString &s1,const ACHAR *s2);
	friend AFC_API  bool operator!=(const ACHAR *s1,const AString &s2);

	friend AFC_API  bool operator<(const AString &s1,const AString &s2);
	friend AFC_API   bool operator<(const AString &s1,const ACHAR *s2);
	friend AFC_API  bool operator<(const ACHAR *s1,const AString &s2);

	friend AFC_API  bool operator<=(const AString &s1,const AString &s2);
	friend AFC_API  bool operator<=(const AString &s1,const ACHAR *s2);
	friend AFC_API  bool operator<=(const ACHAR *s1,const AString &s2);

	friend AFC_API  bool operator>(const AString &s1,const AString &s2);
	friend AFC_API  bool operator>(const AString &s1,const ACHAR *s2);
	friend AFC_API  bool operator>(const ACHAR *s1,const AString &s2);

	friend AFC_API  bool operator>=(const AString &s1,const AString &s2);
	friend AFC_API  bool operator>=(const AString &s1,const ACHAR *s2);
	friend AFC_API  bool operator>=(const ACHAR *s1,const AString &s2);

	/// Concatenate two strings
	friend AFC_API  AString operator+(const AString& s1,const AString& s2);
	friend AFC_API  AString operator+(const AString& s1,const ACHAR* s2);
	friend AFC_API  AString operator+(const ACHAR* s1,const AString& s2);

	// Concatenate with single character
	friend AFC_API  AString operator+(const AString& s,ACHAR c);
	friend AFC_API  AString operator+(ACHAR c,const AString& s);

	/// Format a string a-la printf
	friend  AString AStringFormat(const ACHAR *fmt,...) ;
	friend  AString AStringVFormat(const ACHAR *fmt,va_list args);

	/// Convert number to a string
	friend  AFC_API AString AStringVal(int num,int base=10);
	friend  AFC_API AString AStringVal(UINT num,int base=10);
	friend  AFC_API AString AStringVal(__int64 num,int base=10);
	friend  AFC_API AString AStringVal(unsigned __int64 num,int base=10);
	friend  AFC_API AString AStringVal(float num,int prec=6);
	friend  AFC_API AString AStringVal(double num,int prec=6);

	/// Convert string to a number
	friend  AFC_API __int64 AInt64Val(const AString& s,int base=10);
	friend AFC_API unsigned __int64 AUInt64Val(const AString& s,int base=10);
	friend  AFC_API int AIntVal(const AString& s,int base=10);
	friend  AFC_API UINT AUINTVal(const AString& s,int base=10);
	friend  AFC_API float AFloatVal(const AString& s);
	friend  AFC_API double ADoubleVal(const AString& s);

	AString();
	AString(const AString& s);
	AString(ACHAR ch);

	AString(const ACHAR* s1);
	AString(const UACHAR* s1);

	AString(const ACHAR* s1,const ACHAR* s2);
	AString(const ACHAR* s1,int nCount);
	AString(const UACHAR* s1,int nCount);
	AString(ACHAR ch,int nCount);
	~AString();
	int Length() const;
	const ACHAR* Text() const;
	operator const ACHAR* () const;
	bool IsEmpty() const;
	ACHAR& operator[](int i);
	const ACHAR& operator[](int i) const;

	AString& operator =(const AString& s);
	AString& operator =(const ACHAR* s);
	AString& operator=(const UACHAR* s);

	AString& operator+=(const AString& s);
	AString& operator+=(const ACHAR* s);
	AString& operator +=(const UACHAR* s);
	AString& operator+=(ACHAR c);
	AString& operator +=(UACHAR c);

	AString& ToLower();
	AString& ToUpper();

	AString& Replace(const ACHAR* sOld,const ACHAR* sNew);
	AString& Trim();
	AString& TrimLeft();
	AString& TrimRight();
	AString& Clear();

	AString Left(int n) const;
	AString Right(int n) const;
	AString Mid(int pos,int nCount) const;

	//确保是以某个字符结尾
	AString& SureEndWith(ACHAR ch);
	//确保是以某个字符串结尾
	AString& SureEndWith(const AString& s);

	int Find(const ACHAR* s,int pos=0) const;
	int ReverseFind(const ACHAR* s,int pos=2147483647) const;
	AString& Format(const ACHAR *fmt,...);
	AString& VFormat(const ACHAR *fmt,va_list args);

	const UACHAR* ConvertTo();

	AString& ConvertFrom(const UACHAR* str);

private:

	class _StrData
	{
	public:
		_StrData(int iSize=0);
		~_StrData();
                void AddRef();
                void Release();
		LONG GetRefCount();
		_StrData* Clone();
		ACHAR*& GetText();
	private:
		void Clear();
		ACHAR* m_szData;
		LONG m_iRefCount;
	};

        _StrData* m_pData;
	UACHAR*	m_pConvData;//char -> wchar_t  OR wchar_t -> char

        void CopyOnWrite();
};


AFC_API int ATextCompare(const ACHAR *s1,const ACHAR *s2);

AFC_API int ATextCompare(const ACHAR *s1,const AString &s2);
AFC_API int ATextCompare(const AString &s1,const ACHAR *s2);
AFC_API int ATextCompare(const AString &s1,const AString &s2);

	/// Compare up to n

AFC_API int ATextCompare(const ACHAR *s1,const ACHAR *s2,int n);

AFC_API int ATextCompare(const ACHAR *s1,const AString &s2,int n);

AFC_API int ATextCompare(const AString &s1,const ACHAR *s2,int n);

AFC_API int ATextCompare(const AString &s1,const AString &s2,int n);
	/// Compare case insensitive

AFC_API int ATextCompareCase(const ACHAR *s1,const ACHAR *s2);
AFC_API int ATextCompareCase(const ACHAR *s1,const AString &s2);
AFC_API int ATextCompareCase(const AString &s1,const ACHAR *s2);

AFC_API int ATextCompareCase(const AString &s1,const AString &s2);

	/// Compare case insensitive up to n

AFC_API int ATextCompareCase(const ACHAR *s1,const ACHAR *s2,int n);

AFC_API int ATextCompareCase(const ACHAR *s1,const AString &s2,int n);
AFC_API int ATextCompareCase(const AString &s1,const ACHAR *s2,int n);

AFC_API int ATextCompareCase(const AString &s1,const AString &s2,int n);
	/// Comparison operators

AFC_API bool operator==(const AString &s1,const AString &s2);
AFC_API bool operator==(const AString &s1,const ACHAR *s2);

AFC_API bool operator==(const ACHAR *s1,const AString &s2);

AFC_API bool operator!=(const AString &s1,const AString &s2);

AFC_API bool operator!=(const AString &s1,const ACHAR *s2);

AFC_API bool operator!=(const ACHAR *s1,const AString &s2);


AFC_API bool operator<(const AString &s1,const AString &s2);
AFC_API bool operator<(const AString &s1,const ACHAR *s2);

AFC_API bool operator<(const ACHAR *s1,const AString &s2);


AFC_API bool operator<=(const AString &s1,const AString &s2);
AFC_API bool operator<=(const AString &s1,const ACHAR *s2);

AFC_API bool operator<=(const ACHAR *s1,const AString &s2);


AFC_API bool operator>(const AString &s1,const AString &s2);

AFC_API bool operator>(const AString &s1,const ACHAR *s2);

AFC_API bool operator>(const ACHAR *s1,const AString &s2);


AFC_API bool operator>=(const AString &s1,const AString &s2);

AFC_API bool operator>=(const AString &s1,const ACHAR *s2);

AFC_API bool operator>=(const ACHAR *s1,const AString &s2);

	/// Concatenate two strings

AFC_API AString operator+(const AString& s1,const AString& s2);

AFC_API AString operator *(const AString& s1,const AString& s2);

AFC_API AString operator /(const AString& s1,const AString& s2);
AFC_API AString operator+(const AString& s1,const ACHAR* s2);

AFC_API AString operator+(const ACHAR* s1,const AString& s2);

	// Concatenate with single character

AFC_API AString operator+(const AString& s,ACHAR c);

AFC_API AString operator+(ACHAR c,const AString& s);

	/// Format a string a-la printf

AString AStringFormat(const ACHAR *fmt,...);

AString AStringVFormat(const ACHAR *fmt,va_list args);
	/// Convert number to a string
AFC_API AString AStringVal(__int64 num,int base);
AFC_API AString AStringVal(unsigned __int64 num,int base);

AFC_API AString AStringVal(int num,int base);

AFC_API AString AStringVal(UINT num,int base);

AFC_API AString AStringVal(float num,int prec);

AFC_API AString AStringVal(double num,int prec);

	/// Convert string to a number

AFC_API int AIntVal(const AString& s,int base);
AFC_API __int64 AInt64Val(const AString& s,int base);
AFC_API unsigned __int64 AUInt64Val(const AString& s,int base);
AFC_API UINT AUINTVal(const AString& s,int base);
AFC_API float AFloatVal(const AString& s);

AFC_API double ADoubleVal(const AString& s);



#endif//__ASTRING_H__
