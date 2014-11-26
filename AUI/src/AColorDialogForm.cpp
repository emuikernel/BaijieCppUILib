#include "stdafx.h"
#include "AColorDialogForm.h"

static ACHAR* g_szDialogWnd=
	L"<c type=@ColorDialogForm@ w=@454@ h = @367@ tx=@颜色@ szb=@0@ >"
	L"<children>"
		L"<c type=@TitleBar@ n=@TitleBar@ al=@t@ h=@52@>"
			L"<children>"
				L"<c type=@Container@ n=@ButtonsPanel@ al=@t@ h = @28@ >"
					L"<children>"
						L"<c type=@CloseButton@ n=@cb@ al=@r@ w = @26@ />"
						L"<c type=@MinimizeButton@ n=@ib@ al=@r@ w = @26@ />"
					L"</children>"
				L"</c>"
			L"</children>"
		L"</c>"
		L"<c type=@Container@ al=@t@ h=@200@ >"
			L"<children>"
				L"<c type=@Container@ n=@BaseColorContainer@ l=@10@ t=@10@ w=@179@ h=@180@ />"
				L"<c type=@Container@ n=@SLSelector@  l=@190@ t=@10@ w=@200@ h=@180@ />"
				L"<c type=@VertSlide@ n=@ColorSelector@ l=@400@ t=@5@ w=@34@ h=@190@ />"
			L"</children>"
		L"</c>"
		L"<c type=@Container@ n=@BottomContainer@ al=@b@ h=@40@ >"
			L"<children>"
				L"<c type=@Button@ n=@OK@ tx=@确定@ l=@200@ t=@10@ w=@80@ h=@20@ anc=@rt@ anc_r=@120@ />"
				L"<c type=@Button@ n=@Cancel@ tx=@取消@ l=@300@ t=@10@ w=@80@ h=@20@ anc=@rt@ anc_r=@20@ />"
				L"<c type=@Label@ n=@HexColor@ l=@10@ t=@10@ w=@90@ h=@12@ />"
			L"</children>"
		L"</c>"
		L"<c type=@Container@ al=@c@ >"
			L"<children>"
				L"<c type=@Label@ tx=@R:@ l=@10@ t=@0@ w=@12@ h=@12@ />"
				L"<c type=@Label@ tx=@G:@ l=@10@ t=@20@ w=@12@ h=@12@ />"
				L"<c type=@Label@ tx=@B:@ l=@10@ t=@40@ w=@12@ h=@12@ />"
				L"<c type=@HorzSlide@ n=@RSlide@ l=@30@ t=@0@ w=@99@ h=@18@ />"
				L"<c type=@HorzSlide@ n=@GSlide@ l=@30@ t=@20@ w=@99@ h=@18@ />"
				L"<c type=@HorzSlide@ n=@BSlide@ l=@30@ t=@40@ w=@99@ h=@18@ />"
				L"<c type=@Edit@ n=@REdit@ l=@139@ t=@0@ w=@30@ h=@18@ />"
				L"<c type=@Edit@ n=@GEdit@ l=@139@ t=@20@ w=@30@ h=@18@ />"
				L"<c type=@Edit@ n=@BEdit@ l=@139@ t=@40@ w=@30@ h=@18@ />"
				L"<c type=@Label@ tx=@H:@ l=@190@ t=@0@ w=@12@ h=@12@ />"
				L"<c type=@Label@ tx=@S:@ l=@190@ t=@20@ w=@12@ h=@12@ />"
				L"<c type=@Label@ tx=@L:@ l=@190@ t=@40@ w=@12@ h=@12@ />"
				L"<c type=@HorzSlide@ n=@HSlide@ l=@210@ t=@0@ w=@99@ h=@18@ />"
				L"<c type=@HorzSlide@ n=@SSlide@ l=@210@ t=@20@ w=@99@ h=@18@ />"
				L"<c type=@HorzSlide@ n=@LSlide@ l=@210@ t=@40@ w=@99@ h=@18@ />"
				L"<c type=@Edit@ n=@HEdit@ l=@319@ t=@0@ w=@30@ h=@18@ />"
				L"<c type=@Edit@ n=@SEdit@ l=@319@ t=@20@ w=@30@ h=@18@ />"
				L"<c type=@Edit@ n=@LEdit@ l=@319@ t=@40@ w=@30@ h=@18@ />"
				L"<c type=@Label@ tx=@度@ l=@354@ t=@0@ w=@12@ h=@16@ />"
				L"<c type=@Label@ tx=@%@ l=@354@ t=@20@ w=@12@ h=@12@ />"
				L"<c type=@Label@ tx=@%@ l=@354@ t=@40@ w=@12@ h=@12@ />"
			L"</children>"
		L"</c>"
	L"</children>"
	L"</c>"
	;

