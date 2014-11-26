#include "stdafx.h"
#include "ADirectoryForm.h"

static ACHAR* g_szDialogWnd=
	L"<c type=@DirectoryForm@ w=@423@ h = @437@ tx=@请选择@ >"
	L"<children>"
		L"<c type=@SysIconImageList@ n=@IconImageList@ />"
		L"<c type=@TitleBar@ n=@TitleBar@ al=@t@ h=@52@>"
		L"<children>"
			L"<c type=@Container@ n=@ButtonsPanel@ al=@t@ h = @28@ >"
			L"<children>"
				L"<c type=@CloseButton@ n=@cb@ al=@r@ w = @26@ />"
				L"<c type=@MaximizeButton@ n=@mb@ al=@r@ w = @26@ />"
				L"<c type=@MinimizeButton@ n=@ib@ al=@r@ w = @26@ />"
			L"</children>"
			L"</c>"
			L"<c type=@Container@ n=@MenuBarPanel@ al=@t@ h=@28@  />"
		L"</children>"
		L"</c>"
		L"<c type=@Container@ n=@LeftContainer@ al=@c@ w=@150@>"
		L"<children>"
			L"<c type=@TreeView@ n=@DirView@ al=@c@ >"
			L"</c>"
		L"</children>"
		L"</c>"
		L"<c type=@Container@ n=@BottomContainer@ al=@b@ h=@46@>"
		L"<children>"
			L"<c type=@Button@ n=@OK@ tx=@打开@ l=@400@ t=@10@ w=@80@ h=@20@ anc=@rt@ anc_r=@120@ />"
			L"<c type=@Button@ n=@Cancel@ tx=@取消@ l=@500@ t=@10@ w=@80@ h=@20@ anc=@rt@ anc_r=@20@ />"
		L"</children>"
		L"</c>"
	L"</children>"
	L"</c>"
	;

ADirectoryForm::ADirectoryForm(AComponent* pOwner):AForm(pOwner)
{
	m_nButtonID = NULL;
	m_sDirectory = NULL;

	MAP_CONTROL_INIT(DirView);
	MAP_CONTROL_INIT(OK);
	MAP_CONTROL_INIT(Cancel);
	MAP_CONTROL_INIT(ButtonsPanel);
	MAP_CONTROL_INIT(IconImageList);
	MAP_CONTROL_INIT(TitleBar);
	MAP_CONTROL_INIT(MenuBarPanel);
}

ADirectoryForm::~ADirectoryForm()
{

}

void ADirectoryForm::CreateFromXML()
{
	afc_object<AMLBody> pBody(new AMLBody());
	AString xml;
	xml = g_szDialogWnd;
	xml.Replace(_T("@"),_T("\""));
	pBody->FromXML(xml);
	this->FromXML(&pBody);
	this->CenterToScreen();
}

void ADirectoryForm::ConnectEvent()
{
	EVENT_CONNECT(GetMenuBarPanel(),OnMouseDown,TitleBackDoMouseDown);
	EVENT_CONNECT(GetMenuBarPanel(),OnMouseMove,TitleBackDoMouseMove);
	EVENT_CONNECT(GetButtonsPanel(),OnMouseDown,TitleBackDoMouseDown);
	EVENT_CONNECT(GetButtonsPanel(),OnMouseMove,TitleBackDoMouseMove);
	EVENT_CONNECT(GetOK(),OnClick,OnOKClick);
	EVENT_CONNECT(GetCancel(),OnClick,OnCancelClick);
	EVENT_CONNECT(GetDirView(),OnChange,OnDirChange);
}

void ADirectoryForm::TitleBackDoMouseDown(AObject* pSender,AEvent* pEvent)
{
	GetTitleBar()->DoMouseDown(pEvent);
}

void ADirectoryForm::TitleBackDoMouseMove(AObject* pSender,AEvent* pEvent)
{
	GetTitleBar()->DoMouseMove(pEvent);
}

