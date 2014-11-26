#ifndef CAPTUREMSGEXTEND_H
#define CAPTUREMSGEXTEND_H
#include "AMsgExtend.h"

class AWindow;
class AUI_API ACaptureMsgExtend : public AMsgExtend
{
public:
	ACaptureMsgExtend();
	virtual ~ACaptureMsgExtend();
	ACaptureMsgExtend(AWindow* pWin);

	//消息处理前缀，如果返回false，则不进行后续处理
	virtual bool BeforeMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret);
	//消息处理后缀
	virtual void AfterMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret);
	//消息过滤处理
	virtual bool DealMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret);

private:
	AWindow*	m_pWindow;
};
#endif // CAPTUREMSGEXTEND_H
