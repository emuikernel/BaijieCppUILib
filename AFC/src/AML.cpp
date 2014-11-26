#include "stdafx.h"
#include <map>
#include "tinyxml.h"

inline bool _issepchar(ACHAR ch)
{
	ACHAR* sz = AFC_CONFIG_STOP_CHAR;
	while(*sz && *sz != ch ) sz++;
	if( *sz == 0 ) return false;
	return true;
}
AMLBody::AMLBody():base_class()
{
	//
}

AMLBody::~AMLBody()
{
	Clear();
}

AString AMLBody::GetName()
{
	return m_sName;
}
void AMLBody::SetName(const AString& sName)
{
	m_sName = sName;
}

AString AMLBody::GetProp(const AString& sKey)
{
	
	AString s;
	if( m_aProp.IsExist(sKey) )
	{
		void* p = m_aProp.GetItem(sKey);
		if( p )
		{
			s = (ACHAR*)p;
		}
	}
	
	return s;
}

bool AMLBody::HasProp(const AString& sKey)
{
	return m_aProp.IsExist(sKey);
}

void AMLBody::SetProp(const AString& sKey,const AString& sValue)
{
	ACHAR* s = NULL;
	if( m_aProp.IsExist(sKey) )
	{
		s = (ACHAR *)m_aProp.GetItem(sKey);
		if( s ) delete[] s;
	}
	s = NULL;
	if( sValue.Length() )
	{
		s = new ACHAR[sValue.Length()+1];
		_tcscpy(s,sValue.Text());
	}
	m_aProp.SetItem(sKey,s);
}

AMLBody* AMLBody::AddBody(const AString& sBodyName)
{
	AMLBody* p = new AMLBody();
	p->AddRef();
	m_aChild.Add(p);
	p->SetName(sBodyName);
	return p;
}
void AMLBody::AddBody(AMLBody* p)
{
	p->AddRef();
	m_aChild.Add(p);
}
void AMLBody::RemoveBody(AMLBody* pBody)
{
	m_aChild.Delete(m_aChild.IndexOf(pBody));
	if( pBody ) pBody->Release();
}

AMLBody* AMLBody::FindBody(const AString& sBodyName)
{
	AString s1,s2;
	int iPos = sBodyName.Find(_T("/"));
	if( iPos > 0 )
	{
		s2 = sBodyName.Left(iPos);
		s1 = sBodyName.Right(sBodyName.Length()-iPos-1);
	}
	else
	{
		s2 = sBodyName;
		s1 = _T("");
	}

	int i,iCount = m_aChild.GetCount();
	AMLBody* px = NULL;
	for(i=0;i<iCount;i++)
	{
		AMLBody* p = dynamic_cast<AMLBody*>( (AObject *)m_aChild[i] );
		if( p && p->GetName() == s2 )
		{
			px = p;
			break;
		}
	}
	if( px )
	{
		if( s1.Length() ) px = px->FindBody(s1);
	}
	return px;
}

void AMLBody::Clear()
{
	AStringMapEnumerator em(&m_aProp);
	while( em.Next() )
	{
		ACHAR* sz = (ACHAR*)em.Cur();
		if( sz ) delete[] sz;
	}
	m_aProp.Clear();
	int i,iCount = m_aChild.GetCount();
	for(i=0;i<iCount;i++)
	{
		AMLBody* p = dynamic_cast<AMLBody*>( (AObject *)m_aChild[i] );
		if( p ) p->Release();
	}
	m_aChild.Clear();
}

AString AMLBody::Escape(const AString& s)
{
	if( s == _T("") ) return _T("\"\"");
	AString sv;
	sv = s;
	bool b = false;
	if( s.Find(_T("\"")) > -1 )
	{
		sv.Replace(_T("\""),_T("\\\""));
		b = true;
	}
	ACHAR* sz = (ACHAR *)AFC_CONFIG_STOP_CHAR;
	int iLen = sv.Length();
	if( !b )
	{
		for(int i=0;i<iLen;i++)
		{
			if( _issepchar(sv[i]))
			{
				b = true;
				break;
			}
		}
	}
	

	if( b ) sv = _T("\"") + sv + _T("\"");
	return sv;
}

