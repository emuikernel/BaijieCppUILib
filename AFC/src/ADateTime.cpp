#include "stdafx.h"

ADateTime::ADateTime():base_class()
{
	SetDateTime(0,0,0,0,0,0);
}

ADateTime::ADateTime(const ADateTime& dt):base_class()
{
	SetDateTime(dt.m_year,dt.m_month,dt.m_day,dt.m_hour,dt.m_minute,dt.m_second,dt.m_msecond);
}
ADateTime::ADateTime(int y,int m,int d,int h,int n,int s):base_class()
{
	SetDateTime(y,m,d,h,n,s);
}
ADateTime::ADateTime(const DATE d):base_class()
{
	time_t t1;
	t1 = lotus_to_timet(d);
	SYSTEMTIME st;
	st = timet_to_systime(t1);
	SetDateTime(st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
}

ADateTime::~ADateTime()
{
	//
}

ADateTime ADateTime::GetCurrentTime()
{
	ADateTime dt;
	SYSTEMTIME st;
	GetLocalTime(&st);
	dt.m_year = st.wYear;dt.m_month = st.wMonth;dt.m_day = st.wDay;
	dt.m_hour = st.wHour; dt.m_minute = st.wMinute; dt.m_second = st.wSecond;
	dt.m_msecond = st.wMilliseconds;
	return dt;
}

AString ADateTime::Format()
{
	AString s;
	s.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),m_year,m_month,m_day,m_hour,m_minute,m_second);
	return s;
}
ADateTime::operator DATE()
{
	SYSTEMTIME st;
	st.wYear = m_year;
	st.wMonth = m_month;
	st.wDay = m_day;
	st.wHour = m_hour;
	st.wMinute = m_minute;
	st.wSecond = m_second;
	st.wMilliseconds = m_msecond;
	time_t t1;
	t1 = systime_to_timet(st);
	double d;
	d = timet_to_lotus(t1);
	return d;
}
ADateTime& ADateTime::operator =(DATE d)
{
	time_t t1;
	t1 = lotus_to_timet(d);
	SYSTEMTIME st;
	st = timet_to_systime(t1);
	SetDateTime(st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	return *this;
}
time_t ADateTime::systime_to_timet( const SYSTEMTIME& st )
{
	struct tm gm = { st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900, st.wDayOfWeek, 0, 0 };
	return mktime( &gm );
}
SYSTEMTIME ADateTime::timet_to_systime( time_t t )
{
	tm gm = *localtime( &t );
	SYSTEMTIME st = { 1900+gm.tm_year, 1+gm.tm_mon, gm.tm_wday, gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec, 0 };
	return st;
}
time_t ADateTime::lotus_to_timet( double dt )
{
	int timezone = 0;
	return (time_t)( (dt-25569)*(24*60*60)-timezone + 0.5 ); // +0.5是什么原因大家肯定都知道。俺就奇怪了，这格式是哪个SN制定的，如果当时没有long long，那也宁可用struct，而不是double
}
double ADateTime::timet_to_lotus( time_t t )
{
	int timezone = 0;
		/*return t/(24*60*60) + 25569　　　　　　　　　　　　 // t/(24*60*60)就只留下天数。time_t以1970/1/1为起点，lotus以1899/12/30为基点，差25569天
		//　　　 +　 t%(24*60*60)/(60*60) / (24.0)　　　 // t%(24*60*60)就只留下时分秒，再/(60*60)就留下时，除24就转化为lotus时
		//　　　 +　 t%(60*60)/(60) / (24.0*60.0)　　　　 // t%(60*60)就只留下分秒，再/(60)就留下分，除24*60就转化为lotus分
		//　　　 +　 t%60 / (24.0*60.0*60.0);　　　　　　　　 // t%(60)就只留下秒，除24*60*60就转化为lotus秒
		*/
		return (t+timezone)/(24.0*60.0*60.0) + 25569; // timezone是时区差
}
void ADateTime::SetDateTime(unsigned short y, unsigned short m, unsigned short d, unsigned short h, unsigned short n, unsigned short s,unsigned short ms/*=0*/)
{
	m_year = y;
	m_month = m;
	m_day = d;
	m_hour = h;
	m_minute = n;
	m_second = s;
	m_msecond = ms;
}