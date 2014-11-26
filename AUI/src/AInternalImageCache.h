#ifndef AINTERNALIMAGECACHE_H
#define AINTERNALIMAGECACHE_H

#include <map>

class AInternalImageCache : public AComponent
{
	CLASS_INFO(InternalImageCache,Component);
public:
	AInternalImageCache(AComponent* pOwner):base_class(pOwner)
	{
		//
	}
	virtual ~AInternalImageCache()
	{
		Clear();
	}
	void Clear()
	{
		for(std::map<AString,AImage*>::iterator it=m_aGraphic.begin();it!=m_aGraphic.end();it++)
		{
			AImage* p = it->second;
			p->Release();
		}
		m_aGraphic.clear();
	}
	AImage* GetImageByKey(const AString& sKey)
	{
		std::map<AString,AImage*>::iterator it = m_aGraphic.find(sKey);
		if( it != m_aGraphic.end() ) return it->second;
		return NULL;
	}
	void SetImageByKey(const AString& sKey,AImage* pGraphic)
	{
		if( pGraphic == NULL ) return;
		RemoveImageByKey(sKey);
		m_aGraphic[sKey] = pGraphic;
		pGraphic->AddRef();
	}
	void RemoveImageByKey(const AString& sKey)
	{
		std::map<AString,AImage*>::iterator it = m_aGraphic.find(sKey);
		if( it != m_aGraphic.end() )
		{
			AImage* p = it->second;
			p->Release();
			m_aGraphic.erase(it);
		}
	}
	//装载缺省的图标
	void LoadDefault();

	std::map<AString,AImage*>	m_aGraphic;
};

extern AInternalImageCache* g_pInternalImageCache;

#endif//AINTERNALIMAGECACHE_H