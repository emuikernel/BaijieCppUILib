#pragma once
class AToolBoxControl : public AContainer
{
	CLASS_INFO(ToolBoxControl,Container);
public:
	AToolBoxControl(AComponent* pOwner);
	virtual ~AToolBoxControl(void);

	CONFIG_BODY GetControlDefNode(const AString& sName);

	void OnTreeMouseMove(AObject* pSender,AEvent* pEvent);
	void OnTreeDrawDrag(AObject* pSender,AEvent* pEvent);

	ATreeView*	m_pTree;
	AImageList*	m_pImageList;
private:
	void LoadDefFiles();//装载所有的控件定义文件
	void LoadControlDefine(const AString& sFile);
	ATreeViewItem* GetNodeByCat(const AString& sCat);

	afc_object<AMLBody>         m_ControlDefines;
	std::map<AString,ATreeViewItem*>	m_aCatNode;
};

