#include "StdAfx.h"
#include "MainForm.h"
#include "AAboutForm.h"

#define USE_AGG
//#define USE_SKIA

#ifdef USE_AGG
#include "AggRender.h"
#else
#include "SkiaRender.h"
#endif

#ifdef USE_AGG
#ifdef _DEBUG
#pragma comment(lib, "AggRenderd.lib")
#else
#pragma comment(lib,"AggRender.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "SkiaRenderd.lib")
#else
#pragma comment(lib,"SkiaRender.lib")
#endif
#endif

#include "ImageFactory.h"

#ifdef _DEBUG
#pragma comment(lib, "ImageFactoryd.lib")
#else
#pragma comment(lib,"ImageFactory.lib")
#endif



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	try
	{
		AApplication::Get()->SetHandle(hInstance);
		

		Use_ImageFactory();
#ifdef USE_AGG
		Use_AggRender();
#else
		Use_SkiaRender();
#endif


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
