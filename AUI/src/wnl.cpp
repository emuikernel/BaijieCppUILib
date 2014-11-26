#include "StdAfx.h"
#include "wnl.h"


wnl::wnl(void)
{
}


wnl::~wnl(void)
{
}


/*
*蔡勒（Zeller）公式。即w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1 

公式中的符号含义如下，
w：星期；
c：世纪-1；
y：年（两位数）；
m：月（m大于等于3，小于等于14，即在蔡勒公式中，某年的1、2月要看作上一年的13、14月来计算，比如2003年1月1日要看作2002年的13月1日来计算）；
d：日；
[ ]代表取整，即只要整数部分。(C是世纪数减一，y是年份后两位，M是月份，d是日数。1月和2月要按上一年的13月和 14月来算，这时C和y均按上一年取值。) 

算出来的W除以7，余数是几就是星期几。如果余数是0，则为星期日。 
 */

int wnl::GetWeek(ADateTime& dt)
{
	int y,m,d,c;
	y = dt.GetYear();
	m = dt.GetMonth();
	d = dt.GetDay();

	c = y /100;
	y = y % 100;
	if( m < 3 ) m += 12;
	int w = y+y/4+c/4-2*c+26*(m+1)/10+d-1;
	return w % 7;//BUG:2013.3.1会算出一个负数！！
}

DWORD wnl::lunarInfo[]={
	0x04bd8,0x04ae0,0x0a570,0x054d5,0x0d260,0x0d950,0x16554,0x056a0,0x09ad0,0x055d2,
	0x04ae0,0x0a5b6,0x0a4d0,0x0d250,0x1d255,0x0b540,0x0d6a0,0x0ada2,0x095b0,0x14977,
	0x04970,0x0a4b0,0x0b4b5,0x06a50,0x06d40,0x1ab54,0x02b60,0x09570,0x052f2,0x04970,
	0x06566,0x0d4a0,0x0ea50,0x06e95,0x05ad0,0x02b60,0x186e3,0x092e0,0x1c8d7,0x0c950,
	0x0d4a0,0x1d8a6,0x0b550,0x056a0,0x1a5b4,0x025d0,0x092d0,0x0d2b2,0x0a950,0x0b557,
	0x06ca0,0x0b550,0x15355,0x04da0,0x0a5d0,0x14573,0x052d0,0x0a9a8,0x0e950,0x06aa0,
	0x0aea6,0x0ab50,0x04b60,0x0aae4,0x0a570,0x05260,0x0f263,0x0d950,0x05b57,0x056a0,
	0x096d0,0x04dd5,0x04ad0,0x0a4d0,0x0d4d4,0x0d250,0x0d558,0x0b540,0x0b5a0,0x195a6,
	0x095b0,0x049b0,0x0a974,0x0a4b0,0x0b27a,0x06a50,0x06d40,0x0af46,0x0ab60,0x09570,
	0x04af5,0x04970,0x064b0,0x074a3,0x0ea50,0x06b58,0x055c0,0x0ab60,0x096d5,0x092e0,
	0x0c960,0x0d954,0x0d4a0,0x0da50,0x07552,0x056a0,0x0abb7,0x025d0,0x092d0,0x0cab5,
	0x0a950,0x0b4a0,0x0baa4,0x0ad50,0x055d9,0x04ba0,0x0a5b0,0x15176,0x052b0,0x0a930,
	0x07954,0x06aa0,0x0ad50,0x05b52,0x04b60,0x0a6e6,0x0a4e0,0x0d260,0x0ea65,0x0d530,
	0x05aa0,0x076a3,0x096d0,0x04bd7,0x04ad0,0x0a4d0,0x1d0b6,0x0d250,0x0d520,0x0dd45,
	0x0b5a0,0x056d0,0x055b2,0x049b0,0x0a577,0x0a4b0,0x0aa50,0x1b255,0x06d20,0x0ada0
};
//阳历节日
static struct 
{
	char* szDate;//日期 MMDD格式
	char* jieri;//节日
} g_YLJieri[]=
{
	{"0101","元旦"},{"0202","世界湿地日"},{"0210","国际气象节"},{"0214","情人节"},
	{"0303","全国爱耳日"},{"0308","妇女节"},{"0312","植树节"},{"0314","国际警察日"},
	{"0315","国际消费日"},{"0321","世界森林日"},{"0322","世界水日"},{"0323","世界气象日"},
	{"0324","世界防治结核病日"},{"0401","愚人节"},{"0407","世界卫生日"},
	{"0422","世界地球日"},{"0501","国际劳动节"},{"0504","中国青年节"},{"0505","全国碘缺乏病日"},
	{"0508","世界红十字日"},{"0512","国际护士节"},{"0515","国际家庭日"},{"0517","世界电信日"},
	{"0518","国际博物馆日"},{"0519","全国助残日"},{"0520","全国学生营养日"},{"0522","国际生物多样性日"},
	{"0523","国际牛奶日"},{"0531","世界无烟日"},{"0601","国际儿童节"},{"0605","世界环境日"},
	{"0606","全国爱眼日"},{"0617","防治荒漠化和干旱日"},{"0623","国际奥林匹克日"},{"0625","全国土地日"},
	{"0626","国际反毒品日"},{"0701","香港回归日 建党日"},{"0707","中国人民抗日战争纪念日"},{"0711","世界人口日"},
	{"0801","八一建军节"},{"0908","国际扫盲日"},{"0910","教师节"},{"0916","国际臭氧层保护日"},
	{"0917","国际和平日"},{"0920","国际爱牙日"},{"0922","国际聋人节"},{"0927","世界旅游日"},
	{"1001","国庆节 国际音乐节"},{"1002","国际减轻自然灾害日"},{"1004","世界动物日"},{"1006","老人节"},{"1007","国际住房日"},
	{"1008","全国高血压日 世界视觉日"},{"1009","世界邮政日"},{"1010","世界精神卫生日"},{"1015","国际盲人节"},
	{"1016","世界粮食节"},{"1017","世界消除贫困日"},{"1022","世界传统医药日"},{"1024","联合国日"},
	{"1031","万圣节"},{"1108","中国记者日"},{"1109","消防宣传日"},{"1112","孙中山诞辰"},
	{"1114","世界糖尿病日"},
	{"1117","国际大学生节"},{"1128","感恩节"},{"1201","世界艾滋病日"},{"1203","世界残疾人日"},
	{"1209","世界足球日"},{"1225","圣诞节"},{"1226","毛主席诞辰"}

};

