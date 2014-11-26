#include "StdAfx.h"
//#include <Commdlg.h>
#include "AFileDialogForm.h"

AFileSaveDialog::AFileSaveDialog(AComponent* pOwner):AInvisibleControl(pOwner)
{
	m_FloatType = ftFloat;
	m_sFilter=_T("所有文件(*.*)|*.*");
	m_sFileName = _T("");
	m_iFilterIndex = 0;
}

AFileSaveDialog::~AFileSaveDialog()
{

}

void AFileSaveDialog::SetFileName(const AString& sFileName)
{
	m_sFileName = sFileName;
}
AString AFileSaveDialog::GetFileName()
{
	return m_sFileName;
}

void AFileSaveDialog::SetInitDir(const AString& sInitDir)
{
	m_sInitDir = sInitDir;
}

AString AFileSaveDialog::GetInitDir()
{
	return m_sInitDir;
}

void AFileSaveDialog::SetFilter(const AString& sFilter)
{
	m_sFilter = sFilter;
}
AString AFileSaveDialog::GetFilter()
{
	return m_sFilter;
}
int AFileSaveDialog::GetFilterIndex()
{
	return m_iFilterIndex;
}

void AFileSaveDialog::SetFilterIndex(int index)
{
	m_iFilterIndex = index;
}

void AFileSaveDialog::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawFileSaveDialog(cs,this);
}

bool AFileSaveDialog::Execute()
{
	AFileDialogForm* pForm = new AFileDialogForm(this);
	pForm->CreateFromXML();

	pForm->m_bOpenMode = false;

	pForm->SetFilter(m_sFilter,m_iFilterIndex);
	pForm->SetText( GetText() );
	pForm->m_sInitDir = m_sInitDir;

	TDialogButton db;
	pForm->m_nButtonID = &db;
	pForm->m_sFileName = &m_sFileName;
	db = dbCancel;

	pForm->ShowModal();
	pForm = NULL;

	return db == dbOK ? true : false;
}

