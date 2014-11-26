//---------------------------------------------------------------------------

#ifndef UIDrawerH
#define UIDrawerH


#include "uidoc.h"
class CUIDrawer
{
public:
	CUIDrawer(CUIDoc* pDoc,BVCanvas* pCanvas,ARect r);
	~CUIDrawer();

	void Draw();
private:
	CUIDoc*		m_pDoc;
	BVCanvas*	m_pCanvas;
	ARect		m_rBox;

	ARect	Offset(ARect r);
	void	DrawHandler(bool bFocus,BVRect r);
	void	DrawShape(const afc_object<CControl>& pBase);
	void	RenderDraw(BVRect r,const EString& sDraw,const afc_object<CControl>& pBase);
};

//---------------------------------------------------------------------------
#endif
