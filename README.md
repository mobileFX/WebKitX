![](WebKitX.png)

WebKitX
=======

This is a Visual Studio 2010 SP1 MFC ActiveX Project that wraps Chromium Embedded Framework (CEF) version 1.1364.1123 for use with OLE/COM languages such as Visual Basic 6.0 SP1.

Background
----------

Purpose of this project was to create a light-weight CEF ActiveX wrapper for CEF3 but due to several problems with multithreading and VB6, I decided to wrap CEF1 first and then move to CEF3 slowly.

CEF is the acronym for Chromium Embedded Framework, which encapsulates WebKit HTML5 Renderer and Google V8 JavaScript engine. Currently there are two versions available, CEF1 (obsolete) and CEF3. 

The main architectural difference between CEF1 and CEF3 is that CEF3 is using using sub-processes (.exe files) mainly for rendering, and synchronizes all processes using IPC. This adds some process-management complexity wrapping it into an ActiveX and for that reason I decided to deal with CEF1 first.

CEF has two static libraries: libcef.lib and libcef_dll_wrapper.lib. Both static libraries must be compiled with /MD before you can successfully link them with an ActiveX DLL project. I have added some comments about it at the end of this doc.

The idea of wrapping CEF into an ActiveX is very simple:

* You create a MFC/ATL ActiveX Control class that will create the CefBrowser object. MFC ActiveX controls inherit from COleControl which in turn inherits from CWnd and therefore have a hWnd window handle. We wait until the first paint of the control and create the CefBrowser passing to it the hWnd handle of the control.

>> The class must inherit from COleControl and CefApp IN THAT ORDER - IT IS IMPORTANT!!

```C++
class CWebKitXCtrl : public COleControl, public CefApp
{
}
```

* We also create WebKitHandler, a simple CefClient descendant class that will be receiving callbacks from CefBrowser using callback definition classes such as CefLifeSpanHandler. There is a vast array of callback classes you can multiple-inherit WebKitHandler from, depending on the extend of control you need.

```C++
class WebKitHandler : public CefClient, public CefLifeSpanHandler
{
}
```

>> The trick for successfully wrapping CEF into an ActiveX is two use two classes, one for the actual ActiveX Control and one for the Browser handler. The reason is that CEF classes implement some short of reference counting mechanism and if you combine them it creates a reference counting mess.

The Problem
-----------
CEF1 implementation was very straight forward, but there was a problem related with CefShutdown() that if called on ActiveX Control Destructor it would permanently unload CEF Runtime without being able to re-initialize on the same process. 

As a result of this problem, the ActiveX would work perfectly only once and next time you would try to run it from the same process (eg. reload the Form that uses it) it wouldn't load CEF Runtime.

This behavior is related with CEF assuming that runs inside an .EXE and not inside an OLE Host. CefShutdown() must be called in order to terminate several background threads, that if not terminated crash the ActiveX when it unloads.

The Hack
--------

The hack I decided to do was to prevent the ActiveX .OCX file from unloading until the host process actually dies. To do so I return S_FALSE on DllCanUnloadNow().

```C++
	STDAPI DllCanUnloadNow(void)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		if (_AtlModule.GetLockCount() > 0)
			return S_FALSE;

		if(HOST_PROCESS_STILL_RUNNING)
			return S_FALSE;

		CefShutdown();

		return S_OK;
	}
```

Also, I initialize CEF only once using a static class variable:

```C++

bool CWebKitXCtrl::InitCEF()
{
	if(!CWebKitXCtrl::CEF_INITIALIZED)
	{
		CefSettings settings;
		settings.multi_threaded_message_loop			= true;
		settings.graphics_implementation				= ANGLE_IN_PROCESS; // H/W Accelerated Canvas & WebGL
		settings.pack_loading_disabled					= true;
		settings.auto_detect_proxy_settings_enabled		= true;
		settings.uncaught_exception_stack_size			= 10;			

		CefRefPtr<CefApp> app;
		CWebKitXCtrl::CEF_INITIALIZED = CefInitialize(settings, app);		
	}
	return CWebKitXCtrl::CEF_INITIALIZED;
};
```

Visual Basic 6 Sample
---------------------

![](cef_on_vb6.jpeg)

The Visual Basic sample project simply demonstrates how to use WebKitX API.

```VB6
Private Sub Form_Resize()
    On Error Resume Next
    WebKitX1.Move 0, 0, ScaleWidth, ScaleHeight
    Err.Clear
End Sub

Private Sub mnuEditable_Click()
    mnuEditable.Checked = Not mnuEditable.Checked
    If mnuEditable.Checked Then
        WebKitX1.Edit
    Else
        WebKitX1.Preview
    End If
End Sub

Private Sub mnuGet_Click()
    Debug.Print WebKitX1.HTML
End Sub

Private Sub mnuOpen_Click()
    WebKitX1.OpenURL "http://www.mobilefx.com"
End Sub

Private Sub mnuPut_Click()
    WebKitX1.HTML = "<html><body>Hello CEF</body></html>"
End Sub
```

ActiveX Interface
------------------
I have implemented a small robust API on the ActiveX as an example. The API functions open a URL, put/get HTML and enable/disable HTML5 editing. The API demonstrates all CEF-related bizzares and how to do things properly; Some CEF functions are asynchronous and must run of certain CEF threads before the ActiveX can get the results on its main thread, and pass it to VB6. It should be very easy to extend my code and add your own functions.

Future Work
-----------
I plan to integrate CEF3 but my goal is to create an HTML5 WYSIWYG Editor for my Coconut2D Studio IDE project, so I if CEF3 is a pain to integrate I won't bother too much.

DLLs and Libraries
------------------
I have included pre-built CEF1 and CEF3 libraries and their required DLLS that are needed only if you enable Hardware Accelerated Graphics (settings.graphics_implementation = ANGLE_IN_PROCESS). In theory if you choose a software graphics engine then the DLLs are not required.

Notes about compiling CEF1|3 for ActiveX
----------------------------------------
If you decide to compile CEF yourself you will need to do the following:
1. Disable all C++ Compiler Warnings and remove the Treat Warnings as Errors
2. Change in C++ Code Generation the Threading Model from /MT (for .exe) to /MD (for .DLL)

Contact me if you need any help.

Elias Politakis,
mobileFX CTO/Partner.

