#include "StdAfx.h"
#include "resource.h"
#include "MainForm.h"
#include "AAboutForm.h"
#include "ui/MainForm.ui"
#include "UIDrawer.h"
#include "UIXMLGen.h"
#include "UIHeaderGen.h"

REG_CLASS(AMainForm);
AMainForm* AMainForm::CreateMe()
{
	afc_object<AMLBody> pBody(new AMLBody());
	AString xml;
	xml = g_szFormXML;
	xml.Replace(_T("@"),_T("\""));
	pBody->FromXML(xml);
	AMainForm* p = dynamic_cast<AMainForm*>( AControl::CreateInstance(&pBody,AApplication::Get()) );
	return p;
}

AMainForm::AMainForm(AComponent* pOwner):
	base_class(pOwner),
	m_pForm(NULL)
{
	MAP_CONTROL_INIT(mnAbout);
	MAP_CONTROL_INIT(MenuBar);
	MAP_CONTROL_INIT(TitleBar);
	MAP_CONTROL_INIT(Exit);
	MAP_CONTROL_INIT(Box);
	MAP_CONTROL_INIT(BoxContainer);
	MAP_CONTROL_INIT(PropEdit);
	MAP_CONTROL_INIT(SaveUI);
	MAP_CONTROL_INIT(OpenUI);
	MAP_CONTROL_INIT(PreviewUI);
	MAP_CONTROL_INIT(ClosePreview);
	MAP_CONTROL_INIT(SaveDialog);
	MAP_CONTROL_INIT(OpenDialog);
	MAP_CONTROL_INIT(GenUI);
	MAP_CONTROL_INIT(GenUIH);
}

AMainForm::~AMainForm()
{

}

void AMainForm::DoCreate()
{
	SetIcon(::LoadIcon(AApplication::Get()->GetHandle(), MAKEINTRESOURCE(IDI_ICON1)));

	m_pVB = new AVertScrollBar(GetBoxContainer());
	m_pHB = new AHorzScrollBar(GetBoxContainer());
	GetBoxContainer()->SetVScrollBar(m_pVB);
	GetBoxContainer()->SetHScrollBar(m_pHB);

	m_Doc = new CUIDoc();
	EVENT_CONNECT(m_Doc,m_OnRefresh,OnRefresh);
	m_pUIPropMgr = new CUIPropMgr(&m_Doc,GetPropEdit());
}