//---------- 基本颜色表
typedef struct _ColorEntry
{
	ACHAR* szName;
	AColor cr;
}TColorEntry;
TColorEntry g_ColorEntrys[]={
	{ _T("黑色"),AColor(255,0,0,0)},		{ _T("棕色"),AColor(255,0x99,0x33,0)},
	{ _T("橄榄绿"),AColor(255,0x33,0x33,0)},	{ _T("深绿"),AColor(255,0,0x33,0)},
	{ _T("深湖色"),AColor(255,0,0x33,0x66)},	{ _T("深蓝"),AColor(255,0,0,0x80)},
	{ _T("靛青"),AColor(255,0x33,0x33,0x99)},	{ _T("80%灰"),AColor(255,0x33,0x33,0x33)},

	{ _T("深红"),AColor(255,0x80,0,0)},		{ _T("橙色"),AColor(255,0xFF,0x66,0)},
	{ _T("深黄"),AColor(255,0x80,0x80,0)},		{ _T("绿色"),AColor(255,0,0x80,0)},
	{ _T("湖色"),AColor(255,0,0x80,0x80)},		{ _T("蓝色"),AColor(255,0,0,0xFF)},
	{ _T("灰蓝"),AColor(255,0x66,0x66,0x99)},	{ _T("50%灰"),AColor(255,0x80,0x80,0x80)},

	{ _T("红色"),AColor(255,0xFF,0,0)},		{ _T("浅橙"),AColor(255,0xFF,0x99,0)},
	{ _T("石灰"),AColor(255,0x99,0xcc,0)},		{ _T("海绿"),AColor(255,0x33,0x99,0x66)},
	{ _T("水绿"),AColor(255,0x33,0xcc,0xcc)},	{ _T("浅蓝"),AColor(255,0x33,0x66,0xff)},
	{ _T("紫罗兰"),AColor(255,0x80,0,0x80)},	{ _T("40%灰"),AColor(255,0x96,0x96,0x96)},

	{ _T("粉红"),AColor(255,0xff,0,0xff)},		{ _T("金色"),AColor(255,0xff,0xcc,0)},
	{ _T("黄色"),AColor(255,0xff,0xff,0)},		{ _T("亮绿"),AColor(255,0,0xff,0)},
	{ _T("蓝绿"),AColor(255,0,0xff,0xff)},		{ _T("天蓝"),AColor(255,0,0xcc,0xff)},
	{ _T("紫红"),AColor(255,0x99,0x33,0x66)},	{ _T("25%灰"),AColor(255,0xc0,0xc0,0xc0)},

	{ _T("玫瑰"),AColor(255,0xff,0x99,0xcc)},	{ _T("黄褐"),AColor(255,0xff,0xcc,0x99)},
	{ _T("浅黄"),AColor(255,0xff,0xff,0x99)},	{ _T("浅绿"),AColor(255,0xcc,0xff,0xcc)},
	{ _T("浅蓝绿"),AColor(255,0xcc,0xff,0xff)},	{ _T("粉青"),AColor(255,0x99,0xcc,0xff)},
	{ _T("淡紫色"),AColor(255,0xcc,0x99,0xff)},	{ _T("白色"),AColor(255,0xff,0xff,0xff)}
};

