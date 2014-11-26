#include "StdAfx.h"
#include "UIXMLGen.h"


UIXMLGen::UIXMLGen(AMLBody* pRoot):m_pRoot(pRoot)
{
}


UIXMLGen::~UIXMLGen(void)
{
}

afc_object<AMLBody> UIXMLGen::ToNode()
{
	return GenControl(m_pRoot);
}
AString UIXMLGen::ToString()
{
	afc_object<AMLBody> p;
	p = GenControl(m_pRoot);
	return p->ToXML();
}

afc_object<AMLBody> UIXMLGen::GenControl(AMLBody* pRoot)
{
	AString sType = pRoot->GetName();

	afc_object<AMLBody> pn(new AMLBody());
	pn->SetName(_T("c"));
	pn->SetProp(_T("type"),sType);

	//AControl基本属性
	AStringMapEnumerator em(&(pRoot->m_aProp));
	while( em.Next() )
	{
		pn->SetProp(em.Index(),(ACHAR *)em.Cur());
	}

	//字体
	afc_object<AMLBody> pnFind;
	pnFind = pRoot->FindBody(_T("font"));
	if( pnFind != ANULL )
	{
		pn->AddBody(&pnFind);
	}

	//========== 特定控件的特有属性 ====================
	if( sType == _T("Form") )
	{
		//fix me later : 似乎已经没什么好处理的了
	}
	else if( sType == _T("ScrollBar") )
	{
		AString s = pn->GetProp(_T("aim"));
		if( s == _T("垂直") ) pn->SetProp(_T("type"),_T("VertScrollBar"));
		else pn->SetProp(_T("type"),_T("HorzScrollBar"));
	}
	else if( sType == _T("Slide") )
	{
		AString s = pn->GetProp(_T("aim"));
		if( s == _T("垂直") ) pn->SetProp(_T("type"),_T("VertSlide"));
		else pn->SetProp(_T("type"),_T("HorzSlide"));
	}

	//---- 处理子控件
	pnFind = pRoot->FindBody(_T("@"));
	if( pnFind != ANULL )
	{
		afc_object<AMLBody> pChildren(new AMLBody());
		
		pChildren->SetName(_T("children"));

		for(int i=0;i<pnFind->GetChildCount();i++)
		{
			afc_object<AMLBody> pChild;
			pChild = pnFind->GetChild(i);
			if( pChild->GetName() == _T("Array") )
			{
				AMLBody* pa = pChild->FindBody(_T("@"));
				if( pa != NULL)
				{
					afc_object<AMLBody> pc(new AMLBody());
					pc->SetName(_T("children"));
					for(int j=0;j<pa->GetChildCount();j++)
					{
						afc_object<AMLBody> pChild2;
						pChild2 = pa->GetChild(j);
						afc_object<AMLBody> pnTmp;
						pnTmp = GenControl(&pChild2);
						pc->AddBody(&pnTmp);
					}
					if( pc->GetChildCount() > 0 )
						pn->AddBody( &pc );
				}
				
			}
			else
			{
				afc_object<AMLBody> pnTmp;
				pnTmp = GenControl(&pChild);
				pChildren->AddBody(&pnTmp);
			}
		}
		if( pChildren->GetChildCount() > 0 )
			pn->AddBody( &pChildren );
		
	}
	return pn;
}