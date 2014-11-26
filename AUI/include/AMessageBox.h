#ifndef AMESSAGEBOX_H
#define AMESSAGEBOX_H

typedef bool (*TWaitBoxCallBack)(void* pArg);//返回false表示结束等待

class AUI_API AMessageBox : public AObject
{
	CLASS_INFO(MessageBox,Object);
public:
	AMessageBox();
	virtual ~AMessageBox();

	static void Infor(const AString& sText,TDialogButton db=dbOK);
	static void Warning(const AString& sText,TDialogButton db=dbOK);
	static void Error(const AString& sText,TDialogButton db=dbOK);
	static void Wait(const AString& sText,int nMS,TDialogButton db=dbOK);
	static void Wait(const AString& sText,TWaitBoxCallBack wcb,TDialogButton db=dbOK);
	static TDialogButton Question(const AString& sText,TDialogButton db=(TDialogButton)(dbYes | dbNo | dbCancel));
	static TDialogButton Input(const AString& sTitle,const AString& sHint,AString& sInput);
};

#endif//AMESSAGEBOX_H