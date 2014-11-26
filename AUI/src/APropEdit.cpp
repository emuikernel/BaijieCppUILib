#include "StdAfx.h"
#include "PropItemControl.h"

#define PROPITEM_HEIGHT	26
#define PROPITEM_INDENT	32
#define PROPITEM_PLUS	9

APropItem::APropItem():AObject()
{
	m_iLevel = 0;
	m_iRow = 0;
	m_bReadOnly = false;
	m_pAppData = (void *)0;
	m_bModified = false;
	m_pTag = NULL;
}

APropItem::~APropItem()
{
	//
}

AString APropItem::GetKey()
{
	return m_sKey;
}

APropInt::APropInt():APropItem()
{
	m_Val = 0;
}

APropInt::APropInt(const AString& sKey,const AString& sDisp,int val):APropItem()
{
	m_Val = val;
	m_sKey = sKey;
	m_sDisplayName = sDisp;
}
APropInt::~APropInt()
{
	//
}

int APropInt::GetValue()
{
	return m_Val;
}

APropBool::APropBool():APropItem()
{
	m_Val = false;
}

APropBool::APropBool(const AString& sKey,const AString& sDisp,bool val):APropItem()
{
	m_Val = val;
	m_sKey = sKey;
	m_sDisplayName = sDisp;
}
APropBool::~APropBool()
{
	//
}

bool APropBool::GetValue()
{
	return m_Val;
}

APropData::APropData():base_class()
{
}

APropData::APropData(const AString& sKey,const AString& sDisp):base_class()
{
	m_sKey = sKey;
	m_sDisplayName = sDisp;
}
APropData::~APropData()
{
	//
}

APropColor::APropColor():base_class()
{
	m_Val = AColor(255,0,0,0);
}

APropColor::APropColor(const AString& sKey,const AString& sDisp,AColor val):base_class()
{
	m_Val = val;
	m_sKey = sKey;
	m_sDisplayName = sDisp;
}
APropColor::~APropColor()
{
	//
}

AColor APropColor::GetValue()
{
	return m_Val;
}


APropString::APropString():APropItem()
{
	m_Val = _T("");
	m_bMultiLine = false;
}

APropString::APropString(const AString& sKey,const AString& sDisp,const AString& val,bool bMulti):APropItem()
{
	m_Val = val;
	m_sKey = sKey;
	m_sDisplayName = sDisp;
	m_bMultiLine = bMulti;
}
APropString::~APropString()
{
	//
}

AString APropString::GetValue()
{
	return m_Val;
}

APropHint::APropHint():APropItem()
{
	m_Val = _T("");
}

APropHint::APropHint(const AString& sKey,const AString& sDisp,const AString& val):APropItem()
{
	m_Val = val;
	m_sKey = sKey;
	m_sDisplayName = sDisp;
}
APropHint::~APropHint()
{
	//
}

AString APropHint::GetValue()
{
	return m_Val;
}

class APropComboChoice : public AObject
{
public:
	APropComboChoice():AObject()
	{
		//
	}
	virtual ~APropComboChoice()
	{
		//
	}
	AString		m_sKey;
	AString		m_sDisp;
};
APropCombo::APropCombo():APropItem()
{
	m_IntVal = -1;
	m_bCanEdit = false;
}
APropCombo::APropCombo(const AString& sKey,const AString& sDisp,const AString& sVal):APropItem()
{
	m_sKey = sKey;
	m_sDisplayName = sDisp;
	m_Val = sVal;
	m_IntVal = -1;
	m_bCanEdit = false;
}
APropCombo::APropCombo(const AString& sKey,const AString& sDisp,int iVal):APropItem()
{
	m_sKey = sKey;
	m_sDisplayName = sDisp;
	m_IntVal = iVal;
	m_bCanEdit = false;
}
APropCombo::~APropCombo()
{
	for(int i=0;i<m_aChoice.GetCount();i++)
	{
		APropComboChoice* p = (APropComboChoice *)m_aChoice.GetItem(i);
		delete p;
	}
	m_aChoice.Clear();
}

APropCombo* APropCombo::AddChoice(const AString& s,const AString& sDisp)
{
	APropComboChoice* p = new APropComboChoice();
	p->m_sKey = s;
	p->m_sDisp = sDisp;
	m_aChoice.Add(p);
	return this;
}