AColorDialogForm::AColorDialogForm(AComponent* pOwner):AForm(pOwner)
{
	m_nButtonID = NULL;
	m_pColor = NULL;
	m_bDisableChange = false;

	MAP_CONTROL_INIT(OK);
	MAP_CONTROL_INIT(Cancel);

	MAP_CONTROL_INIT(ButtonsPanel);
	MAP_CONTROL_INIT(ColorSelector);
	MAP_CONTROL_INIT(TitleBar);
	MAP_CONTROL_INIT(SLSelector);
	MAP_CONTROL_INIT(BaseColorContainer);
	MAP_CONTROL_INIT(HexColor);
	MAP_CONTROL_INIT(RSlide);
	MAP_CONTROL_INIT(GSlide);
	MAP_CONTROL_INIT(BSlide);
	MAP_CONTROL_INIT(REdit);
	MAP_CONTROL_INIT(GEdit);
	MAP_CONTROL_INIT(BEdit);
	MAP_CONTROL_INIT(HSlide);
	MAP_CONTROL_INIT(SSlide);
	MAP_CONTROL_INIT(LSlide);
	MAP_CONTROL_INIT(HEdit);
	MAP_CONTROL_INIT(SEdit);
	MAP_CONTROL_INIT(LEdit);
}

AColorDialogForm::~AColorDialogForm()
{

}

void AColorDialogForm::CreateFromXML()
{
	afc_object<AMLBody> pBody(new AMLBody());
	AString xml;
	xml = g_szDialogWnd;
	xml.Replace(_T("@"),_T("\""));
	pBody->FromXML(xml);
	this->FromXML(&pBody);
	this->CenterToScreen();
}

void AColorDialogForm::ConnectEvent()
{
	EVENT_CONNECT(GetButtonsPanel(),OnMouseDown,TitleBackDoMouseDown);
	EVENT_CONNECT(GetButtonsPanel(),OnMouseMove,TitleBackDoMouseMove);
	EVENT_CONNECT(GetOK(),OnClick,OnOKClick);
	EVENT_CONNECT(GetCancel(),OnClick,OnCancelClick);
	EVENT_CONNECT(GetColorSelector(),OnPaint,OnColorSelectorPaint);
	EVENT_CONNECT(GetColorSelector(),OnMouseDown,OnColorSelectorMouseDown);
	EVENT_CONNECT(GetSLSelector(),OnPaint,OnSLPaint);
	EVENT_CONNECT(GetSLSelector(),OnMouseDown,OnSLMouseDown);
	EVENT_CONNECT(GetColorSelector(),OnScroll,OnColorScroll);
	EVENT_CONNECT(GetSLSelector(),OnMouseMove,OnSLMouseMove);
}

void AColorDialogForm::TitleBackDoMouseDown(AObject* pSender,AEvent* pEvent)
{
	GetTitleBar()->DoMouseDown(pEvent);
}

void AColorDialogForm::TitleBackDoMouseMove(AObject* pSender,AEvent* pEvent)
{
	GetTitleBar()->DoMouseMove(pEvent);
}

void AColorDialogForm::OnOKClick(AObject* pSender,AEvent* pEvent)
{
	*m_pColor = m_Color;
	if( m_nButtonID ) 
	{
		*m_nButtonID = dbOK;
	}
	Destroy();
}
void AColorDialogForm::OnCancelClick(AObject* pSender,AEvent* pEvent)
{
	if( m_nButtonID ) *m_nButtonID = dbCancel;
	Destroy();
}



void AColorDialogForm::InitControls()
{
	
}


void AColorDialogForm::DoCreate()
{
	InitColorControl();
	m_bDisableChange = true;
	{
		_updateRGB();
		_updateHSL();
	}
	m_bDisableChange = false;
}

