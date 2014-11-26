#pragma once
class AAboutForm : public AForm
{
	CLASS_INFO(AboutForm,Form);
public:
	AAboutForm(AComponent* pOwner);
	virtual ~AAboutForm(void);

	static AAboutForm* CreateMe();

	virtual void ConnectEvent();

	MAP_CONTROL(AButton,OKButton);

	void OnOK(AObject* pSender,AEvent* pEvent);
};

