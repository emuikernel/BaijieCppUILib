#include "stdafx.h"
#include "SkiaRender.h"
#include "ACanvasSkia.h"

ACanvas* _OnCreateCanvas()
{
	ACanvasSkia* pCanvas = new ACanvasSkia();
	pCanvas->AddRef();
	return pCanvas;
}

SKIARENDER_API void RegSkiaRender()
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
		OutputDebugString(_T("\r\n-----º”‘ÿSkiaRender---\r\n"));
		g_hInstance = hinstDLL;
		break;

	case DLL_PROCESS_DETACH:
		// detach from process
		OutputDebugString(_T("\r\n........–∂‘ÿSkiaRender.....\r\n"));
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