AString APropCombo::GetKey(int index)
{
	AString str;
	APropComboChoice* p = (APropComboChoice *)m_aChoice[index];
	str = p->m_sKey;
	return str;
}
AString APropCombo::GetDisp(int index)
{
	AString str;
	APropComboChoice* p = (APropComboChoice *)m_aChoice[index];
	str = p->m_sDisp;
	return str;
}
APropGroup::APropGroup():APropItem()
{
	m_bExpanded = false;
}

APropGroup::~APropGroup()
{
	for(int i=0;i<m_aChild.GetCount();i++)
	{
		APropItem * pItem = (APropItem *)m_aChild.GetItem(i);
		if( pItem ) pItem->Release();
	}
	m_aChild.Clear();
}

void APropGroup::AddProp(APropItem* pItem)
{
	if( pItem )
	{
		m_aChild.Add(pItem);
		pItem->AddRef();
	}
}
bool APropGroup::RemoveProp(APropItem* pItem)
{
	for(int i=0;i<m_aChild.GetCount();i++)
	{
		APropItem * p = (APropItem *)m_aChild.GetItem(i);
		if( p == pItem )
		{
			m_aChild.Delete(i);
			p->Release();
			return true;
		}
	}
	for(int j=0;j<m_aChild.GetCount();j++)
	{
		APropGroup * p = dynamic_cast<APropGroup*>((APropItem *)m_aChild.GetItem(j));
		if( p )
		{
			if( p->RemoveProp(pItem) ) return true;
		}
	}
	return false;
}

class AColorPropButton : public AButton
{
	CLASS_INFO(ColorPropButton,Button);
public:
	AColorPropButton(AComponent* pOwner):base_class(pOwner)
	{

	}
	virtual ~AColorPropButton()
	{

	}

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent)
	{
		ARect r = GetAbsPosition();
		if( !GetUICore()->InDrawArea(r) ) return;

		APropColor* pPropColor = (APropColor *)GetTag();
		AColor cr(255,0,0,0);
		if( pPropColor ) cr = pPropColor->GetValue();

		AAutoClip _clip(cs,r);

		AString sText;
		sText.Format(_T("RGB(%d,%d,%d,%d)"),cr.alpha,cr.red,cr.green,cr.blue);

		ASolidBrush br(cr);
		ARect rColor(r.left+2,r.top+3,r.left+2+20,r.bottom-3);
		cs->Use(&br)->FillRect(rColor);

		ARect rText(rColor.right + 2,r.top,r.right,r.bottom);
		cs->Use(&Font)->DrawText(rText,sText,StringAlignmentNear,StringAlignmentMiddle);
	}
};
//=================================================================

APropEdit::APropEdit(AComponent* pOwner):
	base_class(pOwner),
	m_pSpliter(NULL)
{
	m_pEventReceiver = NULL;
	m_funcOnChange = NULL;

	SetAutoVScrollBar(true);

}

APropEdit::~APropEdit()
{
	ResetAll();
}

void APropEdit::SetOnChange(AObject* p,FOnPropChange func)
{
	m_pEventReceiver = p;
	m_funcOnChange = func;
}
void APropEdit::AddProp(APropItem* pItem)
{
	if( pItem )
	{
		m_aItem.Add( pItem );
		pItem->AddRef();
	}
}

void APropEdit::ResetAll()
{
	//time_counter tc;
	for(int i=0;i<m_aItem.GetCount();i++)
	{
		APropItem* pItem = (APropItem *)m_aItem.GetItem(i);
		if( pItem ) pItem->Release();
	}
	m_aItem.Clear();
	//tc.Print(L"APropEdit::Clear Items...");
	//tc.Reset();
	RemoveAllChild();
	//tc.Print(L"APropEdit::RemoveAllChild...");
	m_pSpliter = NULL;
}
void APropEdit::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawPropEdit(cs,this);

	AContainer::DoPaintChildren(pEvent);
}