void ADirectoryForm::OnOKClick(AObject* pSender,AEvent* pEvent)
{
	ATreeViewItem* pSelNode = GetDirView()->GetSelected();
	if( pSelNode == NULL )
	{
		AApplication::Get()->MsgBox(_T("请选择目录！"));
		return;
	}
	std::map<ATreeViewItem*,AString>::iterator it = m_aPath.find(pSelNode);
	if( it != m_aPath.end() ) *m_sDirectory = it->second;
	if( m_nButtonID ) 
	{
		*m_nButtonID = dbOK;
	}
	Destroy();
}
void ADirectoryForm::OnCancelClick(AObject* pSender,AEvent* pEvent)
{
	if( m_nButtonID ) *m_nButtonID = dbCancel;
	Destroy();
}



void ADirectoryForm::InitControls()
{
	
}


void ADirectoryForm::DoCreate()
{
	GetDirView()->SetImageList(GetIconImageList());
	FillNode(NULL);
	if( m_sInitDir == _T("") )
	{
		ACHAR szBuf[1024];
		::GetModuleFileName(NULL,szBuf,1024);
		m_sInitDir = szBuf;
		int iPos = m_sInitDir.ReverseFind(_T("\\"));
		if( iPos > -1 ) m_sInitDir = m_sInitDir.Left(iPos);
	}
	LocateTo(m_sInitDir);
}

static AString GetDirByIndex(int index)
{
	LPITEMIDLIST lpItemIDList;
	SHGetSpecialFolderLocation (NULL, index, &lpItemIDList);
	ACHAR buf[512];
	SHGetPathFromIDList(lpItemIDList,buf);
	AString s;
	s = buf;
	return s;
}

void ADirectoryForm::FillNode(ATreeViewItem* pNode)
{
	if( pNode == NULL )
	{
		ATreeView* tv = GetDirView();
		ATreeViewItem* pFavorite = tv->AddItem(NULL,_T("收藏夹"));
		pFavorite->SetExpanded(true);
		pFavorite->SetImageIndex( GetIconImageList()->GetIndex(_T("Favorites")) );
		m_aPath[ pFavorite ] = GetDirByIndex(CSIDL_FAVORITES);

		ATreeViewItem* pProfile = tv->AddItem(NULL,_T("库"));
		pProfile->SetExpanded(true);
		pProfile->SetImageIndex( GetIconImageList()->GetIndex(_T("Profile")) );
		m_aPath[ pProfile ] = GetDirByIndex(CSIDL_PROFILE);

		ATreeViewItem* pComputeNode = tv->AddItem(NULL,_T("计算机"));
		pComputeNode->SetExpanded(true);
		pComputeNode->SetImageIndex( GetIconImageList()->GetIndex(_T("Computer")) );
		m_aPath[ pComputeNode ] = GetDirByIndex(CSIDL_DRIVES);

		ACHAR buf[256];
		GetLogicalDriveStrings(256,buf);
		ACHAR* sz = buf;
		for(;*sz;sz += _tcslen(sz)+1)
		{
			
			ACHAR Volumelabel[120];
			DWORD SerialNumber;
			DWORD MaxCLength;
			DWORD FileSysFlag;
			ACHAR FileSysName[80];
			GetVolumeInformation( sz,Volumelabel,255,&SerialNumber,&MaxCLength,&FileSysFlag,FileSysName,255);
			AString s;
			s = sz;
			if( s.ReverseFind(_T("\\")) > -1 ) s = s.Left(s.Length()-1);
			s = AString(Volumelabel) + _T(" (") + s + _T(")");
			ATreeViewItem* pItem = tv->AddItem(pComputeNode,s);
			pItem->SetExpanded(true);
			pItem->SetImageIndex( GetIconImageList()->GetIndex(_T("Folder")) );
			m_aPath[ pItem ] = sz;
		}

		ATreeViewItem* pNetwork = tv->AddItem(NULL,_T("网络"));
		pNetwork->SetExpanded(true);
		pNetwork->SetImageIndex( GetIconImageList()->GetIndex(_T("Network")) );
		m_aPath[ pNetwork ] = GetDirByIndex(CSIDL_NETWORK);
	}
	else
	{
		m_pUICore->SetLayoutEnabled(false);
		if( pNode->GetChildItemCount() < 1 )
		{
			std::map<ATreeViewItem*,AString>::iterator it = m_aPath.find(pNode);
			if( it != m_aPath.end() )
			{
				ADirectoryEnumerator dl(it->second);
				dl.ListDirectory(true);
				dl.ListFile(false);
				dl.ListReadOnly(true);

				while( dl.Next() )
				{
					if( dl.IsDirectory()==false) continue;
					ATreeViewItem* pChildNode = GetDirView()->AddItem(pNode,dl.Cur());
					pChildNode->SetExpanded(true);
					pChildNode->SetImageIndex( GetIconImageList()->GetIndex(_T("Folder")) );
					m_aPath[ pChildNode ] = dl.CurWithPath();
				}
			}
		}
		pNode->SetExpanded( !pNode->GetExpanded() );
		m_pUICore->SetLayoutEnabled(true);
		GetDirView()->DoLayout();
	}
}

