#include "StdAfx.h"
//#include <Commdlg.h>
#include "ADirectoryForm.h"

ADirectoryDialog::ADirectoryDialog(AComponent* pOwner):AInvisibleControl(pOwner)
{
	m_FloatType = ftFloat;
	m_sDirectory = _T("");
}

ADirectoryDialog::~ADirectoryDialog()
{
}

void ADirectoryDialog::SetDirectory(const AString& sDirectory)
{
	m_sDirectory = sDirectory;
}
AString ADirectoryDialog::GetDirectory()
{
	return m_sDirectory;
}

void ADirectoryDialog::DoPaint(ACanvas* cs,AEvent* pEvent)
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


bool ADirectoryDialog::Execute()
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

	ADirectoryForm* pForm = new ADirectoryForm(this);
	pForm->m_sInitDir = m_sDirectory;
	pForm->CreateFromXML();


	TDialogButton db;
	pForm->m_nButtonID = &db;
	pForm->m_sDirectory = &m_sDirectory;
	db = dbCancel;
	

	pForm->ShowModal();
	pForm = NULL;

	return db == dbOK ? true : false;
}

