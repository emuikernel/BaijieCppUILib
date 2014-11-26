#ifndef ATABLECONTAINER_H
#define ATABLECONTAINER_H

class AUI_API ATableContainer : public AContainer
{
	CLASS_INFO(TableContainer,Container);
public:
	ATableContainer(AComponent* pOwner);
	virtual ~ATableContainer();

	virtual void DoLayout();

	virtual int GetColCount();
	virtual void SetColCount(int v);

	virtual int GetRowCount();
	virtual void SetRowCount(int v);

	//对ES的支持
	virtual void ESGet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESSet(const AString& sPath,AUI_ES_VALUE_INTF* pValue);
	virtual void ESCall(const AString& sPath,AUI_ES_VALUE_GROUP_INTF* args,AUI_ES_VALUE_INTF* pRetValue);

	virtual void SetPropFromNode(AMLBody* pNode);
	virtual void CreateChildren(AMLBody* pNode);
private:
	int		m_iColCount;
	int		m_iRowCount;
};

#endif//ATABLECONTAINER_H