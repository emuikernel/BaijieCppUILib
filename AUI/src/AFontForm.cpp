#include "stdafx.h"
#include "AFontForm.h"

static ACHAR* g_szDialogWnd=
	L"<c type=@FontForm@ w=@423@ h = @437@ tx=@字体@ >"
	L"<children>"
		L"<c type=@TitleBar@ n=@TitleBar@ al=@t@ h=@28@>"
		L"<children>"
			L"<c type=@Container@ n=@ButtonsPanel@ al=@c@ h = @28@ >"
			L"<children>"
				L"<c type=@CloseButton@ n=@cb@ al=@r@ w = @26@ />"
				L"<c type=@MaximizeButton@ n=@mb@ al=@r@ w = @26@ />"
				L"<c type=@MinimizeButton@ n=@ib@ al=@r@ w = @26@ />"
			L"</children>"
			L"</c>"
		L"</children>"
		L"</c>"
		L"<c type=@Container@ n=@SelectPanel@ pd=@5@ al=@c@ w=@150@>"
			L"<children>"
				L"<c type=@Container@ al=@c@  pd=@3@ >"
					L"<children>"
						L"<c type=@Label@ tx=@字体：@  al=@t@ h=@16@ />"
						L"<c type=@Edit@ n=@FontEdit@ al=@t@ h=@24@ />"
						L"<c type=@ListBox@ n=@FontListBox@ al=@c@ />"
					L"</children>"
				L"</c>"
				L"<c type=@Container@ al=@r@ w=@87@ pd=@3@ >"
					L"<children>"
						L"<c type=@Label@ tx=@大小：@ al=@t@ h=@16@ />"
						L"<c type=@Edit@ n=@FontSizeEdit@ al=@t@ h=@24@ />"
						L"<c type=@ListBox@ n=@FontSizeListBox@ al=@c@ />"
					L"</children>"
				L"</c>"
				L"<c type=@Container@ al=@r@ w=@146@ pd=@3@ >"
					L"<children>"
						L"<c type=@Label@ tx=@字型：@  h=@16@ al=@t@ />"
						L"<c type=@Edit@ n=@FontStyleEdit@ al=@t@ h=@24@ />"
						L"<c type=@ListBox@ n=@FontStyleListBox@ al=@c@ />"
					L"</children>"
				L"</c>"
			L"</children>"
		L"</c>"
		L"<c type=@Container@ n=@BottomContainer@ al=@b@ h=@46@>"
			L"<children>"
				L"<c type=@Button@ n=@OK@ tx=@打开@ l=@400@ t=@10@ w=@80@ h=@20@ anc=@rt@ anc_r=@120@ />"
				L"<c type=@Button@ n=@Cancel@ tx=@取消@ l=@500@ t=@10@ w=@80@ h=@20@ anc=@rt@ anc_r=@20@ />"
			L"</children>"
		L"</c>"

		L"<c type=@Container@ n=@DemoPanel@ pd=@5@ al=@b@ h=@100@>"
			L"<children>"
				L"<c type=@Container@ al=@t@ pd=@3@ h=@23@ >"
					L"<children>"
						L"<c type=@Label@ tx=@ 示例 @ al=@l@ w=@60@ />"
						L"<c type=@Button@ n=@ColorSelector@ al=@r@ w=@87@ />"
						L"<c type=@Label@ tx=@颜色：@ al=@r@ w=@50@ />"
					L"</children>"
				L"</c>"
				
				L"<c type=@Container@ n=@Demo@ al=@c@ pd=@3@/>"
			L"</children>"
		L"</c>"
		
	L"</children>"
	L"</c>"
	;

class _FontNameCache
{
public:
	_FontNameCache()
	{
		m_bLoaded = false;
	}
	~_FontNameCache()
	{

	}
	void FillFonts(AListBox* pBox)
	{
		if( !m_bLoaded )
		{
			LoadFonts();
			m_bLoaded = true;
		}
		//
		AStringArrayEnumerator em(&m_aFontName);
		while( em.Next() )
		{
			pBox->AddString( em.Cur() );
		}
	}
private:
	bool m_bLoaded;
	AStringArray	m_aFontName;
	void LoadFonts()
	{
		HDC hDC =  AApplication::Get()->GetMainForm()->GetWindow()->GetDC();
		::EnumFontFamilies((HDC)hDC,NULL,(FONTENUMPROC)EnumFontFamProc,(LPARAM)this);  
		//::ReleaseDC(0,hDC);
	}
	static int CALLBACK EnumFontFamProc(  
		ENUMLOGFONT *lpelf,    // logical-font data  
		NEWTEXTMETRIC *lpntm,  // physical-font data  
		DWORD nFontType,        // type of font  
		LPARAM lParam          // application-defined data  
		)  
	{  
		if(  (nFontType & TRUETYPE_FONTTYPE) )//( nFontType & RASTER_FONTTYPE ) ||
		{
			_FontNameCache* pCache = (_FontNameCache *)lParam; 
			AString sFont;
			sFont = lpelf->elfLogFont.lfFaceName;
			pCache->m_aFontName.Add(sFont);
		}
		
		return 1;
	}
};
_FontNameCache g_FontNameCache;

