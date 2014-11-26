#ifndef AUIDEF_H
#define AUIDEF_H



#ifdef AUI_EXPORTS
#define AUI_API __declspec(dllexport)
#else
#define AUI_API __declspec(dllimport)
#endif

typedef enum{alNone,alLeft,alTop,alRight,alBottom,alClient}eAlign;
typedef enum{sbVert,sbHorz}eScrollBarKind;
enum TCloseAction { caNone, caHide, caFree, caMinimize };
enum TKeyStateFlag{ ksLeftButton=1,ksRightButton=2,ksMiddleButton=4,ksShift=8,ksAlt=16,ksCtrl=32};
enum TKeyEventType{ketPress,ketDown,ketUp};

enum TImageDrawStyle{idsNormal,idsStretch,idsTitle};
enum TTextAlign{taLeftTop,taCenterTop,taRightTop,taLeftMiddle,taCenterMiddle,taRightMiddle,taLeftBottom,taCenterBottom,taRightBottom};
enum TListViewStyle{lvsReport,lvsList,lvsIcon};
enum TFloatType{ftFloatNone,ftFloatX,ftFloatY,ftFloat};
enum TCursorType{ctAppStarting,ctArrow,ctCross,ctHand,ctHelp,ctIBeam,ctNo,ctSizeAll,ctSizeNESW,ctSizeNS,ctSizeNWSE,ctSizeWE,ctUpArrow,ctWait,ctUser};
enum TClipboardFormat{ cf_text,cf_unicodetext,cf_bitmap };
enum TDialogButton{dbOK=1,dbCancel=2,dbYes=4,dbNo=8};
enum TWindowPos{wpDefault,wpDesign,wpCenterScreen};
//========================= 跟绘图有关的定义 ===================================
//--------------------------------------------------------------------------
// Dash style constants
//--------------------------------------------------------------------------

enum DashStyle
{
	DashStyleSolid,          // 0
	DashStyleDash,           // 1
	DashStyleDot,            // 2
	DashStyleDashDot,        // 3
	DashStyleDashDotDot,     // 4
	DashStyleCustom          // 5
};

//--------------------------------------------------------------------------
// Line join constants
//--------------------------------------------------------------------------

enum LineJoin
{
	LineJoinMiter        = 0,
	LineJoinBevel        = 1,
	LineJoinRound        = 2,
	LineJoinMiterClipped = 3
};

//--------------------------------------------------------------------------
// Brush types
//--------------------------------------------------------------------------

enum BrushType
{
	BrushTypeSolidColor		= 0,
	BrushTypeHatchFill		= 1,
	BrushTypeTextureFill		= 2,
	BrushTypePathGradient		= 3,
	BrushTypeLinearGradient		= 4,
	BrushTypeRadialGradient		= 5
};

//--------------------------------------------------------------------------
// FontStyle: face types and common styles
//--------------------------------------------------------------------------

enum FontStyle
{
	FontStyleRegular    = 0,
	FontStyleBold       = 1,
	FontStyleItalic     = 2,
	FontStyleBoldItalic = 3,
	FontStyleUnderline  = 4,
	FontStyleStrikeout  = 8
};

//---------------------------------------------------------------------------
// String alignment flags
//---------------------------------------------------------------------------

enum StringAlignment
{
	// Left edge for left-to-right text,
	// right for right-to-left text,
	// and top for vertical
	StringAlignmentNear   = 0,
	StringAlignmentCenter = 1,
	StringAlignmentFar    = 2
};

enum StringAlignmentV
{
	// Left edge for left-to-right text,
	// right for right-to-left text,
	// and top for vertical
	StringAlignmentTop   = 0,
	StringAlignmentMiddle = 1,
	StringAlignmentBottom    = 2
};

//--------------------------------------------------------------------------
// Quality mode constants
//--------------------------------------------------------------------------

enum QualityMode
{
	QualityModeInvalid   = -1,
	QualityModeDefault   = 0,
	QualityModeLow       = 1, // Best performance
	QualityModeHigh      = 2  // Best rendering quality
};

//--------------------------------------------------------------------------
// Alpha Compositing mode constants
//--------------------------------------------------------------------------

