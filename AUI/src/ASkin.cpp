#include "StdAfx.h"

ASkin::ASkin(AComponent* pOwner):AComponent(pOwner)
{
	Init();
}

ASkin::~ASkin()
{
	//
}

/*
 * 基准色：RGB(165,222,235)
 * 提醒色：RGB(67,103,139)
 */
void ASkin::Init()
{
	m_BackColor = AColor(255,249,251,251);//缺省的背景颜色
	m_DeckBackColor = AColor(255,173,214,204);//修饰背景色
	m_ForeColor = AColor(255,163,177,175);//缺省的前景色（如：小三角，最大化按钮等的颜色）
	m_OverColor = AColor(200,113,202,117);//鼠标经过时的颜色
	m_DownColor = AColor(255,67,120,152);//鼠标按下时的颜色
	m_BorderColor = AColor(255,39,106,114);//缺省的边框颜色
	m_CaptionColor = AColor(255,105,196,205);//标题背景色
	m_EmphasizeColor = AColor(255,67,103,139);//着重色（提醒色）
	m_ButtonColor = AColor(255,228,240,241);//按钮颜色
	m_SpliterColor = AColor(255,226,230,234);//分割条颜色
	m_DisableColor = AColor(255,157,166,172);//禁用颜色
	m_SelectedColor = AColor(210,183,214,236);//选中后的颜色

	m_ScrollBarWidth = 9;//滚动条的宽度（纵向）或高度（横向）

	m_DefaultFont.SetColor(AColor(255,0,0,0));
	m_DefaultFont.SetName(_T("新宋体"));
	m_DefaultFont.SetSize(12);
}


void ASkin::DrawLabel(ACanvas* cs,AControl* pControl)
{
	ALabel* pLabel = (ALabel*)( pControl );
	if( pLabel == NULL ) return;

	ARect r = pLabel->GetAbsPosition();
	

	StringAlignment ha;
	StringAlignmentV va;
	AUTIL::TextAlignToStringAlignmeng(pLabel->GetTextAlign(),ha,va);

	
	if( pControl->GetUseSkin() ) cs->Use(&m_DefaultFont);
	else cs->Use(&(pLabel->Font));

	cs->DrawText(r,pLabel->GetText(),ha,va);
}

void ASkin::DrawSlide(ACanvas* cs,AControl* pControl)
{
	ASlide* pSlide = (ASlide*)( pControl );
	if( pSlide == NULL ) return;

	
	ARect r = pSlide->GetAbsPosition();
	int nRadius = 11;
	if( r.GetWidth() < nRadius ) return;
	//if( r.GetHeight() < nRadius * 2 ) return;

	APen pen;
	pen.SetWidth(0.5);
	pen.SetColor( m_BorderColor );
	cs->Use(&pen);

	if( pSlide->GetKind() == sbVert )
		cs->DrawLine(APoint(r.left+r.GetWidth()/2,r.top),APoint(r.left+r.GetWidth()/2,r.bottom));
	else
		cs->DrawLine(APoint(r.left,r.top+r.GetHeight()/2),APoint(r.right,r.top+r.GetHeight()/2));
	
	ARect rButton = pSlide->GetSlideButtonPos();

	ARadialBrush br;
	br.SetStartColor( m_EmphasizeColor );
	br.SetEndColor( AColor(0,255,255,255) );
	br.SetCenter( APoint(rButton.left+rButton.GetWidth()/2,rButton.top+rButton.GetHeight()/2) );
	br.SetRadius( nRadius );
	cs->Use(&br)->FillEllipse(rButton);
}

void ASkin::DrawEdit(ACanvas* cs,AControl* pControl)
{
	AEdit* pEdit = (AEdit*)pControl;

	
	ARect r = pEdit->GetAbsPosition();
	

	ASolidBrush b;
	if( pEdit->GetEnabled() == false ) b.SetColor(m_DisableColor);
	else b.SetColor(WHITE_COLOR);

	cs->Use(&b)->FillRect(r);
	if( pEdit->GetBorderWidth() )
	{
		APen p(0.3,AColor(255,0,0,0),DashStyleSolid);
		cs->Use(&p)->DrawRect(r);
	}
	
	r.Deflate(pEdit->GetBorderWidth(),pEdit->GetBorderWidth());

	AAutoClip autoClip(cs,r);

	r.Deflate(pEdit->GetPadding(),pEdit->GetPadding());

	//显示选中文本的背景
	if( pEdit->GetSelCount() != 0 )
	{
		int iPosStart = pEdit->GetSelStartPos();
		int iPosEnd = pEdit->GetSelEndPos();
		if( iPosEnd < iPosStart )
		{
			int x = iPosStart;
			iPosStart = iPosEnd;
			iPosEnd = x;
		}
		ASolidBrush br(m_SelectedColor);
		cs->Use(&br)->FillRect(ARect(r.left+iPosStart,r.top,r.left+iPosEnd,r.bottom));
	}

	AFont* pUseFont=NULL;
	if( pControl->GetUseSkin() ) 
		pUseFont = &m_DefaultFont;
	else 
		pUseFont=&(pEdit->Font);
	int iWidth = r.GetWidth();
	r.left = r.left + pEdit->GetScrollPos();
	r.right = r.left + ( iWidth - pEdit->GetScrollPos());
	if( pEdit->GetMaskChar() != 0 )
	{
		AString s(pEdit->GetMaskChar(),pEdit->GetText().Length());
		cs->Use(pUseFont)->DrawText(r,s,StringAlignmentNear,StringAlignmentMiddle);
	}
	else
		cs->Use(pUseFont)->DrawText(r,pEdit->GetText(),StringAlignmentNear,StringAlignmentMiddle);

	//显示光标
	if( (AControl *)pEdit == pEdit->GetUICore()->GetFocusControl() && pEdit->ShowCaret() )
	{
		int iPos = pEdit->GetCaretPos();

		APen penCaret(1,m_EmphasizeColor,DashStyleSolid);
		cs->Use(&penCaret)->DrawLine(APoint(r.left+iPos,r.bottom-pUseFont->GetSize()-3),APoint(r.left+iPos,r.bottom));
	}

	
	
}

