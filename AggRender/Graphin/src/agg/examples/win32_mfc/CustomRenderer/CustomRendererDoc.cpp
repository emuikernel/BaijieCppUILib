// CustomRendererDoc.cpp : implementation of the CCustomRendererDoc class
//

#include "stdafx.h"
#include "CustomRenderer.h"

#include "CustomRendererDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomRendererDoc

IMPLEMENT_DYNCREATE(CCustomRendererDoc, CDocument)

BEGIN_MESSAGE_MAP(CCustomRendererDoc, CDocument)
	//{{AFX_MSG_MAP(CCustomRendererDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomRendererDoc construction/destruction

CCustomRendererDoc::CCustomRendererDoc()
{
	// TODO: add one-time construction code here

}

CCustomRendererDoc::~CCustomRendererDoc()
{
}

BOOL CCustomRendererDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCustomRendererDoc serialization

void CCustomRendererDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCustomRendererDoc diagnostics

#ifdef _DEBUG
void CCustomRendererDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCustomRendererDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCustomRendererDoc commands