enum CompositingMode
{
	CompositingModeSourceOver,    // 0
	CompositingModeSourceCopy     // 1
};

//--------------------------------------------------------------------------
// Alpha Compositing quality constants
//--------------------------------------------------------------------------

enum CompositingQuality
{
	CompositingQualityInvalid          = QualityModeInvalid,
	CompositingQualityDefault          = QualityModeDefault,
	CompositingQualityHighSpeed        = QualityModeLow,
	CompositingQualityHighQuality      = QualityModeHigh,
	CompositingQualityGammaCorrected,
	CompositingQualityAssumeLinear
};

//---------------------------------------------------------------------------
// Smoothing Mode
//---------------------------------------------------------------------------

enum SmoothingMode
{
	SmoothingModeInvalid     = QualityModeInvalid,
	SmoothingModeDefault     = QualityModeDefault,
	SmoothingModeHighSpeed   = QualityModeLow,
	SmoothingModeHighQuality = QualityModeHigh,
	SmoothingModeNone,
	SmoothingModeAntiAlias
};

//--------------------------------------------------------------------------
// Unit constants
//--------------------------------------------------------------------------

enum Unit
{
	UnitWorld,      // 0 -- World coordinate (non-physical unit)
	UnitDisplay,    // 1 -- Variable -- for PageTransform only
	UnitPixel,      // 2 -- Each unit is one device pixel.
	UnitPoint,      // 3 -- Each unit is a printer's point, or 1/72 inch.
	UnitInch,       // 4 -- Each unit is 1 inch.
	UnitDocument,   // 5 -- Each unit is 1/300 inch.
	UnitMillimeter  // 6 -- Each unit is 1 millimeter.
};

enum MatrixOrder
{
	MatrixOrderPrepend    = 0,
	MatrixOrderAppend     = 1
};

typedef double REAL;

// MVC 视图更新模式
enum ViewUpdateType
{
	vutAll,//完全更新
	vutModify,//元素属性变化
	vutAdd,//增加元素
	vutDel,//删除元素
	vutClear//清空元素
};

enum TextAction
{
	taInsert,
	taRemove
};

enum TextHint
{
	thNone,//不美化
	thDefault,//基础美化
	thFull//最强美化
};

enum _MessageType
{
	MESSAGE_TYPE_INFOR,
	MESSAGE_TYPE_WARN,
	MESSAGE_TYPE_QUESTION,
	MESSAGE_TYPE_ERROR,
	MESSAGE_TYPE_WAIT,
	MESSAGE_TYPE_INPUT
};

#define PROP_SET(x) if( pNode->HasProp(_T(#x)) ) x.SetMCF( pNode->GetProp(_T(#x)) )


//=========== 对ES的支持的功能扩展[begin] ====================
typedef enum _AUIESDataType
{
	aetInt,//整数
	aetDouble,//浮点数
	aetString,//字符串
	aetArray,//一维数组
	aetNode,//树状结构
	aetArray2//二维数组
}AUI_ES_TYPE;

struct AUI_ES_VALUE_INTF
{
	//类型
	virtual AUI_ES_TYPE GetType()=0;
	virtual void SetType(AUI_ES_TYPE t)=0;

	//整数操作
	virtual int GetInt()=0;
	virtual void SetInt(int v)=0;

	//浮点数操作
	virtual double GetDouble()=0;
	virtual void SetDouble(double v)=0;

	//字符串操作
	virtual AString GetString()=0;
	virtual void SetString(const AString& s)=0;

	//一维数组操作
	virtual void GetArray(AStringArray& sa)=0;
	virtual void SetArray(AStringArray& sa)=0;

	//树节点操作
	virtual void GetNode(AMLBody* pNode)=0;
	virtual void SetNode(AMLBody* pNode)=0;

	//二维数组操作（AObjectArray嵌套AStringArray）
	virtual AObjectArray* GetArray2()=0;
};

//值群组，用于参数组
struct AUI_ES_VALUE_GROUP_INTF
{
	virtual int GetCount()=0;
	virtual AUI_ES_VALUE_INTF* Add()=0;
	virtual AUI_ES_VALUE_INTF* GetItem(int index)=0;
};
//=========== 对ES的支持的功能扩展[end] ====================

#endif//AUIDEF_H