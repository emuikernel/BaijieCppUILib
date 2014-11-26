#include "stdafx.h"
#include "AControl.h"
#include "AFileDialogForm.h"
#include "ADirectoryForm.h"
#include "AFontForm.h"
#include "AColorDialogForm.h"
#include "AMessageForm.h"

AControl::AControl(AComponent* pParent):
	base_class(pParent),
	m_pParent(NULL),
	m_bDestroyed(false),
	m_bCanFocus(false)
{
	if( pParent )
	{
		AContainer* pc=dynamic_cast<AContainer*>(pParent);
		if( pc )
		{
			m_pParent = pc;
			m_pUICore = pc->GetUICore();
			pc->GetChildControls()->Add(this);
		}
	}
	else
	{
		m_pOwner = AApplication::Get();
		AApplication::Get()->GetChildren()->Add(this);
	}

	m_bEnabled = true;
	m_bVisible = true;
	m_bUseSkin = true;
	m_FloatType = ftFloatNone;
	m_Align = alNone;
	m_CursorType = ctArrow;
	m_Color = LIGHT_COLOR;
	BorderColor = BORDER_COLOR;
	m_bIsDesign = false;

	m_iLeft = 0;
	m_iTop = 0;
	m_iWidth = 0;
	m_iHeight = 0;

	m_iZOrder = 0;
	m_pTag = (void *)0;
	m_iBorderWidth = 1;
	m_iPadding = 0;
}

AControl::~AControl()
{

}

bool AControl::IsDestroyed()
{
	return m_bDestroyed;
}

//添加关注。表示：p关注自己，p要等我销毁了才能释放它
void AControl::WatchDelete(AControl* p)
{
	//普通控件不需要被关注，释放就释放了 m_aWatchDelete.Add(p);
}

void AControl::Destroy()
{
	if( m_bDestroyed ) return;
	m_bDestroyed = true;
	//DebugStr(AString().Format(_T("销毁一个控件 type=%s this=%p pos=(%d,%d,%d,%d)"),GetClass(),this,m_iLeft,m_iTop,m_iWidth,m_iHeight));
	if( m_pUICore->GetFocusControl() == this )
		m_pUICore->SetFocusControl(NULL);
	if( m_pUICore->GetMouseDownControl() == this )
		m_pUICore->SetMouseDownControl(NULL);
	if( m_pUICore->GetMouseOverControl() == this )
		m_pUICore->SetMouseOverControl(NULL);
	//return;//for debug
	//m_pUICore->AddDeleting(this);

	AEvent evt;
	OnDestroy.Call(this,&evt);
	AContainer* pc = dynamic_cast<AContainer*>(m_pParent);
	if( pc ) 
		pc->GetChildControls()->Delete(this);
	base_class::Free();
}

void AControl::SetName(const AString& sName)
{
	m_sName = sName;
}
AString AControl::GetName()
{
	return m_sName;
}

void AControl::SetFloatType(TFloatType ft)
{
	m_FloatType = ft;
}
TFloatType AControl::GetFloatType()
{
	return m_FloatType;
}
void AControl::SetCursorType(TCursorType ct)
{
	if( ct != m_CursorType )
	{
		m_CursorType = ct;
	}
}

TCursorType AControl::GetCursorType()
{
	return m_CursorType;
}

void AControl::SetUseSkin(bool v)
{
	m_bUseSkin = v;
	AControl* p = dynamic_cast<AControl*>(GetParent());
	if( p ) p->DoLayout();
	else this->DoLayout();
	Refresh();
}

bool AControl::GetUseSkin()
{
	return m_bUseSkin;
}
bool AControl::IsChildOf(AControl* pParent)
{
	AControl* pp = dynamic_cast<AControl*>(GetParent());
	while( pp )
	{
		if( pp == pParent ) return true;
		pp = dynamic_cast<AControl*>( pp->GetParent() );
	}
	return false;
}

