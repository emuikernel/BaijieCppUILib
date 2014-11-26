
#include "stdafx.h"

//=============================== AEdit ====================================
AEdit::AEdit(AComponent* pOwner):AControl(pOwner)
{
	m_bReadOnly = false;
	m_iCaretPos = 0;
	m_iScrollPos = 0;
	m_iCaretIndex = 0;
	m_iPadding = 2;
	m_nCaretTimerID = 0;
	m_iCaret = 0;
	m_chMaskChar = 0;
	m_iSelCount = 0;
	m_CursorType = ctIBeam;

	//Font.SetOnChange(this,(TOnControlFontChange)&AEdit::OnFontChange);

}

AEdit::~AEdit(void)
{
	if(m_nCaretTimerID)
	{
		ATimer::EndTimer(m_nCaretTimerID);
		m_nCaretTimerID = 0;
	}
}


void AEdit::OnFontChange()
{

	m_pUICore->Refresh(GetAbsPosition());
}

bool AEdit::GetReadOnly()
{
	return m_bReadOnly;
}

void AEdit::SetReadOnly(bool b)
{
	m_bReadOnly = b;
}

void AEdit::SetCaretIndex(int iPos)
{
	if( iPos < 0 ) iPos = 0;
	if( iPos > m_sText.Length() ) iPos = m_sText.Length();

	m_iCaretIndex = iPos;
	
	_updatePos();
}

int AEdit::GetCaretIndex()
{
	return m_iCaretIndex;
}

int AEdit::GetCaretPos()
{
	return m_iCaretPos;
}

int AEdit::GetScrollPos()
{
	return m_iScrollPos;
}

int AEdit::GetSelStartPos()
{
	return m_iSelStartPos;
}

int AEdit::GetSelEndPos()
{
	return m_iSelEndPos;
}

ACHAR AEdit::GetMaskChar()
{
	return m_chMaskChar;
}

void AEdit::SetMaskChar(ACHAR ch)
{
	m_chMaskChar = ch;
}

int AEdit::GetSelStart()
{
	return m_iSelStart;
}

void AEdit::SetSelStart(int iIndex)
{
	m_iSelStart = iIndex;
	if( iIndex < 0 ) m_iSelStart = 0;
	if( iIndex >= m_sText.Length() ) m_iSelStart=m_sText.Length()-1;
}

int AEdit::GetSelCount()
{
	return m_iSelCount;
}
void AEdit::SetSelCount(int iCount)
{
	m_iSelCount = iCount;
	int a = (iCount < 0 ? -iCount : iCount);
	if( a > m_sText.Length() )
	{
		a = m_sText.Length();
		m_iSelCount = (iCount < 0 ? -a : a);
	}
	_updateSelPos();
}
void AEdit::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid())
	{
		OnPaint.Call(this,pEvent);
	}
	else
	{
		AApplication::Get()->GetCurSkin()->DrawEdit(cs,this);
	}
}

void AEdit::DoMouseDown(AEvent* pEvent)
{
	if( OnMouseDown.IsValid() )
	{
		OnMouseDown.Call(this,pEvent);
		if( GetDesignMode()) return;
	}
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt )
	{
		ARect r = GetAbsPosition();
		//更新下标
		AFont f(Font);
		ACanvas* cs = GetUICore()->GetCanvas();
		if( cs == NULL ) return;
		int index = cs->Use(&f)->MeasurePosition(m_sText,evt->m_X-r.left+m_iScrollPos);
		if( index < 0 ) index = 0;
		if( index != m_iCaretIndex )
			SetCaretIndex(index);
		m_iSelStart = m_iCaretIndex;
		m_iSelCount = 0;
		
	}
}

