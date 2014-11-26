#ifndef ABITMAP_H
#define ABITMAP_H

class AFC_API ABitmap : public AGraphic
{
public:
	ABitmap(void);
	virtual ~ABitmap(void);

	virtual void Draw(ACanvas* pCanvas,ARect r);
	virtual void DrawTransparent(ACanvas* pCanvas,ARect r,AColor crTransparent);
	virtual bool IsValid();
	virtual int GetWidth();
	virtual int GetHeight();
	virtual void LoadFromFile(const AString& sFileName);
	virtual void LoadFromStream(void* pData,int iDataLen);
	virtual void LoadFromResource(HINSTANCE hInstance,int nResID);
protected:
	CBitmapEx	m_Bitmap;
};


#endif//ABITMAP_H
