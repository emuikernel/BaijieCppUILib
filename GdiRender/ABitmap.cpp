
#include "stdafx.h"


ABitmap::ABitmap(void) : AGraphic()
{
}

ABitmap::~ABitmap(void)
{
}

void ABitmap::Draw(ACanvas* pCanvas,ARect r)
{
	if( m_Bitmap.IsValid() !=  TRUE ) throw AException(_T("Bitmap is not valid"));
	long horizontalPercent=100;
	long verticalPercent=100;
	if( m_Bitmap.GetWidth() != r.GetWidth() ) horizontalPercent = r.GetWidth() * 100 / m_Bitmap.GetWidth();
	if( m_Bitmap.GetHeight() != r.GetHeight() ) verticalPercent = r.GetHeight() * 100 / m_Bitmap.GetHeight();
	m_Bitmap.Scale(horizontalPercent,verticalPercent);
	m_Bitmap.Draw(pCanvas->Handle,r.left,r.top);
}
void ABitmap::DrawTransparent(ACanvas* pCanvas,ARect r,AColor crTransparent)
{
	if( m_Bitmap.IsValid() !=  TRUE ) throw AException(_T("Bitmap is not valid"));
	m_Bitmap.DrawTransparent(r.left,r.top,r.GetWidth(),r.GetHeight(),m_Bitmap,r.left,r.top,(COLORREF)crTransparent);
}
bool ABitmap::IsValid()
{
	return m_Bitmap.IsValid()== TRUE ? true : false;
}
int ABitmap::GetWidth()
{
	return m_Bitmap.GetWidth();
}
int ABitmap::GetHeight()
{
	return m_Bitmap.GetHeight();
}
void ABitmap::LoadFromFile(const AString& sFileName)
{
	m_Bitmap.Load((LPTSTR)sFileName.Text());
}
void ABitmap::LoadFromStream(void* pData,int iDataLen)
{
	m_Bitmap.Load((LPBYTE)pData);//fix me later
}
void ABitmap::LoadFromResource(HINSTANCE hInstance,int nResID)
{
	HBITMAP h = (HBITMAP)::LoadImage(hInstance,MAKEINTRESOURCE(nResID),IMAGE_BITMAP,0,0,0);
	m_Bitmap.Load(h);
	::DeleteObject( (HGDIOBJ)h );
}