void ASkin::DrawScrollBar(ACanvas* cs,AControl* pControl)
{
	AScrollBar* pScrollBar = (AScrollBar*)(pControl);
	if( pScrollBar == NULL ) return;
	AControl* pParent = (AControl*)(pControl->GetParent());
	AControl* pOverControl = pControl->GetUICore()->GetMouseOverControl();
	AUICore* pCore = pControl->GetUICore();

	

	ARect r1 = pScrollBar->GetTopButton1Pos();
	ARect r2 = pScrollBar->GetTopButton2Pos();

	APen pen;
	pen.SetWidth(2);
	pen.SetColor(m_BorderColor);

	ASolidBrush br;
	
	br.SetColor(m_ButtonColor);
	cs->Use(&br)->FillRect(pControl->GetAbsPosition());
	br.SetColor(m_BorderColor);

	ARect rb1 = pScrollBar->GetButton1Pos();
	ARect rb2 = pScrollBar->GetButton2Pos();

	if( pScrollBar->GetKind() == sbVert )
	{
		//显示Top1,2
		//if( pScrollBar->GetShowTop() && ( pScrollBar->IsContainerScrollBar() && pCore->IsOverMe( pScrollBar ) || !pScrollBar->IsContainerScrollBar()) )
		{
			cs->Use(&pen)->DrawLine(APoint(r1.left,r1.top+1),APoint(r1.right,r1.top+1));
			cs->Use(&pen)->DrawLine(APoint(r1.left,r1.bottom),APoint(r1.left+r1.GetWidth()/2,r1.top+1));
			cs->Use(&pen)->DrawLine(APoint(r1.left+r1.GetWidth()/2,r1.top+1),APoint(r1.right,r1.bottom));

			cs->Use(&pen)->DrawLine(APoint(r2.left,r2.bottom-1),APoint(r2.right,r2.bottom-1));
			cs->Use(&pen)->DrawLine(APoint(r2.left,r2.top),APoint(r2.left+r2.GetWidth()/2,r2.bottom-1));
			cs->Use(&pen)->DrawLine(APoint(r2.left+r2.GetWidth()/2,r2.bottom-1),APoint(r2.right,r2.top));
		}
		//显示Button1,2
		//if(pScrollBar->GetShowButton() && ( pScrollBar->IsContainerScrollBar() && pCore->IsOverMe( pScrollBar ) || !pScrollBar->IsContainerScrollBar()) )
		{
			APoint pt1,pt2,pt3;
			pt1 = APoint(rb1.left+rb1.GetWidth()/2,rb1.top);
			pt2 = APoint(rb1.left,rb1.bottom);
			pt3 = APoint(rb1.right,rb1.bottom);
			cs->Use(&br)->FillTrigle(pt1,pt2,pt3);

			pt1 = APoint(rb2.left+rb2.GetWidth()/2,rb2.bottom);
			pt2 = APoint(rb2.left,rb2.top);
			pt3 = APoint(rb2.right,rb2.top);
			cs->Use(&br)->FillTrigle(pt1,pt2,pt3);
		}
		//显示轨道
		//if( pCore->IsOverMe( pScrollBar ) )
		{
			ARect rr = pScrollBar->GetRailPos();
			br.SetColor( m_ButtonColor );
			cs->Use(&br)->FillRoundRect(rr,rr.GetWidth()/2);
		}
		//显示滑块
		{
			ARect rs = pScrollBar->GetSlideButtonPos();
			if( pOverControl == pControl ) br.SetColor( m_EmphasizeColor );
			else br.SetColor( m_ForeColor );
			cs->Use(&br)->FillRoundRect(rs,rs.GetWidth()/2);
		}
	}
	else
	{
		//显示Top1,2
		//if( pScrollBar->GetShowTop() && ( pScrollBar->IsContainerScrollBar() && pCore->IsOverMe( pScrollBar ) || !pScrollBar->IsContainerScrollBar()) )
		{
			cs->Use(&pen)->DrawLine(APoint(r1.left+1,r1.top),APoint(r1.left+1,r1.bottom));
			cs->Use(&pen)->DrawLine(APoint(r1.left+1,r1.top+r1.GetHeight()/2),APoint(r1.right,r1.top));
			cs->Use(&pen)->DrawLine(APoint(r1.left+1,r1.top+r1.GetHeight()/2),APoint(r1.right,r1.bottom));

			cs->Use(&pen)->DrawLine(APoint(r2.right-1,r2.top),APoint(r2.right-1,r2.bottom));
			cs->Use(&pen)->DrawLine(APoint(r2.right-1,r2.top+r2.GetHeight()/2),APoint(r2.left,r2.top));
			cs->Use(&pen)->DrawLine(APoint(r2.right-1,r2.top+r2.GetHeight()/2),APoint(r2.left,r2.bottom));
		}
		//显示Button1,2
		//if( pScrollBar->GetShowButton() && ( pScrollBar->IsContainerScrollBar() && pCore->IsOverMe( pScrollBar ) || !pScrollBar->IsContainerScrollBar()) )
		{
			APoint pt1,pt2,pt3;
			pt1 = APoint(rb1.left,rb1.top+rb1.GetHeight()/2);
			pt2 = APoint(rb1.right,rb1.top);
			pt3 = APoint(rb1.right,rb1.bottom);
			cs->Use(&br)->FillTrigle(pt1,pt2,pt3);

			pt1 = APoint(rb2.right,rb2.top+rb2.GetHeight()/2);
			pt2 = APoint(rb2.left,rb2.top);
			pt3 = APoint(rb2.left,rb2.bottom);
			cs->Use(&br)->FillTrigle(pt1,pt2,pt3);
		}
		//显示轨道
		//if( pCore->IsOverMe( pScrollBar ) )
		{
			ARect rr = pScrollBar->GetRailPos();
			br.SetColor( m_ButtonColor );
			cs->Use(&br)->FillRoundRect(rr,rr.GetHeight()/2);
		}
		//显示滑块
		{
			ARect rs = pScrollBar->GetSlideButtonPos();
			if( pOverControl == pControl ) br.SetColor( m_EmphasizeColor );
			else br.SetColor( m_ForeColor );
			cs->Use(&br)->FillRoundRect(rs,rs.GetHeight()/2);
		}
	}

}

void ASkin::DrawScrollBarOverlap(ACanvas* cs,AControl* p,ARect r)
{
	ASolidBrush br;
	br.SetColor( m_BackColor );
	cs->Use(&br)->FillRect(r);
}

void ASkin::DrawButton(ACanvas* cs,AControl* pControl)
{
	AButton* pButton = (AButton*)pControl;

	ARect r = pButton->GetAbsPosition();
	

	
	AString sText;
	AAction* pAction = pButton->GetAction();
	if( pAction )
	{
		sText = pAction->GetText();
	}
	else
	{
		sText = pButton->GetText();
	}
	APen pen;
	pen.SetColor(m_BorderColor);
	pen.SetWidth(0.3);
	AUICore* pCore = pControl->GetUICore();
	ASolidBrush br;
	if( pControl == pCore->GetMouseDownControl() )
		br.SetColor( m_DownColor );
	else if( pCore->IsOverMe(pControl) )
		br.SetColor( m_OverColor);
	else
		br.SetColor(m_ButtonColor);
	if( !pButton->GetTransparent() )
	{
		cs->Use(&br)->FillRoundRect(r,1);
	}
	if( pCore->IsOverMe( pControl ) )
		cs->Use(&pen)->DrawRoundRect(r,1);
	
	

	if( pButton->GetUseSkin() ) cs->Use(&m_DefaultFont);
	else cs->Use(&(pButton->Font));

	AImage* pImage = pButton->GetImage();
	if( pImage && pImage->IsValid() )
	{
		int nImageWidth = pImage->GetWidth();
		int nImageHeight = pImage->GetHeight();
		if( sText != _T("") )
		{
			switch(pButton->GetImageAlign())
			{
			case alTop:
				{
					int delta = (r.GetWidth()-nImageWidth)/2;
					cs->DrawGraphic(pImage,r.left+delta,r.top);
					r.Deflate(0,nImageHeight,0,0);
					cs->DrawText(r,sText,StringAlignmentCenter,StringAlignmentMiddle);
				}
				break;
			case alRight:
				{
					int delta = (r.GetHeight()-nImageHeight)/2;
					cs->DrawGraphic(pImage,r.right-nImageWidth,r.top+delta);
					r.Deflate(0,0,nImageWidth,0);
					cs->DrawText(r,sText,StringAlignmentCenter,StringAlignmentMiddle);
				}
				break;
			case alBottom:
				{
					int delta = (r.GetWidth()-nImageWidth)/2;
					cs->DrawGraphic(pImage,r.left+delta,r.bottom-nImageHeight);
					r.Deflate(0,0,0,nImageHeight);
					cs->DrawText(r,sText,StringAlignmentCenter,StringAlignmentMiddle);
				}
				break;
			case alClient:
				break;
			case alLeft:
			default:
				{
					int delta = (r.GetHeight()-nImageHeight)/2;
					cs->DrawGraphic(pImage,r.left,r.top+delta);
					r.Deflate(nImageWidth,0,0,0);
					cs->DrawText(r,sText,StringAlignmentCenter,StringAlignmentMiddle);
				}
				break;
			}
		}
		else if( sText == _T("") )
		{
			if( pButton->GetImageAlign() == alClient )
				cs->DrawGraphic(pImage,r.left,r.top,r.GetWidth(),r.GetHeight());
			else
			{
				int dw = (r.GetWidth()-nImageWidth)/2;
				int dh = (r.GetHeight()-nImageHeight)/2;
				cs->DrawGraphic(pImage,r.left+dw,r.top+dh);
			}
		}
	}
	else
		cs->DrawText(r,sText,StringAlignmentCenter,StringAlignmentMiddle);
}

