#include "StdAfx.h"
#include "wnl.h"

static int g_days[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
static ACHAR* g_szDayWeek[]={L"日",L"一",L"二",L"三",L"四",L"五",L"六"};

APopupDateBox::APopupDateBox(AComponent* pOwner):
			base_class(pOwner),
			m_nYear(2013),
			m_nMonth(1),
			m_nDays(28),
			m_nDay(0)
{
	m_iBorderWidth = 0;
	m_iPadding = 0;

	m_pTitle = new AContainer(this);
	m_pTitle->SetAlign(alTop);
	m_pTitle->SetHeight(40);
	EVENT_CONNECT(m_pTitle,OnPaint,OnTitlePaint);

	m_pWeekContainer = new AContainer(this);
	m_pWeekContainer->SetAlign( alTop );
	m_pWeekContainer->SetHeight(20);
	m_pWeekLeftSpace = new AContainer(m_pWeekContainer);
	m_pWeekLeftSpace->SetWidth(30);
	m_pWeekLeftSpace->SetAlign( alLeft );

	m_pWeek = new ATableContainer(m_pWeekContainer);
	m_pWeek->SetAlign( alClient );
	m_pWeek->SetColCount(7);
	m_pWeek->SetRowCount(1);
	for(int i=0;i<7;i++)
	{
		m_pWeekDay[i] = new AContainer(m_pWeek);
		m_pWeekDay[i]->SetText( g_szDayWeek[i] );
		m_pWeekDay[i]->SetTag( (void *)i);
		EVENT_CONNECT(m_pWeekDay[i],OnPaint,OnWeekPaint);
	}

	m_pZhou = new ATableContainer(this);
	m_pZhou->SetAlign(alLeft);
	m_pZhou->SetWidth(30);
	m_pZhou->SetColCount(1);
	m_pZhou->SetRowCount(6);
	for(int i=0;i<6;i++)
	{
		m_pZhouCell[i] = new AContainer(m_pZhou);
		m_pZhouCell[i]->SetTag( (void *)i );
		EVENT_CONNECT(m_pZhouCell[i],OnPaint,OnZhouPaint);
	}

	m_pDayContainer = new ATableContainer(this);
	m_pDayContainer->SetAlign( alClient );
	m_pDayContainer->SetRowCount(6);
	m_pDayContainer->SetColCount(7);
	EVENT_CONNECT(m_pDayContainer,OnPaint,OnCellContainerPaint);

	for(int i=0;i<42;i++)
	{
		AContainer* p = new AContainer(m_pDayContainer);
		p->SetTag((void *)i);
		EVENT_CONNECT(p,OnPaint,OnCellPaint);

		m_aDayHead[i] = new AContainer(p);
		m_aDayHead[i]->SetAlign(alClient);
		m_aDayHead[i]->SetTag((void *)i);
		EVENT_CONNECT(m_aDayHead[i],OnPaint,OnDayPaint);
		EVENT_CONNECT(m_aDayHead[i],OnMouseMove,OnCellMouseMove);
		EVENT_CONNECT(m_aDayHead[i],OnMouseUp,OnCellMouseMove);
		EVENT_CONNECT(m_aDayHead[i],OnClick,OnDayCellClick);
	}

	m_pLongLiNian = new ALabel(m_pTitle);
	m_pLongLiNian->SetLeft(46);
	m_pLongLiNian->SetTop(4);
	m_pLongLiNian->SetWidth(84);
	m_pLongLiNian->SetHeight(32);

	ADateTime dt=ADateTime::GetCurrentTime();

	m_pYearLeft = new AButton(m_pTitle);
	m_pYearLeft->SetLeft(140);
	m_pYearLeft->SetTop(4+10);
	m_pYearLeft->SetWidth(11);
	m_pYearLeft->SetHeight(32-10*2);
	EVENT_CONNECT(m_pYearLeft,OnPaint,OnLeftButtonPaint);
	EVENT_CONNECT(m_pYearLeft,OnClick,OnLRButtonClick);

	m_pEditYear = new AEdit(m_pTitle);
	m_pEditYear->SetLeft(156);
	m_pEditYear->SetTop(4+5);
	m_pEditYear->SetWidth(40);
	m_pEditYear->SetHeight(32-5*2);
	m_pEditYear->SetText(AStringVal(dt.GetYear()));
	EVENT_CONNECT(m_pEditYear,OnKeyboard,OnEditKey);

	m_pYearRight = new AButton(m_pTitle);
	m_pYearRight->SetLeft(201);
	m_pYearRight->SetTop(4+10);
	m_pYearRight->SetWidth(11);
	m_pYearRight->SetHeight(32-10*2);
	EVENT_CONNECT(m_pYearRight,OnPaint,OnRightButtonPaint);
	EVENT_CONNECT(m_pYearRight,OnClick,OnLRButtonClick);

	m_pYearLabel = new ALabel(m_pTitle);
	m_pYearLabel->SetLeft(214);
	m_pYearLabel->SetTop(4+10);
	m_pYearLabel->SetWidth(16);
	m_pYearLabel->SetHeight(32-10*2);
	m_pYearLabel->SetText(L"年");

	m_pMonthLeft = new AButton(m_pTitle);
	m_pMonthLeft->SetLeft(234);
	m_pMonthLeft->SetTop(4+10);
	m_pMonthLeft->SetWidth(11);
	m_pMonthLeft->SetHeight(32-10*2);
	EVENT_CONNECT(m_pMonthLeft,OnPaint,OnLeftButtonPaint);
	EVENT_CONNECT(m_pMonthLeft,OnClick,OnLRButtonClick);

	m_pEditMonth = new AEdit(m_pTitle);
	m_pEditMonth->SetLeft(250);
	m_pEditMonth->SetTop(4+5);
	m_pEditMonth->SetWidth(20);
	m_pEditMonth->SetHeight(32-5*2);
	m_pEditMonth->SetText(AStringVal(dt.GetMonth()));
	EVENT_CONNECT(m_pEditMonth,OnKeyboard,OnEditKey);

	m_pMonthRight = new AButton(m_pTitle);
	m_pMonthRight->SetLeft(275);
	m_pMonthRight->SetTop(4+10);
	m_pMonthRight->SetWidth(11);
	m_pMonthRight->SetHeight(32-10*2);
	EVENT_CONNECT(m_pMonthRight,OnPaint,OnRightButtonPaint);
	EVENT_CONNECT(m_pMonthRight,OnClick,OnLRButtonClick);

	m_pMonthLael = new ALabel(m_pTitle);
	m_pMonthLael->SetLeft(291);
	m_pMonthLael->SetTop(4+10);
	m_pMonthLael->SetWidth(16);
	m_pMonthLael->SetHeight(32-10*2);
	m_pMonthLael->SetText(L"月");

	m_pBackTodayButton = new AButton(m_pTitle);
	m_pBackTodayButton->SetLeft(310);
	m_pBackTodayButton->SetTop(4+6);
	m_pBackTodayButton->SetWidth(40);
	m_pBackTodayButton->SetHeight(32-6*2);
	m_pBackTodayButton->SetText(L"回今天");
	EVENT_CONNECT(m_pBackTodayButton,OnClick,OnTodayClick);
}

APopupDateBox::~APopupDateBox()
{

}

ADateTime APopupDateBox::GetSelected()
{
	return ADateTime(m_nYear,m_nMonth,m_nDay,0,0,0);
}


void APopupDateBox::FillCell(int year,int month)
{
	m_nYear = year;
	m_nMonth = month;
	m_nDay = 0;
	wnl m_wnl;
	//计算总天数
	bool b = false;
	if( ( year % 4 == 0 && year % 100 != 0 ) || year % 400 == 0 ) b = true;
	m_nDays = g_days[month % 13];
	if( month == 2 && b ) m_nDays ++;


	wnl::myDATE de1,de2;
	de2.year = m_nYear; de2.month = m_nMonth;de2.day = 1;
	de1 = m_wnl.Lunar(de2);
	AString sYear;
	sYear = m_wnl.GetYearString(de1);
	m_pLongLiNian->SetText(sYear);//农历年

	//更新周数
	ADateTime dt2(year,1,1,0,0,0);
	ADateTime dt1(year,month,1,0,0,0);
	int delta = (int)((double)dt1.operator DATE() - (double)dt2.operator DATE()) +1;
	int weekIndex= (delta+6)/7;
	for(int i=0;i<6;i++)
	{
		m_pZhouCell[i]->SetText( AString().Format(_T("%02d周"),weekIndex+i) );
	}
	
	//
	int week =0;// m_wnl.GetWeek(dt1);
	wnl::myDATE deOne;
	deOne.year = year;deOne.month = month; deOne.day=1;
	week = m_wnl.dayOfWeek(deOne);
	for(int i=0;i<week;i++)
	{
		m_aDayHead[i]->SetTag( (void *)0 );
	}
	for(int i=week;i<42;i++)
	{
		m_aDayHead[i]->SetTag( (void *)(i-week+1) );
	}

	Refresh();
}

void APopupDateBox::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	ARect r = GetAbsPosition();
	APen pen;
	pen.SetWidth(0.3);
	pen.SetColor(AColor(255,72,191,99));
	cs->Use(&pen)->DrawRect(r);

	base_class::DoPaintChildren(pEvent);
}

