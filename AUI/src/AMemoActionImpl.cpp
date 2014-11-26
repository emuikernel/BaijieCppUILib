#include "stdafx.h"
#include "AMemoActionImpl.h"
#include "AMemo.h"

AMemoActionImpl::AMemoActionImpl(AMemo* pMemo):
	base_class(),
	m_pMemo(pMemo)
{
	m_iCaretStep = 0;
	m_seatCaret.iRow = -1;
	m_seatCaret.iCol = -1;
	m_iSelType = SEL_TYPE_NONE;
	SetCaretSeat( ACaretSeat(0,0) );
}

AMemoActionImpl::~AMemoActionImpl()
{

}

//鼠标和键盘事件
void AMemoActionImpl::OnMouseDown(AObject* pSender,AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL ) return;

	_unselect();
	int x = evt->m_X;
	int y = evt->m_Y;
	ARect r = m_pMemo->GetEditor()->GetAbsPosition();
	x -= r.left;
	y -= r.top;
	m_ptDown = APoint(x,y);
	ACaretSeat seat = m_pMemo->GetTextStorage()->SeatByPos(APoint(x,y));
	SetCaretSeat(seat);
	m_pMemo->Refresh();
}
void AMemoActionImpl::OnMouseMove(AObject* pSender,AEvent* pEvent)
{
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt == NULL ) return;

	int x = evt->m_X;
	int y = evt->m_Y;
	ARect r = m_pMemo->GetEditor()->GetAbsPosition();
	x -= r.left;
	y -= r.top;
	if( evt->m_KeyStates.Has(ksLeftButton) )
	{
		ACaretSeat ss = m_pMemo->GetTextStorage()->SeatByPos(m_ptDown);
		ACaretSeat se = m_pMemo->GetTextStorage()->SeatByPos(APoint(x,y));
		m_seatSelStart = ss;
		m_seatSelEnd = se;
		if( evt->m_KeyStates.Has(ksAlt) ) m_iSelType = SEL_TYPE_COL;
		else m_iSelType = SEL_TYPE_ROW;
		SetCaretSeat(se);
		m_pMemo->Refresh();
	}
}
void AMemoActionImpl::OnMouseUp(AObject* pSender,AEvent* pEvent)
{

}
void AMemoActionImpl::OnKeyBoard(AObject* pSender,AEvent* pEvent)
{
	AKeyEvent* evt = dynamic_cast<AKeyEvent*>(pEvent);
	if( evt == NULL ) return;
	/**   要注意：一定要优先处理ketPress事件    **/
	WCHAR ch = evt->m_chKey;
	if( evt->m_Type == ketPress )
	{
		
		if( ch >= 0x20 )
			InputChar(ch);
	}
	else if( evt->m_Type == ketDown )
	{
		if( (ch == 'V' || ch == 'v') && evt->m_KeyStates.Has(ksCtrl) )
		{
			Paste();
		}
		else if( (ch == 'C' || ch == 'c') && evt->m_KeyStates.Has(ksCtrl) )
		{
			Copy();
		}
		else if( (ch == 'X' || ch == 'x') && evt->m_KeyStates.Has(ksCtrl) )
		{
			Cut();
		}
		else if( ch == VK_TAB /*9*/ )// tab
		{
			if( evt->m_KeyStates.Has(ksAlt) )
			{
				//焦点转到下一个控件
				AContainer* pc = dynamic_cast<AContainer*>(m_pMemo->GetParent());
				if( pc )
				{
					int index,iCount;
					iCount = pc->GetChildControls()->GetCount();
					index = pc->GetChildControls()->IndexOf(m_pMemo);
					index ++;
					if( index >= iCount ) index = 0;
					AControl* pNext= dynamic_cast<AControl*>(pc->GetChildControls()->GetItem(index));
					m_pMemo->GetUICore()->SetFocusControl(pNext);
					AEvent ev;
					m_pMemo->DoLostFocus(&ev);
					pNext->DoEnter(&ev);
				}
			}
			else
				InputTab();
		}
		else if( ch == VK_RETURN )
			InputEnter();
		else if( ch == VK_BACK /*8*/ ) // 退格键
			InputBackspace();
		else if( ch == VK_LEFT )
		{
			//改变光标位置
			if( m_iSelType == SEL_TYPE_NONE )
				SetCaretSeat(_toPrev(m_seatCaret));
			else _unselect();
		}
		else if( ch ==  VK_RIGHT )
		{
			//改变光标位置
			if( m_iSelType == SEL_TYPE_NONE )
				SetCaretSeat(_toNext(m_seatCaret));
			else _unselect();
		}
		else if( ch == VK_UP )
		{
			//改变光标位置
			if( m_iSelType == SEL_TYPE_NONE )
			{
				AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
				ACaretSeat seat = m_seatCaret;
				if( seat.iRow > 0 )
				{
					seat.iRow --;
					if( seat.iCol > pTextStorage->GetLineCharCount(seat.iRow) )
						seat.iCol = pTextStorage->GetLineCharCount(seat.iRow);
					SetCaretSeat(seat);
				}
			}
			else
				_unselect();
			
		}
		else if( ch == VK_DOWN )
		{
			//改变光标位置
			if( m_iSelType == SEL_TYPE_NONE )
			{
				AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
				ACaretSeat seat = m_seatCaret;
				if( seat.iRow < pTextStorage->GetLineCount() - 1 )
				{
					seat.iRow ++;
					if( seat.iCol > pTextStorage->GetLineCharCount(seat.iRow) )
						seat.iCol = pTextStorage->GetLineCharCount(seat.iRow);
					SetCaretSeat(seat);
				}
			}
			else
				_unselect();
		}
		else if( ch == VK_DELETE )
		{
			InputDel();
		}
		
	}
	else if( evt->m_Type == ketUp )
	{
		
		if( ch == VK_HOME )
		{
			if( m_pMemo->GetTextStorage()->GetLineCount()>0)
			{
				ACaretSeat cs = GetCaretSeat();
				AString sLine = m_pMemo->GetTextStorage()->GetLine(cs.iRow);
				ACHAR* sz = (ACHAR *)sLine.Text();
				while(*sz && *sz == ' ' || *sz == '\t' ) sz++;
				if( cs.iCol == sz - sLine.Text() ) cs.iCol = 0;
				else cs.iCol = sz-sLine.Text();
				SetCaretSeat(cs);
			}
			
		}
		else if( ch == VK_END )
		{
			if( m_pMemo->GetTextStorage()->GetLineCount()>0)
			{
				ACaretSeat cs = GetCaretSeat();
				AString sLine = m_pMemo->GetTextStorage()->GetLine(cs.iRow);
				ACHAR* sz = (ACHAR *)sLine.Text() + sLine.Length();
				while(sz != sLine.Text() && *sz == ' ' || *sz == '\t' ) sz--;
				cs.iCol = sz - sLine.Text();
				SetCaretSeat(cs);
			}
			
		}
	}
}

