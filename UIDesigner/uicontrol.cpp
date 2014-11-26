//---------------------------------------------------------------------------


#include "stdafx.h"
#include "uicontrol.h"

//---------------------------------------------------------------------------


#define SETPROP(x,y)	SetProp(_ES(x),_ES(y))


static void __DefProp2Node(const CONFIG_BODY& pDefProp,CONFIG_BODY& pNode)
{
	CONFIG_BODY ps;
	ps = pDefProp->FindBody(L"props");
	if( &ps != NULL )
	{
		CONFIG_BODY x;
		x = new AMLBody();
		pNode->AddBody(&x);

		x->SetName(pDefProp->GetProp(_ES("name")));

		for(int i=0;i<ps->GetChildCount();i++)
		{
			CONFIG_BODY tmp;
			tmp = ps->GetChild(i);
			__DefProp2Node( tmp,x );
		}

	}
        else
	{
		EString s;
		s = pDefProp->GetProp(_ES("name"));
		if( s == L"Color" )
		{
			EString sc;
			sc = pDefProp->GetProp(_ES("default"));
			int r=0,g=0,b=0;
			int iPos = sc.Find(L"(");
			if( iPos > -1 )
			{
				int iPos2 = sc.Find(L":");
				r = EIntVal( sc.Mid(iPos+1,iPos2-iPos-1) );
				sc = sc.Right(sc.Length()-iPos2-1);
				iPos = sc.Find(L":");
				g = EIntVal( sc.Left(iPos) );
				sc = sc.Mid(iPos+1,sc.Length()-2-iPos-1);
				b = EIntVal( sc );
			}
			pNode->SetProp(s,EStringVal((UINT)RGB(r,g,b)) );
		}
		else
			pNode->SetProp( pDefProp->GetProp(_ES("name")),pDefProp->GetProp(_ES("default")));
	}
}

afc_object<CControl> CControl::CreateInstance(const CONFIG_BODY& pDefBody)
{
        afc_object<CControl> p(new CControl());

	CONFIG_BODY pRoot;
	pRoot = new AMLBody();
	pRoot->SetName(pDefBody->GetProp(_ES("type")));

	p->m_Config.SetRootBody( &pRoot );
        p->m_pDefNode = pDefBody;

        CONFIG_BODY pProps;
        pProps = pDefBody->FindBody(_ES("props"));
        if( &pProps == NULL ) throw 0;//fix me later

        for(int i=0;i<pProps->GetChildCount();i++)
        {
                CONFIG_BODY tmp;
                tmp = pProps->GetChild(i);
                __DefProp2Node(tmp,pRoot);
        }

        return p;
}


void CControl::LoadChildren()//用于根据IConfig的子控件节点生成子控件CControl实例
{
        CONFIG_BODY pBody;
	pBody = m_Config.GetRootBody();//m_aBody[0];
	if( &pBody == NULL ) return;

        m_pDefNode = GetControlDefNode(pBody->GetName().Text());

        CONFIG_BODY pbChild;
        pbChild = pBody->FindBody(_ES("@"));
        if( &pbChild != NULL )
        {
                for(int i=0;i<pbChild->GetChildCount();i++)
                {
                        CONFIG_BODY px;
                        px = pbChild->GetChild(i);

                        afc_object<CControl> c;
                        c = new CControl();
                        c->m_pParent = this;
                        c->m_Config.SetRootBody(&px);// m_aBody.push( px );

                        m_aChild.push( c );
                        c->LoadChildren();
                }
        }

}
