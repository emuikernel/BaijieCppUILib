#ifndef ASLIDE_H
#define ASLIDE_H

class AUI_API ASlide: public AScrollBar
{
	CLASS_INFO(Slide,ScrollBar);
public:
	ASlide(AComponent* pOwner);
	virtual ~ASlide();

	virtual eScrollBarKind GetKind()=0;

	virtual int GetRadius();

	virtual ARect GetButton1Pos();
	virtual ARect GetButton2Pos();
	virtual ARect GetTopButton1Pos();//ÖÃ¶¥
	virtual ARect GetTopButton2Pos();//ÖÃµ×
	virtual ARect GetRailPos_1();
	virtual ARect GetRailPos_2();
	virtual ARect GetRailPos();
	virtual ARect GetSlideButtonPos();

	virtual int GetVirtualSize();
	virtual int GetTrueSize();
	virtual double GetRate();
	virtual int GetRailLen();
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);

};


class AUI_API AVertSlide: public ASlide
{
	CLASS_INFO(VertSlide,Slide);
public:
	AVertSlide(AComponent* pOwner);
	virtual ~AVertSlide();


	virtual eScrollBarKind GetKind(){ return m_Kind; }
protected:

};

class AUI_API AHorzSlide: public ASlide
{
	CLASS_INFO(HorzSlide,Slide);
public:
	AHorzSlide(AComponent* pOwner);
	virtual ~AHorzSlide();


	virtual eScrollBarKind GetKind(){ return m_Kind; }
protected:

};

#endif//ASLIDE_H
