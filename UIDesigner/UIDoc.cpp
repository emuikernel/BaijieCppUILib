//---------------------------------------------------------------------------
#include "stdafx.h"
#include "UIDoc.h"
#include "DbgHelp.h"

//---------------------------------------------------------------------------

CUIDoc::CUIDoc() : IDocument()
{
	Init();
}

CUIDoc::~CUIDoc()
{
}

void CUIDoc::Init()
{
	m_bLocked		=	false;
	m_bModified		=	false;
	m_pFocus		=	ANULL;
	m_ptDown		=	BVPoint(-1,-1);
	m_ptMove		=	BVPoint(-1,-1);

	m_aSelected.clear();
	m_pWebForm = ANULL;
}


void CUIDoc::GetHandle(BVRect r,BVRect* rx)
{
		rx[0] = BVRect(r.left-HANDLE_WIDTH,r.top-HANDLE_WIDTH,r.left,r.top);
		rx[1] = BVRect(r.left+r.GetWidth()/2-HANDLE_WIDTH/2,r.top-HANDLE_WIDTH,r.left+r.GetWidth()/2+HANDLE_WIDTH/2,r.top);
		rx[2] = BVRect(r.right,r.top-HANDLE_WIDTH,r.right+HANDLE_WIDTH,r.top);
		rx[3] = BVRect(r.right,r.top+r.GetHeight()/2-HANDLE_WIDTH/2,r.right+HANDLE_WIDTH,r.top+r.GetHeight()/2+HANDLE_WIDTH/2);
		rx[4] = BVRect(r.right,r.bottom,r.right+HANDLE_WIDTH,r.bottom+HANDLE_WIDTH);
		rx[5] = BVRect(r.left+r.GetWidth()/2-HANDLE_WIDTH/2,r.bottom,r.left+r.GetWidth()/2+HANDLE_WIDTH/2,r.bottom+HANDLE_WIDTH);
		rx[6] = BVRect(r.left-HANDLE_WIDTH,r.bottom,r.left,r.bottom+HANDLE_WIDTH);
		rx[7] = BVRect(r.left-HANDLE_WIDTH,r.top+r.GetHeight()/2-HANDLE_WIDTH/2,r.left,r.top+r.GetHeight()/2+HANDLE_WIDTH/2);

}

THitResult			CUIDoc::HitTest(int X,int Y)
{
	THitResult hr;
	if( &m_pWebForm != NULL )
	{
		BVRect r;
		r = m_pWebForm->GetAbsPosition();
		BVRect rx[8];
		GetHandle(r,rx);
		int iHandle = _HitTestHandle(rx,X,Y);
		if( iHandle > HANDLE_DUMMY )
		{
			hr.m_iHandle = iHandle;
			hr.m_pBase = &m_pWebForm;
			return hr;
		}
		else
		{
			THitResult hrc = _HitTest(m_pWebForm,X,Y);
			if( hrc.m_pBase != ANULL ) return hrc;
		}
	}
	return hr;
}

int					CUIDoc::_HitTestHandle(BVRect* r,int X,int Y)
{
	ARect rx(r[0].left,r[0].top,r[4].right,r[4].bottom);
	if( !rx.PtInRect(APoint(X,Y)) )
		return HANDLE_DUMMY;
	for(int i=0;i<8;i++)
	{
		if( X >= r[i].left && X <= r[i].right && Y >= r[i].top && Y <= r[i].bottom )
			return i + HANDLE_LEFTTOP;
	}
	return HANDLE_DUMMY;
}

