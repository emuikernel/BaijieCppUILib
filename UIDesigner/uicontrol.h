//---------------------------------------------------------------------------

#ifndef uicontrolH
#define uicontrolH

class CProp : public afc_object_base
{
public:
	friend class CControl;
	CProp():afc_object_base()
	{
	}
	virtual ~CProp()
	{
	}
	afc_object<CProp> operator[](const EString& sPath)
	{
		CONFIG_BODY p;
		EString sLeft,sProp;
		int iPos = sPath.ReverseFind(L"/");
		if( iPos > -1 )
		{
			sLeft = sPath.Left(iPos);
			sProp = sPath.Right(sPath.Length()-iPos-1);
			p = m_pBody->FindBody( sLeft );
			if( &p == NULL )
			{
				p = m_pBody->AddBody(sLeft);
			}
		}
		else
		{
			p = m_pBody;
			sProp = sPath;
		}
		//if( &p == NULL )
		//	throw 2;

		afc_object<CProp> px;
		px = new CProp();
		px->m_pBody = p;
		px->m_sProp = sProp;

		return px;
	}
	int AsInt()
	{
		if( &m_pBody == NULL ) return 0;
		else return EIntVal( m_pBody->GetProp(m_sProp) );
	}
	void AsInt(int v)
	{
		if( &m_pBody != NULL )
			m_pBody->SetProp(m_sProp,EStringVal(v));
	}
	EString AsString()
	{
		if( &m_pBody == NULL ) return _ES("");
		else return m_pBody->GetProp(m_sProp);
	}
	void AsString(const EString& v)
	{
		if( &m_pBody != NULL )
			m_pBody->SetProp(m_sProp,v);
	}
	BVColor AsColor()
	{
		BVColor cl(255,255,255,255);
		if( &m_pBody != NULL )
		{
			cl = Int2Color(AIntVal(m_pBody->GetProp(m_sProp)));
		}
		return cl;
	}
	void AsColor(const BVColor v)
	{
		if( &m_pBody != NULL )
			m_pBody->SetProp(m_sProp,EStringVal(Color2Int(v)));
	}
	BVFont AsFont()
	{
		if( &m_pBody == NULL ) return BVFont("ËÎÌו",12,AColor(255,0,0,0));
		else
		{
			EString sName=this->operator[](_ES("n"))->AsString();
			int iSize= this->operator[](_ES("sz"))->AsInt();
			BVColor cl = this->operator[](_ES("cr"))->AsColor();
			EString st = this->operator [](_ES("st"))->AsString();
			int style=0;
			if( st.Find(_T("b")) > -1 ) style |= FontStyleBold;
			if( st.Find(_T("i")) > -1 ) style |= FontStyleItalic;
			if( st.Find(_T("u")) > -1 ) style |= FontStyleUnderline;
			if( st.Find(_T("s")) > -1 ) style |= FontStyleStrikeout;
			AFont font;
			font.SetName( sName );
			font.SetSize( iSize );
			font.SetColor( cl );
			font.SetStyle( (FontStyle)style );
			return font;
		}
	}
	void AsFont(const BVFont& v)
	{
		AFont font;
		font = v;
		if( &m_pBody != NULL )
		{
			this->operator[](_ES("n"))->AsString(font.GetName());
			this->operator[](_ES("sz"))->AsInt(font.GetSize());
			this->operator[](_ES("cr"))->AsColor(font.GetColor());
			AString st;
			if( font.GetStyle() & FontStyleBold ) st += _T("b");
			if( font.GetStyle() & FontStyleItalic ) st += _T("i");
			if( font.GetStyle() & FontStyleUnderline ) st += _T("u");
			if( font.GetStyle() & FontStyleStrikeout ) st += _T("s");
			this->operator[](_ES("st"))->AsString(st);
		}
	}
	void GetProps(afc_vector<CProp>& aProp)
	{
		CONFIG_BODY pRoot=m_pBody;
		if( m_sProp == _ES("") )
		{
			for(int i=0;i<pRoot->GetChildCount();i++)
			{
				CONFIG_BODY px;
				px = pRoot->GetChild(i);
				afc_object<CProp> p;
				p = new CProp();
				p->m_pBody = px;
				p->m_sProp = _ES("");
				aProp.push(p);
			}
                }
	}
	EString GetPropName(){ return m_sProp; }
private:
	CONFIG_BODY	m_pBody;
	EString		m_sProp;
};