ARect AControl::GetControlPosition()
{
	return ARect(m_iLeft,m_iTop,m_iLeft+m_iWidth,m_iTop+m_iHeight);
}
ARect AControl::GetControlScrollPosition()
{
	ARect r;
	r = GetControlPosition();
	if( GetAlign() == alNone )
	{
		AContainer* pc = dynamic_cast<AContainer*>(GetParent());
		if( pc && pc->GetHScrollBar() != this && pc->GetVScrollBar() != this )
		{
			ASize sz = pc->GetScrollSize();
			if( GetFloatType() == ftFloatY || GetFloatType() == ftFloat) sz.cy = 0;
			if( GetFloatType() == ftFloatX || GetFloatType() == ftFloat ) sz.cx = 0;
			r.Offset(-sz.cx,-sz.cy);
		}
	}

	return r;
}
ARect AControl::GetAbsPosition()
{
	ARect r;
	r = m_rAbsPos;
// 	r = GetControlScrollPosition();
// 
// 	AApplication* pApp = dynamic_cast<AApplication *>(GetParent());
// 	if( pApp ) return r;
// 	AContainer* pc = dynamic_cast<AContainer*>(GetParent());
// 	if( pc )
// 	{
// 		ARect rp;
// 		rp = pc->GetAbsPosition();
// 		int bp = pc->GetBorderWidth() + pc->GetPadding();
// 		r.Offset(rp.left+bp,rp.top+bp);
// 	}
	return r;
}

ARect AControl::GetScreenPosition()
{
	ARect r;
	r = GetAbsPosition();
	ARect rx;
	::GetWindowRect(m_pUICore->GetHandle(),&rx);
	r.Offset(rx.left,rx.top);
	return r;
}

void AControl::SetParent(AControl* pParent)
{
	if( pParent != m_pParent )
	{
		if( pParent )
		{
			AContainer* pc = dynamic_cast<AContainer*>(pParent);
			if(pc) 
				pc->GetChildControls()->Add(this);
		}
		if( m_pParent )
		{
			AContainer* pc = dynamic_cast<AContainer*>(m_pParent);
			if( pc )
				pc->GetChildControls()->Delete(this);
		}
		m_pParent = pParent;
		if( m_pParent )
			m_pParent->DoLayout();
	}
}

AControl* AControl::GetParent()
{
	return m_pParent;
}

void AControl::SetEnabled(bool b)
{
	m_bEnabled = b;
}
bool AControl::GetEnabled()
{
	return m_bEnabled;
}

void AControl::SetVisible(bool b)
{
	m_bVisible = b;
}

void AControl::SetLeft(int v)
{
	if( m_iLeft != v )
	{
		m_iLeft = v;
		if( Anchors.m_bLeft ) Anchors.m_iLeftDelta = v;
		Refresh();
	}
}

int AControl::GetLeft()
{
	return m_iLeft;
}

void AControl::SetTop(int v)
{
	if( m_iTop != v )
	{
		m_iTop = v;
		if( Anchors.m_bTop ) Anchors.m_iTopDelta = v;
		Refresh();
	}
}
int AControl::GetTop()
{
	return m_iTop;
}

void AControl::SetWidth(int v)
{
	if( v != m_iWidth )
	{
		m_iWidth = v;
		if( Anchors.m_bRight )
		{
			AControl* pParent = dynamic_cast<AControl*>( GetParent() );
			if( pParent )
			{
				Anchors.m_iRightDelta = pParent->GetWidth() - m_iLeft - v;
			}
		}
		DoLayout();
	}
}

int AControl::GetWidth()
{
	return m_iWidth;
}
void AControl::SetHeight(int v)
{
	if( v != m_iHeight )
	{
		m_iHeight = v;
		if( Anchors.m_bBottom )
		{
			AControl* pParent = dynamic_cast<AControl*>( GetParent() );
			if( pParent )
			{
				Anchors.m_iBottomDelta = pParent->GetHeight() - m_iTop - v;
			}
		}
		DoLayout();
	}
}
int AControl::GetHeight()
{
	return m_iHeight;
}

