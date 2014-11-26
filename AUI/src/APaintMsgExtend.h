#ifndef PAINTMSGEXTEND_H
#define PAINTMSGEXTEND_H
#include "AMsgExtend.h"

class AWindow;
class AUI_API APaintMsgExtend : public AMsgExtend
{
public:
	APaintMsgExtend();
	virtual ~APaintMsgExtend();
	APaintMsgExtend(AWindow* pWin);

	//消息处理前缀，如果返回false，则不进行后续处理
	virtual bool BeforeMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret);
	//消息处理后缀
	virtual void AfterMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret);
	//消息过滤处理
	virtual bool DealMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret);

private:
	AWindow*	m_pWindow;
};
#endif // PAINTMSGEXTEND_H
