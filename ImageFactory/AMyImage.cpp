#include "stdafx.h"
#include "AMyImage.h"

AMyImage::AMyImage():base_class()
{

}

AMyImage::~AMyImage()
{
	//
}

bool AMyImage::IsValid()
{
	return m_Image.IsValid();
}
int AMyImage::GetWidth()
{
	return m_Image.GetWidth();
}
int AMyImage::GetHeight()
{
	return m_Image.GetHeight();
}
int AMyImage::GetStride()
{
	return GetWidth()*4;
}

void AMyImage::LoadFromFile(const AString& sFileName)
{
	m_Image.Load(sFileName.Text());
	ConvertBits();
	ToTransparent();
}

void AMyImage::LoadFromResource(HINSTANCE hInstance,int nResID,Image_Type it,const ACHAR* ResType)
{
	HRSRC h = FindResource(hInstance,MAKEINTRESOURCE(nResID), ResType);
	int nFormat = 0;
	if( it == IMAGE_BMP ) nFormat = CXIMAGE_FORMAT_BMP;
	else if( it == IMAGE_JPG ) nFormat = CXIMAGE_FORMAT_JPG;
	else if( it == IMAGE_PNG ) nFormat = CXIMAGE_FORMAT_PNG;
	m_Image.LoadResource(h,nFormat,hInstance);
	ConvertBits();
	ToTransparent();
}

void AMyImage::LoadIcon(HICON hIcon)
{
	m_Image.CreateFromHICON(hIcon);
	ConvertBits();
	ToTransparent();
}

void* AMyImage::GetPixels()
{
	return m_pBits.GetData();
}

void AMyImage::ConvertBits()
{
	BYTE* pBits = m_Image.GetBits();
	int w = GetWidth();
	int h = GetHeight();

	m_pBits.SetSize( h * w * 4 );
	BYTE* p = m_pBits.GetData();
	int x=3;
	switch( m_Image.GetBpp() )
	{
	case 24: x = 3; break;
	case 32: x = 4; break;
	default: return;
	}
	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			int r1 = (h-1-i) * w * 4;
			int r2 = i * w * x;
			p[r1+j*4] = pBits[r2+j*x];
			if( x > 1 ) p[r1+j*4+1] = pBits[r2+j*x+1];
			if( x > 2 ) p[r1+j*4+2] = pBits[r2+j*x+2];

			if( x > 3 ) p[r1+j*4+3] = pBits[r2+j*x+3];
			else p[r1 + j *4+3]=255;
		}
	}
}

void AMyImage::LoadBitmap(HBITMAP hBmp)
{
	m_Image.CreateFromHBITMAP(hBmp);
	ConvertBits();
	ToTransparent();
}

void AMyImage::ToTransparent()
{
	//转换透明色
	int w = GetWidth();
	int h = GetHeight();
	BYTE* p = m_pBits.GetData();
	if( w > 1 && h > 1 )
	{
		BYTE p4[4];
		for(int i=0;i<h;i++)
		{
			for(int j=0;j<w;j++)
			{
				int r1 = i * w * 4;
				if( i==0 && j==0)
				{
					//保存首个像素
					p4[0] = p[r1]; p4[1] = p[r1+1]; p4[2] = p[r1+2];p4[3] =p[r1+3];
					p[r1+3] = 0;
				}
				else
				{
					if( p4[0] == p[r1+j*4] && p4[1] == p[r1+j*4+1] && p4[2] == p[r1+j*4+2] && p4[3] == p[r1+j*4+3] )
					{
						p[r1+j*4+3] = 0;//设为透明
					}
				}
			}
		}
	}
}