//光标控制

ACaretSeat AMemoActionImpl::GetCaretSeat()
{
	return m_seatCaret;
}
void AMemoActionImpl::SetCaretSeat(ACaretSeat seat)//设置光标位子
{
	if( seat.iRow == m_seatCaret.iRow && seat.iCol == m_seatCaret.iCol ) return;
	int r = seat.iRow;
	int c = seat.iCol;
	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();

	int iLineCount = pTextStorage->GetLineCount();
	if( r < 0 ) r = 0;
	if( r > iLineCount ) r = iLineCount - 1;

	if( r < iLineCount )
	{
		int iCharsCount = pTextStorage->GetLineCharCount(r);
		if( c < 0 ) c = 0;
		if( c > iCharsCount ) c = iCharsCount;
	}
	m_seatCaret = ACaretSeat(r,c);
	_updateCaretPos();
	//确保光标在可见区域
	ARect rAbs = m_pMemo->GetEditor()->GetAbsPosition();
	ARect rMemo = m_pMemo->GetAbsPosition();
	ARect rc = m_rCaret;
	rc.Offset(rAbs.left,rAbs.top);
	AScrollBar* pHB = m_pMemo->GetHScrollBar();
	AScrollBar* pVB = m_pMemo->GetVScrollBar();
	int delta=0;
	int dw=0,dh=0;
	if( pHB ) dh = pHB->GetHeight();
	if( pVB ) dw = pVB->GetWidth();
	rMemo.right -= dw;
	rMemo.bottom -= dh;

	if( pHB )
	{
		if( rc.left < rMemo.left )
		{
			delta = rc.left - rMemo.left;
			pHB->SetPosition( pHB->GetPosition() + delta );
		}
		else if( rc.left >= rMemo.right )
		{
			delta = rc.left - rMemo.right+rc.GetWidth()*2;
			pHB->SetPosition( pHB->GetPosition() + delta );
		}
	}
	if( pVB )
	{
		if( rc.top < rMemo.top )
		{
			delta = rc.top - rMemo.top;
			pVB->SetPosition( pVB->GetPosition() + delta );
		}
		else if( rc.top >= rMemo.bottom )
		{
			delta = rc.top - rMemo.bottom+rc.GetHeight();
			pVB->SetPosition( pVB->GetPosition() + delta );
		}
	}
	if( delta != 0 )
	{
		m_pMemo->DoLayout();
		m_pMemo->Refresh();
	}
}

