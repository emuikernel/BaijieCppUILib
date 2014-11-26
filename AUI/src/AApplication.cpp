#include "stdafx.h"
#include "AInternalImageCache.h"

static AApplication g_Application(NULL);


AApplication::AApplication(AComponent* pOwner):
	base_class(pOwner),
	m_pMainForm(NULL),
	m_MCFCall(NULL)
{
	g_DragInfo.bDragging = false;

	if( g_pInternalImageCache == NULL )
	{
		g_pInternalImageCache = new AInternalImageCache(this);
	}

	m_pCurSkin = new ASkin(this);
	m_hInstance = NULL;
	for(int i=0;i<ApplicationCursorCount;i++) m_aCursor[i] = NULL;
}

AApplication::~AApplication()
{
	//begin====
	//防止在winmain中进入Application.Run之前出现异常
	//在AApplication析构前给个机会来销毁主窗口
	AForm* p = GetMainForm();
	if( p && !p->IsDestroyed() && ::IsWindow(p->GetHandle()) )
	{
		if( p->GetWindow()) p->GetWindow()->SetPostQuitFlag(true);
		p->Destroy();
		p->MessageLoop();
	}
	//end=====
	OutputDebugString(_T("^^^^^^ 进入AApplication析构^^^^^^^"));
}

AApplication* AApplication::Get()
{
	return &g_Application;
}

//如果需要释放（抛弃）一个对象，就用Discard，它会正确地调用Destroy或Free
void AApplication::Discard(AObject* p)
{
	AWinControlBase* pBase = dynamic_cast<AWinControlBase*>(p);
	if( pBase )
	{
		pBase->Destroy();
		return;
	}
	AComponent* pCom = dynamic_cast<AComponent*>(p);
	if( pCom )
	{
		pCom->Free();
		return;
	}
	if( p )
		p->Release();
}
void AApplication::Init()
{
	g_pInternalImageCache->LoadDefault();
}

void AApplication::Clean()
{
	m_aControlCreator.Clear();
	m_aChildComponent.Clear();
}

HCURSOR AApplication::GetCursor(TCursorType ct)
{
	if( (int)ct < 15 )
	{
		if( m_aCursor[ct] == NULL && ct < ctUser ) 
		{
			switch( ct )
			{
			case ctAppStarting:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_APPSTARTING);
				break;
			case ctArrow:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_ARROW);
				break;
			case ctCross:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_CROSS);
				break;
			case ctHand:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_HAND);
				break;
			case ctHelp:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_HELP);
				break;
			case ctIBeam:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_IBEAM);
				break;
			case ctNo:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_NO);
				break;
			case ctSizeAll:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_SIZEALL);
				break;
			case ctSizeNESW:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_SIZENESW);
				break;
			case ctSizeNS:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_SIZENS);
				break;
			case ctSizeNWSE:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_SIZENWSE);
				break;
			case ctSizeWE:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_SIZEWE);
				break;
			case ctUpArrow:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_UPARROW);
				break;
			case ctWait:
				m_aCursor[ct] = ::LoadCursor(NULL,IDC_WAIT);
				break;
			case ctUser:
				//fix me later
				break;
			default:
				break;
			}
		}
		return m_aCursor[ct];
	}
	return NULL;
}
HINSTANCE AApplication::GetHandle()
{
	if( m_hInstance == NULL ) return ::GetModuleHandle(NULL);
	return m_hInstance;
}

void AApplication::SetHandle(HINSTANCE h)
{
	m_hInstance = h;
}
void AApplication::Run()
{
	AWinControlBase::MessageLoop();
	Clean();
	OutputDebugString(_T("\r\n/////////////// 退出Application.Run /////////////  \r\n"));
}

int AApplication::MsgBox(const AString& sText,const AString& sCaption,UINT nTyppe)
{
	return ::MessageBox(NULL,sText.Text(),sCaption.Text(),0);
}

AForm* AApplication::GetMainForm()
{
	if( m_pMainForm && !m_pMainForm->IsDestroyed() ) 
		return m_pMainForm;
	int i,iCount = GetChildren()->GetCount();
	for(i=0;i<iCount;i++)
	{
		AComponent* p = dynamic_cast<AControl*>(GetChildren()->GetItem(i));
		if( p != NULL )
		{
			AForm* pForm = (AForm *)p->ToClass( _T("Form") );
			if( pForm != NULL )
			{
				if( pForm->IsDestroyed() == false )
				m_pMainForm = pForm;
				break;
			}
		}
	}

	return m_pMainForm;
}

void AApplication::SetCurSkin(ASkin* pSkin)
{
	m_pCurSkin = pSkin;
}

ASkin* AApplication::GetCurSkin()
{
	return m_pCurSkin;
}


void AApplication::RegisterControlCreator(const AString& sControlType,TControlCreator cc)
{
	m_aControlCreator.SetItem(sControlType,(void *)cc);
}

void AApplication::RegisterMCFCall(TCallMCF c)
{
	m_MCFCall = c;
}

TCallMCF AApplication::GetMCFCall()
{
	return m_MCFCall;
}