void ASkin::DrawImage(ACanvas* cs,AControl* pControl)
{
	AImageControl* pImage = (AImageControl*)(pControl);
	
	AImage* pGraphic = (AImage *)pImage->GetGraphic();

	ARect r = pImage->GetAbsPosition();
	
	if( pImage->GetDesignMode() && pGraphic == NULL )
	{
		ASolidBrush br;
		APen pen;
		br.SetColor(  WHITE_COLOR );
		pen.SetColor(  BLACK_COLOR );
		cs->Use(&br)->FillRect(r);
		cs->Use(&pen)->DrawRect(r);
	}

	if( pGraphic == NULL ) return;

	AAutoClip autoClip(cs,r);

	switch( pImage->GetDrawStyle() )
	{
	case idsNormal:
		{
			cs->DrawGraphic(pGraphic,r.left,r.top);
		}
		break;
	case idsStretch:
	default:
		cs->DrawGraphic(pGraphic,r.left,r.top,r.GetWidth(),r.GetHeight());
		break;
	}
}

//绘制CheckBox左边的勾选框
void ASkin::DrawCheckBoxImage(ACanvas* cs,AControl* pControl)
{
	AImageControl* pImage = (AImageControl *)pControl;
	

	ASolidBrush br;
	APen pen;

	ARect r = pImage->GetAbsPosition();
	

	AImage* pGraphic = (AImage *)pImage->GetGraphic();
	if( pGraphic ) cs->DrawGraphic(pGraphic,r.left,r.top);
	else
	{
		
		ARect rc(r.left,r.top+ (r.GetHeight()-r.GetWidth())/2,r.right,r.bottom-(r.GetHeight()-r.GetWidth())/2);
		pen.SetColor(  BORDER_COLOR );
		br.SetColor(  WHITE_COLOR );
		cs->Use(&br)->FillRect(rc);
		cs->Use(&pen)->DrawRect(rc);

		rc.Deflate(2,2,2,2);
		br.SetColor(  LIGHT_COLOR );
		pen.SetColor(  LIGHT_COLOR );
		cs->Use(&br)->FillRect(rc);
		cs->Use(&pen)->DrawRect(rc);

		ACheckBox* pBox = (ACheckBox *)(pImage->GetParent());
		if( pBox && pBox->GetChecked() == true )
		{
			rc.Deflate(1,1,1,1);

			int iWidth = rc.GetWidth()/4;
			if( iWidth < 1 ) iWidth = 1;
			if( iWidth > 5 ) iWidth = 5;

			APoint pt1(rc.left,rc.bottom - rc.GetHeight() /3 - iWidth );
			APoint pt2(rc.left+rc.GetWidth()/3,rc.bottom - iWidth );
			APoint pt3(rc.right,rc.top+rc.GetHeight()/3 - iWidth );


			pen.SetColor(  BLACK_COLOR );

			pen.SetWidth( 2 );

			cs->Use(&pen)->DrawLine(pt1.x,pt1.y,pt2.x,pt2.y);
			cs->Use(&pen)->DrawLine(pt2.x,pt2.y,pt3.x,pt3.y);
		}
	}
}

//绘制RadioButton左边的圆框
void ASkin::DrawRadioButtonImage(ACanvas* cs,AControl* pControl)
{
	AImageControl* pImage = (AImageControl *)pControl;
	

	ASolidBrush br;
	APen pen;

	ARect r = pImage->GetAbsPosition();
	

	AImage* pGraphic = (AImage *)pImage->GetGraphic();
	if( pGraphic ) cs->DrawGraphic(pGraphic,r.left,r.top);
	else
	{
		
		int y =  r.GetHeight() / 2;
		r.top = r.top + y - r.GetWidth() /2;
		r.bottom = r.top + r.GetWidth();

		int delta = r.GetWidth()/6;
		r.Deflate(delta,delta,delta,delta);

		pen.SetColor(  DARK_COLOR );
		br.SetColor(  WHITE_COLOR );
		cs->Use(&br)->FillEllipse(r);
		cs->Use(&pen)->DrawEllipse(r);

		r.Deflate(3,3,3,3);
		pen.SetColor(  LIGHT_COLOR );
		br.SetColor(  LIGHT_COLOR );
		cs->Use(&br)->FillEllipse(r);
		cs->Use(&pen)->DrawEllipse(r);

		ARadioButton* pButton = (ARadioButton *)(pImage->GetParent());
		if( pButton && pButton->GetChecked() == true )
		{
			delta = r.GetWidth()/2;
			int iWidth = 4;
			ARect rc(r.left+delta-iWidth/2,r.top+delta-iWidth/2,r.right-delta+iWidth/2,r.bottom-delta+iWidth/2);
			pen.SetColor(  BLACK_COLOR );
			br.SetColor(  BLACK_COLOR );
			cs->Use(&br)->FillEllipse(rc);
			cs->Use(&pen)->DrawEllipse(rc);
		}
	}
}

void ASkin::DrawForm(ACanvas* cs,AControl* pControl)
{
	
	AForm* pForm = (AForm*)(pControl);

	ARect r(0,0,pForm->GetWidth(),pForm->GetHeight());
	int bw = pForm->GetBorderWidth();
	//r.Deflate(bw,bw,bw,bw);
	ASolidBrush b;
	b.SetColor( m_BackColor );
	APen p;
	p.SetColor( m_BorderColor );
	p.SetWidth( (REAL)bw);

	cs->Use(&b)->FillRect(r);
 	cs->Use(&p)->DrawRect(r);

}

void ASkin::DrawPopup(ACanvas* cs,AControl* pControl)
{
	
	APopupContainer* pPopup = (APopupContainer *)(pControl);
	ARect r(0,0,pPopup->GetWidth(),pPopup->GetHeight());
	ASolidBrush b;
	b.SetColor(WHITE_COLOR);

	cs->Use(&b)->FillRect(r);
}


void ASkin::DrawListBox(ACanvas* cs,AControl* pControl)
{
	
	AListBox* pBox = (AListBox*)(pControl);
	ARect r = pBox->GetAbsPosition();
	
	ASolidBrush b;
	b.SetColor(WHITE_COLOR);
	APen p;
	p.SetColor(BORDER_COLOR);
	cs->Use(&b)->FillRect(r);
	cs->Use(&p)->DrawRect(r);
}