ARect AMemoActionImpl::GetCaretPos()
{
	return m_rCaret;
}

ARect AMemoActionImpl::GetCaretPosBySeat(ACaretSeat seat)
{
	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
	int iRowSpace = m_pMemo->GetRowSpace();

	int x=0,y=0;
	y = pTextStorage->GetLineTop(seat.iRow);
	if( seat.iCol > 0 ) 
	{
		ACaretSeat seat2;
		seat2.iRow = seat.iRow;
		seat2.iCol = seat.iCol-1;
		x = pTextStorage->GetMeasure(seat2);
	}
	int iLineHeight = _getLineHeight(seat.iRow);
	return ARect(x,y,x+1,y+iLineHeight);
}
void AMemoActionImpl::_updateCaretPos()
{
	m_rCaret = GetCaretPosBySeat(m_seatCaret);
}


int AMemoActionImpl::_getLineHeight(int iLine)
{
	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
	int h=0;
	if( iLine >= 0 && iLine < pTextStorage->GetLineCount() )
	{
		h = pTextStorage->GetLineHeight(iLine);
	}
	if( h == 0 )
	{
		if( m_pMemo->GetUseSkin() ) h = AApplication::Get()->GetCurSkin()->m_DefaultFont.GetSize();
		else h = m_pMemo->Font.GetSize();
		h+= 4;
	}
	return h;
}

