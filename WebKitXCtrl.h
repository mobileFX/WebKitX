#pragma once

//$(SolutionDir)..\Studio\
//$(SolutionDir)\CEF\CEF1\DLLs\


#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/cef_client.h"

#include "WebKitHandler.h"

#define COMMAND_TIMEOUT_LOW		 2000
#define COMMAND_TIMEOUT_HIGH	10000

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
	void __set_attribute(std::string selector, std::string attrName, std::string attrValue);
	void __set_style(std::string selector, std::string attrName, std::string attrValue);

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
		dispidPreview = 4L,
		dispidEdit = 3L,
		dispidOpenURL = 1L,
		dispidHTML = 2L
	};
	
	void OpenURL(LPCTSTR URL);
	BSTR GetHTML();
	void SetHTML(LPCTSTR newVal);		
	void Edit(void);
	void Preview(void);
};