void AMainForm::ConnectEvent()
{
	base_class::ConnectEvent();

	EVENT_CONNECT(GetmnAbout(),OnClick,OnAboutClick);
	EVENT_CONNECT(GetMenuBar(),OnMouseDown,OnMenuBarMouseDown);
	EVENT_CONNECT(GetMenuBar(),OnMouseMove,OnMenuBarMouseMove);
	EVENT_CONNECT(GetExit(),OnClick,OnExitExecute);
	EVENT_CONNECT(this,OnCloseQuery,OnCloseQueryExecute);

	EVENT_CONNECT(GetBox(),OnPaint,OnBoxPaint);
	EVENT_CONNECT(GetBox(),OnDragOver,OnBoxDragOver);
	EVENT_CONNECT(GetBox(),OnDragDrop,OnBoxDragDrop);
	EVENT_CONNECT(GetBox(),OnMouseDown,OnBoxMouseDown);
	EVENT_CONNECT(GetBox(),OnMouseMove,OnBoxMouseMove);
	EVENT_CONNECT(GetBox(),OnMouseUp,OnBoxMouseUp);
	EVENT_CONNECT(GetBox(),OnKeyboard,OnKeyboardExecute);
	EVENT_CONNECT(GetSaveUI(),OnClick,OnSaveExecute);
	EVENT_CONNECT(GetOpenUI(),OnClick,OnOpenExecute);
	EVENT_CONNECT(GetPreviewUI(),OnClick,OnPreviewExecute);
	EVENT_CONNECT(GetClosePreview(),OnClick,OnClosePreviewExecute);
	EVENT_CONNECT(GetGenUI(),OnClick,OnGenUIExecute);
	EVENT_CONNECT(GetGenUIH(),OnClick,OnGenUIHExecute);

	m_pActionList = new AActionList(this);
	AAction* pAction = m_pActionList->AddAction(_T("LeftAlign"));
	EVENT_CONNECT(pAction,OnExecute,OnLeftAlign);
	EVENT_CONNECT(pAction,OnUpdate,OnLeftAlignUpdate);

	pAction = m_pActionList->AddAction(_T("RightAlign"));
	EVENT_CONNECT(pAction,OnExecute,OnRightAlign);
	EVENT_CONNECT(pAction,OnUpdate,OnRightAlignUpdate);

	pAction = m_pActionList->AddAction(_T("TopAlign"));
	EVENT_CONNECT(pAction,OnExecute,OnTopAlign);
	EVENT_CONNECT(pAction,OnUpdate,OnTopAlignUpdate);

	pAction = m_pActionList->AddAction(_T("BottomAlign"));
	EVENT_CONNECT(pAction,OnExecute,OnBottomAlign);
	EVENT_CONNECT(pAction,OnUpdate,OnBottomAlignUpdate);

	pAction = m_pActionList->AddAction(_T("MiddleAlign"));
	EVENT_CONNECT(pAction,OnExecute,OnMiddleAlign);
	EVENT_CONNECT(pAction,OnUpdate,OnMiddleAlignUpdate);

	pAction = m_pActionList->AddAction(_T("CenterAlign"));
	EVENT_CONNECT(pAction,OnExecute,OnCenterAlign);
	EVENT_CONNECT(pAction,OnUpdate,OnCenterAlignUpdate);

	pAction = m_pActionList->AddAction(_T("HorzSpaceAlign"));
	EVENT_CONNECT(pAction,OnExecute,OnHorzSpaceAlign);
	EVENT_CONNECT(pAction,OnUpdate,OnHorzSpaceAlignUpdate);

	pAction = m_pActionList->AddAction(_T("VertSpaceAlign"));
	EVENT_CONNECT(pAction,OnExecute,OnVertSpaceAlign);
	EVENT_CONNECT(pAction,OnUpdate,OnVertSpaceAlignUpdate);

	pAction = m_pActionList->AddAction(_T("SameHeightAlign"));
	EVENT_CONNECT(pAction,OnExecute,OnSameHeightAlign);
	EVENT_CONNECT(pAction,OnUpdate,OnSameHeightAlignUpdate);

	pAction = m_pActionList->AddAction(_T("SameWidthAlign"));
	EVENT_CONNECT(pAction,OnExecute,OnSameWidthAlign);
	EVENT_CONNECT(pAction,OnUpdate,OnSameWidthAlignUpdate);

	pAction = m_pActionList->AddAction(_T("SameSizeAlign"));
	EVENT_CONNECT(pAction,OnExecute,OnSameSizeAlign);
	EVENT_CONNECT(pAction,OnUpdate,OnSameSizeAlignUpdate);

	pAction = m_pActionList->AddAction(_T("Copy"));
	EVENT_CONNECT(pAction,OnExecute,OnCopyExecute);

	pAction = m_pActionList->AddAction(_T("Paste"));
	EVENT_CONNECT(pAction,OnExecute,OnPasteExecute);

	pAction = m_pActionList->AddAction(_T("Delete"));
	EVENT_CONNECT(pAction,OnExecute,OnDeleteExecute);

	pAction = m_pActionList->AddAction(_T("Cut"));
	EVENT_CONNECT(pAction,OnExecute,OnCutExecute);

	pAction = m_pActionList->AddAction(_T("Lock"));
	EVENT_CONNECT(pAction,OnExecute,OnLockExecute);


}

void AMainForm::OnAboutClick(AObject* pSender,AEvent* pEvent)
{
	AAboutForm* p = AAboutForm::CreateMe();
	p->CenterToScreen();
	p->ShowModal();
}

void AMainForm::OnMenuBarMouseDown(AObject* pSender,AEvent* pEvent)
{
	GetTitleBar()->DoMouseDown(pEvent);
}

void AMainForm::Save()
{
	m_Doc->SaveToFile(m_Doc->GetFileName());
	m_Doc->SetModified(false);
}

int AMainForm::TrySave()
{
	if( m_Doc->GetModified() )
	{
		EString str;
		str = L"是否保存对[";
		if( m_Doc->GetFileName() == L"" ) str += GetText();
		else str += m_Doc->GetFileName();
		str += L"]所作的修改？";

		TDialogButton db = AMessageBox::Question(str);

		if( db == dbCancel )
		{
			return dbCancel;
		}
		else if( db == dbYes )
		{
			m_Doc->SaveToFile(m_Doc->GetFileName());
			return dbYes;
		}
		else
		{
			return dbNo;
		}
	}
	else
	{
		return dbYes;
	}
}

void AMainForm::OnMenuBarMouseMove(AObject* pSender,AEvent* pEvent)
{
	GetTitleBar()->DoMouseMove(pEvent);
}

void AMainForm::OnExitExecute(AObject* pSender,AEvent* pEvent)
{
	Destroy();
}

