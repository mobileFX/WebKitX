#include "stdafx.h"
#include "WebKitX.h"
#include "WebKitXCtrl.h"
#include "afxdialogex.h"
#include "WebKitHandler.h"


#include "strings.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CWebKitXCtrl, COleControl)

CWebKitXCtrl* CWebKitXCtrl::g_instnace = NULL;
extern CefRefPtr<WebKitHandler> g_handler = NULL;
std::string CWebKitXCtrl::focusNodes("");
bool CWebKitXCtrl::CEF_INITIALIZED = false;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__forceinline void WaitForSignal(HANDLE signal, DWORD TimeOut)
{
	MSG msg;
	for(;;)
	{	
		if(::WaitForSingleObject(signal, 0)==WAIT_OBJECT_0)
			return;

		if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string Replace(std::string str, const std::string& find, const std::string& replace)
{
	size_t start_pos = 0;
	while((start_pos = str.find(find, start_pos)) != std::string::npos) 
	{
		str.replace(start_pos, find.length(), replace);
		start_pos += replace.length();
	}
	return str;
}

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
	DISP_STOCKPROP_HWND()
	DISP_FUNCTION_ID(CWebKitXCtrl, "OpenURL", dispidOpenURL, OpenURL, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CWebKitXCtrl, "Edit", dispidEdit, Edit, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CWebKitXCtrl, "Preview", dispidPreview, Preview, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CWebKitXCtrl, "Modified", dispidModified, Modified, VT_BOOL, VTS_NONE)
	DISP_FUNCTION_ID(CWebKitXCtrl, "Reload", dispidReload, Reload, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CWebKitXCtrl, "addEventListener", dispidaddEventListener, addEventListener, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_I4 VTS_BOOL)
	DISP_FUNCTION_ID(CWebKitXCtrl, "addEventListenerEx", dispidaddEventListenerEx, addEventListenerEx, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_DISPATCH VTS_BSTR VTS_BOOL)	
	DISP_FUNCTION_ID(CWebKitXCtrl, "Repaint", dispidRepaint, Repaint, VT_EMPTY, VTS_NONE)	
	DISP_FUNCTION_ID(CWebKitXCtrl, "SetStyle", dispidSetStyle, SetStyle, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CWebKitXCtrl, "SelectElement", dispidSelectElement, SelectElement, VT_EMPTY, VTS_BSTR VTS_BOOL VTS_BOOL)		
	DISP_FUNCTION_ID(CWebKitXCtrl, "hBrowserHWND", dispidhBrowserHWND, hBrowserHWND, VT_HANDLE, VTS_NONE)
	DISP_FUNCTION_ID(CWebKitXCtrl, "LoadHTML", dispidLoadHTML, LoadHTML, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CWebKitXCtrl, "Created", dispidCreated, Created, VT_BOOL, VTS_NONE)
	DISP_FUNCTION_ID(CWebKitXCtrl, "SelectedHTML", dispidSelectedHTML, SelectedHTML, VT_BSTR, VTS_BOOL)		
	DISP_PROPERTY_EX_ID(CWebKitXCtrl, "HTML", dispidHTML, GetHTML, SetHTML, VT_BSTR)	
	DISP_FUNCTION_ID(CWebKitXCtrl, "ExecJavaScript", dispidExecJavaScript, ExecJavaScript, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CWebKitXCtrl, "ExecCommand", dispidExecCommand, ExecCommand, VT_BSTR, VTS_I4 VTS_VARIANT)
	DISP_FUNCTION_ID(CWebKitXCtrl, "TidyHTML", dispidTidyHTML, TidyHTML, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CWebKitXCtrl, "Editable", dispidEditable, Editable, VT_BOOL, VTS_NONE)
END_DISPATCH_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event map
BEGIN_EVENT_MAP(CWebKitXCtrl, COleControl)
	EVENT_CUSTOM_ID("OnReady", eventidOnReady, OnReady, VTS_NONE)
	EVENT_CUSTOM_ID("OnCreate", eventidOnCreate, OnCreate, VTS_NONE)
	EVENT_CUSTOM_ID("OnFocus", eventidOnFocus, OnFocus, VTS_BSTR)
	EVENT_CUSTOM_ID("OnModified", eventidOnModified, OnModified, VTS_NONE)	
	EVENT_STOCK_MOUSEDOWN()
	EVENT_CUSTOM_ID("OnSelectionChanged", eventidOnSelectionChanged, OnSelectionChanged, VTS_BSTR VTS_BSTR)
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
	SIG_HTML_READY(NULL),
	CEF_BROWSER_CREATED(false),
	m_ActiveXCreated(VARIANT_FALSE),
	hook(NULL),
	UID_COUNTER(0),
	LoadingHTML(false),
	SIG_HTML_SELECTION_READY(NULL),
	SIG_JAVASCRIPT_READY(NULL),
	SIG_TIDY_HTML_READY(NULL)

{
	InitializeIIDs(&IID_DWebKitX, &IID_DWebKitXEvents);	
	g_instnace = this;		

	EditorCommands.push_back("backColor");
	EditorCommands.push_back("bold");
	EditorCommands.push_back("contentReadOnly");
	EditorCommands.push_back("copy");
	EditorCommands.push_back("createLink");
	EditorCommands.push_back("cut");
	EditorCommands.push_back("decreaseFontSize");
	EditorCommands.push_back("delete");
	EditorCommands.push_back("enableInlineTableEditing");
	EditorCommands.push_back("enableObjectResizing");
	EditorCommands.push_back("fontName");
	EditorCommands.push_back("fontSize");
	EditorCommands.push_back("foreColor");
	EditorCommands.push_back("formatBlock");
	EditorCommands.push_back("forwardDelete");
	EditorCommands.push_back("heading");
	EditorCommands.push_back("hiliteColor");
	EditorCommands.push_back("increaseFontSize");
	EditorCommands.push_back("indent");
	EditorCommands.push_back("insertBrOnReturn");
	EditorCommands.push_back("insertHorizontalRule");
	EditorCommands.push_back("insertHTML");
	EditorCommands.push_back("insertImage");
	EditorCommands.push_back("insertOrderedList");
	EditorCommands.push_back("insertUnorderedList");
	EditorCommands.push_back("insertParagraph");
	EditorCommands.push_back("insertText");
	EditorCommands.push_back("italic");
	EditorCommands.push_back("justifyCenter");
	EditorCommands.push_back("justifyFull");
	EditorCommands.push_back("justifyLeft");
	EditorCommands.push_back("justifyRight");
	EditorCommands.push_back("outdent");
	EditorCommands.push_back("paste");
	EditorCommands.push_back("redo");
	EditorCommands.push_back("removeFormat");
	EditorCommands.push_back("selectAll");
	EditorCommands.push_back("strikeThrough");
	EditorCommands.push_back("subscript");
	EditorCommands.push_back("superscript");
	EditorCommands.push_back("underline");
	EditorCommands.push_back("undo");
	EditorCommands.push_back("unlink");
	EditorCommands.push_back("useCSS"); 
	EditorCommands.push_back("styleWithCSS");

	debugPrint("CWebKitXCtrl Created %d.\n", (int)this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWebKitXCtrl::~CWebKitXCtrl()
{	
	if(SIG_HTML_READY)
	{
		CloseHandle(SIG_HTML_READY);
		SIG_HTML_READY=NULL;
	}

	if(SIG_HTML_SELECTION_READY)
	{
		CloseHandle(SIG_HTML_SELECTION_READY);
		SIG_HTML_SELECTION_READY=NULL;
	}	

	if(SIG_JAVASCRIPT_READY)
	{
		CloseHandle(SIG_JAVASCRIPT_READY);
		SIG_JAVASCRIPT_READY=NULL;
	}

	if(SIG_TIDY_HTML_READY)
	{
		CloseHandle(SIG_TIDY_HTML_READY);
		SIG_TIDY_HTML_READY=NULL;
	}

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
	browser_settings.javascript_disabled = false;
	browser_settings.javascript_access_clipboard_disallowed = false;
	browser_settings.javascript_close_windows_disallowed = true;
	browser_settings.javascript_open_windows_disallowed = true;
	browser_settings.page_cache_disabled = true;
	browser_settings.plugins_disabled = false;		

	g_handler = new WebKitHandler(this);

	debugPrint("Creating CEF Browser...\n");
	if(CefBrowser::CreateBrowser(info,	static_cast<CefRefPtr<CefClient>>(g_handler), "about:blank", browser_settings))
	{				
		WebKitV8Extension::RegisterExtension(this);
		SIG_HTML_READY = CreateEvent(NULL, true, false, NULL);		
		SIG_HTML_SELECTION_READY = CreateEvent(NULL, true, false, NULL);
		SIG_JAVASCRIPT_READY = CreateEvent(NULL, true, false, NULL);
		SIG_TIDY_HTML_READY	= CreateEvent(NULL, true, false, NULL);
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
		SIG_HTML_READY = CreateEvent(NULL, true, false, NULL);
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
			Repaint();
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

VARIANT_BOOL CWebKitXCtrl::Editable(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_Editable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OLE_HANDLE CWebKitXCtrl::hBrowserHWND(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (OLE_HANDLE) m_BrowserHwnd;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VARIANT_BOOL CWebKitXCtrl::Created(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_ActiveXCreated;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VARIANT_BOOL CWebKitXCtrl::Modified(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	return IsModified()==FALSE ? VARIANT_FALSE : VARIANT_TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Add Event Listener to Visual Basic 6 Class Public Function
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		m_Browser->ReloadIgnoreCache();
	SetModifiedFlag(FALSE);	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::SetHTML(LPCTSTR HTML)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!AmbientUserMode() || !m_Browser) return;
	REQUIRE_UI_THREAD();	
	m_Browser->StopLoad();
	m_Browser->GetMainFrame()->LoadStringW(LPCTSTR_to_CefString(HTML,m_Editable==VARIANT_TRUE), "http://localhost");	
	SetModifiedFlag(FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::LoadHTML(LPCTSTR HTML, LPCTSTR URL)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!AmbientUserMode() || !m_Browser) return;
	REQUIRE_UI_THREAD();
	m_Browser->StopLoad();
	m_Browser->GetMainFrame()->LoadStringW(LPCTSTR_to_CefString(HTML,true), CefString(URL));	
	SetModifiedFlag(FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BSTR CWebKitXCtrl::GetHTML(void)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	if(AmbientUserMode() && m_Browser)
	{
		document_html = "";

		// Get HTML from CEF UI Thread
		ResetEvent(SIG_HTML_READY);
		CefPostTask(TID_UI,	NewCefRunnableFunction(&ExecuteGetSource, m_Browser->GetMainFrame()));
		WaitForSignal(SIG_HTML_READY, COMMAND_TIMEOUT_HIGH);		
	}
	return CComBSTR(document_html).Detach();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::ExecuteGetSource(CefRefPtr<CefFrame> frame)
{	
	#ifdef CEF_VERSION_1	
	g_instnace->document_html = CefString_to_BSTR(frame->GetSource(), true);	 
	#endif

	#ifdef CEF_VERSION_3
	#endif

	SetEvent(g_instnace->SIG_HTML_READY);
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
	g_instnace->__addEventHandler<EVENT_HANDLER_FN, CefRefPtr<CefDOMEvent>*>(std::string("document"), std::string("mouseup"), &CWebKitXCtrl::__HandleDOMEvent, VARIANT_TRUE);
	g_instnace->__addEventHandler<EVENT_HANDLER_FN, CefRefPtr<CefDOMEvent>*>(std::string("document"), std::string("selectionchange"), &CWebKitXCtrl::__HandleDOMEvent, VARIANT_TRUE);
	g_instnace->__addEventHandler<EVENT_HANDLER_FN, CefRefPtr<CefDOMEvent>*>(std::string("document"), std::string("DOMSubtreeModified"), &CWebKitXCtrl::__HandleDOMEvent, VARIANT_TRUE);	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __stdcall CWebKitXCtrl::__HandleDOMEvent(CefRefPtr<CefDOMEvent>* e)
{
	g_instnace->HandleDOMEvent(e);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::HandleDOMEvent(CefRefPtr<CefDOMEvent>* e)
{
	if(!m_Editable) return;

	static bool ReEnter	= false;
	if(ReEnter) return;
	ReEnter=true;

	CefRefPtr<CefDOMEvent> DOMEvent = *e;
	
	// Get event type (eg. mousedown)
	std::string type(DOMEvent->GetType());
	
	CefRefPtr<CefDOMNode> node = DOMEvent->GetTarget();
	//selectedNode = node;

	// Get DOM path
	std::vector<std::string> path;	
	while(node)
	{
		std::string tag(node->GetElementTagName());
		if(tag=="HTML") break;
		std::string uid(node->GetElementAttribute("__uid"));
		if(uid.size()==0)
		{
			char buff[255];
			memset(buff ,0, 255);
			sprintf(buff, "__uid_%d", ++UID_COUNTER);
			uid = std::string(buff);			
			node->SetElementAttribute("__uid", uid);
		}
		std::string id(node->GetElementAttribute("id"));
		if(id.length()>0) id = "[" + id + "]";		
		std::string item = tag+id+":"+uid;		
		path.insert(path.begin(), item);
		node = node->GetParent();		
	}	
	std::string target = join(path.begin(), path.end(), std::string("/"));

	// Trace
	debugPrint("DOMEvent: %s, target: %s\n", type.c_str(), target.c_str());

	//==============================================================================================================================================
	if(type=="DOMSubtreeModified") 
	{		
		FireOnModified();
	}
	//==============================================================================================================================================
	else if(type=="selectionchange")
	{
		document_html = CefString_to_BSTR(DOMEvent->GetTarget()->GetDocument()->GetDocument()->GetAsMarkup(), false);
		
		selection = "";

		std::string code =  "(function(){var sel=window.getSelection().focusNode; return (sel.nodeType==3?sel.parentNode:sel).outerHTML;})()";

		CefString _code(code.c_str());
		CefRefPtr<CefV8Value> result;
		CefRefPtr<CefV8Exception> exception;

		v8context->Enter();
		{
			v8context->Eval(_code, result, exception);
		}				
		v8context->Exit();

		if(result && result->IsString())
		{
			CefString html = result->GetStringValue();
			selection = CefString_to_BSTR(html, false);		
		}				

		FireOnSelectionChanged();
	}
	//==============================================================================================================================================
	else if(type=="mouseup")		
	{		
		focusNodes = target;		
		//FireOnMouseDown();		
		if(!LoadingHTML)
			FireOnFocus();		
	}

	ReEnter=false;
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
		std::string selector;
		std::string attrName;
		std::string attrValue;		
		Visitor(std::string selector, std::string attrName, std::string attrValue) 
		{
			this->selector = selector;
			this->attrName = attrName;
			this->attrValue = attrValue;
		}
		virtual void Visit(CefRefPtr<CefDOMDocument> document) 
		{						
			CefRefPtr<CefDOMNode> body = document->GetBody();
			if(body.get())
			{
				body->SetElementAttribute(attrName, attrValue);				
			}
		}
		IMPLEMENT_REFCOUNTING(Visitor);
	};	

	m_Browser->GetMainFrame()->VisitDOM(new Visitor(selector, attrName, attrValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::Repaint(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	InvalidateControl();
	REQUIRE_UI_THREAD();		
	RECT rc;
	GetClientRect(&rc);
	CefRect rect;
	rect.Set(0,0,rc.right, rc.bottom);
	m_Browser->Invalidate(rect);
	SendMessage(WM_PAINT,0,0);
	::SendMessage(m_BrowserHwnd, WM_PAINT, 0 , 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::SetStyle(LPCTSTR Selector, LPCTSTR StyleName, LPCTSTR StyleValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	REQUIRE_UI_THREAD();	
	//__set_style(std::string(T2A(Selector)), std::string(T2A(StyleName)), std::string(T2A(StyleValue)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::SelectElement(LPCTSTR Selector, VARIANT_BOOL MoveCaret, VARIANT_BOOL SelectContents)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	__selectSingleNode(std::string(T2A(Selector)), (bool)(MoveCaret!=0), (bool)(SelectContents!=0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CefRefPtr<CefDOMNode> CWebKitXCtrl::__selectSingleNode(std::string selector, bool MoveCaret, bool SelectContents)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());		
	CefPostTask(TID_UI, NewCefRunnableFunction(&ExecuteSelectNode, selector, MoveCaret, SelectContents));	
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::ExecuteSelectNode(std::string selector, bool MoveCaret, bool SelectContents)
{
	REQUIRE_UI_THREAD();

	if(g_instnace->v8context)
	{
		g_instnace->v8context->Enter();
		{
			std::string code = "(function(MoveCaret, SelectContents){\n" 
				"var target = document.querySelectorAll('" + selector + "')[0];\n"
				"if(!target) return;"
				"target.focus();\n"		
				"var range = document.createRange();\n"				
				"if(SelectContents)\n"
				"  range.selectNodeContents(target);\n"
				"else\n"
				"  range.selectNode(target);\n"
				"if(MoveCaret) {\n"
				"  var startNode = target.firstChild;\n"
				"  var endNode = target.firstChild;\n"
				"  range.setStart(startNode, 0);\n"
				"  range.setEnd(endNode, 0);\n"
				"}\n"
				"var sel = window.getSelection();\n"
				"sel.removeAllRanges();\n"
				"sel.addRange(range);\n"				
				"var sel=window.getSelection().focusNode;\n"
				"return (sel.nodeType==3?sel.parentNode:sel).outerHTML;\n"
				"})(" + (MoveCaret?"true":"false") + "," + (SelectContents?"true":"false") + ");";
			
			CefRefPtr<CefV8Value> retval;
			CefRefPtr<CefV8Exception> exception;
			g_instnace->v8context->Eval(code.c_str(), retval, exception);			

			if(retval && retval->IsString())
			{
				CefString html = retval->GetStringValue();
				g_instnace->document_html = CefString_to_BSTR(html, true);
			}				

		}
		g_instnace->v8context->Exit();
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BSTR CWebKitXCtrl::SelectedHTML(VARIANT_BOOL FullHTML)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	REQUIRE_UI_THREAD();	

	class Visitor : public CefDOMVisitor
	{
	public:
		VARIANT_BOOL FullHTML;
		Visitor(VARIANT_BOOL FullHTML)
		{
			this->FullHTML = FullHTML;
		}
		virtual void Visit(CefRefPtr<CefDOMDocument> document) 
		{
			
			if(FullHTML==VARIANT_TRUE)
			{				
				g_instnace->selection = CefString_to_BSTR(document->GetSelectionAsMarkup(), true);
			}
			else
			{
				std::string code = "(function(){var sel=window.getSelection().focusNode; return (sel.nodeType==3?sel.parentNode:sel).outerHTML;})()";

				CefString _code(code.c_str());
				CefRefPtr<CefV8Value> result;
				CefRefPtr<CefV8Exception> exception;
				
				g_instnace->v8context->Enter();
				{
					g_instnace->v8context->Eval(_code, result, exception);
				}				
				g_instnace->v8context->Exit();
				
				if(result && result->IsString())
				{
					CefString html = result->GetStringValue();
					g_instnace->selection = CefString_to_BSTR(html, true);
				}				
			}
			
			SetEvent(g_instnace->SIG_HTML_SELECTION_READY);
		}
		IMPLEMENT_REFCOUNTING(Visitor);
	};
	
	selection = "";
	ResetEvent(SIG_HTML_SELECTION_READY);
	m_Browser->GetMainFrame()->VisitDOM(new Visitor(FullHTML));
	WaitForSignal(SIG_HTML_SELECTION_READY, COMMAND_TIMEOUT_LOW);
	return CComBSTR(selection).Detach();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BSTR CWebKitXCtrl::ExecJavaScript(LPCTSTR Code)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());		
	jsresult="";
	ResetEvent(SIG_JAVASCRIPT_READY);
	CefPostTask(TID_UI,	NewCefRunnableFunction(&ExecuteCode, Code));
	WaitForSignal(SIG_JAVASCRIPT_READY, COMMAND_TIMEOUT_LOW);		
	return CComBSTR(jsresult).Detach();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::ExecuteCode(LPCTSTR JavaScript)
{
	REQUIRE_UI_THREAD();	
	g_instnace->jsresult="";
	CefString _code(LPCTSTR_to_CefString(JavaScript, false));
	CefRefPtr<CefV8Value> result;
	CefRefPtr<CefV8Exception> exception;
	g_instnace->v8context->Enter();
	{
		g_instnace->v8context->Eval(_code, result, exception);
	}				
	g_instnace->v8context->Exit();
	if(result && result->IsString())
	{
		g_instnace->jsresult = CefString_to_BSTR(result->GetStringValue(), false);		
	}	
	SetEvent(g_instnace->SIG_JAVASCRIPT_READY);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BSTR CWebKitXCtrl::ExecCommand(LONG id, VARIANT& Params)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;

	CComBSTR code("document.execCommand(\"");
	code.Append(EditorCommands[id].c_str());
	code.Append("\", false, \"");
	code.Append(Params.bstrVal);
	code.Append("\")");	

	debugPrint("%s\n", _com_util::ConvertBSTRToString(code));
	
	ExecJavaScript(code);
	return CComBSTR(jsresult).Detach();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BSTR CWebKitXCtrl::TidyHTML(LPCTSTR HTML)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());		
	jsresult="";
	ResetEvent(SIG_TIDY_HTML_READY);
	CefPostTask(TID_UI,	NewCefRunnableFunction(&ExecuteTidy, HTML, true));
	WaitForSignal(SIG_TIDY_HTML_READY, INFINITE);		
	return CComBSTR(jsresult).Detach();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWebKitXCtrl::ExecuteTidy(LPCTSTR HTML, bool CleanHTML)
{
	REQUIRE_UI_THREAD();
	g_instnace->jsresult="";	
	g_instnace->v8context->Enter();
	{
		CefRefPtr<CefV8Value> fnPtr;
		CefRefPtr<CefV8Exception> exception;
		
		//Written by Nochum Sossonko, (nsossonko@hotmail.com)
		CefString code = "__tidy_html=function(e){function t(e,t){function u(){this.pos=0;this.token=\"\";this.current_mode=\"CONTENT\";this.tags={parent:\"parent1\",parentcount:1,parent1:\"\"};this.tag_type=\"\";this.token_text=this.last_token=this.last_text=this.token_type=\"\";this.Utils={whitespace:\"\\n\\r	 \".split(\"\"),single_token:\"br,input,link,meta,!doctype,basefont,base,area,hr,wbr,param,img,isindex,?xml,embed\".split(\",\"),extra_liners:\"head,body,/html\".split(\",\"),in_array:function(e,t){for(var n=0;n<t.length;n++){if(e===t[n]){return true}}return false}};this.get_content=function(){var e=\"\";var t=[];var n=false;while(this.input.charAt(this.pos)!==\"<\"){if(this.pos>=this.input.length){return t.length?t.join(\"\"):[\"\",\"TK_EOF\"]}e=this.input.charAt(this.pos);this.pos++;this.line_char_count++;if(this.Utils.in_array(e,this.Utils.whitespace)){if(t.length){n=true}this.line_char_count--;continue}else if(n){if(this.line_char_count>=this.max_char){t.push(new_line);for(var r=0;r<this.indent_level;r++){t.push(this.indent_string)}this.line_char_count=0}else{t.push(\" \");this.line_char_count++}n=false}t.push(e)}return t.length?t.join(\"\"):\"\"};this.get_contents_to=function(e){if(this.pos==this.input.length){return[\"\",\"TK_EOF\"]}var t=\"\";var n=\"\";var r=new RegExp(\"</\"+e+\"\\\\s*>\",\"igm\");r.lastIndex=this.pos;var i=r.exec(this.input);var s=i?i.index:this.input.length;if(this.pos<s){n=this.input.substring(this.pos,s);this.pos=s}return n};this.record_tag=function(e){if(this.tags[e+\"count\"]){this.tags[e+\"count\"]++;this.tags[e+this.tags[e+\"count\"]]=this.indent_level}else{this.tags[e+\"count\"]=1;this.tags[e+this.tags[e+\"count\"]]=this.indent_level}this.tags[e+this.tags[e+\"count\"]+\"parent\"]=this.tags.parent;this.tags.parent=e+this.tags[e+\"count\"]};this.retrieve_tag=function(e){if(this.tags[e+\"count\"]){var t=this.tags.parent;while(t){if(e+this.tags[e+\"count\"]===t){break}t=this.tags[t+\"parent\"]}if(t){this.indent_level=this.tags[e+this.tags[e+\"count\"]];this.tags.parent=this.tags[t+\"parent\"]}delete this.tags[e+this.tags[e+\"count\"]+\"parent\"];delete this.tags[e+this.tags[e+\"count\"]];if(this.tags[e+\"count\"]==1){delete this.tags[e+\"count\"]}else{this.tags[e+\"count\"]--}}};this.get_tag=function(){var e=\"\";var t=[];var n=false;do{if(this.pos>=this.input.length){return t.length?t.join(\"\"):[\"\",\"TK_EOF\"]}e=this.input.charAt(this.pos);this.pos++;this.line_char_count++;if(this.Utils.in_array(e,this.Utils.whitespace)){n=true;this.line_char_count--;continue}if(e===\"'\"||e==='\"'){if(!t[1]||t[1]!==\"!\"){e+=this.get_unformatted(e);n=true}}if(e===\"=\"){n=false}if(t.length&&t[t.length-1]!==\"=\"&&e!==\">\"&&n){if(this.line_char_count>=this.max_char){this.print_newline(false,t);this.line_char_count=0}else{t.push(\" \");this.line_char_count++}n=false}t.push(e)}while(e!==\">\");var r=t.join(\"\");var i;if(r.indexOf(\" \")!=-1){i=r.indexOf(\" \")}else{i=r.indexOf(\">\")}var s=r.substring(1,i).toLowerCase();if(r.charAt(r.length-2)===\"/\"||this.Utils.in_array(s,this.Utils.single_token)){this.tag_type=\"SINGLE\"}else if(s===\"script\"){this.record_tag(s);this.tag_type=\"SCRIPT\"}else if(s===\"style\"){this.record_tag(s);this.tag_type=\"STYLE\"}else if(this.Utils.in_array(s,unformatted)){var o=this.get_unformatted(\"</\"+s+\">\",r);t.push(o);this.tag_type=\"SINGLE\"}else if(s.charAt(0)===\"!\"){if(s.indexOf(\"[if\")!=-1){if(r.indexOf(\"!IE\")!=-1){var o=this.get_unformatted(\"-->\",r);t.push(o)}this.tag_type=\"START\"}else if(s.indexOf(\"[endif\")!=-1){this.tag_type=\"END\";this.unindent()}else if(s.indexOf(\"[cdata[\")!=-1){var o=this.get_unformatted(\"]]>\",r);t.push(o);this.tag_type=\"SINGLE\"}else{var o=this.get_unformatted(\"-->\",r);t.push(o);this.tag_type=\"SINGLE\"}}else{if(s.charAt(0)===\"/\"){this.retrieve_tag(s.substring(1));this.tag_type=\"END\"}else{this.record_tag(s);this.tag_type=\"START\"}if(this.Utils.in_array(s,this.Utils.extra_liners)){this.print_newline(true,this.output)}}return t.join(\"\")};this.get_unformatted=function(e,t){if(t&&t.indexOf(e)!=-1){return\"\"}var n=\"\";var r=\"\";var i=true;do{if(this.pos>=this.input.length){return r}n=this.input.charAt(this.pos);this.pos++;if(this.Utils.in_array(n,this.Utils.whitespace)){if(!i){this.line_char_count--;continue}if(n===\"\\n\"||n===\"\\r\"){r+=new_line;this.line_char_count=0;continue}}r+=n;this.line_char_count++;i=true}while(r.indexOf(e)==-1);return r};this.get_token=function(){var e;if(this.last_token===\"TK_TAG_SCRIPT\"||this.last_token===\"TK_TAG_STYLE\"){var t=this.last_token.substr(7);e=this.get_contents_to(t);if(typeof e!==\"string\"){return e}return[e,\"TK_\"+t]}if(this.current_mode===\"CONTENT\"){e=this.get_content();if(typeof e!==\"string\"){return e}else{return[e,\"TK_CONTENT\"]}}if(this.current_mode===\"TAG\"){e=this.get_tag();if(typeof e!==\"string\"){return e}else{var n=\"TK_TAG_\"+this.tag_type;return[e,n]}}};this.get_full_indent=function(e){e=this.indent_level+e||0;if(e<1)return\"\";return Array(e+1).join(this.indent_string)};this.printer=function(e,t,n,r,i){this.input=e||\"\";this.output=[];this.indent_character=t;this.indent_string=\"\";this.indent_size=n;this.brace_style=i;this.indent_level=0;this.max_char=r;this.line_char_count=0;for(var s=0;s<this.indent_size;s++){this.indent_string+=this.indent_character}this.print_newline=function(e,t){this.line_char_count=0;if(!t||!t.length){return}if(!e){while(this.Utils.in_array(t[t.length-1],this.Utils.whitespace)){t.pop()}}t.push(new_line);for(var n=0;n<this.indent_level;n++){t.push(this.indent_string)}};this.print_token=function(e){this.output.push(e)};this.indent=function(){this.indent_level++};this.unindent=function(){if(this.indent_level>0){this.indent_level--}}};return this}var n,r,i,s,o;t=t||{};r=t.indent_size||1;i=t.indent_char||\"	\";o=t.brace_style||\"collapse\";s=t.max_char==0?Infinity:t.max_char||180;unformatted=t.unformatted||[\"a\",\"sub\",\"sup\",\"b\",\"i\",\"u\",\"pre\"];new_line=t.new_line||\"\\r\\n\";n=new u;n.printer(e,i,r,s,o);while(true){var a=n.get_token();n.token_text=a[0];n.token_type=a[1];if(n.token_type===\"TK_EOF\"){break}switch(n.token_type){case\"TK_TAG_START\":n.print_newline(false,n.output);n.print_token(n.token_text);n.indent();n.current_mode=\"CONTENT\";break;case\"TK_TAG_STYLE\":case\"TK_TAG_SCRIPT\":n.print_newline(false,n.output);n.print_token(n.token_text);n.current_mode=\"CONTENT\";break;case\"TK_TAG_END\":if(n.last_token===\"TK_CONTENT\"&&n.last_text===\"\"){var f=n.token_text.match(/\\w+/)[0];var l=n.output[n.output.length-1].match(/<\\s*(\\w+)/);if(l===null||l[1]!==f)n.print_newline(true,n.output)}n.print_token(n.token_text);n.current_mode=\"CONTENT\";break;case\"TK_TAG_SINGLE\":n.print_newline(false,n.output);n.print_token(n.token_text);n.current_mode=\"CONTENT\";break;case\"TK_CONTENT\":if(n.token_text!==\"\"){n.print_token(n.token_text)}n.current_mode=\"TAG\";break;case\"TK_STYLE\":case\"TK_SCRIPT\":if(n.token_text!==\"\"){n.output.push(new_line);var c=n.token_text;if(n.token_type==\"TK_SCRIPT\"){var h=typeof js_beautify==\"function\"&&js_beautify}else if(n.token_type==\"TK_STYLE\"){var h=typeof css_beautify==\"function\"&&css_beautify}if(t.indent_scripts==\"keep\"){var p=0}else if(t.indent_scripts==\"separate\"){var p=-n.indent_level}else{var p=1}var d=n.get_full_indent(p);if(h){c=h(c.replace(/^\\s*/,d),t)}else{var v=c.match(/^\\s*/)[0];var m=v.match(/[^\\n\\r]*$/)[0].split(n.indent_string).length-1;var g=n.get_full_indent(p-m);c=c.replace(/^\\s*/,d).replace(/\\r\\n|\\r|\\n/g,\"\\n\"+g).replace(/\\s*$/,\"\")}if(c){n.print_token(c);n.print_newline(true,n.output)}}n.current_mode=\"TAG\";break}n.last_token=n.token_type;n.last_text=n.token_text}return n.output.join(\"\")}return t(e)};";

		g_instnace->v8context->Eval(code, fnPtr, exception);
		if(fnPtr && fnPtr->IsFunction())
		{
			CefV8ValueList argList;
			CefRefPtr<CefV8Value> arg0 = CefV8Value::CreateString(LPCTSTR_to_CefString(HTML, false));
			argList.push_back(arg0);
			CefRefPtr<CefV8Value> result = fnPtr->ExecuteFunction(g_instnace->v8context->GetGlobal(), argList);
			if(result && result->IsString())
			{
				g_instnace->jsresult = CefString_to_BSTR(result->GetStringValue(), CleanHTML);		
			}	
		}
	}				
	g_instnace->v8context->Exit();
	SetEvent(g_instnace->SIG_TIDY_HTML_READY);
}


//"var mousedownEvent = document.createEvent(\"MouseEvent\");\n"
//"mousedownEvent.initMouseEvent(\"mousedown\",false,false,window,0,0,0,0,0,0,0,0,0,1,target);\n"				
//"target.dispatchEvent(mousedownEvent);\n"


