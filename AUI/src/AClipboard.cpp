#include "stdafx.h"
#include <map>

AClipboard::AClipboard(AComponent* pOwner):AComponent(pOwner)
{

}

AClipboard::~AClipboard()
{
	//
}

void AClipboard::SetText(const AString& sText,bool bUnicode /*=true */ )
{
	if(::OpenClipboard(NULL))
	{
		::EmptyClipboard();
		HGLOBAL clipbuffer;
		char *buffer;
		if( bUnicode )
		{
			clipbuffer = ::GlobalAlloc(GMEM_DDESHARE, (sText.Length()+1)*2);
			buffer = (char *)::GlobalLock(clipbuffer);
			wcscpy((wchar_t*)buffer,sText.Text());
			::GlobalUnlock(clipbuffer);
			::SetClipboardData(CF_UNICODETEXT, clipbuffer);
		}
		else
		{
			AString str2;
			str2 = sText;
			const UACHAR* sz = str2.ConvertTo();
			clipbuffer = ::GlobalAlloc(GMEM_DDESHARE, strlen(sz)+1);
			buffer = (char *)::GlobalLock(clipbuffer);
			strcpy(buffer,sz);
			::GlobalUnlock(clipbuffer);
			::SetClipboardData(CF_TEXT, clipbuffer);
		}
		::CloseClipboard();
	}
}

AString AClipboard::GetText()
{
	AString sRet;
	if(::OpenClipboard(NULL))
	{
		std::map<UINT,int> aFormat;
		UINT nFormat = 0;
		while(true)
		{
			nFormat = ::EnumClipboardFormats(nFormat);
			if( !nFormat ) break;
			aFormat[ nFormat ] = 0;
		}

		//优先判断位图格式
		std::map<UINT,int>::iterator it;
		it = aFormat.find(CF_BITMAP);
		if( it != aFormat.end() ){}
		else
		{
			it = aFormat.find(CF_UNICODETEXT);
			if( it != aFormat.end() )
			{
				//获得剪贴板数据
				HGLOBAL hMem = GetClipboardData(CF_UNICODETEXT);
				if(NULL != hMem)
				{
					wchar_t* lpStr = (wchar_t*)::GlobalLock(hMem); 
					if(NULL != lpStr)
					{
						sRet = lpStr;
						::GlobalUnlock(hMem);
					}
				}
				::CloseClipboard();
			}
			else
			{
				it = aFormat.find(CF_TEXT);
				if( it != aFormat.end() )
				{
					//获得剪贴板数据
					HGLOBAL hMem = GetClipboardData(CF_TEXT);
					if(NULL != hMem)
					{
						char* lpStr = (char*)::GlobalLock(hMem); 
						if(NULL != lpStr)
						{
							sRet = lpStr;
							::GlobalUnlock(hMem);
						}
					}
					::CloseClipboard();
				}
			}
		}
		

	}
	return sRet;
}