void AColorDialogForm::InitColorControl()
{
	if( m_pColor ) 
	{
		m_Color = *m_pColor;
		m_HSLColor.FromRGB(m_Color);
	}

	GetColorSelector()->SetRange(360);
	GetColorSelector()->SetMiniStep(2);
	GetColorSelector()->SetBigStep(2);

	//创建基本色对应的按钮
	ALabel* pLabel = new ALabel(GetBaseColorContainer());
	pLabel->SetText(_T("基本颜色："));
	pLabel->SetAlign(alTop);
	pLabel->SetHeight(18);
	
	int iBaseCount = sizeof(g_ColorEntrys) / sizeof(g_ColorEntrys[0]);
	int iButtonWidth = 14;
	int iRowSpace = 6;
	int iOffX = 0;
	int iOffY = 23;

	for(int i=0;i<iBaseCount;i++)
	{
		int row,col;
		row = i / 8;
		col = i % 8;

		int x,y;
		x = col * (iButtonWidth+iRowSpace);
		y = row * (iButtonWidth+iRowSpace);

		AButton* pButton = new AButton(GetBaseColorContainer());
		pButton->SetLeft(x + iOffX);
		pButton->SetTop(y+iOffY);
		pButton->SetWidth(iButtonWidth);
		pButton->SetHeight(iButtonWidth);
		pButton->SetTag( (void *)i );
		pButton->SetName( AString().Format(_T("cb%d"),i));
		pButton->SetHint( g_ColorEntrys[i].szName );
		EVENT_CONNECT(pButton,OnPaint,OnBaseColorButtonPaint);
		EVENT_CONNECT(pButton,OnClick,OnBaseColorClick);
	}

	//颜色对比控件
	AButton* pColorDiffButton = new AButton(GetBaseColorContainer());
	pColorDiffButton->SetLeft(iOffX);
	pColorDiffButton->SetTop(iOffY + (iButtonWidth+iRowSpace) * 5 + 10);
	pColorDiffButton->SetWidth((iButtonWidth+iRowSpace)*8);
	pColorDiffButton->SetHeight( 180 - pColorDiffButton->GetTop() );
	EVENT_CONNECT(pColorDiffButton,OnPaint,OnColorDiffButtonPaint);

	//滑块
	_initSlide(GetRSlide(),255);
	_initSlide(GetGSlide(),255);
	_initSlide(GetBSlide(),255);
	_initSlide(GetHSlide(),360);
	_initSlide(GetSSlide(),100);
	_initSlide(GetLSlide(),100);

	//控件改变事件
	EVENT_CONNECT(GetRSlide(),OnScroll,OnSlideChange);
	EVENT_CONNECT(GetGSlide(),OnScroll,OnSlideChange);
	EVENT_CONNECT(GetBSlide(),OnScroll,OnSlideChange);
	EVENT_CONNECT(GetHSlide(),OnScroll,OnSlideChange);
	EVENT_CONNECT(GetSSlide(),OnScroll,OnSlideChange);
	EVENT_CONNECT(GetLSlide(),OnScroll,OnSlideChange);
	EVENT_CONNECT(GetREdit(),OnChange,OnEditChange);
	EVENT_CONNECT(GetGEdit(),OnChange,OnEditChange);
	EVENT_CONNECT(GetBEdit(),OnChange,OnEditChange);
	EVENT_CONNECT(GetHEdit(),OnChange,OnEditChange);
	EVENT_CONNECT(GetSEdit(),OnChange,OnEditChange);
	EVENT_CONNECT(GetLEdit(),OnChange,OnEditChange);
}

void AColorDialogForm::_initSlide(AHorzSlide* pSlide,int iRange)
{
	pSlide->SetRange(iRange);
	pSlide->SetMiniStep(1);
	pSlide->SetBigStep(1);
}

