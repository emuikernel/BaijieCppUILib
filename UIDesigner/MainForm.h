#pragma once

#include "UIDoc.h"
#include "UIPropMgr.h"

class AMainForm : public AForm
{
	CLASS_INFO(MainForm,Form);
public:
	AMainForm(AComponent* pOwner);
	virtual ~AMainForm();

	static AMainForm* CreateMe();

	virtual void ConnectEvent();

	MAP_CONTROL(AMenuItem,mnAbout);
	MAP_CONTROL(ATitleBar,TitleBar);
	MAP_CONTROL(AMenuBar,MenuBar);
	MAP_CONTROL(AMenuItem,Exit);
	MAP_CONTROL(AContainer,Box);
	MAP_CONTROL(AContainer,BoxContainer);
	MAP_CONTROL(APropEdit,PropEdit);
	MAP_CONTROL(AMenuItem,SaveUI);
	MAP_CONTROL(AMenuItem,OpenUI);
	MAP_CONTROL(AMenuItem,PreviewUI);
	MAP_CONTROL(AMenuItem,ClosePreview);
	MAP_CONTROL(AFileSaveDialog,SaveDialog);
	MAP_CONTROL(AFileOpenDialog,OpenDialog);
	MAP_CONTROL(AMenuItem,GenUI);
	MAP_CONTROL(AMenuItem,GenUIH);


	void OnAboutClick(AObject* pSender,AEvent* pEvent);
	void OnMenuBarMouseDown(AObject* pSender,AEvent* pEvent);
	void OnMenuBarMouseMove(AObject* pSender,AEvent* pEvent);
	void OnExitExecute(AObject* pSender,AEvent* pEvent);
	virtual void OnCloseQueryExecute(AObject* pSender,AEvent* pEvent);

	void OnBoxPaint(AObject* pSender,AEvent* pEvent);
	void OnBoxDragOver(AObject* pSender,AEvent* pEvent);
	void OnBoxDragDrop(AObject* pSender,AEvent* pEvent);
	void OnBoxMouseDown(AObject* pSender,AEvent* pEvent);
	void OnBoxMouseMove(AObject* pSender,AEvent* pEvent);
	void OnBoxMouseUp(AObject* pSender,AEvent* pEvent);

	void OnRefresh(AObject* pSender,AEvent* pEvent);
	void OnKeyboardExecute(AObject* pSender,AEvent* pEvent);
	void OnLeftAlign(AObject* pSender,AEvent* pEvent);
	void OnTopAlign(AObject* pSender,AEvent* pEvent);
	void OnRightAlign(AObject* pSender,AEvent* pEvent);
	void OnBottomAlign(AObject* pSender,AEvent* pEvent);
	void OnCenterAlign(AObject* pSender,AEvent* pEvent);
	void OnMiddleAlign(AObject* pSender,AEvent* pEvent);
	void OnHorzSpaceAlign(AObject* pSender,AEvent* pEvent);
	void OnVertSpaceAlign(AObject* pSender,AEvent* pEvent);
	void OnSameHeightAlign(AObject* pSender,AEvent* pEvent);
	void OnSameWidthAlign(AObject* pSender,AEvent* pEvent);
	void OnSameSizeAlign(AObject* pSender,AEvent* pEvent);
	void OnLeftAlignUpdate(AObject* pSender,AEvent* pEvent);
	void OnTopAlignUpdate(AObject* pSender,AEvent* pEvent);
	void OnRightAlignUpdate(AObject* pSender,AEvent* pEvent);
	void OnBottomAlignUpdate(AObject* pSender,AEvent* pEvent);
	void OnCenterAlignUpdate(AObject* pSender,AEvent* pEvent);
	void OnMiddleAlignUpdate(AObject* pSender,AEvent* pEvent);
	void OnHorzSpaceAlignUpdate(AObject* pSender,AEvent* pEvent);
	void OnVertSpaceAlignUpdate(AObject* pSender,AEvent* pEvent);
	void OnSameHeightAlignUpdate(AObject* pSender,AEvent* pEvent);
	void OnSameWidthAlignUpdate(AObject* pSender,AEvent* pEvent);
	void OnSameSizeAlignUpdate(AObject* pSender,AEvent* pEvent);

	void OnCopyExecute(AObject* pSender,AEvent* pEvent);
	void OnPasteExecute(AObject* pSender,AEvent* pEvent);
	void OnDeleteExecute(AObject* pSender,AEvent* pEvent);
	void OnCutExecute(AObject* pSender,AEvent* pEvent);
	void OnLockExecute(AObject* pSender,AEvent* pEvent);
	void OnSaveExecute(AObject* pSender,AEvent* pEvent);
	void OnOpenExecute(AObject* pSender,AEvent* pEvent);
	void OnPreviewExecute(AObject* pSender,AEvent* pEvent);
	void OnClosePreviewExecute(AObject* pSender,AEvent* pEvent);
	void OnPreviewFormClose(AObject* pSender,AEvent* pEvent);
	void OnGenUIExecute(AObject* pSender,AEvent* pEvent);
	void OnGenUIHExecute(AObject* pSender,AEvent* pEvent);
protected:
	afc_object<CUIPropMgr>	m_pUIPropMgr;
	afc_object<CUIDoc>			m_Doc;

	AVertScrollBar*		m_pVB;
	AHorzScrollBar*		m_pHB;
	AActionList*		m_pActionList;
	AForm*			m_pForm;
	AObjectArray		m_aPreviewForm;

	int  TrySave();
	void Save();

	virtual void DoCreate();
	void DoCopy();
	void DoPaste();
	void DoDelete();
	// 	bool QuerySave();
	// 
	// 	void ForceSave();
	// 	void Compile();
	void LocateTo(CControl* pUI);

};

