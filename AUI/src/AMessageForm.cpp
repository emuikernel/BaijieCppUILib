#include "stdafx.h"
#include "AMessageForm.h"
#include "resource.h"

static ACHAR* g_szDialogWnd=
	L"<c type=@MessageForm@ w=@423@ h = @437@ szb=@0@  >"
	L"<children>"
		L"<c type=@TitleBar@ n=@TitleBar@ al=@t@ h=@28@>"
		L"<children>"
			L"<c type=@CloseButton@ n=@cb@ l=@0@ t=@2@ h=@26@ w = @26@ anc=@rt@ anc_r=@2@ />"
		L"</children>"
		L"</c>"
		L"<c type=@Container@ n=@Content@ pd=@5@ al=@c@ />"
		L"<c type=@Container@ n=@BottomContainer@ al=@b@ h=@46@ />"
	L"</children>"
	L"</c>"
	;

AMessageForm::AMessageForm(AComponent* pOwner):AForm(pOwner)
{
	m_nButtonID = NULL;
	m_iMessageType = MESSAGE_TYPE_INFOR;
	m_pOKButton = NULL;
	m_pCancelButton = NULL;
	m_pYesButton = NULL;
	m_pNoButton = NULL;
	m_sInputText = NULL;

	MAP_CONTROL_INIT(Content);
	MAP_CONTROL_INIT(BottomContainer);
}

AMessageForm::~AMessageForm()
{

}

void AMessageForm::CreateFromXML(TDialogButton db)
{
	m_dbs = db;
	afc_object<AMLBody> pBody(new AMLBody());
	AString xml;
	xml = g_szDialogWnd;
	xml.Replace(_T("@"),_T("\""));
	pBody->FromXML(xml);
	this->FromXML(&pBody);
	this->CenterToScreen();
}

void AMessageForm::ConnectEvent()
{

}



void AMessageForm::OnOKClick(AObject* pSender,AEvent* pEvent)
{
	
	if( m_nButtonID ) 
	{
		*m_nButtonID = dbOK;
	}
	if( m_iMessageType == MESSAGE_TYPE_INPUT )
	{
		if( m_sInputText ) *m_sInputText = FindControl(_T("Input"))->GetText();
	}
	Destroy();
}
void AMessageForm::OnCancelClick(AObject* pSender,AEvent* pEvent)
{
	if( m_nButtonID ) *m_nButtonID = dbCancel;
	Destroy();
}

void AMessageForm::OnYesClick(AObject* pSender,AEvent* pEvent)
{
	if( m_nButtonID ) 
	{
		*m_nButtonID = dbYes;
	}
	Destroy();
}
void AMessageForm::OnNoClick(AObject* pSender,AEvent* pEvent)
{
	if( m_nButtonID ) *m_nButtonID = dbNo;
	Destroy();
}




