#include "stdafx.h"
#include "AFileDialogForm.h"

static ACHAR* g_szDialogWnd=
	L"<c type=@FileDialogForm@ w=@700@ h = @400@ tx=@打开@ >"
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
		L"<c type=@Container@ n=@LeftContainer@ al=@l@ w=@250@>"
		L"<children>"
			L"<c type=@TreeView@ n=@DirView@ al=@c@ >"
			L"</c>"
		L"</children>"
		L"</c>"
		L"<c type=@Spliter@ w=@5@ al=@l@ >"
		L"</c>"
		L"<c type=@Container@ n=@ClientContainer@ al=@c@>"
		L"<children>"
			L"<c type=@ListView@ n=@FileView@ al=@c@ vs=@r@>"
			L"<children>"
			L"<c type=@ListHeadItem@ w=@200@ tx=@名称@ />"
			L"<c type=@ListHeadItem@ w=@147@ tx=@修改时间@ />"
			L"<c type=@ListHeadItem@ w=@120@ tx=@类型@ />"
			L"<c type=@ListHeadItem@ w=@80@ tx=@大小@ />"
			L"</children>"
			L"</c>"
		L"</children>"
		L"</c>"
		L"<c type=@Container@ n=@BottomContainer@ al=@b@ h=@80@>"
		L"<children>"
			L"<c type=@Label@ tx=@文件名：@ anc=@rt@ l=@30@ t=@12@ w=@50@ h=@24@ anc_r=@488@/>"
			L"<c type=@Edit@  n=@FileName@ anc=@rt@ l=@85@ t=@12@ w=@260@ h=@24@ anc_r=@202@/>"
			L"<c type=@ComboBox@ n=@FileFilter@ anc=@rt@ l=@350@ t=@12@ w=@172@ h=@24@ anc_r=@20@ />"
			L"<c type=@Button@ n=@OK@ tx=@打开@ l=@400@ t=@48@ w=@80@ h=@20@ anc=@rt@ anc_r=@120@ />"
			L"<c type=@Button@ n=@Cancel@ tx=@取消@ l=@500@ t=@48@ w=@80@ h=@20@ anc=@rt@ anc_r=@20@ />"
		L"</children>"
		L"</c>"
	L"</children>"
	L"</c>"
	;

AFileDialogForm::AFileDialogForm(AComponent* pOwner):AForm(pOwner)
{
	m_nButtonID = NULL;
	m_sFileName = NULL;
	m_bOpenMode = true;

	MAP_CONTROL_INIT(FileFilter);
	MAP_CONTROL_INIT(DirView);
	MAP_CONTROL_INIT(FileView);
	MAP_CONTROL_INIT(OK);
	MAP_CONTROL_INIT(Cancel);
	MAP_CONTROL_INIT(FileName);
	MAP_CONTROL_INIT(ButtonsPanel);
	MAP_CONTROL_INIT(IconImageList);
	MAP_CONTROL_INIT(TitleBar);
	MAP_CONTROL_INIT(MenuBarPanel);
}

AFileDialogForm::~AFileDialogForm()
{

}

void AFileDialogForm::CreateFromXML()
{
	afc_object<AMLBody> pBody(new AMLBody());
	AString xml;
	xml = g_szDialogWnd;
	xml.Replace(_T("@"),_T("\""));
	pBody->FromXML(xml);
	this->FromXML(&pBody);
	this->CenterToScreen();
}

void AFileDialogForm::ConnectEvent()
{
	EVENT_CONNECT(GetMenuBarPanel(),OnMouseDown,TitleBackDoMouseDown);
	EVENT_CONNECT(GetMenuBarPanel(),OnMouseMove,TitleBackDoMouseMove);
	EVENT_CONNECT(GetButtonsPanel(),OnMouseDown,TitleBackDoMouseDown);
	EVENT_CONNECT(GetButtonsPanel(),OnMouseMove,TitleBackDoMouseMove);
	EVENT_CONNECT(GetOK(),OnClick,OnOKClick);
	EVENT_CONNECT(GetCancel(),OnClick,OnCancelClick);
	EVENT_CONNECT(GetDirView(),OnChange,OnDirChange);
	EVENT_CONNECT(GetFileView(),OnClick,OnFileListClick);
	EVENT_CONNECT(GetFileView(),OnDblClick,OnFileListDblClick);
}