void APropEdit::Render()
{
	//time_counter tc;
	m_pUICore->SetDisableEvent(true);
	m_pUICore->SetLayoutEnabled(false);
	if( m_pSpliter == NULL )
	{
		m_pSpliter = new APanel(this);
		m_pSpliter->SetName(_T("spliter"));
		m_pSpliter->SetFloatType(ftFloatY);
		m_pSpliter->SetTop(0);
		m_pSpliter->SetWidth(2);
		m_pSpliter->SetLeft(m_iWidth/2);
		m_pSpliter->SetHeight(m_iHeight);
		m_pSpliter->OnPaint = AEventHandler(this,(FEventHandler)&APropEdit::_PaintSpliter);
		m_pSpliter->SetCursorType(ctSizeWE);
		m_pSpliter->OnMouseDown = AEventHandler(this,(FEventHandler)&APropEdit::OnSplitMouseDown);
		m_pSpliter->OnMouseMove = AEventHandler(this,(FEventHandler)&APropEdit::OnSplitMouseMove);
		m_pSpliter->OnMouseUp = AEventHandler(this,(FEventHandler)&APropEdit::OnSplitMouseUp);
	}
	//tc.Print(L"APropEdit::Render::new Spliter...");
	//tc.Reset();
	//根据PropItem来动态生成PropItemControl
	int iCurLevel = 0;
	for(int i=0;i<m_aItem.GetCount();i++)
	{
		APropItem* pItem = dynamic_cast<APropItem*>( (AObject*)m_aItem.GetItem(i) );
		if( pItem == NULL ) continue;
		RenderItem(this,pItem,iCurLevel,true);
	}
	//tc.Print(L"APropEdit::Render::Render items...");
	//tc.Reset();
	m_pUICore->SetDisableEvent(false);
	m_pUICore->SetLayoutEnabled(true);
	//UpdatePos();
	DoLayout();//这一步Layout是决定滚动条是否显示
	//tc.Print(L"APropEdit::Render::DoLayout");
	//tc.Reset();
	UpdatePos();
	//tc.Print(L"APropEdit::Render::UpdatePos");
	//DoLayout();
}
void APropEdit::RenderItem(AObject* pParent,APropItem* pItem,int iCurLevel,bool bVisible)
{
	pItem->m_iLevel = iCurLevel;

	AControl* pLast = NULL;

	pLast = dynamic_cast<AControl*>( GetChildControls()->GetItem( GetChildControls()->GetCount()-1) );
	if( pLast == m_pSpliter )
		pLast = NULL;

	int deltaR =0;
	if( m_bVertScrollBarShow && m_pVertScrollBar != NULL ) 
		deltaR = AApplication::Get()->GetCurSkin()->m_ScrollBarWidth;

	APanel* pPnl = new APanel(dynamic_cast<AComponent*>(pParent));
	pPnl->SetHeight(PROPITEM_HEIGHT);
	pPnl->SetWidth(GetWidth()-deltaR);
	pPnl->SetLeft(0);
	pPnl->SetPadding(2);
	if( pLast ) pPnl->SetTop(pLast->GetTop()+pLast->GetHeight());
	else pPnl->SetTop(0);

	pPnl->OnPaint = AEventHandler(this,(FEventHandler)&APropEdit::_PaintItem);
	pItem->m_pTag = pPnl;
	pPnl->SetTag(pItem);
	pPnl->OnMouseDown = AEventHandler(this,(FEventHandler)&APropEdit::OnIndentMouseDown);
	pPnl->OnDblClick = OnItemDblClick;

// 	APanel* pnlLeft = new APanel(pPnl);
// 	pnlLeft->SetAlign(alLeft);
// 	pnlLeft->SetHeight(pPnl->GetHeight());
// 	pnlLeft->SetWidth(PROPITEM_INDENT*pItem->m_iLevel+4);
// 	//pnlLeft->SetColor(AColor(255,129,129,129));
// 	pnlLeft->SetBorderWidth(0);
// 	pnlLeft->SetTag(pItem);
// 	pnlLeft->OnPaint = AEventHandler(this,(FEventHandler)&APropEdit::_PaintIndent);
// 	pnlLeft->OnMouseDown = AEventHandler(this,(FEventHandler)&APropEdit::OnIndentMouseDown);
// 
// 	ALabel* pLabel = new ALabel(pPnl);
// 	pLabel->SetAlign(alLeft);
// 	pLabel->SetWidth(0);
// 	pLabel->SetTag(pItem);
// 	pLabel->SetHeight(pPnl->GetHeight());
// 	pLabel->SetText(pItem->m_sDisplayName);

	switch( pItem->GetType() )
	{
	case PROP_ITEM_INT:
		{
			APropInt* pPropInt = dynamic_cast<APropInt*>(pItem);
			if( pPropInt )
			{
				AEdit* pEdit = new AEdit(pPnl);
				pEdit->SetTag(pItem);
				pEdit->SetAlign( alRight );
				pEdit->SetHeight(pPnl->GetHeight());
				pEdit->SetText( AStringVal(pPropInt->m_Val) );
				pEdit->OnChange = AEventHandler(this,(FEventHandler)&APropEdit::OnEditChange);
			}
		}
		break;
	case PROP_ITEM_STRING:
		{
			APropString* pPropString = dynamic_cast<APropString*>(pItem);
			if( pPropString )
			{
				AEdit* pEdit = new AEdit(pPnl);
				pEdit->SetTag(pItem);
				pEdit->SetAlign( alRight );
				pEdit->SetHeight(pPnl->GetHeight());
				pEdit->SetText(pPropString->m_Val);
				pEdit->OnChange = AEventHandler(this,(FEventHandler)&APropEdit::OnEditChange);
			}
		}
		break;
	case PROP_ITEM_HINT:
		{
			APropHint* pPropHint = dynamic_cast<APropHint*>(pItem);
			if( pPropHint )
			{
				ALabel* pEdit = new ALabel(pPnl);
				pEdit->SetTag(pItem);
				pEdit->SetAlign( alRight );
				pEdit->SetHeight(pPnl->GetHeight());
				pEdit->SetText(pPropHint->m_Val);
			}
		}
		break;
	case PROP_ITEM_COMBO:
		{
			APropCombo* pPropCombo = dynamic_cast<APropCombo*>(pItem);
			if( pPropCombo )
			{
				AComboBox* pBox = new AComboBox(pPnl);
				pBox->SetTag(pItem);
				pBox->SetHeight(pPnl->GetHeight());
				pBox->SetAlign(alRight);
				AString sText;
				for(int i=0;i<pPropCombo->m_aChoice.GetCount();i++)
				{
					APropComboChoice* pChoice = (APropComboChoice*)pPropCombo->m_aChoice[i];
					pBox->AddString( pChoice->m_sDisp );
					if( pPropCombo->m_Val == pChoice->m_sKey || pPropCombo->m_IntVal == i ) 
						sText = pChoice->m_sDisp;
				}

				pBox->SetText(sText);

				EVENT_CONNECT(pBox,OnSelChange,OnComboSelChange);
				EVENT_CONNECT(pBox,OnTextChange,OnComboTextChange);
			}
		}
		break;
	case PROP_ITEM_BOOL:
		{
			APropBool* pPropBool = dynamic_cast<APropBool*>(pItem);
			if( pPropBool )
			{
				ACheckBox* pBox = new ACheckBox(pPnl);
				pBox->SetTag(pItem);
				pBox->SetHeight(pPnl->GetHeight());
				pBox->SetAlign(alRight);
				pBox->SetChecked( pPropBool->m_Val );
				pBox->SetText( pPropBool->m_sDisplayName );
				pBox->OnClick = AEventHandler(this,(FEventHandler)&APropEdit::OnCheckboxClick);
			}
		}
		break;
	case PROP_ITEM_COLOR:
		{
			APropColor* pPropColor = dynamic_cast<APropColor*>(pItem);
			if( pPropColor )
			{
				AColorPropButton* pButton = new AColorPropButton(pPnl);
				pButton->SetTag( (void *)pPropColor );
				pButton->SetHeight(pPnl->GetHeight());
				pButton->SetAlign( alRight );
				EVENT_CONNECT(pButton,OnClick,OnColorButtonClick);
			}
		}
		break;
	case PROP_ITEM_DATA:
		{
			APropData* pPropData = dynamic_cast<APropData*>(pItem);
			if( pPropData )
			{
				AButton* pButton = new AButton(pPnl);
				pButton->SetTag( (void *)pPropData );
				pButton->SetWidth(40);
				pButton->SetAlign( alRight );
				EVENT_CONNECT(pButton,OnClick,OnDataClick);
			}
		}
		break;
	case PROP_ITEM_GROUP:
		{
			APropGroup* pGroup = dynamic_cast<APropGroup*>(pItem);
			if( pGroup != NULL  )
			{
				bool b =bVisible;
				if( bVisible == false || pGroup->m_bExpanded == false ) b = false;
				for(int i=0;i<pGroup->m_aChild.GetCount();i++)
				{
					APropItem* pItem = dynamic_cast<APropItem*>((APropItem *)pGroup->m_aChild.GetItem(i));
					RenderItem(this,pItem,iCurLevel+1,b);
				}
			}
		}
		break;
	default:
		break;
	}
	

	//pPnl->SetVisible(bVisible);
	//pPnl->DoLayout();
}

