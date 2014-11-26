#ifndef AFCDEF_H
#define AFCDEF_H

#ifdef AFC_EXPORTS
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
	BrushTypeSolidColor       = 0,
	BrushTypeHatchFill        = 1,
	BrushTypeTextureFill      = 2,
	BrushTypePathGradient     = 3,
	BrushTypeLinearGradient   = 4
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

typedef float REAL;

//控件常量定义




#endif//AFCDEF_H