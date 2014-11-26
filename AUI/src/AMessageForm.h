#ifndef AMESSAGEFORM_H
#define AMESSAGEFORM_H

class AMessageForm : public AForm
{
	CLASS_INFO(MessageForm,Form);
public:
	AMessageForm(AComponent* pOwner);
	virtual ~AMessageForm();

	virtual void InitControls();
	virtual void ConnectEvent();

	virtual void DoCreate();
	void CreateFromXML(TDialogButton db);

	MAP_CONTROL(AContainer,Content);
	MAP_CONTROL(AContainer,BottomContainer);

	TDialogButton *m_nButtonID;
	int m_iMessageType;
	AString m_sUserText;
	AString* m_sInputText;
private:
	TDialogButton	m_dbs;
	void OnOKClick(AObject* pSender,AEvent* pEvent);
	void OnCancelClick(AObject* pSender,AEvent* pEvent);
	void OnYesClick(AObject* pSender,AEvent* pEvent);
	void OnNoClick(AObject* pSender,AEvent* pEvent);

	AButton*	m_pOKButton;
	AButton*	m_pCancelButton;
	AButton*	m_pYesButton;
	AButton*	m_pNoButton;
};

#endif//AMESSAGEFORM_H