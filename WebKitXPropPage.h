#pragma once

// WebKitXPropPage.h : Declaration of the CWebKitXPropPage property page class.


// CWebKitXPropPage : See WebKitXPropPage.cpp for implementation.

class CWebKitXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CWebKitXPropPage)
	DECLARE_OLECREATE_EX(CWebKitXPropPage)

// Constructor
public:
	CWebKitXPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_WEBKITX };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

