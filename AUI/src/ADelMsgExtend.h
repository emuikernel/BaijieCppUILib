#ifndef DELMSGEXTEND_H
#define DELMSGEXTEND_H
#include "AMsgExtend.h"

class AWindow;
class AUI_API ADelMsgExtend : public AMsgExtend
{
public:
	ADelMsgExtend();
	virtual ~ADelMsgExtend();
	ADelMsgExtend(AWindow* pWin);

	//消息处理前缀，如果返回false，则不进行后续处理
	virtual bool BeforeMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret);
	//消息处理后缀
	virtual void AfterMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret);
	//消息过滤处理
	virtual bool DealMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret);

private:
	AWindow*	m_pWindow;
};
#endif // DELMSGEXTEND_H