void AFileDialogForm::TitleBackDoMouseDown(AObject* pSender,AEvent* pEvent)
{
	GetTitleBar()->DoMouseDown(pEvent);
}
void AFileDialogForm::TitleBackDoMouseMove(AObject* pSender,AEvent* pEvent)
{
	GetTitleBar()->DoMouseMove(pEvent);
}

void AFileDialogForm::OnOKClick(AObject* pSender,AEvent* pEvent)
{
	
	if( m_sFileName )
	{
		AString sFileName;
		ATreeViewItem* pSelNode = GetDirView()->GetSelected();
		if( pSelNode )
		{
			std::map<ATreeViewItem*,AString>::iterator it = m_aPath.find(pSelNode);
			if( it != m_aPath.end() ) sFileName = it->second;
		}
		AString st = GetFileName()->GetText();
		if( st != _T("") )
		{
			sFileName.SureEndWith(_T("\\"));
			sFileName += st;
		}
		else
		{
			AApplication::Get()->MsgBox(_T("请选择文件！"));
			return;
		}
		*m_sFileName = sFileName;
		if( AFile::IsExists(sFileName) == false )
		{
			//提示是否覆盖 fix me later AApplication::Get()->MsgBox(_T("请选择存在的文件！"));
			//return;
		}
	}
	if( m_nButtonID ) 
	{
		*m_nButtonID = dbOK;
	}
	Destroy();
}
void AFileDialogForm::OnCancelClick(AObject* pSender,AEvent* pEvent)
{
	if( m_nButtonID ) *m_nButtonID = dbCancel;
	Destroy();
}



void AFileDialogForm::InitControls()
{
	
}

void AFileDialogForm::SetFilter(const AString& sFilter,int iDefaultIndex)
{
	AStringArray saFilter;
	saFilter.FromString(sFilter,_T("|"));
	AStringArrayEnumerator em(&saFilter);

	m_pUICore->SetLayoutEnabled(false);
	int index=-1;
	while( em.Next() )
	{
		AString s;
		s = em.Cur();
		GetFileFilter()->AddString(s);
		index ++;
		if( index == iDefaultIndex ) GetFileFilter()->SetText(s);
		if( !em.Next() ) break;
		s = em.Cur();
		
	}
	m_pUICore->SetLayoutEnabled(true);
	GetFileFilter()->Refresh();
	//GetFileFilter()->DoLayout();
}

void AFileDialogForm::DoCreate()
{
	if( m_bOpenMode == false ) 
	{
		SetText(_T("保存"));
		GetOK()->SetText(_T("保存"));
	}
	GetDirView()->SetImageList(GetIconImageList());
	GetFileView()->SetSmallImageList(GetIconImageList());
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
	FillFiles(m_sInitDir);
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

void AFileDialogForm::FillNode(ATreeViewItem* pNode)
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
			AString s,sText;
			s = sz;
			if( s.ReverseFind(_T("\\")) > -1 ) s = s.Left(s.Length()-1);
			sText = AString(Volumelabel) + _T(" (") + s + _T(")");
			ATreeViewItem* pItem = tv->AddItem(pComputeNode,sText);
			pItem->SetExpanded(true);
			pItem->SetImageIndex( GetIconImageList()->GetIndex(_T("Folder")) );
			m_aPath[ pItem ] = s;
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
				dl.ListReadOnly(true);
		
				while( dl.Next() )
				{
					if( dl.IsDirectory() )
					{
						ATreeViewItem* pChildNode = GetDirView()->AddItem(pNode,dl.Cur());
						pChildNode->SetExpanded(true);
						pChildNode->SetImageIndex( GetIconImageList()->GetIndex(_T("Folder")) );
						m_aPath[ pChildNode ] = dl.CurWithPath();
					}
				}
			}
		}
		pNode->SetExpanded( !pNode->GetExpanded() );
		m_pUICore->SetLayoutEnabled(true);
		GetDirView()->DoLayout();
	}
}

