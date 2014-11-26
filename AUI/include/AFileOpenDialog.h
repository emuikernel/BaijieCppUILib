#ifndef AFILEOPENDIALOG_H
#define AFILEOPENDIALOG_H

class AUI_API AFileOpenDialog: public AInvisibleControl
{
	CLASS_INFO(FileOpenDialog,InvisibleControl);
public:
	AFileOpenDialog(AComponent* pOwner);
	virtual ~AFileOpenDialog();

	virtual void SetFileName(const AString& sFileName);
	virtual AString GetFileName();
	virtual void SetFilter(const AString& sFilter);
	virtual AString GetFilter();
	virtual int GetFilterIndex();
	virtual void SetFilterIndex(int index);
	virtual void SetInitDir(const AString& sInitDir);
	virtual AString GetInitDir();

	virtual bool Execute();
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	AString		m_sFileName;
	AString		m_sInitDir;
	AString		m_sFilter;
	int		m_iFilterIndex;//缺省的过滤器下标
};
#endif//AFILEOPENDIALOG_H
