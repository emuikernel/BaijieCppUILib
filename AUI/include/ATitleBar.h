#ifndef TITLEBAR_H
#define TITLEBAR_H

class AUI_API ATitleBar : public AContainer
{
	CLASS_INFO(TitleBar,Container);
public:
	ATitleBar(AComponent* pOwner);
	virtual ~ATitleBar();

	virtual void DoMouseDown(AEvent* pEvent);
	virtual void DoMouseMove(AEvent* pEvent);
	virtual void DoMouseUp(AEvent* pEvent);
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void DoDblClick(AEvent* pEvent);

	virtual void SetBackgroundImage(AImage* p);
	virtual AImage* GetBackgroundImage();
protected:
	AImage*	m_pBackGroundImage;
};
#endif//TITLEBAR_H
