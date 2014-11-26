//---------------------------------------------------------------------------

#ifndef UIPropMgrH
#define UIPropMgrH

#include "uicontrol.h"

class CUIDoc;

class CUIPropMgr : public AObject
{
public:
	CUIPropMgr(CUIDoc* pDoc,APropEdit* pEditor);
	virtual ~CUIPropMgr();

	void ShowProp(const afc_object<CControl>& pControl);
	afc_object<CControl> GetCurControl()
	{
		return m_pCurControl;
	}
private:
	CUIDoc*	m_pDoc;
	APropEdit*	m_pEditor;
	afc_object<CControl>	m_pCurControl;
private:
	void PropOnChange(APropItem* pItem);
        void ShowProp(const CONFIG_BODY& pProp,APropGroup* pGroup=NULL);
};



//---------------------------------------------------------------------------
#endif