//文本选择
ACaretSeat AMemoActionImpl::GetSelStart()
{
	return m_seatSelStart;
}
ACaretSeat AMemoActionImpl::GetSelEnd()
{
	return m_seatSelEnd;
}
int AMemoActionImpl::GetSelType()
{
	return m_iSelType;
}
int AMemoActionImpl::GetSelCount()
{
	if( m_iSelType == SEL_TYPE_NONE ) return 0;
	int iCount = 0;
	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
	if( m_iSelType == SEL_TYPE_ROW )
	{
		if( m_seatSelStart.iRow == m_seatSelEnd.iRow ) iCount += m_seatSelEnd.iCol - m_seatSelStart.iCol+1;
		else
		{
			for(int i=m_seatSelStart.iRow;i<=m_seatSelEnd.iRow;i++)
			{
				if( i == m_seatSelStart.iRow )
				{
					AString s = pTextStorage->GetLine(i);
					iCount += s.Length() - m_seatSelStart.iCol;
				}
				else if( i == m_seatSelEnd.iRow )
				{
					iCount += m_seatSelEnd.iCol+1;
				}
				else
				{
					iCount += pTextStorage->GetLine(i).Length();
				}
			}
		}
	}
	else
	{
		if( m_seatSelStart.iRow == m_seatSelEnd.iRow ) iCount += m_seatSelEnd.iCol - m_seatSelStart.iCol+1;
		else
		{
			int delta = m_seatSelEnd.iCol - m_seatSelStart.iCol+1;
			for(int i=m_seatSelStart.iRow;i<=m_seatSelEnd.iRow;i++)
			{
				int iLen = pTextStorage->GetLine(i).Length();
				if( i == m_seatSelStart.iRow )
				{
					if( iLen >= m_seatSelStart.iCol+delta ) iCount += delta;
					else iCount += iLen - m_seatSelStart.iCol;
				}
				else if( i == m_seatSelEnd.iRow )
				{
					if( iLen > m_seatSelEnd.iCol ) iCount += delta;
					else iCount += iLen - m_seatSelStart.iCol;
				}
				else
				{
					if( iLen > m_seatSelEnd.iCol )
						iCount += delta;
					else
						iCount += iLen - m_seatSelStart.iCol;
				}
			}
		}
	}
	return iCount;
}
AString AMemoActionImpl::GetSelText()
{
	AString s;
	if( m_iSelType == SEL_TYPE_NONE ) return s;
	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
	AStringArray sa;
	if( m_iSelType == SEL_TYPE_ROW )
	{
		if( m_seatSelStart.iRow == m_seatSelEnd.iRow ) 
			sa.Add( pTextStorage->GetLine(m_seatSelStart.iRow).Mid( m_seatSelStart.iCol,m_seatSelEnd.iCol - m_seatSelStart.iCol ) );
		else
		{
			for(int i=m_seatSelStart.iRow;i<=m_seatSelEnd.iRow;i++)
			{
				if( i == m_seatSelStart.iRow )
				{
					AString str = pTextStorage->GetLine(i);
					sa.Add( str.Right( str.Length() - m_seatSelStart.iCol) );
				}
				else if( i == m_seatSelEnd.iRow )
				{
					sa.Add( pTextStorage->GetLine(i).Left( m_seatSelEnd.iCol) );
				}
				else
				{
					sa.Add( pTextStorage->GetLine(i) );
				}
			}
		}
	}
	else
	{
		if( m_seatSelStart.iRow == m_seatSelEnd.iRow ) 
			sa.Add( pTextStorage->GetLine(m_seatSelStart.iRow).Mid( m_seatSelStart.iCol,m_seatSelEnd.iCol - m_seatSelStart.iCol+1 ) );
		else
		{
			for(int i=m_seatSelStart.iRow;i<=m_seatSelEnd.iRow;i++)
			{
				AString s = pTextStorage->GetLine(i);
				int iLen = s.Length();

				if( iLen > m_seatSelEnd.iCol ) 
					sa.Add( s.Mid(m_seatSelStart.iCol,m_seatSelEnd.iCol-m_seatSelStart.iCol+1) );
				else 
					sa.Add( s.Mid( m_seatSelStart.iCol,iLen - m_seatSelStart.iCol) );
			}
		}
	}
	return sa.ToString(_T("\r\n"));
}

void AMemoActionImpl::SetSel(ACaretSeat seatStart,ACaretSeat seatEnd)//块选择
{
	m_seatSelStart = seatStart;
	m_seatSelEnd = seatEnd;
	m_iSelType = SEL_TYPE_ROW;
}
void AMemoActionImpl::SetColSel(ACaretSeat seatStart,ACaretSeat seatEnd)//列块选择
{
	m_seatSelStart = seatStart;
	m_seatSelEnd = seatEnd;
	m_iSelType = SEL_TYPE_COL;
}

