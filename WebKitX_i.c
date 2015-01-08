

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Jan 08 20:06:46 2015
 */
/* Compiler settings for WebKitX.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_WebKitXLib,0x7493F856,0x8B5B,0x47C2,0x83,0x36,0x71,0xD5,0xCA,0x84,0x33,0xD9);


MIDL_DEFINE_GUID(IID, DIID__DWebKitX,0xECEC4ADA,0xDE49,0x4358,0xB7,0x08,0xC3,0x41,0x0F,0x8C,0xDD,0xDA);


MIDL_DEFINE_GUID(IID, DIID__DWebKitXEvents,0x3EE33618,0xBC27,0x40E9,0xB7,0x3D,0x87,0xA4,0x9D,0xEF,0x0C,0x8A);


MIDL_DEFINE_GUID(CLSID, CLSID_WebKitX,0xD9EA077F,0x4E5F,0x4BBD,0x9A,0xCD,0x02,0xE7,0x02,0x1C,0xDE,0xFE);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



