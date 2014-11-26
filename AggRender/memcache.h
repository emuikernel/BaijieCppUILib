#ifndef MEMCACHE_H
#define MEMCACHE_H
#include <map>
class MemCache
{
public:
	MemCache()
	{

	}
	~MemCache()
	{
		Clear();
	}
	AMemory* GetBuf(int iSize)
	{
		for(int i=0;i<m_aBuf.GetCount();i++)
		{
			AMemory* pMem = dynamic_cast<AMemory*>(m_aBuf[i]);
			if( (int)pMem->GetSize() >= iSize )
			{
				pMem->AddRef();
				m_aBuf.Delete(i);
				memset(pMem->GetData(),0,pMem->GetSize());
				OutputDebugString(AString().Format(_T("\r\n====从缓冲区中取得buff size=%d\r\n"),iSize));
				return pMem;
			}
		}
		AMemory* ms = new AMemory();
		ms->SetSize(iSize);
		ms->AddRef();
		OutputDebugString(AString().Format(_T("\r\n====新建buff size=%d\r\n"),iSize));
		return ms;
	}
	void BackBuf(AMemory* pMem)
	{
		while(true)
		{
			bool b=false;
			for(int i=0;i<m_aBuf.GetCount();i++)
			{
				AMemory* ms = dynamic_cast<AMemory*>( m_aBuf[i] );
				if( ms == NULL ) continue;
				if( ms->GetSize() < pMem->GetSize() )
				{
					m_aBuf.Delete(i);
					b = true;
					break;
				}
			}
			if(!b) break;
		}
		if( m_aBuf.GetCount() < 3 )//最多有三个可以比它大的
			m_aBuf.Add(pMem);
		pMem->Release();//平衡引用计数
	}
private:
	AObjectArray		m_aBuf;
	void Clear()
	{
		m_aBuf.Clear();
	}
};
#endif//MEMCACHE_H