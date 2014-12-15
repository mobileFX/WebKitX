#pragma once

#include "include/cef_client.h"

class CWebKitXCtrl;

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
// ClientV8ExtensionHandler Class (Implementation of the V8 extension handler class)
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
class WebKitV8Extension : public CefV8Handler
{
public:	
	
	CWebKitXCtrl* control;

	WebKitV8Extension(CWebKitXCtrl* control);
	virtual ~WebKitV8Extension();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	static void RegisterExtension(CWebKitXCtrl* control)
	{
		// Register a V8 extension that calls native
		// methods implemented in WebKitV8Extension.

		std::string sink =  "var cef;"
							"if(!cef) cef = {};"
							"(function() {"
							"  cef.__defineSetter__('selectedNode', function(uid) {"
							"    native function __selectedNode();"
							"    __selectedNode(uid);"
							"  });"
							"})();";

		CefRegisterExtension("v8/WebKitX", sink, new WebKitV8Extension(control));
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE;
	bool __selectedNode(CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	IMPLEMENT_REFCOUNTING(WebKitV8Extension);
};
