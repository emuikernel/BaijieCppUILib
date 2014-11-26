#ifndef AMEMOTEXTSTORAGEIMPL_H
#define AMEMOTEXTSTORAGEIMPL_H

#include "AMemoTextMatrix.h"
#include <map>

/*
 * 编辑框的文本管理器，提供文本的存储、块、行、字符的操作。暂时不支持列操作
 * 
 */
class AUI_API AMemoTextStorageImpl : public AMemoTextStorage
{
	CLASS_INFO(MemoTextStorageImpl,MemoTextStorage);
public:
	AMemoTextStorageImpl();
	virtual ~AMemoTextStorageImpl();

	//=== 整块文本操作 ===
	virtual void SetSingleText(const AString& sText);
	virtual AString GetSingleText(const AString& sCR=_T("\r\n"));
	virtual void Clear();//清空所有文本
	virtual ASize GetEditorSize();

	//=== 行操作 =====
	virtual int GetLineCount();//得到总行数
	virtual int GetLineCharCount(int iLine);
	virtual AString GetLine(int iLine);//得到指定行内容
	virtual void AddLine(const AString& sText);//增加行
	virtual void SetLine(int iLine,const AString& sNewText);//行替换
	virtual void RemoveLine(int iLine);//删除行
	virtual void InsertLine(int iLine,const AString& sText);//插入行
	
	//=== 单个字符操作 ===
	virtual void InsertChar(ACHAR ch,ACaretSeat seat);//插入字符到指定位子
	virtual ACHAR GetChar(ACaretSeat seat);//得到指定位子的字符
	virtual void RemoveChar(ACaretSeat seat);//删除指定位子的字符

	//=== 文本块操作 ====
	virtual void InsertString(const AString& s,ACaretSeat seat);//插入文本块到指定位子
	virtual AString GetString(ACaretSeat seatStart,ACaretSeat seatEnd);//获取指定位子范围的文本块
	virtual void RemoveString(ACaretSeat seatStart,ACaretSeat seatEnd);//删除指定范围的文本块
	virtual void ReplaceString(ACaretSeat seatStart,ACaretSeat seatEnd,const AString& s);//用指定文本块替换指定范围的文本块；兼任实现单个字符的替换功能

	//=== 列操作 ===
	virtual void InsertColBlock(AStringArray& s,ACaretSeat seatFrom);//插入列块
	virtual void GetColBlock(AStringArray& s,ACaretSeat seatStart,ACaretSeat seatEnd);//得到列块
	virtual void RemoveColBlock(ACaretSeat seatStart,ACaretSeat seatEnd);//删除列块
	virtual void ReplaceColBlock(AStringArray& s,ACaretSeat seatStart,ACaretSeat seatEnd);//列块替换

	//=== 度量信息操作 ===
	virtual int GetMeasure(ACaretSeat seat);//得到指定位子的度量值（宽度，相对于行起始位置）
	virtual void SetMeasure(ACaretSeat seat,int v);//设置指定位子的度量值
	virtual int GetLineHeight(int iRow);//得到行高
	virtual int GetLineWidth(int iRow);//得到行宽
	virtual int GetLineTop(int iRow);//得到行的顶点
	virtual void SetLineHeight(int iRow,int iHeight);//设置行高
	virtual int GetRowSpace();//得到行间距
	virtual void SetRowSpace(int v);//设置行间距

	//=== 着色信息操作 ===
	virtual int GetColoration(ACaretSeat seat);//得到指定位置的着色索引
	virtual void SetColoration(ACaretSeat seat,int iIndex);//设置指定位置的着色索引
	virtual int RegColorationItem(AMemoColorationItem* pItem,const AString& sKey);//注册一个着色信息，返回它对应的下标
	virtual void UnregColorationItem(const AString& sKey);//通过Key来反注册一个着色信息
	virtual void UnregColorationItem(int iIndex);//通过索引下标来反注册一个着色信息
	virtual int GetColorationIndex(const AString& sKey);//根据Key获取下标
	virtual AMemoColorationItem* GetColorationByIndex(int index);

	//=== 参数
	virtual void SetTabWidth(int v);
	virtual void SetDefaultFont(AFont f);

	//=== 定位
	virtual ACaretSeat SeatByPos(APoint pt);

	
protected:
	AMemoTextMatrix		m_Matrix;
	int			m_iRowSpace;
	AObjectArray		m_aColoration;
	AStringMap		m_aColorationMap;
	int			m_iTabWidth;//TAB宽度（影响到度量）
	ASize			m_szEditor;
	int			m_iRightSpace;//右边留白
	int			m_iBottomSpace;//下边留白
	//=== 用于度量的缺省字体
	AFont	m_DefaultFont;


	//根据文本的改变更新格式化和度量信息
	virtual void DoMeasure(ATextEvent* pEvent);
	virtual void DoTint(ATextEvent* pEvent);

	//辅助函数
	virtual void _doMeasure(int rs,int cs,int re,int ce);
	virtual void _doTint(int rs,int cs,int re,int ce,TextAction ta=taInsert);
	virtual void _onchange();

	virtual void _measureBlock(ACanvas* cs,ACaretSeat seat,int iCount);//单行内块度量
};

#endif//AMEMOTEXTSTORAGEIMPL_H