THitResult			CUIDoc::_HitTest(afc_object<CControl> p,int X,int Y)
{
	THitResult hr;

	BVRect cp = p->GetAbsPosition();
	BVRect r = cp;
	if( IsSelected(p) )
	{
		BVRect rx[8];
		GetHandle(r,rx);
		hr.m_iHandle = _HitTestHandle(rx,X,Y);
		if( hr.m_iHandle != HANDLE_DUMMY )
		{
			hr.m_pBase = p;
			return hr;
		}
	}

	if( X >= r.left && X <= r.right && Y >= r.top && Y <= r.bottom )
	{
		hr.m_pBase = p;
		hr.m_iHandle = HANDLE_INME;
	}
	else
	{
		hr.m_iHandle = HANDLE_DUMMY;
		hr.m_pBase = ANULL;
	}

	if( p->m_aChild.size() )
	{
		for(UINT i=0;i<p->m_aChild.size();i++)
		{
			afc_object<CControl> pControl;
			pControl = p->m_aChild[i];
			THitResult ht = _HitTest(pControl,X,Y);
			if( ht.m_pBase != ANULL ) return ht;
		}
	}

	return hr;
}


void				CUIDoc::SaveToFile(const EString& sFile)
{
	if( &m_pWebForm == NULL ) return;
	CONFIG_BODY pRoot;
	pRoot = m_pWebForm->m_Config.GetRootBody();
	if( &pRoot == NULL ) return;

	EString str;
	str = sFile;
	//::MakeSureDirectoryPathExists(str.ConvertTo());

	XFile f;
	if( f.Create(sFile) )
	{
		BYTE x[]={0xFF,0xFE};
		f.Write(x,2);
		EString str = pRoot->ToString();
		f.Write((BYTE *)str.Text(),str.Length()*sizeof(ECHAR));
		m_bModified = false;
	}

}

void				CUIDoc::LoadFromFile(const EString& sFile)
{
	Init();
	UnSelectAll();

	AFile f;
	if( f.Open(sFile,afcRead) )
	{
		xmemory ms;
		ms.SetSize( f.Length() );
		f.Read(ms.GetData(),ms.GetSize());
		BYTE* ptr = ms.GetData();
		int iLen = ms.GetSize()/2;

		if( ms.GetSize() > 2 && ptr[0] == 0xFF && ptr[1] == 0xFE )
		{  //兼容旧版本文件的处理
			ptr += 2;
			iLen --;
		}
		EString sDoc( (wchar_t *)ptr,iLen);

		m_pWebForm = new CControl();
		m_pWebForm->m_Config.LoadText( (ECHAR *)sDoc.Text() );
		if( m_pWebForm->m_Config.GetRootBody() )
			m_pWebForm->LoadChildren();
		else
			m_pWebForm = ANULL;
		
		SetFileName( sFile );
	}
}

SELECTED_TYPE&		CUIDoc::GetSelected()
{
	return m_aSelected;
}

bool				CUIDoc::IsSelected(afc_object<CControl> p)
{
	SELECTED_TYPE::iterator it=m_aSelected.find(&p);
	if( it !=m_aSelected.end())  return true;
	return false;
}

void				CUIDoc::SetSelected(afc_object<CControl> p,bool bSelected)
{
	if( bSelected )
	{
		if( !IsSelected(p) )
		{
			m_aSelected[&p] = true;
		}
		m_pFocus = p;
	}
	else
	{
		SELECTED_TYPE::iterator it = m_aSelected.find(&p);
		if( it !=m_aSelected.end() )
		{
			m_aSelected.erase(it);

			if( m_aSelected.size() )
			{
				m_pFocus = m_aSelected.begin()->first;
			}
			else
				m_pFocus = ANULL;
		}
	}
}

void				CUIDoc::UnSelectAll()
{
	m_pFocus		=	ANULL;
	m_aSelected.clear();
}

int					CUIDoc::GetSelectedCount()
{
	return m_aSelected.size();
}

void				CUIDoc::RemoveControl(afc_object<CControl> p)
{
	if( &p == &m_pWebForm ) m_pWebForm = ANULL;
	else
	{
		if( p->m_pParent )
		{
			for(UINT i=0;i<p->m_pParent->m_aChild.size();i++)
			{
				afc_object<CControl> px;
				px = p->m_pParent->m_aChild[i];
				if( px == &p )
				{
					p->m_pParent->RemoveChild( p );
					break;
				}
			}
		}
	}
}

