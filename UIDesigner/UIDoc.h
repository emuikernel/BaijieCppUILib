//---------------------------------------------------------------------------

#ifndef UIDocH
#define UIDocH

#include "uicontrol.h"
#include "document.h"

#define HANDLE_WIDTH	5

//句柄类型定义
#define HANDLE_DUMMY	0
#define HANDLE_INME	1
#define HANDLE_LEFTTOP	2
#define HANDLE_TOP	3
#define HANDLE_RIGHTTOP	4
#define HANDLE_RIGHT	5
#define HANDLE_RIGHTBOTTOM	6
#define HANDLE_BOTTOM	7
#define HANDLE_LEFTBOTTOM	8
#define HANDLE_LEFT	9

class THitResult
{
public:
	THitResult()
	{
		m_pBase = ANULL;
		m_iHandle = HANDLE_DUMMY;
	}
	THitResult(const afc_object<CControl>& pBase,int iHandle)
	{
		m_pBase = pBase;
		m_iHandle = iHandle;
	}
	THitResult& operator=(const THitResult& r)
	{
		m_pBase = r.m_pBase;
		m_iHandle = r.m_iHandle;
		return *this;
	}
	~THitResult()
	{
	}
	int					m_iHandle;
	afc_object<CControl>	m_pBase;
};


typedef std::map<CControl*,bool>		SELECTED_TYPE;

class CUIDoc : public IDocument
{
public:
	CUIDoc();
	virtual ~CUIDoc();

	afc_object<CControl>		GetWebForm(){ return m_pWebForm; }
	void				SetWebForm(const afc_object<CControl>& p){ m_pWebForm = p; }

	bool				IsMoving(){ return (m_ptDown.x > -1 && m_ptDown.y > -1 && ( m_ptDown.x != m_ptMove.x || m_ptDown.y != m_ptMove.y ) ) ? true : false; }

	BVPoint				GetDown(){ return m_ptDown; }
	void				SetDown(BVPoint pt){ m_ptDown = pt;m_ptMove = pt;m_ptMovePrev = pt; }

	BVPoint				GetMove(){ return m_ptMove; }
	void				SetMove(BVPoint pt){ m_ptMovePrev = m_ptMove; m_ptMove = pt; }

	BVPoint				GetMovePrev(){ return m_ptMovePrev; }

	THitResult			HitTest(int X,int Y);
	THitResult			GetCurHitResult(){ return m_HitResult; }
	void				SetCurHitResult(THitResult h){ m_HitResult = h; }
	void				GetHandle(BVRect r,BVRect* rx);

	void				SaveToFile(const EString& sFile);
	void				LoadFromFile(const EString& sFile);

	SELECTED_TYPE&			GetSelected();
	bool				IsSelected(afc_object<CControl> p);
	void				SetSelected(afc_object<CControl> p,bool bSelected);
	void				UnSelectAll();
	int				GetSelectedCount();

	afc_object<CControl>		GetFocus(){ return m_pFocus; }
	void				SetFocus(afc_object<CControl>& pFocus){ m_pFocus = pFocus; }

	void				RemoveControl(afc_object<CControl> p);
	bool				IsChildOf(afc_object<CControl> pParent,afc_object<CControl> pChild);

	EString				NewControlName(const EString& sName);

	bool				GetLocked(){ return m_bLocked; }
	void				SetLocked(bool b){ m_bLocked = b; }

	void				MoveSelected(int dx,int dy);
	ARect				GetChangePos(ARect r,int iHandle,int dx,int dy);

	void				OnAlignLeft();
	void				OnAlignCenter();
	void				OnAlignRight();
	void				OnAlignTop();
	void				OnAlignMiddle();
	void				OnAlignBottom();
	void				OnAlignHorzSpace();
	void				OnAlignVertSpace();
	void				OnAlignSameWidth();
	void				OnAlignSameHeight();
	void				OnAlignSameSize();
public:
	AEventHandler			m_OnRefresh;
private:
	afc_object<CControl>		m_pWebForm;
	BVPoint				m_ptDown;
	BVPoint				m_ptMove;
	BVPoint				m_ptMovePrev;

	SELECTED_TYPE			m_aSelected;
	afc_object<CControl>		m_pFocus;
	THitResult			m_HitResult;
	bool				m_bLocked;
private:
	void				Init();

	EString				BodyToJSON(CONFIG_BODY pRoot);

	int				_HitTestHandle(BVRect* r,int X,int Y);
	THitResult			_HitTest(afc_object<CControl> p,int X,int Y);
	void				_MoveControl(afc_object<CControl>& p,int dx,int dy);

	afc_object<CControl>		_FindControlByName(const EString& sName,afc_vector<CControl>& aControl);
};

//---------------------------------------------------------------------------
#endif