void ASkin::DrawSidePanelTitle(ACanvas* cs,AControl* pControl)
{
	
	AContainer* pTitle = (AContainer*)(pControl);
	ARect r = pTitle->GetAbsPosition();
	
	ASolidBrush br;
	br.SetColor(  m_CaptionColor );
	//pen.SetColor(  BORDER_COLOR;
	cs->Use(&br)->FillRect(r);

	ASidePanel* ps = (ASidePanel *)(pTitle->GetParent());
	if( ps == NULL ) return;

	if( pTitle->GetUseSkin() ) cs->Use(&m_DefaultFont);
	else cs->Use(&(pTitle->Font));

	cs->DrawText(r,ps->GetText());
}

void ASkin::DrawSidePanelArrow(ACanvas* cs,AControl* pControl)
{
	
	AButton* pArrow = (AButton*)(pControl);
	if( pArrow == NULL ) return;

	AContainer* pc = (AContainer*)(pArrow->GetParent());
	if( pc == NULL ) return;

	ASidePanel* ps = (ASidePanel *)(pc->GetParent());
	if( ps == NULL ) return;

	ARect r = pArrow->GetAbsPosition();
	

	ASolidBrush br;
	APen pen;

	if( (AControl *)pArrow == (AControl *)pArrow->GetUICore()->GetMouseOverControl() )
	{
		br.SetColor(  LIGHT_COLOR );
		pen.SetColor(  DARK_COLOR );
		cs->Use(&br)->FillRect(r);
		cs->Use(&pen)->DrawRect(r);
	}
	APoint pt1,pt2,pt3;
	br.SetColor(AColor(255,20,39,67));
	if( ps->GetAlign() == alLeft )
	{
		pt1 = APoint(r.right-12,r.top + r.GetHeight() / 2 );
		pt2 = APoint(pt1.x+6,pt1.y - 4);
		pt3 = APoint(pt1.x+6,pt1.y + 4);
		cs->Use(&br)->FillTrigle(pt1,pt2,pt3);
	}
	else if( ps->GetAlign() == alRight )
	{
		pt1 = APoint(r.left+8,r.top + r.GetHeight() / 2 );
		pt2 = APoint(pt1.x-6,pt1.y - 4);
		pt3 = APoint(pt1.x-6,pt1.y + 4);
		cs->Use(&br)->FillTrigle(pt1,pt2,pt3);
	}
	else if( ps->GetAlign() == alTop || ps->GetAlign() == alBottom )
	{
		//
	}
// 	if( pScrollBar->Kind == sbVert ) r1.Deflate(1,0);
// 	else r1.Deflate(0,1);
// 	cs->DrawRect(r1);
// 	br.SetColor(  SB_ARROW_DARK;
// 	pen.SetColor(  DARK_COLOR );
// 	if( pScrollBar->Kind == sbVert )
// 	{
// 		pt1 = APoint(r1.left+r1.GetWidth()/2,r1.top + r1.GetHeight() / 3 );
// 		pt2 = APoint(pt1.x-r1.GetHeight()/3,pt1.y + r1.GetHeight()/3);
// 		pt3 = APoint(pt1.x+r1.GetHeight()/3,pt2.y);
// 		cs->DrawTriple(pt1,pt2,pt3);
// 	}
// 	else
// 	{
// 		pt1 = APoint(r1.left+r1.GetWidth()/3,r1.top+r1.GetHeight()/2);
// 		pt2 = APoint(pt1.x+r1.GetWidth()/3,pt1.y-r1.GetHeight()/3);
// 		pt3 = APoint(pt2.x,pt1.y+r1.GetHeight()/3);
// 		cs->DrawTriple(pt1,pt2,pt3);
// 	}
// 
// 	if( pScrollBar->Kind == sbVert )
// 	{
// 		pt1 = APoint(r2.left+r2.GetWidth()/2,r2.bottom - r2.GetHeight()/3);
// 		pt2 = APoint(pt1.x-r2.GetWidth()/3,pt1.y-r2.GetHeight()/3);
// 		pt3 = APoint(pt1.x+r2.GetWidth()/3,pt2.y);
// 		cs->DrawTriple(pt1,pt2,pt3);
// 	}
// 	else
// 	{
// 		pt1 = APoint(r2.right-r2.GetWidth()/3,r2.top+r2.GetHeight()/2);
// 		pt2 = APoint(pt1.x-r2.GetWidth()/3,pt1.y-r2.GetHeight()/3);
// 		pt3 = APoint(pt2.x,pt1.y+r2.GetHeight()/3);
// 		cs->DrawTriple(pt1,pt2,pt3);
// 	}
}

void ASkin::DrawPanel(ACanvas* cs,AControl* pControl)
{
	
	APanel* pPanel = (APanel*)(pControl);
	ARect r = pPanel->GetAbsPosition();
	

	ASolidBrush b;
	b.SetColor(pPanel->GetColor());
	APen p;
	p.SetWidth(0.3);
	p.SetColor(pPanel->BorderColor);
	cs->Use(&b)->FillRect(r);
	if( pPanel->GetBorderWidth() > 0 )
		cs->Use(&p)->DrawRect(r);
}

void ASkin::DrawMemo(ACanvas* cs,AControl* pControl)
{
	
	AMemo* pMemo = (AMemo*)(pControl);
	ARect r = pMemo->GetAbsPosition();
	

	ASolidBrush b;
	b.SetColor(pMemo->GetColor());
	APen p;
	p.SetWidth(0.3);
	p.SetColor(pMemo->BorderColor);
	cs->Use(&b)->FillRect(r);
	if( pMemo->GetBorderWidth() > 0 )
		cs->Use(&p)->DrawRect(r);
}

void ASkin::DrawColorDialog(ACanvas* cs,AControl* pControl)
{
	
	AColorDialog* pDlg = (AColorDialog*)(pControl);
	ARect r = pDlg->GetAbsPosition();
	
	ASolidBrush b;
	b.SetColor(WHITE_COLOR);
	APen p;
	p.SetColor(BLACK_COLOR);
	cs->Use(&b)->FillRect(r);
	cs->Use(&p)->DrawRect(r);
	AFont f;
	cs->Use(&f)->DrawText(r,_T("颜色选择"));
}

void ASkin::DrawFileOpenDialog(ACanvas* cs,AControl* pControl)
{
	
	AFileOpenDialog* pDlg = (AFileOpenDialog*)(pControl);
	ARect r = pDlg->GetAbsPosition();
	
	ASolidBrush b;
	b.SetColor(WHITE_COLOR);
	APen p;
	p.SetColor(BLACK_COLOR);
	cs->Use(&b)->FillRect(r);
	cs->Use(&p)->DrawRect(r);
	AFont f;
	cs->Use(&f)->DrawText(r,_T("文件打开"));
}
void ASkin::DrawFileSaveDialog(ACanvas* cs,AControl* pControl)
{
	
	AFileSaveDialog* pDlg = (AFileSaveDialog*)(pControl);
	ARect r = pDlg->GetAbsPosition();
	
	ASolidBrush b;
	b.SetColor(WHITE_COLOR);
	APen p;
	p.SetColor(BLACK_COLOR);
	cs->Use(&b)->FillRect(r);
	cs->Use(&p)->DrawRect(r);
	AFont f;
	cs->Use(&f)->DrawText(r,_T("文件保存"));
}
void ASkin::DrawFontDialog(ACanvas* cs,AControl* pControl)
{
	
	AFontDialog* pDlg = (AFontDialog*)(pControl);
	ARect r = pDlg->GetAbsPosition();
	
	ASolidBrush b;
	b.SetColor(WHITE_COLOR);
	APen p;
	p.SetColor(BLACK_COLOR);
	cs->Use(&b)->FillRect(r);
	cs->Use(&p)->DrawRect(r);
	AFont f;
	cs->Use(&f)->DrawText(r,_T("字体选择"));
}
void ASkin::DrawImageList(ACanvas* cs,AControl* pControl)
{
	
	AImageList* pDlg = (AImageList*)(pControl);
	ARect r = pDlg->GetAbsPosition();
	
	ASolidBrush b;
	b.SetColor(WHITE_COLOR);
	APen p;
	p.SetColor(BLACK_COLOR);
	cs->Use(&b)->FillRect(r);
	cs->Use(&p)->DrawRect(r);
	AFont f;
	cs->Use(&f)->DrawText(r,_T("图像列表"));
}
void ASkin::DrawPropEdit(ACanvas* cs,AControl* pControl)
{
	
	APropEdit* pPanel = (APropEdit*)(pControl);
	ARect r = pPanel->GetAbsPosition();
	
	ASolidBrush b;
	b.SetColor(pPanel->GetColor());
	APen p;
	p.SetColor(pPanel->BorderColor);
	cs->Use(&b)->FillRect(r);
	cs->Use(&p)->DrawRect(r);

}

