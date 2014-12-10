// WebKitXPropPage.cpp : Implementation of the CWebKitXPropPage property page class.

#include "stdafx.h"
#include "WebKitX.h"
#include "WebKitXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CWebKitXPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CWebKitXPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CWebKitXPropPage, "WEBKITX.WebKitXPropPage.1",
	0x7e8752c3, 0xad7e, 0x4a26, 0x8b, 0x6f, 0xa5, 0x45, 0x39, 0x45, 0x73, 0xe0)



// CWebKitXPropPage::CWebKitXPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CWebKitXPropPage

BOOL CWebKitXPropPage::CWebKitXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_WEBKITX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CWebKitXPropPage::CWebKitXPropPage - Constructor

CWebKitXPropPage::CWebKitXPropPage() :
	COlePropertyPage(IDD, IDS_WEBKITX_PPG_CAPTION)
{
}



// CWebKitXPropPage::DoDataExchange - Moves data between page and properties

void CWebKitXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CWebKitXPropPage message handlers
