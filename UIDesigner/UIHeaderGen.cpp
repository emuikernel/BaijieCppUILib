#include "StdAfx.h"
#include "UIHeaderGen.h"
#include "UIXMLGen.h"

UIHeaderGen::UIHeaderGen(AMLBody* pRoot):m_pRoot(pRoot)
{
}


UIHeaderGen::~UIHeaderGen(void)
{
}


AString UIHeaderGen::ToString()
{
	AString sTempl;
	sTempl = AFile::File2String(GetCurPath()+_T("UITemp.h"));
	AString sFormName;
	sFormName = m_pRoot->GetProp(_T("n"));
	sTempl.Replace(_T("$FORMNAME$"),sFormName);
	sTempl.Replace(_T("$FORMNAME_UPCASE$"),sFormName.ToUpper());

	UIXMLGen gen(m_pRoot);
	AString xml=gen.ToNode()->ToXML();
	//xml.Replace(_T("</c>"),_T("\r\n</c>"));
	xml.Replace(_T("\r\n"),_T("\\\r\n"));
	xml.Replace(_T("\""),_T("\\\""));
	sTempl.Replace(_T("$FORMNAME_XML$"),xml);

	AStringArray saInit;
	load_init(m_pRoot,saInit);
	sTempl.Replace(_T("$CONTROL_INIT$"),saInit.ToString(_T("\r\n")));

	AStringArray saMap;
	load_map(m_pRoot,saMap);
	sTempl.Replace(_T("$CONTROL_MAPS$"),saMap.ToString(_T("\r\n")));

	sTempl.Replace(_T("$EVENTS$"),_T(""));//暂时不支持事件
	return sTempl;
}

void UIHeaderGen::load_init(AMLBody* pNode,AStringArray& sa)
{
	AString s;
	s = _T("MAP_CONTROL_INIT(");
	s += pNode->GetProp(_T("n"));
	s += _T(");");
	if( pNode->GetName() != _T("Array") ) 
	{
		if( sa.GetCount() ) s = _T("\t\t")+s;
		sa.Add(s);
	}
	AMLBody* pn = pNode->FindBody(_T("@"));
	if( pn )
	{
		for(int i=0;i<pn->GetChildCount();i++)
		{
			AMLBody* p = pn->GetChild(i);
			load_init(p,sa);
		}
	}
}
void UIHeaderGen::load_map(AMLBody* pNode,AStringArray& sa)
{
	AString s;
	s = _T("MAP_CONTROL(A");
	s += pNode->GetName();
	s += _T(",");
	s += pNode->GetProp(_T("n"));
	s += _T(");");
	if( pNode->GetName() != _T("Array") ) 
	{
		if( sa.GetCount() > 0 ) s = _T("\t")+s;
		sa.Add(s);
	}
	AMLBody* pn = pNode->FindBody(_T("@"));
	if( pn )
	{
		for(int i=0;i<pn->GetChildCount();i++)
		{
			AMLBody* p = pn->GetChild(i);
			load_map(p,sa);
		}
	}
}