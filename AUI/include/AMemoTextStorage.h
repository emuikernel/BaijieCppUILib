#ifndef AMEMOTEXTSTORAGE_H
#define AMEMOTEXTSTORAGE_H

//着色信息
class AUI_API AMemoColorationItem : public AObject
{
	CLASS_INFO(MemoColorationItem,Object);
public:
	AMemoColorationItem():base_class(){}
	virtual ~AMemoColorationItem(){}

	AFont		m_Font;//字体
	AColor		m_BackColor;//背景色
};

/*
 * 编辑框的文本管理器，提供文本的存储、块、行、字符的操作。同时提供格式化信息和字符度量信息的存储。
 * TextStorage的实现必须保障格式化和度量信息与文本信息的同步！
 */
class AUI_API AMemoTextStorage : public AObject
{
	CLASS_INFO(MemoTextStorage,Object);
public:
	//=== 整块文本操作 ===
	virtual void SetSingleText(const AString& sText)=0;
	virtual AString GetSingleText(const AString& sCR=_T("\r\n"))=0;
	virtual void Clear()=0;//清空所有文本
	virtual ASize GetEditorSize()=0;

	//=== 行操作 =====
	virtual int GetLineCount()=0;//得到总行数
	virtual int GetLineCharCount(int iLine)=0;//得到行内字符总数
	virtual AString GetLine(int iLine)=0;//得到指定行内容
	virtual void AddLine(const AString& sText)=0;//增加行
	virtual void SetLine(int iLine,const AString& sNewText)=0;//行替换
	virtual void RemoveLine(int iLine)=0;//删除行
	virtual void InsertLine(int iLine,const AString& sText)=0;//插入行
	
	//=== 单个字符操作 ===
	virtual void InsertChar(ACHAR ch,ACaretSeat seat)=0;//插入字符到指定位子
	virtual ACHAR GetChar(ACaretSeat seat)=0;//得到指定位子的字符
	virtual void RemoveChar(ACaretSeat seat)=0;//删除指定位子的字符

	//=== 文本块操作 ====
	virtual void InsertString(const AString& s,ACaretSeat seat)=0;//插入文本块到指定位子
	virtual AString GetString(ACaretSeat seatStart,ACaretSeat seatEnd)=0;//获取指定位子范围的文本块
	virtual void RemoveString(ACaretSeat seatStart,ACaretSeat seatEnd)=0;//删除指定范围的文本块
	virtual void ReplaceString(ACaretSeat seatStart,ACaretSeat seatEnd,const AString& s)=0;//用指定文本块替换指定范围的文本块；兼任实现单个字符的替换功能

	//=== 列操作 ===
	virtual void InsertColBlock(AStringArray& s,ACaretSeat seatFrom)=0;//插入列块
	virtual void GetColBlock(AStringArray& s,ACaretSeat seatStart,ACaretSeat seatEnd)=0;//得到列块
	virtual void RemoveColBlock(ACaretSeat seatStart,ACaretSeat seatEnd)=0;//删除列块
	virtual void ReplaceColBlock(AStringArray& s,ACaretSeat seatStart,ACaretSeat seatEnd)=0;//列块替换

	//=== 度量信息操作 ===
	virtual int GetMeasure(ACaretSeat seat)=0;//得到指定位子的度量值（宽度，相对于行起始位置）
	virtual void SetMeasure(ACaretSeat seat,int v)=0;//设置指定位子的度量值
	virtual int GetLineHeight(int iRow)=0;//得到行高
	virtual int GetLineWidth(int iRow)=0;//得到行宽
	virtual void SetLineHeight(int iRow,int iHeight)=0;//设置行高
	virtual int GetLineTop(int iRow)=0;//得到行的顶点
	virtual int GetRowSpace()=0;//得到行间距
	virtual void SetRowSpace(int v)=0;//设置行间距

	//=== 着色信息操作 ===
	virtual int GetColoration(ACaretSeat seat)=0;//得到指定位置的着色索引
	virtual void SetColoration(ACaretSeat seat,int iIndex)=0;//设置指定位置的着色索引
	virtual int RegColorationItem(AMemoColorationItem* pItem,const AString& sKey)=0;//注册一个着色信息，返回它对应的下标
	virtual void UnregColorationItem(const AString& sKey)=0;//通过Key来反注册一个着色信息
	virtual void UnregColorationItem(int iIndex)=0;//通过索引下标来反注册一个着色信息
	virtual int GetColorationIndex(const AString& sKey)=0;//根据Key获取下标
	virtual AMemoColorationItem* GetColorationByIndex(int index)=0;

	//=== 参数
	virtual void SetTabWidth(int v)=0;
	virtual void SetDefaultFont(AFont f)=0;

	//=== 定位
	virtual ACaretSeat SeatByPos(APoint pt)=0;

	//=== 通过响应以下事件来修改或扩展编辑框的功能 ===
	//     事件参数都是ATextEvent类型  
	AEventHandler		OnMeasure;//需要度量
	AEventHandler		OnTint;//需要着色
	AEventHandler		OnChange;//文本有变化
};

#endif//AMEMOTEXTSTORAGE_H