void AEdit::DoMouseMove(AEvent* pEvent)
{
	if( OnMouseMove.IsValid() )
	{
		OnMouseMove.Call(this,pEvent);
		if( GetDesignMode()) return;
	}
	AMouseEvent* evt = dynamic_cast<AMouseEvent*>(pEvent);
	if( evt && evt->m_KeyStates.Has(ksLeftButton))
	{
		//更新下标
		ARect r = GetAbsPosition();
		AFont f(Font);
		ACanvas* cs = GetUICore()->GetCanvas();
		if( cs == NULL ) return;
		int index = cs->Use(&f)->MeasurePosition(m_sText,evt->m_X-r.left+m_iScrollPos);
		if( index < 0 ) index = 0;
		m_iSelCount = index - m_iSelStart;
		m_iCaretIndex = index;
		_updatePos();
		_updateSelPos();
		DebugStr(AString().Format(L"edit::mousemove index=%d iSelCount=%d",index,m_iSelCount));
		Refresh();
	}
}

void AEdit::DoFocus(AEvent* pEvent)
{
	m_nCaretTimerID = ATimer::StartTimer(100,this,(FOnTimerFunc)&AEdit::_onCaretTimer,this);
	GetUICore()->Refresh(GetAbsPosition());
	base_class::DoFocus(pEvent);
}

void AEdit::DoLostFocus(AEvent* pEvent)
{
	SetSelCount(0);
 	ATimer::EndTimer(m_nCaretTimerID);
	m_nCaretTimerID = 0;
	GetUICore()->Refresh(GetAbsPosition());
	base_class::DoLostFocus(pEvent);
}

