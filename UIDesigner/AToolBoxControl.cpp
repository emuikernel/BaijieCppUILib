#include "StdAfx.h"
#include "AToolBoxControl.h"

static AToolBoxControl* g_pToolBox=NULL;
afc_object<AMLBody> GetControlDefNode(const AString& sName)
{
	return g_pToolBox->GetControlDefNode(sName);
}

REG_CLASS(AToolBoxControl);
AToolBoxControl::AToolBoxControl(AComponent* pOwner):base_class(pOwner)
{
	g_pToolBox = this;
	m_pTree = new ATreeView(this);
	m_pTree->SetName(_T("ToolTree"));
	m_pTree->SetAlign( alClient );
	EVENT_CONNECT(m_pTree,OnMouseMove,OnTreeMouseMove);
	EVENT_CONNECT(m_pTree,OnDrawDrag,OnTreeDrawDrag);

	m_pImageList = new AImageList(this);
	m_pTree->SetImageList( m_pImageList );

	m_ControlDefines = new AMLBody();

	LoadDefFiles();
}


AToolBoxControl::~AToolBoxControl(void)
{
}


void AToolBoxControl::LoadDefFiles()//装载所有的控件定义文件
{
	ADirectoryEnumerator de( GetCurPath() + _T("Controls"));
	de.ListExtend(_T("ctld"));
	while( de.Next() )
	{
		LoadControlDefine( de.CurWithPath() );
	}
}

void AToolBoxControl::LoadControlDefine(const AString& sFile)
{
	AFile f;
	if( f.Open(EString(sFile),afcRead) )
	{
		xmemory ms;
		ms.SetSize( f.Length() +1);
		f.Read(ms.GetData(),ms.GetSize()-1);
		BYTE* ptr = ms.GetData();
		ptr[ ms.GetSize() -1 ] = 0;

		IConfig cfg;
		EString sLoad( (char *)ptr );
		cfg.LoadText( sLoad );
		CONFIG_BODY pRoot;
		pRoot = cfg.GetRootBody();
		if( pRoot != ANULL )
		{
			AString sCat = pRoot->GetProp(_T("cat"));
			AString sType = pRoot->GetProp(_T("type"));
			AString sIcon = pRoot->GetProp(_T("icon"));
			AString sDisp = pRoot->GetProp(_T("displayname"));


			if( sCat == _T("") ) sCat = _T("缺省");
			ATreeViewItem* pp = GetNodeByCat(sCat);
			if( pp == NULL )
			{
				pp = m_pTree->AddItem(NULL,sCat);

				int iIconSet = m_pImageList->GetIndex(_T("集合"));
				if( iIconSet == -1 )
				{
					AImage* pImage = AImage::CreateImage();
					pImage->LoadFromFile(GetCurPath()+_T("Images\\集合.bmp"));
					m_pImageList->Add( pImage,_T("集合"));
					iIconSet = m_pImageList->GetIndex(_T("集合"));
				}
				pp->SetImageIndex( iIconSet );
				m_aCatNode[sCat] = pp;
				pp->SetExpanded(true);
			}
			ATreeViewItem* pItem = m_pTree->AddItem(pp,sDisp);
			int iIconIndex = m_pImageList->GetIndex(sType);
			if( iIconIndex == -1 )
			{
				AImage* pImage = AImage::CreateImage();

				EString sIconFile = GetCurPath();
				sIconFile += _ES("Controls\\")+ sIcon;
				pImage->LoadFromFile(sIconFile);
				m_pImageList->Add(pImage,sType);
				iIconIndex = m_pImageList->GetIndex(sType);
			}
			pItem->SetImageIndex(iIconIndex);
			pItem->SetTag( (void *)&pRoot );
			m_ControlDefines->AddBody( &pRoot );
		}
	}
}

ATreeViewItem* AToolBoxControl::GetNodeByCat(const AString& sCat)
{
	AString s;
	s = sCat;
	s.ToLower();
	std::map<AString,ATreeViewItem*>::iterator it = m_aCatNode.find(s);
	if( it != m_aCatNode.end() ) return it->second;
	return NULL;
}

CONFIG_BODY AToolBoxControl::GetControlDefNode(const AString& sName)
{
	for(int i=0;i<m_ControlDefines->GetChildCount();i++)
	{
		afc_object<AMLBody> p;
		p = m_ControlDefines->GetChild(i);
		if( p->GetProp(_T("type")) == sName ) return p;
	}
	return afc_object<AMLBody>();
}

void AToolBoxControl::OnTreeMouseMove(AObject* pSender,AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL ) return;

	if( evt->m_KeyStates.Has(ksLeftButton) )
	{
		m_pTree->StartDrag(m_pTree);
	}
}

void AToolBoxControl::OnTreeDrawDrag(AObject* pSender,AEvent* pEvent)
{
	ADragEvent* evt = dynamic_cast<ADragEvent*>(pEvent);
	if( evt == NULL ) return;

	ACanvas* cs = GetUICore()->GetCanvas();
	int x = evt->m_X;
	int y = evt->m_Y;
	int dx = evt->m_szDrawSize.cx;
	int dy = evt->m_szDrawSize.cy;

	ASolidBrush br;
	if( evt->m_bAccept==false ) 
		br.SetColor(AColor(180,255,0,0));
	else 
		br.SetColor(AColor(180,0,255,0));
	cs->Use(&br)->FillRect(ARect(x,y,x+dx,y+dy));
}