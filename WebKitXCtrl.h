#pragma once

//$(SolutionDir)..\Studio\
//$(SolutionDir)\CEF\CEF1\DLLs\


#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/cef_client.h"
#include "include/cef_dom.h"

#include "WebKitHandler.h"

#define COMMAND_TIMEOUT_LOW		 2000
#define COMMAND_TIMEOUT_HIGH	30000

typedef LONG (__stdcall *VISUAL_BASIC_6_FN_PTR)(LONG, LONG);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CWebKitXCtrl : 
	public COleControl,
	public CefApp
	#ifdef CEF_VERSION_3
	,public CefBrowserProcessHandler
	#endif	
{
	DECLARE_DYNCREATE(CWebKitXCtrl)

public:
		
	static CWebKitXCtrl* g_instnace;
	CefRefPtr<WebKitHandler> g_handler;	
	HANDLE SIG_READY;
	std::string response;
	std::string selector;
	std::string attrName;
	std::string attrValue;	
	CefRefPtr<CefBrowser> m_Browser;		// The child browser window
	CefWindowHandle m_MainHwnd;				// The main frame window handle
	CefWindowHandle m_BrowserHwnd;			// The child browser window handle	
	
	CWebKitXCtrl();
	~CWebKitXCtrl();

	// CEF Life Management
	static bool CEF_INITIALIZED;
	static bool InitCEF();
	static void QuitCEF();
	bool CEF_BROWSER_CREATED;
	void CreateCEFBrowser();
	void DestroyCEFBrowser();

	#ifdef CEF_VERSION_3
	virtual void OnContextInitialized() OVERRIDE;
	#endif

	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid) OVERRIDE;
	virtual BOOL IsSubclassedControl() OVERRIDE;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) OVERRIDE;
	virtual void DoPropExchange(CPropExchange* pPX) OVERRIDE;
	virtual void OnResetState() OVERRIDE;	

	LRESULT OnOcmCommand(WPARAM wParam, LPARAM lParam);
	LRESULT OnResize(WPARAM wParam, LPARAM lParam);
	LRESULT CancelMessage(WPARAM wParam, LPARAM lParam);
	
	static void ExecuteGetSource(CefRefPtr<CefFrame> frame);
	static void ExecuteEditable(CefRefPtr<CefFrame> frame);		
	static void ExecuteAddEvent(std::string elementID, std::string eventType, LONG AddressOfEventHandler);
	static void ExecuteAddEventEx(std::string elementID, std::string eventType, IDispatch* vbObject, std::string vbObjectFunctionName);
	void __set_attribute(std::string selector, std::string attrName, std::string attrValue);
	void __set_style(std::string selector, std::string attrName, std::string attrValue);	

	typedef void(*EVENT_HANDLER_FN)(CefRefPtr<CefDOMEvent>*, CefRefPtr<CefDOMNode>*);
	template<typename T> void __addEventHandler(std::string elementID, std::string eventType, T handler);	
	void addEventListenerEx(LPCTSTR Selector, LPCTSTR Event, IDispatch* vbObject, LPCTSTR vbObjectFunctionName);

	// Asynchronous Events 
	// We need to make sure we are not in the CefBrowser thread and therefore we use timer to fire the events asynchronously.

	void FireOnReady() { SetTimer(eventidOnReady, 10, OnReadyTimerProc); }
	static void CALLBACK OnReadyTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) { g_instnace->OnReadyTimer(); }
	void OnReadyTimer() { KillTimer(eventidOnReady); OnReady(); }

	void FireOnCreate() { SetTimer(eventidOnCreate, 10, OnCreateTimerProc); }	
	static void CALLBACK OnCreateTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) { g_instnace->OnCreateTimer(); }	
	void OnCreateTimer() { KillTimer(eventidOnCreate); OnCreate(); }

	// ActiveX Events
	void OnCreate(void)
	{
		debugPrint("OnCreate.\n");
		FireEvent(eventidOnCreate, EVENT_PARAM(VTS_NONE));
	}

	void OnReady(void)
	{
		debugPrint("OnReady.\n");
		FireEvent(eventidOnReady, EVENT_PARAM(VTS_NONE));
	}

	// ActiveX API
	void OpenURL(LPCTSTR URL);
	BSTR GetHTML();
	void SetHTML(LPCTSTR newVal);		
	void Edit(void);
	void Preview(void);
	VARIANT_BOOL Modified(void);
	void Reload(void);
	void addEventListener(LPCTSTR Selector, LPCTSTR Event, LONG AddressOfEventHandler);

	IMPLEMENT_REFCOUNTING(CWebKitXCtrl);
	IMPLEMENT_LOCKING(CWebKitXCtrl);		

	DECLARE_OLECREATE_EX(CWebKitXCtrl)	// Class factory and guid
	DECLARE_OLETYPELIB(CWebKitXCtrl)	// GetTypeInfo
	DECLARE_PROPPAGEIDS(CWebKitXCtrl)	// Property page IDs
	DECLARE_OLECTLTYPE(CWebKitXCtrl)	// Type name and misc status

	// Message maps
	DECLARE_MESSAGE_MAP()

	// Dispatch maps
	DECLARE_DISPATCH_MAP()

	// Event maps
	DECLARE_EVENT_MAP()

	enum 
	{		
		dispidaddEventListenerEx = 8L,
		dispidaddEventListener = 7L,
		dispidReload = 6L,
		dispidModified = 5L,
		eventidOnCreate = 2L,		
		eventidOnReady = 1L,
		dispidPreview = 4L,
		dispidEdit = 3L,
		dispidOpenURL = 1L,
		dispidHTML = 2L
	};	
};
