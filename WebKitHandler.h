#pragma once

#define CEF_VERSION_1

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/cef_client.h"

#ifdef CEF_VERSION_1
#define WINDOW_HANDLE(B) B->GetWindowHandle()
#endif

#ifdef CEF_VERSION_3
#define WINDOW_HANDLE(B) B->GetHost()->GetWindowHandle()
#endif

#ifdef CEF_VERSION_3
#include "include/wrapper/cef_helpers.h"
#endif

#include "Utils.h"

#define REQUIRE_UI_THREAD()   ASSERT(CefCurrentlyOn(TID_UI));
#define REQUIRE_IO_THREAD()   ASSERT(CefCurrentlyOn(TID_IO));
#define REQUIRE_FILE_THREAD() ASSERT(CefCurrentlyOn(TID_FILE));

class CWebKitXCtrl;

class WebKitHandler :
	public CefClient,
	public CefDisplayHandler,	
	public CefLoadHandler,
	public CefFocusHandler,
	public CefKeyboardHandler,
	public CefDragHandler,
	public CefLifeSpanHandler

	#ifdef CEF_VERSION_1
	,public CefV8ContextHandler,
	public CefPermissionHandler
	#endif
{
public:
	WebKitHandler(CWebKitXCtrl* control);
	~WebKitHandler(void);

	// CefClient methods
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {	return this; }
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {	return this; } 
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }	
	virtual CefRefPtr<CefFocusHandler> GetFocusHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE { return this; }

	#ifdef CEF_VERSION_1	
	virtual CefRefPtr<CefV8ContextHandler> GetV8ContextHandler() OVERRIDE { return this; }	
	virtual CefRefPtr<CefPermissionHandler> GetPermissionHandler() OVERRIDE { return this; }
	#endif

	// CefLifeSpanHandler methods
	#ifdef CEF_VERSION_1
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, const CefString& url, CefRefPtr<CefClient>& client, CefBrowserSettings& settings) OVERRIDE;
	#endif
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;	
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// CefLoadHandler methods
	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) OVERRIDE;
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;
	#ifdef CEF_VERSION_1
	virtual bool OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& failedUrl, CefString& errorText) OVERRIDE;
	#endif

	// CefDisplayHandler methods
	#ifdef CEF_VERSION_1
	virtual void OnNavStateChange(CefRefPtr<CefBrowser> browser, bool canGoBack, bool canGoForward) OVERRIDE;
	#endif
	virtual void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) OVERRIDE;
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) OVERRIDE;
	virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString& message, const CefString& source, int line) OVERRIDE;

	// CefFocusHandler methods.
	#ifdef CEF_VERSION_1
	virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) OVERRIDE;

	// CefKeyboardHandler methods.
	virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser, KeyEventType type, int code, int modifiers, bool isSystemKey, bool isAfterJavaScript) OVERRIDE;

	// CefV8ContextHandler methods
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;

	// CefDragHandler methods.
	virtual bool OnDragStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask) OVERRIDE;
	virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask) OVERRIDE;

	// CefPermissionHandler methods.
	virtual bool OnBeforeScriptExtensionLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& extensionName) OVERRIDE;

	#endif

	CWebKitXCtrl* control;
	typedef std::map<std::string, CefRefPtr<CefDOMVisitor> > DOMVisitorMap;
	DOMVisitorMap m_DOMVisitors;
	void AddDOMVisitor(const std::string& path, CefRefPtr<CefDOMVisitor> visitor);
	CefRefPtr<CefDOMVisitor> GetDOMVisitor(const std::string& path);

	IMPLEMENT_REFCOUNTING(WebKitHandler);
	IMPLEMENT_LOCKING(WebKitHandler);		
};


