#pragma once

//万年历算法类
class wnl
{
public:
	struct myDATE{
		int  year;
		int  month;
		int  day;
		bool isRunYue;
	};
	wnl(void);
	~wnl(void);

	//计算星期几 0：星期天 1:星期一 ...
	static int GetWeek(ADateTime& dt);

	LONG lDaysFrom1900(myDATE date);//date与1900年相差的天数
	UINT lYearDays(UINT y);//农历y年的总天数
	UINT leapDays(UINT y);//传回农历 y年闰月的天数
	UINT leapMonth(UINT y);// 传回农历 y年闰哪个月 1-12 , 没闰传回 0
	UINT monthDays(UINT y,UINT m);//传回农历 y年m月的总天数
	myDATE Lunar(myDATE date);//输入阳历日期，返回阴历日期 
	int dayOfWeek(myDATE date);//输入阳历日期，返回星期几 
	UINT solarDays(UINT y,UINT m);//传回国历 y年某m+1月的天数
	char* GetJieri(int m,int d);//得到节日或纪念日的描述
	const char* GetShuXiang(int y);
	AString GetYinliDayString(myDATE de);
	AString GetYearString(myDATE de);
private:
	char                    m_slunar[100];//用于存储农历信息

	static DWORD   lunarInfo[];//
	static UINT    solarMonth[];
	static char    sSolarTerm[];
	static DWORD   dTermInfo[];
	static char    sFtv[];
};

