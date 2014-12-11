#include "stdafx.h"
#include "WebKitX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWebKitXApp theApp;

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
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