void ASkin::DrawMenu(ACanvas* cs,AControl* pControl)
{
	
	AMenu* pMenu = (AMenu*)(pControl);
	ARect r = pMenu->GetAbsPosition();
	
	ASolidBrush b;
	b.SetColor(m_BackColor );
	APen p;
	p.SetColor( m_BorderColor );
	//r.Deflate(0,0,1,1);
	cs->Use(&b)->FillRect(r);
	cs->Use(&p)->DrawRect(r);
	int iBorder = pMenu->GetBorderWidth();
	r.Deflate(iBorder,iBorder,iBorder,iBorder);
	r.right = r.left + 28;
// 	ALinearGradientBrush lgb;
// 	lgb.SetStartColor( m_DeckBackColor );
// 	lgb.SetEndColor( m_BackColor );
// 	ARect rx;
// 	rx = ARect(r.left,r.top,r.right-1,r.bottom-1);
// 	lgb.SetRect(r);
// 	lgb.IsVert(true);
	ASolidBrush lgb(m_DeckBackColor);
	cs->Use(&lgb)->FillRect(r);
}

void ASkin::DrawListBoxItem(ACanvas* cs,AControl* pControl)
{
	
	AListBoxItem* pItem = (AListBoxItem *)(pControl);
	if( pItem )
	{
		ASolidBrush br;
		ARect r = pItem->GetAbsPosition();
		
		if( pItem->GetSelected() == true )
		{
			br.SetColor(  m_SelectedColor );
			cs->Use(&br)->FillRect(r);
		}
		else if( (AControl *)pItem == pItem->GetUICore()->GetMouseOverControl() )
		{

			br.SetColor(  m_OverColor );
			cs->Use(&br)->FillRect(r);

		}
		DrawLabel(cs,pControl);
	}
}

void ASkin::DrawDateControl(ACanvas* cs,AControl* pControl)
{
	ADateControl* pBox = (ADateControl *)(pControl);
	ARect r = pBox->GetAbsPosition();

	ASolidBrush br;
	APen pen;
	br.SetColor(  WHITE_COLOR );
	pen.SetColor(  BORDER_COLOR );
	cs->Use(&br)->FillRect(r);
	cs->Use(&pen)->DrawRect(r);

	AString str;
	ADateTime dt = pBox->GetDate();
	str.Format(_T("%04d-%02d-%02d"),dt.GetYear(),dt.GetMonth(),dt.GetDay());
	if( pControl->GetUseSkin() ) cs->Use(&m_DefaultFont);
	else cs->Use(&(pBox->Font));

	cs->DrawText(r,str,StringAlignmentCenter,StringAlignmentMiddle);
}

void ASkin::DrawComboBox(ACanvas* cs,AControl* pControl)
{
	
	AComboBox* pBox = (AComboBox *)(pControl);
	ARect r = pBox->GetAbsPosition();
	
	ASolidBrush br;
	APen pen;
	br.SetColor(  WHITE_COLOR );
	pen.SetColor(  BORDER_COLOR );
	cs->Use(&br)->FillRect(r);
	cs->Use(&pen)->DrawRect(r);

	if( pBox->GetCanEdit() == false )
	{
		AString str = pBox->GetText();
		if( pControl->GetUseSkin() ) cs->Use(&m_DefaultFont);
		else cs->Use(&(pBox->Font));

		cs->DrawText(r,str,StringAlignmentNear,StringAlignmentMiddle);
	}
}

void ASkin::DrawComboButton(ACanvas* cs,AControl* pControl)
{
	AButton* pButton = (AButton*)pControl;

	ARect r = pButton->GetAbsPosition();
	

	

	ASolidBrush br;
	APen pen;
	pen.SetColor(  BORDER_COLOR );
	br.SetColor(  GRAY_COLOR );
	cs->Use(&br)->FillRect(r);

	r.Deflate(1,0);

	br.SetColor(  SB_ARROW_DARK );
	pen.SetColor(  DARK_COLOR );

	APoint pts[3];
	pts[0] = APointF(r.left+r.GetWidth()/2,r.bottom - r.GetHeight()/3);
	pts[1] = APointF(pts[0].x-r.GetWidth()/3,pts[0].y-r.GetHeight()/3);
	pts[2] = APointF(pts[0].x+r.GetWidth()/3,pts[1].y);
	
	cs->Use(&br)->FillTrigle(pts[0],pts[1],pts[2]);
	//cs->Use(&pen)->DrawTrigle(pts[0],pts[1],pts[2]);

}

void ASkin::DrawComboEdit(ACanvas* cs,AControl* pControl)
{
	AEdit* pEdit = (AEdit*)pControl;

	
	ARect r = pEdit->GetAbsPosition();
	

	AComboBox* pBox = (AComboBox *)(pEdit->GetParent());


	if( pBox )
	{
		AFont font(pBox->Font);

		r.Deflate(1,2);
		cs->Use(&font)->DrawText(r,pBox->GetText());
	}
}

void ASkin::DrawListHeaderItem(ACanvas* cs,AControl* pControl)
{
	
	AListHeaderItem* pItem = (AListHeaderItem *)(pControl);
	ARect r = pItem->GetAbsPosition();
	
	APen pen;
	pen.SetColor(  AColor(255,223,234,247) );
	cs->Use(&pen)->DrawLine(r.right,r.top,r.right,r.bottom);

	AFont font(pItem->Font);


	cs->Use(&font)->DrawText(r,pItem->GetText());
}

void ASkin::DrawListHeader(ACanvas* cs,AControl* pControl)
{
	
	AListHeader* pHeader = (AListHeader *)(pControl);
	ARect r = pHeader->GetAbsPosition();
	
	ASolidBrush br;
	APen pen;
	br.SetColor(  LIGHT_COLOR );
	cs->Use(&br)->FillRect(r);
	pen.SetColor(  AColor(255,223,234,247) );
	cs->Use(&pen)->DrawLine(r.left,r.top,r.right,r.top);
	cs->Use(&pen)->DrawLine(r.left,r.bottom,r.right,r.bottom);
}