void AControl::SetZOrder(int v)
{
	m_iZOrder = v;
}
int AControl::GetZOrder()
{
	return m_iZOrder;
}

void AControl::SetTag(void* v)
{
	m_pTag = v;
}

void* AControl::GetTag()
{
	return m_pTag;
}
bool AControl::GetVisible()
{
	return m_bVisible;
}

void AControl::SetBorderWidth(int v)
{
	if( v != m_iBorderWidth )
	{
		m_iBorderWidth = v;
		DoLayout();
	}
}
int AControl::GetBorderWidth()
{
	return m_iBorderWidth;
}

void AControl::SetPadding(int v)
{
	m_iPadding = v;
}

int AControl::GetPadding()
{
	return m_iPadding;
}

void AControl::SetAlign(eAlign v)
{
	if( v != m_Align )
		m_Align = v;
}
eAlign AControl::GetAlign()
{
	return m_Align;
}
void AControl::SetText(const AString s)
{
	m_sText = s;
	Refresh();
}
AString AControl::GetText()
{
	return m_sText;
}

void AControl::SetHint(const AString s)
{
	m_sHint = s;
}
AString AControl::GetHint()
{
	return m_sHint;
}

void AControl::SetDesignMode(bool v)
{
	m_bIsDesign = v;
}
bool AControl::GetDesignMode()
{
	return m_bIsDesign;
}
void AControl::SetColor(AColor cr)
{
	m_Color = cr;
}
AColor AControl::GetColor()
{
	return m_Color;
}
void AControl::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	OnPaint.Call(this,pEvent);
}

AUICore* AControl::GetUICore()
{
	return m_pUICore;
}


AControl* AControl::_HitTest(int X,int Y)
{
	if( GetAbsPosition().PtInRect(APoint(X,Y))) return this;
	else return NULL;
}

void AControl::DoMouseDown(AEvent* pEvent)
{
	OnMouseDown.Call(this,pEvent);
}

void AControl::DoMouseUp(AEvent* pEvent)
{
	OnMouseUp.Call(this,pEvent);
}

void AControl::DoMouseMove(AEvent* pEvent)
{
	//if( Cursor.get() != m_pUICore->Cursor.get() )
	{
		//m_pUICore->Cursor = Cursor.get();
		HCURSOR cr = AApplication::Get()->GetCursor(GetCursorType());
		if( cr != ::GetCursor()) 
		{
			//OutputDebugString(_T("AControl::ChangeCursor"));
			::SetCursor(cr);
			//GetUICore()->Refresh(GetAbsPosition());
		}
	}
	

	OnMouseMove.Call(this,pEvent);
}

void AControl::DoMouseWheel(AEvent* pEvent)
{
	OnMouseWheel.Call(this,pEvent);
}
void AControl::DoClick(AEvent* pEvent)
{
	OnClick.Call(this,pEvent);
}

void AControl::DoEnter(AEvent* pEvent)
{
	OnEnter.Call(this,pEvent);
}

void AControl::DoLeave(AEvent* pEvent)
{
	GetUICore()->Refresh(GetAbsPosition());
	OnLeave.Call(this,pEvent);
}

void AControl::DoFocus(AEvent* pEvent)
{
	OnFocus.Call(this,pEvent);
}

void AControl::DoLostFocus(AEvent* pEvent)
{
	OnKillFocus.Call(this,pEvent);
}

void AControl::DoDblClick(AEvent* pEvent)
{
	OnDblClick.Call(this,pEvent);
}

void AControl::DoKeyboard(AEvent* pEvent)
{
	OnKeyboard.Call(this,pEvent);
}

void AControl::DoLayout()
{
	if( !m_pUICore->GetLayoutEnabled() ) return;
	AEvent evt;
	OnLayout.Call(this,&evt);
	//do nothing，由子类去重载
	m_pUICore->Refresh(GetAbsPosition());
}