void APropEdit::SetWidth(int v)
{
	APanel::SetWidth(v);
	if( m_pSpliter )
	{
		m_pSpliter->SetLeft(v/2);
		m_pSpliter->SetTop(0);
		m_pSpliter->SetWidth(2);
	}
	UpdatePos();
}

void APropEdit::SetHeight(int v)
{
	APanel::SetHeight(v);
	if( m_pSpliter ) m_pSpliter->SetHeight(GetHeight());
	UpdatePos();
}

void APropEdit::_PaintSpliter(AControl* pControl,HDC hDC)
{
	ACanvas* cs = GetUICore()->GetCanvas();
	ARect r = pControl->GetAbsPosition();

	//cs->Brush->Color = AColor(255,255,255,255);
	APen p;
	p.SetColor(AColor(255,255,255,255));
	cs->Use(&p)->DrawLine(r.left+1,r.top,r.left+1,r.bottom);
	p.SetColor(AColor(255,122,122,122));
	cs->Use(&p)->DrawLine(r.left,r.top,r.left,r.bottom);
}

void APropEdit::_PaintIndent(AControl* pControl,HDC hDC)
{
	ACanvas* cs = GetUICore()->GetCanvas();
	APanel* pnl = dynamic_cast<APanel*>(pControl);
	if( pnl == NULL ) return;

	ARect r = pnl->GetAbsPosition();

	APropItem* pItem = (APropItem *)pnl->GetTag();
	APropGroup* pGroup = dynamic_cast<APropGroup*>(pItem);
	if( pGroup )
	{
		int iWidth = PROPITEM_INDENT*pItem->m_iLevel;
		if( iWidth < PROPITEM_PLUS ) iWidth = PROPITEM_PLUS+2;
		int iHeight = ( r.GetHeight() - PROPITEM_PLUS ) / 2;
		ARect rPlus(r.left+iWidth - PROPITEM_PLUS-1,r.top+iHeight,r.left+iWidth-1,r.top+iHeight+PROPITEM_PLUS);
		APen pen;
		pen.SetWidth(0.3);
		pen.SetColor(AColor(255,0,0,0));
		ASolidBrush br;
		br.SetColor(WHITE_COLOR);
		cs->Use(&br)->FillRect(rPlus);
		cs->Use(&pen)->DrawRect(rPlus);
		cs->Use(&pen)->DrawLine(rPlus.left,rPlus.top+PROPITEM_PLUS/2,rPlus.right,rPlus.top+PROPITEM_PLUS/2);
		if( !pGroup->m_bExpanded )
		{
			cs->Use(&pen)->DrawLine(rPlus.left+PROPITEM_PLUS/2,rPlus.top,rPlus.left+PROPITEM_PLUS/2,rPlus.bottom);
		}
	}
}

