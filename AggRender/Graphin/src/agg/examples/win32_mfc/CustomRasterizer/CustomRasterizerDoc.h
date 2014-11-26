// CustomRasterizerDoc.h : interface of the CCustomRasterizerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMRASTERIZERDOC_H__153E475E_10B8_4738_8090_F758393DABE8__INCLUDED_)
#define AFX_CUSTOMRASTERIZERDOC_H__153E475E_10B8_4738_8090_F758393DABE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCustomRasterizerDoc : public CDocument
{
protected: // create from serialization only
	CCustomRasterizerDoc();
	DECLARE_DYNCREATE(CCustomRasterizerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomRasterizerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomRasterizerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCustomRasterizerDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMRASTERIZERDOC_H__153E475E_10B8_4738_8090_F758393DABE8__INCLUDED_)
