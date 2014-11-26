#ifndef ACOLORDIALOGFORM_H
#define ACOLORDIALOGFORM_H


class AColorDialogForm : public AForm
{
	CLASS_INFO(ColorDialogForm,Form);
public:
	AColorDialogForm(AComponent* pOwner);
	virtual ~AColorDialogForm();

	virtual void InitControls();
	virtual void ConnectEvent();
	virtual void TitleBackDoMouseDown(AObject* pSender,AEvent* pEvent);
	virtual void TitleBackDoMouseMove(AObject* pSender,AEvent* pEvent);
	virtual void OnOKClick(AObject* pSender,AEvent* pEvent);
	virtual void OnCancelClick(AObject* pSender,AEvent* pEvent);

	virtual void DoCreate();
	void CreateFromXML();

	

	MAP_CONTROL(AButton,OK);
	MAP_CONTROL(AButton,Cancel);

	MAP_CONTROL(AContainer,ButtonsPanel);
	MAP_CONTROL(ATitleBar,TitleBar);
	MAP_CONTROL(AVertSlide,ColorSelector);
	MAP_CONTROL(AContainer,SLSelector);
	MAP_CONTROL(AContainer,BaseColorContainer);
	MAP_CONTROL(ALabel,HexColor);
	MAP_CONTROL(AHorzSlide,RSlide);
	MAP_CONTROL(AHorzSlide,GSlide);
	MAP_CONTROL(AHorzSlide,BSlide);
	MAP_CONTROL(AEdit,REdit);
	MAP_CONTROL(AEdit,GEdit);
	MAP_CONTROL(AEdit,BEdit);
	MAP_CONTROL(AHorzSlide,HSlide);
	MAP_CONTROL(AHorzSlide,SSlide);
	MAP_CONTROL(AHorzSlide,LSlide);
	MAP_CONTROL(AEdit,HEdit);
	MAP_CONTROL(AEdit,SEdit);
	MAP_CONTROL(AEdit,LEdit);


	TDialogButton *m_nButtonID;
	AColor*		m_pColor;

private:
	AColor		m_Color;
	AColorHSL	m_HSLColor;

	bool		m_bDisableChange;//½ûÖ¹´¥·¢OnChange

	void InitColorControl();

	void OnColorSelectorPaint(AObject* pSender,AEvent* pEvent);
	void OnColorSelectorMouseDown(AObject* pSender,AEvent* pEvent);
	void OnSLPaint(AObject* pSender,AEvent* pEvent);
	void OnSLMouseDown(AObject* pSender,AEvent* pEvent);
	void OnSLMouseMove(AObject* pSender,AEvent* pEvent);
	void OnColorScroll(AObject* pSender,AEvent* pEvent);
	void OnBaseColorButtonPaint(AObject* pSender,AEvent* pEvent);
	void OnColorDiffButtonPaint(AObject* pSender,AEvent* pEvent);
	void OnEditChange(AObject* pSender,AEvent* pEvent);
	void OnSlideChange(AObject* pSender,AEvent* pEvent);
	void OnBaseColorClick(AObject* pSender,AEvent* pEvent);

	void _initSlide(AHorzSlide* pSlide,int iRange);

	void _updateRGB();
	void _updateHSL();
	void _updateHSLSlide();
};

#endif//ACOLORDIALOGFORM_H