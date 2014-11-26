#pragma once
class UIXMLGen
{
public:
	UIXMLGen(AMLBody* pRoot);
	~UIXMLGen(void);

	AString ToString();
	afc_object<AMLBody> ToNode();
private:
	AMLBody*	m_pRoot;

	afc_object<AMLBody> GenControl(AMLBody* pRoot);
};

