#include "StdAfx.h"
//#include <Commdlg.h>
#include "AColorDialogForm.h"

AColorDialog::AColorDialog(AComponent* pOwner):AInvisibleControl(pOwner)
{
	m_FloatType = ftFloat;
}

AColorDialog::~AColorDialog()
{

}

void AColorDialog::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		AApplication::Get()->GetCurSkin()->DrawColorDialog(cs,this);
	}

}

bool AColorDialog::Execute()
{
	/*CHOOSECOLOR cc;
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = m_pUICore->GetHandle();
	cc.hInstance = NULL;
	cc.Flags = CC_ANYCOLOR | CC_RGBINIT;
	cc.rgbResult = RGB(m_Color.red,m_Color.green,m_Color.blue);
	COLORREF custColors[16];
	cc.lpCustColors = custColors;
	cc.lCustData=0;
	cc.lpfnHook = NULL;
	cc.lpTemplateName = NULL;
	if( ::ChooseColor(&cc) == TRUE )
	{
		m_Color = AColor(255,GetRValue(cc.rgbResult),GetGValue(cc.rgbResult),GetBValue(cc.rgbResult));
		return true;
	}
	return false;*/
	AColorDialogForm* pForm = new AColorDialogForm(this);
	pForm->CreateFromXML();

	TDialogButton db;
	pForm->m_nButtonID = &db;
	pForm->m_pColor = &m_Color;
	

	db = dbCancel;

	pForm->ShowModal();
	pForm = NULL;

	return db == dbOK ? true : false;
}

AColor AColorDialog::GetColor()
{
	return m_Color;
}

void AColorDialog::SetColor(AColor cr)
{
	m_Color = cr;
}
