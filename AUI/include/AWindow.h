#ifndef AWINDOWFORM_H
#define AWINDOWFORM_H

class AUI_API AWindow : public AWindowBase
{
	CLASS_INFO(Window,WindowBase);
public:
	AWindow(AWinControlBase* pOwner);
	virtual ~AWindow();

	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	virtual DWORD GetClassStyle(DWORD dwOldStyle);
	virtual DWORD GetExStyle(DWORD dwOldExStyle);
	virtual AString GetWindowName();
	virtual AString GetClassName();
	virtual bool Create(HWND hParent = NULL,TWindowPos wp=wpDefault);
	virtual void Close();

	virtual void AddMsgExtend(AMsgExtend* pExt);

	virtual AWinControlBase* GetOwner();
	virtual HDC GetDC();

	virtual void SetPostQuitFlag(bool b);
protected:
	AWinControlBase*		m_pOwnerControl;
	AArray				m_aMsgExtend;
	ARect				m_rHint;//Hint显示位置
	AString				m_sHint;//需要显示的Hint
	bool				m_bPostQuit;
	HDC				m_hDC;

	virtual bool BeforeMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret);
	//消息处理后缀
	virtual void AfterMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret);
	//消息过滤处理
	virtual bool DealMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& ret);

	//====  消息分派
	inline LRESULT _ncdestroy(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _paint(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _lbuttondown(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _lbuttonup(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _erasebkground(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _size(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _move(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _mousemove(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _rbuttonup(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _mousewheel(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _mouseleave(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _killfocus(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _char(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _keydown(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _keyup(HWND hWnd,WPARAM wParam,LPARAM lParam);
	inline LRESULT _close(HWND hWnd,WPARAM wParam,LPARAM lParam);
};


#endif//AWINDOWFORM_H
