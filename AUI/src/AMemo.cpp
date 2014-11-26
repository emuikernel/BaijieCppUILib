#include "stdafx.h"
#include "AMemoTextStorageImpl.h"
#include "AMemoActionImpl.h"

/* 基本思路：
 *	编辑框接收用户的操作，触发Action，即：Editor -> Action
 *　　　键盘和鼠标的操作都是针对“字符串”的操作，即：Action -> TextStorage
 *      字符的改变将会引起光标位子、格式串位置和长度的变化，即：TextStorage -> Format
 *      格式变化后会影响编辑框整体的宽度和高度，即：Format -> Editor
 *   何时着色：在字符串改变时，调整Format的位置和长度，触发着色事件，从而重新着色或调整着色
 *   手工着色：通过接口进行手工着色
 */

AMemo::AMemo(AComponent* pOwner):base_class(pOwner)
{
	m_bReadOnly = false;
	m_pTextStorage = NULL;
	m_pAction = NULL;
	m_iBorderWidth = 1;
	m_iLeftSpace = 2;//fix me later : Action要用到这个变量？
	m_iTopSpace =2 ;
	m_iRowSpace = 6;
	m_iTabChars = 4;
	m_pEdit = NULL;
	m_nCaretTimerID = 0;
	m_iCaret =  0;

	m_pEdit = new AControl(this);
	m_pEdit->SetCursorType(ctIBeam);
	m_pEdit->SetCanFocus(true);
	EVENT_CONNECT(m_pEdit,OnPaint,OnEditPaint);
	EVENT_CONNECT(m_pEdit,OnFocus,OnEditFocus);
	EVENT_CONNECT(m_pEdit,OnKillFocus,OnEditLostFocus);
	EVENT_CONNECT(m_pEdit,OnMouseDown,OnEditMouseDown);
	EVENT_CONNECT(m_pEdit,OnMouseMove,OnEditMouseMove);
	EVENT_CONNECT(m_pEdit,OnMouseUp,OnEditMouseUp);
	EVENT_CONNECT(m_pEdit,OnKeyboard,OnEditKeyBoard);
	EVENT_CONNECT(m_pEdit,OnDblClick,OnEditDblClick);
	
	//设置缺省实现
	SetTextStorage(new AMemoTextStorageImpl());
	SetAction( new AMemoActionImpl(this));

	//设置缺省滚动条
	SetAutoVScrollBar(true);
	SetAutoHScrollBar(true);
}

AMemo::~AMemo()
{
	if( m_pTextStorage ) m_pTextStorage->Release();
	if( m_pAction ) m_pAction->Release();

	m_pTextStorage = NULL;
	m_pAction = NULL;
	if( m_nCaretTimerID )
	{
		ATimer::EndTimer(m_nCaretTimerID);
		m_nCaretTimerID = 0;
	}
}

bool AMemo::GetReadOnly()
{
	return m_bReadOnly;
}

void AMemo::SetReadOnly(bool b)
{
	m_bReadOnly = b;
}

int AMemo::GetLeftSpace()
{
	return m_iLeftSpace;
}
void AMemo::SetLeftSpace(int v)
{
	m_iLeftSpace = v;
}

int AMemo::GetTopSpace()
{
	return m_iTopSpace;
}
void AMemo::SetTopSpace(int v)
{
	m_iTopSpace = v;
}

int AMemo::GetRowSpace()
{
	return m_iRowSpace;
}
void AMemo::SetRowSpace(int v)
{
	m_iRowSpace = v;
	m_pTextStorage->SetRowSpace(v);
}


void AMemo::SetTextStorage(AMemoTextStorage* p)
{
	if( m_pTextStorage ) m_pTextStorage->Release();
	if( p == NULL ) p = new AMemoTextStorageImpl();
	m_pTextStorage = p;
	m_pTextStorage->AddRef();
	m_pTextStorage->SetTabWidth( m_iTabChars );
	m_pTextStorage->SetRowSpace(m_iRowSpace);
	//fix me later : 在修改Memo字体时要同步修改TextStorage的缺省字体
	if( GetUseSkin() ) m_pTextStorage->SetDefaultFont( AApplication::Get()->GetCurSkin()->m_DefaultFont );
	else m_pTextStorage->SetDefaultFont(Font);
}

