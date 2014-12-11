#include "stdafx.h"
#include "WebKitX.h"
#include "WebKitXCtrl.h"
#include "afxdialogex.h"
#include "WebKitHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CWebKitXCtrl, COleControl)

CWebKitXCtrl* CWebKitXCtrl::g_instnace;
extern CefRefPtr<WebKitHandler> g_handler;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Message map
BEGIN_MESSAGE_MAP(CWebKitXCtrl, COleControl)		
	ON_MESSAGE(WM_SIZE, &CWebKitXCtrl::OnResize)
	ON_MESSAGE(OCM_COMMAND, &CWebKitXCtrl::OnOcmCommand)
	ON_MESSAGE(WM_SETCURSOR, &CWebKitXCtrl::CancelMessage)
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dispatch map
BEGIN_DISPATCH_MAP(CWebKitXCtrl, COleControl)
	DISP_FUNCTION_ID(CWebKitXCtrl, "OpenURL", dispidOpenURL, OpenURL, VT_EMPTY, VTS_BSTR)
	DISP_PROPERTY_EX_ID(CWebKitXCtrl, "HTML", dispidHTML, GetHTML, SetHTML, VT_BSTR)	
	DISP_FUNCTION_ID(CWebKitXCtrl, "Edit", dispidEdit, Edit, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CWebKitXCtrl, "Preview", dispidPreview, Preview, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event map
BEGIN_EVENT_MAP(CWebKitXCtrl, COleControl)
END_EVENT_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Property pages
LPCLSID CWebKitXCtrl::GetPropPageIDs(ULONG& cPropPages) { return 0; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid
IMPLEMENT_OLECREATE_EX(CWebKitXCtrl, "WEBKITX.WebKitXCtrl.1", 0xd9ea077f, 0x4e5f, 0x4bbd, 0x9a, 0xcd, 0x2, 0xe7, 0x2, 0x1c, 0xde, 0xfe)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type library ID and version
IMPLEMENT_OLETYPELIB(CWebKitXCtrl, _tlid, _wVerMajor, _wVerMinor)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interface IDs
const IID IID_DWebKitX = { 0xECEC4ADA, 0xDE49, 0x4358, { 0xB7, 0x8, 0xC3, 0x41, 0xF, 0x8C, 0xDD, 0xDA } };
const IID IID_DWebKitXEvents = { 0x3EE33618, 0xBC27, 0x40E9, { 0xB7, 0x3D, 0x87, 0xA4, 0x9D, 0xEF, 0xC, 0x8A } };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Control type information
static const DWORD _dwWebKitXOleMisc = OLEMISC_ACTIVATEWHENVISIBLE|OLEMISC_SETCLIENTSITEFIRST|OLEMISC_IGNOREACTIVATEWHENVISIBLE|OLEMISC_RENDERINGISDEVICEINDEPENDENT|OLEMISC_INSIDEOUT;
IMPLEMENT_OLECTLTYPE(CWebKitXCtrl, IDS_WEBKITX, _dwWebKitXOleMisc)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CWebKitXCtrl::CWebKitXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterControlClass(AfxGetInstanceHandle(), m_clsid, m_lpszProgID, IDS_WEBKITX,	IDB_WEBKITX, afxRegInsertable | afxRegApartmentThreading, _dwWebKitXOleMisc, _tlid, _wVerMajor, _wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWebKitXCtrl::CWebKitXCtrl() :	m_Browser(NULL), m_MainHwnd(NULL), m_BrowserHwnd(NULL), SIG_READY(NULL)
{
	InitializeIIDs(&IID_DWebKitX, &IID_DWebKitXEvents);	
	g_instnace = this;		
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWebKitXCtrl::~CWebKitXCtrl()
{	
	if(SIG_READY)
		CloseHandle(SIG_READY);

	if(AmbientUserMode() && m_Browser)
	{		
		g_instnace = NULL;
		m_Browser = NULL;		
		g_handler = NULL;

		// IMPORTANT: 
		// Here is the problem with this solution, once CefShutdown() 
		// is called the ActiveX cannot reinitialize CEF. If we don't 
		// call it then some thread is not released resulting to crash.
		CefShutdown();			

		// Ideally we should find a way to avoid CefShutdown/CefInitialize
		// and properly dispose CEF objects and threads that assume they
		// run inside an executable and not inside a host such as ActiveX.

	}
	debugPrint("CWebKitXCtrl terminated.\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if(!pdc) return;	
	static bool created = false;
	if(AmbientUserMode() && !created && m_hWnd)
	{		
		created = true;		
		CreateCEF();		
	}
	DoSuperclassPaint(pdc, rcBounds);
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef CEF_VERSION_1
void CWebKitXCtrl::CreateCEF()
{
	CefSettings settings;
	settings.multi_threaded_message_loop			= true;
	settings.graphics_implementation				= ANGLE_IN_PROCESS; // H/W Accelerated Canvas & WebGL
	settings.pack_loading_disabled					= true;
	settings.auto_detect_proxy_settings_enabled		= true;
	settings.uncaught_exception_stack_size			= 10;			

	CefRefPtr<CefApp> app;	

	if(CefInitialize(settings, app))
	{			
		RECT rect;
		::GetClientRect(m_hWnd, &rect);

		CefWindowInfo info;	
		info.SetAsChild(m_hWnd, rect);		

		CefBrowserSettings browser_settings;

		browser_settings.accelerated_2d_canvas_disabled = false;
		browser_settings.accelerated_compositing_enabled = true;
		browser_settings.accelerated_filters_disabled = false;
		browser_settings.accelerated_layers_disabled = false;
		browser_settings.accelerated_painting_disabled = false;
		browser_settings.accelerated_plugins_disabled = false;
		browser_settings.accelerated_video_disabled = false;
		browser_settings.animation_frame_rate = 40;
		browser_settings.application_cache_disabled = true;
		browser_settings.author_and_user_styles_disabled = false;			
		browser_settings.databases_disabled = false;
		browser_settings.developer_tools_disabled = false;
		browser_settings.dom_paste_disabled = false;
		browser_settings.drag_drop_disabled = false;
		browser_settings.encoding_detector_enabled = true;
		browser_settings.file_access_from_file_urls_allowed = true;
		browser_settings.history_disabled = true;
		browser_settings.hyperlink_auditing_disabled = true;
		browser_settings.site_specific_quirks_disabled = true;
		browser_settings.shrink_standalone_images_to_fit = false;
		browser_settings.tab_to_links_disabled = false;
		browser_settings.text_area_resize_disabled = false;
		browser_settings.web_security_disabled = true;
		browser_settings.webgl_disabled = false;
		browser_settings.caret_browsing_enabled = false;

		// FOR HTML5 EDITOR MODE YOU BETTER DISABLE JAVASCRIPT AND PLUGINS
		browser_settings.java_disabled = true;
		browser_settings.javascript_access_clipboard_disallowed = true;
		browser_settings.javascript_close_windows_disallowed = true;
		browser_settings.javascript_disabled = true;
		browser_settings.javascript_open_windows_disallowed = true;
		browser_settings.page_cache_disabled = true;
		browser_settings.plugins_disabled = true;		

		g_handler = new WebKitHandler(this);

		if(CefBrowser::CreateBrowser(info,	static_cast<CefRefPtr<CefClient>>(g_handler), "http://www.google.com", browser_settings))
		{				
			//SetTimer(IDT_TIMER, 1, TimerProc);
			//CefRunMessageLoop();				
			SIG_READY = CreateEvent(NULL, true, false, NULL);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::DestroyCEF()
{
	//KillTimer(IDT_TIMER);
	m_Browser->StopLoad();
	m_Browser->ParentWindowWillClose();
}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef CEF_VERSION_3
void CWebKitXCtrl::CreateCEF()
{	
	CefSettings settings;
	settings.no_sandbox								= true;
	settings.multi_threaded_message_loop			= true;
	settings.single_process							= false;
	settings.pack_loading_disabled					= true;
	settings.uncaught_exception_stack_size			= 10;			

	CefRefPtr<CefApp> app;	
	CefMainArgs main_args;

	int exit_code = CefExecuteProcess(main_args, app.get(), NULL);
	if (exit_code >= 0) return;

	if(CefInitialize(main_args, settings, app.get(), NULL))
	{

	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CefBrowserProcessHandler methods:
void CWebKitXCtrl::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();
	
	RECT rect;
	::GetClientRect(m_hWnd, &rect);

	CefWindowInfo window_info;	
	window_info.SetAsChild(m_hWnd, rect);		

	CefBrowserSettings browser_settings;
	browser_settings.java = STATE_DISABLED;
	browser_settings.javascript = STATE_DISABLED;
	browser_settings.plugins = STATE_DISABLED;
	browser_settings.local_storage = STATE_DISABLED;
	browser_settings.databases = STATE_DISABLED;
	browser_settings.webgl = STATE_ENABLED;

	g_handler = new WebKitHandler(this);

	if(CefBrowserHost::CreateBrowser(window_info, g_handler.get(), "", browser_settings, NULL))
	{				
		//SetTimer(IDT_TIMER, 1, TimerProc);
		//CefRunMessageLoop();				
		SIG_READY = CreateEvent(NULL, true, false, NULL);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::DestroyCEF()
{
	//KillTimer(IDT_TIMER);
	m_Browser->StopLoad();
}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWebKitXCtrl::DoPropExchange - Persistence support
void CWebKitXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWebKitXCtrl::OnResetState - Reset control to default state
void CWebKitXCtrl::OnResetState()
{
	COleControl::OnResetState(); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWebKitXCtrl::PreCreateWindow - Modify parameters for CreateWindowEx
BOOL CWebKitXCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = _T("STATIC");
	return COleControl::PreCreateWindow(cs);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWebKitXCtrl::IsSubclassedControl - This is a subclassed control
BOOL CWebKitXCtrl::IsSubclassedControl()
{
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWebKitXCtrl::OnOcmCommand - Handle command messages
LRESULT CWebKitXCtrl::OnOcmCommand(WPARAM wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CWebKitXCtrl::OnResize(WPARAM wParam, LPARAM lParam)
{
	if(AmbientUserMode() && m_Browser && m_BrowserHwnd && m_hWnd)
	{
		RECT rc1;
		RECT rc2;
		::GetClientRect(m_hWnd, &rc1);
		::GetClientRect(m_BrowserHwnd, &rc2);			
		if(rc1.left!=rc2.left || rc1.top!=rc2.top || rc1.right!=rc2.right || rc1.bottom!=rc2.bottom)
		{
			::SetParent(m_BrowserHwnd, m_hWnd);
			::SetWindowPos(m_BrowserHwnd, HWND_TOP, rc1.left, rc1.top, rc1.right-rc1.left, rc1.bottom-rc1.top, SWP_SHOWWINDOW);
		}
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CWebKitXCtrl::CancelMessage(WPARAM wParam, LPARAM lParam)
{
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::OpenURL(LPCTSTR URL)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	if(AmbientUserMode() && m_Browser)
		m_Browser->GetMainFrame()->LoadURL(CefString(URL));	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BSTR CWebKitXCtrl::GetHTML(void)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strResult;
	if(AmbientUserMode() && m_Browser)
	{
		ResetEvent(SIG_READY);
		CefPostTask(TID_UI,	NewCefRunnableFunction(&ExecuteGetSource, m_Browser->GetMainFrame()));
		WaitForSingleObject(SIG_READY, COMMAND_TIMEOUT_HIGH);
		strResult=response.c_str();
	}
	return strResult.AllocSysString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::ExecuteGetSource(CefRefPtr<CefFrame> frame)
{	
	#ifdef CEF_VERSION_1
	g_instnace->response = frame->GetSource();
	#endif

	#ifdef CEF_VERSION_3
	#endif

	SetEvent(g_instnace->SIG_READY);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::SetHTML(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!AmbientUserMode() || !m_Browser) return;
	REQUIRE_UI_THREAD();	
	m_Browser->GetMainFrame()->LoadStringW(CefString(newVal), "http://localhost");
	SetModifiedFlag();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::Edit(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	if(AmbientUserMode() && m_Browser)
		__set_attribute("body", "contentEditable", "true");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::Preview(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());		
	if(AmbientUserMode() && m_Browser)
		__set_attribute("body", "contentEditable", "false");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::__set_attribute(std::string selector, std::string attrName, std::string attrValue)
{
	class Visitor : public CefDOMVisitor
	{
	public:
		Visitor() {}
		virtual void Visit(CefRefPtr<CefDOMDocument> document) 
		{						
			CefRefPtr<CefDOMNode> body = document->GetBody();
			if(body.get())
			{
				body->SetElementAttribute(g_instnace->attrName, g_instnace->attrValue);				
			}				
			//SetEvent(g_instnace->SIG_READY);
		}
		IMPLEMENT_REFCOUNTING(Visitor);
	};	

	//ResetEvent(SIG_READY);
	{
		this->selector = selector;
		this->attrName = attrName;
		this->attrValue = attrValue;
		m_Browser->GetMainFrame()->VisitDOM(new Visitor);
	}
	//WaitForSingleObject(SIG_READY, COMMAND_TIMEOUT_LOW);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::__set_style(std::string selector, std::string attrName, std::string attrValue)
{
	class Visitor : public CefDOMVisitor
	{
	public:
		Visitor() {}
		virtual void Visit(CefRefPtr<CefDOMDocument> document) 
		{			
			CefRefPtr<CefDOMNode> body = document->GetBody();
			if(body.get())
			{
				body->SetElementAttribute(g_instnace->attrName, g_instnace->attrValue);				
			}				
			//SetEvent(g_instnace->SIG_READY);
		}
		IMPLEMENT_REFCOUNTING(Visitor);
	};	

	//ResetEvent(SIG_READY);
	{
		this->selector = selector;
		this->attrName = attrName;
		this->attrValue = attrValue;
		m_Browser->GetMainFrame()->VisitDOM(new Visitor);
	}
	//WaitForSingleObject(SIG_READY, COMMAND_TIMEOUT_LOW);
}
