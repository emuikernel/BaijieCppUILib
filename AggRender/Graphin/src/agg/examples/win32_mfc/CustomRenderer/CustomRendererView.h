// CustomRendererView.h : interface of the CCustomRendererView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMRENDERERVIEW_H__AC73FE79_5BBA_4273_9E1C_1EFD5CBAE154__INCLUDED_)
#define AFX_CUSTOMRENDERERVIEW_H__AC73FE79_5BBA_4273_9E1C_1EFD5CBAE154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCustomRendererView : public CView
{
protected: // create from serialization only
	CCustomRendererView();
	DECLARE_DYNCREATE(CCustomRendererView)

// Attributes
public:
	CCustomRendererDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomRendererView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomRendererView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCustomRendererView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CustomRendererView.cpp
inline CCustomRendererDoc* CCustomRendererView::GetDocument()
   { return (CCustomRendererDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMRENDERERVIEW_H__AC73FE79_5BBA_4273_9E1C_1EFD5CBAE154__INCLUDED_)