void APropEdit::_PaintItem(AControl* pControl,HDC hDC)
{
	ACanvas* cs = GetUICore()->GetCanvas();
	ARect r = pControl->GetAbsPosition();
	if( !m_pUICore->InDrawArea(r) ) return;

	APropItem* pItem = (APropItem *)pControl->GetTag();

	if( dynamic_cast<APropHint*>(pItem) )
	{
		ASolidBrush br;
		br.SetColor(AColor(255,232,226,115));

		cs->Use(&br)->FillRect(r);
	}

	//显示 + -
	APropGroup* pGroup = dynamic_cast<APropGroup*>(pItem);
	if( pGroup )
	{
		int iWidth = pControl->GetPadding()+PROPITEM_INDENT*pItem->m_iLevel;
		if( iWidth < PROPITEM_PLUS ) iWidth = PROPITEM_PLUS+2;
		int iHeight = ( r.GetHeight() - PROPITEM_PLUS ) / 2;
		ARect rPlus(r.left+iWidth - PROPITEM_PLUS-1,r.top+iHeight,r.left+iWidth-1,r.top+iHeight+PROPITEM_PLUS);
		APen pen;
		pen.SetWidth(0.3);
		pen.SetColor(AColor(255,0,0,0));
		ASolidBrush br;
		br.SetColor(WHITE_COLOR);
		cs->Use(&br)->FillRect(rPlus);
		cs->Use(&pen)->DrawRect(rPlus);
		cs->Use(&pen)->DrawLine(rPlus.left,rPlus.top+PROPITEM_PLUS/2,rPlus.right,rPlus.top+PROPITEM_PLUS/2);
		if( !pGroup->m_bExpanded )
		{
			cs->Use(&pen)->DrawLine(rPlus.left+PROPITEM_PLUS/2,rPlus.top,rPlus.left+PROPITEM_PLUS/2,rPlus.bottom);
		}
	}

	//显示DisplayName
	int iWidth = PROPITEM_INDENT*pItem->m_iLevel;
	if( iWidth < PROPITEM_PLUS ) iWidth = PROPITEM_PLUS+2;
	if( iWidth > m_pSpliter->GetLeft()+2 ) iWidth = m_pSpliter->GetLeft()+2;

	ARect rDisp;
	rDisp.left = iWidth;
	rDisp.top=1;
	rDisp.right = m_pSpliter->GetLeft()-1;
	rDisp.bottom = r.GetHeight()-2;
	rDisp.Offset(r.left,r.top);
	AFont* pFont = NULL;
	if( GetUseSkin() ) pFont = &(AApplication::Get()->GetCurSkin()->m_DefaultFont);
	else pFont = &Font;
	cs->Use(pFont)->DrawText(rDisp,pItem->m_sDisplayName);

	APen pen;
	pen.SetWidth(0.1);
	pen.SetColor(AColor(255,0,0,0));

	cs->Use(&pen)->DrawLine(r.left,r.bottom,r.right,r.bottom);
}

