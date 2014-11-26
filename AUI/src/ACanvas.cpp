
#include "stdafx.h"

ACanvas::ACanvas():AObject()
{
}

ACanvas::~ACanvas()
{
}

static FOnCreateCanvas g_OnCreateCanvas = NULL;
void ACanvas::RegCanvasFactory(FOnCreateCanvas f)
{
	g_OnCreateCanvas = f;
}
ACanvas* ACanvas::CreateCanvas()
{
	if( g_OnCreateCanvas )
		return g_OnCreateCanvas();
	else
		return NULL;
}