bool				CUIDoc::IsChildOf(afc_object<CControl> pParent,afc_object<CControl> pChild)
{
	for(UINT i=0;i<pParent->m_aChild.size();i++)
	{
		afc_object<CControl> pu;
		pu = pParent->m_aChild[i];
		if( pu == &pChild ) return true;
		if( IsChildOf(pu,pChild) ) return true;
	}
	return false;
}

void				CUIDoc::OnAlignLeft()
{
	afc_object<CControl> pFocus = m_pFocus;
	if( pFocus != ANULL )
	{
		BVRect r = pFocus->GetPosition();
		for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
		{
			if( pFocus != it->first )
			{
				BVRect r2=it->first->GetPosition();
				r2.left = r.left;
				it->first->SetPosition(r2);
			}
		}
		m_OnRefresh.Call(this,NULL);
		SetModified( true );
	}
}

void				CUIDoc::OnAlignCenter()
{
	afc_object<CControl> pFocus = m_pFocus;
	if( pFocus != ANULL )
	{
		BVRect r = pFocus->GetPosition();
		for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
		{
			if( pFocus != it->first )
			{
				BVRect r2=it->first->GetPosition();
				r2.left = r.left+r.GetWidth()/2-r2.GetWidth()/2;
				it->first->SetPosition(r2);
			}
		}
		m_OnRefresh.Call(this,NULL);
		SetModified( true );
	}
}

void				CUIDoc::OnAlignRight()
{
	afc_object<CControl> pFocus = m_pFocus;
	if( pFocus != ANULL )
	{
		BVRect r = pFocus->GetPosition();
		for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
		{
			if( pFocus != it->first )
			{
				BVRect r2=it->first->GetPosition();
				r2.left = r.left+r.GetWidth()-r2.GetWidth();
				it->first->SetPosition(r2);
			}
		}
		m_OnRefresh.Call(this,NULL);
		SetModified( true );
	}
}

void				CUIDoc::OnAlignTop()
{
	afc_object<CControl> pFocus = m_pFocus;
	if( pFocus != ANULL )
	{
		BVRect r = pFocus->GetPosition();
		for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
		{
			if( pFocus != it->first )
			{
				BVRect r2=it->first->GetPosition();
				r2.top = r.top;
				it->first->SetPosition(r2);
			}
		}
		m_OnRefresh.Call(this,NULL);
		SetModified( true );
	}
}

void				CUIDoc::OnAlignMiddle()
{
	afc_object<CControl> pFocus = m_pFocus;
	if( pFocus != ANULL )
	{
		BVRect r = pFocus->GetPosition();
		for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
		{
			if( pFocus != it->first )
			{
				BVRect r2=it->first->GetPosition();
				r2.top = r.top+r.GetHeight()/2-r2.GetHeight()/2;
				it->first->SetPosition(r2);
			}
		}
		m_OnRefresh.Call(this,NULL);
		SetModified( true );
	}
}

void				CUIDoc::OnAlignBottom()
{
	afc_object<CControl> pFocus = m_pFocus;
	if( pFocus != ANULL )
	{
		BVRect r = pFocus->GetPosition();
		for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
		{
			if( pFocus != it->first )
			{
				BVRect r2=it->first->GetPosition();
				r2.top = r.top+r.GetHeight()-r2.GetHeight();
				it->first->SetPosition(r2);
			}
		}
		m_OnRefresh.Call(this,NULL);
		SetModified( true );
	}
}

