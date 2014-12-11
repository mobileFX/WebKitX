#include "stdafx.h"
#include "WebKitX.h"
#include <initguid.h>
#include "WebKitX_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CWebKitXModule :
	public ATL::CAtlMfcModule
{
public:
	DECLARE_LIBID(LIBID_WebKitXLib);
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WEBKITX, "{BB160398-EA17-4281-B121-DB8EC7023AAF}");};

CWebKitXModule _AtlModule;

CWebKitXApp theApp;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const GUID CDECL _tlid = { 0x7493F856, 0x8B5B, 0x47C2, { 0x83, 0x36, 0x71, 0xD5, 0xCA, 0x84, 0x33, 0xD9 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CWebKitXApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();
	if (bInit)
	{		
	}
	return bInit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CWebKitXApp::ExitInstance()
{
	return COleControlModule::ExitInstance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STDAPI DllRegisterServer(void)
{
	_AtlModule.UpdateRegistryAppId(TRUE);
	HRESULT hRes2 = _AtlModule.RegisterServer(TRUE);
	if (hRes2 != S_OK)
		return hRes2;
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STDAPI DllUnregisterServer(void)
{
	_AtlModule.UpdateRegistryAppId(FALSE);
	HRESULT hRes2 = _AtlModule.UnregisterServer(TRUE);
	if (hRes2 != S_OK)
		return hRes2;
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}

// DllCanUnloadNow - Allows COM to unload DLL
#if !defined(_WIN32_WCE) && !defined(_AMD64_) && !defined(_IA64_)
#pragma comment(linker, "/EXPORT:DllCanUnloadNow=_DllCanUnloadNow@0,PRIVATE")
#pragma comment(linker, "/EXPORT:DllGetClassObject=_DllGetClassObject@12,PRIVATE")
#pragma comment(linker, "/EXPORT:DllRegisterServer=_DllRegisterServer@0,PRIVATE")
#pragma comment(linker, "/EXPORT:DllUnregisterServer=_DllUnregisterServer@0,PRIVATE")
#else
#if defined(_X86_) || defined(_SHX_)
#pragma comment(linker, "/EXPORT:DllCanUnloadNow=_DllCanUnloadNow,PRIVATE")
#pragma comment(linker, "/EXPORT:DllGetClassObject=_DllGetClassObject,PRIVATE")
#pragma comment(linker, "/EXPORT:DllRegisterServer=_DllRegisterServer,PRIVATE")
#pragma comment(linker, "/EXPORT:DllUnregisterServer=_DllUnregisterServer,PRIVATE")
#else
#pragma comment(linker, "/EXPORT:DllCanUnloadNow,PRIVATE")
#pragma comment(linker, "/EXPORT:DllGetClassObject,PRIVATE")
#pragma comment(linker, "/EXPORT:DllRegisterServer,PRIVATE")
#pragma comment(linker, "/EXPORT:DllUnregisterServer,PRIVATE")
#endif // (_X86_)||(_SHX_)
#endif // !_WIN32_WCE && !_AMD64_ && !_IA64_ 

	STDAPI DllCanUnloadNow(void)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		//Nasty Hack !!!
		return S_FALSE;

		if (_AtlModule.GetLockCount() > 0)
			return S_FALSE;

		//if(HOST_PROCESS_STILL_RUNNING)
		//	return S_FALSE;

		//CefShutdown();

		return S_OK;
	}

// DllGetClassObject - Returns class factory
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (S_OK == _AtlModule.GetClassObject(rclsid, riid, ppv))
		return S_OK;
	return AfxDllGetClassObject(rclsid, riid, ppv);
}