void AColorDialogForm::OnColorSelectorPaint(AObject* pSender,AEvent* pEvent)
{
	AVertSlide* pColorSelector = GetColorSelector();
	ARect rx = pColorSelector->GetAbsPosition();
	ACanvas* cs = GetUICore()->GetCanvas();

	rx.Deflate(7,5,7,5);

	
	cs->SetSmoothingMode(SmoothingModeNone);
	float delta = 255.0/60.0;
	for(int i=0;i<360;)
	{
		BYTE r,g,b;
		if( i < 60 )//红 -> 绿
		{
			r = 255; g = 0; b = 0;
			g += delta * (i-0);
		}
		else if( i < 120 )//红 -> 绿
		{
			r = 255; g = 255; b = 0;
			r -= delta * (i-60);
		}
		else if( i < 180 )//绿 -> 蓝
		{
			r = 0; g = 255; b=0;
			b += delta * (i-120);
		}
		else if( i < 240 )//绿 -> 蓝
		{
			r = 0; g = 255; b = 255;
			g -= delta * (i-180);
		}
		else if( i < 300 )//蓝 -> 红
		{
			r = 0; g = 0; b = 255;
			r += delta * (i - 240);
		}
		else//蓝 -> 红
		{
			r = 255; g = 0; b = 255;
			b -= delta * (i-300);
		}

		APen pen;
		pen.SetWidth(1);
		pen.SetColor(AColor(255,r,g,b));

		i += 2;

		cs->Use(&pen)->DrawLine( APoint(rx.left,rx.bottom-i/2),APoint(rx.right,rx.bottom-i/2) );
	}
	int iPosition = pColorSelector->GetPosition();
	int y = rx.top + rx.GetHeight() * iPosition / pColorSelector->GetRange();
	APoint pt1,pt2,pt3;
	ASolidBrush br;
	br.SetColor(AApplication::Get()->GetCurSkin()->m_BorderColor );

	pt1 = APoint(rx.left-5,y-5);
	pt2 = APoint(rx.left-5,y+5);
	pt3 = APoint(rx.left,y);
	cs->Use(&br)->FillTrigle(pt1,pt2,pt3);

	pt1 = APoint(rx.right-1,y);
	pt2 = APoint(rx.right-1+6,y-5);
	pt3 = APoint(rx.right-1+6,y+5);
	
	cs->Use(&br)->FillTrigle(pt1,pt2,pt3);

	cs->SetSmoothingMode(SmoothingModeAntiAlias);
}

void AColorDialogForm::OnColorSelectorMouseDown(AObject* pSender,AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL ) return;

	ARect rx = GetColorSelector()->GetAbsPosition();
	int dy = evt->m_Y - rx.top;
	if( dy < 0 ) dy = 0;
	if( dy > rx.GetHeight() ) dy = rx.GetHeight();

	int pos = (float)dy / (float)rx.GetHeight() * (float)GetColorSelector()->GetRange();
	m_bDisableChange = true;
	GetColorSelector()->SetPosition(pos);
	m_HSLColor.h = GetColorSelector()->GetRange() - pos;
	m_Color = m_HSLColor.ToRGB();
	_updateRGB();
	_updateHSLSlide();
	m_bDisableChange = false;

	this->Refresh();
}

void AColorDialogForm::OnSLPaint(AObject* pSender,AEvent* pEvent)
{
	AColorHSL hsl;
	hsl = m_HSLColor;
	ACanvas* cs = m_pUICore->GetCanvas();
	ARect rx = GetSLSelector()->GetAbsPosition();
	for(int i=0;i<rx.GetHeight();i++)
	{
		for(int j=0;j<rx.GetWidth();j++)
		{
			AColorHSL tmpHSL;
			tmpHSL.h = hsl.h;
			tmpHSL.s = 100 * (float)j / (float)rx.GetWidth();
			tmpHSL.l = 100 * ( 1.0 - (float)i / (float)rx.GetHeight() );

			cs->DrawPoint(APoint(rx.left+j,rx.top+i),tmpHSL.ToRGB());
		}
	}
	int x = hsl.s * rx.GetWidth() / 100.0;
	int y = rx.GetHeight() - hsl.l * rx.GetHeight() / 100.0;

	APoint pt(rx.left+x,rx.top+y);
	ARect rs(pt.x-5,pt.y-5,pt.x+5,pt.y+5);

	APen pen;
	pen.SetWidth(1);
	rs.Inflate(2,2);
	pen.SetColor( AColor(255,0,0,0) );
	cs->Use(&pen)->DrawEllipse(rs);
	rs.Deflate(2,2);
	pen.SetColor( AColor(255,255,255,255) );
	pen.SetWidth(1.5);

	cs->Use(&pen)->DrawEllipse(rs);
}

