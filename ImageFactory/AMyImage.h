#ifndef AMYIMAGE_H
#define AMYIMAGE_H
#include "ximage.h"

class AMyImage : public AImage
{
	CLASS_INFO(MyImage,Image);
public:
	AMyImage();
	virtual ~AMyImage();

	virtual bool IsValid();
	virtual int GetWidth();
	virtual int GetHeight();
	virtual int GetStride();
	virtual void LoadFromFile(const AString& sFileName);
	virtual void LoadFromResource(HINSTANCE hInstance,int nResID,Image_Type it,const ACHAR* ResType);
	virtual void LoadIcon(HICON h);
	virtual void LoadBitmap(HBITMAP h);
	virtual void* GetPixels();
private:
	CxImage		m_Image;

	AMemory		m_pBits;

	void ConvertBits();
	void ToTransparent();
};
#endif//AMYIMAGE_H