//二十四节气表
char* g_szJieqi[]={
	"小寒","大寒","立春","雨水","惊蛰","春分",
	"清明","谷雨","立夏","小满","芒种","夏至",
	"小暑","大暑","立秋","处暑","白露","秋分",
	"寒露","霜降","立冬","小雪","大雪","冬至"
};
/*
*把当天和1900年1月0日（星期日）的差称为积日，那么第y年（1900年算第0年）第x 个节气的积日是  
F = 365.242 * y + 6.2 + 15.22 * x - 1.9 * sin(0.262 * x)  这个公式的误差在0.05天左右。 
 */


UINT wnl::solarMonth[]={31,28,31,30,31,30,31,31,30,31,30,31};
UINT wnl::lYearDays(UINT y)
{
	UINT i, sum = 348; 
	for(i=0x8000; i>0x8; i>>=1)
	{
		sum += (lunarInfo[y-1900] & i)? 1: 0;
	}
	return(sum+leapDays(y));
}

UINT wnl::leapDays(UINT y)
{
	if(leapMonth(y))
	{
		return (lunarInfo[y-1900] & 0x10000)? 30: 29;
	}
	else
	{
		return 0;
	}
}

UINT wnl::leapMonth(UINT y)
{
	return lunarInfo[y-1900] & 0xf;
}

UINT wnl::monthDays(UINT y,UINT m)
{
	return (lunarInfo[y-1900] & (0x10000>>m))? 30: 29;
}
LONG wnl::lDaysFrom1900(myDATE date)
{
	LONG days;

	days=365*date.year+(date.year-1)/4-(date.year-1)/100+(date.year-1)/400-
		(365*1900+(1900-1)/4-(1900-1)/100+(1900-1)/400);

	for(int i=0;i<date.month-1;i++)
	{
		days+=solarMonth[i];
	}
	days+=date.day;
	if((date.year%4==0&&date.year%100!=0)||date.year%400==0)
	{
		if(date.month>2)
		{
			days++;
		}
	}
	return days;
}

