#include "StdAfx.h"

AImageList::AImageList(AComponent* pOwner):AInvisibleControl(pOwner)
{
	Size = ASize(16,16);//»± °¥Û–°
}

AImageList::~AImageList()
{
	Clear();
}



void AImageList::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawImageList(cs,this);
}

void AImageList::Add(AImage* pGraphic,const AString& sKey)
{
	if(pGraphic == NULL ) return;
	m_aImage.Add(pGraphic);
	m_aIndex.SetItem(sKey,(void *)(m_aImage.GetCount()-1));
}

int AImageList::GetIndex(const AString& sKey)
{
	if( m_aIndex.IsExist(sKey) )
	{
		return (int)m_aIndex.GetItem(sKey);
	}
	return -1;
}
void AImageList::Clear()
{
	m_aImage.Clear();
}
void AImageList::Remove(AImage* pGraphic)
{
	for(int i=0;i<m_aImage.GetCount();i++)
	{
		AImage* p = (AImage *)m_aImage[i];
		if( p == pGraphic ) 
		{
			m_aImage.Delete(i);
			AStringMap::myitem* item = m_aIndex.Begin();
			while( !m_aIndex.IsEnd(item) )
			{
				if( (int)item->pData == i )
				{
					m_aIndex.Delete(item);
					break;
				}
				item = m_aIndex.Next(item);
			}
		}
	}
}
int AImageList::GetCount()
{
	return (int)m_aImage.GetCount();
}

AImage* AImageList::GetItem(int index)
{
	if( index < 0 || index >= GetCount() ) throw AException(_T("Out of Bound"));
	return (AImage *)m_aImage[index];
}
