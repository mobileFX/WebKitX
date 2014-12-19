#pragma once

#ifndef __WEBKITCTL_H__
#define __WEBKITCTL_H__

//$(SolutionDir)..\Studio\
//$(SolutionDir)\CEF\CEF1\DLLs\

#include "comutil.h"

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/cef_client.h"
#include "include/cef_dom.h"

#include "WebKitHandler.h"
#include "WebKitV8Extension.h"

#define COMMAND_TIMEOUT_LOW		 2000
#define COMMAND_TIMEOUT_HIGH	30000

typedef LONG (__stdcall *VISUAL_BASIC_6_FN_PTR)(LONG);
std::string Replace(std::string str, const std::string& find, const std::string& replace);																													\

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
		
	static bool CEF_INITIALIZED;
	static CWebKitXCtrl* g_instnace;

	CefRefPtr<CefBrowser> m_Browser;		// The child browser window
	CefRefPtr<CefV8Context> v8context;		// V8 Context
	WebKitV8Extension* v8handler;
	HANDLE SIG_HTML_READY;					// Wait Event for HTML source to be collected
	HANDLE SIG_HTML_SELECTION_READY;		// Wait Event for selected html
	HANDLE SIG_JAVASCRIPT_READY;			// Wait Event for selected html
	HANDLE SIG_TIDY_HTML_READY;
	CefWindowHandle m_MainHwnd;				// The main frame window handle
	CefWindowHandle m_BrowserHwnd;			// The child browser window handle		
	HHOOK hook;	
	bool CEF_BROWSER_CREATED;
	CComBSTR document_html;
	CComBSTR selection;
	VARIANT_BOOL m_Editable;
	VARIANT_BOOL m_ActiveXCreated;
	UINT UID_COUNTER;
	bool LoadingHTML;	
	CComBSTR jsresult;
	std::vector<std::string> EditorCommands;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	CWebKitXCtrl();
	~CWebKitXCtrl();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	#ifdef CEF_VERSION_3
	virtual void OnContextInitialized() OVERRIDE;
	#endif

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	// Windows WM_MESSAGEs
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid) OVERRIDE;
	virtual BOOL IsSubclassedControl() OVERRIDE;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) OVERRIDE;
	virtual void DoPropExchange(CPropExchange* pPX) OVERRIDE;
	virtual void OnResetState() OVERRIDE;	
	LRESULT OnOcmCommand(WPARAM wParam, LPARAM lParam);
	LRESULT OnResize(WPARAM wParam, LPARAM lParam);
	LRESULT CancelMessage(WPARAM wParam, LPARAM lParam);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	// CEF API

	static bool InitCEF();
	static void QuitCEF();
	void CreateCEFBrowser();
	void DestroyCEFBrowser();

	static void ExecuteGetSource(CefRefPtr<CefFrame> frame);
	static void ExecuteEditable(CefRefPtr<CefFrame> frame);		
	static void ExecuteAddEventEx(std::string elementID, std::string eventType, IDispatch* vbObject, std::string vbObjectFunctionName, VARIANT_BOOL UseCapture);
	static void ExecuteSelectNode(std::string selector, std::string html, bool MoveCaret, bool SelectContents);
	static void ExecuteCode(LPCTSTR JavaScript);
	static void ExecuteTidy(LPCTSTR HTML, bool CleanHTML);
		
	template<typename F, typename P> static void ExecuteAddEvent(std::string elementID, std::string eventType, F handler, VARIANT_BOOL UseCapture);
	template<typename F, typename P> void __addEventHandler(std::string elementID, std::string eventType, F handler, VARIANT_BOOL UseCapture);
	
	typedef void (__stdcall *EVENT_HANDLER_FN)(CefRefPtr<CefDOMEvent>*);
	static void __stdcall __HandleDOMEvent(CefRefPtr<CefDOMEvent>* DOMEvnet);	
	void HandleDOMEvent(CefRefPtr<CefDOMEvent>* e);

	void AttachEditDOMEvents();
	void __set_attribute(std::string selector, std::string attrName, std::string attrValue);	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	// ActiveX API
	
	void OpenURL(LPCTSTR URL);
	BSTR GetHTML();
	void SetHTML(LPCTSTR newVal);		
	void Edit(void);
	void Preview(void);	
	void Reload(void);		
	void Repaint(void);
	void addEventListener(LPCTSTR Selector, LPCTSTR Event, LONG AddressOfEventHandler, VARIANT_BOOL UseCapture);
	void addEventListenerEx(LPCTSTR Selector, LPCTSTR Event, IDispatch* vbObject, LPCTSTR vbObjectFunctionName, VARIANT_BOOL UseCapture);	
	void SetStyle(LPCTSTR Selector, LPCTSTR StyleName, LPCTSTR StyleValue);
	VARIANT_BOOL Modified(void);	
	void SelectElement(LPCTSTR Selector, VARIANT_BOOL MoveCaret, VARIANT_BOOL SelectContents);
	OLE_HANDLE hBrowserHWND(void);
	void LoadHTML(LPCTSTR HTML, LPCTSTR URL);
	VARIANT_BOOL Created(void);
	BSTR SelectedHTML(VARIANT_BOOL FullHTML);	
	BSTR ExecJavaScript(LPCTSTR Code);
	BSTR ExecCommand(LONG id, VARIANT& Params);
	BSTR TidyHTML(LPCTSTR HTML);
	BSTR CleanHTML(LPCTSTR HTML);
	VARIANT_BOOL Editable(void);
	void Find(LPCTSTR HTML);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	// ActiveX Events
	
	//---------------------------------------------------------------------------------------------------------------------------------------------
	void FireOnReady() 
	{
		AttachEditDOMEvents();
		SetTimer(eventidOnReady, 10, OnReadyTimerProc); 
	}
	static void CALLBACK OnReadyTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) { g_instnace->OnReadyTimer(); }
	void OnReadyTimer() { KillTimer(eventidOnReady); OnReady(); }
	void OnReady(void)
	{
		debugPrint("OnReady.\n");

		if(m_Editable==VARIANT_TRUE)
		{
			Edit();
		}
		else
		{
			Preview();
		}

		FireEvent(eventidOnReady, EVENT_PARAM(VTS_NONE));
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------	
	void FireOnCreate() { SetTimer(eventidOnCreate, 10, OnCreateTimerProc); }	
	static void CALLBACK OnCreateTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) { g_instnace->OnCreateTimer(); }	
	void OnCreateTimer() { KillTimer(eventidOnCreate); OnCreate(); }
	void OnCreate(void)
	{
		debugPrint("OnCreate.\n");		
		m_ActiveXCreated = VARIANT_TRUE;
		FireEvent(eventidOnCreate, EVENT_PARAM(VTS_NONE));
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------
	static const UINT_PTR ON_MOUSE_DOWN_TIMER = 1000;	
	void FireOnMouseDown() { KillTimer(ON_MOUSE_DOWN_TIMER); SetTimer(ON_MOUSE_DOWN_TIMER, 10, OnMouseDownTimerProc); }	
	static void CALLBACK OnMouseDownTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) { g_instnace->OnMouseDownTimer(); }	
	void OnMouseDownTimer() { KillTimer(ON_MOUSE_DOWN_TIMER); OnMouseDown(); }
	void OnMouseDown(void)
	{
		debugPrint("OnMouseDown\n");				
		FireMouseDown(1, 0, 0,0);
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------
	static const UINT_PTR ON_FOCUS_TIMER = 1001;	
	static std::string focusNodes;
	void FireOnFocus() { KillTimer(ON_FOCUS_TIMER); SetTimer(ON_FOCUS_TIMER, 10, OnFocusTimerProc); }	
	static void CALLBACK OnFocusTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) { g_instnace->OnFocusTimer(); }	
	void OnFocusTimer() { KillTimer(ON_FOCUS_TIMER); OnFocus(); }
	void OnFocus(void)
	{
		if(focusNodes.size()==0) return;
		debugPrint("OnFocus\n");				
		CComBSTR btarget(focusNodes.c_str());
		FireEvent(eventidOnFocus, EVENT_PARAM(VTS_BSTR), btarget);
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------
	static const UINT_PTR ON_MODIFIED_TIMER = 1002;	
	void FireOnModified() { KillTimer(ON_MODIFIED_TIMER); SetTimer(ON_MODIFIED_TIMER, 1000, OnModifiedTimerProc); }	
	static void CALLBACK OnModifiedTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) { g_instnace->OnModifiedTimer(); }	
	void OnModifiedTimer() { KillTimer(ON_MODIFIED_TIMER); OnModified(); }
	void OnModified(void)
	{
		debugPrint("OnModified\n");
		SetModifiedFlag(TRUE);
		FireEvent(eventidOnModified, EVENT_PARAM(VTS_NONE));
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------
	static const UINT_PTR ON_SELECTION_CHANGED_TIMER = 1003;	
	void FireOnSelectionChanged() { KillTimer(ON_SELECTION_CHANGED_TIMER); SetTimer(ON_SELECTION_CHANGED_TIMER, 1000, OnSelectionChangedTimerProc); }	
	static void CALLBACK OnSelectionChangedTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) { g_instnace->OnSelectionChangedTimer(); }	
	void OnSelectionChangedTimer() { KillTimer(ON_SELECTION_CHANGED_TIMER); OnSelectionChanged(); }
	void OnSelectionChanged(void)
	{
		FireEvent(eventidOnSelectionChanged, EVENT_PARAM(VTS_BSTR VTS_BSTR), document_html, selection);		
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	// TODO: Handle mouse move to prevent CSS3 hover styles during editing
	static LRESULT CALLBACK MouseHook(int nCode, WPARAM wp, LPARAM lp)
	{
		/*
		MOUSEHOOKSTRUCT *pmh = (MOUSEHOOKSTRUCT *) lp;
		if(nCode >= 0)
		{		
			if(wp == WM_RBUTTONDOWN || wp == WM_RBUTTONUP || wp==WM_MOUSEMOVE) 
			{
				return 1;
			}
		}
		*/
		return CallNextHookEx(NULL, nCode, wp, lp);   
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
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
		dispidFind = 23L,
		dispidCleanHTML = 22L,
		dispidEditable = 21L,
		dispidTidyHTML = 20L,
		dispidExecCommand = 19L,
		dispidExecJavaScript = 18L,
		dispidaddEventListener = 7L,
		dispidaddEventListenerEx = 8L,
		dispidCreated = 16L,
		dispidEdit = 3L,		
		dispidhBrowserHWND = 14L,
		dispidHTML = 2L,
		dispidLoadHTML = 15L,
		dispidModified = 5L,
		dispidOpenURL = 1L,
		dispidPreview = 4L,
		dispidReload = 6L,
		dispidRepaint = 10L,		
		dispidSelectedHTML = 17L,
		dispidSelectElement = 13L,
		dispidSetStyle = 11L,

		eventidOnSelectionChanged = 5L,
		eventidOnCreate = 2L,		
		eventidOnFocus = 3L,
		eventidOnModified = 4L,
		eventidOnReady = 1L
	};		
};

#endif