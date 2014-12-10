#pragma once

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/cef_client.h"

#include "Utils.h"

#define REQUIRE_UI_THREAD()   ASSERT(CefCurrentlyOn(TID_UI));
#define REQUIRE_IO_THREAD()   ASSERT(CefCurrentlyOn(TID_IO));
#define REQUIRE_FILE_THREAD() ASSERT(CefCurrentlyOn(TID_FILE));

class CWebKitXCtrl;

class WebKitHandler :
	public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefRequestHandler,	
	public CefFocusHandler,
	public CefKeyboardHandler,
	public CefPrintHandler,
	public CefV8ContextHandler,
	public CefDragHandler,
	public CefPermissionHandler,
	public CefGeolocationHandler	

{
public:
	WebKitHandler(CWebKitXCtrl* control);
	~WebKitHandler(void);

	// CefClient methods
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {	return this; }
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {	return this; } 
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefFocusHandler> GetFocusHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefPrintHandler> GetPrintHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefV8ContextHandler> GetV8ContextHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefPermissionHandler> GetPermissionHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() OVERRIDE { return this; }

	// CefLifeSpanHandler methods
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, const CefString& url, CefRefPtr<CefClient>& client, CefBrowserSettings& settings) OVERRIDE;
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;	
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// CefLoadHandler methods
	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) OVERRIDE;
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;
	virtual bool OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& failedUrl, CefString& errorText) OVERRIDE;

	// CefRequestHandler methods
	virtual bool OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefRequest> request, CefString& redirectUrl, CefRefPtr<CefStreamReader>& resourceStream, CefRefPtr<CefResponse> response, int loadFlags) OVERRIDE;
	virtual bool GetDownloadHandler(CefRefPtr<CefBrowser> browser, const CefString& mimeType, const CefString& fileName, int64 contentLength, CefRefPtr<CefDownloadHandler>& handler) OVERRIDE;

	// CefDisplayHandler methods
	virtual void OnNavStateChange(CefRefPtr<CefBrowser> browser, bool canGoBack, bool canGoForward) OVERRIDE;
	virtual void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) OVERRIDE;
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) OVERRIDE;
	virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString& message, const CefString& source, int line) OVERRIDE;

	// CefFocusHandler methods.
	virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) OVERRIDE;

	// CefKeyboardHandler methods.
	virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser, KeyEventType type, int code, int modifiers, bool isSystemKey, bool isAfterJavaScript) OVERRIDE;

	// CefPrintHandler methods.
	virtual bool GetPrintHeaderFooter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefPrintInfo& printInfo, const CefString& url, const CefString& title, int currentPage, int maxPages, CefString& topLeft, CefString& topCenter, CefString& topRight, CefString& bottomLeft, CefString& bottomCenter, CefString& bottomRight) OVERRIDE;

	// CefV8ContextHandler methods
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;

	// CefDragHandler methods.
	virtual bool OnDragStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask) OVERRIDE;
	virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask) OVERRIDE;

	// CefPermissionHandler methods.
	virtual bool OnBeforeScriptExtensionLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& extensionName) OVERRIDE;

	// CefGeolocationHandler methods.
	virtual void OnRequestGeolocationPermission(CefRefPtr<CefBrowser> browser, const CefString& requesting_url, int request_id, CefRefPtr<CefGeolocationCallback> callback) OVERRIDE;

	CWebKitXCtrl* control;
	typedef std::map<std::string, CefRefPtr<CefDOMVisitor> > DOMVisitorMap;
	DOMVisitorMap m_DOMVisitors;
	void AddDOMVisitor(const std::string& path, CefRefPtr<CefDOMVisitor> visitor);
	CefRefPtr<CefDOMVisitor> GetDOMVisitor(const std::string& path);

	IMPLEMENT_REFCOUNTING(WebKitHandler);
	IMPLEMENT_LOCKING(WebKitHandler);		
};