AControl* AControl::CreateInstance(const AString& sClassName,AComponent* pParent)
{
	if( sClassName == _T("ACheckBox") ) return new ACheckBox(pParent);
	if( sClassName == _T("AButton")) return new AButton(pParent);
	if( sClassName == _T("AColorDialog")) return new AColorDialog(pParent);
	if( sClassName == _T("AComboBox")) return new AComboBox(pParent);
	if( sClassName == _T("AContainer")) return new AContainer(pParent);
	if( sClassName == _T("ATableContainer")) return new ATableContainer(pParent);
	if( sClassName == _T("AEdit")) return new AEdit(pParent);
	if( sClassName == _T("AMemo")) return new AMemo(pParent);
	if( sClassName == _T("ADateControl")) return new ADateControl(pParent);
	if( sClassName == _T("ASpinControl")) return new ASpinControl(pParent);
	if( sClassName == _T("AFileOpenDialog")) return new AFileOpenDialog(pParent);
	if( sClassName == _T("AFileSaveDialog")) return new AFileSaveDialog(pParent);
	if( sClassName == _T("ADirectoryDialog")) return new ADirectoryDialog(pParent);
	if( sClassName == _T("AFontDialog")) return new AFontDialog(pParent);
	if( sClassName == _T("AColorDialog")) return new AColorDialog(pParent);


	if( sClassName == _T("AFileDialogForm") ) return new AFileDialogForm(pParent);
	if( sClassName == _T("ADirectoryForm") ) return new ADirectoryForm(pParent);
	if( sClassName == _T("AFontForm") ) return new AFontForm(pParent);
	if( sClassName == _T("AColorDialogForm") ) return new AColorDialogForm(pParent);
	if( sClassName == _T("AMessageForm") ) return new AMessageForm(pParent);
	
	if( sClassName == _T("AForm")) return new AForm(pParent);
	if( sClassName == _T("AImageControl")) return new AImageControl(pParent);
	if( sClassName == _T("AImageList")) return new AImageList(pParent);
	if( sClassName == _T("ALabel")) return new ALabel(pParent);
	if( sClassName == _T("AListBox")) return new AListBox(pParent);
	if( sClassName == _T("AListView")) return new AListView(pParent);
	if( sClassName == _T("AMenu")) return new AMenu(pParent);
	if( sClassName == _T("AMenuBar")) return new AMenuBar(pParent);
	if( sClassName == _T("APageControl")) return new APageControl(pParent);
	if( sClassName == _T("APageSheet")) return new APageSheet(pParent);
	if( sClassName == _T("APanel")) return new APanel(pParent);
	if( sClassName == _T("APropEdit")) return new APropEdit(pParent);
	if( sClassName == _T("ARadioButton")) return new ARadioButton(pParent);
	if( sClassName == _T("AVertScrollBar")) return new AVertScrollBar(pParent);
	if( sClassName == _T("AHorzScrollBar")) return new AHorzScrollBar(pParent);
	if( sClassName == _T("ASidePanel")) return new ASidePanel(pParent);
	if( sClassName == _T("ASpliter")) return new ASpliter(pParent);
	if( sClassName == _T("ATreeView")) return new ATreeView(pParent);
	if( sClassName == _T("AVertSlide")) return new AVertSlide(pParent);
	if( sClassName == _T("AHorzSlide")) return new AHorzSlide(pParent);
	
	if( sClassName == _T("ATitleBar") ) return new ATitleBar(pParent);
	if( sClassName == _T("AContainer") ) return new AContainer(pParent);
	if( sClassName == _T("AMaximizeButton") ) return new AMaximizeButton(pParent);
	if( sClassName == _T("AMinimizeButton") ) return new AMinimizeButton(pParent);
	if( sClassName == _T("ACloseButton") ) return new ACloseButton(pParent);
	if( sClassName == _T("ASysIconImageList")) return new ASysIconImageList(pParent);
	if( sClassName == _T("ATimeControl")) return new ATimeControl(pParent);

	TControlCreator cc = NULL;
	if( AApplication::Get()->m_aControlCreator.IsExist(sClassName) )
	{
		cc = (TControlCreator)AApplication::Get()->m_aControlCreator.GetItem(sClassName);
		if( cc != NULL ) return dynamic_cast<AControl*>( cc(pParent) );
	}

	return NULL;
}

