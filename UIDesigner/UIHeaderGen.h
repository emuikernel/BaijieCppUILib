#pragma once
class UIHeaderGen
{
public:
	UIHeaderGen(AMLBody* pRoot);
	~UIHeaderGen(void);

	AString ToString();
private:
	AMLBody*	m_pRoot;

	afc_object<AMLBody> GenControl(AMLBody* pRoot);
	void load_init(AMLBody* pNode,AStringArray& sa);
	void load_map(AMLBody* pNode,AStringArray& sa);
};

