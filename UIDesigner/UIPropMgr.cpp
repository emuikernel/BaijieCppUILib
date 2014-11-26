//---------------------------------------------------------------------------

#include "stdafx.h"
#include "UIPropMgr.h"
#include "UIDoc.h"

//---------------------------------------------------------------------------

static void ParseChoices(CONFIG_BODY& pBody,APropCombo* pItem)
{
	if( &pBody != NULL )
	{
		AStringMapEnumerator  em(&(pBody->m_aProp));
		while( em.Next() ) 
		{
			AString sKey,sDisp;
			sKey = em.Index();
			sDisp = (ACHAR *)em.Cur();
			if( sDisp == _T("") ) sDisp = sKey;
			pItem->AddChoice(sKey,sDisp);
		}
	}
}

static void ParseColor(const EString& sColor,BYTE& r,BYTE& g,BYTE& b)
{
        EString s;
        s = sColor;
        int iPos = s.Find(_ES(","));
        if( iPos > -1 )
        {
                r = EIntVal( s.Left(iPos) );
                s = s.Right(s.Length()-iPos-1);

                iPos = s.Find(_ES(","));
                if( iPos > -1 )
                {
                        g = EIntVal( s.Left(iPos) );
                        s = s.Right(s.Length()-iPos-1);

                        b = EIntVal( s );
                }
        }
}

CUIPropMgr::CUIPropMgr(CUIDoc* pDoc,APropEdit* pEditor)
	:AObject(),
	m_pDoc(pDoc),m_pEditor(pEditor)
{
}

CUIPropMgr::~CUIPropMgr()
{
}

void CUIPropMgr::ShowProp(const afc_object<CControl>& pControl)
{
	//OutputDebugString(L"[Studio]ResetAll......");
	//time_counter tc1;
	m_pEditor->ResetAll();
	//tc1.Print(L"ResetAll()");
	//tc1.Reset();
	m_pEditor->SetOnChange(this, (FOnPropChange)&CUIPropMgr::PropOnChange );
	m_pCurControl = pControl;

	if( &pControl != NULL && pControl->m_pDefNode != ANULL )
	{
		//先显示类型
		afc_object<TPropString> pItem(new TPropString(L"_控件类型_",
								L"类型",
								pControl->m_pDefNode->GetProp(_ES("displayname"))
								//pControl->GetType()
								));
		pItem->m_bReadOnly = true;
		m_pEditor->AddProp( (APropItem *)&pItem);

                CONFIG_BODY pPropsNode;
                pPropsNode = pControl->m_pDefNode->FindBody(_ES("props"));
                if( pPropsNode != ANULL )
                {
                        for(int i=0;i<pPropsNode->GetChildCount();i++)
                        {
                                CONFIG_BODY px;
                                px = pPropsNode->GetChild(i);
                                ShowProp(px);
                        }
                }
	}
	//tc1.Print(L"ShowProp.....");
	//tc1.Reset();
	m_pEditor->Render();
	//tc1.Print(L"Render...");
	//tc1.Reset();
	m_pEditor->DoLayout();
	//m_pEditor->Refresh();
	//tc1.Print(L"DoLayout...");
}