void				CUIDoc::OnAlignHorzSpace()
{
	afc_object<CControl> pFocus = m_pFocus;
	if( pFocus != ANULL )
	{
		BVRect r = pFocus->GetPosition();
		int iFirst=r.left,iLast=r.left;
		for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
		{
			if( pFocus != it->first )
			{
				BVRect r2 = it->first->GetPosition();
				if( r2.left < iFirst ) iFirst = r2.left;
				if( r2.left > iLast ) iLast = r2.left;
			}
		}
		int iDelta = (iLast-iFirst)/(GetSelectedCount()-1);
		for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
		{
			BVRect rx = it->first->GetPosition();
			rx.left = iFirst;
			it->first->SetPosition(rx);
			iFirst += iDelta;
		}
		m_OnRefresh.Call(this,NULL);
		SetModified( true );
	}
}

void				CUIDoc::OnAlignVertSpace()
{
	afc_object<CControl> pFocus = m_pFocus;
	if( pFocus != ANULL )
	{
		BVRect r = pFocus->GetPosition();
		int iFirst=r.top,iLast=r.top;
		for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
		{
			if( pFocus != it->first )
			{
				BVRect r2 = it->first->GetPosition();
				if( r2.top < iFirst ) iFirst = r2.top;
				if( r2.top > iLast ) iLast = r2.top;
			}
		}
		int iDelta = (iLast-iFirst)/(GetSelectedCount()-1);
		for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
		{
			BVRect rx = it->first->GetPosition();
			rx.top = iFirst;
			it->first->SetPosition(rx);
			iFirst += iDelta;
		}
		m_OnRefresh.Call(this,NULL);
		SetModified( true );
	}
}

void				CUIDoc::OnAlignSameWidth()
{
	afc_object<CControl> pFocus = m_pFocus;
	if( pFocus != ANULL )
	{
		BVRect r = pFocus->GetPosition();
		for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
		{
			if( pFocus != it->first )
			{
				BVRect r2=it->first->GetPosition();
				r2.right = r2.left+r.GetWidth();
				it->first->SetPosition(r2);
			}
		}
		m_OnRefresh.Call(this,NULL);
		SetModified( true );
	}
}

void				CUIDoc::OnAlignSameHeight()
{
	afc_object<CControl> pFocus = m_pFocus;
	if( pFocus != ANULL )
	{
		BVRect r = pFocus->GetPosition();
		for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
		{
			if( pFocus != it->first )
			{
				BVRect r2=it->first->GetPosition();
				r2.bottom = r2.top+r.GetHeight();
				it->first->SetPosition(r2);
			}
		}
		m_OnRefresh.Call(this,NULL);
		SetModified( true );
	}
}

void				CUIDoc::OnAlignSameSize()
{
	afc_object<CControl> pFocus = m_pFocus;
	if( pFocus != ANULL )
	{
		BVRect r = pFocus->GetPosition();
		for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
		{
			if( pFocus != it->first )
			{
				BVRect r2=it->first->GetPosition();
				r2.right = r2.left+r.GetWidth();
				r2.bottom = r2.top+r.GetHeight();
				it->first->SetPosition(r2);
			}
		}
		m_OnRefresh.Call(this,NULL);
		SetModified( true );
	}
}

EString CUIDoc::NewControlName(const EString& sName)
{
	EString sNameBase;
	int iCountBase=1;
	if( sName == "" ) sNameBase = "New_";
	else
	{
		int iPos = sName.ReverseFind(L"_");
		if( iPos > -1 )
		{
			int i=EIntVal(sName.Right(sName.Length()-iPos-1));
			if( i > 0 )
			{
				iCountBase = i+1;
				sNameBase = sName.Left(iPos+1);
			}
			else
			{
				sNameBase = sName+"_";
			}
		}
		else
		{
			sNameBase = sName;
		}
	}
	if( &m_pWebForm != NULL )
	{
		afc_vector<CControl> aControl;
		aControl.push( m_pWebForm );

		while( true )
		{
			if( _FindControlByName(sNameBase + EStringVal(iCountBase),aControl) == ANULL ) break;
			else iCountBase ++;
		}
	}
	return sNameBase + EStringVal(iCountBase);
}

