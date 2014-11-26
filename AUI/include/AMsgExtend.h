#ifndef MSGEXTEND_H
#define MSGEXTEND_H

class AUI_API AMsgExtend
{
public:
	AMsgExtend();
	virtual ~AMsgExtend();

	//消息处理前缀，如果返回false，则不进行后续处理
	virtual bool BeforeMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)=0;
	//消息处理后缀
	virtual void AfterMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)=0;
	//消息过滤处理
	virtual bool DealMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret)=0;
};

#endif // MSGEXTEND_H