void CUIPropMgr::ShowProp(const CONFIG_BODY& pProp,TPropGroup* pGroup)
{
        EString sType;
	sType = pProp->GetProp(_ES("type"));


        EString sKey,sDisp,sDefault;
        if( pGroup ) sKey = pGroup->m_sKey+_ES("/");
        sKey += pProp->GetProp(_ES("name"));
        sDisp = pProp->GetProp(_ES("displayname"));
        sDefault = (*m_pCurControl)[sKey]->AsString();

        if( sType == _ES("string") )
        {
                afc_object<TPropString> pItem(new TPropString(sKey,
                                                                sDisp,
                                                                sDefault));
                if( pGroup ) pGroup->AddProp( pItem.get() );
                else m_pEditor->AddProp(&pItem);
        }

        else if( sType == _ES("int") )
        {

                afc_object<TPropInt> pItem(new TPropInt(sKey,
                                                                sDisp,
                                                                EIntVal(sDefault)));
                if( pGroup ) pGroup->AddProp( &pItem );
                else m_pEditor->AddProp( &pItem);
        }
        else if( sType == _ES("combo") )
        {
                afc_object<TPropCombo> pItem(new TPropCombo(sKey,sDisp,EString()));
	        if( pGroup ) pGroup->AddProp( (APropItem *)&pItem );
		else m_pEditor->AddProp( &pItem);
		CONFIG_BODY pChoicesBody;
		pChoicesBody = pProp->FindBody(_ES("choices"));
		ParseChoices(pChoicesBody,&pItem);
                pItem->m_Val = sDefault;
                pItem->m_bCanEdit = (pProp->GetProp(_ES("canedit")) == _ES("1") ? true : false);
        }
        else if( sType == _ES("color") )
        {
                BVColor cl = (*m_pCurControl)[sKey]->AsColor();

                afc_object<APropColor> pItem(new APropColor(sKey,sDisp,cl));
                if( pGroup ) pGroup->AddProp( (APropItem *)&pItem );
                else m_pEditor->AddProp( (APropItem *)&pItem);

                //
        }
        else if( sType == _ES("data") )
        {
                afc_object<APropData> pItem(new APropData(sKey,sDisp));
	        if( pGroup ) pGroup->AddProp( (APropItem *)&pItem );
	        else m_pEditor->AddProp( (APropItem *)&pItem);
        }
        else if( sType == _ES("bool") )
        {
                afc_object<APropBool> pItem(new APropBool(sKey,sDisp,EIntVal(sDefault) == 1 ? true : false) );
	        if( pGroup ) pGroup->AddProp( (APropItem *)&pItem );
	        else m_pEditor->AddProp( (APropItem *)&pItem);
	}
	else
	{
		CONFIG_BODY ps;
		ps = pProp->FindBody(L"props");
		if(&ps != NULL)
		{
			afc_object<TPropGroup> pItem(new TPropGroup());
			pItem->m_sKey = sKey;
			pItem->m_sDisplayName = sDisp;
			pItem->m_bExpanded = (pProp->GetProp(_ES("expand"))==_ES("1") ? true : false);
			if( pGroup ) pGroup->AddProp( (APropItem *)&pItem );
			else m_pEditor->AddProp( (APropItem *)&pItem );

			for(int j=0;j<ps->GetChildCount();j++)
			{
				CONFIG_BODY pChildrenProp;
				pChildrenProp = ps->GetChild(j);
				ShowProp( pChildrenProp,&pItem );
			}
                }
	}
}


void CUIPropMgr::PropOnChange(TPropItem* pItem)
{
	if( &m_pCurControl == NULL || pItem == NULL ) return;


	if( dynamic_cast<APropInt*>(pItem) )
	{
		int iOld = (*m_pCurControl)[pItem->m_sKey]->AsInt();
		(*m_pCurControl)[pItem->m_sKey]->AsInt(dynamic_cast<APropInt*>(pItem)->m_Val);
		if( iOld != dynamic_cast<APropInt*>(pItem)->m_Val) pItem->m_bModified = true;
	}
	else if( dynamic_cast<APropString*>(pItem) )
	{
		EString sOld = (*m_pCurControl)[pItem->m_sKey]->AsString();
		(*m_pCurControl)[pItem->m_sKey]->AsString(dynamic_cast<APropString*>(pItem)->m_Val);
		if( sOld != dynamic_cast<APropString*>(pItem)->m_Val ) pItem->m_bModified = true;
	}
	else if( dynamic_cast<APropCombo*>(pItem) )
	{
		APropCombo* ptr = dynamic_cast<APropCombo*>(pItem);
		if( ptr->m_IntVal == -1 )
		{
			EString sOld = (*m_pCurControl)[pItem->m_sKey]->AsString();
			(*m_pCurControl)[pItem->m_sKey]->AsString(ptr->m_Val);
			if( sOld != ptr->m_Val ) pItem->m_bModified = true;
		}
		else
		{
			int iOld = (*m_pCurControl)[pItem->m_sKey]->AsInt();
			(*m_pCurControl)[pItem->m_sKey]->AsString( ptr->GetKey( ptr->m_IntVal ) );
			//for debug
			//AString sDisp = (*m_pCurControl)[pItem->m_sKey]->AsString();
			if( iOld != ptr->m_IntVal ) pItem->m_bModified = true;
		}
	}
	else if( dynamic_cast<APropColor*>(pItem) )
	{
		APropColor* ptr = dynamic_cast<APropColor*>(pItem);
		AColor cr = ptr->GetValue();
		(*m_pCurControl)[pItem->m_sKey]->AsColor(cr);
		AColor cr2 = (*m_pCurControl)[pItem->m_sKey]->AsColor();
	}
	else if( dynamic_cast<APropData*>(pItem) )
	{
		//do nothing now
	}
	else if( dynamic_cast<APropBool*>(pItem) )
	{
		int iOld =(*m_pCurControl)[pItem->m_sKey]->AsInt();
		(*m_pCurControl)[pItem->m_sKey]->AsInt(dynamic_cast<APropBool*>(pItem)->m_Val ? 1 : 0);
		if( iOld != (dynamic_cast<APropBool*>(pItem)->m_Val ? 1 : 0) ) pItem->m_bModified = true;
	}
	m_pDoc->m_OnRefresh.Call(this,NULL);
	m_pDoc->SetModified( true );
}