void AControl::Refresh()
{
	if( m_pUICore->GetLayoutEnabled() )
	{
		m_pUICore->Refresh(GetAbsPosition());
	}
}
bool AControl::CanFocus()
{
	return m_bCanFocus;
}
static bool ToBool(const AString& s)
{
	if( s == _T("0") ) return false;
	else return true;
}
static TFloatType ToFloatType(const AString& s)
{
	TFloatType ft = ftFloatNone;
	if( s == _T("x") ) ft= ftFloatX;
	else if( s == _T("y") ) ft = ftFloatY;
	else if( s == _T("xy") ) ft = ftFloat;
	return ft;
}
static int ToInt(const AString& s)
{
	return AIntVal(s);
}
static eAlign ToAlign(const AString& s)
{
	eAlign al = alNone;
	if( s == _T("l") ) al = alLeft;
	else if( s == _T("r") ) al = alRight;
	else if( s == _T("t") ) al = alTop;
	else if( s == _T("b") ) al = alBottom;
	else if( s == _T("c") ) al = alClient;
	return al;
}
static AColor ToColor(const AString& s)
{
	AColor cr;
	AStringArray sa;
	sa.FromString(s,_T(","));
	if( sa.GetCount() > 0 ) cr.alpha = ToInt(sa[0]);
	if( sa.GetCount() > 1 ) cr.red = ToInt(sa[1]);
	if( sa.GetCount() > 2 ) cr.green = ToInt(sa[2]);
	if( sa.GetCount() > 3 ) cr.blue = ToInt(sa[3]);
	return cr;
}
static void ToAnchor(const AString& s,AAnchors& ar)
{
	ar.m_bLeft = (s.Find(_T("l")) > -1 ? true : false);
	ar.m_bTop = (s.Find(_T("t")) > -1 ? true : false);
	ar.m_bRight = (s.Find(_T("r")) > -1 ? true : false);
	ar.m_bBottom = (s.Find(_T("b")) > -1 ? true : false);
}
static FontStyle ToFontStyle(const AString& s)
{
	int fs = FontStyleRegular;
	if( s.Find(_T("b")) > -1 ) fs |= FontStyleBold;
	if( s.Find(_T("i")) > -1 ) fs |= FontStyleItalic;
	if( s.Find(_T("u")) > -1 ) fs |= FontStyleUnderline;
	if( s.Find(_T("s")) > -1 ) fs |= FontStyleStrikeout;
	return (FontStyle)fs;
}
void AControl::SetPropFromNode(AMLBody* pNode)
{
	if( pNode == NULL ) return;
	if( pNode->HasProp(_T("n")) ) SetName( pNode->GetProp(_T("n")) );//Name
	if( pNode->HasProp(_T("en")) ) SetEnabled( ToBool(pNode->GetProp(_T("en"))));//Enabled
	if( pNode->HasProp(_T("vi")) ) SetVisible( ToBool(pNode->GetProp(_T("vi"))) );//Visible
	if( pNode->HasProp(_T("ft")) ) SetFloatType(ToFloatType(pNode->GetProp(_T("ft"))));//Float
	if( pNode->HasProp(_T("anc")) ) ToAnchor(pNode->GetProp(_T("anc")),Anchors);//Anchors

	if( pNode->HasProp(_T("l"))) 
		SetLeft( ToInt(pNode->GetProp(_T("l"))) );//Left
	if( pNode->HasProp(_T("t"))) 
		SetTop( ToInt(pNode->GetProp(_T("t"))) );//Top
	if( pNode->HasProp(_T("w"))) 
		SetWidth( ToInt(pNode->GetProp(_T("w"))) );//Width
	if( pNode->HasProp(_T("h"))) 
		SetHeight( ToInt(pNode->GetProp(_T("h"))) );//Height
	if( pNode->HasProp(_T("al"))) SetAlign( ToAlign(pNode->GetProp(_T("al"))));//Align
	if( pNode->HasProp(_T("tx")) ) SetText( pNode->GetProp(_T("tx")) );//Text
	if( pNode->HasProp(_T("hnt"))) SetHint(pNode->GetProp(_T("hnt")));//Hint
	if( pNode->HasProp(_T("bw")) ) SetBorderWidth(ToInt(pNode->GetProp(_T("bw"))));//BorderWidth
	if( pNode->HasProp(_T("pd")) ) SetPadding(ToInt(pNode->GetProp(_T("pd"))));//Padding
	if( pNode->HasProp(_T("cr")) ) SetColor( ToColor(pNode->GetProp(_T("cr"))));//Color
	if( pNode->HasProp(_T("anc_r"))) Anchors.m_iRightDelta = ToInt( pNode->GetProp(_T("anc_r")));
	if( pNode->HasProp(_T("anc_b"))) Anchors.m_iBottomDelta = ToInt( pNode->GetProp(_T("anc_b")));
	
	if( pNode->HasProp(_T("bcr"))) BorderColor = ToColor( pNode->GetProp(_T("bcr")));//BorderColor

	AMLBody* pFontBody = pNode->FindBody(_T("font"));//Font
	if( pFontBody )
	{
		if( pFontBody->HasProp(_T("n")) ) Font.SetName( pFontBody->GetProp(_T("n")));
		if( pFontBody->HasProp(_T("sz"))) Font.SetSize( ToInt(pFontBody->GetProp(_T("sz"))));
		if( pFontBody->HasProp(_T("cr"))) Font.SetColor(ToColor(pFontBody->GetProp(_T("cr"))));
		if( pFontBody->HasProp(_T("st"))) Font.SetStyle( ToFontStyle(pFontBody->GetProp(_T("st"))));
	}
	PROP_SET(OnMouseDown);
	PROP_SET(OnMouseUp);
	PROP_SET(OnMouseMove);
	PROP_SET(OnClick);
	PROP_SET(OnPaint);
	PROP_SET(OnEnter);
	PROP_SET(OnLeave);
	PROP_SET(OnKillFocus);
	PROP_SET(OnFocus);
	PROP_SET(OnMouseWheel);
	PROP_SET(OnDblClick);
	PROP_SET(OnKeyboard);
	PROP_SET(OnStartDrag);
	PROP_SET(OnDragOver);
	PROP_SET(OnDragDrop);
	PROP_SET(OnEndDrag);
	PROP_SET(OnDrawDrag);
	PROP_SET(OnLayout);
	PROP_SET(OnDestroy);

}

