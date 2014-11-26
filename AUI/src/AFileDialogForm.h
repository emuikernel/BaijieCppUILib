#ifndef AFILEDIALOGFORM_H
#define AFILEDIALOGFORM_H


class AFileDialogForm : public AForm
{
	CLASS_INFO(FileDialogForm,Form);
public:
	AFileDialogForm(AComponent* pOwner);
	virtual ~AFileDialogForm();

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
	

	MAP_CONTROL(AComboBox,FileFilter);
	MAP_CONTROL(ATreeView,DirView);
	MAP_CONTROL(AListView,FileView);
	MAP_CONTROL(AButton,OK);
	MAP_CONTROL(AButton,Cancel);
	MAP_CONTROL(AEdit,FileName);
	MAP_CONTROL(ASysIconImageList,IconImageList);
	MAP_CONTROL(AContainer,ButtonsPanel);
	MAP_CONTROL(ATitleBar,TitleBar);
	MAP_CONTROL(AContainer,MenuBarPanel);


	TDialogButton *m_nButtonID;
	AString*	m_sFileName;

	bool		m_bOpenMode;//true:打开 false:保存
	AString		m_sInitDir;
private:
	void FillNode(ATreeViewItem* pNode);
	void FillFiles(const AString& sPath);

	void OnFileListClick(AObject* pSender,AEvent* pEvent);
	void OnFileListDblClick(AObject* pSender,AEvent* pEvent);

	void LocateTo(const AString& sPath);

	std::map<ATreeViewItem*,AString>	m_aPath;//节点与目录的对应关系
};

#endif//AFILEDIALOGFORM_H