void AMainForm::OnRefresh(AObject* pSender,AEvent* pEvent)
{
	if( m_Doc != ANULL )
	{
		afc_object<CControl> p;
		p = m_Doc->GetFocus();
		if( p != ANULL )
		{
			APoint ptMove = m_Doc->GetMove();
			APoint ptMovePrev = m_Doc->GetMovePrev();

			APoint ptDown = m_Doc->GetDown();
			int dx = ptMove.x - ptDown.x;
			int dy = ptMove.y - ptDown.y;
			ARect rt = p->GetAbsPosition();
			ARect r=rt;
			r.Inflate(HANDLE_WIDTH,HANDLE_WIDTH);
			r = m_Doc->GetChangePos(r,m_Doc->GetCurHitResult().m_iHandle,dx,dy);
			
			ARect rx = GetBoxContainer()->GetAbsPosition();
			r.Offset(rx.left,rx.top);
			m_pUICore->Refresh(r);

			dx = ptMovePrev.x - ptDown.x;
			dy = ptMovePrev.y - ptDown.y;
			r = rt;
			r.Inflate(HANDLE_WIDTH,HANDLE_WIDTH);
			r = m_Doc->GetChangePos(r,m_Doc->GetCurHitResult().m_iHandle,dx,dy);
 			r.Offset(rx.left,rx.top);
 			m_pUICore->Refresh(r);
		}
	}
	//GetBoxContainer()->Refresh();
}

void AMainForm::OnCloseQueryExecute(AObject* pSender,AEvent* pEvent)
{
	ACloseQueryEvent* evt = dynamic_cast<ACloseQueryEvent*>(pEvent);
	if( evt == NULL ) return;

	if( m_Doc->GetModified() )
	{
		TDialogButton db = (TDialogButton)AMessageBox::Question(_T("文档已修改，是否保存？"));
		if( db == dbCancel ) 
		{
			evt->m_bCanClose = false;
			return;
		}
		if( db == dbYes ) Save();
	}
	
	for(int i=0;i<m_aPreviewForm.GetCount();i++)
	{
		//AApplication::Discard(m_aPreviewForm[i]);
	}
	m_aPreviewForm.Clear();
	evt->m_bCanClose = true;//
}

void AMainForm::OnBoxPaint(AObject* pSender,AEvent* pEvent)
{
	ARect r = GetBox()->GetAbsPosition();
	if( !GetUICore()->InDrawArea(r) ) return;
	ACanvas* cs = GetUICore()->GetCanvas();

	//time_counter tc;
// 	ALinearGradientBrush br;
// 	br.SetStartColor(AColor(255,200,232,227));
// 	br.SetEndColor(AColor(255,255,255,255));
// 	br.SetRect(r);
// 	br.IsVert(true);
// 	cs->Use(&br)->FillRect(r);

	ASolidBrush br( AColor(255,200,232,227) );
	cs->Use(&br)->FillRect(r);

	CUIDrawer d(&m_Doc,cs,r);
	d.Draw();

	APen pen(1,AApplication::Get()->GetCurSkin()->m_BorderColor,DashStyleSolid);
	cs->Use(&pen)->DrawRect(r);

	//tc.Print(L"OnBoxPaint-----------");
}