AControl* AControl::CreateInstance(AMLBody* pBody,AComponent* pParent)
{
	AControl* p = NULL;
	if( pBody )
	{
		AString sType = pBody->GetProp(_T("type"));
		if( sType == _T("") ) sType = pBody->GetName();//为了支持EShell，追加对节点名的判断
		p = CreateInstance(_T("A")+sType,pParent);
		if( p )
		{
			p->SetPropFromNode(pBody);

			AContainer* pContainer = dynamic_cast<AContainer*>(p);
			if( pContainer )
			{
				for(int i=0;i<pBody->GetChildCount();i++)
				{
					AMLBody* p = pBody->GetChild(i);
					AString sn = p->GetName();
					if( sn == _T("children") || sn == _T("@") ) //@for EShell
						pContainer->CreateChildren(p);
				}
			}
			
		}
	}
	return p;
}

void AControl::StartDrag(AControl* pSource,ASize szDraw)
{
	if( pSource == NULL ) pSource = this;
	if( szDraw.cx == 0 && szDraw.cy == 0 ) szDraw = ASize(48,48);
	g_DragInfo.pSource = pSource;
	g_DragInfo.szDraw = szDraw;
	g_DragInfo.bDragging = true;
	g_DragInfo.bCanceled = false;
	g_DragInfo.bAccept = false;
	POINT pt;
	::GetCursorPos(&pt);
	ARect ra=GetUICore()->GetWin()->GetAbsPosition();
	pt.x -= ra.left;pt.y-=ra.top;

	ADragEvent evt;
	evt.m_pSource = pSource;
	evt.m_szDrawSize = szDraw;
	evt.m_X = pt.x;
	evt.m_Y = pt.y;
	int dx = szDraw.cx;
	int dy = szDraw.cy;
	g_DragInfo.X = pt.x;
	g_DragInfo.Y = pt.y;
	GetUICore()->Refresh(ARect(pt.x,pt.y,pt.x+dx+2,pt.y+dy+2));
	OnStartDrag.Call(this,&evt);
}

