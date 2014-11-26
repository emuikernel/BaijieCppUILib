#pragma once


class AMainForm : public AForm
{
	CLASS_INFO(MainForm,Form);
public:
	AMainForm(AComponent* pOwner);
	virtual ~AMainForm();

	static AMainForm* CreateMe();

	virtual void ConnectEvent();

	MAP_CONTROL(AMenuItem,mnAbout);
	MAP_CONTROL(ATitleBar,TitleBar);
	MAP_CONTROL(AMenuBar,MenuBar);
	MAP_CONTROL(AEdit,EditText);



	void OnAboutClick(AObject* pSender,AEvent* pEvent);
	void OnMenuBarMouseDown(AObject* pSender,AEvent* pEvent);
	void OnMenuBarMouseMove(AObject* pSender,AEvent* pEvent);
protected:
	virtual void DoCreate();


};

