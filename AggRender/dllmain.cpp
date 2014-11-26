#include "stdafx.h"
#include "agg.h"
#include "AggRender.h"
#include "ACanvasAgg.h"

ACanvas* _OnCreateCanvas()
{
	ACanvasAgg* pCanvas = new ACanvasAgg();
	pCanvas->AddRef();
	return pCanvas;
}

AGGRENDER_API void RegAggRender()
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
		g_hInstance = hinstDLL;
		break;

	case DLL_PROCESS_DETACH:
		// detach from process
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
