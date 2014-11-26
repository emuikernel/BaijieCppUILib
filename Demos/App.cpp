#include "StdAfx.h"
#include "MainForm.h"
#include "AAboutForm.h"

#include "AggRender.h"



#ifdef _DEBUG
#pragma comment(lib, "AggRenderd.lib")
#else
#pragma comment(lib,"AggRender.lib")
#endif



#include "ImageFactory.h"

#ifdef _DEBUG
#pragma comment(lib, "ImageFactoryd.lib")
#else
#pragma comment(lib,"ImageFactory.lib")
#endif

//--- Àà´´½¨Æ÷
AObject* MainForm_ControlCreator(AComponent* pOwner)
{
	return new AMainForm(pOwner);
}

AObject* AAboutForm_ControlCreator(AComponent* pOwner)
{
	return new AAboutForm(pOwner);
}

void RegClasses()
{
	AApplication::Get()->RegisterControlCreator(L"AMainForm",MainForm_ControlCreator);
	AApplication::Get()->RegisterControlCreator(L"AAboutForm",AAboutForm_ControlCreator);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	try
	{
		AApplication::Get()->SetHandle(hInstance);
		RegClasses();

		Use_ImageFactory();

		Use_AggRender();


		AApplication::Get()->Init();

		AMainForm::CreateMe()->Show();


		AApplication::Get()->Run();

	}
	catch(AException e)
	{
		AMessageBox::Error(e.GetDesc());
	}

	return 0;
}
