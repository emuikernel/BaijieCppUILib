#ifndef ATIMER_H
#define ATIMER_H

typedef void (AObject::*FOnTimerFunc)(AObject* pAsso,int nTimerID);

class AUI_API ATimer : public AInvisibleControl
{
	CLASS_INFO(Timer,InvisibleControl);
public:
	ATimer(AComponent* pOwner);
	virtual ~ATimer();

	//nInterval ： 定时器间隔时间，单位是毫秒
	//pEvtObject和evtFunc是当时间到时要执行的类成员函数
	//pAsso是与定时器关联的对象
	static int StartTimer(UINT nInterval,AObject* pEvtObject,FOnTimerFunc evtFunc,AObject* pAsso);
	static void EndTimer(int nTimerID);
};

#endif//ATIMER_H