AString AMLBody::ToString()
{
	AString str;
	str = m_sName;
	str += _T("\n{\n");
	AStringMapEnumerator em(&m_aProp);
	while( em.Next() )
	{
		AString s;
		s = Escape(em.Index())+_T(":")+Escape((ACHAR *)em.Cur())+_T("\n");
		str += s;
	}
	AArrayEnumerator aem(&m_aChild);
	while( aem.Next() )
	{
		AMLBody* p = dynamic_cast<AMLBody*>( (AObject *)aem.Cur() );
		if( p ) str += p->ToString();
	}

	str += _T("}\n");
	return str;
}

int AMLBody::GetChildCount()
{
	return m_aChild.GetCount();
}

int AMLBody::GetBodyCount()
{
	return m_aChild.GetCount();
}

AMLBody* AMLBody::GetChild(int index)
{
	return dynamic_cast<AMLBody*>( (AObject*)m_aChild[index] );
}

AMLBody* AMLBody::GetBody(int index)
{
	return GetChild(index);
}


AML::AML():base_class()
{
	m_pRoot = NULL;
}

AML::~AML()
{
	if( m_pRoot ) m_pRoot->Release();
}

void AML::LoadText(const AString& str)
{
	m_szPos = (ACHAR *)str.Text();

	AMLBody* pCurBody=NULL;
	m_pRoot = NULL;

	while( true )
	{
		//EString sToken = GetToken();
		ACHAR* szStart=NULL;
		int iLen = 0;
		GetToken(&szStart,&iLen);
		//if( sToken == _ES("") ) break;
		if( iLen < 1 ) break;
		AString sKey(szStart,iLen);// = sToken;

		//sToken = GetToken();
		iLen = 0;
		GetToken(&szStart,&iLen);
		//if( sToken == _ES("{") ) ReadBody(sKey,&pCurBody);
		if( iLen == 1 && *szStart == '{' ) ReadBody(sKey,pCurBody);
		else break;
	}

}

AMLBody* AML::FindBody(const AString& sBodyName)
{
	if( m_pRoot == NULL ) return NULL;

	AString s1,s2;
	int iPos = sBodyName.Find(_T("/"));
	if( iPos > 0 )
	{
		s2 = sBodyName.Left(iPos);
		s1 = sBodyName.Right(sBodyName.Length()-iPos-1);
	}
	else
	{
		s2 = sBodyName;
		s1 = _T("");
	}
	if( m_pRoot->GetName() == s2 )
	{
		if( s1.Length() )
			return m_pRoot->FindBody(s1);
		else
			return m_pRoot;
	}
	return NULL;
}

void AML::GetToken(ACHAR* *szStart,int* iLen)
{
	*szStart = NULL;
	*iLen = 0;//default
	//EString sRet;
	ACHAR* str = m_szPos;
	while( true )
	{
		if( *str == 0 )
		{
			break;
		}
		else if( *str == '#' )
		{
			while( *str && *str != '\n' ) str ++;
		}
		else if( *str == ' ' || *str == '\t' || *str == '\r' || *str == '\n' )
		{
			str ++;
		}
		else if( *str == '{' )
		{
			//sRet = _ES("{");
			*szStart = str;
			*iLen = 1;
			str ++;
			break;
		}
		else if( *str == '}' )
		{
			//sRet = _ES("}");
			*szStart = str;
			*iLen = 1;
			str ++;
			break;
		}
		else if( *str == ':' )
		{
			//sRet = _ES(":");
			*szStart = str;
			*iLen = 1;
			str ++;
			break;
		}
		else if( *str == '\"' )
		{
			str ++;
			ACHAR* p = str;
			*szStart = p;
			while( *str )
			{
				if( *str == '\"' && *(str-1) != '\\' ) break;
				str ++;
			}
			*iLen = str-p;
			if( *str == '\"' )
			{
				//sRet = EString(p,str-p);
				//sRet.Replace(_ES("\\\""),_ES("\""));
				str ++;
			}
			break;
		}
		else
		{
			ACHAR* p = str;
			while( true )
			{
				str ++;
				if( _issepchar(*str) ) break;
			}
			//sRet = EString(p,str-p);
			*szStart = p;
			*iLen = str-p;
			break;
		}
	}
	m_szPos = str;
	//return sRet;
}