void APopupDateBox::OnCellContainerPaint(AObject* pSender,AEvent* pEvent)
{
	ATableContainer* pc = dynamic_cast<ATableContainer*>(pSender);
	if( pc == NULL ) return;
	if( pc == NULL ) return;
	ACanvas* cs = m_pUICore->GetCanvas();
	ARect r = pc->GetAbsPosition();

	int row = pc->GetRowCount();
	int col = pc->GetColCount();

	double dw = (double)r.GetWidth() / (double)col;
	double dh = (double)r.GetHeight() / (double)row;

	ASolidBrush br;
	br.SetColor(AColor(200,233,225,163));

	cs->Use(&br)->FillRect(r.left,r.top,r.left+dw,r.bottom);
	cs->Use(&br)->FillRect(r.left+6*dw,r.top,r.left+7*dw,r.bottom);

	APen pen;
	pen.SetWidth(0.5);
	pen.SetColor(AColor(255,72,191,99));
	//pen.SetStyle(DashStyleDot);
	ASolidBrush br1,br2;
	br1.SetColor(AColor(128,219,238,234));
	br2.SetColor(AColor(128,243,235,243));
	for(int i=0;i<row;i++)
	{
		if( i % 2 ) cs->Use(&br1);
		else cs->Use(&br2);
		cs->FillRect(r.left,r.top+i*dh,r.left+7*dw,r.top+(i+1)*dh);
	}
	for(int i=0;i<row-1;i++)
	{
		cs->Use(&pen)->DrawLine(r.left,r.top+(i+1)*dh,r.left+7*dw,r.top+(i+1)*dh);
	}
	for(int i=0;i<col-1;i++)
	{
		cs->Use(&pen)->DrawLine(r.left+(i+1)*dw,r.top,r.left+(i+1)*dw,r.bottom);
	}
}

