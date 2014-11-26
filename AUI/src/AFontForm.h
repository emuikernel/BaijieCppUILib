#ifndef AFONTFORM_H
#define AFONTFORM_H


class AFontForm : public AForm
{
	CLASS_INFO(FontForm,Form);
public:
	AFontForm(AComponent* pOwner);
	virtual ~AFontForm();

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
	MAP_CONTROL(AListBox,FontListBox);
	MAP_CONTROL(AEdit,FontEdit);
	MAP_CONTROL(AEdit,FontStyleEdit);
	MAP_CONTROL(AListBox,FontStyleListBox);
	MAP_CONTROL(AEdit,FontSizeEdit);
	MAP_CONTROL(AListBox,FontSizeListBox);
	MAP_CONTROL(AContainer,Demo);
	MAP_CONTROL(AButton,ColorSelector);


	TDialogButton *m_nButtonID;
	AFont*		m_pFont;
	AFont		m_Font;
private:
	void OnFontItemPaint(AObject* pSender,AEvent* pEvent);
	void OnFontNameClick(AObject* pSender,AEvent* pEvent);
	void OnFontStyleClick(AObject* pSender,AEvent* pEvent);
	void OnFontSizeClick(AObject* pSender,AEvent* pEvent);
	void OnDemoPaint(AObject* pSender,AEvent* pEvent);
	void OnColorSelectorPaint(AObject* pSender,AEvent* pEvent);
	void OnColorSelectorClick(AObject* pSender,AEvent* pEvent);

	void FillFontStyle();
	void FillFontSize();
};

#endif//AFONTFORM_H