AMemoTextStorage* AMemo::GetTextStorage()
{
	return m_pTextStorage;
}


AMemoAction* AMemo::GetAction()
{
	return m_pAction;
}
void AMemo::SetAction(AMemoAction* p)
{
	if( m_pAction ) m_pAction->Release();
	if( p == NULL ) p = new AMemoActionImpl(this);
	m_pAction = p;
	m_pAction->AddRef();
}

void AMemo::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawMemo(cs,this);

	AContainer::DoPaintChildren(pEvent);
}

void AMemo::SetText(AString s)
{
	m_pTextStorage->SetSingleText(s);
}

AString AMemo::GetText()
{
	return m_pTextStorage->GetSingleText();
}



int AMemo::GetTabChars()
{
	return m_iTabChars;
}

void AMemo::SetTabChars(int v)
{
	m_iTabChars = v;
	m_pTextStorage->SetTabWidth(v);
}

void AMemo::DoLayout()
{
	if( m_pEdit )
	{
		ASize sz = m_pTextStorage->GetEditorSize();
		m_pEdit->SetWidth(sz.cx);
		m_pEdit->SetHeight(sz.cy);
		if( m_pEdit->GetLeft() != m_iLeftSpace ) m_pEdit->SetLeft(m_iLeftSpace);
		if( m_pEdit->GetTop() != m_iTopSpace ) m_pEdit->SetTop(m_iTopSpace);
		if( m_pEdit->GetWidth() < GetWidth() ) m_pEdit->SetWidth(GetWidth() - m_iLeftSpace);
		if( m_pEdit->GetHeight() < GetHeight() ) m_pEdit->SetHeight(GetHeight() - m_iTopSpace);
	}
	
	base_class::DoLayout();
}