AString AML::Unescape(const AString& s)
{
	AString sv;
	sv = s;
	return sv;
}

void AML::ReadBody(const AString& sName,AMLBody* pParent)
{
	AMLBody* pBody = NULL;
	if( pParent ) pBody = pParent->AddBody( sName );
	else 
	{
		pBody = new AMLBody();
		pBody->SetName(sName);
		m_pRoot = pBody;
		m_pRoot->AddRef();
	}

	AString sToken;
	ACHAR* szStart=NULL;
	int iLen = 0;
	while( true )
	{
		//sToken = GetToken();
		GetToken(&szStart,&iLen);
		if( iLen == 1 && *szStart == '}' ) break;
		//if( iLen == 0 ) return;
		//if( sToken == _ES("}") ) break;

		AString sKey(szStart,iLen);// = sToken;

		//sToken = GetToken();
		GetToken(&szStart,&iLen);
		if( iLen == 1 && *szStart == ':' )//sToken == _ES(":") )
		{
			//sToken = GetToken();
			GetToken(&szStart,&iLen);
			sToken = AString(szStart,iLen);
			//sKey = Unescape(sKey);
			//sToken = Unescape(sToken);
			sToken.Replace(_T("\\\""),_T("\""));
			pBody->SetProp( sKey,sToken );
		}
		else if( iLen == 1 && *szStart == '{' )//sToken == _ES("{") )
			ReadBody(sKey,pBody);
		else
			break;
	}
}

void AMLBody::FromXML(const AString& xml)
{
	AString str;
	str = xml;
	TiXmlDocument doc;
	doc.Parse(str.ConvertTo());

	if( doc.RootElement() != NULL )
	{
		_fromXML(doc.RootElement());
		
	}
	else
	{
		//ODS(L"[es]root ЮЊПе");
	}
	
}

void AMLBody::_fromXML(void* node)
{
	TiXmlElement* pNode = (TiXmlElement *)node;
	this->SetName( AString((char *)pNode->Value()));
	TiXmlAttribute* pAttr = pNode->FirstAttribute();
	while( pAttr )
	{
		AString sn,sv;
		sn = pAttr->Name();
		sv = pAttr->Value();
		this->SetProp(sn,sv);
		pAttr = pAttr->Next();
	}
	if( pNode->FirstChild() && pNode->FirstChild()->Type() == TiXmlNode::TINYXML_TEXT )
		this->SetProp(L"__value__",AString((char *)pNode->FirstChild()->Value()) );
	TiXmlElement* pc = pNode->FirstChildElement();
	while( pc )
	{
		AMLBody* b = this->AddBody(_T(""));
		b->_fromXML(pc);
		pc = pc->NextSiblingElement();
	}
}
AString AMLBody::ToXML()
{
	TiXmlElement* elem = (TiXmlElement*)this->_toXML();

	TiXmlPrinter printer;
	elem->Accept( &printer );

	AString str;
	str = printer.CStr();
	return str;
}

void* AMLBody::_toXML()
{
	TiXmlElement* pe = new TiXmlElement("");

	AString s;
	s = this->GetName();
	pe->SetValue(s.ConvertTo());
	AStringMap::myitem* pItem=m_aProp.Begin();
	while( m_aProp.IsEnd(pItem) == false )
	{
		AString sn,sv;
		sn = pItem->sKey;
		sv = (ACHAR *)pItem->pData;
		if( sn == L"__value__" )
		{
			TiXmlText* t = new TiXmlText(sv.ConvertTo());
			pe->LinkEndChild( t );
		}
		else
		{
			pe->SetAttribute(sn.ConvertTo(),sv.ConvertTo());
		}
		pItem = m_aProp.Next(pItem);
	}
	for(int i=0;i<GetChildCount();i++)
	{
		pe->LinkEndChild( (TiXmlElement *) GetChild(i)->_toXML() );
	}
	return pe;
}