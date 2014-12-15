#pragma once

#define CEF_VERSION_1

#include "include/cef_client.h"
#include "include/cef_nplugin.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef CEF_VERSION_1
#define WINDOW_HANDLE(B) B->GetWindowHandle()
#endif
#ifdef CEF_VERSION_3
#define WINDOW_HANDLE(B) B->GetHost()->GetWindowHandle()
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "libcef.lib")
#pragma comment(lib, "libcef_dll_wrapper.lib")

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define REQUIRE_UI_THREAD()   ASSERT(CefCurrentlyOn(TID_UI));
#define REQUIRE_IO_THREAD()   ASSERT(CefCurrentlyOn(TID_IO));
#define REQUIRE_FILE_THREAD() ASSERT(CefCurrentlyOn(TID_FILE));

template <class T, class A>
T join(const A &begin, const A &end, const T &t)
{
	T result;
	for (A it=begin;
		it!=end;
		it++)
	{
		if (!result.empty())
			result.append(t);
		result.append(*it);
	}
	return result;
}

class CWebKitXCtrl;

/////////////////////////////////////////////////////////////////////////////////////////////////////
class WebKitHandler :
	public CefClient,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefKeyboardHandler,
	public CefRenderHandler,
	public CefV8ContextHandler
{
public:

	CWebKitXCtrl* control;	

	WebKitHandler(CWebKitXCtrl* control);
	~WebKitHandler(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// CefClient methods
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {	return this; } 
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {	return this; } 
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefV8ContextHandler> GetV8ContextHandler() OVERRIDE { return this; }	

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// CefLifeSpanHandler callbacks
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;	
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// CefLoadHandler  callbacks
	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,	CefRefPtr<CefFrame> frame) OVERRIDE;
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;
	virtual bool OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& failedUrl, CefString& errorText) OVERRIDE;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// CefKeyboardHandler methods.
	virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser, KeyEventType type, int code, int modifiers, bool isSystemKey, bool isAfterJavaScript) OVERRIDE;	

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// TODO: CefRenderHandler (Off Screen Rendering)
	virtual bool GetViewRect(CefRefPtr<CefBrowser> browser,	CefRect& rect) OVERRIDE { return false; }
	virtual bool GetScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE { return false; }
	virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY) OVERRIDE { return false; }
	virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) OVERRIDE {}
	virtual void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) OVERRIDE {}
	virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer) OVERRIDE {}
	virtual void OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor) OVERRIDE {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// CefV8ContextHandler
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
	virtual void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
	virtual void OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace) OVERRIDE;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	IMPLEMENT_REFCOUNTING(WebKitHandler);
	IMPLEMENT_LOCKING(WebKitHandler);		
};