void APropEdit::OnSplitMouseMove(AObject* pSender,AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL) return;

	if( evt->m_KeyStates.Has(ksLeftButton) && m_iOldSplitPos != -1 )
	{
		ARect r = GetAbsPosition();
		int iLeft = evt->m_X - r.left;
		if( iLeft < 5 ) iLeft = 5;
		if( iLeft > r.GetWidth()-5 ) iLeft = r.GetWidth()-5;
		m_pSpliter->SetLeft(iLeft);
		DoLayout();
		UpdatePos();
		Refresh();
	}
}
void APropEdit::OnSplitMouseDown(AObject* pSender,AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL) return;

	if( evt->m_KeyStates.Has(ksLeftButton) )
	{
		m_iOldSplitPos = evt->m_X;
	}
}
void APropEdit::OnSplitMouseUp(AObject* pSender,AEvent* pEvent)
{
	m_iOldSplitPos = -1;
	UpdatePos();
	DoLayout();
}

void APropEdit::UpdatePos()
{
	for(int i=0;i<m_aItem.GetCount();i++)
	{
		APropItem* pItem = dynamic_cast<APropItem*>( (AObject*)m_aItem.GetItem(i) );
		if( pItem == NULL ) continue;
		UpdateItemPos(pItem);
	}
}

