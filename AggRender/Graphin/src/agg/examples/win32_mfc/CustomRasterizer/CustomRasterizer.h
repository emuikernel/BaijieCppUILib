// CustomRasterizer.h : main header file for the CUSTOMRASTERIZER application
//

#if !defined(AFX_CUSTOMRASTERIZER_H__DE938D13_D7F5_449B_8091_E969FA454953__INCLUDED_)
#define AFX_CUSTOMRASTERIZER_H__DE938D13_D7F5_449B_8091_E969FA454953__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCustomRasterizerApp:
// See CustomRasterizer.cpp for the implementation of this class
//

class CCustomRasterizerApp : public CWinApp
{
public:
	CCustomRasterizerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomRasterizerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCustomRasterizerApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMRASTERIZER_H__DE938D13_D7F5_449B_8091_E969FA454953__INCLUDED_)
