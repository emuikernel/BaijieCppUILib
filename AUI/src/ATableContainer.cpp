#include "stdafx.h"
#include "ATableContainer.h"

ATableContainer::ATableContainer(AComponent* pOwner):base_class(pOwner)
{
	m_iColCount = 1;
	m_iRowCount = 1;
}

ATableContainer::~ATableContainer()
{

}

void ATableContainer::DoLayout()
{
	
	double dw = (double)GetWidth() / (double)m_iColCount;
	double dh = (double)GetHeight() / (double)m_iRowCount;
	int iCount = m_iRowCount * m_iColCount;
	int iControls = GetChildControls()->GetCount();
	for(int i=0;i<iControls&&i<iCount;i++)
	{
		int iRow = i/m_iColCount;
		int iCol = i % m_iColCount;
		AControl* p = (AControl*)GetChildControls()->GetItem(i);
		if( p->GetVisible() == false ) continue;
		if( p->GetAlign() != alNone ) continue;
		if( p->GetFloatType() != ftFloatNone ) continue;
		p->SetLeft(iCol*dw);
		p->SetTop(iRow*dh);
		p->SetWidth(dw);
		p->SetHeight(dh);
	}
	base_class::DoLayout();
}


int ATableContainer::GetColCount()
{
	return m_iColCount;
}

void ATableContainer::SetColCount(int v)
{
	if( v < 1 ) v = 1;
	m_iColCount = v;
}

int ATableContainer::GetRowCount()
{
	return m_iRowCount;
}

void ATableContainer::SetRowCount(int v)
{
	if( v < 1 ) v = 1;
	m_iRowCount = v;
}

void ATableContainer::CreateChildren(AMLBody* pNode)
{
	base_class::CreateChildren(pNode);
}

void ATableContainer::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
	if( pNode->HasProp(_T("rc")) ) SetRowCount(AIntVal(pNode->GetProp(_T("rc"))));
	if( pNode->HasProp(_T("cc"))) SetColCount(AIntVal(pNode->GetProp(_T("cc"))));
	
}

void ATableContainer::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == _T("RowCount") ) pValue->SetInt( GetRowCount() );
	else if( sPath == _T("ColCount") ) pValue->SetInt( GetColCount() );
}

void ATableContainer::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("RowCount") ) SetRowCount( pValue->GetInt() );
	else if( sPath == _T("ColCount") ) SetColCount( pValue->GetInt());
}
void ATableContainer::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{

}