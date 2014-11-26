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

#include "AFC.h"
#include "AUI.h"


#ifdef _DEBUG
#pragma comment(lib, "afcd.lib")
#else
#pragma comment(lib,"afc.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "auid.lib")
#else
#pragma comment(lib,"aui.lib")
#endif

extern HMODULE g_hInstance;

// TODO: 在此处引用程序需要的其他头文件
