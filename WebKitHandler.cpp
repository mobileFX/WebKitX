
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

		// Hook the mouse
		//DWORD threadId = GetWindowThreadProcessId(browser->GetWindowHandle(), NULL);
		//hook = SetWindowsHookEx(WH_MOUSE, MouseHook, NULL, threadId);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WebKitHandler::MouseHook(int nCode, WPARAM wp, LPARAM lp)
{
	MOUSEHOOKSTRUCT *pmh = (MOUSEHOOKSTRUCT *) lp;

	if(nCode >= 0)
	{
		if(wp == WM_RBUTTONDOWN || wp == WM_RBUTTONUP) 
		{
			return 1;
		}
	}
	return CallNextHookEx(NULL, nCode, wp, lp);   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) 
{
	REQUIRE_UI_THREAD();
	if(control->m_BrowserHwnd == WINDOW_HANDLE(browser))
	{
		UnhookWindowsHookEx(hook);
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

