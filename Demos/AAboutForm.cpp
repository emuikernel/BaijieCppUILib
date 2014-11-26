#include "StdAfx.h"
#include "AAboutForm.h"
#include "ui/aboutForm.ui"

AAboutForm::AAboutForm(AComponent* pOwner):base_class(pOwner)
{
	MAP_CONTROL_INIT(OKButton);
}


AAboutForm::~AAboutForm(void)
{
}

AAboutForm* AAboutForm::CreateMe()
{
	afc_object<AMLBody> pBody(new AMLBody());
	AString xml;
	xml = g_szFormXML;
	xml.Replace(_T("@"),_T("\""));
	pBody->FromXML(xml);
	AAboutForm* p = dynamic_cast<AAboutForm*>( AControl::CreateInstance(&pBody,AApplication::Get()) );
	return p;
}

void AAboutForm::ConnectEvent()
{
	EVENT_CONNECT(GetOKButton(),OnClick,OnOK);
}
void AAboutForm::OnOK(AObject* pSender,AEvent* pEvent)
{
	Destroy();
}