wnl::myDATE wnl::Lunar(myDATE date)
{
	LONG offset;
	int i, leap=0, temp=0;
	myDATE retdate;

	offset=lDaysFrom1900(date)-30;

	for(i=1900; i<2050 && offset>0; i++)
	{
		temp = lYearDays(i);
		offset -= temp;
	}

	if(offset<0)
	{
		offset += temp;
		i--;
	}

	retdate.year = i;

	leap = leapMonth(i);
	bool isLeap = false;

	for(i=1; i<13 && offset>0; i++)
	{
		if(leap>0 && i==(leap+1) && isLeap==false)
		{
			--i;
			isLeap = true;
			temp = leapDays(retdate.year);
		}
		else
		{
			temp = monthDays(retdate.year, i);
		}
		if(isLeap==true && i==(leap+1)) 
		{
			isLeap = false;
		}
		offset -= temp;
	}
	if(offset==0 && leap>0 && i==leap+1)
	{
		if(isLeap)
		{ 
			isLeap = false;
		}
		else
		{
			isLeap = true;
			--i;
		}
	}
	if(offset<=0)
	{
		offset += temp;
		--i;
	}
	retdate.month = i;
	retdate.day = offset ;
	retdate.isRunYue=isLeap;
	return retdate;
}

static const char *g_cDayName[]  = { "*","初一","初二","初三","初四","初五", /*农历日期名*/
	"初六","初七","初八","初九","初十",
	"十一","十二","十三","十四","十五",
	"十六","十七","十八","十九","二十",
	"廿一","廿二","廿三","廿四","廿五",       
	"廿六","廿七","廿八","廿九","三十"};
static const char *g_cMonName[]  = {"*","正","二","三","四","五","六", "七","八","九","十","十一","腊"};

AString wnl::GetYinliDayString(myDATE de)
{
	AString s;
	if( de.day == 1 )
	{
		if( de.isRunYue ) s += L"闰";
		s += AString( g_cMonName[de.month] );
		s += L"月";
	}
	else
	{
		s = g_cDayName[de.day];
	}
	
	return s;
}
int wnl::dayOfWeek(myDATE date)
{
	//char *cWeekName[]  = {"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};

	if(date.month==1||date.month==2)
	{
		date.month+=12;
		date.year--;
	}
	return (date.day+1+2*date.month+3*(date.month+1)/5+date.year+date.year/4-date.year/100+date.year/400)%7;
}

UINT wnl::solarDays(UINT y,UINT m)
{
	if(m==1)//2月
	{
		return(((y%4 == 0) && (y%100 != 0) || (y%400 == 0))? 29: 28);
	}
	else
	{
		return(solarMonth[m]);
	}
}

char* wnl::GetJieri(int m,int d)
{
	char szBuf[48];
	sprintf(szBuf,"%02d%02d",m,d);
	int iCount = sizeof(g_YLJieri)/sizeof(g_YLJieri[0]);
	for(int i=0;i<iCount;i++)
	{
		if( strcmp(szBuf,g_YLJieri[i].szDate) == 0 )
			return g_YLJieri[i].jieri;
	}
	return NULL;
}

const char *g_cShuXiang[] = {"鼠","牛","虎","兔","龙","蛇","马","羊","猴","鸡","狗","猪"}; /*属相名称*/

const char* wnl::GetShuXiang(int y)
{
	int nShuXiang = ((y - 4) % 60) % 12;
	return g_cShuXiang[nShuXiang];
}

static const char *g_cTianGan[]  = {"甲","乙","丙","丁","戊","己","庚","辛","壬","癸"}; /*天干名称*/
static const char *g_cDiZhi[]    = {"子","丑","寅","卯","辰","巳","午", "未","申","酉","戌","亥"}; /*地支名称*/

AString wnl::GetYearString(myDATE de)
{
	AString s(L"农历");
	/*--生成农历天干、地支、属相 ==> wNongli--*/
	int nShuXiang = ((de.year - 4) % 60) % 12;
	
	int nTianGan = ((de.year - 4) % 60) % 10;
	int nDiZhi = ((de.year - 4) % 60) % 12;
	s += AString(g_cTianGan[nTianGan]) + AString(g_cDiZhi[nDiZhi]);
	s += AString(L"(")+AString(g_cShuXiang[nShuXiang])+L")年";

	return s;
}