void AColorDialogForm::OnSLMouseDown(AObject* pSender,AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL ) return;
	ARect r = GetSLSelector()->GetAbsPosition();
	int x,y;
	x = evt->m_X - r.left;
	y = evt->m_Y - r.top;
	
	m_HSLColor.s = x * 100 / r.GetWidth();
	m_HSLColor.l = (r.GetHeight()-y) * 100 / r.GetHeight();
	m_Color = m_HSLColor.ToRGB();

	m_bDisableChange = true;
	{
		_updateRGB();
		_updateHSLSlide();
	}
	m_bDisableChange = false;
}

void AColorDialogForm::OnSLMouseMove(AObject* pSender,AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL ) return;
	if( evt->m_KeyStates.Has(ksLeftButton) )
	{
		ARect r = GetSLSelector()->GetAbsPosition();
		int x,y;
		x = evt->m_X - r.left;
		y = evt->m_Y - r.top;

		m_HSLColor.s = x * 100 / r.GetWidth();
		m_HSLColor.l = 100 * ( 1.0 - (float)y / (float)r.GetHeight() );
		m_Color = m_HSLColor.ToRGB();
		m_bDisableChange = true;
		{
			_updateRGB();
			_updateHSLSlide();
		}
		m_bDisableChange = false;
	}
	
}

void AColorDialogForm::OnColorScroll(AObject* pSender,AEvent* pEvent)
{
	if( m_bDisableChange ) return;
	int iPos = GetColorSelector()->GetPosition();
	iPos = GetColorSelector()->GetRange() - iPos;
	m_HSLColor.h = iPos;

	m_bDisableChange = true;
	_updateHSLSlide();

	m_Color = m_HSLColor.ToRGB();
	_updateRGB();
	m_bDisableChange = false;

}

void AColorDialogForm::OnBaseColorButtonPaint(AObject* pSender,AEvent* pEvent)
{
	AButton* pButton = dynamic_cast<AButton *>( pSender );
	if( pButton == NULL ) return;

	ACanvas* cs = m_pUICore->GetCanvas();
	ARect rx = pButton->GetAbsPosition();

	ASolidBrush br;
	if( pButton == m_pUICore->GetMouseOverControl() )
	{
		br.SetColor( AApplication::Get()->GetCurSkin()->m_OverColor );
		cs->Use(&br)->FillRect(rx);
	}
	else if( pButton == m_pUICore->GetMouseDownControl() )
	{
		br.SetColor( AApplication::Get()->GetCurSkin()->m_DownColor );
		cs->Use(&br)->FillRect(rx);
	}
	else
	{
		br.SetColor( AApplication::Get()->GetCurSkin()->m_ButtonColor );
		cs->Use(&br)->FillRect(rx);
	}

	ARect r = rx;
	r.Deflate(2,2,2,2);

	br.SetColor( g_ColorEntrys[ (int)pButton->GetTag() ].cr );
	cs->Use(&br)->FillRect(r);
}

