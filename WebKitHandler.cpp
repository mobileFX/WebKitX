
#include "StdAfx.h"
#include "WebKitHandler.h"
#include "WebKitXCtrl.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WebKitHandler::WebKitHandler(CWebKitXCtrl* control)
{
	this->control = control;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WebKitHandler::~WebKitHandler(void)
{
	this->control = NULL;	
	debugPrint("WebKitHandler terminated.\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) 
{
	REQUIRE_UI_THREAD();
	AutoLock lock_scope(this);
	if (!control->m_Browser.get())   
	{		
		control->m_Browser = browser;
		control->m_BrowserHwnd = browser->GetWindowHandle();		
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) 
{
	REQUIRE_UI_THREAD();
	if(control->m_BrowserHwnd == browser->GetWindowHandle()) 
	{
		control->DestroyCEF();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) 
{
	REQUIRE_UI_THREAD();
	if (control->m_BrowserHwnd == browser->GetWindowHandle() && frame->IsMain()) 
	{
		// We've just started loading a page		
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) 
{
	REQUIRE_UI_THREAD();
	if (control->m_BrowserHwnd == browser->GetWindowHandle() && frame->IsMain()) 
	{
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& failedUrl, CefString& errorText) 
{
	REQUIRE_UI_THREAD();
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitHandler::GetDownloadHandler(CefRefPtr<CefBrowser> browser, const CefString& mimeType, const CefString& fileName, int64 contentLength, CefRefPtr<CefDownloadHandler>& handler) 
{
	REQUIRE_UI_THREAD();

	// Close the browser window if it is a popup with no other document contents.
	if (browser->IsPopup() && !browser->HasDocument())
		browser->CloseBrowser();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnNavStateChange(CefRefPtr<CefBrowser> browser, bool canGoBack, bool canGoForward) 
{
	REQUIRE_UI_THREAD();
	//SetNavState(canGoBack, canGoForward);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString& message, const CefString& source, int line) 
{
	REQUIRE_UI_THREAD();
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) 
{
	REQUIRE_UI_THREAD();		
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser, KeyEventType type, int code, int modifiers, bool isSystemKey, bool isAfterJavaScript) 
{
	REQUIRE_UI_THREAD();
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitHandler::GetPrintHeaderFooter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefPrintInfo& printInfo, const CefString& url, const CefString& title, int currentPage, int maxPages, CefString& topLeft, CefString& topCenter, CefString& topRight, CefString& bottomLeft, CefString& bottomCenter, CefString& bottomRight)
{
	REQUIRE_UI_THREAD();
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) 
{
	REQUIRE_UI_THREAD();
	CefRefPtr<CefV8Value> object = context->GetGlobal();	
	std::string url = frame->GetURL();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitHandler::OnDragStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask) 
{
	REQUIRE_UI_THREAD();
	// Forbid dragging of image files.
	if (dragData->IsFile()) {
		std::string fileName = dragData->GetFileName();
		if (fileName.find(".png") != std::string::npos ||
			fileName.find(".jpg") != std::string::npos ||
			fileName.find(".gif") != std::string::npos)
			return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitHandler::OnDragEnter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask) 
{
	REQUIRE_UI_THREAD();
	if (dragData->IsLink()) return true;
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitHandler::OnBeforeScriptExtensionLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& extensionName) 
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnRequestGeolocationPermission(CefRefPtr<CefBrowser> browser, const CefString& requesting_url, int request_id, CefRefPtr<CefGeolocationCallback> callback) 
{
	callback->Continue(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitHandler::OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, const CefString& url, CefRefPtr<CefClient>& client, CefBrowserSettings& settings)
{
	REQUIRE_UI_THREAD();
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefRequest> request, CefString& redirectUrl, CefRefPtr<CefStreamReader>& resourceStream, CefRefPtr<CefResponse> response, int loadFlags) 
{
	REQUIRE_IO_THREAD();
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) 
{
	REQUIRE_UI_THREAD();
	if (control->m_BrowserHwnd == browser->GetWindowHandle() && frame->IsMain()) {}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) 
{
	REQUIRE_UI_THREAD();
	CefWindowHandle hwnd = browser->GetWindowHandle();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::AddDOMVisitor(const std::string& path, CefRefPtr<CefDOMVisitor> visitor) 
{
	AutoLock lock_scope(this);
	DOMVisitorMap::iterator it = m_DOMVisitors.find(path);
	if (it == m_DOMVisitors.end())
		m_DOMVisitors.insert(std::make_pair(path, visitor));
	else
		it->second = visitor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CefRefPtr<CefDOMVisitor> WebKitHandler::GetDOMVisitor(const std::string& path) 
{
	AutoLock lock_scope(this);
	DOMVisitorMap::iterator it = m_DOMVisitors.find(path);
	if(it != m_DOMVisitors.end())
		return it->second;
	return NULL;
}
