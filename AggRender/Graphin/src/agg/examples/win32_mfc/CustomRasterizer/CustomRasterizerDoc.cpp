// CustomRasterizerDoc.cpp : implementation of the CCustomRasterizerDoc class
//

#include "stdafx.h"
#include "CustomRasterizer.h"

#include "CustomRasterizerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomRasterizerDoc

IMPLEMENT_DYNCREATE(CCustomRasterizerDoc, CDocument)

BEGIN_MESSAGE_MAP(CCustomRasterizerDoc, CDocument)
	//{{AFX_MSG_MAP(CCustomRasterizerDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomRasterizerDoc construction/destruction

CCustomRasterizerDoc::CCustomRasterizerDoc()
{
}

CCustomRasterizerDoc::~CCustomRasterizerDoc()
{
}

BOOL CCustomRasterizerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCustomRasterizerDoc serialization

void CCustomRasterizerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCustomRasterizerDoc diagnostics

#ifdef _DEBUG
void CCustomRasterizerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCustomRasterizerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCustomRasterizerDoc commands