void AFileDialogForm::OnDirChange(AObject* pSender,AEvent* pEvent)
{
	ATreeViewItem* pSel = GetDirView()->GetSelected();
	std::map<ATreeViewItem*,AString>::iterator it = m_aPath.find(pSel);
	if( it != m_aPath.end() )
	{
		DebugStr(_T("OnDirChange FillNode..."));
		FillNode(pSel);
		DebugStr(_T("OnDirChange FillFiles..."));
		FillFiles(it->second);
		DebugStr(_T("OnDirChange AfterFill..."));
		pSel->SetExpanded( !pSel->GetExpanded() );
	}
}

void AFileDialogForm::FillFiles(const AString& sPath)
{
	GetFileView()->RemoveAllItems();
	if( sPath == _T("") ) return;

	//m_pUICore->SetLayoutEnabled(false);

	ADirectoryEnumerator dl(sPath);
	dl.ListFile(true);
	dl.ListDirectory(true);
	dl.ListReadOnly(true);
	DebugStr(_T("   >>> 开始遍历文件  <<"));
	while(dl.Next())
	{
  		AListViewItem* pItem = GetFileView()->AddItem();
		pItem->SetText(dl.Cur());//名称
		pItem->SubItems.Add(dl.GetModifyTime().Format());//修改时间
		pItem->SubItems.Add(dl.IsDirectory() ? _T("目录") : _T("文件"));//类型
		pItem->SubItems.Add(dl.GetSizeString());//文件大小

		AString sFile;
		sFile = dl.Cur();
		int iPos = sFile.ReverseFind(_T("."));
		AString sExt;
		if( iPos > -1 ) sExt = sFile.Right(sFile.Length()-iPos);
		if( dl.IsDirectory())
			pItem->SetImageIndex( GetIconImageList()->GetIndex(_T("Folder")));
		else
			pItem->SetImageIndex( GetIconImageList()->GetIndex(sExt));
	}
	DebugStr(AString().Format(_T("   >> 结束遍历文件 %d个<<   "),dl.Index()).Text());
	//m_pUICore->SetLayoutEnabled(true);
	GetFileView()->DoLayout();
}

void AFileDialogForm::OnFileListClick(AObject* pSender,AEvent* pEvent)
{
	AListViewItem* pSel = GetFileView()->GetSelected();
	if( pSel == NULL ) return;
	if( pSel->SubItems.GetItem(1) == _T("目录") ) return;

	GetFileName()->SetText( pSel->GetText() );
	GetFileName()->Refresh();
}

void AFileDialogForm::OnFileListDblClick(AObject* pSender,AEvent* pEvent)
{
	ATreeViewItem* pDirSel = GetDirView()->GetSelected();
	if( pDirSel == NULL ) return;
	std::map<ATreeViewItem*,AString>::iterator it = m_aPath.find(pDirSel);
	if( it == m_aPath.end() ) return;

	AListViewItem* pSel = GetFileView()->GetSelected();
	if( pSel == NULL ) return;
	if( pSel->SubItems.GetItem(1) == _T("目录") ) 
	{
		AString sDir = it->second;
		AString sFoldName = pSel->GetText();
		sDir += _T("\\")+sFoldName;
		LocateTo(sDir);
		FillFiles(sDir);
// 		for(int i=0;i<pDirSel->GetChildItemCount();i++)
// 		{
// 			ATreeViewItem* pItem = pDirSel->GetChildItem(i);
// 			if( pItem->GetText() == sFoldName )
// 			{
// 				pItem->SetExpanded(true);
// 				GetDirView()->SetSelected(pItem);
// 
// 				LocateTo(sDir);
// 				FillFiles(sDir);
// 				break;
// 			}
// 		}
	}
}

void AFileDialogForm::LocateTo(const AString& sPath)
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