AFontForm::AFontForm(AComponent* pOwner):AForm(pOwner)
{
	m_nButtonID = NULL;
	m_pFont = NULL;

	MAP_CONTROL_INIT(OK);
	MAP_CONTROL_INIT(Cancel);
	MAP_CONTROL_INIT(ButtonsPanel);
	MAP_CONTROL_INIT(TitleBar);
	MAP_CONTROL_INIT(FontListBox);
	MAP_CONTROL_INIT(FontEdit);
	MAP_CONTROL_INIT(FontStyleEdit);
	MAP_CONTROL_INIT(FontStyleListBox);
	MAP_CONTROL_INIT(FontSizeEdit);
	MAP_CONTROL_INIT(FontSizeListBox);
	MAP_CONTROL_INIT(Demo);
	MAP_CONTROL_INIT(ColorSelector);
}

AFontForm::~AFontForm()
{

}

void AFontForm::CreateFromXML()
{
	afc_object<AMLBody> pBody(new AMLBody());
	AString xml;
	xml = g_szDialogWnd;
	xml.Replace(_T("@"),_T("\""));
	pBody->FromXML(xml);
	this->FromXML(&pBody);
	this->CenterToScreen();
}

void AFontForm::ConnectEvent()
{
	EVENT_CONNECT(GetButtonsPanel(),OnMouseDown,TitleBackDoMouseDown);
	EVENT_CONNECT(GetButtonsPanel(),OnMouseMove,TitleBackDoMouseMove);
	EVENT_CONNECT(GetOK(),OnClick,OnOKClick);
	EVENT_CONNECT(GetCancel(),OnClick,OnCancelClick);
	EVENT_CONNECT(GetFontListBox(),OnItemPaint,OnFontItemPaint);
	EVENT_CONNECT(GetFontListBox(),OnClick,OnFontNameClick);
	EVENT_CONNECT(GetFontStyleListBox(),OnClick,OnFontStyleClick);
	EVENT_CONNECT(GetFontSizeListBox(),OnClick,OnFontSizeClick);
	EVENT_CONNECT(GetDemo(),OnPaint,OnDemoPaint);
	EVENT_CONNECT(GetColorSelector(),OnPaint,OnColorSelectorPaint);
	EVENT_CONNECT(GetColorSelector(),OnClick,OnColorSelectorClick);
}

void AFontForm::TitleBackDoMouseDown(AObject* pSender,AEvent* pEvent)
{
	GetTitleBar()->DoMouseDown(pEvent);
}
void AFontForm::TitleBackDoMouseMove(AObject* pSender,AEvent* pEvent)
{
	GetTitleBar()->DoMouseMove(pEvent);
}

void AFontForm::OnOKClick(AObject* pSender,AEvent* pEvent)
{
	
	if( m_nButtonID ) 
	{
		*m_nButtonID = dbOK;
	}
	Destroy();
}
void AFontForm::OnCancelClick(AObject* pSender,AEvent* pEvent)
{
	if( m_nButtonID ) *m_nButtonID = dbCancel;
	Destroy();
}



void AFontForm::InitControls()
{
	
}


void AFontForm::DoCreate()
{
	//GetFontListBox()->SetPadding(3);

	m_pUICore->SetLayoutEnabled(false);
	g_FontNameCache.FillFonts(GetFontListBox());
	m_pUICore->SetLayoutEnabled(true);
	GetFontListBox()->DoLayout();

	FillFontStyle();
	FillFontSize();
}

