//---------------------------------------------------------------------------

#ifndef documentH
#define documentH


class IDocument : public afc_object_base
{
public:
	IDocument();
	virtual ~IDocument();

	virtual EString		GetFileName();
	virtual void		SetFileName(const EString& s);

	virtual void	LoadFromFile(const EString& sFile)=0;
	virtual void	SaveToFile(const EString& sFile)=0;

	virtual void		SetModified(bool b);
	virtual bool		GetModified();

	virtual void		SetForm(AObject* p);
	virtual AObject*		GetForm();

	virtual void		SetNode(ATreeViewItem* p);
	virtual ATreeViewItem*	GetNode();
protected:
	EString		m_sFileName;
	bool		m_bModified;
	ATreeViewItem*	m_pNode;
	AObject*	m_pForm;

};

class ICodeDocument : public IDocument
{
public:
	ICodeDocument();
	virtual ~ICodeDocument();

	void	LoadFromFile(const EString& sFile);
	void	SaveToFile(const EString& sFile);

	void	SetCode(const EString& s);
	EString	GetCode();
private:
	EString		m_sCode;
};




//---------------------------------------------------------------------------
#endif