void ADirectoryForm::OnDirChange(AObject* pSender,AEvent* pEvent)
{
	ATreeViewItem* pSel = GetDirView()->GetSelected();
	std::map<ATreeViewItem*,AString>::iterator it = m_aPath.find(pSel);
	if( it != m_aPath.end() )
	{
		DebugStr(_T("OnDirChange FillNode..."));
		FillNode(pSel);
		DebugStr(_T("OnDirChange AfterFill..."));
		pSel->SetExpanded( !pSel->GetExpanded() );
	}
}

void ADirectoryForm::LocateTo(const AString& sPath)
{
	int iPos = sPath.Find(_T(":"));
	if( iPos < 0 ) return;//目前不支持网络路径，只支持本地硬盘路径

	ATreeView* pTree = GetDirView();

	AString s1 = sPath.Left(2);
	AString s2 = sPath.Right(sPath.Length()-3);

	//先找到根路径节点
	ATreeViewItem* pPathNode=NULL;
	for(int i=0;i<pTree->GetChildControls()->GetCount();i++)
	{
		ATreeViewItem* pItem = dynamic_cast<ATreeViewItem*>(pTree->GetChildControls()->GetItem(i));
		if( pItem == NULL ) continue;

		std::map<ATreeViewItem*,AString>::iterator it = m_aPath.find(pItem);
		if( it == m_aPath.end() ) continue;

		AString str = it->second;

		if( ATextCompareCase(str,s1) == 0 )
		{
			pPathNode = pItem;
			break;
		}
	}

	AString sp;
	sp = s1;

	while( pPathNode && s2 != _T("") )
	{
		int iPos = s2.Find(_T("\\"));
		if( iPos > -1 )
		{
			s1 = s2.Left(iPos);
			s2 = s2.Right(s2.Length()-iPos-1);
		}
		else
		{
			s1 = s2;
			s2 = _T("");
		}

		ATreeViewItem* pTmp = pPathNode;
		pPathNode = NULL;
		for(int i=0;i<pTmp->GetChildItemCount();i++)
		{
			ATreeViewItem* pItem = pTmp->GetChildItem(i);

			AString str = pItem->GetText();

			if( ATextCompareCase(str,s1) == 0 )
			{
				pPathNode = pItem;
				break;
			}
		}
		sp += _T("\\")+s1;
		if( pPathNode == NULL )
		{
			pPathNode = GetDirView()->AddItem(pTmp,s1);
			pPathNode->SetExpanded(true);
			pPathNode->SetImageIndex( GetIconImageList()->GetIndex(_T("Folder")) );
			m_aPath[ pPathNode ] = sp;
		}
		pTree->SetSelected( pPathNode );
	}
}