void APropEdit::UpdateItemPos(APropItem* pItem)
{
	APanel* pPnl = (APanel *)pItem->m_pTag;
	int deltaR =0;
	if( m_bVertScrollBarShow && m_pVertScrollBar != NULL ) 
		deltaR = AApplication::Get()->GetCurSkin()->m_ScrollBarWidth;
	pPnl->SetWidth(GetWidth()-deltaR);

// 	AControl* pIndent = dynamic_cast<AControl*>(pPnl->GetChildControls()->GetItem(0));
// 
// 	int iWidth = PROPITEM_INDENT*pItem->m_iLevel;
// 	if( iWidth < PROPITEM_PLUS ) iWidth = PROPITEM_PLUS+2;
// 	if( iWidth > m_pSpliter->GetLeft()+2 ) iWidth = m_pSpliter->GetLeft()+2;
// 	pIndent->SetWidth(iWidth);
// 
// 	iWidth = m_pSpliter->GetLeft()+1- iWidth;
// 	if( iWidth < 0 ) iWidth = 0;
// 	AControl* pLabel = (AControl *)pPnl->GetChildControls()->GetItem(1);
// 	pLabel->SetWidth(iWidth);

	AControl* px = NULL;
	if( pPnl->GetChildControls()->GetCount() > 0 && pItem->GetType() != PROP_ITEM_DATA )
	{
		px = dynamic_cast<AControl*>(pPnl->GetChildControls()->GetItem(0));
		px->SetWidth(pPnl->GetWidth()-m_pSpliter->GetLeft()-m_pSpliter->GetWidth()-pPnl->GetPadding()-1);
	}
	
	APropGroup* pGroup = dynamic_cast<APropGroup*>(pItem);
	if( pGroup != NULL )
	{
		for(int i=0;i<pGroup->m_aChild.GetCount();i++)
		{
			APropItem* pChildItem = dynamic_cast<APropItem*>((APropItem *)pGroup->m_aChild.GetItem(i));
			UpdateItemPos(pChildItem);
		}
	}
	pPnl->DoLayout();
}

void APropEdit::OnIndentMouseDown(AObject* pSender,AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL) return;
	APanel* pnlIndent = dynamic_cast<APanel*>(pSender);
	if( pnlIndent == NULL ) return;

	APropItem* pItem = (APropItem *)pnlIndent->GetTag();
	if( pItem == NULL ) return;

	APropGroup* pGroup = dynamic_cast<APropGroup*>(pItem);
	if( pGroup == NULL ) return;

	ARect r = pnlIndent->GetAbsPosition();
	int iWidth = PROPITEM_INDENT*pItem->m_iLevel;
	if( iWidth < PROPITEM_PLUS ) iWidth = PROPITEM_PLUS+2;
	int iHeight = ( r.GetHeight() - PROPITEM_PLUS ) / 2;
	//ARect rPlus(r.left+iWidth - PROPITEM_PLUS-1,r.top+4,r.left+iWidth-1,r.top+4+PROPITEM_PLUS);
	ARect rPlus(r.left+iWidth - PROPITEM_PLUS-1,r.top+iHeight,r.left+iWidth-1,r.top+iHeight+PROPITEM_PLUS);

	if( rPlus.PtInRect(APoint(evt->m_X,evt->m_Y)) )
	{
		pGroup->m_bExpanded = !pGroup->m_bExpanded;
		AControl* pParent = (AControl *)pnlIndent->GetParent();
		int iTop = pParent->GetTop()+pParent->GetHeight();
		for(int i=0;i<pGroup->m_aChild.GetCount();i++)
		{
			APropItem* pChildItem = dynamic_cast<APropItem*>((APropItem *)pGroup->m_aChild.GetItem(i));
			UpdateVisible(pChildItem,pGroup->m_bExpanded ? true : false);
		}
		UpdateExpand();
		DoLayout();
		//Refresh();
	}
}

void APropEdit::UpdateVisible(APropItem* pItem,bool bVisible)
{
	AControl* pControl = (AControl*)pItem->m_pTag;
// 	for(int i=0;i<GetChildControls()->GetCount();i++)
// 	{
// 		AControl* p = dynamic_cast<AControl*>(GetChildControls()->GetItem(i));
// 		if( pItem == (APropItem *)p->GetTag() )
// 		{
// 			pControl = p;
// 			break;
// 		}
// 	}
	if( pControl == NULL ) return;
	
	//pControl->SetTop(iTop);
	//if( pControl->GetVisible() ) iTop += PROPITEM_HEIGHT;

	APropGroup* pGroup = dynamic_cast<APropGroup *>(pItem);
	if( pGroup )
	{
		for(int i=0;i<pGroup->m_aChild.GetCount();i++)
		{
			APropItem* pChildItem = dynamic_cast<APropItem*>((APropItem *)pGroup->m_aChild.GetItem(i));
			UpdateVisible(pChildItem,bVisible == false ? false : pGroup->m_bExpanded);
		}
	}
	pControl->SetVisible(bVisible );
}