void AMainForm::OnBoxDragOver(AObject* pSender,AEvent* pEvent)
{
	ADragEvent* evt = dynamic_cast<ADragEvent*>(pEvent);
	if( evt == NULL ) return;

	ATreeView* plv = dynamic_cast<ATreeView*>(evt->m_pSource);
	if( plv == NULL || plv->GetName() != _T("ToolTree") ) evt->m_bAccept = false;
	else evt->m_bAccept = true;
}
void AMainForm::OnBoxDragDrop(AObject* pSender,AEvent* pEvent)
{
	ADragEvent* evt = dynamic_cast<ADragEvent*>(pEvent);
	if( evt == NULL ) return;

	ATreeView* plv = dynamic_cast<ATreeView*>(evt->m_pSource);
	if( plv == NULL || plv->GetName() != _T("ToolTree") ) return;

	ATreeViewItem* pSel = plv->GetSelected();
	if( pSel == NULL ) return;

	CONFIG_BODY pBodyFrom;
	pBodyFrom = (AMLBody *)(void *)pSel->GetTag();
	if( pBodyFrom == ANULL ) return;

	afc_object<CControl> pSource;
	pSource = CControl::CreateInstance( pBodyFrom );

	if( pSource->GetType() == _ES("Form") )
	{
		if( m_Doc->GetWebForm() != ANULL )
		{
			AMessageBox::Infor(_ES("表单控件只能有一个！"));
			return;
		}
	}
	else
	{
		if( m_Doc->GetWebForm() == ANULL )
		{
			AMessageBox::Infor(_ES("添加控件前请先添加表单控件！"));
			return;
		}
	}

	ARect rBox = GetBox()->GetAbsPosition();
	int X = evt->m_X - rBox.left;
	int Y = evt->m_Y - rBox.top;

	afc_object<CControl> pc;
	THitResult hr = m_Doc->HitTest(X,Y);
	afc_object<CControl> pContainer = hr.m_pBase;
	if( pContainer != ANULL )
	{
		if( pContainer->m_pDefNode->GetProp(_ES("iscontainer")) == _ES("1") ) pc =pContainer;
		else
		{
			while( true )
			{
				if( pContainer->m_pParent == NULL )
				{
					pc = m_Doc->GetWebForm();
					break;
				}
				if( pContainer->m_pParent->m_pDefNode->GetProp(_ES("iscontainer")) == _ES("1") )
				{
					pc = pContainer->m_pParent;
					break;
				}
				pContainer = pContainer->m_pParent;
			}
		}
	}
	else
		pc = m_Doc->GetWebForm();

	if( pSource->GetType() == _ES("Form") ) pc = ANULL;

	BVRect r=pSource->GetPosition();
	r.Offset(X,Y);
	if( r.GetWidth() < 10 ) r.right = r.left+10;
	if( r.GetHeight() < 10 ) r.bottom = r.top + 10;;
	//设置Name, Position(X,Y,100,100) ??

	BVRect pcp;
	if( pc != ANULL ) 
	{
		pcp = pc->GetAbsPosition();
	}
	r.Offset(-pcp.left,-pcp.top);

	pSource->SetPosition(r);
	ARect x = pSource->GetPosition();

	EString sName;
	sName = m_Doc->NewControlName(pSource->GetType());
	(*pSource)[_ES("n")]->AsString( sName );
	(*pSource)[_ES("tx")]->AsString( sName );

	if( pc != ANULL )
	{
		pc->AddChild( pSource );
		pSource->m_pParent = &pc;
	}
	else
	{
		m_Doc->SetWebForm( pSource );
	}
	m_Doc->SetModified(true);
	GetBox()->Refresh();
}

