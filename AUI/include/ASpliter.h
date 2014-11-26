#ifndef ASPLITER_H
#define ASPLITER_H

class AUI_API ASpliter: public AContainer
{
	CLASS_INFO(Spliter,Container);
public:
	ASpliter(AComponent* pOwner);
	virtual ~ASpliter();

	virtual void DoMouseMove(AEvent* pEvent);
	virtual void DoMouseDown(AEvent* pEvent);
	virtual void DoMouseUp(AEvent* pEvent);

	virtual void SetAlign(eAlign al);

protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	AControl* FindBoundControl();

	int	m_iOldPos;
	int	m_iOldLength;

};
#endif//ASPLITER_H
