#include "stdafx.h"
#include "AMessageForm.h"

AMessageBox::AMessageBox():base_class()
{

}

AMessageBox::~AMessageBox()
{
	//
}
void AMessageBox::Infor(const AString& sText,TDialogButton db)
{
	AMessageForm* pForm = new AMessageForm(AApplication::Get());
	pForm->CreateFromXML(db);


	TDialogButton dbRet;
	pForm->m_nButtonID = &dbRet;
	pForm->m_iMessageType = MESSAGE_TYPE_INFOR;
	pForm->m_sUserText = sText;
	dbRet = dbCancel;

	pForm->ShowModal();
	pForm = NULL;
}
void AMessageBox::Warning(const AString& sText,TDialogButton db)
{
	AMessageForm* pForm = new AMessageForm(AApplication::Get());
	pForm->CreateFromXML(db);


	TDialogButton dbRet;
	pForm->m_nButtonID = &dbRet;
	pForm->m_iMessageType = MESSAGE_TYPE_WARN;
	pForm->m_sUserText = sText;
	dbRet = dbCancel;

	pForm->ShowModal();
	pForm = NULL;
}
void AMessageBox::Error(const AString& sText,TDialogButton db)
{
	AMessageForm* pForm = new AMessageForm(AApplication::Get());
	pForm->CreateFromXML(db);


	TDialogButton dbRet;
	pForm->m_nButtonID = &dbRet;
	pForm->m_iMessageType = MESSAGE_TYPE_ERROR;
	pForm->m_sUserText = sText;
	dbRet = dbCancel;

	pForm->ShowModal();
	pForm = NULL;
}
void AMessageBox::Wait(const AString& sText,int nMS,TDialogButton db)
{

}
void AMessageBox::Wait(const AString& sText,TWaitBoxCallBack wcb,TDialogButton db)
{

}
TDialogButton AMessageBox::Question(const AString& sText,TDialogButton db)
{
	AMessageForm* pForm = new AMessageForm(AApplication::Get());
	pForm->CreateFromXML(db);


	TDialogButton dbRet;
	pForm->m_nButtonID = &dbRet;
	pForm->m_iMessageType = MESSAGE_TYPE_QUESTION;
	pForm->m_sUserText = sText;
	dbRet = dbCancel;

	pForm->ShowModal();
	pForm = NULL;
	return dbRet;
}

TDialogButton AMessageBox::Input(const AString& sTitle,const AString& sHint,AString& sInput)
{
	AMessageForm* pForm = new AMessageForm(AApplication::Get());
	pForm->CreateFromXML( (TDialogButton)(dbOK | dbCancel));


	TDialogButton dbRet;
	pForm->m_nButtonID = &dbRet;
	pForm->m_iMessageType = MESSAGE_TYPE_INPUT;
	pForm->m_sUserText = sHint;
	pForm->m_sInputText = &sInput;
	dbRet = dbCancel;

	pForm->SetText( sTitle );
	pForm->ShowModal();
	pForm = NULL;

	return dbRet;
}