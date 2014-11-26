#include "StdAfx.h"
#include "resource.h"
#include "MainForm.h"
#include "AAboutForm.h"
#include "ui/MainForm.ui"

AMainForm* AMainForm::CreateMe()
{
	afc_object<AMLBody> pBody(new AMLBody());
	AString xml;
	xml = g_szFormXML;
	xml.Replace(_T("@"),_T("\""));
	pBody->FromXML(xml);
	AMainForm* p = dynamic_cast<AMainForm*>( AControl::CreateInstance(&pBody,AApplication::Get()) );
	return p;
}

AMainForm::AMainForm(AComponent* pOwner):base_class(pOwner)
{
	MAP_CONTROL_INIT(mnAbout);
	MAP_CONTROL_INIT(MenuBar);
	MAP_CONTROL_INIT(TitleBar);
	MAP_CONTROL_INIT(EditText);
}

AMainForm::~AMainForm()
{

}

void AMainForm::DoCreate()
{
	SetIcon(::LoadIcon(AApplication::Get()->GetHandle(), MAKEINTRESOURCE(IDI_ICON1)));

}

void AMainForm::ConnectEvent()
{
	base_class::ConnectEvent();

	EVENT_CONNECT(GetmnAbout(),OnClick,OnAboutClick);
	EVENT_CONNECT(GetMenuBar(),OnMouseDown,OnMenuBarMouseDown);
	EVENT_CONNECT(GetMenuBar(),OnMouseMove,OnMenuBarMouseMove);
}

void AMainForm::OnAboutClick(AObject* pSender,AEvent* pEvent)
{
	AAboutForm* p = AAboutForm::CreateMe();
	p->CenterToScreen();
	p->ShowModal();
}

void AMainForm::OnMenuBarMouseDown(AObject* pSender,AEvent* pEvent)
{
	GetTitleBar()->DoMouseDown(pEvent);
}

void AMainForm::OnMenuBarMouseMove(AObject* pSender,AEvent* pEvent)
{
	GetTitleBar()->DoMouseMove(pEvent);
}