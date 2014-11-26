#ifndef ADIRECTORYDIALOG_H
#define ADIRECTORYDIALOG_H

class AUI_API ADirectoryDialog: public AInvisibleControl
{
	CLASS_INFO(DirectoryDialog,InvisibleControl);
public:
	ADirectoryDialog(AComponent* pOwner);
	virtual ~ADirectoryDialog();

	virtual void SetDirectory(const AString& sDirectory);
	virtual AString GetDirectory();

	virtual bool Execute();
protected:
	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	AString		m_sDirectory;
};
#endif//ADIRECTORYDIALOG_H