void AMainForm::OnBoxMouseDown(AObject* pSender,AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL ) return;

	time_counter tc;

	ARect rBox = GetBox()->GetAbsPosition();
	int X = evt->m_X - rBox.left;
	int Y = evt->m_Y - rBox.top;
	tc.Print(L"before HitTest");
	THitResult hr = m_Doc->HitTest(X,Y );
	tc.Print(L"after hittest");
	afc_object<CControl> p = hr.m_pBase;
	if( p != ANULL)
	{
		//OutputDebugString("2");
		if( evt->m_KeyStates.Has(ksLeftButton) )
		{
			m_Doc->SetDown( BVPoint(X,Y) );
			m_Doc->SetMove( BVPoint(X,Y) );
		}
		if( m_Doc->IsSelected( p ) )
		{
			if( evt->m_KeyStates.Has(ksShift) )
			{
				m_Doc->SetSelected(p,false);
				time_counter c;
				OnRefresh(this,NULL);
				c.Print(L"OnRefresh");
			}
		}
		else
		{
			if( !evt->m_KeyStates.Has(ksShift) ) m_Doc->UnSelectAll();
			m_Doc->SetSelected(p,true);
			//OutputDebugString("6");
			//m_pUIPropMgr->ShowProp(p);
			//OutputDebugString("7");
			OnRefresh(this,NULL);
			//OutputDebugString("8");
		}
		//time_counter tc2;
		//m_pUIPropMgr->ShowProp(p);
		//tc2.Print(L"ShowProp");
	}
	else
	{
		//OutputDebugString("4");
		//m_pUIPropMgr->ShowProp(afc_object<CControl>());


		int iCount = m_Doc->GetSelectedCount();
		if(iCount)
		{
			m_Doc->UnSelectAll();
		}
		OnRefresh(this,NULL);
	}
	m_Doc->SetCurHitResult( hr );

	tc.Print(L"OnBoxMouseDown");
}
void AMainForm::OnBoxMouseMove(AObject* pSender,AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL ) return;

	//time_counter tc;

	ARect rBox = GetBox()->GetAbsPosition();
	int X = evt->m_X - rBox.left;
	int Y = evt->m_Y - rBox.top;

	if( evt->m_KeyStates.Has(ksLeftButton) )
		m_Doc->SetMove( BVPoint(X,Y) );
	if( evt->m_KeyStates.Has(ksLeftButton) && m_Doc->IsMoving() && m_Doc->GetCurHitResult().m_iHandle != HANDLE_DUMMY )
	{//do nothing,保持不变
	}
	else
	{//重新测试
		//OutputDebugString("before hittest");
		//time_counter c;
		THitResult hr = m_Doc->HitTest(X,Y);
		//c.Print(L"OnBoxMouseMove.HitTest used time");
		//OutputDebugString("after hittest");
		TCursorType iNewCursor = ctArrow;
		switch( hr.m_iHandle )
		{
		case HANDLE_DUMMY:
			iNewCursor = ctArrow;
			break;
		case HANDLE_INME:
			iNewCursor = ctArrow;
			break;
		case HANDLE_LEFTTOP:
			iNewCursor = ctSizeNWSE;
			break;
		case HANDLE_TOP:
			iNewCursor = ctSizeNS;
			break;
		case HANDLE_RIGHTTOP:
			iNewCursor = ctSizeNESW;
			break;
		case HANDLE_RIGHT:
			iNewCursor = ctSizeWE;
			break;
		case HANDLE_RIGHTBOTTOM:
			iNewCursor = ctSizeNWSE;
			break;
		case HANDLE_BOTTOM:
			iNewCursor = ctSizeNS;
			break;
		case HANDLE_LEFTBOTTOM:
			iNewCursor = ctSizeNESW;
			break;
		case HANDLE_LEFT:
			iNewCursor = ctSizeWE;
			break;
		default:
			break;
		}
		GetBox()->SetCursorType(iNewCursor);
		m_Doc->SetCurHitResult( hr );
	}

	if( m_Doc->IsMoving() && evt->m_KeyStates.Has(ksLeftButton) )
	{
		OnRefresh(this,NULL);
	}	
	//tc.Print(L"OnBoxMouseMove used time.");
}
void AMainForm::OnBoxMouseUp(AObject* pSender,AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL ) return;

	ARect rBox = GetBox()->GetAbsPosition();
	int X = evt->m_X - rBox.left;
	int Y = evt->m_Y - rBox.top;

	if( m_Doc->GetLocked() == false && m_Doc->IsMoving() )
	{
		BVPoint ptDown = m_Doc->GetDown();
		BVSize siz(X-ptDown.x,Y-ptDown.y);
		SELECTED_TYPE& aSelected = m_Doc->GetSelected();
		for(SELECTED_TYPE::iterator it=aSelected.begin();it!=aSelected.end();it++)
		{
			afc_object<CControl> pBase;
			pBase = it->first;
			BVRect cp = pBase->GetAbsPosition();
			BVRect r = cp;

			switch( m_Doc->GetCurHitResult().m_iHandle )
			{
			case HANDLE_DUMMY:
				break;
			case HANDLE_INME:
				r.Offset(siz.cx,siz.cy);
				break;
			case HANDLE_LEFTTOP:
				r.left += siz.cx;
				r.top += siz.cy;
				break;
			case HANDLE_TOP:
				r.top += siz.cy;
				break;
			case HANDLE_RIGHTTOP:
				r.right += siz.cx;
				r.top += siz.cy;
				break;
			case HANDLE_RIGHT:
				r.right += siz.cx;
				break;
			case HANDLE_RIGHTBOTTOM:
				r.right += siz.cx;
				r.bottom += siz.cy;
				break;
			case HANDLE_BOTTOM:
				r.bottom += siz.cy;
				break;
			case HANDLE_LEFTBOTTOM:
				r.left += siz.cx;
				r.bottom += siz.cy;
				break;
			case HANDLE_LEFT:
				r.left += siz.cx;
				break;
			default:
				break;
			}

			if( pBase->m_pParent )
			{
				BVRect pcp = pBase->m_pParent->GetAbsPosition();
				r.Offset(-pcp.left,-pcp.top);
			}
			pBase->SetPosition( r );
			ARect xPos = pBase->GetPosition();

		}
		m_Doc->SetModified(true);
	}
	m_Doc->SetDown( BVPoint(-1,-1) );
	m_Doc->SetMove( BVPoint(-1,-1) );
	if( evt->m_KeyStates.Has(ksLeftButton))
	{
		m_pUIPropMgr->ShowProp(m_Doc->GetFocus());
		OnRefresh(this,NULL);
	}
	afc_object<CControl> pFocus;
	pFocus = m_Doc->GetFocus();
	//if( m_pUIPropMgr->GetCurControl() != &pFocus )
		m_pUIPropMgr->ShowProp( pFocus );
}