void APopupDateBox::OnCellPaint(AObject* pSender,AEvent* pEvent)
{
	AContainer* pc = dynamic_cast<AContainer*>(pSender);
	if( pc == NULL ) return;
	if( pc == NULL ) return;
	ACanvas* cs = m_pUICore->GetCanvas();
	ARect r = pc->GetAbsPosition();
	int nTag = (int)pc->GetTag();
	ASolidBrush br;
	if( m_pUICore->IsOverMe(pc) )
	{
		br.SetColor(AColor(240,40,37,250));
		cs->Use(&br)->FillRect(r);
	}
	if( nTag == 0 ) br.SetColor(AColor(200,235,230,226));
	else br.SetColor(AColor(128,225,238,219));

	cs->Use(&br)->FillRect(r);
}

void APopupDateBox::OnDayPaint(AObject* pSender,AEvent* pEvent)
{
	AContainer* pc = dynamic_cast<AContainer*>(pSender);
	if( pc == NULL ) return;
	ACanvas* cs = m_pUICore->GetCanvas();
	ARect r = pc->GetAbsPosition();
	int nTag = (int)pc->GetTag();
	wnl m_wnl;

	if( nTag > 0 && nTag <= m_nDays )
	{
		if( nTag == m_nDay && m_nDay != 0 )
		{
			ASolidBrush br(AColor(200,83,107,149));
			cs->Use(&br)->FillRect(r);
		}
		//判断是不是今天
		bool bIsToDay=false;
		ADateTime now = ADateTime::GetCurrentTime();
		if( m_nYear == now.GetYear() && m_nMonth == now.GetMonth() && nTag == now.GetDay() ) bIsToDay = true;
		if( bIsToDay )
		{
			APen pen(3,AColor(200,250,64,31),DashStyleDot);
			ARect rx(r.left,r.top,r.left+41,r.top+28);
			cs->Use(&pen)->DrawEllipse(rx);
		}
		ADateTime dt(m_nYear,m_nMonth,nTag,0,0,0);
		AFont f(L"Tunga",20,AColor(200,41,82,39),FontStyleBold);
		//判断是不是周末
		int nParentTag = (int)pc->GetParent()->GetTag();
		if( nParentTag % 7 == 0 || (nParentTag + 1) % 7 == 0 )
			f.SetColor(AColor(200,196,52,40));
		cs->Use(&f)->DrawText(r.left+2,r.top+2,AString().Format(L"%d",nTag));


		AFont fYinli(L"新宋体",10,AColor(200,12,7,35),FontStyleRegular);

		wnl::myDATE de;
		de.year = m_nYear;
		de.month = m_nMonth;
		de.day = nTag;
		wnl::myDATE de2 = m_wnl.Lunar(de);
		AString sYinli = m_wnl.GetYinliDayString(de2);
		cs->Use(&fYinli)->DrawText(r.left+23,r.top+2,sYinli);

		AString sJieri;
		sJieri = m_wnl.GetJieri(m_nMonth,nTag);
		cs->Use(&fYinli)->DrawText(r.left+23,r.top+2+12,sJieri);
	}
}
void APopupDateBox::OnWeekPaint(AObject* pSender,AEvent* pEvent)
{
	AContainer* pc = dynamic_cast<AContainer*>(pSender);
	if( pc == NULL ) return;
	ARect r = pc->GetAbsPosition();
	ACanvas* cs = m_pUICore->GetCanvas();
	ASolidBrush br;
	if( pc->GetTag() == (void *)0 || pc->GetTag() == (void*)6 )
	{
		br.SetColor(AColor(200,233,225,163));
	}
	else
		br.SetColor(AColor(200,225,238,219));
	cs->Use(&br)->FillRect(r);
	AString text = pc->GetText();
	AFont f;
	cs->Use(&f)->DrawText(r,text,StringAlignmentCenter,StringAlignmentMiddle);
}

