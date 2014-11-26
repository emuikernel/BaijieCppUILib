#ifndef APROPEDIT_H
#define APROPEDIT_H

#define PROP_ITEM_DUMMY		0
#define PROP_ITEM_INT		1
#define PROP_ITEM_STRING	2
#define PROP_ITEM_HINT		3
#define PROP_ITEM_COMBO		4
#define PROP_ITEM_BOOL		5
#define PROP_ITEM_COLOR		6
#define PROP_ITEM_DATA		7
#define PROP_ITEM_GROUP		8


class AUI_API APropItem : public AObject
{
	CLASS_INFO(PropItem,Object);
public:
	APropItem();
	virtual ~APropItem();
	virtual int GetType(){ return PROP_ITEM_DUMMY; }

	virtual AString GetKey();

	int m_iLevel;//缩进的层
	int m_iRow;//所在的行
	AString m_sKey;
	AString m_sDisplayName;
	bool m_bReadOnly;
	void* m_pAppData;
	bool m_bModified;//值是否有修改？
	AObject* m_pTag;
};

typedef void (AObject::*FOnPropChange)(APropItem* pSender);

class AUI_API APropInt : public APropItem
{
	CLASS_INFO(PropInt,PropItem);
public:
	APropInt();
	APropInt(const AString& sKey,const AString& sDisp,int val);
	virtual ~APropInt();
	virtual int GetType(){ return PROP_ITEM_INT; }

	int GetValue();

	int	m_Val;
};

class AUI_API APropString : public APropItem
{
	CLASS_INFO(PropString,PropItem);
public:
	APropString();
	APropString(const AString& sKey,const AString& sDisp,const AString& val,bool bMulti=false);
	virtual ~APropString();
	virtual int GetType(){ return PROP_ITEM_STRING; }

	AString GetValue();

	AString	m_Val;
	bool	m_bMultiLine;
};

class AUI_API APropHint : public APropItem
{
	CLASS_INFO(PropHint,PropItem);
public:
	APropHint();
	APropHint(const AString& sKey,const AString& sDisp,const AString& val);
	virtual ~APropHint();
	virtual int GetType(){ return PROP_ITEM_HINT; }

	AString GetValue();

	AString	m_Val;
};

class AUI_API APropCombo : public APropItem
{
	CLASS_INFO(PropCombo,PropItem);
public:
	APropCombo();
	APropCombo(const AString& sKey,const AString& sDisp,const AString& sVal);
	APropCombo(const AString& sKey,const AString& sDisp,int iVal);
	virtual ~APropCombo();
	virtual int GetType(){ return PROP_ITEM_COMBO; }

	APropCombo* AddChoice(const AString& s,const AString& sDisp);
	AString GetKey(int index);
	AString GetDisp(int index);

	AArray			m_aChoice;
	AString			m_Val;
	int			m_IntVal;
	bool			m_bCanEdit;

};

class AUI_API APropBool : public APropItem
{
	CLASS_INFO(PropBool,PropItem);
public:
	APropBool();
	APropBool(const AString& sKey,const AString& sDisp,bool val);
	virtual ~APropBool();
	virtual int GetType(){ return PROP_ITEM_BOOL; }

	bool GetValue();

	bool	m_Val;
};

class AUI_API APropColor : public APropItem
{
	CLASS_INFO(PropColor,PropItem);
public:
	APropColor();
	APropColor(const AString& sKey,const AString& sDisp,AColor val);
	virtual ~APropColor();
	virtual int GetType(){ return PROP_ITEM_COLOR; }

	AColor GetValue();

	AColor	m_Val;
};

class AUI_API APropData : public APropItem
{
	CLASS_INFO(PropData,PropItem);
public:
	APropData();
	APropData(const AString& sKey,const AString& sDisp);
	virtual ~APropData();
	virtual int GetType(){ return PROP_ITEM_DATA; }
};

class AUI_API APropGroup : public APropItem
{
	CLASS_INFO(PropGroup,PropItem);
public:
	APropGroup();
	virtual ~APropGroup();
	virtual int GetType(){ return PROP_ITEM_GROUP; }

	void AddProp(APropItem* pItem);
	bool RemoveProp(APropItem* pItem);

	bool	m_bExpanded;
	AString	m_sDispValue;
	AArray	m_aChild;
};

class AUI_API APropEdit: public APanel
{
	CLASS_INFO(PropEdit,Panel);
public:
	APropEdit(AComponent* pOwner);
	virtual ~APropEdit();

	void AddProp(APropItem* pItem);
	void ResetAll();
	void Render();

	virtual void SetWidth(int v);
	virtual void SetHeight(int v);

	virtual void DoPaint(ACanvas* cs,AEvent* pEvent);
	virtual void SetOnChange(AObject* p,FOnPropChange func);

	AEventHandler		OnItemDblClick;
protected:
	virtual void _PaintSpliter(AControl* pControl,HDC hDC);
	virtual void _PaintItem(AControl* pControl,HDC hDC);
	virtual void _PaintIndent(AControl* pControl,HDC hDC);
	virtual void RenderItem(AObject* pParent,APropItem* pItem,int iCurLevel,bool bVisible);

	virtual void OnSplitMouseMove(AObject* pSender,AEvent* pEvent);
	virtual void OnSplitMouseDown(AObject* pSender,AEvent* pEvent);
	virtual void OnSplitMouseUp(AObject* pSender,AEvent* pEvent);

	virtual void OnIndentMouseDown(AObject* pSender,AEvent* pEvent);
	virtual void OnDataClick(AObject* pSender,AEvent* pEvent);

	virtual void OnEditChange(AObject* pSender,AEvent* pEvent);
	virtual void OnCheckboxClick(AObject* pSender,AEvent* pEvent);
	virtual void OnColorButtonClick(AObject* pSender,AEvent* pEvent);
	virtual void OnComboSelChange(AObject* pSender,AEvent* pEvent);
	virtual void OnComboTextChange(AObject* pSender,AEvent* pEvent);

	void UpdatePos();
	void UpdateItemPos(APropItem* pItem);
	void UpdateVisible(APropItem* pItem,bool bVisible);
	void UpdateExpand();

	AArray		m_aItem;
	APanel*		m_pSpliter;
	int		m_iOldSplitPos;

	AObject*	m_pEventReceiver;
	FOnPropChange	m_funcOnChange;

};
#endif//APROPEDIT_H
