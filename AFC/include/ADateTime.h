#ifndef ADATETIME_H
#define ADATETIME_H

#include <math.h>
#include <time.h>

#ifndef DATE
#define DATE double
#endif

class AFC_API ADateTime : public AObject
{
	CLASS_INFO(DateTime,Object);
public:
	ADateTime();
	ADateTime(const ADateTime& dt);
	ADateTime(int y,int m,int d,int h,int n,int s);
	ADateTime(const DATE d);
	virtual ~ADateTime();

	const unsigned short GetYear() const { return m_year; }
	const unsigned short GetMonth() const { return m_month; }
	const unsigned short GetDay() const { return m_day; }
	const unsigned short GetHour() const { return m_hour; }
	const unsigned short GetMinute() const { return m_minute; }
	const unsigned short GetSecond() const { return m_second; }
	const unsigned short GetMSecond() const { return m_msecond; }

	static ADateTime GetCurrentTime();

	AString Format();
	operator DATE();
	ADateTime& operator =(DATE d);
	time_t systime_to_timet( const SYSTEMTIME& st );
	SYSTEMTIME timet_to_systime( time_t t );
	time_t lotus_to_timet( double dt );
	double timet_to_lotus( time_t t );

protected:
	unsigned short m_year,m_month,m_day,m_hour,m_minute,m_second,m_msecond;

	void SetDateTime(unsigned short y, unsigned short m, unsigned short d, unsigned short h, unsigned short n, unsigned short s,unsigned short ms=0);
};

#endif//ADATETIME_H