//---------------------------------------------------------------------------
#include "stdafx.h"

#include "UIDrawer.h"

//---------------------------------------------------------------------------

CUIDrawer::CUIDrawer(CUIDoc* pDoc,BVCanvas* pCanvas,ARect r):
			m_pDoc(pDoc),
			m_pCanvas(pCanvas),
			m_rBox(r)
{
}

CUIDrawer::~CUIDrawer()
{
}

ARect	CUIDrawer::Offset(ARect r)
{
	r.Offset(m_rBox.left,m_rBox.top);
	return r;
}
void CUIDrawer::Draw()
{
	if( m_pDoc == NULL ) return;
	if( &(m_pDoc->GetWebForm()) == NULL ) return;

	DrawShape( m_pDoc->GetWebForm() );


	//ÏÔÊ¾ÒÆ¶¯×´Ì¬
	if( m_pDoc->GetLocked() == false && m_pDoc->IsMoving() )
	{
		BVPoint ptDown = m_pDoc->GetDown();
		BVPoint ptMove = m_pDoc->GetMove();
		BVSize siz(ptMove.x-ptDown.x,ptMove.y-ptDown.y);
		SELECTED_TYPE& aSel = m_pDoc->GetSelected();
		for(SELECTED_TYPE::iterator it=aSel.begin();it!=aSel.end();it++)
		{
			afc_object<CControl> psel ;
			psel = it->first;
			BVRect r = Offset(psel->GetAbsPosition());

			switch( m_pDoc->GetCurHitResult().m_iHandle )
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
				r.top += siz.cy;
				r.right += siz.cx;
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

			BVBrush br(BVColor(128,0,0,200));
			m_pCanvas->Use(&br)->FillRect(r);

			BVPen pen(1,BVColor(255,0,0,0),DashStyleDot);
			m_pCanvas->Use(&pen)->DrawRect(r);
		}
	}  
}

void CUIDrawer::DrawHandler(bool bFocus,BVRect r)
{
	BVRect rx[8];
	m_pDoc->GetHandle(r,rx);
	for(int i=0;i<8;i++)
	{
		BVColor cr;
		if( bFocus ) cr = BVColor(255,128,128,255);
		else cr = BVColor(255,255,255,255);
		ASolidBrush br(cr);
		APen pen(1,BVColor(255,0,0,255),DashStyleSolid);
		ARect rh = rx[i];//Offset(rx[i]);
		m_pCanvas->Use(&br)->FillRect(rh);
		m_pCanvas->Use(&pen)->DrawRect(rh);
	}
}


void CUIDrawer::DrawShape(const afc_object<CControl>& pBase)
{
	BVRect r= Offset(pBase->GetAbsPosition());

        EString sDrawStyle;
        if( pBase->m_pDefNode != ANULL )
                sDrawStyle = pBase->m_pDefNode->GetProp(_ES("drawstyle"));

	RenderDraw(r,sDrawStyle,pBase);



	if( m_pDoc->IsSelected(pBase) )
	DrawHandler(m_pDoc->GetFocus() == &pBase ? true : false ,r);

	if( pBase->m_aChild.size() > 0 )
	{
		for(int i=pBase->m_aChild.size()-1;i>=0;i--)
		{
			afc_object<CControl> pc;
			pc = pBase->m_aChild[i];
			DrawShape(pc);
		}
	}
}

