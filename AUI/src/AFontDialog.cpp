#include "StdAfx.h"
#include "AFontForm.h"

AFontDialog::AFontDialog(AComponent* pOwner):AInvisibleControl(pOwner)
{
	m_FloatType = ftFloat;
}

AFontDialog::~AFontDialog()
{

}

void AFontDialog::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawFontDialog(cs,this);
}

bool AFontDialog::Execute()
{
	AFontForm* pForm = new AFontForm(this);
	pForm->CreateFromXML();


	TDialogButton db;
	pForm->m_nButtonID = &db;
	pForm->m_pFont = &SelectedFont;
	db = dbCancel;

	pForm->ShowModal();
	pForm = NULL;

	return db == dbOK ? true : false;
}

