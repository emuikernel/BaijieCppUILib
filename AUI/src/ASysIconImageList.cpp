#include "stdafx.h"

ASysIconImageList::ASysIconImageList(AComponent* pOwner):base_class(pOwner)
{

}

ASysIconImageList::~ASysIconImageList()
{

}

static HICON GetIconByIndex(int csidl)
{
	LPITEMIDLIST lpItemIDList;
	SHFILEINFO shinfo;
	SHGetSpecialFolderLocation (NULL, csidl, &lpItemIDList);
	SHGetFileInfo( (LPCTSTR)lpItemIDList,
		NULL,
		&shinfo,
		sizeof(shinfo),
		SHGFI_SYSICONINDEX | SHGFI_DISPLAYNAME | SHGFI_ICON| SHGFI_SMALLICON|SHGFI_PIDL);
	return shinfo.hIcon;
}

int ASysIconImageList::GetIndex(const AString& s)
{
	AString sKey = s;
	if( sKey.Length() && sKey[0] == '.' )
	{
		//是文件扩展名作key
		//sKey = sKey.Right(sKey.Length()-1);
		sKey.ToLower();
	}
	if( m_aIndex.IsExist(sKey) )
		return (int)m_aIndex.GetItem(sKey);
	//不存在，则添加
	AImage* p = AImage::CreateImage();
	if( sKey == _T("Favorites"))
		p->LoadIcon(GetIconByIndex(CSIDL_FAVORITES));//收藏夹
	else if( sKey == _T("Computer"))
		p->LoadIcon(GetIconByIndex(CSIDL_DRIVES));//计算机
	else if( sKey == _T("Profile"))
		p->LoadIcon(GetIconByIndex(CSIDL_PROFILE));//库
	else if( sKey == _T("Network"))
		p->LoadIcon(GetIconByIndex(CSIDL_NETWORK));//网络
	else if( sKey == _T("Folder"))
		p->LoadIcon(GetIconByIndex(CSIDL_WINDOWS));//目录
	else
	{		
		SHFILEINFO shinfo;
		SHGetFileInfo( (LPCTSTR)sKey.Text(),
			FILE_ATTRIBUTE_NORMAL,
			&shinfo,
			sizeof(shinfo),
			SHGFI_SYSICONINDEX | SHGFI_DISPLAYNAME | SHGFI_ICON| SHGFI_SMALLICON| SHGFI_USEFILEATTRIBUTES);
		if( shinfo.hIcon == NULL )
		{
			delete p;
			return -1;
		}
		p->LoadIcon(shinfo.hIcon);
	}
	this->Add(p,sKey);
	return (int)m_aIndex.GetItem(sKey);
}