void AMainForm::OnKeyboardExecute(AObject* pSender,AEvent* pEvent)
{
	AKeyEvent* evt = dynamic_cast<AKeyEvent*>(pEvent);
	if( evt == NULL ) return;

	WCHAR ch = evt->m_chKey;
	if( evt->m_Type == ketPress )
	{
		if( ch >= 0x20 )
		{
			//
		}
	}
	else if( evt->m_Type == ketDown )
	{
		if( ch == 'C' && evt->m_KeyStates.Has(ksCtrl) )
			DoCopy();
		else if( ch == 'V' && evt->m_KeyStates.Has(ksCtrl) )
			DoPaste();
		else if( ch == 'X' && evt->m_KeyStates.Has(ksCtrl) )
		{
			DoCopy();
			DoDelete();
		}
		else if( ch == 'L' && evt->m_KeyStates.Has(ksCtrl) )
			OnLockExecute(this,pEvent);
		else if( ch == VK_DELETE )
			DoDelete();
		else if( ch ==  VK_ESCAPE )
		{
			if( &m_Doc != NULL )
			{
				afc_object<CControl> pParent;
				pParent = m_Doc->GetFocus();
				if( &pParent != NULL ) pParent = pParent->m_pParent;
				if( &pParent == NULL )
				{
					pParent = m_Doc->GetWebForm();
					if( pParent == ANULL ) return;
				}
				m_Doc->UnSelectAll();
				m_Doc->SetSelected(pParent,true);
				m_pUIPropMgr->ShowProp( pParent );
				OnRefresh(this,NULL);
			}
		}
		else if( ch == VK_LEFT )
		{
			m_Doc->MoveSelected(evt->m_KeyStates.Has(ksCtrl) ? -10 : -1,0);
			GetBox()->Refresh();
		}
		else if( ch == VK_RIGHT )
		{
			m_Doc->MoveSelected(evt->m_KeyStates.Has(ksCtrl) ? 10 : 1,0);
			GetBox()->Refresh();
		}
		else if( ch == VK_UP )
		{
			m_Doc->MoveSelected(0,evt->m_KeyStates.Has(ksCtrl) ? -10 : -1);
			GetBox()->Refresh();
		}
		else if( ch == VK_DOWN )
		{
			m_Doc->MoveSelected(0,evt->m_KeyStates.Has(ksCtrl) ? 10 : 1);
			GetBox()->Refresh();
		}
	}
}

void AMainForm::OnLeftAlign(AObject* pSender,AEvent* pEvent)
{
	m_Doc->OnAlignLeft();
}
void AMainForm::OnTopAlign(AObject* pSender,AEvent* pEvent)
{
	m_Doc->OnAlignTop();
}
void AMainForm::OnRightAlign(AObject* pSender,AEvent* pEvent)
{
	m_Doc->OnAlignRight();
}
void AMainForm::OnBottomAlign(AObject* pSender,AEvent* pEvent)
{
	m_Doc->OnAlignBottom();
}
void AMainForm::OnCenterAlign(AObject* pSender,AEvent* pEvent)
{
	m_Doc->OnAlignCenter();
}
void AMainForm::OnMiddleAlign(AObject* pSender,AEvent* pEvent)
{
	m_Doc->OnAlignMiddle();
}
void AMainForm::OnHorzSpaceAlign(AObject* pSender,AEvent* pEvent)
{
	m_Doc->OnAlignHorzSpace();
}
void AMainForm::OnVertSpaceAlign(AObject* pSender,AEvent* pEvent)
{
	m_Doc->OnAlignVertSpace();
}
void AMainForm::OnSameHeightAlign(AObject* pSender,AEvent* pEvent)
{
	m_Doc->OnAlignSameHeight();
}
void AMainForm::OnSameWidthAlign(AObject* pSender,AEvent* pEvent)
{
	m_Doc->OnAlignSameWidth();
}
void AMainForm::OnSameSizeAlign(AObject* pSender,AEvent* pEvent)
{
	m_Doc->OnAlignSameSize();
}

void AMainForm::OnLeftAlignUpdate(AObject* pSender,AEvent* pEvent)
{

}
void AMainForm::OnTopAlignUpdate(AObject* pSender,AEvent* pEvent)
{

}
void AMainForm::OnRightAlignUpdate(AObject* pSender,AEvent* pEvent)
{

}
void AMainForm::OnBottomAlignUpdate(AObject* pSender,AEvent* pEvent)
{

}
void AMainForm::OnCenterAlignUpdate(AObject* pSender,AEvent* pEvent)
{

}
void AMainForm::OnMiddleAlignUpdate(AObject* pSender,AEvent* pEvent)
{

}
void AMainForm::OnHorzSpaceAlignUpdate(AObject* pSender,AEvent* pEvent)
{

}
void AMainForm::OnVertSpaceAlignUpdate(AObject* pSender,AEvent* pEvent)
{

}
void AMainForm::OnSameHeightAlignUpdate(AObject* pSender,AEvent* pEvent)
{

}
void AMainForm::OnSameWidthAlignUpdate(AObject* pSender,AEvent* pEvent)
{

}
void AMainForm::OnSameSizeAlignUpdate(AObject* pSender,AEvent* pEvent)
{

}