void APopupDateBox::OnZhouPaint(AObject* pSender,AEvent* pEvent)
{
	AContainer* pc = dynamic_cast<AContainer*>(pSender);
	if( pc == NULL ) return;
	ARect r = pc->GetAbsPosition();
	ACanvas* cs = m_pUICore->GetCanvas();
	ASolidBrush br;
	int iTag = (int)pc->GetTag();
	if( iTag % 2 )
	{
		br.SetColor(AColor(200,219,238,234));
	}
	else
		br.SetColor(AColor(200,243,235,243));
	cs->Use(&br)->FillRect(r);
	AString text = pc->GetText();
	AFont f;
	cs->Use(&f)->DrawText(r,text,StringAlignmentCenter,StringAlignmentMiddle);
}

void APopupDateBox::OnTitlePaint(AObject* pSender,AEvent* pEvent)
{
	AContainer* pc = dynamic_cast<AContainer*>(pSender);
	if( pc == NULL ) return;
	ARect r = pc->GetAbsPosition();
	ACanvas* cs = m_pUICore->GetCanvas();
	ASolidBrush br(AColor(203,250,252,251));
	cs->Use(&br)->FillRect(r);
}

void APopupDateBox::OnLeftButtonPaint(AObject* pSender,AEvent* pEvent)
{
	AButton* pc = dynamic_cast<AButton*>(pSender);
	if( pc == NULL ) return;
	ARect r = pc->GetAbsPosition();
	ACanvas* cs = m_pUICore->GetCanvas();
	APen pen;
	if( m_pUICore->IsOverMe(pc) ) pen.SetColor(AColor(255,234,61,23));
	else if( m_pUICore->GetMouseDownControl() == pc ) pen.SetColor(AColor(255,70,187,90));
	else pen.SetColor(AColor(255,29,122,29));
	pen.SetWidth(3);
	cs->Use(&pen)->DrawLine(r.right,r.top,r.left,r.top+r.GetHeight()/2);
	cs->Use(&pen)->DrawLine(r.left,r.top+r.GetHeight()/2,r.right,r.bottom);
}
void APopupDateBox::OnRightButtonPaint(AObject* pSender,AEvent* pEvent)
{
	AButton* pc = dynamic_cast<AButton*>(pSender);
	if( pc == NULL ) return;
	ARect r = pc->GetAbsPosition();
	ACanvas* cs = m_pUICore->GetCanvas();
	APen pen;
	if( m_pUICore->IsOverMe(pc) ) pen.SetColor(AColor(255,234,61,23));
	else if( m_pUICore->GetMouseDownControl() == pc ) pen.SetColor(AColor(255,70,187,90));
	else pen.SetColor(AColor(255,29,122,29));
	pen.SetWidth(3);
	cs->Use(&pen)->DrawLine(r.left,r.top,r.right,r.top+r.GetHeight()/2);
	cs->Use(&pen)->DrawLine(r.right,r.top+r.GetHeight()/2,r.left,r.bottom);
}

