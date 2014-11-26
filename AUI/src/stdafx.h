// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>
#include <winuser.h>
//#include <olectl.h>
#include <shellapi.h>
#include <Shlobj.h>

#include <map>

#include "AFC.h"

#ifdef _DEBUG
#pragma comment(lib, "afcd.lib")
#else
#pragma comment(lib,"afc.lib")
#endif


#include "ASys.h"

#ifdef _DEBUG
#pragma comment(lib, "asysd.lib")
#else
#pragma comment(lib,"asys.lib")
#endif

#include "AUI.h"



// TODO: 在此处引用程序需要的其他头文件
#ifdef _DEBUG
extern void DebugStr(const AString& s);
#else
#define DebugStr(x) OutputDebugString(x)
#endif

extern HMODULE g_hInstance;

struct ADragInfo
{
	AControl*	pSource;
	ASize		szDraw;
	bool		bDragging;
	bool		bCanceled;//是否已经取消
	bool		bAccept;
	int		X;
	int		Y;//上次的位置
};
extern ADragInfo g_DragInfo;

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