void AMemo::OnEditPaint(AObject* pSender,AEvent* pEvent)
{
	AControl* pEdit = (AControl*)pSender;

	ACanvas* cs = pEdit->GetUICore()->GetCanvas();
	cs->SetSmoothingMode(SmoothingModeNone);
	cs->SetTextHint(thNone);

	ARect r = pEdit->GetAbsPosition();
	if( !pEdit->GetUICore()->InDrawArea(r) ) return;

	ASolidBrush b;
	if( this->GetEnabled() == false ) b.SetColor(AApplication::Get()->GetCurSkin()->m_DisableColor);
	else b.SetColor(WHITE_COLOR);

	cs->Use(&b)->FillRect(r);

	//显示选中行的背景
	if( m_pAction->GetSelType() != SEL_TYPE_NONE )
	{
		int delta=2;//修正显示误差
		ACaretSeat ss = m_pAction->GetSelStart();
		ACaretSeat se = m_pAction->GetSelEnd();
		if( !(se.iRow > ss.iRow || (se.iRow == ss.iRow && se.iCol >= ss.iCol ) ) )
		{
			ss = m_pAction->GetSelEnd();
			se = m_pAction->GetSelStart();
		}
		ARect rs,re;
		rs = m_pAction->GetCaretPosBySeat(ss);
		re = m_pAction->GetCaretPosBySeat(se);

		ASolidBrush br;
		br.SetColor( AColor(200,136,183,221) );

		if( m_pAction->GetSelType() == SEL_TYPE_COL )
		{
			ARect rd(rs.left,rs.top,re.right,re.bottom+delta);
			rd.Offset(r.left,r.top);
			cs->Use(&br)->FillRect(rd);
		}
		else
		{
			//先显示起始行背景
			if( ss.iRow >= 0  )
			{
				int ic = m_pTextStorage->GetLineCharCount(ss.iRow) -1;
				if( se.iRow == ss.iRow ) ic = se.iCol;

				ARect rTmp = m_pAction->GetCaretPosBySeat(ACaretSeat(ss.iRow,ic-1));
				ARect rd(rs.left,rs.top,rTmp.right,rTmp.bottom+delta);
				rd.Offset(r.left,r.top);
				cs->Use(&br)->FillRect(rd);

			}
			//再显示最下一行
			if( se.iRow < m_pTextStorage->GetLineCount() )
			{
				int ic=0;
				if( ss.iRow == se.iRow ) ic = ss.iCol;
				ARect rTmp = m_pAction->GetCaretPosBySeat(ACaretSeat(se.iRow,ic));
				ARect rd(rTmp.left,rTmp.top,re.right,re.bottom+delta);
				rd.Offset(r.left,r.top);
				cs->Use(&br)->FillRect(rd);
			}
			//再显示中间的行
			for(int k=ss.iRow+1;k<se.iRow;k++)
			{
				int w = m_pTextStorage->GetLineWidth(k);
				int h = m_pTextStorage->GetLineHeight(k);
				int t = m_pTextStorage->GetLineTop(k);
				ARect rd(0,t - m_pTextStorage->GetRowSpace()/2,w,t+h+m_pTextStorage->GetRowSpace()/2+delta);
				rd.Offset(r.left,r.top);
				cs->Use(&br)->FillRect(rd);
			}
		}
	}
	int iLineCount = m_pTextStorage->GetLineCount();

	AString sTab;
	for(int i=0;i<m_iTabChars;i++) sTab += _T(" ");
	int y=0;
	for(int i=0;i<iLineCount;i++)
	{
		AString s = m_pTextStorage->GetLine(i);
		int iLineHeight = m_pTextStorage->GetLineHeight(i);
		int iLen = s.Length();
		int x=0;

		int iLineWidth = m_pTextStorage->GetLineWidth(i);
		ARect rDraw(0,y,iLineWidth,y+iLineHeight);
		rDraw.Offset(r.left,r.top);
		if( !pEdit->GetUICore()->InDrawArea(rDraw) ) 
		{
			y += iLineHeight + m_pTextStorage->GetRowSpace();
			continue;
		}

		if( iLen == 1 )
		{
			if( s.Find(_T("\t")) > -1 ) s.Replace(_T("\t"),sTab);
			_drawText(cs,s,x+rDraw.left,rDraw.top,m_pTextStorage->GetMeasure(ACaretSeat(i,0)),iLineHeight,m_pTextStorage->GetColoration(ACaretSeat(i,0)));
		}
		else if( iLen > 1 )
		{
			int indexFlag=m_pTextStorage->GetColoration(ACaretSeat(i,0));
			int iPosFlag=0;
			int iPos = 1;
			
			while( iPos < iLen )
			{
				int index = m_pTextStorage->GetColoration(ACaretSeat(i,iPos));
				if( index != indexFlag )
				{
					AString sDraw = s.Mid(iPosFlag,iPos-iPosFlag);
					if( sDraw.Find(_T("\t")) > -1 )
						sDraw.Replace(_T("\t"),sTab);
					_drawText(cs,sDraw,x+rDraw.left,rDraw.top,m_pTextStorage->GetMeasure(ACaretSeat(i,iPos-1)),iLineHeight,indexFlag);

					x = m_pTextStorage->GetMeasure( ACaretSeat(i,iPos-1) );
					indexFlag = index;
					iPosFlag = iPos;
				}
				iPos++;
			}
			if( iPos >= iLen && iPosFlag < iLen )
			{
				AString sDraw = s.Mid(iPosFlag,iPos-iPosFlag);
				if( sDraw.Find(_T("\t")) > -1 )
					sDraw.Replace(_T("\t"),sTab);
				_drawText(cs,sDraw,x+rDraw.left,rDraw.top,m_pTextStorage->GetMeasure(ACaretSeat(i,iPos-1)),iLineHeight,indexFlag);
			}
		}
		y += iLineHeight + m_pTextStorage->GetRowSpace();
	}
	//显示光标
	int imod = m_iCaret % 10;
	bool bShowCaret = ( imod == 0 || imod == 1 || imod == 2 ? false : true);
	if( GetUICore()->IsFocus(this) && bShowCaret )
	{
		ARect rCaret = m_pAction->GetCaretPos();
		rCaret.Offset(r.left,r.top);

		//APen penCaret(1,AApplication::Get()->GetCurSkin()->m_EmphasizeColor,DashStyleSolid);
		APen penCaret(1,AColor(255,0,0,0),DashStyleSolid);
		//cs->Use(&penCaret)->DrawRect(rCaret);
		cs->Use(&penCaret)->DrawLine(APoint(rCaret.left,rCaret.top),APoint(rCaret.left,rCaret.bottom+4));
	}
}

