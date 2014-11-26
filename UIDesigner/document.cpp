//---------------------------------------------------------------------------

#include "stdafx.h"
#include "document.h"

//---------------------------------------------------------------------------


IDocument::IDocument():afc_object_base()
{
	m_bModified = false;
	m_pForm	=	NULL;
	m_pNode	=	NULL;

}
IDocument::~IDocument()
{
        //
}

EString IDocument::GetFileName()
{
	return m_sFileName;
}

void IDocument::SetFileName(const EString& s)
{
	m_sFileName = s;
}

void IDocument::SetModified(bool b)
{
	m_bModified = b;
}

bool IDocument::GetModified()
{
        return m_bModified;
}
void IDocument::SetForm(AObject* p)
{
	m_pForm = p;
}
AObject* IDocument::GetForm()
{
	return m_pForm;
}
void IDocument::SetNode(ATreeViewItem* p)
{
	m_pNode = p;
}
ATreeViewItem* IDocument::GetNode()
{
        return m_pNode;
}

ICodeDocument::ICodeDocument():IDocument()
{
	//
}

ICodeDocument::~ICodeDocument()
{
	//
}

void	ICodeDocument::LoadFromFile(const EString& sFile)
{
	AFile f;
	if( f.Open(sFile,afcRead) )
	{
		xmemory ms;
		int iSize = f.Length() + 2;
		ms.SetSize(iSize);
		BYTE* p = ms.GetData();
		f.Read(p,iSize-2);
		p[iSize-1] = 0;
		p[iSize-2] = 0;
		if( p[0] == 0xFF && p[1] == 0xFE )
		{
			p += 2;
			m_sCode = (wchar_t *)p;
		}
		m_sFileName = sFile;
	}
	//else
	//	throw Exception(L"打开文件失败！");
}
void	ICodeDocument::SaveToFile(const EString& sFile)
{
	XFile f;
	if( f.Create(sFile,true) )
	{
		BYTE x[]={0xFF,0xFE};
		f.Write(x,2);
		f.Write((BYTE *)m_sCode.Text(),m_sCode.Length()*sizeof(wchar_t));
		m_bModified = false;
	}
	else
		throw AException(L"写文件失败！");
}

void	ICodeDocument::SetCode(const EString& s)
{
	m_sCode = s;
}
EString ICodeDocument::GetCode()
{
	if( GetForm() == NULL ) LoadFromFile(m_sFileName);
	return m_sCode;
}