//文本操作
void AMemoActionImpl::Copy()//拷贝当前选中文本到剪切块
{
	AClipboard* cp = new AClipboard(AApplication::Get());
	cp->SetText( GetSelText() );
	cp->Free();
}
void AMemoActionImpl::Cut()//剪切文本　
{
	AClipboard* cp = new AClipboard(AApplication::Get());
	cp->SetText( GetSelText() );
	cp->Free();

	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
	if( m_iSelType ==  SEL_TYPE_ROW ) pTextStorage->RemoveString(m_seatSelStart,m_seatSelEnd);
	else if(m_iSelType == SEL_TYPE_COL ) pTextStorage->RemoveColBlock(m_seatSelStart,m_seatSelEnd);
}
void AMemoActionImpl::Paste()//粘贴纯文本到当前光标位子
{
	AClipboard* cp = new AClipboard(AApplication::Get());
	AString s = cp->GetText();
	cp->Free();

	_delSelected();
	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
	pTextStorage->InsertString(s,m_seatCaret);
	//移动光标到字符串尾
	int iLen = s.Length();
	ACaretSeat cs = m_seatCaret;
	for(int i=0;i<iLen;i++)
	{
		if(s[i]=='\n')
		{
			cs.iCol = 0;
			cs.iRow++;
		}
		else
			cs.iCol++;
	}
	SetCaretSeat(cs);
	UpdateEditorSize();
	m_pMemo->Refresh();
}
void AMemoActionImpl::InputChar(ACHAR ch)//普通字符的输入（插入到光标处）
{
	_delSelected();
	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
	pTextStorage->InsertChar(ch,m_seatCaret);
	SetCaretSeat( ACaretSeat(m_seatCaret.iRow,m_seatCaret.iCol+1) );
	UpdateEditorSize();
	m_pMemo->Refresh();
}
void AMemoActionImpl::InputTab()
{
	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
	if( m_iSelType != SEL_TYPE_NONE )
	{
		ACaretSeat ss = m_seatSelStart;
		ACaretSeat se = m_seatSelEnd;
		for(int i=ss.iRow;i<=se.iRow;i++)
		{
			pTextStorage->InsertChar('\t',ACaretSeat(i,0));
		}
	}
	else
	{
		pTextStorage->InsertChar('\t',m_seatCaret);
		SetCaretSeat( ACaretSeat(m_seatCaret.iRow,m_seatCaret.iCol+1) );
	}
	UpdateEditorSize();
	m_pMemo->Refresh();
}
void AMemoActionImpl::InputEnter()
{
	_delSelected();
	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
	int iCount = pTextStorage->GetLineCharCount(m_seatCaret.iRow);
	AString s;
	if( iCount > m_seatCaret.iCol )
	{
		ACaretSeat seat;
		seat.iRow = m_seatCaret.iRow;
		seat.iCol = iCount -1 ;
		
		s = pTextStorage->GetString(m_seatCaret,seat);
		pTextStorage->RemoveString(m_seatCaret,seat);
	}
	pTextStorage->InsertLine(m_seatCaret.iRow+1,_T(""));
	ACaretSeat seatInsert;
	seatInsert.iRow = m_seatCaret.iRow+1;
	seatInsert.iCol = 0;
	if( s.Length() ) pTextStorage->InsertString(s,seatInsert);
	SetCaretSeat(seatInsert);
	UpdateEditorSize();
	m_pMemo->Refresh();
}
void AMemoActionImpl::InputEsc()
{
	_unselect();
	m_pMemo->Refresh();
}
void AMemoActionImpl::InputDel()
{
	if( m_iSelType != SEL_TYPE_NONE )
	{
		_delSelected();
		return;
	}
	int r = m_seatCaret.iRow;
	int c = m_seatCaret.iCol;

	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
	int iLineCount = pTextStorage->GetLineCount();

	if( r >= iLineCount ) return;

	int iCount = pTextStorage->GetLineCharCount(r);
	if( iCount > c )
	{
		pTextStorage->RemoveChar(m_seatCaret);
		UpdateEditorSize();
		m_pMemo->Refresh();
	}
	else
	{
		AString s;
		if( r < pTextStorage->GetLineCount() - 1 )
		{
			s = pTextStorage->GetLine(r+1);
			pTextStorage->RemoveLine(r+1);
			ACaretSeat seat;
			seat.iRow = r;
			seat.iCol = iCount;
			pTextStorage->InsertString(s,seat);
			UpdateEditorSize();
			m_pMemo->Refresh();
		}
	}
}
void AMemoActionImpl::InputBackspace()
{
	if( m_iSelType != SEL_TYPE_NONE )
	{
		_delSelected();
		return;
	}
	int r = m_seatCaret.iRow;
	int c = m_seatCaret.iCol;

	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
	if( r >= pTextStorage->GetLineCount() ) return;

	int iCount = pTextStorage->GetLineCharCount(r);
	if( c > 0 )
	{
		pTextStorage->RemoveChar(ACaretSeat(r,c-1));
		SetCaretSeat( ACaretSeat(m_seatCaret.iRow,m_seatCaret.iCol-1) );
		UpdateEditorSize();
		m_pMemo->Refresh();
	}
	else
	{
		if( r > 0 )
		{
			AString s;
			s = pTextStorage->GetLine(r);
			int ic = pTextStorage->GetLineCharCount(r-1);
			pTextStorage->RemoveLine(r);
			pTextStorage->InsertString(s,ACaretSeat(r-1,ic-1));
			SetCaretSeat( ACaretSeat(r-1,ic-1) );
			UpdateEditorSize();

			m_pMemo->Refresh();
		}
	}
}