void AMemo::_drawText(ACanvas* cs,const AString& sText,int x,int y,int w,int iLineHeight,int indexColoration)
{
	AFont* pFont = NULL;
	if( indexColoration == -1 )
	{
		if( GetUseSkin() ) pFont = &(AApplication::Get()->GetCurSkin()->m_DefaultFont);
		else pFont = &(this->Font);
	}
	else
	{
		AMemoColorationItem* pItem = m_pTextStorage->GetColorationByIndex(indexColoration);
		pFont = &(pItem->m_Font);

		if( pItem->m_BackColor.alpha > 0 )
		{
			ASolidBrush br;
			br.SetColor( pItem->m_BackColor );
			cs->Use(&br)->FillRect(ARect(x,y,x+w,y+iLineHeight));
		}
		
	}
	cs->Use(pFont)->DrawText(x,y,sText);
}
void AMemo::OnEditDblClick(AObject* pSender,AEvent* pEvent)
{
	m_pAction->OnDblClick(pSender,pEvent);
}
void AMemo::OnEditFocus(AObject* pSender,AEvent* pEvent)
{
	if( m_nCaretTimerID == 0 )
	{
		m_nCaretTimerID = ATimer::StartTimer(200,this,(FOnTimerFunc)&AMemo::_onCaretTimer,this);
		GetUICore()->Refresh(GetAbsPosition());
	}
}

void AMemo::DoFocus(AEvent* pEvent)
{
	OnEditFocus(m_pEdit,pEvent);
	base_class::DoFocus(pEvent);
}
void AMemo::OnEditLostFocus(AObject* pSender,AEvent* pEvent)
{
	if( !GetUICore()->IsFocus(this) )
	{
		ATimer::EndTimer(m_nCaretTimerID);
		m_nCaretTimerID = 0;
		GetUICore()->Refresh(GetAbsPosition());
	}
}

void AMemo::_onCaretTimer(AObject* pAsso,int nTimerID)
{
	if( GetUICore()->GetFocusControl() != (AControl*)m_pEdit )
		return;
	m_iCaret ++;
	if( m_iCaret > 10000 ) m_iCaret = 0;
	//由于不经过WM_TIMER 事件，因此也不经过核心事件处理，所以需要手工触发GetUICore()->Refresh(GetAbsPosition());
	ARect r = GetAbsPosition();
	::InvalidateRect(GetUICore()->GetHandle(),&r,FALSE);
	::UpdateWindow(GetUICore()->GetHandle());
}

void AMemo::OnEditMouseDown(AObject* pSender,AEvent* pEvent)
{
	m_pAction->OnMouseDown(pSender,pEvent);
}
void AMemo::OnEditMouseMove(AObject* pSender,AEvent* pEvent)
{
	m_pAction->OnMouseMove(pSender,pEvent);
}
void AMemo::OnEditMouseUp(AObject* pSender,AEvent* pEvent)
{
	m_pAction->OnMouseUp(pSender,pEvent);
}
void AMemo::OnEditKeyBoard(AObject* pSender,AEvent* pEvent)
{
	m_pAction->OnKeyBoard(pSender,pEvent);
}

AControl* AMemo::GetEditor()
{
	return m_pEdit;
}
bool AMemo::CanFocus()
{
	return true;
}