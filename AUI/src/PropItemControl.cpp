#include "stdafx.h"
#include "PropItemControl.h"

#define INDENT_SIZE	11
#define PROPITEM_PLUS	9

AExpandButton::AExpandButton(AComponent* pOwner) : AButton(pOwner)
{
	//
}

AExpandButton::~AExpandButton(void)
{
}

void AExpandButton::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;

	AContainer* pLeft = dynamic_cast<AContainer*>(GetParent());
	if( pLeft == NULL ) return;
	APropItemControl* pItemControl = dynamic_cast<APropItemControl*>(pLeft);
	if( pItemControl == NULL ) return;

	APropGroup* pGroup = dynamic_cast<APropGroup *>(pItemControl->m_pPropItem);
	if( pGroup == NULL ) return;

	ARect r = GetAbsPosition();

	ARect rx;
	rx.left = r.left + (r.GetWidth() - PROPITEM_PLUS)/2;
	rx.top = r.top + (r.GetHeight()-PROPITEM_PLUS)/2;
	rx.right = rx.left + PROPITEM_PLUS;
	rx.bottom = rx.top + PROPITEM_PLUS;

	APen p;
	p.SetColor(AColor(255,0,0,0));
	ASolidBrush b;
	b.SetColor(WHITE_COLOR);
	cs->Use(&b)->FillRect(r);
	cs->Use(&p)->DrawRect(r);
	cs->Use(&p)->DrawLine(rx.left,rx.top+PROPITEM_PLUS/2,rx.right,rx.top+PROPITEM_PLUS/2);

	if( pGroup->m_bExpanded == false )
		cs->Use(&p)->DrawLine(rx.left+PROPITEM_PLUS/2,rx.top,rx.left+PROPITEM_PLUS/2,rx.bottom);
}

APropItemControl::APropItemControl(AComponent* pOwner) : AContainer(pOwner)
{
	m_iBorderWidth = 1;
	m_pPropItem = NULL;
	m_pLeft = new AContainer(this);
	m_pLeft->SetAlign(alLeft);
	m_pLeft->SetWidth(1);
	m_pRight = new AContainer(this);
	m_pRight->SetAlign(alClient);
}

APropItemControl::~APropItemControl(void)
{
}

void APropItemControl::DoPaint(ACanvas* cs,AEvent* pEvent)
{
	APaintEvent* evt = dynamic_cast<APaintEvent*>(pEvent);
	if( evt == NULL ) return;
	ARect r = GetAbsPosition();
	if( m_pPropItem )
	{
		ACanvas *cs = m_pUICore->GetCanvas();

		ASolidBrush b;
		b.SetColor(LIGHT_COLOR);
		cs->Use(&b)->FillRect(r);


		ARect r2 = r;
		r2.left = r2.left + INDENT_SIZE * (1 + m_pPropItem->m_iLevel);
		r2.bottom = r2.bottom - 1;
		ASolidBrush b2;
		b2.SetColor(WHITE_COLOR);
		cs->Use(&b2)->FillRect(r2);
	}

	AContainer::DoPaintChildren(pEvent);
}
