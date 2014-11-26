#ifndef AWINDOWBASE_H
#define AWINDOWBASE_H

/*
 * AWindowBase ： 封装微软视窗系统中的Windows概念
 */
class AUI_API AWindowBase : public AObject
{
	CLASS_INFO(WindowBase,Object);
public:
	AWindowBase();
	virtual ~AWindowBase();

	static LRESULT CALLBACK msgRouter(HWND, UINT, WPARAM, LPARAM);

	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM) = 0;
	virtual DWORD GetClassStyle(DWORD dwOldStyle)=0;
	virtual DWORD GetExStyle(DWORD dwOldExStyle)=0;
	virtual AString GetWindowName()=0;
	virtual AString GetClassName()=0;
	virtual AString GetSuperClassName();
	virtual bool RegisterSupperClass();
	virtual bool RegisterClass();
	virtual void Subclass();
	virtual void Close();

	virtual bool Create (HWND hParent=NULL,TWindowPos wp=wpDefault);
	virtual HWND GetHandle(){ return m_hWnd; }

	virtual LRESULT SendMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	
protected:
	HWND	m_hWnd;

	WNDPROC	m_OldProc;
	
};

#endif//AWINDOWBASE_H