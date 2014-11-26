#ifndef ASYSICONIMAGELIST_H
#define ASYSICONIMAGELIST_H

/*
 * 内置的图标Key
 * 收藏夹 Favorites
 * 计算机 Computer
 * 库  Profile
 * 网络 Network
 * 目录 Folder
 */
class AUI_API ASysIconImageList : public AImageList
{
	CLASS_INFO(SysIconImageList,ImageList);
public:
	ASysIconImageList(AComponent* pOwner);
	virtual ~ASysIconImageList();

	virtual int GetIndex(const AString& sKey);//参数可以是内置的Key，也可以是文件扩展名
};

#endif//ASYSICONIMAGELIST_H