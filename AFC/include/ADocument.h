#ifndef ADOCUMENT_H
#define ADOCUMENT_H

class AFC_API ADocument:public AObject
{
	CLASS_INFO(Document,Object);
public:
	ADocument(AModelBase* pModel);
	virtual ~ADocument();

	virtual AModelBase* GetModel(){ return m_pModel; }

	//need to be override
	virtual void LoadFromFile(const AString& sFile){}
	virtual void SaveToFile(const AString& sFile){}

	virtual bool GetModified();
	virtual void SetModified(bool v);
protected:
	AModelBase*	m_pModel;
	bool		m_bModified;
};


#endif//ADOCUMENT_H