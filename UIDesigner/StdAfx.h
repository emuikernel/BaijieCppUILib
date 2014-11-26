// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_DEPRECATE // Don't bother us about depricated functions

#include <windows.h>
#include <objbase.h>

#include "AFC.h"
#include "ASys.h"
#include "AUI.h"


//==== 为兼容旧代码的宏定义 ======
#define EString AString
#define ECHAR ACHAR

#define IConfig AML

#define afc_object_base AObject
#define _COMOBJ_TYPEDEF(x,y) typedef afc_object<y> x
#define AOBJECT afc_object_base*
#define CComNode afc_object<AMLBody>
#define CONFIG_BODY afc_object<AMLBody>
#define ANULL (afc_object_base *)0
#define XFile AFile
#define xmemory AMemory
#define EStringVal AStringVal
#define ETextCompare ATextCompare
#define ETextCompareCase ATextCompareCase
#define EIntVal AIntVal
#define EInt64Val AInt64Val
#define EDoubleVal ADoubleVal

#define BVRect ARect
#define BVCanvas ACanvas
#define _ES _T
#define BVFont AFont
#define BVBrush ASolidBrush
#define BVPen APen
#define BVColor AColor
#define BVPoint APoint
#define BVSize ASize
#define BVImage AImage
#define TPropItem APropItem
#define TPropInt APropInt
#define TPropString APropString
#define TPropHint APropHint
#define TPropCombo APropCombo
#define TPropGroup APropGroup

extern AString NewGUID();

#ifdef _DEBUG
#pragma comment(lib, "afcd.lib")
#else
#pragma comment(lib,"afc.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "asysd.lib")
#else
#pragma comment(lib,"asys.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "auid.lib")
#else
#pragma comment(lib,"aui.lib")
#endif

#include <vector>
#include <map>

extern AColor Int2Color(int v);
extern int Color2Int(AColor cr);
extern afc_object<AMLBody> LoadNodeFromFile(const EString& sFile);
extern afc_object<AMLBody> GetControlDefNode(const AString& sName);
extern EString GetExt(const EString& sFileName);
extern EString GetCurPath();
extern EString GetNamespacePath(const EString& sProjectPath,const EString& sFile);
extern EString GetRelPath(const EString& sProjectPath,const EString& sFile);
extern APropEdit* GetPropEdit();
extern EString AddPath(const EString& sPath,const EString& s);


class time_counter
{
public:
	time_counter()
	{
		m_dwStart = ::GetTickCount();
	}
	~time_counter()
	{

	}
	DWORD Tick()
	{
		DWORD dw = ::GetTickCount();
		DWORD delta;
		if( dw >= m_dwStart ) delta = dw - m_dwStart;
		else delta = (DWORD)-1 - m_dwStart + dw;
		return delta;
	}
	void Reset()
	{
		m_dwStart = ::GetTickCount();
	}
	void Print(const AString& str)
	{
		OutputDebugString(AString().Format(_T("\r\n####### [%d]%s ########"),Tick(),str.Text()));
	}
private:
	DWORD		m_dwStart;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