void CUIDrawer::RenderDraw(BVRect r,const EString& sDraw,const afc_object<CControl>& pBase)
{
	if( sDraw == _ES("Custom") )
	{
		ASolidBrush br(BVColor(255,255,255,255));
		APen pen;
		pen.SetColor(BVColor(255,0,0,0));
		m_pCanvas->Use(&br)->FillRect(r);
		m_pCanvas->Use(&pen)->DrawRect(r);
	}
	else if( sDraw == _ES("Box") )
	{
		ASolidBrush br(AColor(255,255,255,255));
		APen pen;
		pen.SetColor(AColor(255,112,162,202));
		m_pCanvas->Use(&br)->FillRect(r);
		m_pCanvas->Use(&pen)->DrawRect(r);
	}
	else if( sDraw == _ES("Button") )
	{
		ASolidBrush br(BVColor(230,158,225,85));
		APen pen;
		pen.SetColor(BVColor(255,13,67,10));
		m_pCanvas->Use(&br)->FillRect(r);
		m_pCanvas->Use(&pen)->DrawRect(r);

		EString sCaption = (*pBase)[_ES("tx")]->AsString();
		if( sCaption.Length() < 1 ) sCaption = _ES("...");

		AFont f;
		f = (*pBase)[_ES("font")]->AsFont();
		m_pCanvas->Use(&f)->DrawText(r,sCaption,StringAlignmentCenter,StringAlignmentMiddle);
	}
	else if( sDraw == _ES("Label") )
	{
		EString sCaption = (*pBase)[_ES("tx")]->AsString();
		if( sCaption.Length() < 1 ) sCaption = _ES("...");
		AFont f;
		f = (*pBase)[_ES("font")]->AsFont();
		m_pCanvas->Use(&f)->DrawText(r,sCaption,StringAlignmentNear,StringAlignmentMiddle);

	}
	else if( sDraw == _ES("Link") )
	{
		EString sCaption = (*pBase)[_ES("tx")]->AsString();
		if( sCaption.Length() < 1 ) sCaption = _ES("http://");

		AFont f;
		f = (*pBase)[_ES("font")]->AsFont();
		f.SetStyle(FontStyleUnderline);
		f.SetColor(BVColor(255,0,0,255));
		m_pCanvas->Use(&f)->DrawText(r,sCaption,StringAlignmentNear,StringAlignmentMiddle);
	}
	else if( sDraw == _ES("Image") )
	{
		ASolidBrush br(BVColor(255,137,182,140));
		APen pen;
		pen.SetColor(BVColor(255,0,0,0));
		m_pCanvas->Use(&br)->FillRect(r);
		m_pCanvas->Use(&pen)->DrawRect(r);

		int iSize=r.GetWidth();
		if( iSize > r.GetHeight() ) iSize = r.GetHeight();
		iSize /= 4;

		AFont f;
		f.SetName(_T("ÐÂËÎÌå"));
		f.SetSize(iSize);
		f.SetColor(BVColor(255,0,0,0));

		EString sCaption = _ES("Í¼Æ¬");

		m_pCanvas->Use(&f)->DrawText(r,sCaption,StringAlignmentCenter,StringAlignmentMiddle);
	}
	else if( sDraw == _ES("CheckBox") )
	{
		EString sCaption = (*pBase)[_ES("tx")]->AsString();
		sCaption = "¡õ" + sCaption;

		AFont f;
		f = (*pBase)[_ES("font")]->AsFont();
		m_pCanvas->Use(&f)->DrawText(r,sCaption,StringAlignmentNear,StringAlignmentMiddle);
	}
	else if( sDraw == _ES("RadioButton") )
	{
		EString sCaption = (*pBase)[_ES("tx")]->AsString();
		sCaption = "¡ð" + sCaption;

		AFont f;
		f = (*pBase)[_ES("font")]->AsFont();
		m_pCanvas->Use(&f)->DrawText(r,sCaption,StringAlignmentNear,StringAlignmentMiddle);
	}
	else if( sDraw == _ES("ComboBox") )
	{
		ASolidBrush br(BVColor(255,255,255,255));
		APen pen;
		pen.SetColor((*pBase)[_ES("bcr")]->AsColor());
		m_pCanvas->Use(&br)->FillRect(r);
		m_pCanvas->Use(&pen)->DrawRect(r);

		BVRect rDot(r.right-r.GetHeight(),r.top+1,r.right-1,r.top+r.GetHeight()-1);
		br.SetColor(BVColor(192,189,189,251));
		pen.SetColor(BVColor(204,13,13,176));
		m_pCanvas->Use(&br)->FillRect(rDot);
		m_pCanvas->Use(&pen)->DrawRect(rDot);

		BVPoint pts[4];
		pts[0] = BVPoint(rDot.left+rDot.GetWidth()/2,rDot.bottom-2);
		pts[1] = BVPoint(rDot.left+rDot.GetWidth()/2-r.GetHeight()/4,rDot.bottom-2-r.GetHeight()/2);
		pts[2] = BVPoint(rDot.left+rDot.GetWidth()/2+r.GetHeight()/4,rDot.bottom-2-r.GetHeight()/2);
		pts[3] = pts[0];
		br.SetColor(BVColor(255,0,0,0));
		m_pCanvas->Use(&br)->FillPolygon(pts,4);
	}
	else if( sDraw == _ES("Edit") )
	{
		ASolidBrush br(BVColor(255,255,255,255));
		APen pen;
		pen.SetColor(AColor(255,0,0,0));
		m_pCanvas->Use(&br)->FillRect(r);
		m_pCanvas->Use(&pen)->DrawRect(r);

	}
	else if( sDraw == _ES("ExPanel") )
	{
		ASolidBrush br(BVColor(255,255,255,255));
		APen pen;
		pen.SetColor((*pBase)[_ES("bcr")]->AsColor());
		m_pCanvas->Use(&br)->FillRect(r);
		m_pCanvas->Use(&pen)->DrawRect(r);

// 		EString sCaption = (*pBase)[_ES("tx")]->AsString();
// 		//sCaption = "¡õ" + sCaption;
// 
// 		m_pCanvas->DrawString(
// 								sCaption.ConvertTo(),
// 								r,
// 								(*pBase)[_ES("font")]->AsFont(),
// 								BVTA_Near,
// 								BVTA_Near);
// 								BVRect rDot(r.x+r.width-10,r.y+4,7,5);
// 		//m_pCanvas->FillRectangle(BVBrush(BVColor(192,189,189,251)),rDot);
// 		//m_pCanvas->DrawRectangle(BVPen(BVColor(204,13,13,176),1),rDot);
// 
// 		BVPoint pts[4];
// 		pts[0] = BVPoint(rDot.x,rDot.y);
// 		pts[1] = BVPoint(rDot.x+rDot.width,rDot.y);
// 		pts[2] = BVPoint(rDot.x+rDot.width/2,rDot.y+rDot.height);
// 		pts[3] = pts[0];
// 		m_pCanvas->FillPolygon(BVColor(255,0,0,0),pts,4);
// 
// 		BVPen pen;
// 		pen.width = 1;
// 		pen.color = BVColor(255,0,0,0);
// 
// 		BVPoint ps = BVPoint(r.x,r.y+14);
// 		BVPoint pe = BVPoint(r.x+r.width,r.y+14);
// 		m_pCanvas->DrawLine(pen,ps,pe);

	}
	else if( sDraw == _ES("Form") )
	{
		ASolidBrush br(BVColor(255,255,255,255));
		APen pen;
		pen.SetColor(BVColor(255,0,0,0));
		m_pCanvas->Use(&br)->FillRect(r);
		m_pCanvas->Use(&pen)->DrawRect(r);

	}
        else if( sDraw == _ES("menu") )
        {
        }
	else
	{
		ASolidBrush br(BVColor(255,255,255,255));
		APen pen;
		pen.SetColor(BVColor(255,0,0,0));
		m_pCanvas->Use(&br)->FillRect(r);
		m_pCanvas->Use(&pen)->DrawRect(r);

		EString sCaption = (*pBase)[_ES("tx")]->AsString();
		//sCaption = "¡õ" + sCaption;

		AFont f;
		f = (*pBase)[_ES("font")]->AsFont();
		m_pCanvas->Use(&f)->DrawText(r,sCaption,StringAlignmentCenter,StringAlignmentMiddle);

	}
}