void AControl::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	if( sPath == _T("Name") ) pValue->SetString(GetName());
	else if( sPath == _T("Enabled") ) pValue->SetString(GetEnabled() ? _T("1") : _T("0") );
	else if( sPath == _T("Visible") ) pValue->SetString(GetVisible() ? _T("1") : _T("0") );
	else if( sPath == _T("Float") ) pValue->SetString(AUTIL::FloatTypeToString(GetFloatType()));
	else if( sPath == _T("Anchors") ) pValue->SetString(AUTIL::AnchorsToString(this->Anchors));
	else if( sPath == _T("Left") ) pValue->SetInt( GetLeft());
	else if( sPath == _T("Top") ) pValue->SetInt( GetTop());
	else if( sPath == _T("AbsLeft") ) pValue->SetInt( GetAbsPosition().left);
	else if( sPath == _T("AbsTop") ) pValue->SetInt( GetAbsPosition().top );
	else if( sPath == _T("Width") ) pValue->SetInt( GetWidth());
	else if( sPath == _T("Height") ) pValue->SetInt( GetHeight());
	else if( sPath == _T("Align")) pValue->SetString( AUTIL::AlignToString(GetAlign()));
	else if( sPath == _T("Text")) pValue->SetString( GetText() );
	else if( sPath == _T("BorderWidth")) pValue->SetString( AStringVal(GetBorderWidth()));
	else if( sPath == _T("Padding")) pValue->SetString( AStringVal(GetPadding()));
	else if( sPath == _T("Right")) pValue->SetString( AStringVal(Anchors.m_iRightDelta));
	else if( sPath == _T("Bottom")) pValue->SetString( AStringVal(Anchors.m_iBottomDelta));
	else if( sPath == _T("OnMouseDown")) pValue->SetString(OnMouseDown.GetMCF());
	else if( sPath == _T("OnMouseUp")) pValue->SetString(OnMouseUp.GetMCF());
	else if( sPath == _T("OnMouseMove")) pValue->SetString(OnMouseMove.GetMCF());
	else if( sPath == _T("OnClick")) pValue->SetString(OnClick.GetMCF());
	else if( sPath == _T("OnPaint")) pValue->SetString(OnPaint.GetMCF());
	else if( sPath == _T("OnEnter")) pValue->SetString(OnEnter.GetMCF());
	else if( sPath == _T("OnLeave")) pValue->SetString(OnLeave.GetMCF());
	else if( sPath == _T("OnKillFocus")) pValue->SetString(OnKillFocus.GetMCF());
	else if( sPath == _T("OnFocus")) pValue->SetString(OnFocus.GetMCF());
	else if( sPath == _T("OnMouseWheel")) pValue->SetString(OnMouseWheel.GetMCF());
	else if( sPath == _T("OnDblClick")) pValue->SetString(OnDblClick.GetMCF());
	else if( sPath == _T("OnKeyboard")) pValue->SetString(OnKeyboard.GetMCF());
	else if( sPath == _T("OnStartDrag")) pValue->SetString(OnStartDrag.GetMCF());
	else if( sPath == _T("OnDragOver")) pValue->SetString(OnDragOver.GetMCF());
	else if( sPath == _T("OnDragDrop")) pValue->SetString(OnDragDrop.GetMCF());
	else if( sPath == _T("OnEndDrag")) pValue->SetString(OnEndDrag.GetMCF());
	else if( sPath == _T("OnDrawDrag")) pValue->SetString(OnDrawDrag.GetMCF());
	else if( sPath == _T("OnLayout")) pValue->SetString(OnLayout.GetMCF());
	else if( sPath == _T("OnDestroy")) pValue->SetString(OnDestroy.GetMCF());
}

