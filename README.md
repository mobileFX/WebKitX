WebKitX
=======

This is a Visual Studio 2010 SP1 MFC ActiveX Project that wraps Chromium Embedded Framework (CEF) version 1.1364.1123 for use with OLE/COM languages such as Visual Basic 6.0 SP1.

Background
----------
Purpose of this project was to create a light-weight CEF ActiveX wrapper for CEF3 but due to several problems with multithreading and VB6, I decided to wrap CEF1 first and then move to CEF3 slowly.

Known Issues
------------
CEF1 implementation was very straight forward, but there is a problem related with CefShutdown() that if called on ActiveX Control destructor it permanently unloads CEF Runtime without being able to re-initialize on the same process. 

Thus the ActiveX works perfectly only once! :-( Next time you try to run it from the same process (eg. reload the Form that uses it) it won't load CEF Runtime.

This is related with CEF assuming that runs inside an .EXE and not inside an OLE Host and because CefShutdown() must be called in order to terminate several background threads, that if not terminated will crash the ActiveX when it unloads.

ActiveX Interface
------------------
I have implemented a small robust API on the ActiveX as an example. The API functions open a URL, put/get HTML and enable/disable HTML5 editing. The API demonstrates all CEF-related bizzares and how to do things properly since some CEF functions are asynchronous and must run of certain CEF threads before the ActiveX can get the results on its main thread and pass it to VB6. It should be very easy to extend my code and add your own functions.

Future Work
-----------
I plan to integrate CEF3 but my goal is to create an HTML5 WYSIWYG Editor for my Coconut2D Studio IDE project, so I if CEF3 is a pain to integrate I won't bother too much.

DLLs and Libraries
------------------
I have included pre-built CEF1 DLLS thare are required only if you enable Hardware Accelerated Graphics (settings.graphics_implementation = ANGLE_IN_PROCESS). In theory if you choose a software graphics engine then the DLLs are not required.

Notes about compiling CEF1|3 for ActiveX
----------------------------------------
If you decide to compile CEF yourself you will need to do the following:
1. Disable all C++ Compiler Warnings and remove the Treat Warnings as Errors
2. Change in C++ Code Generation the Threading Model from /MT (for .exe) to /MD (for .DLL)

Contact me if you need any help.

Elias Politakis,
mobileFX CTO/Partner.