void AMainForm::DoCopy()
{
	CONFIG_BODY pRoot(new AMLBody());
	pRoot->SetName( _T("ControlsCopied") );

	SELECTED_TYPE& aSel = m_Doc->GetSelected();
	for(SELECTED_TYPE::iterator it=aSel.begin();it!=aSel.end();it++)
	{
		afc_object<CControl> pc;
		pc = it->first;

		/*bool bParentSel = false;
		CControl* pParent = pc->m_pParent;
		while( true )
		{
			if( pParent == NULL ) break;
			afc_object<CControl> x;
			x = pParent;
			if( m_Doc->IsSelected(x) )
			{
				bParentSel = true;
				break;
			}
			else pParent = pParent->m_pParent;
		}
		if( !bParentSel )
		{
			CONFIG_BODY px;
			px = pc->m_Config.m_aBody[0];
			pRoot->AddBody( px );
		} */
		CONFIG_BODY pn;
		pn = new AMLBody();
		pn->SetName(L"Control");

		afc_object<CControl> pcCopy;
		pcCopy = pc->Clone();

		pn->SetProp(L"text",pcCopy->m_Config.GetRootBody()->ToString());
		pn->SetProp(L"def",pcCopy->m_pDefNode->ToString());
		pRoot->AddBody( &pn );
	}

	afc_object<AClipboard> pClip(new AClipboard(this));
	pClip->SetText( pRoot->ToString() );
}

void AMainForm::DoPaste()
{
	afc_object<AClipboard> pClip(new AClipboard(this));
	EString s;
	s = pClip->GetText();

	IConfig fig;
	fig.LoadText(s);
	//
	CONFIG_BODY pRoot;
	pRoot = fig.FindBody(_ES("ControlsCopied"));
	if( pRoot != ANULL )
	{
		afc_object<CControl> pParent;
		pParent = m_Doc->GetFocus();
		if( &pParent != NULL && pParent->GetType() != L"Box" ) pParent = pParent->m_pParent;
		if( &pParent == NULL )
		{
			pParent = m_Doc->GetWebForm();
			if( pParent == ANULL ) return;
		}
		for(int i=0;i<pRoot->GetChildCount();i++)
		{
			afc_object<CControl> c;
			c = new CControl();
			c->m_Config.LoadText( pRoot->GetChild(i)->GetProp(L"text") );
			IConfig fg;
			fg.LoadText(pRoot->GetChild(i)->GetProp(L"def"));
			c->m_pDefNode = fg.GetRootBody();
			c->m_pParent = &pParent;
			c->LoadChildren();
			pParent->m_aChild.push( c );
			m_Doc->SetModified( true );
		}
		GetBox()->Refresh();
	}

}

void AMainForm::DoDelete()
{
	if( m_Doc->GetSelectedCount() )
	{
		TDialogButton db = AMessageBox::Question(L"是否删除选定的控件？");
		if( db == dbYes )
		{
			SELECTED_TYPE& aSelected = m_Doc->GetSelected();
			for(SELECTED_TYPE::iterator it=aSelected.begin();it!=aSelected.end();it++)
			{
				afc_object<CControl> pc(it->first);
				m_Doc->RemoveControl( pc );
			}
			m_Doc->UnSelectAll();
			GetBox()->Refresh();
			m_pUIPropMgr->ShowProp(afc_object<CControl>());
			m_Doc->SetModified( true );
		}

	}
}

void AMainForm::OnCopyExecute(AObject* pSender,AEvent* pEvent)
{
	DoCopy();
}
void AMainForm::OnPasteExecute(AObject* pSender,AEvent* pEvent)
{
	DoPaste();
}
void AMainForm::OnDeleteExecute(AObject* pSender,AEvent* pEvent)
{
	DoDelete();
}

void AMainForm::OnCutExecute(AObject* pSender,AEvent* pEvent)
{
	DoCopy();
	DoDelete();
}

void AMainForm::LocateTo(CControl* pUI)
{
	m_Doc->UnSelectAll();
	afc_object<CControl> pu;
	pu = pUI;
	m_Doc->SetSelected(pu,true);
	GetBox()->Refresh();
}

void AMainForm::OnLockExecute(AObject* pSender,AEvent* pEvent)
{
	if( & m_Doc != NULL )
		m_Doc->SetLocked(!m_Doc->GetLocked());
}

