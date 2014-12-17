#include "StdAfx.h"
#include "WebKitV8Extension.h"
#include "WebKitXCtrl.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
void WebKitV8Extension::RegisterExtension(CWebKitXCtrl* control)
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

	control->v8handler = new WebKitV8Extension(control);

	CefRegisterExtension("v8/WebKitX", sink, control->v8handler);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
WebKitV8Extension::WebKitV8Extension(CWebKitXCtrl* control)
{
	this->control = control;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
WebKitV8Extension::~WebKitV8Extension(void)
{
	control = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitV8Extension::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) 
{
	std::string fname(name);

	debugPrint("V8 Native Function Call: %s\n", fname.c_str());

	if(fname == "__selectedNode") 
	{			
		return __selectedNode(object, arguments, retval, exception);
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool WebKitV8Extension::__selectedNode(CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	std::string uid(arguments[0]->GetStringValue());
	debugPrint("uid=%s\n", uid.c_str());
	return true;
}
