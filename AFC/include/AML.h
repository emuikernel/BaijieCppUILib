#ifndef AML_H
#define AML_H

/*	format example
config
{
	server
	{
		port:8080
	}
	vd
	{
		name:MyWeb
		path:c:\test
	}
}

*/
#define AFC_CONFIG_STOP_CHAR _T("# \t\r\n{}:\"")

class AFC_API AMLBody : public AObject
{
	CLASS_INFO(MLBody,Object);
public:
	AMLBody();
	virtual ~AMLBody();

	AString GetName();
	void SetName(const AString& sName);

	AString GetProp(const AString& sKey);
	void SetProp(const AString& sKey,const AString& sValue);
	bool HasProp(const AString& sKey);

	AMLBody* AddBody(const AString& sBodyName);
	void AddBody(AMLBody* p);
	void RemoveBody(AMLBody* pBody);
	AMLBody* FindBody(const AString& sBodyName);

	int GetChildCount();
	int GetBodyCount();

	AMLBody* GetChild(int index);
	AMLBody* GetBody(int index);

	void Clear();

	AString Escape(const AString& s);
	AString ToString();

	void FromXML(const AString& xml);
	AString ToXML();

	AStringMap	m_aProp;

protected:
	
	AArray		m_aChild;
	AString		m_sName;

	void _fromXML(void* node);
	void* _toXML();
};

class AFC_API AML : public AObject
{
	CLASS_INFO(ML,Object);
public:

	AML();
	virtual ~AML();

	void LoadText(const AString& str);
	AMLBody* FindBody(const AString& sBodyName);

	

	void SetRootBody(AMLBody* p){ m_pRoot = p; if( p ) p->AddRef(); }

	AMLBody* GetRootBody(){ return m_pRoot; }
public:
	AMLBody*	m_pRoot;
private:
	ACHAR*	m_szPos;

	void GetToken(ACHAR* *szStart,int* iLen);
	AString Unescape(const AString& s);
	void ReadBody(const AString& sName,AMLBody* pParent);
};

#define CREATE_CONFIG_BODY(node,name) xobject<AMLBody> node(new AMLBody()); node->SetName(name)

#endif//AML_H