// stdafx.cpp : 只包括标准包含文件的源文件
// AFC.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
#ifdef _DEBUG

void DebugStr(const AString& s)
{
	ADateTime dt = ADateTime::GetCurrentTime();
	AString sx;
	sx.Format(_T("\r\n[%02d:%02d:%02d:%02d] "),dt.GetHour(),dt.GetMinute(),dt.GetSecond(),dt.GetMSecond());
	sx += s;
	OutputDebugString(sx);
}

#endif

ADragInfo g_DragInfo;