void AMessageForm::InitControls()
{
	AutoDisableLayout adl(m_pUICore);

	int delta = 5;
	int iIconWidth = 48;
	//初始化图标
	AContainer* pContent = GetContent();
	
	
	int iconID=0;
	switch(m_iMessageType)
	{
	case MESSAGE_TYPE_INFOR:iconID = IDI_ICON_INFOR; break;
	case MESSAGE_TYPE_WARN:iconID = IDI_ICON_WARNING;break;
	case MESSAGE_TYPE_QUESTION:iconID = IDI_ICON_QUESTION;break;
	case MESSAGE_TYPE_ERROR:iconID = IDI_ICON_ERROR;break;
	case MESSAGE_TYPE_WAIT:iconID = IDI_ICON_WAITING;break;
	default:
		break;
	}
	if( iconID != 0 )
	{
		AImageControl* pImageControl = new AImageControl(pContent);
		pImageControl->SetLeft(delta);
		pImageControl->SetTop(delta);
		pImageControl->SetWidth(iIconWidth);
		pImageControl->SetHeight(iIconWidth);
		//pImageControl->SetDrawStyle(idsStretch);

		AImage* pImage = AImage::CreateImage();
		pImage->LoadIcon( ::LoadIcon(g_hInstance,MAKEINTRESOURCE(iconID)) );
		pImageControl->SetGraphic(pImage);
	}
	

	//初始化文本
	ACanvas* cs = m_pUICore->GetCanvas();
	AStringArray sa;
	sa.FromString(m_sUserText,_T("\n"));
	ASize szText(0,0);
	AFont f;//缺省字体
	int iw=0;
	int ih=0;
	for(int i=0;i<sa.GetCount();i++)
	{
		
		ASize sz( (sa[i].Length()) * f.GetSize(),f.GetSize());//缺省度量
		if( cs ) sz = cs->MeasureText(sa[i]);
		if( sz.cx > szText.cx ) szText.cx = sz.cx;
		szText.cy += sz.cy + 6;
	}
	szText.cy += 23;
	ALabel* pTextLabel = new ALabel(pContent);
	if( m_iMessageType == MESSAGE_TYPE_INPUT )
	{
		pTextLabel->SetLeft(16);
		pTextLabel->SetTop(10);
		pTextLabel->SetWidth(300);
		pTextLabel->SetHeight(20);

		AEdit* pEdit = new AEdit(pContent);
		pEdit->SetLeft(16);
		pEdit->SetTop(40);
		pEdit->SetWidth(200);
		pEdit->SetHeight(22);
		pEdit->SetName(_T("Input"));
	}
	else
	{
		pTextLabel->SetLeft(delta*2+96);
		pTextLabel->SetTop(delta);
		pTextLabel->SetWidth(szText.cx);
		pTextLabel->SetHeight(szText.cy+2 * delta);
	}
	
	pTextLabel->SetText(m_sUserText);
	pTextLabel->SetTextAlign(taLeftMiddle);
	//pTextLabel->Font.SetSize(16);
	//pTextLabel->SetUseSkin(false);

	int iWidth = delta * 3 + 32 + szText.cx+100;
	if( iWidth < 320 ) iWidth = 320;
	if( m_iMessageType == MESSAGE_TYPE_INPUT ) iWidth = 320;
	this->SetWidth(iWidth);
	this->SetHeight(28+(iIconWidth > szText.cy ? iIconWidth : szText.cy )+46+delta*2);
	if( m_iMessageType == MESSAGE_TYPE_INPUT ) this->SetHeight(160);

	//初始化按钮
	int iButtonCount = 0;
	if( m_dbs & dbOK) iButtonCount ++;
	if( m_dbs & dbCancel ) iButtonCount ++;
	if( m_dbs & dbYes ) iButtonCount ++;
	if( m_dbs & dbNo ) iButtonCount ++;
	int iButtonWidth = 72;
	int iButtonHeight = 22;
	if( iButtonCount > 0 )
	{
		int iButtonSpace = 10;//间隔距离
		int iLeft = iButtonSpace * (iButtonCount-1) + iButtonWidth * iButtonCount;
		iLeft = (iWidth - iLeft)/2;
		int index=0;
		AContainer* pButtonContainer = GetBottomContainer();
		
		if( m_dbs & dbOK )
		{
			m_pOKButton = new AButton(pButtonContainer);
			m_pOKButton->SetWidth( iButtonWidth );
			m_pOKButton->SetLeft( iLeft );
			m_pOKButton->SetHeight( iButtonHeight );
			m_pOKButton->SetTop( 10 );
			m_pOKButton->SetText(_T("确定"));
			EVENT_CONNECT(m_pOKButton,OnClick,OnOKClick);
			iLeft += iButtonWidth+iButtonSpace;
		}

		if( m_dbs & dbYes )
		{
			m_pYesButton = new AButton(pButtonContainer);
			m_pYesButton->SetWidth( iButtonWidth );
			m_pYesButton->SetLeft( iLeft );
			m_pYesButton->SetHeight( iButtonHeight );
			m_pYesButton->SetTop( 10 );
			m_pYesButton->SetText(_T("是"));
			EVENT_CONNECT(m_pYesButton,OnClick,OnYesClick);
			iLeft += iButtonWidth+iButtonSpace;
		}

		if( m_dbs & dbNo )
		{
			m_pNoButton = new AButton(pButtonContainer);
			m_pNoButton->SetWidth( iButtonWidth );
			m_pNoButton->SetLeft( iLeft );
			m_pNoButton->SetHeight( iButtonHeight );
			m_pNoButton->SetTop( 10 );
			m_pNoButton->SetText(_T("否"));
			EVENT_CONNECT(m_pNoButton,OnClick,OnNoClick);
			iLeft += iButtonWidth+iButtonSpace;
		}

		if( m_dbs & dbCancel )
		{
			m_pCancelButton = new AButton(pButtonContainer);
			m_pCancelButton->SetWidth( iButtonWidth );
			m_pCancelButton->SetLeft( iLeft );
			m_pCancelButton->SetHeight( iButtonHeight );
			m_pCancelButton->SetTop( 10 );
			m_pCancelButton->SetText(_T("取消"));
			EVENT_CONNECT(m_pCancelButton,OnClick,OnCancelClick);
			iLeft += iButtonWidth+iButtonSpace;
		}
	}

	//初始化标题
	switch(m_iMessageType)
	{
	case MESSAGE_TYPE_INFOR:this->SetText(_T("提示"));break;
	case MESSAGE_TYPE_WARN:this->SetText(_T("警告"));break;
	case MESSAGE_TYPE_QUESTION:this->SetText(_T("询问"));break;
	case MESSAGE_TYPE_ERROR:this->SetText(_T("错误"));break;
	case MESSAGE_TYPE_WAIT:this->SetText(_T("等待"));break;
	default:
		break;
	}
}


void AMessageForm::DoCreate()
{

}