void AEdit::DoKeyboard(AEvent* pEvent)
{
	AKeyEvent* evt = dynamic_cast<AKeyEvent*>(pEvent);
	if( evt == NULL ) return;

	WCHAR ch = evt->m_chKey;
	if( evt->m_Type == ketPress )
	{
		if( ch >= 0x20 )
		{
			//删除选中文本并重置光标位置
			if( m_iSelCount )
			{
				int s = m_iSelStart;
				int e = m_iSelStart+m_iSelCount;
				if( m_iSelCount < 0 ) { s = m_iSelStart+m_iSelCount; e = m_iSelStart;}
				m_sText = m_sText.Left(s)+m_sText.Right(m_sText.Length()-e);
				m_iSelCount = 0;
				m_iCaretIndex = s;
			}
			DebugStr(AString().Format(_T("\r\nOnChar:%s"),AString(ch).Text()));
			AString s1 = m_sText.Left(m_iCaretIndex);
			AString s2 = m_sText.Right(m_sText.Length()-m_iCaretIndex);
			m_sText = s1 + AString(ch)+ s2;
			int iNewIndex = m_iCaretIndex+1;
			SetCaretIndex(iNewIndex);
			GetUICore()->Refresh(GetAbsPosition());
			OnChange.Call(this,evt);
			
			
		}
	}
	else if( evt->m_Type == ketDown )
	{
		if( (ch == 'C' || ch == 'c') && evt->m_KeyStates.Has(ksCtrl) )
		{
			AClipboard* cp = new AClipboard(AApplication::Get());
			cp->SetText(GetText());
			cp->Free();
		}
		else if( (ch == 'V' || ch == 'v') && evt->m_KeyStates.Has(ksCtrl) )
		{
			if( m_iSelCount )
			{
				int s = m_iSelStart;
				int e = m_iSelStart+m_iSelCount;
				if( m_iSelCount < 0 ) { s = m_iSelStart+m_iSelCount; e = m_iSelStart;}
				m_sText = m_sText.Left(s)+m_sText.Right(m_sText.Length()-e);
				m_iSelCount = 0;
				m_iCaretIndex = s;
			}
			AClipboard* cp = new AClipboard(AApplication::Get());
			AString sText = cp->GetText();
			cp->Free();

			DebugStr(AString().Format(_T("\r\nOnChar:%s"),AString(ch).Text()));
			AString s1 = m_sText.Left(m_iCaretIndex);
			AString s2 = m_sText.Right(m_sText.Length()-m_iCaretIndex);
			m_sText = s1 + sText + s2;
			int iNewIndex = m_iCaretIndex+1;
			SetCaretIndex(iNewIndex);
			GetUICore()->Refresh(GetAbsPosition());
			OnChange.Call(this,evt);
		}
		else if( (ch == 'X' || ch == 'x') && evt->m_KeyStates.Has(ksCtrl) )
		{
			AClipboard* cp = new AClipboard(AApplication::Get());
			cp->SetText(GetText());
			cp->Free();
			SetText(_T(""));
			OnChange.Call(this,evt);
		}
		else if( ch == VK_BACK /*8*/ ) // 退格键
		{
			//删除选中文本并重置光标位置
			if( m_iSelCount )
			{
				int s = m_iSelStart;
				int e = m_iSelStart+m_iSelCount;
				if( m_iSelCount < 0 ) { s = m_iSelStart+m_iSelCount; e = m_iSelStart;}
				m_sText = m_sText.Left(s)+m_sText.Right(m_sText.Length()-e);
				m_iSelCount = 0;
				m_iCaretIndex = s;
				SetCaretIndex(s);
				OnChange.Call(this,evt);
			}
			else
			{
				if( m_iCaretIndex > 0 && m_iCaretIndex <= m_sText.Length() )
				{
					//m_sText = m_sText.Left(m_iCaretIndex)+m_sText.Right(m_sText.Length()-m_iCaretIndex-1);
					m_sText = m_sText.Left(m_iCaretIndex-1)+m_sText.Right(m_sText.Length()-m_iCaretIndex);
					int iNewIndex = m_iCaretIndex-1;
					SetCaretIndex(iNewIndex);
					GetUICore()->Refresh(GetAbsPosition());
					OnChange.Call(this,evt);
				}
			}
		}
		else if( ch == VK_TAB /*9*/ )// tab
		{
			//焦点转到下一个控件
			AContainer* pc = dynamic_cast<AContainer*>(GetParent());
			if( pc )
			{
				int index,iCount;
				iCount = pc->GetChildControls()->GetCount();
				index = pc->GetChildControls()->IndexOf(this);
				index ++;
				if( index >= iCount ) index = 0;
				AControl* pNext= dynamic_cast<AControl*>(pc->GetChildControls()->GetItem(index));
				GetUICore()->SetFocusControl(pNext);
				AEvent ev;
				this->DoLostFocus(&ev);
				pNext->DoEnter(&ev);
			}

		}
		else if( ch == VK_DELETE )
		{
			//删除选中文本并重置光标位置
			if( m_iSelCount )
			{
				int s = m_iSelStart;
				int e = m_iSelStart+m_iSelCount;
				if( m_iSelCount < 0 ) { s = m_iSelStart+m_iSelCount; e = m_iSelStart;}
				m_sText = m_sText.Left(s)+m_sText.Right(m_sText.Length()-e);
				m_iSelCount = 0;
				m_iCaretIndex = s;
				SetCaretIndex(s);
				OnChange.Call(this,evt);
			}
			else
			{
				if( m_iCaretIndex >= 0 && m_iCaretIndex < m_sText.Length() )
				{
					m_sText = m_sText.Left(m_iCaretIndex)+m_sText.Right(m_sText.Length()-m_iCaretIndex-1);
					GetUICore()->Refresh(GetAbsPosition());
					OnChange.Call(this,evt);
				}
			}
			
		}
		else if( ch == VK_INSERT )
		{
			//
			OnChange.Call(this,evt);
		}
		else if( ch == VK_LEFT )
		{
			int iOldIndex = m_iCaretIndex;
			int index = m_iCaretIndex-1;
			SetCaretIndex(index);
			if((GetKeyState(VK_SHIFT) & (1<<(sizeof(SHORT)*8-1))) != 0)
			{
				if( m_iSelCount == 0 ) m_iSelStart = iOldIndex;
				int iCount = m_iCaretIndex - m_iSelStart;
				SetSelCount(iCount);
			}
			else
				SetSelCount(0);
			GetUICore()->Refresh(GetAbsPosition());

		}
		else if( ch ==  VK_RIGHT )
		{
			int iOldIndex = m_iCaretIndex;
			int index = m_iCaretIndex+1;
			SetCaretIndex(index);
			if((GetKeyState(VK_SHIFT) & (1<<(sizeof(SHORT)*8-1))) != 0)
			{
				if( m_iSelCount == 0 ) m_iSelStart = iOldIndex;
				int iCount = m_iCaretIndex - m_iSelStart;
				SetSelCount(iCount);
			}
			else
				SetSelCount(0);
			GetUICore()->Refresh(GetAbsPosition());
		}
		else if( ch == VK_UP )
		{
			//
		}
		else if( ch == VK_DOWN )
		{
			//
		}
	}
	else if( evt->m_Type == ketUp )
	{
		//
	}
	OnKeyboard.Call(this,pEvent);
}