void AMemoActionImpl::UpdateEditorSize()
{
	AMemoTextStorage* p = m_pMemo->GetTextStorage();
	AControl* pEditor = m_pMemo->GetEditor();
	if( pEditor == NULL ) return;

	ASize sz = p->GetEditorSize();
	sz.cx += 2;//右边多留2个像素
	sz.cy += 2;//下边也多留2个像素
	ASize sz2(pEditor->GetWidth(),pEditor->GetHeight());
	if( sz.cx != sz2.cx ) 
	{
		if( sz.cx < m_pMemo->GetWidth() ) sz.cx = m_pMemo->GetWidth();
		pEditor->SetWidth(sz.cx);
	}
	if( sz.cy != sz2.cy )
	{
		int cy = sz.cy;
		if( cy > p->GetRowSpace() ) cy -= p->GetRowSpace();
		if( cy < m_pMemo->GetHeight() ) cy = m_pMemo->GetHeight();
		pEditor->SetHeight(cy);
	}
	if( sz.cx != sz2.cx|| sz.cy != sz2.cy ) 
		m_pMemo->DoLayout();
}
void AMemoActionImpl::_unselect()
{
	if( m_iSelType != SEL_TYPE_NONE )
	{
		m_iSelType = SEL_TYPE_NONE;
		m_pMemo->Refresh();
	}
}
void AMemoActionImpl::_delSelected()
{
	if( m_iSelType != SEL_TYPE_NONE )
	{
		ACaretSeat ss = m_seatSelStart;
		ACaretSeat se = m_seatSelEnd;
		if( !(se.iRow > ss.iRow || (se.iRow == ss.iRow && se.iCol >= ss.iCol ) ) )
		{
			ss = m_seatSelEnd;
			se = m_seatSelStart;
		}
		if( !(ss.iRow == se.iRow && ss.iCol == se.iCol ) )
		{
			se = _toPrev(se);

			if( m_iSelType == SEL_TYPE_ROW )
				m_pMemo->GetTextStorage()->RemoveString(ss,se);
			else if( m_iSelType == SEL_TYPE_COL )
				m_pMemo->GetTextStorage()->RemoveColBlock(ss,se);
			SetCaretSeat(ss);
			UpdateEditorSize();
			m_pMemo->DoLayout();
			m_iSelType = SEL_TYPE_NONE;
			m_pMemo->Refresh();
		}
	}
}

ACaretSeat AMemoActionImpl::_toNext(ACaretSeat seat)
{
	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
	int iLines = pTextStorage->GetLineCount();
	if( iLines == 0 ) return seat;//空内容，位子不变
	int iChars = pTextStorage->GetLineCharCount(seat.iRow);
	if( seat.iCol + 1 > iChars )
	{
		if( seat.iRow < iLines - 1 ) 
		{
			seat.iRow ++;
			seat.iCol = 0;
		}
		else 
			;//位子不变
	}
	else
		seat.iCol++;
	return seat;
}
ACaretSeat AMemoActionImpl::_toPrev(ACaretSeat seat)
{
	AMemoTextStorage* pTextStorage = m_pMemo->GetTextStorage();
	int iLines = pTextStorage->GetLineCount();
	if( iLines == 0 ) return seat;//空内容，位子不变
	if( seat.iCol == 0  )
	{
		if( seat.iRow > 0 ) 
		{
			seat.iRow --;
			seat.iCol = pTextStorage->GetLineCharCount(seat.iRow);
		}
		else 
			;//位子不变
	}
	else
		seat.iCol--;
	return seat;
}

void AMemoActionImpl::OnDblClick(AObject* pSender,AEvent* pEvent)
{
	if( m_pMemo->GetTextStorage()->GetLineCount() < 1 ) return;
	ACaretSeat cs = GetCaretSeat();
	AString sLine = m_pMemo->GetTextStorage()->GetLine(cs.iRow);
	int iLen = sLine.Length();
	if( iLen )
	{
		ACHAR* sz = (ACHAR *)sLine.Text();
		int s=cs.iCol,e=cs.iCol;
		for(int i=cs.iCol;i>=0 && i < iLen;i--)
		{
			if( AUTIL::IsSeperatorChar(sz[i]) ) 
			{
				break;
			}
			s = i;
		}
		for(int i=cs.iCol;i < iLen;i++)
		{
			e=i;
			if( AUTIL::IsSeperatorChar(sz[i]) ) 
			{
				break;
			}
			//e=i;
		}
		SetSel(ACaretSeat(cs.iRow,s),ACaretSeat(cs.iRow,e));

	}
}