void AApplication::_InitOSVersion()
{
#ifndef SM_SERVERR2
#define SM_SERVERR2 89
#endif
	SYSTEM_INFO info;        //用SYSTEM_INFO结构判断64位AMD处理器   
	GetSystemInfo(&info);    //调用GetSystemInfo函数填充结构   
	OSVERSIONINFOEX os;   
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);     

	AString osname = _T("unknown OperatingSystem.");  
	AString vmark;
	m_iOSVersion = 400;//init
	if(GetVersionEx((OSVERSIONINFO *)&os))  
	{   
		//下面根据版本信息判断操作系统名称   
		switch(os.dwMajorVersion)//判断主版本号  
		{  
		case 4:  
			switch(os.dwMinorVersion)//判断次版本号   
			{   
			case 0:  
				if(os.dwPlatformId==VER_PLATFORM_WIN32_NT)  
					osname =_T("Microsoft Windows NT 4.0"); //1996年7月发布   
				else if(os.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)  
					osname =_T("Microsoft Windows 95");  
				m_iOSVersion = 400;
				break;  
			case 10:  
				osname =_T("Microsoft Windows 98");  
				m_iOSVersion = 410;
				break;  
			case 90:  
				osname =_T("Microsoft Windows Me");  
				m_iOSVersion = 490;
				break;  
			}  
			break;  

		case 5:  
			switch(os.dwMinorVersion)   //再比较dwMinorVersion的值  
			{   
			case 0:  
				osname =_T("Microsoft Windows 2000");//1999年12月发布  
				m_iOSVersion = 500;
				if(os.wSuiteMask==VER_SUITE_ENTERPRISE)   
					vmark=_T("Advanced Server"); 
				break;  

			case 1:  
				osname =_T("Microsoft Windows XP");//2001年8月发布  
				m_iOSVersion = 501;
				if(os.wSuiteMask==VER_SUITE_EMBEDDEDNT)   
					vmark=_T("Embedded");   
				else if(os.wSuiteMask==VER_SUITE_PERSONAL)   
					vmark=_T("Home Edition");   
				else   
					vmark=_T("Professional"); 
				break;  

			case 2:  
				if(os.wProductType==VER_NT_WORKSTATION   
					&& info.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)  
				{  
					osname =_T("Microsoft Windows XP Professional x64 Edition");  
				}  
				else if(GetSystemMetrics(SM_SERVERR2)==0)  
					osname =_T("Microsoft Windows Server 2003");//2003年3月发布   
				else if(GetSystemMetrics(SM_SERVERR2)!=0)  
					osname =_T("Microsoft Windows Server 2003 R2");  
				m_iOSVersion = 502;
				if(GetSystemMetrics(SM_SERVERR2)==0   
					&&os.wSuiteMask==VER_SUITE_BLADE)  //Windows Server 2003   
					vmark=_T("Web Edition");  
				else if(GetSystemMetrics(SM_SERVERR2)==0   
					&& os.wSuiteMask==VER_SUITE_COMPUTE_SERVER)  
					vmark=_T("Compute Cluster Edition");  
				else if(GetSystemMetrics(SM_SERVERR2)==0  
					&& os.wSuiteMask==VER_SUITE_STORAGE_SERVER)  
					vmark=_T("Storage Server");  
				else if(GetSystemMetrics(SM_SERVERR2)==0   
					&& os.wSuiteMask==VER_SUITE_DATACENTER)  
					vmark=_T("Datacenter Edition");  
				else if(GetSystemMetrics(SM_SERVERR2)==0   
					&& os.wSuiteMask==VER_SUITE_ENTERPRISE)  
					vmark=_T("Enterprise Edition");  
				else if(GetSystemMetrics(SM_SERVERR2)!=0  
					&& os.wSuiteMask==VER_SUITE_STORAGE_SERVER)  
					vmark=_T("Storage Server");  
				break;  
			}  
			break;  

		case 6:  
			switch(os.dwMinorVersion)  
			{  
			case 0:  
				if(os.wProductType == VER_NT_WORKSTATION)  
					osname =_T("Microsoft Windows Vista");  
				else  
					osname =_T("Microsoft Windows Server 2008");//服务器版本   
				m_iOSVersion = 600;
				if(os.wProductType!=VER_NT_WORKSTATION   
					&& os.wSuiteMask==VER_SUITE_DATACENTER)  
					vmark=_T("Datacenter Server");  
				else if(os.wProductType!=VER_NT_WORKSTATION   
					&& os.wSuiteMask==VER_SUITE_ENTERPRISE)  
					vmark=_T("Enterprise");  
				else if(os.wProductType==VER_NT_WORKSTATION   
					&& os.wSuiteMask==VER_SUITE_PERSONAL)  //Windows Vista  
					vmark =_T("Home");  
				break;  
			case 1:  
				if(os.wProductType == VER_NT_WORKSTATION)  
					osname =_T("Microsoft Windows 7");  
				else  
					osname =_T("Microsoft Windows Server 2008 R2");  
				m_iOSVersion = 601;
				break;  
			}  
			break;  
		}  
	}//if(GetVersionEx((OSVERSIONINFO *)&os)) 
	m_sOSName = osname;
	m_sOSVersionMark = vmark;
}