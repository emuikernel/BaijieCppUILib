// CustomRasterizerView.h : interface of the CCustomRasterizerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMRASTERIZERVIEW_H__58952BE1_E114_4F26_9ED2_F2CBA116948A__INCLUDED_)
#define AFX_CUSTOMRASTERIZERVIEW_H__58952BE1_E114_4F26_9ED2_F2CBA116948A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCustomRasterizerView : public CView
{
protected: // create from serialization only
	CCustomRasterizerView();
	DECLARE_DYNCREATE(CCustomRasterizerView)

// Attributes
public:
	CCustomRasterizerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomRasterizerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomRasterizerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCustomRasterizerView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CustomRasterizerView.cpp
inline CCustomRasterizerDoc* CCustomRasterizerView::GetDocument()
   { return (CCustomRasterizerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMRASTERIZERVIEW_H__58952BE1_E114_4F26_9ED2_F2CBA116948A__INCLUDED_)
