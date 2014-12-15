#include "stdafx.h"
#include "WebKitX.h"
#include "WebKitXCtrl.h"
#include "afxdialogex.h"
#include "WebKitHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CWebKitXCtrl, COleControl)

CWebKitXCtrl* CWebKitXCtrl::g_instnace = NULL;
extern CefRefPtr<WebKitHandler> g_handler = NULL;
bool CWebKitXCtrl::CEF_INITIALIZED = false;

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
	DISP_FUNCTION_ID(CWebKitXCtrl, "Modified", dispidModified, Modified, VT_BOOL, VTS_NONE)
	DISP_FUNCTION_ID(CWebKitXCtrl, "Reload", dispidReload, Reload, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CWebKitXCtrl, "addEventListener", dispidaddEventListener, addEventListener, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_I4 VTS_BOOL)
	DISP_FUNCTION_ID(CWebKitXCtrl, "addEventListenerEx", dispidaddEventListenerEx, addEventListenerEx, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_DISPATCH VTS_BSTR VTS_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CWebKitXCtrl, "Created", dispidCreated, m_Created, OnCreatedChanged, VT_BOOL)
	DISP_FUNCTION_ID(CWebKitXCtrl, "Repaint", dispidRepaint, Repaint, VT_EMPTY, VTS_NONE)	
	DISP_FUNCTION_ID(CWebKitXCtrl, "SetStyle", dispidSetStyle, SetStyle, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CWebKitXCtrl, "Editable", dispidEditable, m_Editable, OnEditableChanged, VT_BOOL)
	DISP_FUNCTION_ID(CWebKitXCtrl, "SelectElement", dispidSelectElement, SelectElement, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event map
BEGIN_EVENT_MAP(CWebKitXCtrl, COleControl)
	EVENT_CUSTOM_ID("OnReady", eventidOnReady, OnReady, VTS_NONE)
	EVENT_CUSTOM_ID("OnCreate", eventidOnCreate, OnCreate, VTS_NONE)
	EVENT_CUSTOM_ID("OnFocus", eventidOnFocus, OnFocus, VTS_BSTR)
	EVENT_CUSTOM_ID("OnModified", eventidOnModified, OnModified, VTS_NONE)
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
CWebKitXCtrl::CWebKitXCtrl() :	
	m_Browser(NULL), 
	m_MainHwnd(NULL), 
	m_BrowserHwnd(NULL), 
	SIG_READY(NULL),
	CEF_BROWSER_CREATED(false),
	m_Created(VARIANT_FALSE),
	hook(NULL)
{
	InitializeIIDs(&IID_DWebKitX, &IID_DWebKitXEvents);	
	g_instnace = this;		
	debugPrint("CWebKitXCtrl Created %d.\n", (int)this);
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

		if(hook)
			UnhookWindowsHookEx(hook);
	}		
	debugPrint("CWebKitXCtrl Terminated %d.\n", (int)this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if(!pdc) return;		
	DoSuperclassPaint(pdc, rcBounds);
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	if(AmbientUserMode() && !CEF_BROWSER_CREATED && m_hWnd)
	{
		debugPrint("OnDraw\n");
		CEF_BROWSER_CREATED = true;		
		if(InitCEF())
			CreateCEFBrowser();	
	}
	if(m_Browser)
	{
		::InvalidateRect(m_BrowserHwnd, NULL, TRUE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef CEF_VERSION_1

bool CWebKitXCtrl::InitCEF()
{
	if(!CWebKitXCtrl::CEF_INITIALIZED)
	{
		debugPrint("Initializing CEF1\n");
		CefSettings settings;
		settings.multi_threaded_message_loop			= true;
		settings.graphics_implementation				= ANGLE_IN_PROCESS; // H/W Accelerated Canvas & WebGL
		settings.pack_loading_disabled					= true;
		settings.auto_detect_proxy_settings_enabled		= true;
		settings.uncaught_exception_stack_size			= 10;			

		CefRefPtr<CefApp> app;
		CWebKitXCtrl::CEF_INITIALIZED = CefInitialize(settings, app);		

		if(CWebKitXCtrl::CEF_INITIALIZED)
			debugPrint("CEF1 Initialized\n");
		else
			debugPrint("ERROR: CEF1 Initialization Failed\n");
	}
	return CWebKitXCtrl::CEF_INITIALIZED;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  CWebKitXCtrl::QuitCEF()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::CreateCEFBrowser()
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
	browser_settings.tab_to_links_disabled = true;
	browser_settings.text_area_resize_disabled = false;
	browser_settings.web_security_disabled = true;
	browser_settings.webgl_disabled = false;	
	browser_settings.animation_frame_rate = 30;

	// FOR HTML5 EDITOR MODE YOU BETTER DISABLE JAVASCRIPT AND PLUGINS
	browser_settings.java_disabled = true;
	browser_settings.javascript_access_clipboard_disallowed = true;
	browser_settings.javascript_close_windows_disallowed = true;
	browser_settings.javascript_disabled = true;
	browser_settings.javascript_open_windows_disallowed = true;
	browser_settings.page_cache_disabled = true;
	browser_settings.plugins_disabled = true;		

	g_handler = new WebKitHandler(this);

	debugPrint("Creating CEF Browser...\n");
	if(CefBrowser::CreateBrowser(info,	static_cast<CefRefPtr<CefClient>>(g_handler), "about:blank", browser_settings))
	{				
		SIG_READY = CreateEvent(NULL, true, false, NULL);		
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::DestroyCEFBrowser()
{
	if(m_Browser)
	{
		debugPrint("Destroying CEF Browser...\n");
		m_Browser->StopLoad();
		m_Browser->ParentWindowWillClose();
	}
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
			debugPrint("Resizing Browser...\n");
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

// ==================================================================================================================================
//	 _       __     __    __ __ _ __ _  __    ___    ____  ____
//	| |     / /__  / /_  / //_/(_) /| |/ /   /   |  / __ \/  _/
//	| | /| / / _ \/ __ \/ ,<  / / __/   /   / /| | / /_/ // /  
//	| |/ |/ /  __/ /_/ / /| |/ / /_/   |   / ___ |/ ____// /   
//	|__/|__/\___/_.___/_/ |_/_/\__/_/|_|  /_/  |_/_/   /___/   
//	                                                           
// ==================================================================================================================================

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Add Event Listener to Visual Basic 6 Class Public Function
void CWebKitXCtrl::addEventListenerEx(LPCTSTR Selector, LPCTSTR Event, IDispatch* vbObject, LPCTSTR vbObjectFunctionName, VARIANT_BOOL UseCapture)
{
	USES_CONVERSION;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CefPostTask(TID_UI,	NewCefRunnableFunction(&ExecuteAddEventEx, std::string(T2A(Selector)), std::string(T2A(Event)), vbObject, std::string(T2A(vbObjectFunctionName)), UseCapture));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::ExecuteAddEventEx(std::string elementID, std::string eventType, IDispatch* vbObject, std::string vbObjectFunctionName, VARIANT_BOOL UseCapture)
{		
	REQUIRE_UI_THREAD();	

	struct T
	{
		IDispatch* vbObject;
		std::string vbObjectFunctionName;
		VARIANT_BOOL UseCapture;
	};

	T handler;
	handler.vbObject = vbObject;
	handler.vbObjectFunctionName = vbObjectFunctionName;	
	handler.UseCapture = UseCapture;

	debugPrint("addEventListenerEx(%s, %s, %d, %s)\n", elementID.c_str(), eventType.c_str(), (LONG)handler.vbObject, handler.vbObjectFunctionName.c_str());

	//------------------------------------------------------------------------------------
	class Listener : public CefDOMEventListener 
	{
	public:		
		T handler;
		Listener(T handler)
		{
			REQUIRE_UI_THREAD();	
			this->handler = handler;			
		}
		virtual void HandleEvent(CefRefPtr<CefDOMEvent> event) 
		{
			USES_CONVERSION;
			REQUIRE_UI_THREAD();	
			CefRefPtr<CefDOMNode> element = event->GetTarget();
			DISPID disp;
			LPOLESTR fnName = A2OLE(this->handler.vbObjectFunctionName.c_str());			
			HRESULT hr = this->handler.vbObject->GetIDsOfNames(IID_NULL, &fnName, 1, LOCALE_SYSTEM_DEFAULT, &disp);
			if(SUCCEEDED(hr))
			{
				DISPPARAMS dispparamsNoArgs;			
				dispparamsNoArgs.cArgs=0;
				dispparamsNoArgs.cNamedArgs=0;
				this->handler.vbObject->Invoke(disp, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparamsNoArgs, NULL, NULL, NULL);
			}
		}
		IMPLEMENT_REFCOUNTING(Listener);	
	};

	//------------------------------------------------------------------------------------
	class Visitor : public CefDOMVisitor 
	{
	public:
		T handler;
		std::string elementID;
		std::string eventType;
		Visitor(std::string elementID, std::string eventType, T handler) 
		{
			REQUIRE_UI_THREAD();	
			this->handler = handler;
			this->elementID = elementID;
			this->eventType = eventType;
		}
		virtual void Visit(CefRefPtr<CefDOMDocument> document) 	OVERRIDE
		{
			REQUIRE_UI_THREAD();	
			CefRefPtr<CefDOMEventListener> listener(new Listener(this->handler));
			CefRefPtr<CefDOMNode> element = document->GetElementById(this->elementID);
			if(element || element.get())
				element->AddEventListener(this->eventType, listener, this->handler.UseCapture);
		}
		IMPLEMENT_REFCOUNTING(Visitor);
	};

	g_instnace->m_Browser->GetFocusedFrame()->VisitDOM(new Visitor(elementID, eventType, handler));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Add Event Listener to Visual Basic 6 Global Function Callback
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::addEventListener(LPCTSTR Selector, LPCTSTR Event, LONG AddressOfEventHandler, VARIANT_BOOL UseCapture)
{
	USES_CONVERSION;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());		
	CefPostTask(TID_UI,	NewCefRunnableFunction(&ExecuteAddEvent<VISUAL_BASIC_6_FN_PTR,LONG>, std::string(T2A(Selector)), std::string(T2A(Event)), (VISUAL_BASIC_6_FN_PTR)AddressOfEventHandler, UseCapture));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename F, typename P> void CWebKitXCtrl::ExecuteAddEvent(std::string elementID, std::string eventType, F handler, VARIANT_BOOL UseCapture)
{		 
	REQUIRE_UI_THREAD();	
	g_instnace->__addEventHandler<F,P>(elementID, eventType, handler, UseCapture); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename F, typename P> void CWebKitXCtrl::__addEventHandler(std::string elementID, std::string eventType, F handler, VARIANT_BOOL UseCapture)
{
	REQUIRE_UI_THREAD();	

	debugPrint("addEventListener(%s, %s)\n", elementID.c_str(), eventType.c_str());
	
	//------------------------------------------------------------------------------------
	class Listener : public CefDOMEventListener 
	{
	public:		
		F handler;
		Listener(F handler)
		{
			this->handler = handler;
		}
		virtual void HandleEvent(CefRefPtr<CefDOMEvent> event) 
		{				
			handler( (P) &event);
		}
		IMPLEMENT_REFCOUNTING(Listener);	
	};

	//------------------------------------------------------------------------------------
	class Visitor : public CefDOMVisitor 
	{
	public:
		F handler;
		std::string elementID;
		std::string eventType;
		VARIANT_BOOL UseCapture;
		Visitor(std::string elementID, std::string eventType, F handler, VARIANT_BOOL UseCapture) 
		{
			REQUIRE_UI_THREAD();	
			this->handler = handler;
			this->elementID = elementID;
			this->eventType = eventType;
			this->UseCapture = UseCapture;			
		}
		virtual void Visit(CefRefPtr<CefDOMDocument> document) 	OVERRIDE
		{
			REQUIRE_UI_THREAD();	
			CefRefPtr<CefDOMEventListener> listener(new Listener(this->handler));
			if(this->elementID=="document")
			{
				document->GetDocument()->AddEventListener(this->eventType, listener, this->UseCapture==VARIANT_TRUE);
			}
			else
			{
				CefRefPtr<CefDOMNode> element = document->GetElementById(this->elementID);
				if(element || element.get())
					element->AddEventListener(this->eventType, listener, false);
			}
		}
		IMPLEMENT_REFCOUNTING(Visitor);
	};

	m_Browser->GetFocusedFrame()->VisitDOM(new Visitor(elementID, eventType, handler, UseCapture));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VARIANT_BOOL CWebKitXCtrl::Modified(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	return m_bModified ? VARIANT_TRUE : VARIANT_FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::OpenURL(LPCTSTR URL)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());		
	if(!AmbientUserMode() || !m_Browser) return;
	REQUIRE_UI_THREAD();	
	debugPrint("Open URL %s.\n", URL);
	m_Browser->StopLoad();
	m_Browser->GetMainFrame()->LoadURL(CefString(URL));	
	SetModifiedFlag(FALSE);	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::Reload(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!AmbientUserMode() || !m_Browser) return;
	REQUIRE_UI_THREAD();	
	if(m_Browser)
		m_Browser->Reload();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::SetHTML(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!AmbientUserMode() || !m_Browser) return;
	REQUIRE_UI_THREAD();
	debugPrint("Put HTML %s.\n", newVal);
	m_Browser->StopLoad();
	m_Browser->GetMainFrame()->LoadStringW(CefString(newVal), "http://localhost");	
	SetModifiedFlag(FALSE);
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
void CWebKitXCtrl::Edit(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	if(AmbientUserMode() && m_Browser)
	{
		debugPrint("Edit Mode\n");
		
		// Set Editable Content
		__set_attribute("body", "contentEditable", "true");

		// Hook Mouse
		DWORD threadId = GetWindowThreadProcessId(m_Browser->GetWindowHandle(), NULL);
		hook = SetWindowsHookEx(WH_MOUSE, MouseHook, NULL, threadId);

		m_Editable=VARIANT_TRUE;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::AttachEditDOMEvents()
{
	REQUIRE_UI_THREAD();	
	g_instnace->__addEventHandler<EVENT_HANDLER_FN, CefRefPtr<CefDOMEvent>*>(std::string("document"), std::string("mousedown"), &CWebKitXCtrl::__HandleDOMEvent, VARIANT_TRUE);
	g_instnace->__addEventHandler<EVENT_HANDLER_FN, CefRefPtr<CefDOMEvent>*>(std::string("document"), std::string("DOMSubtreeModified"), &CWebKitXCtrl::__HandleDOMEvent, VARIANT_TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __stdcall CWebKitXCtrl::__HandleDOMEvent(CefRefPtr<CefDOMEvent>* e)
{
	if(!g_instnace->m_Editable) return;
	CefRefPtr<CefDOMEvent> DOMEvnet = *e;
	
	// Get event type (eg. mousedown)
	std::string type(DOMEvnet->GetType());

	// Get DOM path
	std::vector<std::string> path;
	CefRefPtr<CefDOMNode> node = DOMEvnet->GetTarget();
	while(node)
	{
		std::string id(node->GetElementAttribute("id"));
		if(id.length()>0) id = "[" + id + "]";
		std::string tag(node->GetElementTagName());
		std::string item = std::string(tag+id);		
		path.insert(path.begin(), item);
		node = node->GetParent();
	}	
	std::string target = join(path.begin(), path.end(), std::string("/"));

	// Trace
	debugPrint("DOMEvent: %s, target: %s\n", type.c_str(), target.c_str());

	// Fire OnFocus ActiveX Event
	if(type=="DOMSubtreeModified") 
	{		
		g_instnace->OnModified();
	}
	else
	{		
		CComBSTR btarget(target.c_str());
		g_instnace->FireEvent(eventidOnFocus, EVENT_PARAM(VTS_BSTR), btarget);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::Preview(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());		
	if(AmbientUserMode() && m_Browser)
	{
		debugPrint("Preview Mode\n");
		__set_attribute("body", "contentEditable", "false");
		m_Editable=VARIANT_FALSE;
		if(hook)
		{
			UnhookWindowsHookEx(hook);
			hook=NULL;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::__set_attribute(std::string selector, std::string attrName, std::string attrValue)
{
	REQUIRE_UI_THREAD();	
	
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
		}
		IMPLEMENT_REFCOUNTING(Visitor);
	};	

	this->selector = selector;
	this->attrName = attrName;
	this->attrValue = attrValue;
	m_Browser->GetMainFrame()->VisitDOM(new Visitor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::__set_style(std::string selector, std::string attrName, std::string attrValue)
{
	REQUIRE_UI_THREAD();	
	
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
		}
		IMPLEMENT_REFCOUNTING(Visitor);
	};	

	this->selector = selector;
	this->attrName = attrName;
	this->attrValue = attrValue;
	m_Browser->GetMainFrame()->VisitDOM(new Visitor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::OnCreatedChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	REQUIRE_UI_THREAD();		
	SetModifiedFlag();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::Repaint(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	REQUIRE_UI_THREAD();	
	RECT rc;
	GetClientRect(&rc);
	CefRect rect;
	rect.Set(0,0,rc.right, rc.bottom);
	m_Browser->Invalidate(rect);
	::SendMessage(m_BrowserHwnd, WM_PAINT, 0 , 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::SetStyle(LPCTSTR Selector, LPCTSTR StyleName, LPCTSTR StyleValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	REQUIRE_UI_THREAD();	
	__set_style(std::string(T2A(Selector)), std::string(T2A(StyleName)), std::string(T2A(StyleValue)));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::OnEditableChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	REQUIRE_UI_THREAD();	
	SetModifiedFlag();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::SelectElement(LPCTSTR Selector)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	REQUIRE_UI_THREAD();	
	//m_Browser->GetMainFrame()->ExecuteJavaScript()
}