afc_object<CControl>		CUIDoc::_FindControlByName(const EString& sName,afc_vector<CControl>& aControl)
{
	afc_object<CControl> pRet;
	for(UINT i=0;i<aControl.size();i++)
	{
		afc_object<CControl> p(aControl[i]);
		if( (*p)[_ES("Name")]->AsString() == sName )
		{
			pRet = p;
			break;
		}
		pRet = _FindControlByName(sName,p->m_aChild);
		if( pRet != ANULL ) break;
	}
	return pRet;
}



EString CUIDoc::BodyToJSON(CONFIG_BODY pRoot)
{
	EString s;
	s = pRoot->GetName();
	s += _ES(":{");
	int index=0;
	AStringMapEnumerator em(&(pRoot->m_aProp));
	while(em.Next())
	{
		EString str;
		EString sp;
		sp = (ACHAR *)em.Cur();
		sp.Replace(_ES("\""),_ES("\\\""));
		sp.Replace(_ES("\'"),_ES("\\\'"));
		sp.Replace(_ES("\\"),_ES("\\\\"));
		str = _ES("\"")+em.Index()+_ES("\":\"")+sp+_ES("\"");
		if( index > 0 ) str = _ES(",")+str;
		index++;

		s += str;
        }
        if( pRoot->m_aProp.GetCount() && pRoot->GetChildCount() ) s += _ES(",");
        for(int i=0;i<pRoot->GetChildCount();i++)
        {
                CONFIG_BODY pNode;
		pNode = pRoot->GetChild(i);
                if( pNode->GetName() == _ES("@") )
                {
                        EString sv;
                        if( i > 0 ) sv = _ES(",");
			sv += _ES("_Children:[");
			for(int j=0;j<pNode->GetChildCount();j++)
                        {
                                CONFIG_BODY pn;
                                pn = pNode->GetChild(j);
                                EString sm= BodyToJSON(pn);
                                if( j > 0 ) sm = _ES(",")+sm;
				sv += sm;
                        }
                        sv += _ES("]");
                        s += sv;
                }
                else
		{
                        EString sv;
                        if( i > 0 ) sv = _ES(",");
                        sv += BodyToJSON(pNode);
                        s += sv;
                }
	}
        s += _ES("}");
        return s;
}

void				CUIDoc::MoveSelected(int dx,int dy)
{
	if( &m_pFocus == NULL ) return;
	for(SELECTED_TYPE::iterator it=m_aSelected.begin();it!=m_aSelected.end();it++)
	{
		CControl* px = it->first;
		if( &m_pFocus != px && px->m_pParent == m_pFocus->m_pParent )
		{
			afc_object<CControl> pc;
			pc = px;
			_MoveControl(pc,dx,dy);
		}
	}
	_MoveControl(m_pFocus,dx,dy);
	SetModified( true );
}

void				CUIDoc::_MoveControl(afc_object<CControl>& p,int dx,int dy)
{
	BVRect r;
	r = p->GetPosition();
	r.Offset(dx,dy);
	p->SetPosition(r);
}

ARect				CUIDoc::GetChangePos(ARect r,int iHandle,int dx,int dy)
{
	switch( iHandle )
	{
	case HANDLE_DUMMY:
		break;
	case HANDLE_INME:
		r.Offset(dx,dy);
		break;
	case HANDLE_LEFTTOP:
		r.left += dx;
		r.top += dy;
		break;
	case HANDLE_TOP:
		r.top += dy;
		break;
	case HANDLE_RIGHTTOP:
		r.right += dx;
		r.top += dy;
		break;
	case HANDLE_RIGHT:
		r.right += dx;
		break;
	case HANDLE_RIGHTBOTTOM:
		r.right += dx;
		r.bottom += dy;
		break;
	case HANDLE_BOTTOM:
		r.bottom += dy;
		break;
	case HANDLE_LEFTBOTTOM:
		r.left += dx;
		r.bottom += dy;
		break;
	case HANDLE_LEFT:
		r.left += dx;
		break;
	default:
		break;
	}
	return r;
}