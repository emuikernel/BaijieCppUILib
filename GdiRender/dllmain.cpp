#include "stdafx.h"
#include "ACanvasGdi.h"
#include "GdiRender.h"

ACanvas* _OnCreateCanvas()
{
	ACanvasGdi* pCanvas = new ACanvasGdi();
	pCanvas->AddRef();
	return pCanvas;
}

GDIRENDER_API void RegGdiRender()
{
	ACanvas::RegCanvasFactory(_OnCreateCanvas);
}

HMODULE g_hInstance=NULL;
extern "C" __declspec(dllexport) BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		// attach to process
		// return FALSE to fail DLL load
		OutputDebugString(_T("\r\n-----º”‘ÿGdiRender---\r\n"));
		g_hInstance = hinstDLL;
		break;

	case DLL_PROCESS_DETACH:
		// detach from process
		OutputDebugString(_T("\r\n........–∂‘ÿGdiRender.....\r\n"));
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
