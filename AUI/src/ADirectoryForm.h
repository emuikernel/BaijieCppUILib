#ifndef ADIRECTORYFORM_H
#define ADIRECTORYFORM_H


class ADirectoryForm : public AForm
{
	CLASS_INFO(DirectoryForm,Form);
public:
	ADirectoryForm(AComponent* pOwner);
	virtual ~ADirectoryForm();

	virtual void InitControls();
	virtual void ConnectEvent();
	virtual void TitleBackDoMouseDown(AObject* pSender,AEvent* pEvent);
	virtual void TitleBackDoMouseMove(AObject* pSender,AEvent* pEvent);
	virtual void OnOKClick(AObject* pSender,AEvent* pEvent);
	virtual void OnCancelClick(AObject* pSender,AEvent* pEvent);
	virtual void OnDirChange(AObject* pSender,AEvent* pEvent);
	virtual void DoCreate();
	void CreateFromXML();
	void SetFilter(const AString& sFilter,int iDefaultIndex);
	

	MAP_CONTROL(ATreeView,DirView);
	MAP_CONTROL(AButton,OK);
	MAP_CONTROL(AButton,Cancel);
	MAP_CONTROL(ASysIconImageList,IconImageList);
	MAP_CONTROL(AContainer,ButtonsPanel);
	MAP_CONTROL(ATitleBar,TitleBar);
	MAP_CONTROL(AContainer,MenuBarPanel);


	TDialogButton *m_nButtonID;
	AString*	m_sDirectory;
	AString		m_sInitDir;
private:
	void FillNode(ATreeViewItem* pNode);
	void LocateTo(const AString& sPath);

	std::map<ATreeViewItem*,AString>	m_aPath;//节点与目录的对应关系
};

#endif//AFILEDIALOGFORM_H