void ASkin::DrawListViewItem(ACanvas* cs,AControl* pControl)
{
	
	AListViewItem* pItem = (AListViewItem* )(pControl);
	ARect r = pItem->GetAbsPosition();
	
	if( pItem->GetListView() == NULL ) return;
	ASolidBrush br;
	APen pen;
	switch((TListViewStyle)pItem->GetListView()->GetViewStyle())
	{
	case lvsReport:
		{
			if( pItem->GetSelected() == true )
			{
				br.SetColor(  m_SelectedColor );
				cs->Use(&br)->FillRect(r);
			}
			else if( (AControl *)pItem == pItem->GetUICore()->GetMouseOverControl() )
			{
				br.SetColor(  m_OverColor );
				cs->Use(&br)->FillRect(r);
			}

			AListView* pListView = pItem->GetListView();
			if( pListView == NULL ) return;
			AListHeader* pHeader = pListView->GetHeader();
			if( pListView->GetViewStyle() == lvsReport )
			{
				for(int i=0;i<pHeader->GetListHeaderItemCount()&& i < pItem->SubItems.GetCount()+1;i++)
				{
					AListHeaderItem* pHeaderItem = pHeader->GetListHeaderItem(i);
					ARect rHeaderItem = pHeaderItem->GetAbsPosition();
					ARect rx;
					rx.left = rHeaderItem.left+(i==0?10:0);
					rx.top = r.top;
					rx.right = rx.left + rHeaderItem.GetWidth();
					rx.bottom = rx.top + rHeaderItem.GetHeight();
					AString s;
					if( i == 0 ) 
					{
						//rx.left += 10;
						s = pItem->GetText();
					}
					else s = pItem->SubItems.GetItem(i-1);

					AAutoClip autoClip(cs,rx);
					if( i == 0 )
					{
						ARect rImage = rx;
						rImage.right = rImage.left+16;
						rImage.bottom = rImage.top+16;
						rx.left += 16+6;
						if( pItem->GetImageIndex() > -1 && pListView->GetSmallImageList() != NULL )
						{
							AImage* pImage = pListView->GetSmallImageList()->GetItem(pItem->GetImageIndex());
							if( pImage )
								cs->DrawGraphic(pImage,rImage.left,rImage.top);
						}
					}
					AFont f(pItem->Font);
					cs->Use(&f)->DrawText(rx,s);
				}
			}
		}
		break;
	case lvsList:
		break;
	case lvsIcon:
		{
			if( pItem->GetSelected() == true )
			{
				br.SetColor(  m_SelectedColor );
				cs->Use(&br)->FillRect(r);
			}
			else if( (AControl *)pItem == pItem->GetUICore()->GetMouseOverControl() )
			{
				br.SetColor(  m_OverColor );
				cs->Use(&br)->FillRect(r);
			}

			AImageList* pLargeImageList = pItem->GetListView()->GetLargeImageList();
			if( pLargeImageList != NULL )
			{
				ASize sz = pLargeImageList->Size;
				ARect rx(r.left,r.top,r.left+sz.cx,r.top+sz.cy);
				AImage* g = NULL;
				if( (int)pItem->GetImageIndex() >= 0 && (int)pItem->GetImageIndex() < pLargeImageList->GetCount() )
					g = pLargeImageList->GetItem(pItem->GetImageIndex());
				if( g ) cs->DrawGraphic(g,rx.left,rx.top,rx.GetWidth(),rx.GetHeight());// g->Draw(cs,rx);//,AColor(0,255,255,255));
				ARect r2(r.left,rx.bottom,rx.right,rx.bottom+pItem->GetListView()->GetItemHeight());
				AFont f(pItem->Font);

				cs->Use(&f)->DrawText(r2,pItem->GetText());
			}
			else
			{
				AFont f(pItem->Font);

				cs->Use(&f)->DrawText(r,pItem->GetText());
			}
		}
		break;
	default:
		break;
	}

}

void ASkin::DrawListView(ACanvas* cs,AControl* pControl)
{
	
	AListView* pView = (AListView* )(pControl);
	ARect r = pView->GetAbsPosition();
	
	ASolidBrush br;
	APen pen;
	pen.SetColor(  m_BorderColor );
	br.SetColor(  m_BackColor );
	pen.SetWidth(0.3);
	cs->Use(&br)->FillRect(r);
	cs->Use(&pen)->DrawRect(r);
}

void ASkin::DrawTreeViewItem(ACanvas* cs,AControl* pControl)
{
	
	ATreeViewItem* pItem = (ATreeViewItem* )(pControl);
	ARect r = pItem->GetAbsPosition();
	
	bool bHovered = false;

	if( pItem->GetUICore()->IsOverMe(pItem) || pItem->GetSelected() == true )
	{
		ARect rx = pItem->GetTextControl()->GetAbsPosition();
		APen pen;
		ASolidBrush br;
		pen.SetColor(  m_BorderColor );
		if( pItem->GetSelected() == true )
		{
			br.SetColor(  m_SelectedColor );
		}
		else
		{
			br.SetColor(  m_OverColor );
		}
		pen.SetWidth(0.3);
		cs->Use(&br)->FillRoundRect(rx,1);
		cs->Use(&pen)->DrawRoundRect(rx,1);
	}
	//
}

void ASkin::DrawTreeView(ACanvas* cs,AControl* pControl)
{
	
	ATreeView* pView = (ATreeView *)(pControl);
	ARect r = pView->GetAbsPosition();
	
	ASolidBrush br;
	APen pen;
	pen.SetColor(  DARK_COLOR );
	br.SetColor(  LIGHT_COLOR );
	cs->Use(&br)->FillRect(r);
	cs->Use(&pen)->DrawRect(r);
}

void ASkin::DrawTreeViewItemArrow(ACanvas* cs,AControl* pControl)
{
	
	AImageControl* pImage = (AImageControl *)(pControl);
	ATreeViewItem* pItem = (ATreeViewItem*)(pControl->GetParent());

	ASolidBrush br;
	APen pen;

	if( pItem->GetChildItemCount() > 0 )
	{
		ARect r = pImage->GetAbsPosition();
		//r.Deflate(1,1,1,1);
		r.Offset(r.GetWidth()/3,r.GetHeight()/3);
		
#define TREEITEMARROWWIDTH	8
		if( r.GetHeight() > TREEITEMARROWWIDTH ) r.bottom = r.top + TREEITEMARROWWIDTH;
		if( r.GetWidth() > TREEITEMARROWWIDTH ) r.right = r.left + TREEITEMARROWWIDTH;

		AImage* pImage = NULL;
		if( (bool)pItem->GetExpanded() == false )
// 			pImage = AImage::GetImageByKey(_T("FolderCollapse"));
// 		else
// 			pImage = AImage::GetImageByKey(_T("FolderExpand"));
// 		cs->DrawGraphic(pImage,r.left,r.top);
		{
			pen.SetColor(  m_BorderColor );
			br.SetColor(  m_BorderColor );//WHITE_COLOR );

			APoint pts[3];
			pts[0] = APoint(r.left,r.top);
			pts[2] = APoint(r.left,r.bottom-1);
			pts[1] = APoint(r.left+r.GetWidth()/2+1,r.top+r.GetHeight()/2);

			cs->Use(&br)->FillTrigle(pts[0],pts[1],pts[2]);
			//cs->Use(&pen)->DrawTrigle(pts[0],pts[1],pts[2]);
			
			//cs->DrawText(r,L"+");
		}
		else
		{
			pen.SetColor(  m_BorderColor );
			br.SetColor(  m_EmphasizeColor );
			APoint pts[3];
			pts[0] = APoint(r.left,r.top);
			pts[1] = APoint(r.right,r.top);
			pts[2] = APoint(r.left+r.GetWidth()/2,r.top+r.GetHeight()/2+1);
			
			cs->Use(&br)->FillTrigle(pts[0],pts[1],pts[2]);
			//cs->Use(&pen)->DrawTrigle(pts[0],pts[1],pts[2]);
			//cs->DrawText(r,L"-");
		}
	}
}

