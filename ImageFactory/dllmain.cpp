#include "stdafx.h"
#include "ImageFactory.h"
#include "AMyImage.h"

AImage* _OnCreateImage()
{
	AImage* p = new AMyImage();
	return p;
}

IMAGEFACTORY_API void RegImageFactory()
{
	AImage::RegImageFactory(_OnCreateImage);
}

HMODULE g_hInstance=NULL;
extern "C" __declspec(dllexport) BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		// attach to process
		// return FALSE to fail DLL load
		g_hInstance = hinstDLL;
		OutputDebugString(_T("\r\n-----º”‘ÿImageFactory---\r\n"));
		break;

	case DLL_PROCESS_DETACH:
		// detach from process
		OutputDebugString(_T("\r\n......–∂‘ÿImageFactory.......\r\n"));
		break;

	case DLL_THREAD_ATTACH:
		// attach to thread
		break;

	case DLL_THREAD_DETACH:
		// detach from thread
		break;
	}
	return TRUE; // succesful
}