void AMainForm::OnSaveExecute(AObject* pSender,AEvent* pEvent)
{
	if( m_Doc->GetFileName() == _T("") )
	{
		if( GetSaveDialog()->Execute() )
		{
			m_Doc->SetFileName(GetSaveDialog()->GetFileName());
		}
		else return;
	}
	m_Doc->SaveToFile( m_Doc->GetFileName() );
}

void AMainForm::OnOpenExecute(AObject* pSender,AEvent* pEvent)
{
	if( GetOpenDialog()->Execute() )
	{
		m_Doc->LoadFromFile(GetOpenDialog()->GetFileName());
		m_pUIPropMgr->ShowProp(afc_object<CControl>());
		Refresh();
	}
}

void AMainForm::OnPreviewExecute(AObject* pSender,AEvent* pEvent)
{
	if( m_Doc->GetWebForm() == ANULL )
	{
		AMessageBox::Error(_T("请先至少放一个窗口控件！"));
		return;
	}
	afc_object<CControl> pWebForm;
	pWebForm = m_Doc->GetWebForm();
	CONFIG_BODY pRoot;
	pRoot = pWebForm->m_Config.GetRootBody();

	UIXMLGen gen(&pRoot);
	afc_object<AMLBody> pn;
	pn = gen.ToNode();

	AFile f;
	if( f.Create(L"d:\\test.xml") )
	{
		AString s;
		s = pn->ToXML();
		const char* sz = s.ConvertTo();
		f.Write((BYTE *)sz,strlen(sz));
	}

	AForm* pForm = dynamic_cast<AForm*>( AControl::CreateInstance(&pn,this));//AApplication::Get()) );
	if( pForm == NULL )
	{
		AMessageBox::Error(_T("窗口生成失败！"));
		return;
	}
	EVENT_CONNECT(pForm,OnClose,OnPreviewFormClose);
	pForm->SetVisible(false);
	pForm->Show();
	m_aPreviewForm.Add( pForm );
}

void AMainForm::OnClosePreviewExecute(AObject* pSender,AEvent* pEvent)
{
	for(int i=0;i<m_aPreviewForm.GetCount();i++)
	{
		AApplication::Discard(m_aPreviewForm[i]);
	}
	m_aPreviewForm.Clear();
}

void AMainForm::OnPreviewFormClose(AObject* pSender,AEvent* pEvent)
{
	m_aPreviewForm.Delete( pSender );
}

void AMainForm::OnGenUIExecute(AObject* pSender,AEvent* pEvent)
{
	AString sFile;
	sFile = m_Doc->GetFileName();
	if( sFile.Length() < 1 )
	{
		TDialogButton db = (TDialogButton)TrySave();
		if( db != dbOK ) return;
		sFile = m_Doc->GetFileName();
	}

	afc_object<CControl> pWebForm;
	pWebForm = m_Doc->GetWebForm();
	CONFIG_BODY pRoot;
	pRoot = pWebForm->m_Config.GetRootBody();

	UIXMLGen gen(&pRoot);
	afc_object<AMLBody> pn;
	pn = gen.ToNode();

	int iPos = sFile.ReverseFind(_T("."));
	if( iPos > -1 ) sFile = sFile.Left(iPos+1);
	sFile += _T("xml");

	AFile f;
	if( f.Create(sFile) )
	{
		AString xml = pn->ToXML();
		const char* sz = xml.ConvertTo();
		f.Write((BYTE*)sz,strlen(sz));
	}
	AMessageBox::Infor(_T("保存成功"));
}
void AMainForm::OnGenUIHExecute(AObject* pSender,AEvent* pEvent)
{
	AString sFile;
	sFile = m_Doc->GetFileName();
	if( sFile.Length() < 1 )
	{
		TDialogButton db = (TDialogButton)TrySave();
		if( db != dbOK ) return;
		sFile = m_Doc->GetFileName();
	}

	afc_object<CControl> pWebForm;
	pWebForm = m_Doc->GetWebForm();
	CONFIG_BODY pRoot;
	pRoot = pWebForm->m_Config.GetRootBody();

	UIHeaderGen gen(&pRoot);

	int iPos = sFile.ReverseFind(_T("."));
	if( iPos > -1 ) sFile = sFile.Left(iPos+1);
	sFile += _T("h");

	AFile f;
	if( f.Create(sFile) )
	{
		AString xml = gen.ToString();
		const char* sz = xml.ConvertTo();
		f.Write((BYTE*)sz,strlen(sz));
	}
	AMessageBox::Infor(_T("保存成功"));
}

