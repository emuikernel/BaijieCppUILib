#include "StdAfx.h"
//#include <Commdlg.h>
#include "AFileDialogForm.h"

AFileOpenDialog::AFileOpenDialog(AComponent* pOwner):AInvisibleControl(pOwner)
{
	m_FloatType = ftFloat;
	m_sFilter=_T("所有文件(*.*)|*.*");
	m_sFileName = _T("");
	m_iFilterIndex = 0;
}

AFileOpenDialog::~AFileOpenDialog()
{
}

void AFileOpenDialog::SetFileName(const AString& sFileName)
{
	m_sFileName = sFileName;
}
AString AFileOpenDialog::GetFileName()
{
	return m_sFileName;
}

void AFileOpenDialog::SetFilter(const AString& sFilter)
{
	m_sFilter = sFilter;
}
AString AFileOpenDialog::GetFilter()
{
	return m_sFilter;
}
int AFileOpenDialog::GetFilterIndex()
{
	return m_iFilterIndex;
}

void AFileOpenDialog::SetFilterIndex(int index)
{
	m_iFilterIndex = index;
}

void AFileOpenDialog::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawFileOpenDialog(cs,this);
}



bool AFileOpenDialog::Execute()
{
	/*OPENFILENAME of;
	memset(&of,0,sizeof(OPENFILENAME));
	of.lStructSize = sizeof(OPENFILENAME);
	of.hwndOwner = m_pUICore->GetHandle();
	of.lpstrFilter = m_sFilter.Text();
	TCHAR szFileBuf[1024];
	if( m_sFileName != _T("") )
	{
		_tcscpy(szFileBuf,m_sFileName.Text());
	}
	else
		szFileBuf[0] = 0;
	of.lpstrFile = szFileBuf;
	of.nMaxFile = 1024-1;
	if( ::GetOpenFileName(&of) == TRUE )
	{
		m_sFileName = AString(szFileBuf);
		return true;
	}*/

	AFileDialogForm* pForm = new AFileDialogForm(this);
	pForm->CreateFromXML();

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


void AFileOpenDialog::SetInitDir(const AString& sInitDir)
{
	m_sInitDir = sInitDir;
}

AString AFileOpenDialog::GetInitDir()
{
	return m_sInitDir;
}