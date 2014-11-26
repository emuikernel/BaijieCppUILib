
#include "stdafx.h"
#include "GFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GCanvasFont::GCanvasFont(HDC hDC)
	:AObject(),m_hDC(hDC),m_hOldFont(NULL),m_hFont(NULL)
{
	m_bDirty	=	FALSE;
	m_bColorDirty	=	FALSE;
}

GCanvasFont::GCanvasFont()
{
	m_hDC		=	NULL;
	m_hOldFont	=	NULL;
	m_hFont		=	NULL;
	m_bDirty	=	FALSE;
	m_bColorDirty	=	FALSE;
}

GCanvasFont::~GCanvasFont()
{
}



HFONT GCanvasFont::GetHandle()
{
	return m_hFont;
}
void GCanvasFont::Do_Clean()
{
	if( m_hDC != NULL && m_hOldFont != NULL ) 
	{
		::SelectObject(m_hDC,m_hOldFont);
		::DeleteObject(m_hFont);
	}
}

AString GCanvasFont::GetName()
{
	GetInfor();
	return m_sName;
}

void	GCanvasFont::SetName(LPCTSTR lpszName)
{
	GetInfor();
	m_sName		=	lpszName;
	m_bDirty	=	TRUE;
}

BYTE GCanvasFont::GetCharset()
{
	GetInfor();
	return m_Charset;
}

void GCanvasFont::SetCharset(BYTE byChar)
{
	GetInfor();
	m_Charset	=	byChar;
	m_bDirty	=	TRUE;
}

bool GCanvasFont::IsItalic()
{
	GetInfor();
	return m_bItalic;
}

void GCanvasFont::SetItalic(bool b)
{
	GetInfor();
	m_bItalic	=	b;
	m_bDirty	=	TRUE;
}

bool GCanvasFont::IsUnderline()
{
	GetInfor();
	return m_bUnderline;
}

void GCanvasFont::SetUnderline(bool b)
{
	GetInfor();
	m_bUnderline	=	b;
	m_bDirty	=	TRUE;
}

bool GCanvasFont::IsStrikeOut()
{
	GetInfor();
	return m_bStrikeOut;
}

void GCanvasFont::SetStrikeOut(bool b)
{
	GetInfor();
	m_bStrikeOut	=	b;
	m_bDirty	=	TRUE;
}

int GCanvasFont::GetBold()
{
	GetInfor();
	return m_iBold;
}

void GCanvasFont::SetBold(int iBold)
{
	GetInfor();
	m_iBold		=	iBold;
	m_bDirty	=	TRUE;
}

long GCanvasFont::GetHeight()
{
	GetInfor();
	return m_lHeight;
}

void GCanvasFont::SetHeight(long lHeight)
{
	GetInfor();
	m_lHeight	=	lHeight;
	m_bDirty	=	TRUE;
}

long GCanvasFont::GetWidth()
{
	GetInfor();
	return m_lWidth;
}

void GCanvasFont::SetWidth(long lWidth)
{
	GetInfor();
	m_lWidth	=	lWidth;
	m_bDirty	=	TRUE;
}

long GCanvasFont::GetEscapement()
{
	GetInfor();
	return m_lEscapement;
}

void GCanvasFont::SetEscapement(long lEsc)
{
	GetInfor();
	m_lEscapement	=	lEsc;
	m_bDirty	=	TRUE;
}

long GCanvasFont::GetOrientation()
{
	GetInfor();
	return m_lOrientation;
}

void GCanvasFont::SetOrientation(long lOri)
{
	GetInfor();
	m_lOrientation	=	lOri;
	m_bDirty	=	TRUE;
}

void GCanvasFont::GetInfor()
{
	if( m_hFont != NULL ) return;
	HFONT hFont = (HFONT)::GetCurrentObject(m_hDC,OBJ_FONT);
	LOGFONT	lf;
	::GetObject(hFont,sizeof(lf),&lf);
	m_sName		=	lf.lfFaceName;
	m_Charset	=	lf.lfCharSet;
	m_bItalic	=	lf.lfItalic == 1 ? true : false;
	m_bUnderline	=	lf.lfUnderline == 1 ? true : false;
	m_bStrikeOut	=	lf.lfStrikeOut == 1 ? true : false;
	m_iBold		=	lf.lfWeight;
	m_lHeight	=	lf.lfHeight;
	m_lWidth	=	lf.lfWidth;
	m_lEscapement	=	lf.lfEscapement;
	m_lOrientation	=	lf.lfOrientation;
	m_Color		=	::GetTextColor(m_hDC);

	m_hFont = ::CreateFontIndirect(&lf);
	m_hOldFont = (HFONT)::SelectObject(m_hDC,m_hFont);
}

AColor GCanvasFont::GetColor()
{
	GetInfor();
	return m_Color;
}

void GCanvasFont::SetColor(AColor cr)
{
	GetInfor();
	m_Color		=	cr;
	m_bDirty	=	TRUE;
	m_bColorDirty	=	TRUE;
}

void GCanvasFont::Flush( )
{
	if( m_bDirty == TRUE )
	{
		m_bDirty	=	FALSE;
		GetInfor();
		if( m_hFont != NULL )
		{
			if( m_hOldFont != NULL  )
			{
				::SelectObject(m_hDC,m_hOldFont);
				m_hOldFont = NULL;
			}
			::DeleteObject(m_hFont);

			LOGFONT lf;
			lf.lfCharSet	=	m_Charset;
			lf.lfItalic	=	m_bItalic;
			lf.lfStrikeOut	=	m_bStrikeOut;
			lf.lfUnderline	=	m_bUnderline;
			lf.lfWeight	=	m_iBold;
			lf.lfEscapement	=	m_lEscapement;
			lf.lfHeight	=	m_lHeight;
			lf.lfOrientation=	m_lOrientation;
			lf.lfWidth	=	m_lWidth;
			_tcsncpy(lf.lfFaceName,m_sName,LF_FACESIZE - 1 );
			m_hFont = ::CreateFontIndirect(&lf);
			m_hOldFont = (HFONT)::SelectObject(m_hDC,m_hFont);

			if( m_bColorDirty == TRUE )
			{
				m_bColorDirty	=	FALSE;
				::SetTextColor(m_hDC,m_Color);
			}
		}
	}
}

