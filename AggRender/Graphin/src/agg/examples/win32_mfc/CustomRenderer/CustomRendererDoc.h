// CustomRendererDoc.h : interface of the CCustomRendererDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMRENDERERDOC_H__426C7135_9FEF_4941_B682_078E200885AB__INCLUDED_)
#define AFX_CUSTOMRENDERERDOC_H__426C7135_9FEF_4941_B682_078E200885AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCustomRendererDoc : public CDocument
{
protected: // create from serialization only
	CCustomRendererDoc();
	DECLARE_DYNCREATE(CCustomRendererDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomRendererDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomRendererDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCustomRendererDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMRENDERERDOC_H__426C7135_9FEF_4941_B682_078E200885AB__INCLUDED_)