void AColorDialogForm::OnColorDiffButtonPaint(AObject* pSender,AEvent* pEvent)
{
	AButton* pButton = dynamic_cast<AButton *>( pSender );
	if( pButton == NULL ) return;

	ACanvas* cs = m_pUICore->GetCanvas();
	ARect rx = pButton->GetAbsPosition();
	ARect r1 = rx,r2 = rx;
	r1.Deflate(0,0,rx.GetWidth()/2,0);
	r2.Deflate(rx.GetWidth()/2,0,0,0);

	APen pen;
	pen.SetWidth(0.4);
	pen.SetColor( AApplication::Get()->GetCurSkin()->m_BorderColor );
	
	ASolidBrush br;
	br.SetColor( *m_pColor );
	cs->Use(&br)->FillRect(r1);
	br.SetColor( m_Color );
	cs->Use(&br)->FillRect(r2);
	cs->Use(&pen)->DrawRect(rx);

}


void AColorDialogForm::OnEditChange(AObject* pSender,AEvent* pEvent)
{
	if( m_bDisableChange ) return;

	if( (AObject*)GetREdit() == pSender )
	{
		int r = AIntVal( GetREdit()->GetText() );
		if( r < 0 ) r = 0;
		if( r > 255 ) r = 255;
		m_bDisableChange = true;
		{
			GetRSlide()->SetPosition(r);
			m_Color.red = r;
			m_HSLColor.FromRGB(m_Color);
			_updateHSL();
		}
		m_bDisableChange = false;
	
	}
	else if( (AObject*)GetGEdit() == pSender )
	{
		int r = AIntVal( GetGEdit()->GetText() );
		if( r < 0 ) r = 0;
		if( r > 255 ) r = 255;
		m_bDisableChange = true;
		{
			GetGSlide()->SetPosition(r);
			m_Color.green = r;
			m_HSLColor.FromRGB(m_Color);
			_updateHSL();
		}
		m_bDisableChange = false;
	
	}
	else if( (AObject*)GetBEdit() == pSender )
	{
		int r = AIntVal( GetBEdit()->GetText() );
		if( r < 0 ) r = 0;
		if( r > 255 ) r = 255;
		m_bDisableChange = true;
		{
			GetBSlide()->SetPosition(r);
			m_Color.blue = r;
			m_HSLColor.FromRGB(m_Color);
			_updateHSL();
		}
		m_bDisableChange = false;
		
	}
	else if( (AObject*)GetHEdit() == pSender )
	{
		int r = AIntVal( GetHEdit()->GetText() );
		if( r < 0 ) r = 0;
		if( r > 360 ) r = 360;
		m_bDisableChange = true;
		{
			GetHSlide()->SetPosition(r);
			m_HSLColor.h = r;
			m_Color = m_HSLColor.ToRGB();
			_updateRGB();
		}
		m_bDisableChange = false;
	}
	else if( (AObject*)GetSEdit() == pSender )
	{
		int r = AIntVal( GetSEdit()->GetText() );
		if( r < 0 ) r = 0;
		if( r > 100 ) r = 100;
		m_bDisableChange = true;
		{
			GetSSlide()->SetPosition(r);
			m_HSLColor.s = r;
			m_Color = m_HSLColor.ToRGB();
			_updateRGB();
		}
		m_bDisableChange = false;
	}
	else if( (AObject*)GetLEdit() == pSender )
	{
		int r = AIntVal( GetLEdit()->GetText() );
		if( r < 0 ) r = 0;
		if( r > 100 ) r = 100;
		m_bDisableChange = true;
		{
			GetLSlide()->SetPosition(r);
			m_HSLColor.l = r;
			m_Color = m_HSLColor.ToRGB();
			_updateRGB();
		}
		m_bDisableChange = false;
	}
}