void AEdit::DoDblClick(AEvent* pEvent)
{
	int iLen = m_sText.Length();
	if( iLen )
	{
		ACHAR* sz = (ACHAR *)m_sText.Text();
		int s=m_iCaretIndex,e=m_iCaretIndex;
		for(int i=m_iCaretIndex;i>=0 && i < iLen;i--)
		{
			if( sz[i] == ' ' ) 
			{
				break;
			}
			s = i;
		}
		for(int i=m_iCaretIndex;i < iLen;i++)
		{
			if( sz[i] == ' ' ) 
			{
				break;
			}
			e=i;
		}
		m_iSelStart = s;
		m_iSelCount = e-s+1;
		m_iCaretIndex = e+1;
		_updatePos();
		_updateSelPos();
		Refresh();
	}
}

void AEdit::_updatePos()
{
	AFont f(Font);
	ACanvas* cs = GetUICore()->GetCanvas();
	if( cs == NULL ) return;

	AFont* pUseFont = NULL;
	if( GetUseSkin() ) 
		pUseFont =&( AApplication::Get()->GetCurSkin()->m_DefaultFont);
	else 
		pUseFont = &Font;

	ASize sz = cs->Use(pUseFont)->MeasureText(m_sText.Left(m_iCaretIndex));//光标所在处的最大宽度

	ARect r = GetControlPosition();
	r.Deflate(m_iBorderWidth+m_iPadding,m_iBorderWidth+m_iPadding);

	int iEditWidth = r.GetWidth();
	m_iCaretPos = sz.cx;
	if( m_iCaretPos + m_iScrollPos < 0 )
	{
		//向左边溢界，则需要调整滚动距离
		m_iScrollPos = 0 - m_iCaretPos;
	}
	else if( m_iCaretPos + m_iScrollPos > iEditWidth )
	{
		//向右溢界，也需要调整滚动距离
		m_iScrollPos = iEditWidth - m_iCaretPos;
	}
}

void AEdit::_updateSelPos()
{
	if( m_iSelCount == 0 ) return;

	ACanvas* cs = GetUICore()->GetCanvas();
	if( cs == NULL ) return;

	AFont* pUseFont = NULL;
	if( GetUseSkin() ) 
		pUseFont =&( AApplication::Get()->GetCurSkin()->m_DefaultFont);
	else 
		pUseFont = &Font;
	
	ASize sz = cs->Use(pUseFont)->MeasureText(m_sText.Left(m_iSelStart));
	ASize sz2 = cs->Use(pUseFont)->MeasureText(m_sText.Left(m_iSelStart+m_iSelCount));

	m_iSelStartPos = sz.cx;
	m_iSelEndPos = sz2.cx;
}

void AEdit::_onCaretTimer(AObject* pAsso,int nTimerID)
{
	if( GetUICore()->GetFocusControl() != (AControl*)this )
		return;
	m_iCaret ++;
	if( m_iCaret > 10000 ) m_iCaret = 0;
	//由于不经过WM_TIMER 事件，因此也不经过核心事件处理，所以需要手工触发GetUICore()->Refresh(GetAbsPosition());
	ARect r = GetAbsPosition();
	::InvalidateRect(GetUICore()->GetHandle(),&r,FALSE);
	::UpdateWindow(GetUICore()->GetHandle());
}

bool AEdit::ShowCaret()
{
	int imod = m_iCaret % 10;
	return (imod == 0 || imod == 1 || imod == 2 ) ? false : true;
}

void AEdit::CreateChildren(AMLBody* pNode)
{
	//base_class::CreateChildren(pNode);
	//DoLayout();
}

void AEdit::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);


	PROP_SET(OnKeyDown);
	PROP_SET(OnKeyPress);
	PROP_SET(OnKeyUp);
	PROP_SET(OnChange);
}

bool AEdit::CanFocus()
{
	return true;
}