void ASkin::DrawPageControl(ACanvas* cs,AControl* pControl)
{
	
	APageControl* pPage = (APageControl *)(pControl);
	ARect r = pPage->GetAbsPosition();
	
	//r.Deflate(1,1,1,1);


 	APageSheet* pActiveSheet = pPage->GetActiveSheet();
	if( pActiveSheet )
	{
		AControl* pButton = (AControl*)( pActiveSheet->GetTabButton());
		ARect rb = pButton->GetAbsPosition();
		APen pen;
		pen.SetColor(m_EmphasizeColor);
		pen.SetWidth(4);
		rb.Deflate(3,0);
		cs->Use(&pen)->DrawLine(APoint(rb.left,rb.bottom-3),APoint(rb.right,rb.bottom-3));
	}
}

void ASkin::DrawMenuItem(ACanvas* cs,AControl* pControl)
{
	AMenuItem* pMenuItem = (AMenuItem*)(pControl);
	if( pMenuItem == NULL ) return;

	bool bHover = ( pMenuItem->GetUICore()->GetMouseOverControl() == (AControl *)pMenuItem ? true : false );

	AImageList* pImageList = pMenuItem->GetImageList();
	AImage* pImage = NULL;
	if( pImageList != NULL && pMenuItem->GetImageIndex() > -1 && pMenuItem->GetImageIndex() < pImageList->GetCount() )
		pImage = pImageList->GetItem( pMenuItem->GetImageIndex() );

	ARect r = pMenuItem->GetAbsPosition();
	
	
	ASolidBrush br;
	APen pen;

	if( bHover )
	{
		ARect rHover = r;
		//rHover.Deflate(1,1,1,1);
		br.SetColor(  m_OverColor );
		cs->Use(&br)->FillRect(rHover);
	}

	//r.Deflate(1,1,1,1);
	AString sText;
	bool bChecked;
	AAction* pAction = pMenuItem->GetAction();
	if( pAction )
	{
		sText = pAction->GetText();
		if( sText == _T("") ) sText = pMenuItem->GetText();
		AUpdateEvent evt;
		pAction->OnUpdate.Call(pMenuItem,&evt);
		bChecked = evt.Checked;
	}
	else
	{
		sText = pMenuItem->GetText();
		bChecked = pMenuItem->GetChecked();
	}


	if( pImage )
	{
		ASize sz( pImage->GetWidth(),pImage->GetHeight() );
		int x,y;
		x = r.left;
		y = (r.GetHeight() - sz.cy ) / 2;
		cs->DrawGraphic(pImage,x,y,sz.cx,sz.cy);
	}
	else
	{
		if( bChecked )
		{
			ARect rc(r.left+2,r.top,r.left+2+11,r.top+11);

			int iWidth = rc.GetWidth()/4;
			if( iWidth < 2 ) iWidth = 2;
			if( iWidth > 4 ) iWidth = 4;
			
			APoint pt1(rc.left,rc.bottom - rc.GetHeight() /3 - iWidth );
			APoint pt2(rc.left+rc.GetWidth()/3,rc.bottom - iWidth );
			APoint pt3(rc.right,rc.top+rc.GetHeight()/3 - iWidth );
			
			
			pen.SetColor(  m_EmphasizeColor );
			
			pen.SetWidth( (REAL)iWidth );
			
			cs->Use(&pen)->DrawLine(pt1.x,pt1.y,pt2.x,pt2.y);
			cs->Use(&pen)->DrawLine(pt2.x,pt2.y,pt3.x,pt3.y);
			
			pen.SetWidth( 1 );
		}
	}
	
	r.left += 28;

	AFont f(pMenuItem->Font);

	cs->SetTextHint(thFull);
	if( sText == _T("-") )
	{
		APen pen;
		pen.SetWidth(0.2);
		pen.SetColor(m_EmphasizeColor);
		cs->SetSmoothingMode(SmoothingModeAntiAlias);
		cs->Use(&pen)->DrawLine(APoint(r.left,r.top+r.GetHeight()/2),APoint(r.right,r.top+r.GetHeight()/2));
		cs->SetSmoothingMode(SmoothingModeNone);
	}
	else
		cs->Use(&f)->DrawText(r,sText);
	cs->SetTextHint(thNone);

	if( pMenuItem->GetPopupMenu() != NULL )
	{
		ARect r2 = r;
		r2.left = r2.right - 12;
		r2.Deflate(1,2,0,2);

		APoint pts[3];
		pts[0] = APoint(r2.right-r2.GetWidth()/3,r2.top+r2.GetHeight()/2);
		pts[1] = APoint(pts[0].x-r2.GetWidth()/3,pts[0].y-r2.GetHeight()/3);
		pts[2] = APoint(pts[1].x,pts[0].y+r2.GetHeight()/3);
		//cs->Use(&br)->DrawTrigle(pts[0],pts[1],pts[2]);
		br.SetColor( m_EmphasizeColor );
		cs->SetSmoothingMode(SmoothingModeAntiAlias);
		cs->Use(&br)->FillTrigle(pts[0],pts[1],pts[2]);
		cs->SetSmoothingMode(SmoothingModeNone);
	}
}

void ASkin::DrawTitleBar(ACanvas* cs,AControl* pControl)
{
	ATitleBar* pBar = (ATitleBar*)(pControl);
	if( pBar == NULL ) return;
	
	ARect r = pBar->GetAbsPosition();
	

	ALinearGradientBrush br;
	br.SetRect(ARectF(0,0,(REAL)r.GetWidth()+1,(REAL)r.GetHeight()+1));
	br.IsVert(true);
	br.SetStartColor(m_CaptionColor);
	br.SetEndColor(m_BackColor);
	cs->Use(&br)->FillRect(r);
// 	ASolidBrush br(m_CaptionColor);
// 	cs->Use(&br)->FillRect(r);
	

	AForm* pParent=NULL;
	AControl* pf = (AControl *)(pControl->GetParent());
	while( pf )
	{
		pParent = (AForm*)(pf);
		if( pParent ) break;
		pf = (AControl *)( pf->GetParent() );
	}
	AForm* pForm = (AForm*)(pParent);
	if( pForm )
	{
		AString sText = pForm->GetText();
		//cs->GetFont()->FromControlFont(&(pLabel->Font));
		AFont font(_T("新宋体"),13,AColor(255,13,16,68));
		//font.SetStyle(FontStyleBold);

		r.Deflate(pBar->GetBorderWidth(),pBar->GetBorderWidth());
		ARect rf = pf->GetAbsPosition();
		int x = 10;
		int y = 4;//r.GetHeight()-font.GetSize())/2;
		cs->Use(&font)->DrawText(r.left+x,r.top+y,sText);
	}
}