void AColorDialogForm::OnSlideChange(AObject* pSender,AEvent* pEvent)
{
	if( m_bDisableChange ) return;

	if( (AObject *)GetRSlide() == pSender )
	{
		int r = GetRSlide()->GetPosition();
		if( r < 0 ) r = 0;
		if( r > 255 ) r = 255;
		m_bDisableChange = true;
		{
			GetREdit()->SetText( AStringVal(r) );
			m_Color.red = r;
			m_HSLColor.FromRGB(m_Color);
			_updateHSL();
		}
		m_bDisableChange = false;
	}
	else if( (AObject*)GetGSlide() == pSender )
	{
		int r = GetGSlide()->GetPosition();
		if( r < 0 ) r = 0;
		if( r > 255 ) r = 255;
		m_bDisableChange = true;
		{
			GetGEdit()->SetText( AStringVal(r) );
			m_Color.green = r;
			m_HSLColor.FromRGB(m_Color);
			_updateHSL();
		}
		m_bDisableChange = false;
	}
	else if( (AObject*)GetBSlide() == pSender )
	{
		int r = GetBSlide()->GetPosition();
		if( r < 0 ) r = 0;
		if( r > 255 ) r = 255;
		m_bDisableChange = true;
		{
			GetBEdit()->SetText( AStringVal(r) );
			m_Color.blue = r;
			m_HSLColor.FromRGB(m_Color);
			_updateHSL();
		}
		m_bDisableChange = false;
	}
	else if( (AObject*)GetHSlide() == pSender )
	{
		int r = GetHSlide()->GetPosition();
		if( r < 0 ) r = 0;
		if( r > 360 ) r = 360;
		m_HSLColor.h = r;
		m_bDisableChange = true;
		{
			GetHEdit()->SetText( AStringVal(r) );
			GetColorSelector()->SetPosition(GetHSlide()->GetPosition());
			m_Color = m_HSLColor.ToRGB();
			_updateRGB();
		}
		m_bDisableChange = false;
	}
	else if( (AObject*)GetSSlide() == pSender )
	{
		int r = GetSSlide()->GetPosition();
		if( r < 0 ) r = 0;
		if( r > 100 ) r = 100;
		m_HSLColor.s = r;
		m_bDisableChange = true;
		{
			GetSEdit()->SetText( AStringVal(r) );
			m_Color = m_HSLColor.ToRGB();
			_updateRGB();
		}
		m_bDisableChange = false;
	}
	else if( (AObject*)GetLSlide() == pSender )
	{
		int r = GetLSlide()->GetPosition();
		if( r < 0 ) r = 0;
		if( r > 100 ) r = 100;
		m_HSLColor.l = r;
		m_bDisableChange = true;
		{
			GetLEdit()->SetText( AStringVal(r) );
			m_Color = m_HSLColor.ToRGB();
			_updateRGB();
		}
		m_bDisableChange = false;
	}
}

void AColorDialogForm::OnBaseColorClick(AObject* pSender,AEvent* pEvent)
{
	AButton* pButton = dynamic_cast<AButton*>(pSender);
	if( pButton == NULL ) return;

	AColor cr = g_ColorEntrys[ (int)pButton->GetTag() ].cr;
	m_Color = cr;
	m_HSLColor.FromRGB(cr);
	m_bDisableChange = true;
	{
		_updateHSL();
		_updateRGB();
	}
	m_bDisableChange = false;
}

void AColorDialogForm::_updateRGB()
{
	AColor cr;
	cr = m_Color;
	GetRSlide()->SetPosition( cr.red );
	GetGSlide()->SetPosition( cr.green );
	GetBSlide()->SetPosition( cr.blue );
	GetREdit()->SetText( AStringVal(cr.red) );
	GetGEdit()->SetText( AStringVal(cr.green ) );
	GetBEdit()->SetText( AStringVal( cr.blue ) );
}

void AColorDialogForm::_updateHSL()
{
	_updateHSLSlide();
	GetColorSelector()->SetPosition(360 - m_HSLColor.h);
}

void AColorDialogForm::_updateHSLSlide()
{
	AColorHSL hsl;
	hsl = m_HSLColor;
	GetHEdit()->SetText( AStringVal( hsl.h ) );
	GetSEdit()->SetText( AStringVal( hsl.s ) );
	GetLEdit()->SetText( AStringVal( hsl.l ) );
	GetHSlide()->SetPosition( hsl.h );
	GetSSlide()->SetPosition( hsl.s );
	GetLSlide()->SetPosition( hsl.l );
}