class CControl : public afc_object_base
{
public:
	CControl():afc_object_base()
	{
		m_pParent = NULL;
	}
	virtual ~CControl()
	{
	}
        static afc_object<CControl> CreateInstance(const CONFIG_BODY& pDefBody);
	afc_object<CProp> operator[](const EString& sPath)
	{
		CONFIG_BODY pRoot;
		pRoot = m_Config.GetRootBody();
		if( pRoot == ANULL ) throw AException(_T("null root body"));

		CONFIG_BODY pBody;
		EString sLeft,sProp;
		int iPos = sPath.ReverseFind(L"/");
		if( iPos > -1 )
		{
			sLeft = sPath.Left(iPos);
			sProp = sPath.Right(sPath.Length()-iPos-1);
			pBody = pRoot->FindBody( sLeft );
			if( &pBody == NULL )
			{
				pBody = pRoot->AddBody( sLeft );
			}
		}
		else
		{
			pBody = pRoot->FindBody( sPath );
			if( &pBody == NULL )
			{
				pBody = pRoot;
				sProp = sPath;
			}
			else
			{
				sProp = _ES("");
			}
		}
		//if( &pBody == NULL ) throw 1;

		afc_object<CProp> p;
		p = new CProp();
		p->m_pBody = pBody;
		p->m_sProp = sProp;

		return p;
	}
	BVRect GetPosition()
	{
		int x= (*this)[_ES("l")]->AsInt();
		int y= (*this)[_ES("t")]->AsInt();
		int w = (*this)[_ES("w")]->AsInt();
		int h = (*this)[_ES("h")]->AsInt();
		return ARect(x,y,x+w,y+h);
	}
	void SetPosition(BVRect r)
	{
		(*this)[_ES("l")]->AsInt(r.left);
		(*this)[_ES("t")]->AsInt(r.top);
		(*this)[_ES("w")]->AsInt(abs(r.GetWidth()));
		(*this)[_ES("h")]->AsInt(abs(r.GetHeight()));
	}
	BVRect GetAbsPosition()
	{
		BVRect r=GetPosition();
		if( m_pParent )
		{
			BVRect rp = m_pParent->GetAbsPosition();
			r.Offset(rp.left,rp.top);
		}
		return r;
	}
	EString GetType()
	{
	        CONFIG_BODY pRoot;
		pRoot = m_Config.GetRootBody();
		if( pRoot == ANULL ) throw AException(_T("Null root body"));
		return pRoot->GetName();
	}
	afc_object<CControl> Clone()
	{
		CONFIG_BODY pBody;
		pBody = m_Config.GetRootBody();
		afc_object<CControl> c;
		c = new CControl();
		c->m_Config.LoadText( pBody->ToString() );
		c->m_pDefNode = m_pDefNode;
		c->m_pParent = m_pParent;
		for(UINT i=0;i<m_aChild.size();i++)
		{
                        c->m_aChild.push(m_aChild[i]);
                }
		return c;
	}
	void AddChild(const afc_object<CControl>& pc)
	{
		m_aChild.push( pc );
		CONFIG_BODY pBody;
		pBody = m_Config.GetRootBody();

		CONFIG_BODY pbChild;
		pbChild = pBody->FindBody(_ES("@"));
		if( &pbChild == NULL )
		{
			pbChild = pBody->AddBody(_T("@"));
		}

		CONFIG_BODY pBody2;
		pBody2 = pc->m_Config.GetRootBody();
		pbChild->AddBody( &pBody2 );
	}
	void RemoveChild(const afc_object<CControl>& pc)
	{
	        CONFIG_BODY pBody;
		pBody = m_Config.GetRootBody();

		CONFIG_BODY pBody2;
		pBody2 = pc->m_Config.GetRootBody();

		CONFIG_BODY pbChild;
		pbChild = pBody->FindBody(_ES("@"));
		if( &pbChild != NULL )
		{
			pbChild->RemoveBody(&pBody2);
		}

		for(unsigned int i=0;i<m_aChild.size();i++)
		{
			afc_object<CControl> x;
			x = m_aChild[i];
			if( x == &pc )
			{
				m_aChild.erase(i);
				break;
			}
		}
	}
	void LoadChildren();
	void GetProps(afc_vector<CProp>& aProp)
	{
                CONFIG_BODY pRoot;
		pRoot = m_Config.GetRootBody();
		if( pRoot == ANULL ) return;
		AStringMapEnumerator em(&(pRoot->m_aProp));
		while( em.Next() )
		{
			afc_object<CProp> p;
			p = new CProp();
			p->m_pBody = pRoot;
			p->m_sProp = em.Index();
			aProp.push(p);
		}
		for(int i=0;i<pRoot->GetChildCount();i++)
		{
			CONFIG_BODY px;
			px = pRoot->GetChild(i);
			afc_object<CProp> p;
			p = new CProp();
			p->m_pBody = px;
			p->m_sProp = _ES("");
			aProp.push(p);
                }
        }
	
	IConfig			m_Config;
        CONFIG_BODY             m_pDefNode;
	afc_vector<CControl>	m_aChild;
	CControl*		m_pParent;
};
//---------------------------------------------------------------------------
#endif
