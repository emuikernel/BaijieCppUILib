#ifndef APOPUPDATEBOX_H
#define APOPUPDATEBOX_H

class AUI_API APopupDateBox: public APopupContainer
{
	CLASS_INFO(PopupDateBox,PopupContainer);
public:
	APopupDateBox(AComponent* pOwner);
	virtual ~APopupDateBox();

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

	virtual ADateTime GetSelected();

	void OnWeekPaint(AObject* pSender,AEvent* pEvent);
	void OnZhouPaint(AObject* pSender,AEvent* pEvent);
	void OnDayPaint(AObject* pSender,AEvent* pEvent);
	void OnCellContainerPaint(AObject* pSender,AEvent* pEvent);
	void OnCellPaint(AObject* pSender,AEvent* pEvent);
	void OnTitlePaint(AObject* pSender,AEvent* pEvent);
	void OnLeftButtonPaint(AObject* pSender,AEvent* pEvent);
	void OnRightButtonPaint(AObject* pSender,AEvent* pEvent);
	void OnLRButtonClick(AObject* pSender,AEvent* pEvent);
	void OnEditKey(AObject* pSender,AEvent* pEvent);
	void OnTodayClick(AObject* pSender,AEvent* pEvent);
	void OnCellMouseMove(AObject* pSender,AEvent* pEvent);
	void OnDayCellClick(AObject* pSender,AEvent* pEvent);
		
	void FillCell(int year,int month);

	virtual int GetCurYear(){ return m_nYear; }
	virtual int GetCurMonth(){ return m_nMonth; }
	virtual int GetCurDay(){ return m_nDay; }

	//对ES的支持
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

	AEventHandler		OnSelected;
protected:
	

private:
	int			m_nYear;//当前年
	int			m_nMonth;//当前月
	int			m_nDays;//当前月的总天数
	int			m_nDay;//当前选中的天，缺省是每月1日
	AContainer*		m_pTitle;//抬头
	AContainer*		m_pWeekContainer;//星期几容器
	AContainer*		m_pWeekLeftSpace;//星期几的左边留白
	ATableContainer*	m_pWeek;//星期几
	ATableContainer*	m_pZhou;//第几周
	AContainer*		m_pWeekDay[7];
	AContainer*		m_pZhouCell[6];
	ATableContainer*	m_pDayContainer;//日期单元格容器
	AContainer*		m_aDayHead[42];//6 * 7的表格
	ALabel*			m_pLongLiNian;//农历年
	AButton*		m_pYearLeft;//年左边的调节按钮
	AButton*		m_pYearRight;
	AButton*		m_pMonthLeft;//月左边的调节按钮
	AButton*		m_pMonthRight;
	ALabel*			m_pYearLabel;
	ALabel*			m_pMonthLael;
	AEdit*			m_pEditYear;
	AEdit*			m_pEditMonth;
	AButton*		m_pBackTodayButton;//回今天
};
#endif//APOPUPDATEBOX_H
