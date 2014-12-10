#include <windows.h>
#include "include/cef_app.h"

#pragma comment(lib, "libcef.lib")
#pragma comment(lib, "libcef_dll_wrapper_mt.lib")

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCEF3ProcessApp : public CefApp
{
private: 
	IMPLEMENT_REFCOUNTING(CCEF3ProcessApp);	

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static int init(HINSTANCE hInstance)
	{
		CefMainArgs main_args(hInstance);
		CefRefPtr<CCEF3ProcessApp> app(new CCEF3ProcessApp);
		return CefExecuteProcess(main_args, app.get(), NULL);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CCEF3ProcessApp()
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~CCEF3ProcessApp()
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) 
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	return CCEF3ProcessApp::init(hInstance);
}


/*

#include <windows.h>
#include <list>
#include <string>

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_sandbox_win.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_dom.h"
#include "include/cef_dom.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_message_router.h"

#pragma comment(lib, "libcef.lib")
#pragma comment(lib, "libcef_dll_wrapper_mt.lib")

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCEF3ProcessApp : 		
	public CefApp, 
	public CefBrowserProcessHandler,
	public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler
{
private: 
	IMPLEMENT_REFCOUNTING(CCEF3ProcessApp);
	CefRefPtr<CefMessageRouterRendererSide> message_router_;

public:

	typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
	BrowserList browser_list_;
	bool is_closing_;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CCEF3ProcessApp() : is_closing_(false)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~CCEF3ProcessApp()
	{
	}

	static int init(HINSTANCE hInstance)
	{
		void* sandbox_info = NULL;
		CefMainArgs main_args(hInstance);
		CefRefPtr<CCEF3ProcessApp> app(new CCEF3ProcessApp);
		int exit_code = CefExecuteProcess(main_args, app.get(), sandbox_info);
		if (exit_code >= 0) return exit_code;
		CefSettings settings;
		settings.no_sandbox = true;
		CefInitialize(main_args, settings, app.get(), sandbox_info);
		{
			CefRunMessageLoop();
		}	
		CefShutdown();
		return -1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CefApp methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() OVERRIDE
	{
		CEF_REQUIRE_UI_THREAD();
		CefWindowInfo window_info;
		window_info.SetAsPopup(NULL, "CEF3Process");
		//HWND hWnd = (HWND)1575992;
		//RECT rc;
		//GetClientRect(hWnd, &rc);
		//window_info.SetAsChild(hWnd, rc);
		CefRefPtr<CCEF3ProcessApp> handler(this);
		CefBrowserSettings browser_settings;
		CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();
		std::string url = command_line->GetSwitchValue("url");
		if (url.empty()) url = "http://www.google.com";
		CefBrowserHost::CreateBrowser(window_info, handler.get(), url, browser_settings, NULL);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OnAfterCreated(CefRefPtr<CefBrowser> browser) 
	{
		CEF_REQUIRE_UI_THREAD();
		browser_list_.push_back(browser);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool DoClose(CefRefPtr<CefBrowser> browser) 
	{
		CEF_REQUIRE_UI_THREAD();
		if (browser_list_.size() == 1) 
		{
			is_closing_ = true;
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OnBeforeClose(CefRefPtr<CefBrowser> browser) 
	{
		CEF_REQUIRE_UI_THREAD();
		BrowserList::iterator bit = browser_list_.begin();
		for (; bit != browser_list_.end(); ++bit) 
		{
			if ((*bit)->IsSame(browser)) 
			{
				browser_list_.erase(bit);
				break;
			}
		}

		if (browser_list_.empty()) 
		{
			CefQuitMessageLoop();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) 
	{
		CEF_REQUIRE_UI_THREAD();
		if (errorCode == ERR_ABORTED) return;
		std::stringstream ss;
		ss << "<html><body bgcolor=\"white\">"
			"<h2>Failed to load URL " << std::string(failedUrl) <<
			" with error " << std::string(errorText) << " (" << errorCode <<
			").</h2></body></html>";
		frame->LoadString(ss.str(), failedUrl);
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) 
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	return CCEF3ProcessApp::init(hInstance);
}
*/