void APopupDateBox::OnLRButtonClick(AObject* pSender,AEvent* pEvent)
{
	int year = AIntVal( m_pEditYear->GetText() );
	int month = AIntVal( m_pEditMonth->GetText() );
	if( (AControl *)pSender == m_pYearLeft ) year --;
	else if( (AControl *)pSender == m_pYearRight ) year ++;
	else if( (AControl *)pSender == m_pMonthLeft ) month --;
	else if( (AControl *)pSender == m_pMonthRight ) month ++;
	if( year < 1900 ) year = 1900;
	if( year > 3000 ) year = 3000;
	if( month < 1 ) month = 12;
	if( month > 12 ) month = 1;
	m_pEditYear->SetText( AStringVal(year) );
	m_pEditMonth->SetText( AStringVal(month) );

	FillCell(year,month);
}

void APopupDateBox::OnEditKey(AObject* pSender,AEvent* pEvent)
{
	AKeyEvent* evt = dynamic_cast<AKeyEvent*>(pEvent);
	if( evt == NULL ) return;

	if( (AControl*)pSender == m_pEditYear || (AControl*)pSender == m_pEditMonth )
	{
		if( evt->m_Type == ketPress && evt->m_chKey == VK_RETURN )
		{
			int year = AIntVal( m_pEditYear->GetText() );
			int month = AIntVal( m_pEditMonth->GetText() );
			if( year < 1900 ) year = 1900;
			if( year > 3000 ) year = 3000;
			if( month < 1 ) month = 12;
			if( month > 12 ) month = 1;
			FillCell(year,month);
		}
	}
}

void APopupDateBox::OnTodayClick(AObject* pSender,AEvent* pEvent)
{
	ADateTime dt = ADateTime::GetCurrentTime();
	m_pEditYear->SetText( AStringVal(dt.GetYear()) );
	m_pEditMonth->SetText( AStringVal(dt.GetMonth()) );
	FillCell(dt.GetYear(),dt.GetMonth());
}

void APopupDateBox::OnCellMouseMove(AObject* pSender,AEvent* pEvent)
{
	m_pDayContainer->Refresh();
}

//对ES的支持
void APopupDateBox::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
}

void APopupDateBox::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("Date") )
	{
		AString s = pValue->GetString();
		AStringArray sa;
		sa.FromString(s,L"-");
		if( sa.GetCount() == 2 )
		{
			FillCell(AIntVal(sa[0]),AIntVal(sa[1]));
		}
	}
}
void APopupDateBox::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{

}

void APopupDateBox::OnDayCellClick(AObject* pSender,AEvent* pEvent)
{
	AControl* p = (AControl*)pSender;
	m_nDay = (int)p->GetTag();
	OnSelected.Call(this,pEvent);
	ClosePopup();
}