//因为展开/收拢的变化而调整位置
void APropEdit::UpdateExpand()
{
	int iTop = 0;
	for(int i=0;i<GetChildControls()->GetCount();i++)
	{
		AControl* p = dynamic_cast<AControl*>( GetChildControls()->GetItem(i) );
		if( p == NULL || p->GetName() == _T("spliter") ) continue;
		if( p->GetVisible() == true )
		{
			p->SetTop(iTop);
			iTop += p->GetHeight();
		}
	}
}
void APropEdit::OnEditChange(AObject* pSender,AEvent* pEvent)
{
	APropItem* pItem = (APropItem *)((AControl *)pSender)->GetTag();
	AEdit* pEdit = dynamic_cast<AEdit*>(pSender);
	if( pEdit == NULL ) return;

	if( m_pEventReceiver && m_funcOnChange )
	{

		APropString* pStringProp = dynamic_cast<APropString*>(pItem);
		if( pStringProp ) pStringProp->m_Val = pEdit->GetText();

		APropInt* pIntProp = dynamic_cast<APropInt*>(pItem);
		if( pIntProp ) pIntProp->m_Val = AIntVal(pEdit->GetText());

		((*m_pEventReceiver).*m_funcOnChange)(pItem);
	}
}

void APropEdit::OnCheckboxClick(AObject* pSender,AEvent* pEvent)
{
	ACheckBox* pBox = dynamic_cast<ACheckBox *>(pSender);
	APropItem* pItem = (APropItem *)(AControl*)pBox->GetTag();
	if( pItem == NULL ) return;

	if( m_pEventReceiver && m_funcOnChange )
	{

		APropBool* pBoolProp = dynamic_cast<APropBool*>(pItem);
		if( pBoolProp ) pBoolProp->m_Val = pBox->GetChecked();

		((*m_pEventReceiver).*m_funcOnChange)(pItem);
	}
}

void APropEdit::OnColorButtonClick(AObject* pSender,AEvent* pEvent)
{
	AColorPropButton* pButton = dynamic_cast<AColorPropButton *>(pSender);
	if( pButton == NULL ) return;
	APropColor* pColorProp = (APropColor* )pButton->GetTag();
	AColorDialog* p = new AColorDialog(this);
	p->SetColor( pColorProp->GetValue() );
	if( p->Execute() )
	{
		pColorProp->m_Val = p->GetColor();
		if( m_pEventReceiver && m_funcOnChange )
		{
			((*m_pEventReceiver).*m_funcOnChange)(pColorProp);
		}
	}
	p->Destroy();
}

void APropEdit::OnDataClick(AObject* pSender,AEvent* pEvent)
{
	AButton* pButton = dynamic_cast<AButton *>(pSender);
	if( pButton == NULL ) return;
	APropData* pProp = (APropData* )pButton->GetTag();

	if( m_pEventReceiver && m_funcOnChange )
	{
		((*m_pEventReceiver).*m_funcOnChange)(pProp);
	}
}

void APropEdit::OnComboSelChange(AObject* pSender,AEvent* pEvent)
{
	AComboBox* pBox = dynamic_cast<AComboBox *>(pSender);
	if( pBox == NULL ) return;
	APropCombo* pProp = (APropCombo* )pBox->GetTag();
	pProp->m_IntVal = pBox->GetItemIndex();
	if( m_pEventReceiver && m_funcOnChange )
	{
		((*m_pEventReceiver).*m_funcOnChange)(pProp);
	}
}
void APropEdit::OnComboTextChange(AObject* pSender,AEvent* pEvent)
{
	AComboBox* pBox = dynamic_cast<AComboBox *>(pSender);
	if( pBox == NULL ) return;
	APropCombo* pProp = (APropCombo* )pBox->GetTag();
	if( pProp->m_bCanEdit == false ) return;
	pProp->m_Val = pBox->GetText();
	if( m_pEventReceiver && m_funcOnChange )
	{
		((*m_pEventReceiver).*m_funcOnChange)(pProp);
	}
}