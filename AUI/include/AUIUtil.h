#ifndef AUIUTIL_H
#define AUIUTIL_H

//控件名称与对象的关联
#define MAP_CONTROL(controlType,controlName) \
	protected:\
	controlType*	m_##controlName;\
	public:\
	virtual controlType* Get##controlName()\
		{\
		if( m_##controlName == NULL )\
			{\
			m_##controlName = dynamic_cast<controlType*>( FindControl(_T(#controlName)) );\
			}\
			if( m_##controlName == NULL ) throw AException(AString(_T("访问不存在的控件("))+_T(#controlName)+_T(") ！"));\
			return m_##controlName;\
		}

//初始化关联的控件变量
#define MAP_CONTROL_INIT(controlName) m_##controlName=NULL

class AUI_API AUTIL
{
public:
	static void TextAlignToStringAlignmeng(TTextAlign ta,StringAlignment& ha,StringAlignmentV &va);
	static int Point2Pixel(int pt,bool x);
	static AString FloatTypeToString(TFloatType ft);
	static TFloatType StringToFloatType(const AString& s);
	static AString AnchorsToString(const AAnchors& a);
	static void StringToAnchors(const AString& s,AAnchors& a);
	static AString AlignToString(eAlign e);
	static eAlign StringToAlign(const AString& s);
	static AString ListViewStyleToString(TListViewStyle e);
	static TListViewStyle StringToListViewStyle(const AString& s);
	static bool IsSeperatorChar(ACHAR ch);
};

//类注册器
#define REG_CLASS(class_name) \
AObject* class_name##_ControlCreator(AComponent* pOwner)\
{\
	return new class_name(pOwner);\
}\
\
void RegClass_##class_name()\
{\
	AApplication::Get()->RegisterControlCreator(_T(#class_name),class_name##_ControlCreator);\
}\
\
class auto_reg_class_##class_name\
{\
public:\
	auto_reg_class_##class_name()\
	{\
		RegClass_##class_name();\
	}\
	~auto_reg_class_##class_name(){}\
};\
auto_reg_class_##class_name g_auto_##class_name




#endif//AUIUTIL_H