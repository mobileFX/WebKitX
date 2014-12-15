
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
		debugPrint("Browser Created.\n");						   
		control->m_Browser = browser;
		control->m_BrowserHwnd = WINDOW_HANDLE(browser);		
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) 
{
	REQUIRE_UI_THREAD();
	if(control->m_BrowserHwnd == WINDOW_HANDLE(browser))
	{		
		control->DestroyCEFBrowser();
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,	CefRefPtr<CefFrame> frame)
{
	REQUIRE_UI_THREAD();
	if(control->m_BrowserHwnd == WINDOW_HANDLE(browser))
	{
		std::string s(frame->GetURL());		
		debugPrint("Loading %s ...\n", s.c_str()); 		
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	REQUIRE_UI_THREAD();
	if(control->m_BrowserHwnd == WINDOW_HANDLE(browser))
	{	
		std::string s(frame->GetURL());
		debugPrint("Loaded %s.\n", s.c_str());
		if(s!="about:blank")
		{			
			control->FireOnReady();
		}
		else
		{
			control->FireOnCreate();
		}
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& failedUrl, CefString& errorText)
{
	REQUIRE_UI_THREAD();
	if(control->m_BrowserHwnd == WINDOW_HANDLE(browser))
	{		
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser, KeyEventType type, int code, int modifiers, bool isSystemKey, bool isAfterJavaScript) 
{
	REQUIRE_UI_THREAD();
	if(type==KEYEVENT_RAWKEYDOWN) 
	{
		// Modified!
		control->SetModifiedFlag(TRUE);
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	control->v8context = context;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	control->v8context = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace)
{

}

