#include "StdAfx.h"

APageControl::APageControl(AComponent* pOwner):
	AContainer(pOwner),
	m_iActiveIndex(-1)
{
	m_bEnableScrollTop = false;
	m_bEnableScroll = false;
	m_pButtonPrev = new AButton(this);
	m_pButtonNext = new AButton(this);
	m_pButtonFirst = new AButton(this);
	m_pButtonLast = new AButton(this);
	m_iShift = 0;
}

APageControl::~APageControl()
{

}

void APageControl::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	if( OnPaint.IsValid() )
	{
		OnPaint.Call(this,pEvent);
	}
	else
		AApplication::Get()->GetCurSkin()->DrawPageControl(cs,this);

	AContainer::DoPaintChildren(pEvent);
}

void APageControl::DoLayout()
{
	if( !m_pUICore->GetLayoutEnabled() ) return;
	ACanvas* cs = GetUICore()->GetCanvas();

	int iLeft = 0;
	int iHeight = 24;
	int iButtonSize=12;//12 * 12大小的小按钮
	int iReserve=0;
	if( m_bEnableScroll ) iReserve += 30;
	if( m_bEnableScrollTop ) iReserve += 30;

	for(int i=0;i<GetChildControls()->GetCount();i++)
	{
		APageSheet* pSheet = dynamic_cast<APageSheet*>( GetChildControls()->GetItem(i));
		if( pSheet == NULL ) continue;

		SIZE sz;
		sz.cx = 40;
		sz.cy = 20;
		AFont font(pSheet->Font);
		if( cs )
		{
			if( pSheet->GetButtonText()->GetUseSkin() ) cs->Use(&(AApplication::Get()->GetCurSkin()->m_DefaultFont));
			else cs->Use(&(pSheet->GetButtonText()->Font));
			sz = cs->MeasureText(pSheet->m_pButtonText->GetText());
		}
		sz.cx += 20;
		sz.cy += 14;

		pSheet->m_pTabButton->SetLeft(iLeft);
		pSheet->m_pTabButton->SetTop(0);
		pSheet->m_pTabButton->SetWidth(sz.cx);
		pSheet->m_pTabButton->SetHeight(iHeight);
		if( iLeft + sz.cx > m_iWidth - iReserve  ) pSheet->m_pTabButton->SetWidth(0);//超过范围的不显示

		iLeft += sz.cx;

		pSheet->SetLeft(m_iBorderWidth+2);
		pSheet->SetTop(iHeight+ (int)m_iBorderWidth+2);
		pSheet->SetWidth(GetWidth() - (int)m_iBorderWidth * 2 - 4);
		pSheet->SetHeight(GetHeight() - (int)m_iBorderWidth*2- iHeight - 4);
		pSheet->DoLayout();

	}
}

APageSheet* APageControl::GetActiveSheet()
{
	if( m_iActiveIndex < 0 || m_iActiveIndex >= GetChildControls()->GetCount() ) return NULL;
	int ix=0;
	for(int i=0;i<GetChildControls()->GetCount();i++)
	{
		APageSheet* ps = dynamic_cast<APageSheet *>(GetChildControls()->GetItem(i));
		if( ps )
		{
			if( ix == m_iActiveIndex ) return ps;
			ix ++;
		}
	}
	return NULL;
}
void APageControl::SetActiveSheet(APageSheet* pSheet)
{
	int ix = 0;
	for(int i=0;i<GetChildControls()->GetCount();i++)
	{
		APageSheet* ps = dynamic_cast<APageSheet *>(GetChildControls()->GetItem(i));
		if( ps )
		{
			if( ps == pSheet )
			{
				ps->SetVisible(true);
				m_iActiveIndex = ix;
			}
			else ps->SetVisible(false);
			ix ++;
		}

	}
	AEvent evt;
	OnActiveChange.Call(this,&evt);
	DoLayout();
}

int APageControl::GetActiveSheetIndex()
{
	return m_iActiveIndex;
}
void APageControl::SetActiveSheetIndex(int iIndex)
{
	if( iIndex >= -1 && iIndex < GetChildControls()->GetCount() )
	{
		m_iActiveIndex = iIndex;
		int ix = 0;
		for(int i=0;i<GetChildControls()->GetCount();i++)
		{
			APageSheet* ps = dynamic_cast<APageSheet *>(GetChildControls()->GetItem(i));
			if( ps == NULL ) continue;

			if( ix == iIndex )
			{
				ps->SetVisible(true);
			}
			else ps->SetVisible(false);
			ix ++;
		}
	}
	else
		m_iActiveIndex = -1;
	AEvent evt;
	OnActiveChange.Call(this,&evt);
	DoLayout();
}

APageSheet* APageControl::AddSheet()
{
	APageSheet* pSheet = new APageSheet(this);
	SetActiveSheet(pSheet);
	DoLayout();
	return pSheet;
}

void APageControl::RemoveSheet(APageSheet* pSheet)
{
	APageSheet* p = GetActiveSheet();
	if( p == pSheet ) SetActiveSheetIndex(-1);
	if( pSheet == NULL ) return;
	//GetChildControls()->Delete(pSheet->GetTabButton());
	//GetChildControls()->Delete(pSheet);
	pSheet->GetTabButton()->Destroy();
	pSheet->m_pTabButton = NULL;
	pSheet->Destroy();
	DoLayout();
}

void APageControl::CreateChildren(AMLBody* pNode)
{
	for(int i=0;i<pNode->GetChildCount();i++)
	{
		AMLBody* pChildBody = pNode->GetChild(i);
		if( pChildBody->GetProp(_T("type")) != _T("PageSheet") ) continue;
		APageSheet* pSheet = this->AddSheet();
		pSheet->SetPropFromNode(pChildBody);

		AMLBody* pChildren = pChildBody->FindBody(_T("children"));
		if( pChildren )
		{
			pSheet->CreateChildren(pChildren);
		}
	}
}

void APageControl::SetPropFromNode(AMLBody* pNode)
{
	base_class::SetPropFromNode(pNode);
	if( pNode->HasProp(_T("asi")) ) SetActiveSheetIndex( AIntVal(pNode->GetProp(_T("asi"))) );//ActiveSheetIndex
	PROP_SET(OnActiveChange);
}

bool APageControl::GetEnableScroll()
{
	return m_bEnableScroll;
}
void APageControl::SetEnableScroll(bool v)
{
	m_bEnableScroll = v;
}

bool APageControl::GetEnableScrollTop()
{
	return m_bEnableScrollTop;
}
void APageControl::SetEnableScrollTop(bool v)
{
	m_bEnableScrollTop = v;
}
void APageControl::ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESGet(sPath,pValue);
	if( sPath == _T("ActiveIndex") ) pValue->SetString( AStringVal(GetActiveSheetIndex()) );
	else if( sPath == _T("OnActiveChange") ) pValue->SetString( OnActiveChange.GetMCF() );
}

void APageControl::ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue)
{
	if( pValue == NULL ) return;
	base_class::ESSet(sPath,pValue);
	if( sPath == _T("ActiveIndex") ) SetActiveSheetIndex( AIntVal(pValue->GetString()));
	else if( sPath == _T("OnActiveChange") ) OnActiveChange.SetMCF( pValue->GetString() ); 
}
void APageControl::ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue)
{

}