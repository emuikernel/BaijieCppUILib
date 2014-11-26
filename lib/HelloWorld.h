#ifndef AFORM1_H
#define AFORM1_H


class AForm1UI : public AForm
{
	CLASS_INFO(Form1UI,Form);
public:
	AForm1UI(AComponent* pOwner):base_class(pOwner)
	{
		MAP_CONTROL_INIT(Form1);
		MAP_CONTROL_INIT(Button1);
	}
	virtual ~AForm1UI()
	{
	}

	virtual void InitControls()
	{
	}
	virtual void ConnectEvent()
	{
	}
	virtual void DoCreate()
	{
	}
	virtual void CreateFromXML()
	{
		static ACHAR* szForm1 = _T("<c OnShow=\"\" szb=\"1\" type=\"Form\" bw=\"1\" h=\"333\" l=\"159\" n=\"Form1\" t=\"220\" w=\"477\" OnResize=\"\" en=\"1\" OnCreate=\"\" OnClose=\"\" bcr=\"-15725560\" pd=\"0\" tx=\"Form1\" OnHide=\"\" vi=\"0\" OnMove=\"\" OnCloseQuery=\"\" pos=\"s\">
    <children>
        <c al=\"\" type=\"Button\" OnDblClick=\"\" OnMouseMove=\"\" bw=\"1\" h=\"46\" l=\"98\" n=\"Button1\" OnClick=\"\" t=\"96\" w=\"191\" OnFocus=\"\" OnMouseUp=\"\" en=\"1\" ft=\"\" OnPaint=\"\" hnt=\"\" bcr=\"-14545407\" OnKeyboard=\"\" OnKillFocus=\"\" pd=\"0\" OnMouseDown=\"\" tx=\"Hello,World\" OnLeave=\"\" vi=\"1\" anc=\"lt\" anc_b=\"\" anc_r=\"\" OnMouseWheel=\"\" OnEnter=\"\">
            <font n=\"ËÎÌו\" cr=\"-15725560\" st=\"\" sz=\"12\" />
        </c>
    </children>
</c>
");
		xobject<AMLBody> pBody(new AMLBody());
		pBody->FromXML(szForm1);
		this->FromXML(&pBody);
	}

	MAP_CONTROL(AForm,Form1);
	MAP_CONTROL(AButton,Button1);

	
};

#endif//AFORM1_H