void AControl::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	if( sPath == _T("Name") ) SetName( pValue->GetString() );
	else if( sPath == _T("Enabled") ) SetEnabled( pValue->GetString()==_T("1") ? true : false );
	else if( sPath == _T("Visible") ) SetVisible( pValue->GetString()==_T("1") ? true : false );
	else if( sPath == _T("Float") ) SetFloatType( AUTIL::StringToFloatType(pValue->GetString()) );
	else if( sPath == _T("Anchors") ) AUTIL::StringToAnchors(pValue->GetString(),Anchors);
	else if( sPath == _T("Left") ) SetLeft( AIntVal(pValue->GetString()) );
	else if( sPath == _T("Top") ) SetTop( AIntVal(pValue->GetString()) );
	else if( sPath == _T("Width") ) SetWidth( AIntVal(pValue->GetString()) );
	else if( sPath == _T("Height") ) SetHeight( AIntVal(pValue->GetString()) );
	else if( sPath == _T("Align")) SetAlign( AUTIL::StringToAlign(pValue->GetString()) );
	else if( sPath == _T("Text")) SetText( pValue->GetString() );
	else if( sPath == _T("BorderWidth")) SetBorderWidth( AIntVal(pValue->GetString()) );
	else if( sPath == _T("Padding")) SetPadding( AIntVal(pValue->GetString()) );
	else if( sPath == _T("Right")) Anchors.m_iRightDelta = AIntVal(pValue->GetString());
	else if( sPath == _T("Bottom")) Anchors.m_iBottomDelta = AIntVal(pValue->GetString());
	else if( sPath == _T("OnMouseDown")) OnMouseDown.SetMCF(pValue->GetString());
	else if( sPath == _T("OnMouseUp")) OnMouseUp.SetMCF(pValue->GetString());
	else if( sPath == _T("OnMouseMove")) OnMouseMove.SetMCF(pValue->GetString());
	else if( sPath == _T("OnClick")) OnClick.SetMCF(pValue->GetString());
	else if( sPath == _T("OnPaint")) OnPaint.SetMCF(pValue->GetString());
	else if( sPath == _T("OnEnter")) OnEnter.SetMCF(pValue->GetString());
	else if( sPath == _T("OnLeave")) OnLeave.SetMCF(pValue->GetString());
	else if( sPath == _T("OnKillFocus")) OnKillFocus.SetMCF(pValue->GetString());
	else if( sPath == _T("OnFocus")) OnFocus.SetMCF(pValue->GetString());
	else if( sPath == _T("OnMouseWheel")) OnMouseWheel.SetMCF(pValue->GetString());
	else if( sPath == _T("OnDblClick")) OnDblClick.SetMCF(pValue->GetString());
	else if( sPath == _T("OnKeyboard")) OnKeyboard.SetMCF(pValue->GetString());
	else if( sPath == _T("OnStartDrag")) OnStartDrag.SetMCF(pValue->GetString());
	else if( sPath == _T("OnDragOver")) OnDragOver.SetMCF(pValue->GetString());
	else if( sPath == _T("OnDragDrop")) OnDragDrop.SetMCF(pValue->GetString());
	else if( sPath == _T("OnEndDrag")) OnEndDrag.SetMCF(pValue->GetString());
	else if( sPath == _T("OnDrawDrag")) OnDrawDrag.SetMCF(pValue->GetString());
	else if( sPath == _T("OnLayout")) OnLayout.SetMCF(pValue->GetString());
	else if( sPath == _T("OnDestroy")) OnDestroy.SetMCF(pValue->GetString());
}

void AControl::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{

}

void AControl::SetCanFocus(bool v)
{
	m_bCanFocus = v;
}