void AFontForm::OnFontItemPaint(AObject* pSender,AEvent* pEvent)
{
	AListBoxItem* pItem = dynamic_cast<AListBoxItem*>(pSender);
	if( pItem == NULL ) return;
	ACanvas* cs = pItem->GetUICore()->GetCanvas();

	ASolidBrush br;
	ARect r = pItem->GetAbsPosition();
	if( pItem->GetUICore()->InDrawArea(r) == false ) return;

	AString sFontName;
	sFontName = pItem->GetText();

	if( pItem->GetSelected() == true )
	{
		br.SetColor(  AApplication::Get()->GetCurSkin()->m_SelectedColor );
		cs->Use(&br)->FillRect(r);
	}
	else if( (AControl *)pItem == pItem->GetUICore()->GetMouseOverControl() )
	{
		br.SetColor(  AApplication::Get()->GetCurSkin()->m_OverColor );
		cs->Use(&br)->FillRect(r);

	}

	AFont f;
	f.SetName( sFontName );  //fix me later : 为什么有些字体显示不出来？
	f.SetSize( 12 );

	
	cs->Use(&f)->DrawText(r,sFontName,StringAlignmentNear,StringAlignmentMiddle);
}

void AFontForm::OnFontNameClick(AObject* pSender,AEvent* pEvent)
{
	GetFontEdit()->SetText( GetFontListBox()->GetSelect() );
	m_Font.SetName( GetFontListBox()->GetSelect() );
	GetDemo()->Refresh();
}

void AFontForm::FillFontStyle()
{
	GetFontStyleListBox()->AddString(_T("常规"));
	GetFontStyleListBox()->AddString(_T("粗体"));
	GetFontStyleListBox()->AddString(_T("斜体"));
	GetFontStyleListBox()->AddString(_T("粗体 斜体"));
}

void AFontForm::OnFontStyleClick(AObject* pSender,AEvent* pEvent)
{
	GetFontStyleEdit()->SetText( GetFontStyleListBox()->GetSelect() );
}

static ACHAR* g_szFontSize[]=
{
	L"8",L"9",L"10",L"11",L"12",L"14",L"16",L"18",L"20",L"22",L"24",L"26",L"28",L"36",L"48",L"72",
	L"初号",L"小初",L"一号",L"小一",L"二号",L"小二",L"三号",L"小三",L"四号",L"小四",
	L"五号",L"小五",L"六号",L"小六",L"七号",L"八号",NULL
};
void AFontForm::FillFontSize()
{
	ACHAR** p = g_szFontSize;
	while( *p ) 
	{
		GetFontSizeListBox()->AddString(*p);
		p++;
	}
}

void AFontForm::OnFontSizeClick(AObject* pSender,AEvent* pEvent)
{
	GetFontSizeEdit()->SetText( GetFontSizeListBox()->GetSelect() );
}

static ACHAR* g_szFontDemo=_T("中国梦 China Dream 2013");

void AFontForm::OnDemoPaint(AObject* pSender,AEvent* pEvent)
{
	AContainer* pItem = dynamic_cast<AContainer*>(pSender);
	if( pItem == NULL ) return;
	ACanvas* cs = pItem->GetUICore()->GetCanvas();

	ASolidBrush br;
	ARect r = pItem->GetAbsPosition();
	if( pItem->GetUICore()->InDrawArea(r) == false ) return;

	ARect rx = r;
	rx.Deflate(10,15,10,10);

	cs->Use(&m_Font)->DrawText( rx,g_szFontDemo );

	
	APen pen;
	pen.SetWidth(0.5);
	pen.SetColor( AApplication::Get()->GetCurSkin()->m_BorderColor );
	r.Deflate(0,10,0,0);
	cs->Use(&pen)->DrawRoundRect(r,5);
}

void AFontForm::OnColorSelectorPaint(AObject* pSender,AEvent* pEvent)
{
	AButton* pButton = dynamic_cast<AButton *>(pSender);
	if( pButton == NULL ) return;
	ACanvas* cs = GetUICore()->GetCanvas();

	ARect r = pButton->GetAbsPosition();
	APen pen;
	pen.SetWidth(0.5);
	pen.SetColor( AApplication::Get()->GetCurSkin()->m_BorderColor );
	ASolidBrush br;
	br.SetColor( m_Font.GetColor() );
	cs->Use(&br)->FillRect(r);
	cs->Use(&pen)->DrawRect(r);
}

void AFontForm::OnColorSelectorClick(AObject* pSender,AEvent* pEvent)
{
	AColorDialog* cd = new AColorDialog(this);
	cd->SetColor( m_Font.GetColor() );
	if( cd->Execute() )
	{
		m_Font.SetColor( cd->GetColor() );
		this->Refresh();
	}
}

