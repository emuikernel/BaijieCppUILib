#ifndef AIMAGE_H
#define AIMAGE_H

enum Image_Type
{
	IMAGE_BMP,
	IMAGE_JPG,
	IMAGE_PNG
};
class AImage;
typedef AImage* (*FOnCreateImage)();

class AUI_API AImage : public AObject
{
	CLASS_INFO(Image,Object);
public:
	AImage(void);
	virtual ~AImage(void);

	virtual bool IsValid()=0;
	virtual int GetWidth()=0;
	virtual int GetHeight()=0;
	virtual int GetStride()=0;
	virtual void LoadFromFile(const AString& sFileName)=0;
	virtual void LoadFromResource(HINSTANCE hInstance,int nResID,Image_Type it,const ACHAR* ResType)=0;
	virtual void LoadIcon(HICON h)=0;
	virtual void LoadBitmap(HBITMAP h)=0;
	virtual void* GetPixels()=0;

	static void RegImageFactory(FOnCreateImage f);
	static AImage* CreateImage();
	static AImage* GetImageByKey(const AString& sKey);
	static void SetImageByKey(const AString& sKey,AImage* pGraphic);
	static void RemoveImageByKey(const AString& sKey);
	static void RemoveAllImages();
};


#endif//AIMAGE_H