void ASkin::DrawCloseButton(ACanvas* cs,AControl* pControl)
{
	ACloseButton* pButton = (ACloseButton*)(pControl);
	if( pButton == NULL ) return;
	
	ARect r = pButton->GetAbsPosition();
	
	r.Deflate(1,1);

	if( pButton->GetUICore()->GetMouseDownControl() == pControl )
	{
		ASolidBrush br(m_DownColor);
		cs->Use(&br)->FillRect(r);
	}
	else if( pButton->GetUICore()->GetMouseOverControl() == pControl )
	{
		ASolidBrush br(m_OverColor);
		cs->Use(&br)->FillRect(r);
	}
	
	APen pen;
	pen.SetWidth(3);
	pen.SetColor(m_EmphasizeColor);


	r.Deflate(7,7);

	AutoPath pPath(cs);
	pPath->AddLine(APointF((REAL)r.left,(REAL)r.top),APointF((REAL)r.right,(REAL)r.bottom));
	cs->Use(&pen)->DrawPath(&pPath);

	AutoPath pPath2(cs);
	pPath2->AddLine(APointF((REAL)r.right,(REAL)r.top),APointF((REAL)r.left,(REAL)r.bottom));
	cs->Use(&pen)->DrawPath(&pPath2);
}
void ASkin::DrawMinimizeButton(ACanvas* cs,AControl* pControl)
{
	AMinimizeButton* pButton = (AMinimizeButton*)(pControl);
	if( pButton == NULL ) return;
	
	ARect r = pButton->GetAbsPosition();
	
	r.Deflate(1,1);

	if( pButton->GetUICore()->GetMouseDownControl() == pControl )
	{
		ASolidBrush br(m_DownColor);
		cs->Use(&br)->FillRect(r);
	}
	else if( pButton->GetUICore()->GetMouseOverControl() == pControl )
	{
		ASolidBrush br(m_OverColor);
		cs->Use(&br)->FillRect(r);
	}

	r.Deflate(7,7);
	int iHeight = r.bottom-7;

	APen pen;
	pen.SetWidth(3);
	pen.SetColor(m_EmphasizeColor);
	cs->Use(&pen)->DrawLine(r.left,r.bottom,r.right,r.bottom);
}
void ASkin::DrawMaximizeButton(ACanvas* cs,AControl* pControl)
{
	AMaximizeButton* pButton = (AMaximizeButton*)(pControl);
	if( pButton == NULL ) return;
	
	ARect r = pButton->GetAbsPosition();
	
	r.Deflate(1,1);

	if( pButton->GetUICore()->GetMouseDownControl() == pControl )
	{
		ASolidBrush br(m_DownColor);
		cs->Use(&br)->FillRect(r);
	}
	else if( pButton->GetUICore()->GetMouseOverControl() == pControl )
	{
		ASolidBrush br(m_OverColor);
		cs->Use(&br)->FillRect(r);
	}
	
	APen pen;
	
	pen.SetColor(m_EmphasizeColor);

	if( TRUE == ::IsZoomed(pButton->GetUICore()->GetWin()->GetHandle()) )
	{
		r.Deflate(7,8);

		ARect r2=r;
		r2.Offset(5,-3);
	
	
		pen.SetWidth(2);

		cs->Use(&pen)->DrawRect(r2);
		
		cs->Use(&pen)->DrawRect(r);
	}
	else
	{
		r.Deflate(6,7);
		pen.SetWidth(3);
		cs->Use(&pen)->DrawRect(r);
	}
}

void ASkin::DrawMenuBar(ACanvas* cs,AControl* pControl)
{
// 	AMenuBar* pBar = (AMenuBar)(pControl);
// 	if( pBar == NULL ) return;
// 
// 	ARect r = pBar->GetAbsPosition();
// 	ACanvas* cs = pBar->GetUICore()->GetCanvas();
// 
// 	ASolidBrush br;
// 	br.SetColor(AColor(255,124,163,184));
// 	cs->Use(&br)->FillRect(r);

}

void ASkin::DrawMenuBarLabel(ACanvas* cs,AControl* pControl)
{
	ALabel* pLabel = (ALabel*)(pControl);
	if( pLabel == NULL ) return;

	ARect r = pLabel->GetAbsPosition();


	if( (AControl*)pLabel == pLabel->GetUICore()->GetMouseOverControl() )
	{
		ASolidBrush br;
		br.SetColor(m_OverColor);
		cs->Use(&br)->FillRect(r);

		APen pen;
		pen.SetWidth(1);
		pen.SetColor( m_BorderColor );
		cs->Use(&pen)->DrawRect(r);
	}

	if( pLabel->GetUseSkin() ) cs->Use(&m_DefaultFont);
	else cs->Use(&(pLabel->Font));
	cs->DrawText(r,pLabel->GetText(),StringAlignmentCenter,StringAlignmentMiddle);
}

void ASkin::DrawHint(ACanvas* cs,const AString& sText,ARect r)
{
	if( sText.Length() < 1 ) return;

	APen pen;
	pen.SetColor(AColor(255,192,192,192));
	pen.SetWidth(0.3);
	ALinearGradientBrush br;
	br.IsVert(true);
	br.SetStartColor(AColor(200,174,238,231));
	br.SetEndColor(AColor(200,247,253,254));
	br.SetRect(r);

	cs->Use(&br)->FillRoundRect(r,3);
	cs->Use(&pen)->FillRoundRect(r,3);

	AFont f(_T("新宋体"),12,AColor(200,0,0,0));
	cs->Use(&f)->DrawText(r.left+3,r.top+3,sText,9);
}

void ASkin::DrawSpliter(ACanvas* cs,AControl* pControl)
{
	ASpliter* pSpliter = (ASpliter*)pControl;

	ARect r = pSpliter->GetAbsPosition();
	

	

	ASolidBrush br(AColor(255,192,231,233));
	cs->Use(&br)->FillRect(r);
}

void ASkin::DrawSpinControl(ACanvas* cs,AControl* pControl)
{
	ASpinControl* pSpin = (ASpinControl*)pControl;

	ARect r = pSpin->GetAbsPosition();

	ASolidBrush br(m_BackColor);
	cs->Use(&br)->FillRect(r);

	APen pen;
	pen.SetColor(m_BorderColor);
	cs->Use(&pen)->DrawRect(r);
}
void ASkin::DrawSpinUpButton(ACanvas* cs,AControl* pControl)
{
	AButton* pButton = (AButton*)pControl;

	ARect r = pButton->GetAbsPosition();

	ASolidBrush br(m_EmphasizeColor);
	
	if( pButton->GetUICore()->GetMouseDownControl() == pButton )
		br.SetColor( m_DownColor );
	else if( pButton->GetUICore()->IsOverMe(pButton) )
		br.SetColor( m_OverColor );
	APoint pt1(r.left+r.GetWidth()/2,r.top+1);
	APoint pt2(r.left,r.bottom-2);
	APoint pt3(r.right,r.bottom-2);
	cs->Use(&br)->FillTrigle(pt1,pt2,pt3);
}
void ASkin::DrawSpinDownButton(ACanvas* cs,AControl* pControl)
{
	AButton* pButton = (AButton*)pControl;

	ARect r = pButton->GetAbsPosition();

	ASolidBrush br(m_EmphasizeColor);
	if( pButton->GetUICore()->GetMouseDownControl() == pButton )
		br.SetColor( m_DownColor );
	else if( pButton->GetUICore()->IsOverMe(pButton) )
		br.SetColor( m_OverColor );
	APoint pt1(r.left+r.GetWidth()/2,r.bottom-1);
	APoint pt2(r.left,r.top+2);
	APoint pt3(r.right,r.top+2);
	cs->Use(&br)->FillTrigle(pt1,pt2,pt3);
}