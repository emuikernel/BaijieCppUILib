#include "stdafx.h"
#include "AInternalImageCache.h"
#include "resource.h"


AInternalImageCache* g_pInternalImageCache=NULL;

AImage::AImage(void):base_class()
{
}

AImage::~AImage(void)
{
}


void AInternalImageCache::LoadDefault()
{

}


static FOnCreateImage g_OnCreateImage = NULL;

void AImage::RegImageFactory(FOnCreateImage f)
{
	g_OnCreateImage = f;
}

AImage* AImage::CreateImage()
{
	if( g_OnCreateImage )
		return g_OnCreateImage();
	else
		return NULL;
}


AImage* AImage::GetImageByKey(const AString& sKey)
{
	return g_pInternalImageCache->GetImageByKey(sKey);
}

void AImage::SetImageByKey(const AString& sKey,AImage* pGraphic)
{
	g_pInternalImageCache->SetImageByKey(sKey,pGraphic);
}

void AImage::RemoveImageByKey(const AString& sKey)
{
	g_pInternalImageCache->RemoveImageByKey(sKey);
}

void AImage::RemoveAllImages()
{
	g_pInternalImageCache->Clear();
}