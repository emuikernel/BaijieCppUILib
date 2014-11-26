#ifndef APAGECONTROL_H
#define APAGECONTROL_H

class AUI_API APageControl: public AContainer
{
	CLASS_INFO(PageControl,Container);
public:
	APageControl(AComponent* pOwner);
	virtual ~APageControl();

	virtual void DoLayout();

	APageSheet* GetActiveSheet();
	void SetActiveSheet(APageSheet* pSheet);

	int GetActiveSheetIndex();
	void SetActiveSheetIndex(int index);

	APageSheet* AddSheet();
	void RemoveSheet(APageSheet* pSheet);

	bool GetEnableScroll();
	void SetEnableScroll(bool v);

	bool GetEnableScrollTop();
	void SetEnableScrollTop(bool v);

	//对ES的支持
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void CreateChildren(AMLBody* pNode);

	AEventHandler		OnActiveChange;
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	int	m_iActiveIndex;
	bool	m_bEnableScroll;
	bool	m_bEnableScrollTop;
	int		m_iShift;//偏移量（单位：个，代表偏移了多少个Sheet）
	AButton*	m_pButtonPrev;
	AButton*	m_pButtonNext;
	AButton*	m_pButtonFirst;
	AButton*	m_pButtonLast;
};
#endif//APAGECONTROL_H
